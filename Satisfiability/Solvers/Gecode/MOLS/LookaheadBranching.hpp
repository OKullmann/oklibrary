// Oleg Zaikin, 31.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Look-ahead branching for the Gecode library

  Namespace LookaheadBranching, abbreviated "LB".

BUGS:

TODOS:

2. Measure-based distances
    - The number of eliminated values is Delta GV::sumdomsizes.
    - More generally Delta domsizes is used.
    - The weight for dom-size 1 is zero; this is part of the initialisation of
      the weight-vector (from the parameters).

*/

#ifndef LOOKAHEADBRANCHING_wXJWMxXz3R
#define LOOKAHEADBRANCHING_wXJWMxXz3R

#include <vector>
#include <algorithm>
#include <limits>
#include <mutex>
#include <atomic>
#include <utility>
#include <ostream>
#include <iostream>

#include <cassert>
#include <cstdlib>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <Numerics/NumTypes.hpp>
#include <Numerics/Statistics.hpp>
#include <Numerics/Tau.hpp>

#include "Conditions.hpp"
#include "Options.hpp"
#include "Constraints.hpp"
#include "GcVariables.hpp"
#include "LookaheadReduction.hpp"
#include "Encoding.hpp"
#include "BasicLatinSquares.hpp"
#include "Verification.hpp"

namespace LookaheadBranching {

  namespace FP = FloatingPoint;
  namespace GC = Gecode;
  namespace LR = LookaheadReduction;
  namespace OP = Options;
  namespace CD = Conditions;
  namespace CT = Constraints;
  namespace GV = GcVariables;
  namespace EC = Encoding;
  namespace BS = BasicLatinSquares;
  namespace VR = Verification;

  using size_t = CD::size_t;

  typedef std::vector<int> values_t;
  typedef FP::float80 float_t;
  typedef std::vector<float_t> vec_t;
  // A branching tuple, i.e. a tuple of distances:

  // Converting int to size_t:
  inline constexpr size_t tr(const int size,
                           [[maybe_unused]] const size_t bound = 0) noexcept {
    assert(bound <= std::numeric_limits<int>::max());
    assert(size >= int(bound));
    return size;
  }


  /* Distance and measure functions

     PRELIMINARY
  */
  float_t new_vars(const GC::IntVarArray& V, const GC::IntVarArray& Vn,
                   const vec_t* const wghts, const size_t depth) noexcept {
    const size_t n = tr(V.size(), 1);
    float_t s = 0;
    const float_t w1 = FP::exp((*wghts)[0] * depth);
    for (size_t i = 0; i < n; ++i) {
      const size_t ds = tr(V[i].size(), 1), dsn = tr(Vn[i].size(), 1);
      if (dsn == ds) continue;
      assert(1 <= dsn and dsn < ds);
      if (dsn == 1) s += w1;
      else {
        assert(dsn-1 < wghts->size());
        s += (*wghts)[dsn-1];
      }
    }
    return s;
  }
  float_t domsizes(const GC::IntVarArray& V, const vec_t* const wghts) noexcept {
    const size_t n = tr(V.size(), 1);
    float_t s = 0;
    for (size_t i = 0; i < n; ++i) {
      const size_t ds = tr(V[i].size(), 1);
      assert(ds >= 1);
      assert(ds-1 < wghts->size());
      s += (*wghts)[ds-1];
    }
    return s;
  }


  /*
    Simulating Gecode-branching
  */

  struct ValVec : GC::Choice {
    using values_t = GV::values_t;
    const values_t br; // br[0] is the variable (if br is not empty)

    ValVec(const GC::Brancher& b, const values_t branching) noexcept
      : GC::Choice(b, width(branching)), br(branching) {}

    static unsigned width(const values_t& br) noexcept {
      const unsigned size = br.size();
      if (size == 0) return 1;
      assert(size >= 2);
      return size == 2 ? 2 : size-1;
    }
  };

