// Oleg Zaikin, 5.4.2021 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

    An implementation of look-ahead for the Gecode library.

 TODOS:

0. Provide overview on functionality provided.
    - Also each function/class needs at least a short specification.

1. DONE ('failed' -> 'unsat', 'solved' -> 'sat')
   Proper names for variables.
    - According to Gecode, now 'failed leaves' and 'failed branches' are used.
    - One variant is to call them 'unsatisfiable leaves' and 'unsatisfiable
      branches' instead of it.
    - Similarly for 'leaves with solutions' and 'branches with solutions':
      'satisfiable leaves' and 'satisfiable branches'.

2. Statistics are urgently needed.
    - Basic statistics (number of nodes, inner nodes, unsatisfiable leaves,
      and solutions) is now calculated if look-ahead branching is used.
    - More statistics will be added soon.

3. Four levels of LA-reduction:
    - Level 0 :
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

4. Generate examples for which tree sizes when using look-ahead are known.
  - It will allow checking correctness of the look-ahead implementation.
  - By now correctness is checked by hand on several small examples:
      Trivial::Sum; Send-more-money; Send-most-money.

5. Later: we don't want to handle variables, but branchers.
  - We can't restrict to just branching on values.
  - We want access to the given possibilities for branching.

6. Later: general concept of a distance.
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
#include <array>
#include <tuple>
#include <string>
#include <iomanip>

#include <cmath>
#include <cassert>
#include <cstdint>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <Numerics/FloatingPoint.hpp>
#include <Numerics/Tau.hpp>
#include <SystemSpecifics/Timing.hpp>
#include <ProgramOptions/Environment.hpp>

namespace Lookahead {

  namespace FP = FloatingPoint;
  namespace GC = Gecode;


  typedef unsigned size_t; // XXX explanations XXX

  typedef FP::float80 float_t;
  typedef std::uint64_t count_t;


  // Explanations (secifications): XXX
  typedef GC::Int::IntView IntView;
  typedef GC::ViewArray<IntView> IntViewArray;
  typedef GC::IntVarValues IntVarValues;

  typedef std::vector<int> values_t;
  typedef std::vector<bool> eq_values_t;


  // XXX Explanations: XXX
  typedef std::vector<float_t> bt_t;
  typedef std::function<float_t(const GC::IntVarArray)> measure_t;


  // XXX specification XXX
  const Timing::UserTime timing; // XXX shouldn't be here (should be local) XXX


  // XXX Specifications XXX
  enum class BrTypeO {la=0, mind=1};
  enum class BrSourceO {eqv=0, eq=1, v=2};
  enum class BrMeasureO {mu0=0, mu1=1};
  enum class BrSolutionO {one=0, all=1};
}
namespace Environment {
  template <>
  struct RegistrationPolicies<Lookahead::BrTypeO> {
    static constexpr int size = int(Lookahead::BrTypeO::mind)+1;
    static constexpr std::array<const char*, size> string
    {"la", "mind"};
  };
  template <>
  struct RegistrationPolicies<Lookahead::BrSourceO> {
    static constexpr int size = int(Lookahead::BrSourceO::v)+1;
    static constexpr std::array<const char*, size> string
    {"eqv", "eq", "v"}; // XXX "v" ?? "val" XXX
    // ??? always same length ???
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
namespace Lookahead {
  constexpr char sep = ',';
  typedef std::tuple<BrTypeO, BrSourceO, BrMeasureO, BrSolutionO> option_t;

