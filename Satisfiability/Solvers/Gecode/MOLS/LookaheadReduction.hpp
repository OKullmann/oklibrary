// Oleg Zaikin, 29.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

Look-ahead reduction for the Gecode library.

1. A loop over all variables v is run.
2. If the domain of v is unit, the variable is skipped, otherwise a loop over
its values eps is run.
3. The assignment v=eps is probed, that is, the given propagation is performed
(in a "clone"), if pruning is not applicable.
4. There are three possible outcomes of this probing:
   (a) a satisfying assigment was found;
   (b) a contradiction was reached;
   (c) none of these two.
5. In case of sat-decision/solving, in case (a) the computation is
appropriately stopped (but with statistics completed). Otherwise for (a) the
assignment found is stored in a queue.
6. For both (a) and (b), the constraint v != eps is posted.
7. For (c), one inspects the propagation and determines all assignments
v'=eps', and enters them into the pruning-set (with the current counter-value).
8. However, if for v already one case of excluded value ((a) or (b)) happened,
then this is skipped (since superfluous).
9. Once v is completed, and at least one case of (a) or (b) was found, the
counter is incremented, and propagation is triggered.
10. If one reaches the end of the loop over all variables, in case of
super-eager that concludes the reduction, while otherwise one needs to check
whether the counter is larger than its previous value (for the last round), and
if so, repeating the loop.

BUGS:

TODOS:

0. Lookahead-reduction statistics.
    - The reduction-statistics is used in the choice() function of a customised
      brancher to update the global statistics.
    - Statistics collected here (the "important events"):
      - DONE the propagation-counter
      - DONE the number of eliminated values
      - DONE the quotient eliminated-values / all-values
      - DONE the number of successful prunings
      - DONE the number of probings
      - DONE the quotient prunings/probings
      - DONE the number of rounds
      - DONE (not "final", but maximal) the final size of the pruning-set
      - DONE the total time for the reduction
      - DONE the number of satisfying assignments found.
    - DONE satisfying assignments.

1. DONE Collect satisfying assignments, if needed.
    - Use a queue.

2. DONE Pruning:
     - DONE
       Type PLit ("positive literal"), a pair (var,val), meaning "var=val",
       should be basic here.
     - DONE
       Maintain std::set<PLit>, where in case of probing without decision
       one enters the literals "v'=eps'" (and the check for pruning is just
       the check whether the set contains the literal v=eps), while in case of
       propagation the set is just cleared.
     - DONE
       So the function probe(m,v,val,pl) needs to determine the positive
       literals v=eps derived by the probing.
     - DONE
       probe should store the current variable-restriction on entry, and
       compare that with the variable-restriction in case chnode->status()
       did not yield a decision.
     - DONE
       How to return the set of positive literals? Perhaps best providing
       the set of positive literals as reference-parameter.
     - DONE
       After one branch found a restriction, for this variable there is no
       point in updating further the pruning-set.

*/

#ifndef LOOKAHEADREDUCTION_fJ5peeEDiH
#define LOOKAHEADREDUCTION_fJ5peeEDiH

#include <vector>
#include <memory>
#include <queue>
#include <set>
#include <algorithm>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <SystemSpecifics/Timing.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Conditions.hpp"
#include "Options.hpp"

namespace LookaheadReduction {

  namespace FP = FloatingPoint;
  namespace GC = Gecode;
  namespace OP = Options;
  namespace CD = Conditions;

  using size_t = CD::size_t;
  typedef FP::float80 float_t;

  // Array of values of an integer variable:
  typedef GC::Int::IntView IntView;
  // Array of array of values of Gecode integer variables:
  typedef GC::ViewArray<IntView> IntViewArray;
  // Value iterator for an integer variable:
  typedef GC::IntVarValues IntVarValues;

  typedef std::vector<int> values_t;

  // Statistics of the main lookahead-reduction actions:
  struct ReductionStatistics {
    typedef std::queue<GC::IntVarArray> sollist_t;
  private :
    size_t vals_; // the total number of values
    Timing::Time_point time_; // the total time for the reduction

    size_t props_ = 0; // propagation-call-counter
    size_t elimvals_ = 0; // number of eliminated values
    size_t probes_ = 0; // number of probings
    size_t rounds_ = 0; // number of rounds
    size_t prunes_ = 0; // number of successful prunings
    size_t maxprune_ = 0; // maximal size of pruning-set
    size_t solc_ = 0; // number of solutions found
    size_t leafcount_ = 0; // number of leafs as a result of reduction (0 or 1)

    sollist_t sollist_; // list of solutions found

  public:

    explicit ReductionStatistics(const GC::IntVarArray& x) noexcept :
      vals_(count_values(x)) {}
    static size_t count_values(const GC::IntVarArray& x) noexcept {
      assert(x.size() > 0);
      size_t sum = 0;
      for (int v = 0; v < x.size(); ++v) sum += x[v].size();
      assert(sum > 0); return sum;
    }

    void inc_props() noexcept { ++props_; }
    void inc_elimvals() noexcept { ++elimvals_; }
    void inc_prunes() noexcept { ++prunes_; ;}
    void inc_probes() noexcept { ++probes_; }
    void inc_rounds() noexcept { ++rounds_; }
    void inc_solc() noexcept { ++solc_; }
    void inc_leafcount() noexcept { assert(!leafcount_); ++leafcount_; }

