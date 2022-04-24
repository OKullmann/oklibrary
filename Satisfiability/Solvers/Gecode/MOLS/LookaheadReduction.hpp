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

0. Do not find single-child branchings of the kind var=val.
    - Only var!=val are collected and applied.
    - If all values but one (say, val) for a variable var are inconsistent,
      then var=val will be assigned automatically during a Gecode propagation.

1. Super-eager reduction.
    - Restart the main loop (over all variables) after any propagation.
    - It is very similar to the eager reduction - an additional break in the
      main loop is needed. So maybe add a Boolean variable that will switch
      between the eager and super-eager reductions.

2. Lookahead-reduction statistics.
    - An object is created in the choice() function of a customised brancher
      and passed to a reduction function.
    - In a reduction function, all main data should be collected
      and saved to the object.
    - The object is returned to the choice() function.

3. Find all solutions in a lookahead reduction function.
    - All solutions which are found during the reduction, are collected
      and saved to the local statistics' object.
    - The corresponding branches are cut off so Gecode is now aware of any
      found solutions.

4. Pruning.
    - Should work in all types of reductions.


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
    BranchingStatus st;
    int var;
    values_t values;
    bool valid() const noexcept { return var >= 0; }
    ReduceRes() : st(BranchingStatus::branching), var(0), values{} {}
    ReduceRes(const BranchingStatus st=BranchingStatus::branching, const int var=0,
              const values_t values={}) :
      st(st), var(var), values(values) {}

    void update_status(const BranchingStatus st_) noexcept {
      st = st_; assert(valid());
    };
    BranchingStatus status() const noexcept { assert(valid()); return st; }
  };

  // Lookahead-reduction.
  // Consider a variable var and its domain {val1, ..., valk}.
  // For all i, if var==vali is inconsistent, then these constraints
  // (var==vali) are collected and then (after the loop for all i) the
  // corresponding constraints are applied and a Gecode propagation is
  // performed. In such a way, all impossible values of a variable are removed.
  template<class ModSpace>
  ReduceRes lareduction(GC::Space& home, const IntViewArray x,
                        const int start) noexcept {
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
            return ReduceRes(BranchingStatus::sat, var, {values[0]});
          }
        }
      } // for (int var = start; var < x.size(); ++var) {
    } while (reduction);

    return ReduceRes(BranchingStatus::branching);
  }

}

#endif
