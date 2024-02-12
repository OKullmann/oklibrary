// Oleg Zaikin, 29.3.2022 (Swansea)
/* Copyright 2022, 2023, 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Look-ahead reduction for the Gecode library

  Namespace LookaheadReduction, abbreviated "LR".

   - using namespace-abbreviations FP, GC, OP, CD, GV

   - using size_t

   - typedefs:
    - float_t for float80
    - sollist_t for a vector of solutions from GV
    - lit_t for a pair of int ("variable != value")
    - assignment_t for a vector of lit_t
    - pruning_table_t for a set of lit_t

   - class ReductionStatistics for the results of one reduction

   - function child_node(m, v, val): making a copy of node m,
     and assigning there v=val (without propagation)
   - function probe(m, v, val, stats, with_sols): probes v=val,
     returning the statistics in stats (including satisfying
     assignments)
     - version probe(m, v, val, PV, stats, with_sols) updates the
       pruning table PV

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
#include "Measures.hpp" // XXX

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


  /* Statistics */


  // Statistics of the main lookahead-reduction actions:
  class ReductionStatistics {
    const size_t mu0_;
    const size_t numvars_; // number of unassigned variables
    const size_t numvals_; // number of values of unassigned variables
    const size_t depth_;

    Timing::Time_point time_; // total time for the reduction

    size_t props_ = 0; // status-call-counter (one per variable which has
                       // some eliminated values)
    size_t elimvals_ = 0; // number of eliminated values for the la-variable
    /* props_ and elimvals_ only consider the la-variable, not further
       inferred eliminated values (via the status-call).
       While quotelimvals (for "pelvals") below considers all variables.
    */
    size_t probes_ = 0; // number of probings
    size_t rounds_ = 0; // number of rounds
    size_t prunes_ = 0; // number of successful prunings
    size_t maxprune_ = 0; // maximal size of pruning-set
    size_t elimfv_ = 0; // number of formal variables
    size_t autarks_ = 0; // number of la-variables which yielded autarkies
    size_t finalmu0_ = 0, finalnumvars_ = 0, finalnumvals_ = 0;

    size_t solc_ = 0; // number of solutions found
    bool leaf_ = false;

    sollist_t sollist_; // list of solutions found
    assignment_t elims_; // list of eliminations (pairs variable != value)

  public:

    ReductionStatistics(const GC::IntVarArray& x,
                        const size_t d) noexcept :
      mu0_(GV::sumdomsizes(x)-x.size()), numvars_(GV::numopenvars(x)),
      numvals_(mu0_ + numvars_), depth_(d) { assert(numvals_ > 0); }
    // For testing purposes:
    ReductionStatistics(const size_t mu0, const size_t vars,
                        const size_t d) :
      mu0_(mu0), numvars_(vars), numvals_(mu0_ + numvars_), depth_(d) {
      assert(mu0_ > 0);
    }

    void inc_props() noexcept { ++props_; }
    void inc_elimvals() noexcept { ++elimvals_; }
    void inc_prunes() noexcept { ++prunes_; ;}
    void inc_probes() noexcept { ++probes_; }
    void inc_rounds() noexcept { ++rounds_; }
    void inc_elimfv() noexcept { ++elimfv_; }
    void set_final(const GC::IntVarArray& V) noexcept {
      finalmu0_ = Measures::muap(V);
      finalnumvars_ = GV::numopenvars(V);
      finalnumvals_ = finalmu0_ + finalnumvars_;
    }
    void set_final(const size_t mu0, const size_t vars) noexcept {
      finalmu0_ = mu0;
      finalnumvars_ = vars;
      finalnumvals_ = finalmu0_ + finalnumvars_;
    }

    void inc_solc() noexcept { ++solc_; }
    void set_leaf() noexcept { assert(!leaf_); leaf_ = true; }

    size_t mu0() const noexcept { return mu0_; }
    size_t numvars() const noexcept { return numvars_; }
    size_t numvals() const noexcept { return numvals_; }
    size_t depth() const noexcept { return depth_; }
    size_t props() const noexcept { return props_; }
    size_t elimvals() const noexcept { return elimvals_; }
    size_t prunes() const noexcept { return prunes_; }
    size_t maxprune() const noexcept { return maxprune_; }
    size_t probes() const noexcept { return probes_; }
    size_t rounds() const noexcept { return rounds_; }
    size_t elimfv() const noexcept { return elimfv_; }
    size_t autarks() const noexcept { return autarks_; }
    size_t finalmu0() const noexcept { return finalmu0_; }
    size_t finalnumvars() const noexcept { return finalnumvars_; }
    size_t finalnumvals() const noexcept { return finalnumvals_; }

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

    // dmu0:
    float_t dmu0() const noexcept {
      assert(props_ != 0 or elimvals_ == 0);
      assert(elimvals_ != 0 or props_ == 0);
      assert(elimvals_ >= props_);
      assert(numvars_ != 0);
      assert(rounds_ != 0);
      //return 100 * float_t(props_) / numvars_;
      return mu0_ - finalmu0_;
    }
    // pprunes:
    float_t quotprune() const noexcept {
      assert(probes_ != 0);
      return 100 * float_t(prunes_) / probes_;
    }
    // pmprune:
    float_t quotmaxprune() const noexcept {
      return 100 * float_t(maxprune_) / numvals_;
    }
    // pprobes:
    float_t quotprobes() const noexcept {
      return 100 * float_t(probes_) / numvals_;
    }
    // pelvals:
    float_t quotelimvals() const noexcept {
      assert(numvals_ != 0);
      assert(elimvals_ <= numvals_);
      assert(finalnumvals_ <= numvals_);
      return 100 * float_t(numvals_ - finalnumvals_) / numvals_;
    }

    static constexpr size_t num_stats = 11;
    typedef std::array<float_t, num_stats> export_t;
    export_t extract() const noexcept {
      export_t res;
      res[0] = mu0_; res[1] = dmu0(); res[2] = quotprune();
      res[3] = quotmaxprune(); res[4] = quotprobes(); res[5] = rounds_;
      res[6] = solc_; res[7] = time_; res[8] = quotelimvals();
      res[9] = depth_;
      res[10] = elimfv_;
      return res;
    }
    typedef std::vector<std::string> header_t;
    static header_t stats_header() noexcept {
      return {"mu0", "dmu0", "pprunes", "pmprune", "pprobes", "rounds",
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


  /* Probing a single variable */


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


  /* The reduction */


  /* Running through all variables v with current domain {eps1, ..., epsk},
     k >= 2, and all i, applying the constraint v!=epsi, where v=epsi leads to a
     decision (via propagation), until fixed point.
     "Decision" here either means all constraints have been satisfied (via
     a total assignment), or a contradiction was found. So finding
     a satisfying assignment is treated as a failure, after storing
     the solution in the return-object:
      - if rdl=laefv,  then for v having degree 0, v=eps1.
  */
  template <class SPA>
  ReductionStatistics lareduction(SPA* const m,
                        const OP::RT rt,
                        const OP::RDL rdl, const OP::LAR lar) noexcept {
    const GC::IntVarArray V = m->V;
    ReductionStatistics stats(V, m->nodedata().depth);
    Timing::UserTime timing;
    const Timing::Time_point t0 = timing();
    pruning_table_t PT;

    {int last_red = -1;
    do {
      stats.inc_rounds();
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
          const auto status = m->status(); // calling gecode-propagation
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
    stats.set_final(V);
    assert(stats.elims().size() == stats.elimvals());
    return stats.time(timing() - t0);
  }

}

#endif
