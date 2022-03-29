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

0. Transfer a lazy reduction from Euler's Lookahead.hpp.
1. Transfer pruning from Euler's Lookahead.hpp.

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

  enum class NodeStatus { unsat=0, sat=1, single=2, branching=3 };

  template<class ModSpace>
  std::unique_ptr<ModSpace> subproblem(ModSpace* const m, 
                                       const int v, const int val,
                                       const bool eq = true) noexcept {
    assert(m->valid());
    assert(m->valid(v));
    assert(m->status() == GC::SS_BRANCH);
    const Timing::UserTime timing;
    const Timing::Time_point t0 = timing();
    // Clone space:
    std::unique_ptr<ModSpace> c(static_cast<ModSpace*>(m->clone()));
    assert(c->valid());
    assert(c->valid(v));
    assert(c->status() == GC::SS_BRANCH);
    // Add an equality constraint for the given variable and its value:
    if (eq) GC::rel(*(c.get()), (c.get())->at(v), GC::IRT_EQ, val, GC::IPL_DOM);
    else GC::rel(*(c.get()), (c.get())->at(v), GC::IRT_NQ, val, GC::IPL_DOM);
    const Timing::Time_point t1 = timing();
    return c;
  }

  // Data for single child branching:
  struct SingleChildBranching {
    int var;
    int val;
    bool eq;
    bool valid const noexcept () { return var >= 0; }
    SingleChildBranching(const int var, const int val, const bool eq) :
      var(var), val(val), eq(eq) { assert(valid()); }
  };

  // Result of lookahead-reduction:
  struct ReduceRes {
    int var;
    values_t values;
    NodeStatus status;
    bool valid const noexcept () { return var >= 0; }
    ReduceRes() : var(0), values{}, status(NodeStatus::branching) {}
    ReduceRes(const int var, const values_t values, const NodeStatus status) :
      var(var), values(values), status(status) {}

    void update_status(const NodeStatus status_) noexcept {
      status = status_; assert(valid());
    };
    NodeStatus status() const noexcept { return status; assert(valid()); }
  };

  // An eager lookahead-reduction - once a single-child branching is found, it
  // is immediately applied by assigning a value and calling a Gecode-propagation.
  // Possible final states are:
  //  1) a new solution is found (a SAT leaf);
  //  2) Unsatisfiability of the whole problem is proven;
  //  3) a problem is reduced in such a way, that no single-child branching occur,
  //     so everything is ready for calling a lookahead-branching.
  template<class ModSpace>
  ReduceRes reduceEager(GC::Space& home, const IntViewArray x, const int start,
                        const bool eqbr=false) {
    assert(start < x.size());
    ReduceRes res;
    ModSpace* m = &(static_cast<ModSpace&>(home));
    assert(m->status() == GC::SS_BRANCH);
    bool reduction = false;
    do {
      reduction = false;
      for (int var = start; var < x.size(); ++var) {
        const IntView view = x[var];
        if (view.assigned()) continue;
        assert(view.size() >= 2);
        values_t values;
        std::vector<SingleChildBranching> singlechbrs;
        for (IntVarValues j(view); j(); ++j) {
          const int val = j.val();
          assert(m->status() == GC::SS_BRANCH);
          // Make a copy of the current problem, and assign var==val:
          const auto subm = subproblem<ModSpace>(m, var, val, true);
          // Call Gecode propagation:
          const auto status = subm->status();
          // If the assignment var==val is inconsistent, then var!=val:
          if (status == GC::SS_FAILED) {
            SingleChildBranching sch(var, val, false);
            singlechbrs.push_back(sch);
          }
          // The assignment var==val is relatively inconsistent,
          // i.e. it is not clear whether it is inconsistent or not:
          else {
            values.push_back(val);
            if (status == GC::SS_SOLVED) {
              if (eqbr) return ReduceRes(var, {val}, NodeStatus::sat);
              else res.update_status(NodeStatus::sat);
            }
          }
        }

        if (res.status() == NodeStatus::sat) {
          assert(not values.empty());
          return ReduceRes(var, values, NodeStatus::sat);
        }
        // No branches, so the problem is unsatisfiable:
        else if (values.empty()) {
          return ReduceRes(0, {}, NodeStatus::unsat);
        }
        // If single-child branching:
        else if (values.size() == 1) {
          reduction = true;
          GC::rel(home, x[var], GC::IRT_EQ, values[0], GC::IPL_DOM);
          const auto status = home.status();
          if (status == GC::SS_FAILED) return ReduceRes(0, {}, NodeStatus::unsat);
          else if (status == GC::SS_SOLVED) return ReduceRes(var, values, NodeStatus::sat);
        }
        // None from above - non-sat, non-unsat, at least 2 branches:
        else {
          if (not singlechbrs.empty()) {
            reduction = true;
          }
          for (auto& sch : singlechbrs) {
            assert(not sch.eq);
            // assign var!=val:
            GC::rel(home, x[sch.var], GC::IRT_NQ, sch.val, GC::IPL_DOM);
            const auto status = home.status();
            if (status == GC::SS_FAILED) return ReduceRes(0, {}, NodeStatus::unsat);
            else if (status == GC::SS_SOLVED) return ReduceRes(sch.var, {sch.val}, NodeStatus::sat);
          }
        }
      } // for (int var = start; var < x.size(); ++var) {
    } while (reduction);

    return res;
  }

}

#endif
