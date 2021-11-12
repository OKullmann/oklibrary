// Oleg Zaikin, 5.4.2021 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

An implementation of look-ahead for the Gecode library.

 TODOS:

-2. Independent reduction before choosing branchings.
    - The reduction should be either eager or lazy.
    - Additional parameter: restart processing the main loop or not.

-1. Branchers for finding all solutions.
    - Now the same branchers are used to find one or all solutions.
    - The behaviour should be a bit different.

0. Provide overview on functionality provided.
    - Also each function/class needs at least a short specification.

1. Divide Lookahead.hpp into several files.
    - DONE (SearchStat was moved to Statistics.hpp)
      At least the SearchStat struct should mode into new Statistics.hpp,

2. Statistics are urgently needed.
    - DONE Basic statistics (number of nodes, inner nodes, unsatisfiable leaves,
      and solutions)
    - DONE Number of inner nodes with 2 children and 3 children.
    - DONE (single-child branchings are counted and not treated as nodes)
      Distinguish between branchings and nodes.
      Now each branching is treated as an inner node. Rather, single-child
      branchings should not be treated like any node. However, such branchings
      must be also counted.

3. Four levels of LA-reduction:
    - Level 0 :
     - no explicit reduction;
     - for every branching unsatisfiable branches are just removed;
     - if a branching of width 0 is created, the problem is (immediately)
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
f
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
#include <algorithm>
#include <queue>

#include <cmath>
#include <cassert>
#include <cstdint>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <Numerics/FloatingPoint.hpp>
#include <Numerics/Tau.hpp>
#include <SystemSpecifics/Timing.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Statistics.hpp"

namespace Lookahead {

  namespace FP = FloatingPoint;
  namespace GC = Gecode;

  // size_t is used for sizes of Gecode arrays.
  // For a Gecode array, size() returns int, so the function
  // size_t tr(int size) was introduced to convert int to size_t.
  typedef unsigned size_t;
  inline constexpr size_t tr(const int size, [[maybe_unused]] const size_t bound = 0) noexcept {
    assert(bound <= std::numeric_limits<int>::max());
    assert(size >= int(bound));
    return size;
  }

  typedef FP::float80 float_t;

  // Array of values of an integer variable:
  typedef GC::Int::IntView IntView;
  // Array of array of values of integer variables:
  typedef GC::ViewArray<IntView> IntViewArray;
  // Value iterator for an integer variable:
  typedef GC::IntVarValues IntVarValues;

  typedef std::vector<int> values_t;
  typedef std::vector<bool> eq_values_t;

  // bt_t is a branching tuple, i.e. a tuple of distances.
  // Distance is how much simpler the problem became.
  typedef std::vector<float_t> bt_t;
  // measure_t is a function for measuring a given formula.
  // A fromula is an array of integer variables and their values.
  typedef std::function<float_t(const GC::IntVarArray)> measure_t;


  // Branching type, i.e. how branching is formed and executed.
  // la: choose a variable via look-ahead.
  // mind: choose a variable with minimal domain size.
  enum class BrTypeO {la=0, mind=1};

  // How branches are formed for a branching.
  // eq: for a variable var and its values val1, ..., valk,
  //  for each value vali the branching is formed by two branches
  //  var=vali, var!=vali.
  // val: for a variable var and its values val1, ..., valk,
  //  one branching is formed by branches var=val1, ..., var=valk.
  enum class BrSourceO {eqval=0, eq=1, val=2};

  // Function to measure a formula represented by an array of
  // integer variables. The function used to calculate distances,
  // which form branching tuples.
  // mu0: summation of size(var)-1 for all variables.
  // mu1: product of log2 of sizes of all variables.
  enum class BrMeasureO {mu0=0, mu1=1};

  // The number of solutions to find.
  // one: find one solution or prove that no solution exists.
  // all: find all solutions or prove that no solution exists.
  enum class BrSolutionO {one=0, all=1};

  // Mode of applying found single-child branchings.
  // eager - apply a found single-child branching immediately.
  // lazy  - collect all single-child branchings and apply them
  //        as a partial assignment.
  enum class BrEagernessO {eager=0, lazy=1};
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
    static constexpr int size = int(Lookahead::BrSourceO::val)+1;
    static constexpr std::array<const char*, size> string
    {"eqval", "eq", "val"};
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
  template <>
  struct RegistrationPolicies<Lookahead::BrEagernessO> {
    static constexpr int size = int(Lookahead::BrEagernessO::lazy)+1;
    static constexpr std::array<const char*, size> string
    {"eager", "lazy"};
  };
}
namespace Lookahead {
  constexpr char sep = ',';
  typedef std::tuple<BrTypeO, BrSourceO, BrMeasureO, BrSolutionO, BrEagernessO> option_t;

  std::ostream& operator <<(std::ostream& out, const BrTypeO brt) {
    switch (brt) {
    case BrTypeO::mind : return out << "min-domain-size";
    default : return out << "lookahead";}
  }
  std::ostream& operator <<(std::ostream& out, const BrSourceO brsrs) {
    switch (brsrs) {
    case BrSourceO::eq : return out << "equals-only";
    case BrSourceO::val : return out << "values-only";
    default : return out << "equals+values";}
  }
  std::ostream& operator <<(std::ostream& out, const BrMeasureO brm) {
    switch (brm) {
    case BrMeasureO::mu1 : return out << "product-msr";
    default : return out << "sum-msr";}
  }
  std::ostream& operator <<(std::ostream& out, const BrSolutionO brsln) {
    switch (brsln) {
    case BrSolutionO::all : return out << "all-sol";
    default : return out << "one-sol";}
  }
  std::ostream& operator <<(std::ostream& out, const BrEagernessO bregr) {
    switch (bregr) {
    case BrEagernessO::lazy : return out << "lazy";
    default : return out << "eager";}
  }