  // XXX length ??? XXX
  std::ostream& operator <<(std::ostream& out, const BrTypeO brt) {
    switch (brt) {
    case BrTypeO::mind : return out << "minimal-domain-size-variable";
    default : return out << "best-look-ahead-variable";}
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


  // XXX Specification XXX
  struct SearchStat {
    count_t nodes;
    count_t inner_nodes;
    count_t unsat_leaves;
    count_t solutions;
    count_t choice_calls;
    count_t tau_calls;
    count_t subproblem_calls;

    // XXX use Statistics::BasicStats<double,double> XXX
    Timing::Time_point choice_time;
    Timing::Time_point tau_time;
    Timing::Time_point subproblem_time;
    Timing::Time_point propag_time;

    GC::Search::Statistics engine; // XXX check whether it is a value-object, and rename XXX

    option_t br_options; // XXX not a statistics XXX likely shouldn't be here

    SearchStat() : nodes(0), inner_nodes(0), unsat_leaves(0),
                   solutions(0), choice_calls(0), tau_calls(0),
                   subproblem_calls(0), choice_time(0),
                   tau_time(0), subproblem_time(0), propag_time(0),
                   engine(), br_options() {}

    bool valid() const noexcept {
      return unsat_leaves + solutions + inner_nodes == nodes;
      // XXX seems incomplete XXX
    }

    // XXX ??? remove XXX
    void reset() noexcept {
      assert(valid());
      nodes = inner_nodes = unsat_leaves = solutions = 0;
    }

    // XXX Use a proper class, make all data members private, and make all
    // updating-etc automatic (so this should become private) XXX
    void update_nodes() noexcept {
      const BrTypeO brt = std::get<BrTypeO>(br_options);
      if (brt != BrTypeO::la and unsat_leaves < engine.fail)
        unsat_leaves += engine.fail;
      nodes = inner_nodes + unsat_leaves + solutions;
      assert(valid());
    }

    // XXX Likely the two time-points should be provided XXX choice_calls likely becomes superfluous
    // XXX or provide the time-interval XXX
    void update_choice_stat(const Timing::Time_point t0) noexcept {
      ++choice_calls;
      choice_time += timing() - t0;
    }
    // XXX similar XXX
    void update_tau_stat(const Timing::Time_point t0) noexcept {
      ++tau_calls;
      tau_time += timing() - t0;
    }
    void update_subproblem_stat(const Timing::Time_point t0) noexcept {
      ++subproblem_calls;
      subproblem_time += timing() - t0;
    }

    // XXX only for testing XXX
    friend bool operator ==(const SearchStat& lhs, const SearchStat& rhs) noexcept {
      return lhs.nodes == rhs.nodes and lhs.inner_nodes == rhs.inner_nodes and
             lhs.unsat_leaves == rhs.unsat_leaves and lhs.solutions == rhs.solutions;
    }

    void print() const noexcept {
      assert(valid());
      const BrTypeO brt = std::get<BrTypeO>(br_options);
      const BrSourceO brsrc = std::get<BrSourceO>(br_options);
      const BrMeasureO brm = std::get<BrMeasureO>(br_options);
      const BrSolutionO brsln = std::get<BrSolutionO>(br_options);

      using std::setw;
      const auto w = setw(10);
      std::cout << nodes << w << inner_nodes << w << unsat_leaves << w << solutions
                << w << int(brt) << w << int(brsrc) << w << int(brm) << w
                << int(brsln) << "\n";
    }
  };

  // XXX no global variables in header-files !!! XXX
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
    Timing::Time_point t0 = timing();
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
    global_stat.update_subproblem_stat(t0);
    return c;
  }

  enum class BrStatus { unsat=0, sat=1, branching=2 };

  struct BrData {
    BrStatus status;
    int var;
    values_t values;
    eq_values_t eq_values;
    bt_t v_tuple;
    bt_t eq_tuple;
    float_t ltau;

    bool valid() const noexcept {
      return var >= 0 and eq_values.size() <= 2 and ltau >= 0 and
      (eq_values.empty() or eq_values.size() == 1 or eq_values[0] != eq_values[1]) and
      v_tuple.size() <= values.size() and eq_tuple.size() <= eq_values.size() and
      ((status == BrStatus::unsat and values.empty() and eq_values.empty()) or
       (status != BrStatus::unsat and values.size() == 1 and not eq_values.empty()) or
       (status != BrStatus::unsat and not values.empty() and eq_values.empty()));
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
      assert(status != BrStatus::unsat);
      bool brk = false;
      // If branching of width 1, immediately execute:
      if (v_tuple.size() == 1) {
        assert(status != BrStatus::unsat);
        assert(not values.empty());
        brk = true;
      }
      // If branching of width 0, the problem is unsat, immediately execute:
      else if (v_tuple.empty() and values.empty()) {
        assert(status != BrStatus::sat);
        status = BrStatus::unsat;
        brk = true;
      }
      // If all subproblems are satisfiable, immediately execute:
      else if (v_tuple.empty() and not values.empty()) {
        assert(status == BrStatus::sat);
        brk = true;
      }
      else {
        assert(v_tuple.size() > 1);
        Timing::Time_point t0 = timing();
        ltau = Tau::ltau(v_tuple);
        global_stat.update_tau_stat(t0);
      }
      return brk;
    }

