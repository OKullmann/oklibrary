// Oleg Zaikin, 29.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Look-ahead reduction for the Gecode library

  Namespace LookaheadReduction, abbreviated "LR".


BUGS:

TODOS:


*/

#ifndef LOOKAHEADREDUCTION_fJ5peeEDiH
#define LOOKAHEADREDUCTION_fJ5peeEDiH

#include <vector>
#include <memory>
#include <set>
#include <algorithm>
#include <utility>
#include <array>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <SystemSpecifics/Timing.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Conditions.hpp"
#include "Options.hpp"
#include "GcVariables.hpp"

namespace LookaheadReduction {

  namespace FP = FloatingPoint;
  namespace GC = Gecode;
  namespace OP = Options;
  namespace CD = Conditions;
  namespace GV = GcVariables;

  using size_t = CD::size_t;
  typedef FP::float80 float_t;

  // Statistics of the main lookahead-reduction actions:
  struct ReductionStatistics {
    typedef std::vector<GV::solutions_t> sollist_t;
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

    size_t vals() const noexcept { return vals_; }
    size_t props() const noexcept { return props_; }
    size_t elimvals() const noexcept { return elimvals_; }
    size_t prunes() const noexcept { return prunes_; }
    size_t maxprune() const noexcept { return maxprune_; }
    size_t probes() const noexcept { return probes_; }
    size_t rounds() const noexcept { return rounds_; }
    size_t solc() const noexcept { return solc_; }
    size_t leafcount() const noexcept { return leafcount_; }

    void sollist(const GV::solutions_t x) { sollist_.push_back(x); }
    const sollist_t& sollist() const noexcept { return sollist_; }
    sollist_t& sollist() noexcept { return sollist_; }

    void maxprune(const size_t size) noexcept {
      maxprune_ = std::max(maxprune_, size);
    }
    ReductionStatistics& time(const Timing::Time_point t) noexcept {
      time_ = t; return *this;
    }
    Timing::Time_point time() const noexcept { return time_; }

    float_t quotelimvals() const noexcept {return  float_t(elimvals_)/vals_;}
    float_t quotprun() const noexcept {return float_t(prunes_)/probes_;}

    static constexpr size_t num_stats = 12;
    typedef std::array<float_t, num_stats> export_t;
    export_t extract() const noexcept {
      export_t res;
      res[0] = vals_; res[1] = props_; res[2] = elimvals_;
      res[3] = prunes_; res[4] = maxprune_; res[5] = probes_;
      res[6] = rounds_; res[7] = solc_; res[8] = leafcount_;
      res[9] = time_; res[10] = quotelimvals(); res[11] = quotprun();
      return res;
    }

    bool operator ==(const ReductionStatistics&) const noexcept = default;
    // Equality without time:
    friend bool eqwt(ReductionStatistics lhs, ReductionStatistics rhs) noexcept {
      return lhs.time(0) == rhs.time(0);
    }
  };


  // Make a copy of a given problem and assign either var==val or var!=val:
  template <class SPA>
  std::unique_ptr<SPA> child_node(SPA* const m,
                                       const int v, const int val,
                                       const GC::IntPropLevel pl) noexcept {
    assert(v >= 0 and v < m->V.size());
    std::unique_ptr<SPA> c(static_cast<SPA*>(m->clone()));
    assert(c->V.size() == m->V.size());
    GC::rel(*c.get(), c.get()->V[v], GC::IRT_EQ, val, pl);
    return c;
  }


  typedef std::pair<int, int> plit_t;
  typedef std::set<plit_t> pruning_table_t;