  GV::values_t append(const int v, GV::values_t values, const bool notrev) {
    const size_t size = values.size();
    GV::values_t br(size+1); br[0] = v;
    if (notrev) std::ranges::copy(values, br.begin()+1);
    else std::copy(values.rbegin(), values.rend(), br.begin()+1);
    return br;
  }
  const ValVec* create(const int v, GV::values_t values,
                       const OP::BRT bt, const OP::GBO bo,
                       GC::Brancher& b) noexcept {
    assert(values.size() >= 2);
    switch (bt) {
    case OP::BRT::bin :
      return new ValVec(b,
        {v, bo==OP::GBO::asc ? values.front() : values.back()});
    case OP::BRT::enu : {
      assert(values.size() >= 2);
      return new ValVec(b, append(v, values, bo == OP::GBO::asc));
    }
    default : assert(false); return nullptr;}
  }

  struct GcBranching : public GC::Brancher {
    const OP::BHV bv; const OP::BRT bt; const OP::GBO bo;
  private :
    GcBranching(GC::Space& home, GcBranching& b)
      : GC::Brancher(home,b), bv(b.bv), bt(b.bt), bo(b.bo) {}
  public :
    GcBranching(const GC::Home home,
                const OP::BHV bv, const OP::BRT bt, const OP::GBO bo)
      : GC::Brancher(home), bv(bv), bt(bt), bo(bo) {}

    GC::Brancher* copy(GC::Space& home) override {
      return new (home) GcBranching(home,*this);
    }
    std::size_t dispose(GC::Space&) noexcept override { return sizeof(*this); }

    bool status(const GC::Space& s) const noexcept override {
      return not GcVariables::empty(static_cast<const CT::GenericMols0&>(s).V);
    }

    const GC::Choice* choice(GC::Space& s0) override {
      CT::GenericMols0& s = static_cast<CT::GenericMols0&>(s0);
      const int v = GV::gcbv(s.V, bv);
      return create(v, GV::values(s.V, v), bt, bo, *this);
    }
    const GC::Choice* choice(const GC::Space&, GC::Archive&) override {
      throw std::runtime_error("RlaMols::choice(Archive): not implemented.");
    }

    GC::ExecStatus commit(GC::Space& s, const GC::Choice& c0,
                          const unsigned a) override {
      const ValVec& c = static_cast<const ValVec&>(c0);
      const size_t w = c.br.size(); assert(w >= 2);
      const int v = c.br[0]; assert(v >= 0);
      CT::GenericMols0* const node = &(static_cast<CT::GenericMols0&>(s));
      assert(v < node->V.size());
      [[maybe_unused]] const size_t oldsize = node->V[v].size();
      assert(oldsize >= 2);
      if (w == 2) {
        if (a == 0) GV::set_var(s, node->V[v], c.br[1]);
        else GV::unset_var(s, node->V[v], c.br[1]);
        assert(node->V[v].size() == (a==0 ? 1 : oldsize-1));
      }
      else {
        assert(a+1 < w);
        assert(oldsize == w-1);
        GV::set_var(s, node->V[v], c.br[a+1]);
        assert(node->V[v].size() == 1);
      }
      return GC::ES_OK;
    }
  };


  struct VVElim : ValVec {
    using ValVec::values_t;
    using ValVec::br;
    using assignment_t = LR::assignment_t;
    const assignment_t elim;

    VVElim(const GC::Brancher& b, const values_t br, const assignment_t elim)
      noexcept : ValVec(b, br), elim(elim) {}
  };

  const VVElim* create_la(const int v, GV::values_t values,
                          const OP::BRT bt, const OP::GBO bo,
                          GC::Brancher& b,
                          VVElim::assignment_t a) noexcept {
    assert(values.size() >= 2);
    switch (bt) {
    case OP::BRT::bin :
      return new VVElim(b,
        {v, bo==OP::GBO::asc ? values.front() : values.back()}, std::move(a));
    case OP::BRT::enu : {
      assert(values.size() >= 2);
      return new VVElim(b, append(v, values, bo==OP::GBO::asc), std::move(a));
    }
    default : assert(false); return nullptr;}
  }


  /*
    Using simulated Gecode-branching with lookahead-reduction
  */

  struct rlaParams {
    const OP::RT rt;
    const OP::LAR lar;
    const OP::BHV bv;
    const OP::BRT bt;
    const OP::GBO bo;
    const bool parallel;
  };