  // XXX no global variables in header-files !!! XXX
  Statistics::SearchStat global_stat;

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
      s += FP::log2(is);
    }
    return s;
  }


  template<class ModSpace>
  std::shared_ptr<ModSpace> subproblem(ModSpace* const m, const int v, const int val,
                                       const bool eq = true) noexcept {
    const Timing::UserTime timing;
    const Timing::Time_point t0 = timing();
    assert(m->valid());
    assert(m->valid(v));
    assert(m->status() == GC::SS_BRANCH);
    // Clone space:
    std::shared_ptr<ModSpace> c(static_cast<ModSpace*>(m->clone()));
    assert(c->valid());
    assert(c->valid(v));
    assert(c->status() == GC::SS_BRANCH);
    // Add an equality constraint for the given variable and its value:
    if (eq) GC::rel(*(c.get()), (c.get())->at(v), GC::IRT_EQ, val, GC::IPL_DOM);
    else GC::rel(*(c.get()), (c.get())->at(v), GC::IRT_NQ, val, GC::IPL_DOM);
    const Timing::Time_point t1 = timing();
    global_stat.update_subproblem_stat(t1-t0);
    return c;
  }

  enum class BrStatus { unsat=0, sat=1, single=2, branching=3 };

  // Equality branching: at most two branches of the kind var==val and var!=val.
  //  - brvalues : is a Boolean array of branches: true means var==val,
  //    false means var!=val. Possible eq_values are: {}, {false}, {true}, {false, true}.
  //  - tuple : branching tuple, where each value corresponds to a branch.
  //  - ltau : value of the ltau function for the branching tuple.
  struct EqBranching {
    int var;
    int value;
    eq_values_t brvalues;
    bt_t tuple;
    float_t ltau;
    BrStatus brstatus;

    EqBranching(const int v=0, const int val=0, const eq_values_t brvls={}, const bt_t tpl={})
      : var(v), value(val), brvalues(brvls), tuple(tpl), ltau(FP::pinfinity) {
        // If branching of width 0, the problem is unsat:
        if (tuple.empty() and brvalues.empty()) brstatus = BrStatus::unsat;
        // If at least one subproblem is satisfiable:
        else if (tuple.size() != brvalues.size()) brstatus = BrStatus::sat;
        // If branching of width 1:
        else if (tuple.size() == 1 and brvalues.size() == 1) brstatus = BrStatus::single;
        // Two branches, neither unsat or sat:
        else brstatus = BrStatus::branching;
        valid();
    }

   bool valid() const noexcept {
      return var >= 0 and brvalues.size() <= 2 and ltau >= 0 and
      (brvalues.empty() or brvalues.size() == 1 or brvalues[0] != brvalues[1]) and
      tuple.size() <= brvalues.size();
    }

    bool operator <(const EqBranching& a) const noexcept { return ltau < a.ltau; }

    void print() const noexcept {
      std::cout << static_cast<int>(brstatus) << " " << var << " " << value << "{";
      for (auto x : brvalues) std::cout << int(x) << ",";
      std::cout << "} {";
      for (auto& x : tuple) std::cout << (int)x << ",";
      std::cout << "} " << ltau << std::endl;
    }

    BrStatus status() const noexcept { return brstatus; }

    void calc_ltau() noexcept {
      assert(not tuple.empty());
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      ltau = Tau::ltau(tuple);
      const Timing::Time_point t1 = timing();
      global_stat.update_tau_stat(t1-t0);
    }

    size_t branches_num() const noexcept {
      assert(valid());
      if (brstatus == BrStatus::unsat) return 1;
      else return brvalues.size();
    }

  };

  // Value branching of the kind var=val1, ... , var=valk.
  //  - brvalues : is a integer array of branches.
  //  - tuple : branching tuple, where each value corresponds to a branch.
  //  - ltau : value of the ltau function for the branching tuple.
  struct ValBranching {
    int var;
    values_t values;
    bt_t tuple;
    float_t ltau;
    BrStatus brstatus;

    ValBranching(const int v=0, const values_t vls={}, const bt_t tpl={})
      : var(v), values(vls), tuple(tpl), ltau(FP::pinfinity) {
        // If branching of width 0, the problem is unsat:
        if (tuple.empty() and values.empty()) brstatus = BrStatus::unsat;
        // If at least one subproblem is satisfiable:
        else if (tuple.size() != values.size()) brstatus = BrStatus::sat;
        // If branching of width 1:
        else if (tuple.size() == 1 and values.size() == 1) brstatus = BrStatus::single;
        // Two branches, neither unsat or sat:
        else brstatus = BrStatus::branching;
        valid();
    }

   bool valid() const noexcept {
      return var >= 0 and ltau >= 0 and tuple.size() <= values.size();
    }

    bool operator <(const ValBranching& a) const noexcept { return ltau < a.ltau; }

    void print() const noexcept {
      std::cout << static_cast<int>(brstatus) << " " << var << " {";
      for (auto x : values) std::cout << int(x) << ",";
      std::cout << "} {";
      for (auto& x : tuple) std::cout << (int)x << ",";
      std::cout << "} " << ltau << std::endl;
    }

    BrStatus status() const noexcept { return brstatus; }

    void calc_ltau() noexcept {
      assert(not tuple.empty());
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      ltau = Tau::ltau(tuple);
      const Timing::Time_point t1 = timing();
      global_stat.update_tau_stat(t1-t0);
    }

    size_t branches_num() const noexcept {
      assert(valid());
      return values.empty() ? 1 : values.size();
    }

  };

  struct Branching {
    BrStatus status;
    int var;
    values_t values;
    eq_values_t eq_values;
    bt_t v_tuple;
    bt_t eq_tuple;
    float_t ltau;

    Branching(BrStatus status = BrStatus::unsat, const int v=0, const values_t vls={}, const eq_values_t eq_vls={},
              const bt_t v_tpl={}, const bt_t eq_tpl={})
      : status(status), var(v), values(vls), eq_values(eq_vls), v_tuple(v_tpl),
      eq_tuple(eq_tpl), ltau(FP::pinfinity) { valid(); }

   bool valid() const noexcept {
      return var >= 0 and eq_values.size() <= 2 and ltau >= 0 and
      (eq_values.empty() or eq_values.size() == 1 or eq_values[0] != eq_values[1]) and
      v_tuple.size() <= values.size() and eq_tuple.size() <= eq_values.size() and
      ((status == BrStatus::unsat and values.empty() and eq_values.empty()) or
       (status != BrStatus::unsat and values.size() == 1 and not eq_values.empty()) or
       (status != BrStatus::unsat and not values.empty() and eq_values.empty()));
    }

    bool operator <(const Branching& a) const noexcept { return ltau < a.ltau; }

    void print() const noexcept {
      std::cout << static_cast<int>(status) << " " << var << " {";
      for (auto& x : values) std::cout << x << ",";
      std::cout << "} {";
      for (auto x : eq_values) std::cout << int(x) << ",";
      std::cout << "} {";
      for (auto& x : v_tuple) std::cout << (int)x << ",";
      std::cout << "} {";
      for (auto& x : eq_tuple) std::cout << (int)x << ",";
      std::cout << "} " << ltau << std::endl;
    }

    BrStatus status_eq() noexcept {
      status = BrStatus::branching;
      // If branching of width 0, the problem is unsat:
      if (eq_tuple.empty() and eq_values.empty()) {
        status = BrStatus::unsat;
      }
      // If at least one subproblem is satisfiable:
      else if (eq_tuple.size() != eq_values.size()) {
        status = BrStatus::sat;
      }
      // If branching of width 1:
      else if (eq_tuple.size() == 1 and eq_values.size() == 1) {
        status = BrStatus::single;
      }
      return status;
    }

    BrStatus status_val() noexcept {
      status = BrStatus::branching;
      if (v_tuple.empty() and values.empty()) {
        status = BrStatus::unsat;
      }
      else if (v_tuple.size() != values.size()) {
        status = BrStatus::sat;
      }
      else if (v_tuple.size() == 1 and values.size() == 1) {
        status = BrStatus::single;
      }
      return status;
    }

    void calc_ltau() noexcept {
      assert(not eq_tuple.empty() or not v_tuple.empty());
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      if (not eq_tuple.empty()) ltau = Tau::ltau(eq_tuple);
      else ltau = Tau::ltau(v_tuple);
      const Timing::Time_point t1 = timing();
      global_stat.update_tau_stat(t1-t0);
    }

    size_t branches_num() const noexcept {
      assert(valid());
      if (status == BrStatus::unsat) return 1;
      if (eq_values.empty()) return values.empty() ? 1 : values.size();
      else return eq_values.size();
    }

  };

  template<class CustomBranching>
  CustomBranching best_branching(std::vector<CustomBranching>& branchings) {
    assert(not branchings.empty());
    CustomBranching best_br;
    for (auto& br : branchings) {
      assert(br.status() == BrStatus::branching);
      br.calc_ltau();
      best_br = std::min(best_br, br);
    }
    return best_br;
  }

  Branching best_branching(std::vector<Branching>& branchings) {
    assert(not branchings.empty());
    Branching best_br;
    for (auto& br : branchings) {
      assert(br.status == BrStatus::branching);
      br.calc_ltau();
      best_br = std::min(best_br, br);
    }
    return best_br;
  }

  template<class CustomBranching>
  std::vector<CustomBranching> best_branchings(std::vector<CustomBranching>& tau_brs) noexcept {
    std::vector<CustomBranching> branchings;
    assert(not tau_brs.empty());
    CustomBranching br = best_branching<CustomBranching>(tau_brs);
    branchings = {br};
    return branchings;
  }

  std::vector<Branching> best_branchings(std::vector<Branching>& tau_brs) noexcept {
    std::vector<Branching> branchings;
    assert(not tau_brs.empty());
    Branching br = best_branching(tau_brs);
    branchings = {br};
    return branchings;
  }

  struct SingleChildBranching {
    int var;
    int val;
    bool eq;
    SingleChildBranching(const int var, const int val, const bool eq) :
      var(var), val(val), eq(eq) {}
  };
  struct ReduceRes {
    int var;
    values_t values;
    BrStatus status;
    ReduceRes() : var(0), values{}, status(BrStatus::branching) {}
    ReduceRes(const int var, const values_t values, const BrStatus status) :
      var(var), values(values), status(status) {}
  };
  template<class ModSpace>
  ReduceRes reduceEager(GC::Space& home, const IntViewArray x, const int start,
                        const bool eqbr=false) {
    ReduceRes res;
    assert(start < x.size());
    ModSpace* const m = &(static_cast<ModSpace&>(home));
    assert(m->status() == GC::SS_BRANCH);

    /* TODO : used implied vars in the lazy mode
    std::vector<bool> implied_vars(x.size());
    for (int v = start; v < x.size(); ++v) implied_vars[v] = false;
    if (view.assigned() or implied_vars[v]) continue;
    if (tmp_implied_vars.empty()) {
      tmp_implied_vars = implied_vars;
      auto subm_x = subm->at();
      for (int v2 = start; v2 < subm_x.size(); ++v2) {
        const IntView view = x[v2];
        const IntView subm_view = subm_x[v2];
        if (not view.assigned() and subm_view.assigned()) tmp_implied_vars[v2] = true;
      }
    }*/

    bool reduction = false;
    do {
      reduction = false;
      for (int var = start; var < x.size(); ++var) {
        const IntView view = x[var];
        if (view.assigned()) continue;
        assert(view.size() >= 2);
        values_t brvalues;
        std::vector<SingleChildBranching> var_single_child_brs;
        for (IntVarValues j(view); j(); ++j) {
          const int val = j.val();
          assert(m->status() == GC::SS_BRANCH);
          auto subm = subproblem<ModSpace>(m, var, val, true);
          const auto subm_st = subm->status();
          if (subm_st == GC::SS_FAILED) {
            SingleChildBranching sch(var, val, false);
            var_single_child_brs.push_back(sch);
          }
          else {
            brvalues.push_back(val);
            if (subm_st == GC::SS_SOLVED) {
              if (eqbr) return ReduceRes(var, {val}, BrStatus::sat);
              else res.status = BrStatus::sat;
            }
          }
        }

        if (res.status == BrStatus::sat) {
          assert(not brvalues.empty());
          return ReduceRes(var, brvalues, BrStatus::sat);
        }
        // No branches, so the problem is unsatisfiable:
        else if (brvalues.size() == 0) {
          return ReduceRes(0, {}, BrStatus::unsat);
        }
        // If single-child branching:
        else if (brvalues.size() == 1) {
          ++global_stat.single_child_brnch;
          reduction = true;
          GC::rel(home, x[var], GC::IRT_EQ, brvalues[0], GC::IPL_DOM);
          const auto st = home.status();
          if (st == GC::SS_FAILED) return ReduceRes(0, {}, BrStatus::unsat);
          else if (st == GC::SS_SOLVED) return ReduceRes(var, brvalues, BrStatus::sat);
        }
        // None from above - non-sat, non-unsat, at least 2 branches:
        else {
          if (not var_single_child_brs.empty()) reduction = true;
          for (auto& sch : var_single_child_brs) {
            assert(not sch.eq);
            GC::rel(home, x[sch.var], GC::IRT_NQ, sch.val, GC::IPL_DOM);
            const auto st = home.status();
            if (st == GC::SS_FAILED) return ReduceRes(0, {}, BrStatus::unsat);
            else if (st == GC::SS_SOLVED) return ReduceRes(sch.var, {sch.val}, BrStatus::sat);
          }
        }
      } // for (int var = start; var < x.size(); ++var) {
    } while (reduction);

    return res;
  }

  template<class ModSpace>
  ReduceRes reduceLazy(GC::Space& home, const IntViewArray x, const int start,
                       const bool eqbr=false) {
    ReduceRes res;
    assert(start < x.size());
    ModSpace* const m = &(static_cast<ModSpace&>(home));
    assert(m->status() == GC::SS_BRANCH);

    bool reduction = false;
    do {
      reduction = false;
      std::vector<SingleChildBranching> single_child_brs;
      for (int var = start; var < x.size(); ++var) {
        const IntView view = x[var];
        if (view.assigned()) continue;
        assert(view.size() >= 2);
        values_t brvalues;
        std::vector<SingleChildBranching> var_single_child_brs;
        for (IntVarValues j(view); j(); ++j) {
          const int val = j.val();
          assert(m->status() == GC::SS_BRANCH);
          auto subm = subproblem<ModSpace>(m, var, val, true);
          const auto subm_st = subm->status();
          if (subm_st == GC::SS_FAILED) {
            SingleChildBranching sch(var, val, false);
            var_single_child_brs.push_back(sch);
          }
          else {
            brvalues.push_back(val);
            if (subm_st == GC::SS_SOLVED) {
              if (eqbr) return ReduceRes(var, {val}, BrStatus::sat);
              else res.status = BrStatus::sat;
            }
          }
        }

        if (res.status == BrStatus::sat) {
          assert(not brvalues.empty());
          return ReduceRes(var, brvalues, BrStatus::sat);
        }
        // No branches, so the problem is unsatisfiable:
        else if (brvalues.size() == 0) {
          return ReduceRes(0, {}, BrStatus::unsat);
        }
        // If single-child branching:
        else if (brvalues.size() == 1) {
          ++global_stat.single_child_brnch;
          reduction = true;
          // var==val:
          SingleChildBranching sch(var, brvalues[0], true);
          single_child_brs.push_back(sch);
        }
        // None from above - non-sat, non-unsat, at least 2 branches:
        else {
          if (not var_single_child_brs.empty()) reduction = true;
          for (auto& sch : var_single_child_brs) {
            assert(not sch.eq);
            single_child_brs.push_back(sch);
          }
        }
      } // for (int var = start; var < x.size(); ++var) {
      if (not single_child_brs.empty()) {
        for (auto& sch : single_child_brs) {
          if (sch.eq) GC::rel(home, x[sch.var], GC::IRT_EQ, sch.val, GC::IPL_DOM);
          else GC::rel(home, x[sch.var], GC::IRT_NQ, sch.val, GC::IPL_DOM);
          const auto st = home.status();
          if (st == GC::SS_FAILED) return ReduceRes(0, {}, BrStatus::unsat);
          else if (st == GC::SS_SOLVED) return ReduceRes(sch.var, {sch.val}, BrStatus::sat);
        }
      }
    } while (reduction);

    return res;
  }

  template <class CustomisedEqBrancher>
  struct EqBranchingChoice : public GC::Choice {
    EqBranching br;
    bool valid() const noexcept { return br.valid(); }
    EqBranchingChoice(const CustomisedEqBrancher& b, const EqBranching& br = EqBranching())
      : GC::Choice(b, br.branches_num()), br(br) {
      const auto childs = br.branches_num();
      if (childs > 1) ++global_stat.inner_nodes;
      switch (childs) {
        case 1:
          ++global_stat.single_child_brnch;
          break;
        case 2:
          ++global_stat.inner_nodes_2chld;
          break;
        case 3:
          ++global_stat.inner_nodes_3chld;
          break;
        default:
          break;
      }
    }
  };

  template <class CustomisedValBrancher>
  struct ValBranchingChoice : public GC::Choice {
    ValBranching br;
    bool valid() const noexcept { return br.valid(); }
    ValBranchingChoice(const CustomisedValBrancher& b, const ValBranching& br = ValBranching())
      : GC::Choice(b, br.branches_num()), br(br) {
      const auto childs = br.branches_num();
      if (childs > 1) ++global_stat.inner_nodes;
      switch (childs) {
        case 1:
          ++global_stat.single_child_brnch;
          break;
        case 2:
          ++global_stat.inner_nodes_2chld;
          break;
        case 3:
          ++global_stat.inner_nodes_3chld;
          break;
        default:
          break;
      }
    }
  };

  template <class CustomisedBrancher>
  struct BranchingChoice : public GC::Choice {
    Branching br;
    bool valid() const noexcept { return br.valid(); }
    BranchingChoice(const CustomisedBrancher& b, const Branching& br = Branching())
      : GC::Choice(b, br.branches_num()), br(br) {
      const auto childs = br.branches_num();
      if (childs > 1) ++global_stat.inner_nodes;
      switch (childs) {
        case 1:
          ++global_stat.single_child_brnch;
          break;
        case 2:
          ++global_stat.inner_nodes_2chld;
          break;
        case 3:
          ++global_stat.inner_nodes_3chld;
          break;
        default:
          break;
      }
      assert(valid());
    }
  };

  // Struct for transition of an array of branchings from choice to commit.
  template <class CustomisedBrancher>
  struct ArrayBranchingChoice : public GC::Choice {
    std::vector<Branching> branchings;
    bool valid() const noexcept {
      for (auto &br : branchings) if (not br.valid()) return false;
      return true;
    }
    ArrayBranchingChoice(const CustomisedBrancher& b, const std::vector<Branching> branchings = {})
      : GC::Choice(b, branchings[0].branches_num()), branchings(branchings) {
      // Count single-chilg branchings only once:
      if (branchings[0].branches_num() == 1) ++global_stat.single_child_brnch;
      for (auto &br : branchings) {
        const auto childs = br.branches_num();
        if (childs > 1) ++global_stat.inner_nodes;
        switch (childs) {
          case 2:
            ++global_stat.inner_nodes_2chld;
            break;
          case 3:
            ++global_stat.inner_nodes_3chld;
            break;
          default:
            break;
        }
      }
    }
  };

  // Struct for transition of an array of branchings from choice to commit.
  template <class CustomisedBrancher>
  struct ArrayEqBranchingChoice : public GC::Choice {
    std::vector<EqBranching> branchings;
    bool valid() const noexcept {
      for (auto &br : branchings) if (not br.valid()) return false;
      return true;
    }
    ArrayEqBranchingChoice(const CustomisedBrancher& b, const std::vector<EqBranching> branchings = {})
      : GC::Choice(b, branchings[0].branches_num()), branchings(branchings) {
      // Count single-chilg branchings only once:
      if (branchings[0].branches_num() == 1) ++global_stat.single_child_brnch;
      for (auto &br : branchings) {
        const auto childs = br.branches_num();
        if (childs > 1) ++global_stat.inner_nodes;
        switch (childs) {
          case 2:
            ++global_stat.inner_nodes_2chld;
            break;
          case 3:
            ++global_stat.inner_nodes_3chld;
            break;
          default:
            break;
        }
      }
    }
  };