    size_t props() const noexcept { return props_; }
    size_t elimvals() const noexcept { return elimvals_; }
    size_t prunes() const noexcept { return prunes_; }
    size_t maxprune() const noexcept { return maxprune_; }
    size_t probes() const noexcept { return probes_; }
    size_t rounds() const noexcept { return rounds_; }
    size_t solc() const noexcept { return solc_; }
    size_t leafcount() const noexcept { return leafcount_; }

    void sollist(const GC::IntVarArray& x) { sollist_.push(x); }
    const sollist_t& sollist() const noexcept { return sollist_; }

    void maxprune(const size_t size) noexcept {
      maxprune_ = std::max(maxprune_, size);
    }
    void time(const Timing::Time_point t) noexcept { time_ = t; }
    Timing::Time_point time() const noexcept { return time_; }

    float_t quotelimvals() const noexcept {return  float_t(elimvals_)/vals_;}
    float_t quotprun() const noexcept {return float_t(prunes_)/probes_;}
  };


  // Make a copy of a given problem and assign either var==val or var!=val:
  template<class ModSpace>
  std::unique_ptr<ModSpace> child_node(ModSpace* const m,
                                       const int v, const int val,
                                       const GC::IntPropLevel pl,
                                       const bool eq = true) noexcept {
    assert(m->V.size() > 0 and v < m->V.size());
    std::unique_ptr<ModSpace> c(static_cast<ModSpace*>(m->clone()));
    assert(c->V.size() > 0 and v < c->V.size());
    GC::rel(*c.get(), c.get()->V[v], eq?GC::IRT_EQ:GC::IRT_NQ, val, pl);
    return c;
  }

  typedef std::pair<int, int> plit_t;
  typedef std::set<plit_t> pruning_table_t;

  template<class ModSpace>
  GC::SpaceStatus probe(ModSpace* const m,
                        const int v, const int val,
                        const GC::IntPropLevel pl) noexcept {
    assert(m->V.size() > 0 and v < m->V.size());
    const auto chnode = child_node<ModSpace>(m, v, val, pl, true);
    return chnode->status();
  }
  template<class ModSpace>
  GC::SpaceStatus probe(ModSpace* const m,
                        const int v, const int val,
                        const GC::IntPropLevel pl,
                        pruning_table_t& PT) noexcept {
    assert(m->V.size() > 0 and v < m->V.size());
    const auto V0 = m->V;
    const auto chnode = child_node<ModSpace>(m, v, val, pl, true);
    const auto status = chnode->status();
    if (status != GC::SS_BRANCH) return status;
    const auto V1 = chnode->V;
    assert(V0.size() == V1.size());
    for (int v = 0; v < V1.size(); ++v)
      if (V1[v].size() == 1 and V0[v].size() > 1)
        for (GC::IntVarValues i(V1[v]); i(); ++i) PT.insert({v, i.val()});
    return status;
  }

  // Lookahead-reduction:
  // Consider a variable var and its domain {val1, ..., valk}.
  // For all i, the constraints (var!=vali), where var==vali leads to a
  // decision (via propagation), are collected. After the loop over all i,
  // these constraints are applied and the Gecode propagation is
  // performed. So all immediate decisions of all variable are
  // removed.
  template<class ModSpace>
  ReductionStatistics lareduction(ModSpace* const m,
                        const OP::RT rt,
                        const GC::IntPropLevel pl,
                        const OP::LAR lar) noexcept {
    ReductionStatistics stat(m->V);
    Timing::UserTime timing;
    const Timing::Time_point t0 = timing();
    bool repeat = false;
    pruning_table_t PT;
    do {
      repeat = false;
      stat.inc_rounds();
      const GC::IntVarArray x = m->V;
      for (int var = 0; var < x.size(); ++var) {
        const IntView view = x[var];
        if (view.assigned()) continue;
        assert(view.size() >= 2);

        const values_t values = [&view]
          {values_t res;
           for (IntVarValues j(view); j(); ++j) res.push_back(j.val());
           return res;}();
        values_t elimvals;
        for (const auto val : values) {
          if (pruning(lar) and PT.contains({var,val})) {
            stat.inc_prunes(); continue;
          }
          const auto status = pruning(lar) and elimvals.empty() ?
            probe(m, var, val, pl, PT) : probe(m, var, val, pl);
          stat.inc_probes();
          if (status != GC::SS_BRANCH) {
            assert(status == GC::SS_SOLVED or status == GC::SS_FAILED);
            stat.inc_elimvals(); elimvals.push_back(val);
            if (status == GC::SS_SOLVED) {
              if (with_solutions(rt)) stat.sollist(m->V);
              stat.inc_solc();
              if (rt == OP::RT::sat_decision or rt == OP::RT::sat_solving
                  or (test_unique(rt) and stat.solc() >= 2))
                goto END;
            }
          }
        }

        if (not elimvals.empty()) {
          stat.maxprune(PT.size()); PT.clear();
          for (const int val : elimvals)
            GC::rel(*m, x[var], GC::IRT_NQ, val, pl);
          const auto status = m->status();
          stat.inc_props();
          if (status != GC::SS_BRANCH) {
            assert(status == GC::SS_SOLVED or status == GC::SS_FAILED);
            stat.inc_leafcount();
            if (status == GC::SS_SOLVED) {
              if (with_solutions(rt)) stat.sollist(m->V);
              stat.inc_solc();
            }
            goto END;
          }
          if (eager(lar)) { assert(not repeat); break; }
          else repeat = true;
        }
      }
    } while (repeat);

    END: const Timing::Time_point t1 = timing(); stat.time(t1 - t0);
    return stat;
  }

}

#endif