  template <class SPA>
  GC::SpaceStatus probe(SPA* const m,
                        const int v, const int val,
                        const GC::IntPropLevel pl,
                        ReductionStatistics& stats,
                        const bool with_sols) noexcept {
    assert(m->V.size() > 0 and v < m->V.size());
    const auto chnode = child_node<SPA>(m, v, val, pl);
    const auto status = chnode->status();
    if (status == GC::SS_SOLVED) {
      stats.inc_solc(); if (with_sols) stats.sollist(GV::extract(chnode->V));
    }
    return status;
  }
  template <class SPA>
  GC::SpaceStatus probe(SPA* const m,
                        const int v, const int val,
                        const GC::IntPropLevel pl,
                        pruning_table_t& PV,
                        ReductionStatistics& stats,
                        const bool with_sols) noexcept {
    assert(v >= 0 and v < m->V.size());
    const auto V0 = m->V;
    const auto chnode = child_node<SPA>(m, v, val, pl);
    const auto status = chnode->status();
    if (status == GC::SS_SOLVED) {
      stats.inc_solc(); if (with_sols) stats.sollist(GV::extract(chnode->V));
    }
    if (status != GC::SS_BRANCH) return status;
    const auto V1 = chnode->V;
    assert(V0.size() == V1.size());
    for (int v = 0; v < V1.size(); ++v)
      if (V1[v].size() == 1 and V0[v].size() > 1)
        for (GC::IntVarValues i(V1[v]); i(); ++i) PV.insert({v, i.val()});
    return status;
  }


  // Lookahead-reduction:
  // Consider a variable var and its domain {val1, ..., valk}.
  // For all i, the constraints (var!=vali), where var==vali leads to a
  // decision (via propagation), are collected. After the loop over all i,
  // these constraints are applied and the Gecode propagation is
  // performed. So all immediate decisions of all variable are
  // removed.
  template <class SPA>
  ReductionStatistics lareduction(SPA* const m,
                        const OP::RT rt,
                        const GC::IntPropLevel pl,
                        const OP::LAR lar) noexcept {
    ReductionStatistics stats(m->V);
    Timing::UserTime timing;
    const Timing::Time_point t0 = timing();
    pruning_table_t PT;

    {int last_red = -1;
    do {
      stats.inc_rounds();
      const GC::IntVarArray V = m->V;
      for (int v = 0; v < V.size(); ++v) {
        if (v == last_red) {
          if (eager(lar)) {last_red = -1; continue;}
          else goto END;
        }
        const auto vsize = V[v].size();
        if (vsize == 1) continue;

        const GV::values_t values = GV::values(V, v);
        GV::values_t elimvals;

        pruning_table_t PV;
        for (const int val : values) {
          if (pruning(lar) and PT.contains({v,val})) {
            stats.inc_prunes(); continue;
          }
          const auto status = pruning(lar) ?
            probe(m, v, val, pl, PV, stats, with_solutions(rt)) :
            probe(m, v, val, pl,     stats, with_solutions(rt));
          stats.inc_probes();
          if (status != GC::SS_BRANCH) {
            assert(status == GC::SS_SOLVED or status == GC::SS_FAILED);
            stats.inc_elimvals(); elimvals.push_back(val);
            if (status == GC::SS_SOLVED and
                (rt == OP::RT::sat_decision or rt == OP::RT::sat_solving
                 or (test_unique(rt) and stats.solc() >= 2))) {
              stats.inc_leafcount();
              goto END;
            }
          }
        }

        if (elimvals.empty()) PT.merge(PV);
        else {
          [[maybe_unused]] const auto esize = elimvals.size();
          assert(esize <= vsize);
          last_red = v;
          for (const int val : elimvals) GC::rel(*m,V[v],GC::IRT_NQ,val,pl);
          const auto status = m->status();
          // assert(V[v].size() == vsize - esize); // ???
          stats.inc_props();
          assert(status != GC::SS_SOLVED);
          if (status != GC::SS_BRANCH) {
            assert(status==GC::SS_FAILED and elimvals.size()==values.size());
            stats.inc_leafcount();
            goto END;
          }
          stats.maxprune(PT.size()); PT = std::move(PV);
          if (eager(lar)) break;
        }
      }
    } while (last_red >= 0);}

    END:
    stats.maxprune(PT.size());
    return stats.time(timing() - t0);
  }

}

#endif