  struct rlaStats {
    typedef GenStats::GStdStats<LR::ReductionStatistics::num_stats> stats_t;
    typedef LR::sollist_t sollist_t;

    inline static std::atomic_bool abort;

    rlaStats(std::ostream* const log, const EC::EncCond* const enc,
             const size_t threshold) noexcept :
    sol_counter(0), lvs_counter(0), log(log), enc(enc), threshold(threshold) {
      assert(not enc or log);
      abort = false;
    }
    rlaStats(const rlaStats&) = delete;

    size_t sol_count() const noexcept { return sol_counter; }
    size_t lvs() const noexcept { return lvs_counter; }
    const stats_t& stats() const noexcept { return S; }
    const sollist_t& sols() const noexcept { return sols_; }

    void add(LR::ReductionStatistics& s) noexcept {
      S += s.extract();
      lvs_counter += s.leafcount();
      const size_t solc = s.solc();
      if (solc == 0) return;
      assert(BS::alldiffelem(s.sollist()));
      if (log) {
        if (enc) {
          assert(solc == s.sollist().size());
          for (auto& sol : s.sollist()) {
            ++sol_counter;
            const auto dsol = enc->decode(std::move(sol));
            *log << sol_counter << "\n" << dsol << std::endl;
            if (not VR::correct(enc->ac, dsol))
              std::cerr << "\nERROR[LABranching::rlaStats]: "
                "correctness-checking failed for solution " << sol_counter
                        << ":\n" << dsol << "\n";
          }
        }
        else {
          assert(s.sollist().empty());
          *log << " " << solc; log -> flush(); sol_counter += solc;
        }
      }
      else {
        sol_counter += solc;
        if (not s.sollist().empty()) {
          assert(s.sollist().size() == solc);
          sols_.reserve(sols_.size() + solc);
          for (auto& sol : s.sollist()) sols_.push_back(std::move(sol));
        }
      }
      if (threshold != 0 and sol_counter >= threshold)
        abort.store(true, std::memory_order_relaxed);
    }

  private :
    sollist_t sols_;
    stats_t S;
    size_t sol_counter, lvs_counter;
    std::ostream* const log;
    const EC::EncCond* const enc;
  public :
    const size_t threshold;
  };


  struct RlaBranching : public GC::Brancher {
    const rlaParams P;
  private :
    rlaStats* const S;
    inline static std::mutex stats_mutex;

    RlaBranching(GC::Space& home, RlaBranching& b)
      : GC::Brancher(home,b), P(b.P), S(b.S) {}
  public :
    RlaBranching(const GC::Home home, const rlaParams P, rlaStats* const S)
      : GC::Brancher(home), P(P), S(S) { assert(S); }

    GC::Brancher* copy(GC::Space& home) override {
      return new (home) RlaBranching(home,*this);
    }
    std::size_t dispose(GC::Space&) noexcept override { return sizeof(*this); }

    bool status(const GC::Space& s) const noexcept override {
      return not GcVariables::empty(static_cast<const CT::GenericMols0&>(s).V);
    }

    const GC::Choice* choice(GC::Space& s0) override {
      CT::GenericMols0& s = static_cast<CT::GenericMols0&>(s0);
      auto stats = LR::lareduction(&s, P.rt, P.lar);
      if (P.parallel) {
        std::lock_guard<std::mutex> lock(stats_mutex);
        S->add(stats);
      }
      else S->add(stats);
      if (stats.leafcount()) return new VVElim(*this, {}, {});
      const int v = GV::gcbv(s.V, P.bv);
      return create_la(v, GV::values(s.V, v), P.bt, P.bo, *this,
                       std::move(stats.elims()));
    }
    const GC::Choice* choice(const GC::Space&, GC::Archive&) override {
      throw std::runtime_error("RlaMols::choice(Archive): not implemented.");
    }

