// Oleg Zaikin, 29.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

Look-ahead reduction for the Gecode library.

BUGS:

TODOS:

0. Super-eager reduction:
    - Restart the main loop (over all variables) after finding any
      single-child branching (that leads to assinging the variable).
    - It is very similar to the eager reduction - an additional break in the
      main loop is needed. So maybe add a Boolean variable that will switch
      between the eager and super-eager reductions.

1. Lazy reduction.
    - Collect all single-child branchings in the main loop, then apply
      them in one batch.

2. Transfer pruning from Euler's Lookahead.hpp.
    - Should work in all types of reductions.

3. Reduction for the enumerative mode.
    - If a solution is found, it can not be just immediately returned. All
      remaining branches should be collected to not skip possible other
      solutions.

4. The following reduction functions should be implemented:
    - DONE reduction_sat_eager - eager and super-eager sat-solving reduction.
    - reduction_sat_lazy - lazy sat-solving reduction.
    - reduction_enum_eager - eager and super-eager enumerative reduction.
    - reduction_enum_lazy - lazy enumerative reduction.

*/

#ifndef LOOKAHEADREDUCTION_fJ5peeEDiH
#define LOOKAHEADREDUCTION_fJ5peeEDiH

#include <vector>
#include <memory>

#include <cassert>


#include <gecode/int.hh>
#include <gecode/search.hh>

namespace LookaheadReduction {

  //namespace FP = FloatingPoint;
  namespace GC = Gecode;

  // Array of values of an integer variable:
  typedef GC::Int::IntView IntView;
  // Array of array of values of Gecode integer variables:
  typedef GC::ViewArray<IntView> IntViewArray;
  // Value iterator for an integer variable:
  typedef GC::IntVarValues IntVarValues;

  typedef std::vector<int> values_t;

  enum class BranchingStatus { unsat=0, sat=1, single=2, branching=3 };

  // Make a copy of a given problem and assign either var==val or var!=val:
  template<class ModSpace>
  std::unique_ptr<ModSpace> subproblem(ModSpace* const m, 
                                       const int v, const int val,
                                       const bool eq = true) noexcept {
    assert(m->valid());
    assert(m->valid(v));
    assert(m->status() == GC::SS_BRANCH);
    // Clone space:
    std::unique_ptr<ModSpace> c(static_cast<ModSpace*>(m->clone()));
    assert(c->valid());
    assert(c->valid(v));
    assert(c->status() == GC::SS_BRANCH);
    // Add an equality constraint for the given variable and its value:
    if (eq) GC::rel(*(c.get()), (c.get())->var(v), GC::IRT_EQ, val, GC::IPL_DOM);
    else GC::rel(*(c.get()), (c.get())->var(v), GC::IRT_NQ, val, GC::IPL_DOM);
    return c;
  }

  // Result of lookahead-reduction:
  struct ReduceRes {
    int var;
    values_t values;
    BranchingStatus st;
    bool valid() const noexcept { return var >= 0; }
    ReduceRes() : var(0), values{}, st(BranchingStatus::branching) {}
    ReduceRes(const BranchingStatus st=BranchingStatus::branching, const int var=0,
              const values_t values={}) :
      var(var), values(values), st(st) {}

    void update_status(const BranchingStatus st_) noexcept {
      st = st_; assert(valid());
    };
    BranchingStatus status() const noexcept { assert(valid()); return st; }
  };

  // A sat-solving-oriented eager lookahead-reduction.
  // It is applied when either sat-decision or sat-solving mode is active
  // (Run-Type is 0 or 1, see Options.hpp).
  // Consider a variable var and its domain {val1, ..., valk}.
  // If for some i var==vali is inconsistent, then var!=vali is collected and
  // then (after the loop for all i) the corresponding constraints are applied
  // and a Gecode propagation is performed. In such a way, all impossible
  // values of a variable are removed.
  // If after excluding all impossible values for varialbe var it turns out,
  // that the domain has size 1, say {vali}, then a single-child branching of
  // the kind var==vali is found. The assignment is immediately applied and a
  // Gecode propagation is performed.
  //
  // Possible results are:
  //  1) a solution is found (SAT).
  //  2) the unsatisfiability of the whole problem is proven (UNSAT);
  //  3) the backtracking tree is reduced in such a way, that no single-child
  //     branching occurs, so everything is ready for calling a
  //     lookahead-branching.
  template<class ModSpace>
  ReduceRes reduction_sat_eager(GC::Space& home, const IntViewArray x,
                                const int start) {
    assert(start < x.size());
    ModSpace* m = &(static_cast<ModSpace&>(home));
    assert(m->status() == GC::SS_BRANCH);
    bool reduction = false;
    do {
      reduction = false;
      // Iterate over all unassigned variables:
      for (int var = start; var < x.size(); ++var) {
        const IntView view = x[var];
        if (view.assigned()) continue;
        assert(view.size() >= 2);
        values_t values;
        // All such val that var!=val:</font>
        values_t noteqvalues;

        // Iterate over all values of the current variable:
        for (IntVarValues j(view); j(); ++j) {
          const int val = j.val();
          assert(m->status() == GC::SS_BRANCH);
          // Make a copy of the current problem, and assign var==val:
          const auto subm = subproblem<ModSpace>(m, var, val, true);
          // Call Gecode propagation:
          const auto status = subm->status();
          // If a solution if found, return it immediately:
          if (status == GC::SS_SOLVED) {
            return ReduceRes(BranchingStatus::sat, var, {val});
          }
          // If the assignment var==val is inconsistent, then var!=val:
          else if (status == GC::SS_FAILED) noteqvalues.push_back(val);
          // The assignment var==val is relatively inconsistent,
          // i.e. it is not clear whether it is inconsistent or not:
          else values.push_back(val);
        }

        // No branches, so the problem is unsatisfiable:
        if (values.empty()) {
          return ReduceRes(BranchingStatus::unsat);
        }
        // If single-child branching:
        else if (values.size() == 1) {
          reduction = true;
          GC::rel(home, x[var], GC::IRT_EQ, values[0], GC::IPL_DOM);
          const auto status = home.status();
          if (status == GC::SS_FAILED) return ReduceRes(BranchingStatus::unsat);
          else if (status == GC::SS_SOLVED) return ReduceRes(BranchingStatus::sat, var, values);
        }
        // None from above - non-sat, non-unsat, at least 2 branches.
        // Apply all var!=val assignments in one batch:
       if (not noteqvalues.empty()) {
          reduction = true;
          for (auto& noteqval : noteqvalues) {
            GC::rel(home, x[var], GC::IRT_NQ, noteqval, GC::IPL_DOM);
          }
          // Call a propagation:
          const auto status = home.status();
          // Check if the problem is solved:
          if (status == GC::SS_FAILED) {
            return ReduceRes(BranchingStatus::unsat);
          }
          else if (status == GC::SS_SOLVED) {
            assert(not values.empty());
            return ReduceRes(BranchingStatus::sat, var, values[0]);
          }
        }
      } // for (int var = start; var < x.size(); ++var) {
    } while (reduction);

    return ReduceRes(BranchingStatus::branching);
  }

  template<class ModSpace>
  ReduceRes reduction_enum_eager() {
    ReduceRes res;
    // XXX
    return res;
  }

}

#endif