    bool update_eq() noexcept {
      assert(status != BrStatus::unsat);
      bool brk = false;
      if (eq_tuple.size() == 1) {
        assert(status != BrStatus::unsat);
        brk = true;
      }
      else if (eq_tuple.empty() and eq_values.empty()) {
        assert(status != BrStatus::sat);
        values = {};
        status = BrStatus::unsat;
        brk = true;
      }
      else if (eq_tuple.empty() and not eq_values.empty()) {
        assert(status == BrStatus::sat);
        brk = true;
      }
      else {
        assert(not eq_tuple.empty());
        Timing::Time_point t0 = timing();
        ltau = Tau::ltau(eq_tuple);
        global_stat.update_tau_stat(t0);
      }
      return brk;
    }

    size_t branches_num() const noexcept {
      assert(valid());
      if (eq_values.empty()) return values.empty() ? 1 : values.size();
      else return eq_values.size();
    }

    BrData(const BrStatus st=BrStatus::unsat, const int v=0, const values_t vls={},
           const eq_values_t eq_vls={}, const bt_t v_tpl={}, const bt_t eq_tpl={})
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
      Timing::Time_point t0 = timing();
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
      BrData brd(BrStatus::branching, var, values);
      assert(brd.valid());
      global_stat.update_choice_stat(t0);
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
      assert(brd.status == BrStatus::branching);
      assert(var >= 0 and not values.empty());
      assert(branch < values.size());
      // Unsatisfiable leaf:
      if (GC::me_failed(x[var].eq(home, values[branch]))) {
        ++global_stat.unsat_leaves;
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
      Timing::Time_point t0 = timing();
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
      BrData brd(BrStatus::branching, var, values, eq_values);
      assert(brd.valid());
      global_stat.update_choice_stat(t0);
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
      assert(brd.status == BrStatus::branching);
      assert(brd.values.size() == 1);
      const auto var = brd.var;
      const auto val = brd.values[0];
      const auto& eq_values = brd.eq_values;
      assert(var >= 0);
      assert(eq_values.size() == 2);
      assert(branch == 0 or branch == 1);
      if ( (eq_values[branch] == true and GC::me_failed(x[var].eq(home, val))) or
           (eq_values[branch] == false and GC::me_failed(x[var].nq(home, val))) ) {
        ++global_stat.unsat_leaves;
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
      Timing::Time_point t0 = timing();
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
        bt_t v_tuple; values_t vls;
        BrStatus status = BrStatus::branching;
        // For all values of the current variable:
        for (IntVarValues j(view); j(); ++j) {
          // Assign value, propagate, and measure:
          const int val = j.val();
          auto subm = subproblem<ModSpace>(m, v, val, true);
          Timing::Time_point t1 = timing();
          auto subm_st = subm->status();
          global_stat.propag_time += timing() - t1;
          // Skip unsatisfiable branches:
          if (subm_st != GC::SS_FAILED) {
            // Calculate delta of measures:
            float_t dlt = msr - measure(subm->at());
            assert(dlt > 0);
            vls.push_back(val);
            if (subm_st == GC::SS_SOLVED) status = BrStatus::sat;
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
      if (best_brd.status != BrStatus::unsat) ++global_stat.inner_nodes;
      [[maybe_unused]] const auto var = best_brd.var;
      assert(var >= 0 and var >= start and not x[var].assigned());
      assert(best_brd.valid());
      global_stat.update_choice_stat(t0);
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
      assert(status == BrStatus::unsat or branch < values.size());
      // If unsatisfiable branching, stop executing:
      if (status == BrStatus::unsat or
          GC::me_failed(x[var].eq(home, values[branch]))) {
        ++global_stat.unsat_leaves;
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
      Timing::Time_point t0 = timing();
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
        bt_t v_tuple; values_t vls;
        BrStatus status = BrStatus::branching;
        // For all values of the current variable:
        for (IntVarValues j(view); j(); ++j) {
          // Assign value, propagate, and measure:
          const int val = j.val();
          auto subm = subproblem<ModSpace>(m, v, val);
          Timing::Time_point t1 = timing();
          auto subm_st = subm->status();
          global_stat.propag_time += timing() - t1;
          // Stop ff a solution is found:
          if (subm_st == GC::SS_SOLVED) {
            v_tuple.clear(); vls = {val};
            status = BrStatus::sat;
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
        bool brk = (status == BrStatus::sat) or brd.update_v();
        if (brk) { best_brd = brd; break; }
        best_brd = (brd < best_brd) ? brd : best_brd;
      }
      if (best_brd.status != BrStatus::unsat) ++global_stat.inner_nodes;
      [[maybe_unused]] const auto var = best_brd.var;
      assert(var >= 0 and var >= start and not x[var].assigned());
      assert(best_brd.valid());
      global_stat.update_choice_stat(t0);
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
      assert(status == BrStatus::unsat or branch < values.size());
      // If unsatisfiable branching, stop executing:
      if (status == BrStatus::unsat or
          GC::me_failed(x[var].eq(home, values[branch]))) {
        ++global_stat.unsat_leaves;
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
      Timing::Time_point t0 = timing();
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
          bt_t eq_tuple; eq_values_t eq_vls;
          BrStatus status = BrStatus::branching;
          // variable == value:
          auto subm_eq = subproblem<ModSpace>(m, v, val, true);
          Timing::Time_point t1 = timing();
          auto subm_eq_st = subm_eq->status();
          global_stat.propag_time += timing() - t1;
          if (subm_eq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_eq->at());
            assert(dlt > 0);
            eq_vls.push_back(true);
            if (subm_eq_st == GC::SS_SOLVED) status = BrStatus::sat;
            else eq_tuple.push_back(dlt);
          }
          // variable != value:
          auto subm_neq = subproblem<ModSpace>(m, v, val, false);
          t1 = timing();
          auto subm_neq_st = subm_neq->status();
          global_stat.propag_time += timing() - t1;
          if (subm_neq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_neq->at());
            assert(dlt > 0);
            eq_vls.push_back(false);
            if (subm_neq_st == GC::SS_SOLVED) status = BrStatus::sat;
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
      if (best_brd.status != BrStatus::unsat) ++global_stat.inner_nodes;
      [[maybe_unused]] const auto var = best_brd.var;
      assert(var >= 0 and var >= start and not x[var].assigned());
      assert(best_brd.valid());
      global_stat.update_choice_stat(t0);
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
      if (status == BrStatus::unsat) {
        ++global_stat.unsat_leaves; return GC::ES_FAILED;
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
        ++global_stat.unsat_leaves; return GC::ES_FAILED;
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
      Timing::Time_point t0 = timing();
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
          BrStatus status = BrStatus::branching;
          bt_t eq_tuple; eq_values_t eq_vls;
          // variable == value:
          auto subm_eq = subproblem<ModSpace>(m, v, val, true);
          Timing::Time_point t1 = timing();
          auto subm_eq_st = subm_eq->status();
          global_stat.propag_time += timing() - t1;
          if (subm_eq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_eq->at());
            assert(dlt > 0);
            eq_vls.push_back(true);
            if (subm_eq_st == GC::SS_SOLVED) {
              status = BrStatus::sat;
              eq_tuple.clear(); eq_vls = {true};
            }
            else eq_tuple.push_back(dlt);
          }
          // variable != value:
          auto subm_neq = subproblem<ModSpace>(m, v, val, false);
          t1 = timing();
          auto subm_neq_st = subm_neq->status();
          global_stat.propag_time += timing() - t1;
          if (subm_neq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_neq->at());
            assert(dlt > 0);
            eq_vls.push_back(false);
            if (subm_neq_st == GC::SS_SOLVED) {
              status = BrStatus::sat;
              eq_tuple.clear(); eq_vls = {false};
            }
            else eq_tuple.push_back(dlt);
          }
          BrData brd(status, v, {val}, eq_vls, {}, eq_tuple);
          assert(brd.valid());
          brk = (status == BrStatus::sat) or brd.update_eq();
          if (brk) { best_brd = brd; break; }
          best_brd = (brd < best_brd) ? brd : best_brd;
        }
        if (brk) break;
      }
      if (best_brd.status != BrStatus::unsat) ++global_stat.inner_nodes;
      [[maybe_unused]] const auto var = best_brd.var;
      assert(var >= 0 and var >= start and not x[var].assigned());
      assert(best_brd.valid());
      global_stat.update_choice_stat(t0);
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
      if (status == BrStatus::unsat) {
        ++global_stat.unsat_leaves; return GC::ES_FAILED;
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
        ++global_stat.unsat_leaves; return GC::ES_FAILED;
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
      Timing::Time_point t0 = timing();
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
        bt_t v_tuple;
        values_t vls;
        BrStatus status = BrStatus::branching;
        for (IntVarValues j(view); j(); ++j) {
          const int val = j.val();
          bt_t eq_tuple; eq_values_t eq_vls;
          auto subm_eq = subproblem<ModSpace>(m, v, val, true);
          Timing::Time_point t1 = timing();
          auto subm_eq_st = subm_eq->status();
          global_stat.propag_time += timing() - t1;
          if (subm_eq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_eq->at());
            assert(dlt > 0);
            eq_vls.push_back(true); vls.push_back(val);
            if (subm_eq_st == GC::SS_SOLVED) status = BrStatus::sat;
            else { eq_tuple.push_back(dlt); v_tuple.push_back(dlt); }
          }
          auto subm_neq = subproblem<ModSpace>(m, v, val, false);
          t1 = timing();
          auto subm_neq_st = subm_neq->status();
          global_stat.propag_time += timing() - t1;
          if (subm_neq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_neq->at());
            assert(dlt > 0);
            eq_vls.push_back(false);
            if (subm_neq_st == GC::SS_SOLVED) status = BrStatus::sat;
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
      if (best_brd.status != BrStatus::unsat) ++global_stat.inner_nodes;
      [[maybe_unused]] const auto var = best_brd.var;
      assert(var >= 0 and var >= start and not x[var].assigned());
      assert(best_brd.valid());
      global_stat.update_choice_stat(t0);
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
      if (status == BrStatus::unsat) {
        ++global_stat.unsat_leaves; return GC::ES_FAILED;
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
          ++global_stat.unsat_leaves; return GC::ES_FAILED;
        }
      }
      // Value-branching:
      else {
        assert(not values.empty());
        assert(branch < values.size());
        if (GC::me_failed(x[var].eq(home, values[branch]))) {
          ++global_stat.unsat_leaves; return GC::ES_FAILED;
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
      Timing::Time_point t0 = timing();
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
        bt_t v_tuple;
        values_t vls;
        BrStatus status = BrStatus::branching;
        for (IntVarValues j(view); j(); ++j) {
          const int val = j.val();
          bt_t eq_tuple; eq_values_t eq_vls;
          auto subm_eq = subproblem<ModSpace>(m, v, val, true);
          Timing::Time_point t1 = timing();
          auto subm_eq_st = subm_eq->status();
          global_stat.propag_time += timing() - t1;
          if (subm_eq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_eq->at());
            assert(dlt > 0);
            eq_vls.push_back(true); vls.push_back(val);
            if (subm_eq_st == GC::SS_SOLVED) {
              status = BrStatus::sat;
              eq_tuple.clear(); eq_vls = {true};
            }
            else { eq_tuple.push_back(dlt); v_tuple.push_back(dlt); }
          }
          auto subm_neq = subproblem<ModSpace>(m, v, val, false);
          t1 = timing();
          auto subm_neq_st = subm_neq->status();
          global_stat.propag_time += timing() - t1;
          if (subm_neq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_neq->at());
            assert(dlt > 0);
            eq_vls.push_back(false);
            if (subm_neq_st == GC::SS_SOLVED) {
              status = BrStatus::sat;
              eq_tuple.clear(); eq_vls = {false};
            }
            else eq_tuple.push_back(dlt);
          }
          BrData brd(status, v, {val}, eq_vls, {}, eq_tuple);
          assert(brd.valid());
          brk = (status == BrStatus::sat) or brd.update_eq();
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
      if (best_brd.status != BrStatus::unsat) ++global_stat.inner_nodes;
      [[maybe_unused]] const auto var = best_brd.var;
      assert(var >= 0 and var >= start and not x[var].assigned());
      assert(best_brd.valid());
      global_stat.update_choice_stat(t0);
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
      if (status == BrStatus::unsat) {
        ++global_stat.unsat_leaves; return GC::ES_FAILED;
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
          ++global_stat.unsat_leaves; return GC::ES_FAILED;
        }
      }
      // Value-branching:
      else {
        assert(not values.empty());
        assert(branch < values.size());
        if (GC::me_failed(x[var].eq(home, values[branch]))) {
          ++global_stat.unsat_leaves; return GC::ES_FAILED;
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
                   const bool printsol = false) noexcept {
    assert(m->valid());
    global_stat.reset();
    global_stat.br_options = m->branching_options();
    auto const st = m->status();
    if (st == GC::SS_FAILED) global_stat.unsat_leaves = 1;
    const option_t options = m->branching_options();
    const BrSolutionO brsln = std::get<BrSolutionO>(options);
    switch (brsln) {
    case BrSolutionO::all : find_all_solutions(m, printsol); break;
    default : find_one_solution(m, printsol);}
    global_stat.update_nodes();
    return global_stat;
  }

}


#endif