    GC::ExecStatus commit(GC::Space& s, const GC::Choice& c0,
                          const unsigned a) override {
      const VVElim& c = static_cast<const VVElim&>(c0);
      const size_t w = c.br.size();
      if (w == 0) return GC::ExecStatus::ES_FAILED;
      const int v = c.br[0]; assert(v >= 0);
      CT::GenericMols0* const node = &(static_cast<CT::GenericMols0&>(s));
      for (const auto [var,val] : c.elim) {
        assert(var < node->V.size());
        GV::unset_var(s, node->V[var], val);
      }
      assert(v < node->V.size());
      [[maybe_unused]] const size_t oldsize = node->V[v].size();
      assert(oldsize >= 2);
      if (w == 2) {
        if (a == 0) GV::set_var(s, node->V[v], c.br[1]);
        else GV::unset_var(s, node->V[v], c.br[1]);
        assert(node->V[v].size() == (a==0 ? 1 : oldsize-1));
      }
      else {
        assert(a+1 < w);
        // assert(oldsize == w-1); // only if node->status() called before
        GV::set_var(s, node->V[v], c.br[a+1]);
        assert(node->V[v].size() == 1);
      }
      return GC::ES_OK;
    }
  };


  /*
    Lookahead-reduction and branching
  */

  struct laParams {
    const OP::RT rt;
    const OP::LBRT bt;
    const OP::DIS d;
    const OP::LBRO bo;
    const OP::LAR lar;
    const bool parallel;
  };

  class BranchingStatistics {
    Timing::Time_point time_; // total time for the branching construction

  public :

    BranchingStatistics& time(const Timing::Time_point t) noexcept {
      time_ = t; return *this;
    }

    Timing::Time_point time() const noexcept { return time_; }
    static constexpr size_t num_stats = 1;
    typedef std::array<float_t, num_stats> export_t;
    export_t extract() const noexcept {
      export_t res;
      res[0] = time_;
      return res;
    }

  };

  struct laStats {
    typedef GenStats::GStdStats<BranchingStatistics::num_stats> stats_t;
  private :
    rlaStats rla_;
    stats_t S;
  public :

    laStats(std::ostream* const log, const EC::EncCond* const enc,
            const size_t threshold) noexcept
      : rla_(log, enc, threshold) {}

    const rlaStats& rla() const noexcept { return rla_; }
    const stats_t& stats() const noexcept { return S; }

    void add(LR::ReductionStatistics& s0,
             const BranchingStatistics& s1) noexcept {
      rla_.add(s0);
      S += s1.extract();
    }

  };


  struct laBranching : public GC::Brancher {
    const laParams P;
  private :
    laStats* const S;
    inline static std::mutex stats_mutex;

    laBranching(GC::Space& home, laBranching& b)
      : GC::Brancher(home,b), P(b.P), S(b.S) {}
  public :
    laBranching(const GC::Home home, const laParams P, laStats* const S)
      : GC::Brancher(home), P(P), S(S) { assert(S); }

    GC::Brancher* copy(GC::Space& home) override {
      return new (home) laBranching(home,*this);
    }
    std::size_t dispose(GC::Space&) noexcept override { return sizeof(*this); }

    bool status(const GC::Space& s) const noexcept override {
      return not GcVariables::empty(static_cast<const CT::GenericMols0&>(s).V);
    }

    const GC::Choice* choice(GC::Space& s0) override {
      Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      BranchingStatistics stats1;
      CT::GenericMols0& s = static_cast<CT::GenericMols0&>(s0);
      auto stats0 = LR::lareduction(&s, P.rt, P.lar);
      // XXX

    END :
     stats1.time(timing()-t0);
     if (P.parallel) {
        std::lock_guard<std::mutex> lock(stats_mutex);
        S->add(stats0, stats1);
      }
     else S->add(stats0, stats1);
     // XXX
    }
    const GC::Choice* choice(const GC::Space&, GC::Archive&) override {
      throw std::runtime_error("laMols::choice(Archive): not implemented.");
    }

    GC::ExecStatus commit(GC::Space& s, const GC::Choice& c0,
                          const unsigned a) override {
      const VVElim& c = static_cast<const VVElim&>(c0);
      const size_t w = c.br.size();
      if (w == 0) return GC::ExecStatus::ES_FAILED;
      const int v = c.br[0]; assert(v >= 0);
      // XXX
    }


  };

}

#endif