// Struct for transition of an array of branchings from choice to commit.
  template <class CustomisedBrancher>
  struct ArrayValBranchingChoice : public GC::Choice {
    std::vector<ValBranching> branchings;
    bool valid() const noexcept {
      for (auto &br : branchings) if (not br.valid()) return false;
      return true;
    }
    ArrayValBranchingChoice(const CustomisedBrancher& b, const std::vector<ValBranching> branchings = {})
      : GC::Choice(b, branchings[0].branches_num()), branchings(branchings) {
      // Count single-chilg branchings only once:
      if (branchings[0].branches_num() == 1) ++global_stat.single_child_brnch;
      for (auto &br : branchings) {
        const auto childs = br.branches_num();
        if (childs > 1) ++global_stat.inner_nodes;
        switch (childs) {
          case 2:
            ++global_stat.inner_nodes_2chld;
            break;
          case 3:
            ++global_stat.inner_nodes_3chld;
            break;
          default:
            break;
        }
      }
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
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
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
      Branching br(BrStatus::branching, var, values);
      const Timing::Time_point t1 = timing();
      global_stat.update_choice_stat(t1-t0);
      return new BranchingChoice<MinDomValue>(*this, br);
    }
    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new BranchingChoice<MinDomValue>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef BranchingChoice<MinDomValue> BrChoice;
      const BrChoice& brc = static_cast<const BrChoice&>(c);
      const Branching& br = brc.br;
      assert(brc.valid() and br.valid());
      const auto& values = br.values;
      const auto var = br.var;
      assert(br.status == BrStatus::branching);
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
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
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
      Branching br(BrStatus::branching, var, values, eq_values);
      const Timing::Time_point t1 = timing();
      global_stat.update_choice_stat(t1-t0);
      return new BranchingChoice<MinDomMinValEq>(*this, br);
    }
    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new BranchingChoice<MinDomMinValEq>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef BranchingChoice<MinDomMinValEq> BrChoice;
      const BrChoice& brc = static_cast<const BrChoice&>(c);
      const Branching& br = brc.br;
      assert(brc.valid() and br.valid());
      assert(br.status == BrStatus::branching);
      assert(br.values.size() == 1);
      const auto var = br.var;
      const auto val = br.values[0];
      const auto& eq_values = br.eq_values;
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


  // A customised LA-based brancher. Branchings are formed by assigning
  // all possible values to all unassigned variables. The best branching
  // is chosen via the tau-function.
  template <class ModSpace>
  class LookaheadValue : public GC::Brancher {
    IntViewArray x;
    mutable int start;
    measure_t measure;
    BrEagernessO bregr;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    bool valid() const noexcept { return valid(start, x); }

    LookaheadValue(const GC::Home home, const IntViewArray& x,
      const measure_t measure, const BrEagernessO bregr) : GC::Brancher(home), x(x),
      start(0), measure(measure), bregr(bregr) {
    assert(valid(start, x)); }
    LookaheadValue(GC::Space& home, LookaheadValue& b)
      : GC::Brancher(home,b), start(b.start), measure(b.measure), bregr(b.bregr) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x, const measure_t measure,
                     const BrEagernessO bregr) {
      new (home) LookaheadValue(home, x, measure, bregr);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) LookaheadValue(home, *this);
    }
    virtual bool status(const GC::Space&) const noexcept {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space& home) {
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      ReduceRes res = (bregr == BrEagernessO::eager) ?
        reduceEager<ModSpace>(home, x, start):
        reduceLazy<ModSpace>(home, x, start);
      // Update the start variable:
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; break;}
      assert(valid(start, x));
      assert(start < x.size());
      ValBranching best_br;
      ValBranching unsat_br(start);
      assert(unsat_br.status() == BrStatus::unsat);
      if (res.status == BrStatus::unsat) {
        best_br = unsat_br;
      }
      else if (res.status == BrStatus::sat) {
        ValBranching br = ValBranching(res.var, res.values, {});
        assert(br.status() == BrStatus::sat);
        best_br = br;
      }
      else {
        assert(res.status == BrStatus::branching);
        ModSpace* const m = &(static_cast<ModSpace&>(home));
        assert(m->status() == GC::SS_BRANCH);
        const auto msr = measure(m->at());
        std::vector<ValBranching> tau_brs;
        // For remaining variables (all before 'start' are assigned):
        for (int v = start; v < x.size(); ++v) {
          // v is a variable, view is the values in Gecode format:
          const IntView view = x[v];
          // Skip assigned variables:
          if (view.assigned()) continue;
          assert(view.size() >= 2);
          bt_t v_tuple; values_t vls;
          // For all values of the current variable:
          for (IntVarValues j(view); j(); ++j) {
            // Assign value, propagate, and measure:
            const int val = j.val();
            auto subm = subproblem<ModSpace>(m, v, val, true);
            [[maybe_unused]] const auto  subm_st = subm->status();
            assert(subm_st == GC::SS_BRANCH);
            // Calculate delta of measures:
            float_t dlt = msr - measure(subm->at());
            assert(dlt > 0);
            vls.push_back(val);
            v_tuple.push_back(dlt);
          }
          ValBranching br(v, vls, v_tuple);
          assert(br.status() == BrStatus::branching);
          tau_brs.push_back(br);
        } // for (int v = start; v < x.size(); ++v) {
        best_br = best_branching<ValBranching>(tau_brs);
      }

      [[maybe_unused]] const auto var = best_br.var;
      assert(var >= 0 and var >= start);
      assert(not x[var].assigned() or best_br.status() == BrStatus::unsat);
      const Timing::Time_point t1 = timing();
      global_stat.update_choice_stat(t1-t0);
      return new ValBranchingChoice<LookaheadValue>(*this, best_br);
    }

    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new ValBranchingChoice<LookaheadValue>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef ValBranchingChoice<LookaheadValue> BrChoice;
      const BrChoice& brc = static_cast<const BrChoice&>(c);
      const ValBranching& br = brc.br;
      assert(brc.valid());
      const auto status = br.status();
      const auto var = br.var;
      const auto& values = br.values;
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

  // A customised LA-based brancher for finding one solution. Branchings are
  // formed by assigning all possible values to all unassigned variables. The
  // best branching is chosen via the tau-function.
  template <class ModSpace>
  class LookaheadLazyValueOneSln : public GC::Brancher {
    IntViewArray x;
    mutable int start;
    measure_t measure;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    bool valid() const noexcept { return valid(start, x); }

    LookaheadLazyValueOneSln(const GC::Home home, const IntViewArray& x,
      const measure_t measure) : GC::Brancher(home), x(x), start(0), measure(measure) {
    assert(valid(start, x)); }
    LookaheadLazyValueOneSln(GC::Space& home, LookaheadLazyValueOneSln& b)
      : GC::Brancher(home,b), start(b.start), measure(b.measure) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x, const measure_t measure) {
      new (home) LookaheadLazyValueOneSln(home, x, measure);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) LookaheadLazyValueOneSln(home, *this);
    }
    virtual bool status(const GC::Space&) const {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space& home) {
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      assert(valid(start, x));
      assert(start < x.size());
      std::vector<ValBranching> branchings;
      ValBranching best_br;
      ValBranching unsat_br(start);

      ReduceRes res = reduce<ModSpace>(home, x, start);
      if (res.status == BrStatus::unsat) {
        branchings = {unsat_br};
      }
      else if (res.status == BrStatus::sat) {
        ValBranching br = ValBranching(res.var, res.values, {});
        branchings = {br};
      }
      else {
        ModSpace* const m = &(static_cast<ModSpace&>(home));
        assert(m->status() == GC::SS_BRANCH);
        const auto msr = measure(m->at());
        std::vector<ValBranching> tau_brs;

        // For remaining variables (all before 'start' are assigned):
        for (int v = start; v < x.size(); ++v) {
          // v is a variable, view is the values in Gecode format:
          const IntView view = x[v];
          // Skip assigned variables:
          if (view.assigned()) continue;
          assert(view.size() >= 2);
          bt_t tuple; values_t vls;
          std::vector<bool> tmp_implied_vars;
          // For all values of the current variable:
          for (IntVarValues j(view); j(); ++j) {
            // Assign value, propagate, and measure:
            const int val = j.val();
            auto subm = subproblem<ModSpace>(m, v, val, true);
            auto subm_st = subm->status();
            assert(subm_st != GC::SS_SOLVED);
            // Skip unsatisfiable branches:
            if (subm_st == GC::SS_BRANCH) {
              // Calculate delta of measures:
              float_t dlt = msr - measure(subm->at());
              assert(dlt > 0);
              vls.push_back(val);
              tuple.push_back(dlt);
            }
          }
          ValBranching br(v, vls, tuple);
          assert(br.status() == BrStatus::branching);
          tau_brs.push_back(br);
        }
        branchings = best_branchings<ValBranching>(tau_brs);
        assert(not branchings.empty());
      }

      [[maybe_unused]] const auto var = branchings[0].var;
      assert(var >= start and not x[var].assigned());
      const Timing::Time_point t1 = timing();
      global_stat.update_choice_stat(t1-t0);
      return new ArrayValBranchingChoice<LookaheadLazyValueOneSln>(*this, branchings);
    }

    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new BranchingChoice<LookaheadLazyValueOneSln>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef ArrayValBranchingChoice<LookaheadLazyValueOneSln> ArrBrChoice;
      const ArrBrChoice& brc = static_cast<const ArrBrChoice&>(c);
      assert(brc.valid());
      const auto& branchings = brc.branchings;
      for (auto &br : branchings) {
        const auto status = br.status();
        const auto var = br.var;
        const auto& values = br.values;
        // If unsatisfiable branching, stop executing:
        if (status == BrStatus::unsat or
            GC::me_failed(x[var].eq(home, values[branch]))) {
          ++global_stat.unsat_leaves;
          return GC::ES_FAILED;
        }
      }
      // Execute branching:
      return GC::ES_OK;
    }

  };

  // A customised LA-based brancher. For a variable var and its value val,
  // branching is formed by two branches: var==val and
  // var!=val. The best branching is chosen via the tau-function.
  template <class ModSpace>
  class LookaheadEq : public GC::Brancher {
    IntViewArray x;
    mutable int start;
    measure_t measure;
    BrEagernessO bregr;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    bool valid() const noexcept { return valid(start, x); }

    LookaheadEq(const GC::Home home, const IntViewArray& x,
      const measure_t measure, const BrEagernessO bregr) : GC::Brancher(home), x(x), start(0), measure(measure),
        bregr(bregr) {
    assert(valid(start, x)); }
    LookaheadEq(GC::Space& home, LookaheadEq& b)
      : GC::Brancher(home,b), start(b.start), measure(b.measure), bregr(b.bregr) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x, const measure_t measure,
      const BrEagernessO bregr) {
      new (home) LookaheadEq(home, x, measure, bregr);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) LookaheadEq(home, *this);
    }
    virtual bool status(const GC::Space&) const {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space& home) {
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      ReduceRes res = (bregr == BrEagernessO::eager) ?
        reduceEager<ModSpace>(home, x, start, true):
        reduceLazy<ModSpace>(home, x, start, true);
      // Update the start variable:
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; break;}
      assert(valid(start, x));
      assert(start < x.size());
      EqBranching best_br;
      EqBranching unsat_br(start);
      assert(unsat_br.status() == BrStatus::unsat);
      if (res.status == BrStatus::unsat) {
        best_br = unsat_br;
      }
      else if (res.status == BrStatus::sat) {
        assert(res.values.size() == 1);
        EqBranching br = EqBranching(res.var, res.values[0], {true,false});
        assert(br.status() == BrStatus::sat);
        best_br = br;
      }
      else {
        assert(res.status == BrStatus::branching);
        ModSpace* m = &(static_cast<ModSpace&>(home));
        assert(m->status() == GC::SS_BRANCH);
        const auto msr = measure(m->at());
        std::vector<EqBranching> tau_brs;

        for (int var = start; var < x.size(); ++var) {
          const IntView view = x[var];
          if (view.assigned()) continue;
          assert(view.size() >= 2);
          for (IntVarValues j(view); j(); ++j) {
            const int val = j.val();
            auto subm_eq = subproblem<ModSpace>(m, var, val, true);
            [[maybe_unused]] const auto subm_eq_st = subm_eq->status();
            assert(subm_eq_st == GC::SS_BRANCH);
            float_t dlt1 = msr - measure(subm_eq->at());
            assert(dlt1 > 0);
            auto subm_neq = subproblem<ModSpace>(m, var, val, false);
            [[maybe_unused]] const auto subm_neq_st = subm_neq->status();
            assert(subm_neq_st == GC::SS_BRANCH);
            float_t dlt2 = msr - measure(subm_neq->at());
            assert(dlt2 > 0);
            EqBranching br(var, val, {true,false}, {dlt1,dlt2});
            assert(br.status() == BrStatus::branching);
            tau_brs.push_back(br);
          }
        }
        assert(not tau_brs.empty());
        best_br = best_branching<EqBranching>(tau_brs);
      }

      [[maybe_unused]] const auto var = best_br.var;
      assert(var >= 0 and var >= start);
      assert(not x[var].assigned() or best_br.status() == BrStatus::unsat);
      const Timing::Time_point t1 = timing();
      global_stat.update_choice_stat(t1-t0);
      return new EqBranchingChoice<LookaheadEq>(*this, best_br);
    }

    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new EqBranchingChoice<LookaheadEq>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef EqBranchingChoice<LookaheadEq> BrChoice;
      const BrChoice& brc = static_cast<const BrChoice&>(c);
      const EqBranching& br = brc.br;
      if (br.status() == BrStatus::unsat) {
        ++global_stat.unsat_leaves; return GC::ES_FAILED;
      }
      const auto var = br.var;
      const auto& val = br.value;
      const auto& brvalues = br.brvalues;
      assert(var >= 0);
      assert(brvalues.size() == 1 or brvalues.size() == 2);
      assert(branch == 0 or branch == 1);
      assert(branch < brvalues.size());
      if ( (brvalues[branch] == true and GC::me_failed(x[var].eq(home, val))) or
           (brvalues[branch] == false and GC::me_failed(x[var].nq(home, val))) ) {
        ++global_stat.unsat_leaves; return GC::ES_FAILED;
      }
      return GC::ES_OK;
    }

  };

  // A customised LA-based brancher for finding one solution. For a variable var,
  // branching is formed by eq-branches and val-branches.
  template <class ModSpace>
  class LookaheadEagerEqValOneSln : public GC::Brancher {
    IntViewArray x;
    mutable int start;
    measure_t measure;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    bool valid() const noexcept { return valid(start, x); }

    LookaheadEagerEqValOneSln(const GC::Home home, const IntViewArray& x,
      const measure_t measure) : GC::Brancher(home), x(x), start(0), measure(measure) {
    assert(valid(start, x)); }
    LookaheadEagerEqValOneSln(GC::Space& home, LookaheadEagerEqValOneSln& b)
      : GC::Brancher(home,b), start(b.start), measure(b.measure) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x, const measure_t measure) {
      new (home) LookaheadEagerEqValOneSln(home, x, measure);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) LookaheadEagerEqValOneSln(home, *this);
    }
    virtual bool status(const GC::Space&) const {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space& home) {
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      assert(valid(start, x));
      assert(start < x.size());
      Branching unsat_br(BrStatus::unsat, start, {}, {}, {}, {});
      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      const auto msr = measure(m->at());
      Branching best_br;
      std::vector<Branching> tau_brs;
      bool brk = false;

      for (int v = start; v < x.size(); ++v) {
        const IntView view = x[v];
        if (view.assigned()) continue;
        assert(view.size() >= 2);
        bt_t v_tuple;
        values_t vls;
        BrStatus status = BrStatus::branching;
        for (IntVarValues j(view); j(); ++j) {
          const int val = j.val();
          bt_t eq_tuple; eq_values_t eq_vls;
          auto subm_eq = subproblem<ModSpace>(m, v, val, true);
          auto subm_eq_st = subm_eq->status();
          if (subm_eq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_eq->at());
            assert(dlt > 0);
            eq_vls.push_back(true); vls.push_back(val);
            if (subm_eq_st != GC::SS_SOLVED) {
              eq_tuple.push_back(dlt); v_tuple.push_back(dlt); 
            }
          }
          auto subm_neq = subproblem<ModSpace>(m, v, val, false);
          auto subm_neq_st = subm_neq->status();
          if (subm_neq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_neq->at());
            assert(dlt > 0);
            eq_vls.push_back(false);
            if (subm_neq_st != GC::SS_SOLVED) eq_tuple.push_back(dlt);
          }
          Branching br(status, v, {val}, eq_vls, {}, eq_tuple);
          if (br.status_eq() == BrStatus::unsat) {
            best_br = unsat_br;
            brk = true; break;
          }
          else if (br.status_eq() == BrStatus::sat or br.status_eq() == BrStatus::single) {
            best_br = br;
            brk = true; break;
          }
          else if (br.status_eq() == BrStatus::branching) {
            tau_brs.push_back(br);
          }
          if (brk) break;
        }
        if (brk) break;
        Branching br(status, v, vls, {}, v_tuple);
	      if (br.status_val() == BrStatus::unsat) {
          best_br = unsat_br;
          brk = true; break;
        }
        else if (br.status_val() == BrStatus::sat or br.status_val() == BrStatus::single) {
          best_br = br;
          brk = true; break;
        }
        else if (br.status_val() == BrStatus::branching) {
          tau_brs.push_back(br);
        }
      }

      if (not brk) best_br = best_branching(tau_brs);

      [[maybe_unused]] const auto var = best_br.var;
      assert(var >= 0 and var >= start and not x[var].assigned());
      assert(best_br.valid());
      const Timing::Time_point t1 = timing();
      global_stat.update_choice_stat(t1-t0);
      return new BranchingChoice<LookaheadEagerEqValOneSln>(*this, best_br);
    }

    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new BranchingChoice<LookaheadEagerEqValOneSln>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef BranchingChoice<LookaheadEagerEqValOneSln> BrChoice;
      const BrChoice& brc = static_cast<const BrChoice&>(c);
      const Branching& br = brc.br;
      assert(brc.valid() and br.valid());
      const auto status = br.status;
      if (status == BrStatus::unsat) {
        ++global_stat.unsat_leaves; return GC::ES_FAILED;
      }
      const auto var = br.var;
      const auto& values = br.values;
      const auto& eq_values = br.eq_values;
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
  class LookaheadLazyEqValOneSln : public GC::Brancher {
    IntViewArray x;
    mutable int start;
    measure_t measure;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }

  public:

    bool valid() const noexcept { return valid(start, x); }

    LookaheadLazyEqValOneSln(const GC::Home home, const IntViewArray& x,
      const measure_t measure) : GC::Brancher(home), x(x), start(0), measure(measure) {
    assert(valid(start, x)); }
    LookaheadLazyEqValOneSln(GC::Space& home, LookaheadLazyEqValOneSln& b)
      : GC::Brancher(home,b), start(b.start), measure(b.measure) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    static void post(GC::Home home, const IntViewArray& x, const measure_t measure) {
      new (home) LookaheadLazyEqValOneSln(home, x, measure);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) LookaheadLazyEqValOneSln(home, *this);
    }
    virtual bool status(const GC::Space&) const {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space& home) {
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      assert(valid(start, x));
      assert(start < x.size());
      Branching unsat_br(BrStatus::unsat, start, {}, {}, {}, {});
      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      const auto msr = measure(m->at());
      Branching best_br;
      std::vector<Branching> tau_brs, single_brs, branchings;
      bool brk = false;

      for (int v = start; v < x.size(); ++v) {
        const IntView view = x[v];
        if (view.assigned()) continue;
        assert(view.size() >= 2);
        bt_t v_tuple;
        values_t vls;
        BrStatus status = BrStatus::branching;
        for (IntVarValues j(view); j(); ++j) {
          const int val = j.val();
          bt_t eq_tuple; eq_values_t eq_vls;
          auto subm_eq = subproblem<ModSpace>(m, v, val, true);
          auto subm_eq_st = subm_eq->status();
          if (subm_eq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_eq->at());
            assert(dlt > 0);
            eq_vls.push_back(true); vls.push_back(val);
            if (subm_eq_st != GC::SS_SOLVED) {
              eq_tuple.push_back(dlt); v_tuple.push_back(dlt); 
            }
          }
          auto subm_neq = subproblem<ModSpace>(m, v, val, false);
          auto subm_neq_st = subm_neq->status();
          if (subm_neq_st != GC::SS_FAILED) {
            float_t dlt = msr - measure(subm_neq->at());
            assert(dlt > 0);
            eq_vls.push_back(false);
            if (subm_neq_st != GC::SS_SOLVED) eq_tuple.push_back(dlt);
          }
          Branching br(status, v, {val}, eq_vls, {}, eq_tuple);
          if (br.status_eq() == BrStatus::unsat) {
            best_br = unsat_br;
            brk = true; break;
          }
          else if (br.status_eq() == BrStatus::sat) {
            best_br = br;
            brk = true; break;
          }
          else if (br.status_eq() == BrStatus::single) {
            assert(br.eq_values.size() == 1);
            single_brs.push_back(br);
            // Skip other values of the assigned variable:
            if (subm_neq_st == GC::SS_FAILED) break;
          }
          // Add a branching for which later ltau will be possibly calculated:
          else if (br.status_eq() == BrStatus::branching and single_brs.empty()) {
            tau_brs.push_back(br);
          }
          if (brk) break;
        }
        if (brk) break;
        Branching br(status, v, vls, {}, v_tuple);
	      if (br.status_val() == BrStatus::unsat) {
          best_br = unsat_br;
          brk = true; break;
        }
        else if (br.status_val() == BrStatus::sat) {
          best_br = br;
          brk = true; break;
        }
        else if (br.status_val() == BrStatus::single) {
          assert(br.values.size() == 1);
          single_brs.push_back(br);
        }
        else if (br.status_val() == BrStatus::branching) {
          tau_brs.push_back(br);
        }
      }

      if (brk) branchings = {best_br};
      else if (not single_brs.empty()) branchings = single_brs;
      else branchings = best_branchings(tau_brs);
      assert(not branchings.empty());

      [[maybe_unused]] const auto var = branchings[0].var;
      assert(var >= 0 and var >= start and not x[var].assigned());
      const Timing::Time_point t1 = timing();
      global_stat.update_choice_stat(t1-t0);
      return new ArrayBranchingChoice<LookaheadLazyEqValOneSln>(*this, branchings);
    }

    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      return new BranchingChoice<LookaheadLazyEqValOneSln>(*this);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      typedef ArrayBranchingChoice<LookaheadLazyEqValOneSln> ArrBrChoice;
      const ArrBrChoice& brc = static_cast<const ArrBrChoice&>(c);
      const auto& branchings = brc.branchings;
      for (auto& br : branchings) {
        const auto status = br.status;
        if (status == BrStatus::unsat) {
          ++global_stat.unsat_leaves; return GC::ES_FAILED;
        }
        const auto var = br.var;
        const auto& values = br.values;
        const auto& eq_values = br.eq_values;
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
    //const BrSolutionO brsln = std::get<BrSolutionO>(options);
    const BrEagernessO bregr = std::get<BrEagernessO>(options);
    const IntViewArray y(home, V);
    if (brt == BrTypeO::mind and brsrc == BrSourceO::eq) {
      MinDomMinValEq::post(home, y); }
    else if (brt == BrTypeO::mind and brsrc == BrSourceO::val) {
      MinDomValue::post(home, y);
    }
    else if (brt == BrTypeO::mind and brsrc == BrSourceO::eqval) {
      MinDomMinValEq::post(home, y);
    }
    else if (brt == BrTypeO::la) {
      measure_t measure = (brm == BrMeasureO::mu0) ? mu0 : mu1;
      if (brsrc == BrSourceO::eq) {
        LookaheadEq<ModSpace>::post(home, y, measure, bregr);
      }
      else if (brsrc == BrSourceO::val) {
        LookaheadValue<ModSpace>::post(home, y, measure, bregr);
      }
      else if (brsrc == BrSourceO::eqval and bregr == BrEagernessO::eager) {
        LookaheadEagerEqValOneSln<ModSpace>::post(home, y, measure);
      }
      else if (brsrc == BrSourceO::eqval and bregr == BrEagernessO::lazy) {
        LookaheadLazyEqValOneSln<ModSpace>::post(home, y, measure);
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
    global_stat.gecode_stat = e.statistics();
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
    global_stat.gecode_stat = e.statistics();
  }
  template <class ModSpace>
  Statistics::SearchStat solve(const std::shared_ptr<ModSpace> m,
                               const bool printsol = false) noexcept {
    assert(m->valid());
    global_stat.reset();
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
