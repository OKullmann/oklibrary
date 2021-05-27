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

0. Four levels of LA-reduction:
    - Level 0 :
     - no explicit reduction;
     - for every branching unsatisfiable branching are just removed;
     - if a branching of width 0 is created, the problem is (immediately
       recognised as unsatisfiable;
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

1. DONE Is it appropriate to pass Gecode::IntVarArray by copy?

  - Copying would only be appropriate if the internal data stored
    is just a pointer, plus a bit more data.
  - The base class Gecode::VarArray
      https://www.gecode.org/doc-latest/reference/kernel_2data_2array_8hpp_source.html
    seems to contain exactly two data-members:
      int n;
      Var* x;
  - Thus possibly copying is safe and efficient.
  - Indeed, it can be done safely.

2. DONE Call of status() likely needs a check for early abortion.

3. Use Environment to parse enumeration from argv.

4. Generate examples for which tree sizes when using look-ahead are known.
  - It will allow checking correctness of the look-ahead implementation.
  - By now correctness is checked by hand on several small examples:
      Trivial::Sum; Send-more-money; Send-most-money.

*/

#ifndef LOOKAHEAD_lNFKYYpHQ8
#define LOOKAHEAD_lNFKYYpHQ8

#include <iostream>
#include <limits>
#include <vector>
#include <memory>

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
  typedef std::vector<float_t> tuple_t;

  inline constexpr size_t tr(const int size, [[maybe_unused]] const size_t bound = 0) noexcept {
    assert(bound <= std::numeric_limits<int>::max());
    assert(size >= int(bound));
    return size;
  }


  inline float_t mu0(const GC::IntVarArray V) noexcept {
    float_t s = 0;
    for (const auto& v : V) {
      const auto is = tr(v.size(), 1);
      s += is - 1;
    }
    return s;
  }

  inline float_t mu1(const GC::IntVarArray V) noexcept {
    float_t s = 0;
    for (const auto& v : V) {
      const auto is = tr(v.size(), 1);
      s += FloatingPoint::log2(is);
    }
    return s;
  }


  template<class ModSpace>
  inline GC::SpaceStatus constr_var_eq(ModSpace* m, const int i,
                                       const int val) noexcept {
    size_t v = tr(i);
    assert(m->valid()); assert(m->valid(v));
    GC::rel(*m, m->at(v), GC::IRT_EQ, val);
    return m->status();
  }


  template<class ModSpace>
  inline float_t measure(const ModSpace* const m) noexcept {
    assert(m->valid());
    return mu0(m->at());
  }

  struct LaMeasureStat {
    GC::SpaceStatus status;
    float_t delta;
    LaMeasureStat() : status(GC::SS_BRANCH), delta(0) {}
    LaMeasureStat(const GC::SpaceStatus st, const float_t dlt) :
                  status(st), delta(dlt) {}
  };
  template<class ModSpace>
  LaMeasureStat la_measure(ModSpace* const m, const int v,
                           const int val) noexcept {
    assert(m->valid());
    assert(m->valid(v));
    assert(m->status() == GC::SS_BRANCH);
    // Clone space:
    std::unique_ptr<ModSpace> c(static_cast<ModSpace*>(m->clone()));
    assert(c->valid());
    assert(c->valid(v));
    assert(c->status() == GC::SS_BRANCH);
    // Add an equality constraint for the given variable and its value:
    const auto st = constr_var_eq(c.get(), v, val);
    // Calculate the distance between the parent and the child node:
    float_t dlt = (st == GC::SS_BRANCH) ? measure(m) - measure(c.get()) : -1;
    LaMeasureStat res(st, dlt);
    return res;
  }


  template <class NaryBrancher>
  struct VarVal : public GC::Choice {
    int pos;
    values_t values;

    inline bool valid(const values_t v) const noexcept {
      return not v.empty();
    }
    inline bool valid(const int p, const values_t v) const noexcept {
      return p >= 0 and valid(v);
    }
    inline bool valid() const noexcept {
      return valid(pos, values);
    }

    VarVal(const NaryBrancher& b, const int p, const values_t V)
      : GC::Choice(b, V.size()), pos(p), values(V) {
      assert(valid(pos, values));
    }

    virtual void archive(GC::Archive& e) const {
      assert(valid(pos, values));
      GC::Choice::archive(e);
      size_t width = values.size();
      assert(width > 0);
      e << width << pos;
      for (auto v : values) e << v;
      assert(tr(e.size()) == width + 2);
    }

  };


  class NarySizeMin : public GC::Brancher {
    IntViewArray x;
    mutable int start;

    inline bool valid(const IntViewArray x) const noexcept {
      return x.size() > 0;
    }
    inline bool valid(const int s, const IntViewArray x) const noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    inline bool valid() const noexcept {
      return valid(start, x);
    }

    NarySizeMin(const GC::Home home, const IntViewArray& x)
      : GC::Brancher(home), x(x), start(0) { assert(valid(start, x)); }
    NarySizeMin(GC::Space& home, NarySizeMin& b)
      : GC::Brancher(home,b), start(b.start) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x) {
      new (home) NarySizeMin(home, x);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) NarySizeMin(home, *this);
    }
    virtual bool status(const GC::Space&) const {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space&) {
      assert(valid(start, x));
      int pos = start;
      size_t width = tr(x[pos].size());
      assert(width > 0);
      for (int i = start + 1; i < x.size(); ++i)
        if (not x[i].assigned() and x[i].size() < width) {
          pos = i; width = tr(x[pos].size());
          assert(width > 0);
        }
      assert(pos >= start);
      values_t values;
      for (GC::Int::ViewValues i(x[pos]); i(); ++i)
        values.push_back(i.val());
      assert(pos >= 0 and not values.empty());
      return new VarVal<NarySizeMin>(*this, pos, values);
    }
    virtual GC::Choice* choice(const GC::Space&, GC::Archive& e) {
      assert(valid(start, x));
      size_t width; int pos;
      assert(e.size() >= 3);
      e >> width >> pos;
      assert(width > 0 and pos >= 0);
      assert(tr(e.size()) == width + 2);
      int v; values_t values;
      for (size_t i = 0; i < width; ++i) {
        e >> v; values.push_back(v);
      }
      assert(pos >= 0 and not values.empty());
      return new VarVal<NarySizeMin>(*this, pos, values);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef VarVal<NarySizeMin> VarVal;
      const VarVal& pv = static_cast<const VarVal&>(c);
      assert(pv.valid());
      const auto values = pv.values;
      const auto pos = pv.pos;
      assert(pos >= 0 and not values.empty());
      assert(branch < values.size());
      return GC::me_failed(x[pos].eq(home, values[branch])) ?
             GC::ES_FAILED : GC::ES_OK;
    }

  };


  template <class ModSpace>
  class NaryLookahead : public GC::Brancher {
    IntViewArray x;
    mutable int start;

    inline bool valid(const IntViewArray x) const noexcept {
      return x.size() > 0;
    }
    inline bool valid(const int s, const IntViewArray x) const noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    inline bool valid() const noexcept {
      return valid(start, x);
    }

    NaryLookahead(const GC::Home home, const IntViewArray& x)
      : GC::Brancher(home), x(x), start(0) { assert(valid(start, x)); }
    NaryLookahead(GC::Space& home, NaryLookahead& b)
      : GC::Brancher(home,b), start(b.start) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x) {
      new (home) NaryLookahead(home, x);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) NaryLookahead(home, *this);
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
      bool solved = false;
      int pos = start;
      float_t best_ltau = FP::pinfinity;
      values_t best_values;

      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);

      // For remaining variables (all before 'start' are assigned):
      for (int v = start; v < x.size(); ++v) {
        // v is a variable, view is the values in Gecode format:
        const IntView view = x[v];
        // Skip assigned variables:
        if (view.assigned()) continue;
        assert(view.size() >= 2);
        tuple_t tuple; values_t values;
        // For all values of the current variable:
        for (IntVarValues j(view); j(); ++j) {
          // Assign value, propagate, and measure:
          const int val = j.val();
          const auto s = la_measure<ModSpace>(m, v, val);
          assert(s.status != GC::SS_BRANCH or s.delta > 0);
          // Skip failed branches:
          if (s.status != GC::SS_FAILED) {
            values.push_back(val);
            if (s.status == GC::SS_SOLVED) solved = true;
            else tuple.push_back(s.delta);
          }
        }
        // If branching of width 1 or solution is found, choose the variable:
        if (tuple.size() == 1 or solved) {
          assert(not values.empty());
          pos = v; best_values = values; break;
        }
        // If branching of width 0, report that the current branch is failed.
        // This is done by choosing the variable and the first failed value:
        else if (tuple.empty()) {
          assert(values.empty());
          IntVarValues j(x[v]);
          best_values = {j.val()};
          pos = v;
          break;
        }
        // Calculate ltau and update the best value if needed:
        const float_t ltau = Tau::ltau(tuple);
        if (ltau < best_ltau) {
          best_ltau = ltau; pos = v; best_values = values;
        }
      }

      assert(pos >= 0 and pos >= start);
      assert(not x[pos].assigned());
      assert(not best_values.empty());
      return new VarVal<NaryLookahead>(*this, pos, best_values);
    }

    virtual GC::Choice* choice(const GC::Space&, GC::Archive& e) {
      assert(valid(start, x));
      size_t width; int pos;
      assert(e.size() >= 3);
      e >> width >> pos;
      assert(width > 0 and pos >= 0);
      assert(tr(e.size()) == width + 2);
      int v; values_t values;
      for (size_t i = 0; i < width; ++i) {
        e >> v; values.push_back(v);
      }
      assert(pos >= 0 and not values.empty());
      return new VarVal<NaryLookahead>(*this, pos, values);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef VarVal<NaryLookahead> VarVal;
      const VarVal& pv = static_cast<const VarVal&>(c);
      assert(pv.valid());
      const auto& values = pv.values;
      const auto pos = pv.pos;
      assert(pos >= 0 and not values.empty());
      assert(branch < values.size());
      return GC::me_failed(x[pos].eq(home, values[branch])) ?
             GC::ES_FAILED : GC::ES_OK;
    }

  };


  enum class BranchingO { binarysizeminvalmin=0, narysizeminvalmin=1,
                          narylookahead=2 };

  inline bool show_usage(const Environment::ProgramInfo proginfo,
                         const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [branching-type] [visual]\n\n" <<
    " branching-type : \"-binmin\"  (standard Gecode: binary minimal domain size; minimal value) or\n"
    "                  \"-narymin\" (default, customisied: nary minimal domain size; minimal value) or\n"
    "                  \"-naryla\"  (customisied: nary look-ahead lookahead)\n"
    " visual         : \"-gist\" (run Gist to visualise the search tree).\n\n";
    return true;
  }

  inline BranchingO branching_type(const std::string s) noexcept {
    if (s == "-binmin") return BranchingO::binarysizeminvalmin;
    if (s == "-naryla") return BranchingO::narylookahead;
    return BranchingO::narysizeminvalmin;
  }

  template <class ModSpace>
  inline void post_branching(GC::Home home, const GC::IntVarArgs& V,
                             const BranchingO b) noexcept {
    assert(not home.failed());
    switch (b) {
    case BranchingO::binarysizeminvalmin :
      GC::branch(home, V, GC::INT_VAR_SIZE_MIN(), GC::INT_VAL_MIN());
      break;
    case BranchingO::narysizeminvalmin : {
      const IntViewArray y(home, V);
      NarySizeMin::post(home, y);
      break;
    }
    case BranchingO::narylookahead : {
      const IntViewArray y(home, V);
      NaryLookahead<ModSpace>::post(home, y);
      break;
    }
    default :
      GC::branch(home, V, GC::INT_VAR_SIZE_MIN(), GC::INT_VAL_MIN());
      break;
    }
  }

  struct SearchStat {
    count_t solutions;
    GC::Search::Statistics engine;

    SearchStat() : solutions(0), engine() {}

    void print() const noexcept {
      using std::setw;
      const auto w = setw(10);
      std::cout << engine.node << w << engine.fail << w << solutions << "\n";
    }
  };

  template <class ModSpace>
  SearchStat find_all_solutions(const std::shared_ptr<ModSpace> m,
                                const bool print = false) noexcept {
    assert(m->valid());
    typedef std::shared_ptr<ModSpace> node_ptr;
    GC::DFS<ModSpace> e(m.get());
    SearchStat stat;
    while (const node_ptr s{e.next()}) {
      if (print) s->print();
      ++stat.solutions;
    }
    stat.engine = e.statistics();
    return stat;
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

#endif
