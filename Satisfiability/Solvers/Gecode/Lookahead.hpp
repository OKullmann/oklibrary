// Oleg Zaikin, 5.4.2021 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

    An implementation of look-ahead for the Gecode library.

 TODOS:

-1. Provide overview on functionality provided.
    - Also each function/class needs at least a short specification.

0. Proper names for variables.
    - According to Gecode, now 'failed leaves' and 'failed branches' are used.
    - One variant is to call them 'unsatisfiable leaves' and 'unsatisfiable
      branches' instead of it.
    - Similarly for 'leaves with solutions' and 'branches with solutions':
      'satisfiable leaves' and 'satisfiable branches'.

1. DONE (variables and values were renamed)
   Proper naming of variables and values
    - DONE ('pos' was replaced by 'var' that reflects the Gecode variable)
      "pos" etc. says nothing about the meaning.
    - DONE (shorter names (binmin, narymin, and naryla) are now used.
      The names of the enumeration Braching0 are far too long (and
      appear "random").

2. Statistics are urgently needed.
    - Basic statistics (number of nodes, inner nodes, failed leaves,
      and solutions) is now calculated if look-ahead branching is used.
    - More statistics will be added soon.

3. Four levels of LA-reduction:
    - Level 0 : (if "DONE", then how?? documentation is needed)
     - no explicit reduction;
     - for every branching unsatisfiable branches are just removed;
     - if a branching of width 0 is created, the problem is (immediately)
       recognised as unsatisfiable;

       In NaryLookahead::choice(Space&), there one finds "report that the
       current branch is failed": likely this refers to the *branching*?
       And what is "the first failed value"? There should be *no* value here.
     - if a branching of width 1 is created, then this branching, as a single-
       child-branching, is immediately excecuted.
    - Level 1 :
     - still no explicit reduction;
     - additionally to level 1, if in a considered branching a branch is found
       unsatisfiable, then the corresponding restriction is applied to the
       current instance (globally).
    - Level 2 :
     - now there is an explicit reduction, which applies the reductions found
       by falsified branches until a fixed-point is reached;
     - then in the separate branching function no checks for unsatisfiability
       etc. are done (since no such case can occur).
    - Level 3 :
     - additionally to level 2, now in a considered branching also the
       intersection of the branches is considered for a common reduction.

4. DONE (Yes, our policy is now passing Gecode::IntVarArray by copy)
   Is it appropriate to pass Gecode::IntVarArray by copy?
   ??? What is the solution?

  - Copying would only be appropriate if the internal data stored
    is just a pointer, plus a bit more data.
  - The base class Gecode::VarArray
      https://www.gecode.org/doc-latest/reference/kernel_2data_2array_8hpp_source.html
    seems to contain exactly two data-members:
      int n;
      Var* x;
  - Thus possibly copying is safe and efficient.

5. DONE (Early abortion of a problem is not possible in choice(GC::Space& home))
   Call of status() likely needs a check for early abortion.
   ??? When "done", then how??
   - In case of early abortion of a problem, the function choice(GC::Space& home)
     is not called by Gecode's search engine, where home is the current problem.
   - This is checked by asserting that the problem status is SS_BRANCH.

6. DONE (Enumerations BrTypeO and BrSourceO are parsed via Environment)
   Use Environment to parse enumeration from argv.

7. Generate examples for which tree sizes when using look-ahead are known.
  - It will allow checking correctness of the look-ahead implementation.
  - By now correctness is checked by hand on several small examples:
      Trivial::Sum; Send-more-money; Send-most-money.

8. Later: we don't want to handle variables, but branchers.
  - We can't restrict to just branching on values.
  - We want access to the given possibilities for branching.

9. Later: general concept of a distance.
  - Now distance is a delta of measures.
  - A general concept of a distance should be properly supported.
  - A distance can be handled as a function of two arguments dist(F,F').

*/

#ifndef LOOKAHEAD_lNFKYYpHQ8
#define LOOKAHEAD_lNFKYYpHQ8

#include <iostream>
#include <limits>
#include <vector>
#include <memory>
#include <functional>

#include <cmath>
#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/gist.hh>

#include <Numerics/FloatingPoint.hpp>
#include <Numerics/Tau.hpp>

namespace Lookahead {

  namespace FP = FloatingPoint;
  namespace GC = Gecode;

  typedef unsigned size_t;
  typedef FP::float80 float_t;
  typedef std::uint64_t count_t;
  typedef GC::Int::IntView IntView;
  typedef GC::ViewArray<IntView> IntViewArray;
  typedef GC::IntVarValues IntVarValues;
  typedef std::vector<int> values_t;
  typedef std::vector<bool> eq_values_t;
  typedef std::vector<float_t> tuple_t;
  typedef std::function<float_t(const GC::IntVarArray)> measure_t;

  enum class BrTypeO {mind=0, la=1};
  enum class BrSourceO {eq=0, v=1, eqv=2};
  enum class BrMeasureO {mu0=0, mu1=1};
  enum class BrSolutionO {one=0, all=1};

  constexpr char sep = ',';
  typedef std::tuple<BrTypeO, BrSourceO, BrMeasureO, BrSolutionO> option_t;

  std::ostream& operator <<(std::ostream& out, const BrTypeO brt) {
    switch (brt) {
    case BrTypeO::la : return out << "best-look-ahead-variable";
    default : return out << "minimal-domain-size-variable";}
  }
  std::ostream& operator <<(std::ostream& out, const BrSourceO brsrs) {
    switch (brsrs) {
    case BrSourceO::v : return out << "value-only";
    case BrSourceO::eqv : return out << "equality+value";
    default : return out << "equality-only";}
  }
  std::ostream& operator <<(std::ostream& out, const BrMeasureO brm) {
    switch (brm) {
    case BrMeasureO::mu0 : return out << "sum-measure";
    default : return out << "product-measure";}
  }
  std::ostream& operator <<(std::ostream& out, const BrSolutionO brsln) {
    switch (brsln) {
    case BrSolutionO::all : return out << "find-all-solutions";
    default : return out << "find-one-solution";}
  }

  inline bool show_usage(const Environment::ProgramInfo proginfo,
                         const int argc, const char* const argv[],
                         const bool gist = true) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [branching-options] [visual]\n\n" <<
    " branching-options : " << Environment::WRP<BrTypeO>{} << "\n"
    "                   : " << Environment::WRP<BrSourceO>{} << "\n"
    "                   : " << Environment::WRP<BrMeasureO>{} << "\n"
    "                   : " << Environment::WRP<BrSolutionO>{} << "\n";
    if (gist) std::cout <<
    " visual            : \"gist\" (run Gist to visualise the search tree).\n\n";
    std::cout <<
    " solves a given CP-problem via Gecode solvers and given branching options.\n";
    return true;
  }

struct SearchStat {
    count_t nodes;
    count_t inner_nodes;
    count_t failed_leaves;
    count_t solutions;
    GC::Search::Statistics engine;
    option_t br_options;

    SearchStat() : nodes(0), inner_nodes(0), failed_leaves(0),
                   solutions(0), engine(), br_options() {}

    bool valid() const noexcept {
      return (failed_leaves + solutions + inner_nodes == nodes);
    }

    void reset() noexcept {
      assert(valid());
      nodes = inner_nodes = failed_leaves = solutions = 0;
    }

    void update_nodes() noexcept {
      const BrTypeO brt = std::get<BrTypeO>(br_options);
      if (brt != BrTypeO::la and failed_leaves < engine.fail) {
        failed_leaves += engine.fail;
      }
      nodes = inner_nodes + failed_leaves + solutions;
      assert(valid());
    }

    friend bool operator ==(const SearchStat& lhs, const SearchStat& rhs) noexcept {
      return lhs.nodes == rhs.nodes and lhs.inner_nodes == rhs.inner_nodes and
             lhs.failed_leaves == rhs.failed_leaves and lhs.solutions == rhs.solutions;
    }

    void print() const noexcept {
      assert(valid());
      const BrTypeO brt = std::get<BrTypeO>(br_options);
      const BrSourceO brsrc = std::get<BrSourceO>(br_options);
      const BrMeasureO brm = std::get<BrMeasureO>(br_options);
      const BrSolutionO brsln = std::get<BrSolutionO>(br_options);

      using std::setw;
      const auto w = setw(10);
      std::cout << nodes << w << inner_nodes << w << failed_leaves << w << solutions
                << w << int(brt) << w << int(brsrc) << w << int(brm) << w
                << int(brsln) << "\n";
    }
  };

  SearchStat global_stat;

  inline constexpr size_t tr(const int size, [[maybe_unused]] const size_t bound = 0) noexcept {
    assert(bound <= std::numeric_limits<int>::max());
    assert(size >= int(bound));
    return size;
  }


  inline float_t mu0(const GC::IntVarArray& V) noexcept {
    float_t s = 0;
    for (const auto& v : V) {
      const auto is = tr(v.size(), 1);
      s += is - 1;
    }
    return s;
  }

  inline float_t mu1(const GC::IntVarArray& V) noexcept {
    float_t s = 0;
    for (const auto& v : V) {
      const auto is = tr(v.size(), 1);
      s += FloatingPoint::log2(is);
    }
    return s;
  }

  template<class ModSpace>
  std::shared_ptr<ModSpace> subproblem(ModSpace* const m, const int v, const int val,
                                       const bool eq = true) noexcept {
    assert(m->valid());
    assert(m->valid(v));
    assert(m->status() == GC::SS_BRANCH);
    // Clone space:
    std::shared_ptr<ModSpace> c(static_cast<ModSpace*>(m->clone()));
    assert(c->valid());
    assert(c->valid(v));
    assert(c->status() == GC::SS_BRANCH);
    // Add an equality constraint for the given variable and its value:
    if (eq) GC::rel(*(c.get()), (c.get())->at(v), GC::IRT_EQ, val);
    else GC::rel(*(c.get()), (c.get())->at(v), GC::IRT_NQ, val);
    return c;
  }

  enum class BrStatus { failed=0, solved=1, branch=2 };

  struct BrData {
    BrStatus status;
    int var;
    values_t values;
    eq_values_t eq_values;
    tuple_t v_tuple;
    tuple_t eq_tuple;
    float_t ltau;

    bool valid() const noexcept {
      return var >= 0 and eq_values.size() <= 2 and ltau >= 0 and
      (eq_values.empty() or eq_values.size() == 1 or eq_values[0] != eq_values[1]) and
      v_tuple.size() <= values.size() and eq_tuple.size() <= eq_values.size() and
      ((status == BrStatus::failed and values.empty() and eq_values.empty()) or
       (status != BrStatus::failed and values.size() == 1 and not eq_values.empty()) or
       (status != BrStatus::failed and not values.empty() and eq_values.empty()));
    }

    bool operator <(const BrData& a) const noexcept { return (ltau < a.ltau); }

    void print() const noexcept {
      std::cout << static_cast<int>(status) << " " << var << " {";
      for (auto& x : values) std::cout << x << ",";
      std::cout << "} {";
      for (auto x : eq_values) std::cout << (int)x << ",";
      std::cout << "} {";
      for (auto& x : v_tuple) std::cout << (int)x << ",";
      std::cout << "} {";
      for (auto& x : eq_tuple) std::cout << (int)x << ",";
      std::cout << "} " << ltau << std::endl;
    }

    bool update_v() noexcept {
      assert(status != BrStatus::failed);
      bool brk = false;
      // If branching of width 1, immediately execute:
      if (v_tuple.size() == 1) {
        assert(status != BrStatus::failed);
        assert(not values.empty());
        brk = true;
      }
      // If branching of width 0, the problem is unsat, immediately execute:
      else if (v_tuple.empty() and values.empty()) {
        assert(status != BrStatus::solved);
        status = BrStatus::failed;
        brk = true;
      }
      // If all subproblems are satisfiable, immediately execute:
      else if (v_tuple.empty() and not values.empty()) {
        assert(status == BrStatus::solved);
        brk = true;
      }
      else {
        assert(v_tuple.size() > 1);
        ltau = Tau::ltau(v_tuple);
      }
      return brk;
    }

    bool update_eq() noexcept {
      assert(status != BrStatus::failed);
      bool brk = false;
      if (eq_tuple.size() == 1) {
        assert(status != BrStatus::failed);
        brk = true;
      }
      else if (eq_tuple.empty() and eq_values.empty()) {
        assert(status != BrStatus::solved);
        values = {};
        status = BrStatus::failed;
        brk = true;
      }
      else if (eq_tuple.empty() and not eq_values.empty()) {
        assert(status == BrStatus::solved);
        brk = true;
      }
      else {
        assert(not eq_tuple.empty());
        ltau = Tau::ltau(eq_tuple);
      }
      return brk;
    }

    size_t branches_num() const noexcept {
      assert(valid());
      if (eq_values.empty()) return values.empty() ? 1 : values.size();
      else return eq_values.size();
    }

    BrData(const BrStatus st=BrStatus::failed, const int v=0, const values_t vls={},
           const eq_values_t eq_vls={}, const tuple_t v_tpl={}, const tuple_t eq_tpl={})
      : status(st), var(v), values(vls), eq_values(eq_vls), v_tuple(v_tpl),
      eq_tuple(eq_tpl), ltau(FP::pinfinity) {
    }
  };

  template <class CustomisedBrancher>
  struct Branching : public GC::Choice {
    BrData brdata;

    bool valid() const noexcept { return brdata.valid(); }

    Branching(const CustomisedBrancher& b, const BrData& brd = BrData())
      : GC::Choice(b, brd.branches_num()), brdata(brd) {
      assert(valid());
    }

  };

  // A customised brancher. Branchings are formed by assigning all possible
  // values to all unassigned variables. A branching with minimal domain
  // size is chosen as the best branching.
  class MinDomValue : public GC::Brancher {
    IntViewArray x;
    mutable int start;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    bool valid() const noexcept { return valid(start, x); }

    MinDomValue(const GC::Home home, const IntViewArray& x)
      : GC::Brancher(home), x(x), start(0) { assert(valid(start, x)); }
    MinDomValue(GC::Space& home, MinDomValue& b)
      : GC::Brancher(home,b), start(b.start) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x) {
      new (home) MinDomValue(home, x);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) MinDomValue(home, *this);
    }
    virtual bool status(const GC::Space&) const {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space&) {
      assert(valid(start, x));
      int var = start;
      size_t width = tr(x[var].size());
      assert(width > 0);
      for (int i = start + 1; i < x.size(); ++i)
        if (not x[i].assigned() and x[i].size() < width) {
          var = i; width = tr(x[var].size());
          assert(width > 0);
        }
      assert(var >= start and var >= 0 and not x[var].assigned());
      values_t values;
      for (GC::Int::ViewValues i(x[var]); i(); ++i)
        values.push_back(i.val());
      assert(not values.empty());
      ++global_stat.inner_nodes;
      BrData brd(BrStatus::branch, var, values);
      assert(brd.valid());
      return new Branching<MinDomValue>(*this, brd);
    }
    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new Branching<MinDomValue>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef Branching<MinDomValue> Branching;
      const Branching& br = static_cast<const Branching&>(c);
      const BrData& brd = br.brdata;
      assert(br.valid() and brd.valid());
      const auto& values = brd.values;
      const auto var = brd.var;
      assert(brd.status == BrStatus::branch);
      assert(var >= 0 and not values.empty());
      assert(branch < values.size());
      // Failed leaf:
      if (GC::me_failed(x[var].eq(home, values[branch]))) {
        ++global_stat.failed_leaves;
        return GC::ES_FAILED;
      }
      // Execute branching:
      return GC::ES_OK;
    }

  };

  // A customised brancher. For a variable var and its value val, branching is
  // formed by two branches: var==val and var!=val. The best branching
  // corresponds to a variable with minimal domain size and the minimal
  // variable value.
  class MinDomMinValEq : public GC::Brancher {
    IntViewArray x;
    mutable int start;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    bool valid() const noexcept { return valid(start, x); }

    MinDomMinValEq(const GC::Home home, const IntViewArray& x)
      : GC::Brancher(home), x(x), start(0) { assert(valid(start, x)); }
    MinDomMinValEq(GC::Space& home, MinDomMinValEq& b)
      : GC::Brancher(home,b), start(b.start) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x) {
      new (home) MinDomMinValEq(home, x);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) MinDomMinValEq(home, *this);
    }
    virtual bool status(const GC::Space&) const {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space&) {
      assert(valid(start, x));
      int var = start;
      size_t width = tr(x[var].size());
      assert(width > 0);
      for (int i = start + 1; i < x.size(); ++i)
        if (not x[i].assigned() and x[i].size() < width) {
          var = i; width = tr(x[var].size());
          assert(width > 0);
        }
      assert(var >= start and var >= 0 and not x[var].assigned());
      values_t values{x[var].min()};
      assert(values.size() == 1);
      eq_values_t eq_values = {true, false};
      ++global_stat.inner_nodes;
      BrData brd(BrStatus::branch, var, values, eq_values);
      assert(brd.valid());
      return new Branching<MinDomMinValEq>(*this, brd);
    }
    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new Branching<MinDomMinValEq>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef Branching<MinDomMinValEq> Branching;
      const Branching& br = static_cast<const Branching&>(c);
      const BrData& brd = br.brdata;
      assert(br.valid() and brd.valid());
      assert(brd.status == BrStatus::branch);
      assert(brd.values.size() == 1);
      const auto var = brd.var;
      const auto val = brd.values[0];
      const auto& eq_values = brd.eq_values;
      assert(var >= 0);
      assert(eq_values.size() == 2);
      assert(branch == 0 or branch == 1);
      if ( (eq_values[branch] == true and GC::me_failed(x[var].eq(home, val))) or
           (eq_values[branch] == false and GC::me_failed(x[var].nq(home, val))) ) {
        ++global_stat.failed_leaves;
        return GC::ES_FAILED;
      }
      return GC::ES_OK;
    }

  };


  // A customised LA-based brancher for finding all solutions. Branchings are
  // formed by assigning all possible values to all unassigned variables. The
  // best branching is chosen via the tau-function.
  template <class ModSpace>
  class LookaheadValueAllSln : public GC::Brancher {
    IntViewArray x;
    mutable int start;
    measure_t measure;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    bool valid() const noexcept { return valid(start, x); }

    LookaheadValueAllSln(const GC::Home home, const IntViewArray& x,
      const measure_t measure) : GC::Brancher(home), x(x), start(0), measure(measure) {
    assert(valid(start, x)); }
    LookaheadValueAllSln(GC::Space& home, LookaheadValueAllSln& b)
      : GC::Brancher(home,b), start(b.start), measure(b.measure) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x, const measure_t measure) {
      new (home) LookaheadValueAllSln(home, x, measure);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) LookaheadValueAllSln(home, *this);
    }
    virtual bool status(const GC::Space&) const {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space& home) {
      assert(valid(start, x));
      assert(start < x.size());
      BrData best_brd;

      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);

      const auto msr = measure(m->at());

      // For remaining variables (all before 'start' are assigned):
      for (int v = start; v < x.size(); ++v) {
        // v is a variable, view is the values in Gecode format:
        const IntView view = x[v];
        // Skip assigned variables:
        if (view.assigned()) continue;
        assert(view.size() >= 2);
        tuple_t v_tuple; values_t vls;
        BrStatus status = BrStatus::branch;
        // For all values of the current variable:
        for (IntVarValues j(view); j(); ++j) {
          // Assign value, propagate, and measure:
          const int val = j.val();
          auto subm = subproblem<ModSpace>(m, v, val, true);
          auto subm_st = subm->status();
          // Skip failed branches:
          if (subm_st != GC::SS_FAILED) {
            // Calculate delta of measures:
            float_t dlt = msr - measure(subm->at());
            assert(dlt > 0);
            vls.push_back(val);
            if (subm_st == GC::SS_SOLVED) status = BrStatus::solved;
            else v_tuple.push_back(dlt);
          }
        }
        BrData brd(status, v, vls, {}, v_tuple);
        assert(brd.valid());
        bool brk = brd.update_v();
        if (brk) { best_brd = brd; break; }
        // Compare branchings by the ltau value:
        best_brd = (brd < best_brd) ? brd : best_brd;
      }
      if (best_brd.status != BrStatus::failed) ++global_stat.inner_nodes;
      [[maybe_unused]] const auto var = best_brd.var;
      assert(var >= 0 and var >= start and not x[var].assigned());
      assert(best_brd.valid());
      return new Branching<LookaheadValueAllSln>(*this, best_brd);
    }

    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new Branching<LookaheadValueAllSln>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef Branching<LookaheadValueAllSln> Branching;
      const Branching& br = static_cast<const Branching&>(c);
      const BrData& brd = br.brdata;
      assert(br.valid() and brd.valid());
      const auto status = brd.status;
      const auto var = brd.var;
      const auto& values = brd.values;
      assert(status == BrStatus::failed or branch < values.size());
      // If failed branching, stop executing:
      if (status == BrStatus::failed or
          GC::me_failed(x[var].eq(home, values[branch]))) {
        ++global_stat.failed_leaves;
        return GC::ES_FAILED;
      }
      // Execute branching:
      return GC::ES_OK;
    }

  };

  // A customised LA-based brancher for finding one solution. Branchings are formed
  // by assigning all possible values to all unassigned variables. The best
  // branching is chosen via the tau-function.
  template <class ModSpace>
  class LookaheadValueOneSln : public GC::Brancher {
    IntViewArray x;
    mutable int start;
    measure_t measure;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    bool valid() const noexcept { return valid(start, x); }

    LookaheadValueOneSln(const GC::Home home, const IntViewArray& x,
      const measure_t measure) : GC::Brancher(home), x(x), start(0), measure(measure) {
    assert(valid(start, x)); }
    LookaheadValueOneSln(GC::Space& home, LookaheadValueOneSln& b)
      : GC::Brancher(home,b), start(b.start), measure(b.measure) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x, const measure_t measure) {
      new (home) LookaheadValueOneSln(home, x, measure);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) LookaheadValueOneSln(home, *this);
    }
    virtual bool status(const GC::Space&) const {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space& home) {
      assert(valid(start, x));
      assert(start < x.size());
      BrData best_brd;

      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);

      const auto msr = measure(m->at());

      // For remaining variables (all before 'start' are assigned):
      for (int v = start; v < x.size(); ++v) {
        // v is a variable, view is the values in Gecode format:
        const IntView view = x[v];
        // Skip assigned variables:
        if (view.assigned()) continue;
        assert(view.size() >= 2);
        tuple_t v_tuple; values_t vls;
        BrStatus status = BrStatus::branch;
        // For all values of the current variable:
        for (IntVarValues j(view); j(); ++j) {
          // Assign value, propagate, and measure:
          const int val = j.val();
          auto subm = subproblem<ModSpace>(m, v, val);
          auto subm_st = subm->status();
          // Stop ff a solution is found:
          if (subm_st == GC::SS_SOLVED) {
            v_tuple.clear(); vls = {val};
            status = BrStatus::solved;
            break;
          }
          else if (subm_st == GC::SS_BRANCH) {
            // Calculate delta of measures:
            float_t dlt = msr - measure(subm->at());
            assert(dlt > 0);
            vls.push_back(val); v_tuple.push_back(dlt);
          }
        }
        BrData brd(status, v, vls, {}, v_tuple);
        assert(brd.valid());
        bool brk = (status == BrStatus::solved) or brd.update_v();
        if (brk) { best_brd = brd; break; }
        best_brd = (brd < best_brd) ? brd : best_brd;
      }
      if (best_brd.status != BrStatus::failed) ++global_stat.inner_nodes;
      [[maybe_unused]] const auto var = best_brd.var;
      assert(var >= 0 and var >= start and not x[var].assigned());
      assert(best_brd.valid());
      return new Branching<LookaheadValueOneSln>(*this, best_brd);
    }

    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new Branching<LookaheadValueOneSln>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef Branching<LookaheadValueOneSln> Branching;
      const Branching& br = static_cast<const Branching&>(c);
      const BrData brd = br.brdata;
      assert(br.valid() and brd.valid());
      const auto status = brd.status;
      const auto var = brd.var;
      const auto& values = brd.values;
      assert(status == BrStatus::failed or branch < values.size());
      // If failed branching, stop executing:
      if (status == BrStatus::failed or
          GC::me_failed(x[var].eq(home, values[branch]))) {
        ++global_stat.failed_leaves;
        return GC::ES_FAILED;
      }
      // Execute branching:
      return GC::ES_OK;
    }

  };

  // A customised LA-based brancher for finding all solutions. For a variable var
  // and its value val, branching is formed by two branches: var==val and
  // var!=val. The best branching is chosen via the tau-function.
  template <class ModSpace>
  class LookaheadEqAllSln : public GC::Brancher {
    IntViewArray x;
    mutable int start;
    measure_t measure;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    bool valid() const noexcept { return valid(start, x); }

    LookaheadEqAllSln(const GC::Home home, const IntViewArray& x,
      const measure_t measure) : GC::Brancher(home), x(x), start(0), measure(measure) {
    assert(valid(start, x)); }
    LookaheadEqAllSln(GC::Space& home, LookaheadEqAllSln& b)
      : GC::Brancher(home,b), start(b.start), measure(b.measure) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x, const measure_t measure) {
      new (home) LookaheadEqAllSln(home, x, measure);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) LookaheadEqAllSln(home, *this);
    }
    virtual bool status(const GC::Space&) const {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space& home) {
      assert(valid(start, x));
      assert(start < x.size());
      BrData best_brd;

      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);

      const auto msr = measure(m->at());

      for (int v = start; v < x.size(); ++v) {
        const IntView view = x[v];
        if (view.assigned()) continue;
        assert(view.size() >= 2);
        bool brk = false;
        for (IntVarValues j(view); j(); ++j) {
          const int val = j.val();
          tuple_t eq_tuple; eq_values_t eq_vls;
          BrStatus status = BrStatus::branch;
          // variable == value:
          auto subm_eq = subproblem<ModSpace>(m, v, val, true);
          auto subm_eq_st = subm_eq->status();
          if (subm_eq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_eq->at());
            assert(dlt > 0);
            eq_vls.push_back(true);
            if (subm_eq_st == GC::SS_SOLVED) status = BrStatus::solved;
            else eq_tuple.push_back(dlt);
          }
          // variable != value:
          auto subm_neq = subproblem<ModSpace>(m, v, val, false);
          auto subm_neq_st = subm_neq->status();
          if (subm_neq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_neq->at());
            assert(dlt > 0);
            eq_vls.push_back(false);
            if (subm_neq_st == GC::SS_SOLVED) status = BrStatus::solved;
            else eq_tuple.push_back(dlt);
          }
          BrData brd(status, v, {val}, eq_vls, {}, eq_tuple);
          assert(brd.valid());
          brk = brd.update_eq();
          if (brk) { best_brd = brd; break; }
          // Compare branchings by ltau value:
          best_brd = (brd < best_brd) ? brd : best_brd;
        }
        if (brk) break;
      }
      if (best_brd.status != BrStatus::failed) ++global_stat.inner_nodes;
      [[maybe_unused]] const auto var = best_brd.var;
      assert(var >= 0 and var >= start and not x[var].assigned());
      assert(best_brd.valid());
      return new Branching<LookaheadEqAllSln>(*this, best_brd);
    }

    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new Branching<LookaheadEqAllSln>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef Branching<LookaheadEqAllSln> Branching;
      const Branching& br = static_cast<const Branching&>(c);
      const BrData& brd = br.brdata;
      assert(br.valid() and brd.valid());
      const auto status = brd.status;
      if (status == BrStatus::failed) {
        ++global_stat.failed_leaves; return GC::ES_FAILED;
      }
      const auto var = brd.var;
      const auto& values = brd.values;
      const auto& eq_values = brd.eq_values;
      assert(var >= 0 and values.size() == 1);
      assert(eq_values.size() == 1 or eq_values.size() == 2);
      assert(branch == 0 or branch == 1);
      assert(branch < eq_values.size());
      const auto val = values[0];
      if ( (eq_values[branch] == true and GC::me_failed(x[var].eq(home, val))) or
           (eq_values[branch] == false and GC::me_failed(x[var].nq(home, val))) ) {
        ++global_stat.failed_leaves; return GC::ES_FAILED;
      }
      return GC::ES_OK;
    }

  };

  // A customised LA-based brancher for finding one solution. For a variable var
  // and its value val, branching is formed by two branches: var==val and
  // var!=val. The best branching is chosen via the tau-function.
  template <class ModSpace>
  class LookaheadEqOneSln : public GC::Brancher {
    IntViewArray x;
    mutable int start;
    measure_t measure;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    bool valid() const noexcept { return valid(start, x); }

    LookaheadEqOneSln(const GC::Home home, const IntViewArray& x,
      const measure_t measure) : GC::Brancher(home), x(x), start(0), measure(measure) {
    assert(valid(start, x)); }
    LookaheadEqOneSln(GC::Space& home, LookaheadEqOneSln& b)
      : GC::Brancher(home,b), start(b.start), measure(b.measure) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x, const measure_t measure) {
      new (home) LookaheadEqOneSln(home, x, measure);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) LookaheadEqOneSln(home, *this);
    }
    virtual bool status(const GC::Space&) const {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space& home) {
      assert(valid(start, x));
      assert(start < x.size());
      BrData best_brd;

      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);

      const auto msr = measure(m->at());

      for (int v = start; v < x.size(); ++v) {
        const IntView view = x[v];
        if (view.assigned()) continue;
        assert(view.size() >= 2);
        bool brk = false;
        for (IntVarValues j(view); j(); ++j) {
          const int val = j.val();
          BrStatus status = BrStatus::branch;
          tuple_t eq_tuple; eq_values_t eq_vls;
          // variable == value:
          auto subm_eq = subproblem<ModSpace>(m, v, val, true);
          auto subm_eq_st = subm_eq->status();
          if (subm_eq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_eq->at());
            assert(dlt > 0);
            eq_vls.push_back(true);
            if (subm_eq_st == GC::SS_SOLVED) {
              status = BrStatus::solved;
              eq_tuple.clear(); eq_vls = {true};
            }
            else eq_tuple.push_back(dlt);
          }
          // variable != value:
          auto subm_neq = subproblem<ModSpace>(m, v, val, false);
          auto subm_neq_st = subm_neq->status();
          if (subm_neq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_neq->at());
            assert(dlt > 0);
            eq_vls.push_back(false);
            if (subm_neq_st == GC::SS_SOLVED) {
              status = BrStatus::solved;
              eq_tuple.clear(); eq_vls = {false};
            }
            else eq_tuple.push_back(dlt);
          }
          BrData brd(status, v, {val}, eq_vls, {}, eq_tuple);
          assert(brd.valid());
          brk = (status == BrStatus::solved) or brd.update_eq();
          if (brk) { best_brd = brd; break; }
          best_brd = (brd < best_brd) ? brd : best_brd;
        }
        if (brk) break;
      }
      if (best_brd.status != BrStatus::failed) ++global_stat.inner_nodes;
      [[maybe_unused]] const auto var = best_brd.var;
      assert(var >= 0 and var >= start and not x[var].assigned());
      assert(best_brd.valid());
      return new Branching<LookaheadEqOneSln>(*this, best_brd);
    }

    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new Branching<LookaheadEqOneSln>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef Branching<LookaheadEqOneSln> Branching;
      const Branching& br = static_cast<const Branching&>(c);
      const BrData& brd = br.brdata;
      assert(br.valid() and brd.valid());
      const auto status = brd.status;
      if (status == BrStatus::failed) {
        ++global_stat.failed_leaves; return GC::ES_FAILED;
      }
      const auto var = brd.var;
      const auto& values = brd.values;
      const auto& eq_values = brd.eq_values;
      assert(var >= 0 and values.size() == 1);
      assert(eq_values.size() == 1 or eq_values.size() == 2);
      assert(branch == 0 or branch == 1);
      assert(branch < eq_values.size());
      const auto val = values[0];
      if ( (eq_values[branch] == true and GC::me_failed(x[var].eq(home, val))) or
           (eq_values[branch] == false and GC::me_failed(x[var].nq(home, val))) ) {
        ++global_stat.failed_leaves; return GC::ES_FAILED;
      }
      return GC::ES_OK;
    }

  };

  // A customised LA-based brancher for finding all solutions. For a variable var,
  // branching is formed by eq-branches and val-branches.
  template <class ModSpace>
  class LookaheadEqValAllSln : public GC::Brancher {
    IntViewArray x;
    mutable int start;
    measure_t measure;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    bool valid() const noexcept { return valid(start, x); }

    LookaheadEqValAllSln(const GC::Home home, const IntViewArray& x,
      const measure_t measure) : GC::Brancher(home), x(x), start(0), measure(measure) {
    assert(valid(start, x)); }
    LookaheadEqValAllSln(GC::Space& home, LookaheadEqValAllSln& b)
      : GC::Brancher(home,b), start(b.start), measure(b.measure) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x, const measure_t measure) {
      new (home) LookaheadEqValAllSln(home, x, measure);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) LookaheadEqValAllSln(home, *this);
    }
    virtual bool status(const GC::Space&) const {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space& home) {
      assert(valid(start, x));
      assert(start < x.size());

      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);

      BrData best_brd;
      const auto msr = measure(m->at());

      for (int v = start; v < x.size(); ++v) {
        const IntView view = x[v];
        if (view.assigned()) continue;
        assert(view.size() >= 2);
        bool brk = false;
        tuple_t v_tuple;
        values_t vls;
        BrStatus status = BrStatus::branch;
        for (IntVarValues j(view); j(); ++j) {
          const int val = j.val();
          tuple_t eq_tuple; eq_values_t eq_vls;
          auto subm_eq = subproblem<ModSpace>(m, v, val, true);
          auto subm_eq_st = subm_eq->status();
          if (subm_eq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_eq->at());
            assert(dlt > 0);
            eq_vls.push_back(true); vls.push_back(val);
            if (subm_eq_st == GC::SS_SOLVED) status = BrStatus::solved;
            else { eq_tuple.push_back(dlt); v_tuple.push_back(dlt); }
          }
          auto subm_neq = subproblem<ModSpace>(m, v, val, false);
          auto subm_neq_st = subm_neq->status();
          if (subm_neq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_neq->at());
            assert(dlt > 0);
            eq_vls.push_back(false);
            if (subm_neq_st == GC::SS_SOLVED) status = BrStatus::solved;
            else eq_tuple.push_back(dlt);
          }
          BrData brd(status, v, {val}, eq_vls, {}, eq_tuple);
          assert(brd.valid());
          brk = brd.update_eq();
          if (brk) { best_brd = brd; break; }
          best_brd = (brd < best_brd) ? brd : best_brd;
        }
        if (brk) break;
        BrData brd(status, v, vls, {}, v_tuple);
        assert(brd.valid());
        brk = brd.update_v();
        if (brk) { best_brd = brd; break; }
        best_brd = (brd < best_brd) ? brd : best_brd;
      }
      if (best_brd.status != BrStatus::failed) ++global_stat.inner_nodes;
      [[maybe_unused]] const auto var = best_brd.var;
      assert(var >= 0 and var >= start and not x[var].assigned());
      assert(best_brd.valid());
      return new Branching<LookaheadEqValAllSln>(*this, best_brd);
    }

    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new Branching<LookaheadEqValAllSln>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef Branching<LookaheadEqValAllSln> Branching;
      const Branching& br = static_cast<const Branching&>(c);
      const BrData& brd = br.brdata;
      assert(br.valid() and brd.valid());
      const auto status = brd.status;
      if (status == BrStatus::failed) {
        ++global_stat.failed_leaves; return GC::ES_FAILED;
      }
      const auto var = brd.var;
      const auto& values = brd.values;
      const auto& eq_values = brd.eq_values;
      assert(var >= 0);
      assert(not values.empty() or not eq_values.empty());
      // Equality-branching:
      if (not eq_values.empty()) {
        assert(values.size() == 1);
        assert(branch == 0 or branch == 1);
        assert(eq_values.size() == 1 or eq_values.size() == 2);
        const auto val = values[0];
        if ( (eq_values[branch] == true and GC::me_failed(x[var].eq(home, val))) or
             (eq_values[branch] == false and GC::me_failed(x[var].nq(home, val))) ) {
          ++global_stat.failed_leaves; return GC::ES_FAILED;
        }
      }
      // Value-branching:
      else {
        assert(not values.empty());
        assert(branch < values.size());
        if (GC::me_failed(x[var].eq(home, values[branch]))) {
          ++global_stat.failed_leaves; return GC::ES_FAILED;
        }
      }

      return GC::ES_OK;
    }

  };

  // A customised LA-based brancher for finding one solution. For a variable var,
  // branching is formed by eq-branches and val-branches.
  template <class ModSpace>
  class LookaheadEqValOneSln : public GC::Brancher {
    IntViewArray x;
    mutable int start;
    measure_t measure;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    bool valid() const noexcept { return valid(start, x); }

    LookaheadEqValOneSln(const GC::Home home, const IntViewArray& x,
      const measure_t measure) : GC::Brancher(home), x(x), start(0), measure(measure) {
    assert(valid(start, x)); }
    LookaheadEqValOneSln(GC::Space& home, LookaheadEqValOneSln& b)
      : GC::Brancher(home,b), start(b.start), measure(b.measure) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x, const measure_t measure) {
      new (home) LookaheadEqValOneSln(home, x, measure);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) LookaheadEqValOneSln(home, *this);
    }
    virtual bool status(const GC::Space&) const {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space& home) {
      assert(valid(start, x));
      assert(start < x.size());

      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);

      BrData best_brd;
      const auto msr = measure(m->at());

      for (int v = start; v < x.size(); ++v) {
        const IntView view = x[v];
        if (view.assigned()) continue;
        assert(view.size() >= 2);
        bool brk = false;
        tuple_t v_tuple;
        values_t vls;
        BrStatus status = BrStatus::branch;
        for (IntVarValues j(view); j(); ++j) {
          const int val = j.val();
          tuple_t eq_tuple; eq_values_t eq_vls;
          auto subm_eq = subproblem<ModSpace>(m, v, val, true);
          auto subm_eq_st = subm_eq->status();
          if (subm_eq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_eq->at());
            assert(dlt > 0);
            eq_vls.push_back(true); vls.push_back(val);
            if (subm_eq_st == GC::SS_SOLVED) {
              status = BrStatus::solved;
              eq_tuple.clear(); eq_vls = {true};
            }
            else { eq_tuple.push_back(dlt); v_tuple.push_back(dlt); }
          }
          auto subm_neq = subproblem<ModSpace>(m, v, val, false);
          auto subm_neq_st = subm_neq->status();
          if (subm_neq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_neq->at());
            assert(dlt > 0);
            eq_vls.push_back(false);
            if (subm_neq_st == GC::SS_SOLVED) {
              status = BrStatus::solved;
              eq_tuple.clear(); eq_vls = {false};
            }
            else eq_tuple.push_back(dlt);
          }
          BrData brd(status, v, {val}, eq_vls, {}, eq_tuple);
          assert(brd.valid());
          brk = (status == BrStatus::solved) or brd.update_eq();
          if (brk) { best_brd = brd; break; }
          best_brd = (brd < best_brd) ? brd : best_brd;
        }
        if (brk) break;
        BrData brd(status, v, vls, {}, v_tuple);
        assert(brd.valid());
        brk = brd.update_v();
        if (brk) { best_brd = brd; break; }
        best_brd = (brd < best_brd) ? brd : best_brd;
      }
      if (best_brd.status != BrStatus::failed) ++global_stat.inner_nodes;
      [[maybe_unused]] const auto var = best_brd.var;
      assert(var >= 0 and var >= start and not x[var].assigned());
      assert(best_brd.valid());
      return new Branching<LookaheadEqValOneSln>(*this, best_brd);
    }

    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new Branching<LookaheadEqValOneSln>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef Branching<LookaheadEqValOneSln> Branching;
      const Branching& br = static_cast<const Branching&>(c);
      const BrData& brd = br.brdata;
      assert(br.valid() and brd.valid());
      const auto status = brd.status;
      if (status == BrStatus::failed) {
        ++global_stat.failed_leaves; return GC::ES_FAILED;
      }
      const auto var = brd.var;
      const auto& values = brd.values;
      const auto& eq_values = brd.eq_values;
      assert(var >= 0);
      assert(not values.empty() or not eq_values.empty());
      // Equality-branching:
      if (not eq_values.empty()) {
        assert(values.size() == 1);
        assert(branch == 0 or branch == 1);
        assert(eq_values.size() == 1 or eq_values.size() == 2);
        const auto val = values[0];
        if ( (eq_values[branch] == true and GC::me_failed(x[var].eq(home, val))) or
             (eq_values[branch] == false and GC::me_failed(x[var].nq(home, val))) ) {
          ++global_stat.failed_leaves; return GC::ES_FAILED;
        }
      }
      // Value-branching:
      else {
        assert(not values.empty());
        assert(branch < values.size());
        if (GC::me_failed(x[var].eq(home, values[branch]))) {
          ++global_stat.failed_leaves; return GC::ES_FAILED;
        }
      }

      return GC::ES_OK;
    }

  };

  template <class ModSpace>
  inline void post_branching(GC::Home home, const GC::IntVarArgs& V,
                             const option_t options) noexcept {
    assert(not home.failed());
    const BrTypeO brt = std::get<BrTypeO>(options);
    const BrSourceO brsrc = std::get<BrSourceO>(options);
    const BrMeasureO brm = std::get<BrMeasureO>(options);
    const BrSolutionO brsln = std::get<BrSolutionO>(options);
    const IntViewArray y(home, V);
    if (brt == BrTypeO::mind and brsrc == BrSourceO::eq) {
      MinDomMinValEq::post(home, y); }
    else if (brt == BrTypeO::mind and brsrc == BrSourceO::v) {
      MinDomValue::post(home, y);
    }
    else if (brt == BrTypeO::la) {
      measure_t measure = (brm == BrMeasureO::mu0) ? mu0 : mu1;
      if (brsrc == BrSourceO::eq and brsln == BrSolutionO::one) {
        LookaheadEqOneSln<ModSpace>::post(home, y, measure);
      }
      else if (brsrc == BrSourceO::eq and brsln == BrSolutionO::all) {
        LookaheadEqAllSln<ModSpace>::post(home, y, measure);
      }
      else if (brsrc == BrSourceO::v and brsln == BrSolutionO::one) {
        LookaheadValueOneSln<ModSpace>::post(home, y, measure);
      }
      else if (brsrc == BrSourceO::v and brsln == BrSolutionO::all) {
        LookaheadValueAllSln<ModSpace>::post(home, y, measure);
      }
      else if (brsrc == BrSourceO::eqv and brsln == BrSolutionO::one) {
        LookaheadEqValOneSln<ModSpace>::post(home, y, measure);
      }
      else if (brsrc == BrSourceO::eqv and brsln == BrSolutionO::all) {
        LookaheadEqValAllSln<ModSpace>::post(home, y, measure);
      }
    }
  }

  template <class ModSpace>
  void find_all_solutions(const std::shared_ptr<ModSpace> m,
                                const bool print = false) noexcept {
    assert(m->valid());
    typedef std::shared_ptr<ModSpace> node_ptr;
    GC::DFS<ModSpace> e(m.get());
    while (const node_ptr s{e.next()}) {
      if (print) s->print();
      ++global_stat.solutions;
    }
    global_stat.engine = e.statistics();
  }
  template <class ModSpace>
  void find_one_solution(const std::shared_ptr<ModSpace> m,
                                const bool print = false) noexcept {
    assert(m->valid());
    typedef std::shared_ptr<ModSpace> node_ptr;
    GC::DFS<ModSpace> e(m.get());
    if (const node_ptr s{e.next()}) {
      if (print) s->print();
      ++global_stat.solutions;
    }
    global_stat.engine = e.statistics();
  }
  template <class ModSpace>
  SearchStat solve(const std::shared_ptr<ModSpace> m,
                   const bool print = false) noexcept {
    assert(m->valid());
    global_stat.reset();
    global_stat.br_options = m->branching_options();
    auto const st = m->status();
    if (st == GC::SS_FAILED) global_stat.failed_leaves = 1;
    const option_t options = m->branching_options();
    const BrSolutionO brsln = std::get<BrSolutionO>(options);
    switch (brsln) {
    case BrSolutionO::all : find_all_solutions(m, print); break;
    default : find_one_solution(m, print);}
    global_stat.update_nodes();
    if (print) global_stat.print();
    return global_stat;
  }

  template <class ModSpace>
  void visualise(const std::shared_ptr<ModSpace> m) noexcept {
    assert(m->valid());
    GC::Gist::Print<ModSpace> p("Print solution");
    GC::Gist::Options o;
    o.inspect.click(&p);
    GC::Gist::dfs(m.get(),o);
  }

}

namespace Environment {
  template <>
  struct RegistrationPolicies<Lookahead::BrTypeO> {
    static constexpr int size = int(Lookahead::BrTypeO::la)+1;
    static constexpr std::array<const char*, size> string
    {"mind", "la"};
  };
  template <>
  struct RegistrationPolicies<Lookahead::BrSourceO> {
    static constexpr int size = int(Lookahead::BrSourceO::eqv)+1;
    static constexpr std::array<const char*, size> string
    {"eq", "v", "eqv"};
  };
  template <>
  struct RegistrationPolicies<Lookahead::BrMeasureO> {
    static constexpr int size = int(Lookahead::BrMeasureO::mu1)+1;
    static constexpr std::array<const char*, size> string
    {"mu0", "mu1"};
  };
  template <>
  struct RegistrationPolicies<Lookahead::BrSolutionO> {
    static constexpr int size = int(Lookahead::BrSolutionO::all)+1;
    static constexpr std::array<const char*, size> string
    {"one", "all"};
  };
}

#endif
