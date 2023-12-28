// Oleg Zaikin, 29.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Look-ahead reduction for the Gecode library

  Namespace LookaheadReduction, abbreviated "LR".

   - using size_t

   - typedefs:
    - float_t for float80
    - sollist_t for a vector of solutions from GV
    - lit_t for a pair of int
    - assignment_t for a vector of lit_t
    - pruning_table_t for a set of lit_t

   - class ReductionStatistics for the results of one reduction

   - function child_node(m, v, val): making a copy of node m,
     and assigning there v=val (without propagation)
   - function probe(m, v, val, stats, with_sols): probes v=val,
     returning the statistics in stats (including satisfying
     assignments)
     - version probe(m, v, val, PV, stats, with_sols) updates the
       pruning table

   - function lareduction(m, rt, lar) performs la-reduction for m,
     and returns ReductionStatistics.


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
#include <string>
#include <exception>
#include <ostream>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <SystemSpecifics/Timing.hpp>
#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Strings.hpp>

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

  typedef std::vector<GV::solutions_t> sollist_t;
  typedef std::pair<int, int> lit_t;
  typedef std::vector<lit_t> assignment_t;


  // Statistics of the main lookahead-reduction actions:
  class ReductionStatistics {
    const size_t vals_; // measure mu0
    const size_t depth_;
    Timing::Time_point time_; // total time for the reduction

    size_t props_ = 0; // propagation-call-counter
    size_t elimvals_ = 0; // number of eliminated values
    size_t probes_ = 0; // number of probings
    size_t rounds_ = 0; // number of rounds
    size_t prunes_ = 0; // number of successful prunings
    size_t maxprune_ = 0; // maximal size of pruning-set
    size_t elimfv_ = 0; // number of formal variables

    size_t solc_ = 0; // number of solutions found
    bool leaf_ = false;

    sollist_t sollist_; // list of solutions found
    assignment_t elims_; // list of eliminations (where variable != value)

  public:

    ReductionStatistics(const GC::IntVarArray& x,
                        const size_t d) noexcept :
      vals_(GV::sumdomsizes(x)-x.size()), depth_(d) { assert(vals_ > 0); }
    ReductionStatistics(const size_t vals,
                        const size_t d) :
      vals_(vals), depth_(d) { assert(vals_ > 0); }

    void inc_props() noexcept { ++props_; }
    void inc_elimvals() noexcept { ++elimvals_; }
    void inc_prunes() noexcept { ++prunes_; ;}
    void inc_probes() noexcept { ++probes_; }
    void inc_rounds() noexcept { ++rounds_; }
    void inc_elimfv() noexcept { ++elimfv_; }

    void inc_solc() noexcept { ++solc_; }
    void set_leaf() noexcept { assert(!leaf_); leaf_ = true; }

    size_t vals() const noexcept { return vals_; }
    size_t depth() const noexcept { return depth_; }
    size_t props() const noexcept { return props_; }
    size_t elimvals() const noexcept { return elimvals_; }
    size_t prunes() const noexcept { return prunes_; }
    size_t maxprune() const noexcept { return maxprune_; }
    size_t probes() const noexcept { return probes_; }
    size_t rounds() const noexcept { return rounds_; }
    size_t elimfv() const noexcept { return elimfv_; }

    size_t solc() const noexcept { return solc_; }
    bool leaf() const noexcept { return leaf_; }

    ReductionStatistics& sollist(const GV::solutions_t x) {
      sollist_.push_back(x); return *this;
    }
    const sollist_t& sollist() const noexcept { return sollist_; }
    sollist_t& sollist() noexcept { return sollist_; }

    ReductionStatistics& elim(const lit_t x) {
      elims_.push_back(x); return *this;
    }
    assignment_t& elims() noexcept { return elims_; }

    void maxprune(const size_t size) noexcept {
      maxprune_ = std::max(maxprune_, size);
    }
    ReductionStatistics& time(const Timing::Time_point t) noexcept {
      time_ = t; return *this;
    }
    Timing::Time_point time() const noexcept { return time_; }

    float_t quotprops() const noexcept {
      return props_ == 0 ? 0 : float_t(elimvals_) / props_;
    }
    float_t quotprune() const noexcept {
      return 100 * float_t(prunes_) / probes_;
    }
    float_t quotmaxprune() const noexcept {
      return 100 * float_t(maxprune_) / vals_;
    }
    float_t quotprobes() const noexcept {
      return 100 * float_t(probes_) / vals_;
    }
    float_t quotelimvals() const noexcept {
      return 100 * float_t(elimvals_) / vals_;
    }

    static constexpr size_t num_stats = 11;
    typedef std::array<float_t, num_stats> export_t;
    export_t extract() const noexcept {
      export_t res;
      res[0] = vals_; res[1] = quotprops(); res[2] = quotprune();
      res[3] = quotmaxprune(); res[4] = quotprobes(); res[5] = rounds_;
      res[6] = solc_; res[7] = time_; res[8] = quotelimvals();
      res[9] = depth_;
      res[10] = elimfv_;
      return res;
    }
    typedef std::vector<std::string> header_t;
    static header_t stats_header() noexcept {
      return {"mu0", "qfppc", "pprunes", "pmprune", "pprobes", "rounds",
          "solc", "tr", "pelvals", "dp", "efv"};
      // leading "p": percentage, "q": quotient;
      // "f" in "qfppc" for "full".
    }
    static size_t index(const std::string& s) {
      const auto words = stats_header();
      const auto find = std::ranges::find(words, s);
      if (find != words.end()) return find - words.begin();
      throw std::runtime_error("ERROR[LR::index]: unknown variable \""
                               + s + "\"\n");
    }

    bool operator ==(const ReductionStatistics&) const noexcept = default;
    // Equality without time:
    friend bool eqwt(ReductionStatistics lhs, ReductionStatistics rhs)
      noexcept {
      return lhs.time(0) == rhs.time(0);
    }
  };
  std::ostream& operator <<(std::ostream& out, const ReductionStatistics& s) {
    Environment::out_line(out, s.extract());
    return out;
  }


  // Make a copy of a given problem and assign var==val:
  template <class SPA>
  std::unique_ptr<SPA> child_node(SPA* const m,
                                       const int v, const int val) noexcept {
    assert(v >= 0 and v < m->V.size());
    std::unique_ptr<SPA> c(static_cast<SPA*>(m->clone()));
    assert(c->V.size() == m->V.size());
    GV::set_var(*c.get(), c.get()->V[v], val);
    return c;
  }


  typedef std::set<lit_t> pruning_table_t;

  template <class SPA>
  GC::SpaceStatus probe(SPA* const m,
                        const int v, const int val,
                        ReductionStatistics& stats,
                        const bool with_sols) noexcept {
    assert(m->V.size() > 0 and v < m->V.size());
    const auto chnode = child_node<SPA>(m, v, val);
    const auto status = chnode->status();
    if (status == GC::SS_SOLVED) {
      stats.inc_solc(); if (with_sols) stats.sollist(GV::extract(chnode->V));
    }
    return status;
  }
  template <class SPA>
  GC::SpaceStatus probe(SPA* const m,
                        const int v, const int val,
                        pruning_table_t& PV,
                        ReductionStatistics& stats,
                        const bool with_sols) noexcept {
    assert(v >= 0 and v < m->V.size());
    const auto V0 = m->V;
    const auto chnode = child_node<SPA>(m, v, val);
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
  // decision (via propagation), are aoplied, until fixed point.
  // "Decision" here means all constraints have been satisfied (via
  // a total assignment), or a contradiction was found. So finding
  // a satisfying assignment is treated as a failure, after storing
  // the solution (appropriately).
  template <class SPA>
  ReductionStatistics lareduction(SPA* const m,
                        const OP::RT rt,
                        const OP::RDL rdl, const OP::LAR lar) noexcept {
    ReductionStatistics stats(m->V, m->nodedata().depth);
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

        const auto vdeg = V[v].degree();
        if (vdeg == 0) {
          stats.inc_elimfv();
          if (efv(rdl)) {
            const int val = GV::first_value(V,v);
            GV::set_var(*m, V[v], val);
            const auto status = m->status();
            assert(status != GC::SS_FAILED);
            if (status == GC::SS_SOLVED) {
              stats.inc_solc();
              if (with_solutions(rt)) stats.sollist(GV::extract(V));
              stats.set_leaf();
              goto END;
            }
            assert(status == GC::SS_BRANCH);
            continue;
          }
        }

        const GV::values_t values = GV::values(V, v);
        GV::values_t elimvals;

        pruning_table_t PV;
        for (const int val : values) {
          if (pruning(lar) and PT.contains({v,val})) {
            stats.inc_prunes(); continue;
          }
          const auto status = pruning(lar) ?
            probe(m, v, val, PV, stats, with_solutions(rt)) :
            probe(m, v, val,     stats, with_solutions(rt));
          stats.inc_probes();
          if (status != GC::SS_BRANCH) { // eliminate val for v
            assert(status == GC::SS_SOLVED or status == GC::SS_FAILED);
            stats.inc_elimvals(); elimvals.push_back(val);
          }
        }

        if (elimvals.empty()) PT.merge(PV);
        else {
          [[maybe_unused]] const auto esize = elimvals.size();
          assert(esize <= vsize);
          last_red = v;
          for (const int val : elimvals) {
            GV::unset_var(*m, V[v], val);
            stats.elim({v,val});
          }
          const auto status = m->status();
          assert(status != GC::SS_SOLVED);
          assert(status==GC::SS_FAILED or V[v].size() == vsize - esize);
          stats.inc_props();
          if (status != GC::SS_BRANCH) {
            assert(status==GC::SS_FAILED and elimvals.size()==values.size());
            stats.set_leaf();
            goto END;
          }
          stats.maxprune(PT.size()); PT = std::move(PV);
          if (eager(lar)) break;
        }
      }
    } while (last_red >= 0);}

  END:
    stats.maxprune(PT.size());
    assert(stats.elims().size() == stats.elimvals());
    return stats.time(timing() - t0);
  }

}

#endif
