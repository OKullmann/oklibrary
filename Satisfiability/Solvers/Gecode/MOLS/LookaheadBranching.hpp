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

0. Provide documentation.

1. Provide and use better statistics-functions for the branches
    - There needs to be a general facility, which takes as input
      as vector of values, and provides, using the most exact
      computations, all basic statistics.
    - Likely this should include the median.

2. Measure the variation of considered branchings for one node:
    - Just the basic statistics for (l)tau, over all branchings
      considered.
    - The problem here is that the tau-values are very small; some
      standardisation would be helpful.
    - Perhaps the numbers should be divided by the mean.

3. Update member "vals_" of BranchingStatistics
    - More general, it is the "measure".
    - What to do when only the distances are available?
    - One could use the sum of the distances along the path
      from the root: this would be kind of an "inverted measure".
    - The statistics-output currently uses "mu", which could also
      be used for that "inverted measure".

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
#include <functional>
#include <algorithm>
#include <initializer_list>
#include <map>
#include <array>

#include <cassert>
#include <cstdlib>
#include <csignal>

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
  using values_t = GV::values_t;

  typedef FP::float80 float_t;
  typedef std::vector<float_t> vec_t;


  // Converting int to size_t:
  inline constexpr size_t tr(const int size,
                           [[maybe_unused]] const size_t bound = 0) noexcept {
    assert(bound <= std::numeric_limits<int>::max());
    assert(size >= int(bound));
    return size;
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

    VVElim(const GC::Brancher& b, values_t br, assignment_t elim)
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
      return new VVElim(b,
                        append(v, std::move(values), bo==OP::GBO::asc),
                        std::move(a));
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

  template <typename STO>
  struct StoppingData {
    typedef STO st_t;
    const st_t st;
    const size_t val; // the count for time is in seconds
    constexpr StoppingData() noexcept : st(STO::none), val(0) {}
    constexpr StoppingData(const STO st, const size_t val)
      noexcept : st(st), val(val) {}
    operator bool() const noexcept { return st != STO::none; }
    friend std::ostream& operator <<(std::ostream& out, const StoppingData st) {
      return out << st.st << "," << st.val;
    }
  };
  typedef StoppingData<OP::LRST> LRStoppingData;

  struct ListStoppingData {
    typedef std::map<OP::LRST, size_t> map_t;
    typedef std::vector<LRStoppingData> list_t;

    ListStoppingData() noexcept = default;
    ListStoppingData(const std::initializer_list<LRStoppingData> L) noexcept {
      for (const auto st : L) operator +=(st);
    }

    list_t list() const {
      list_t res; res.reserve(m.size());
      for (const auto [s,val] : m) res.emplace_back(s,val);
      return res;
    }
    operator bool() const noexcept {
      return not m.empty();
    }

    ListStoppingData& operator +=(const LRStoppingData st) {
      if (st.st != OP::LRST::none)
        m[st.st] = std::max(m[st.st], st.val);
      return *this;
    }

  private :
    map_t m;
  };

  struct rlaStats {
    typedef GenStats::GStdStats<LR::ReductionStatistics::num_stats> stats_t;
    typedef LR::sollist_t sollist_t;

    typedef ListStoppingData::list_t st_t;
    st_t st;
    inline static std::atomic_bool abort;

    rlaStats(std::ostream* const log, const EC::EncCond* const enc,
             const ListStoppingData& st) noexcept :
      st(st.list()), sol_counter(0), lvs_counter(0), log(log), enc(enc) {
      assert(not enc or log);
      abort = false;
      std::signal(SIGINT, activate_abort);
    }
    rlaStats(const rlaStats&) = delete;

    size_t sol_count() const noexcept { return sol_counter; }
    size_t lvs() const noexcept { return lvs_counter; }
    const stats_t& stats() const noexcept { return S; }
    const sollist_t& sols() const noexcept { return sols_; }

    static void activate_abort([[maybe_unused]]int dummy=0) noexcept {
      abort.store(true, std::memory_order_relaxed);
    }
    void handle_abort() const noexcept {
      for (const auto [s, val] : st)
        switch (s) {
        case OP::LRST::nds :
          if (S.N() > val) {activate_abort(); return;} else break;
        case OP::LRST::lvs :
          if (lvs_counter > val) {activate_abort(); return;} else break;
        case OP::LRST::inds :
          if (S.N()-lvs_counter > val) {activate_abort(); return;} else break;
        case OP::LRST::satc :
          if (sol_counter > val) {activate_abort(); return;} else break;
        case OP::LRST::none : assert(false);
        }
    }

    void add(LR::ReductionStatistics& s) noexcept {
      S += s.extract();
      lvs_counter += s.leafcount();
      const size_t solc = s.solc();
      size_t old_sol_counter = sol_counter;
      sol_counter += solc;
      handle_abort();
      if (solc == 0) return;
      assert(BS::alldiffelem(s.sollist()));
      if (log) {
        if (enc) {
          assert(solc == s.sollist().size());
          for (auto& sol : s.sollist()) {
            ++old_sol_counter;
            const auto dsol = enc->decode(std::move(sol));
            *log << old_sol_counter << "\n" << dsol << std::endl;
            if (not VR::correct(enc->ac, dsol))
              std::cerr << "\nERROR[LABranching::rlaStats]: "
                "correctness-checking failed for solution " << old_sol_counter
                        << ":\n" << dsol << "\n";
          }
        }
        else {
          assert(s.sollist().empty());
          *log << " "; if (solc > 1) *log << solc << ",";
          *log << sol_counter;
          log -> flush(); ;
        }
      }
      else {
        if (not s.sollist().empty()) {
          assert(s.sollist().size() == solc);
          sols_.reserve(sols_.size() + solc);
          for (auto& sol : s.sollist()) sols_.push_back(std::move(sol));
        }
      }
    }

  private :
    sollist_t sols_;
    stats_t S;
    size_t sol_counter, lvs_counter;
    std::ostream* const log;
    const EC::EncCond* const enc;
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

    static GC::ExecStatus commit0(GC::Space& s, const GC::Choice& c0,
                                  const unsigned a) {
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
    GC::ExecStatus commit(GC::Space& s, const GC::Choice& c0,
                          const unsigned a) override {
      return commit0(s, c0, a);
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
    size_t vals_; // XXX
    size_t width_; // width of branching
    float_t ltau_;
    float_t mind_, meand_, maxd_, sdd_;
    size_t depth_;
    Timing::Time_point time_; // total time for the branching construction

  public :

    explicit BranchingStatistics(const size_t depth) noexcept
      : depth_(depth) {}

    void set_vals(const size_t vals) noexcept {
      assert(vals != 0); vals_ = vals;
    }
    void set_width(const size_t w) noexcept {
      assert(w >= 2); width_ = w;
    }
    void set_tau(const float_t t) noexcept {
      assert(t > 0 and t < FP::pinfinity); ltau_ = t;
    }
    template <class STATS>
    void set_dist(const STATS& s) noexcept {
      assert(s.N() == width_);
      mind_ = s.min(); meand_ = s.amean(); maxd_ = s.max();
      sdd_ = s.sd_population();
    }

    BranchingStatistics& time(const Timing::Time_point t) noexcept {
      time_ = t; return *this;
    }
    Timing::Time_point time() const noexcept { return time_; }

    static constexpr size_t num_stats = 9;
    typedef std::array<float_t, num_stats> export_t;
    export_t extract() const noexcept {
      export_t res;
      res[0] = vals_; res[1] = width_; res[2] = ltau_;
      res[3] = mind_; res[4] = meand_; res[5] = maxd_; res[6] = sdd_;
      res[7] = depth_;
      res[num_stats-1] = time_;
      return res;
    }

  };

  const VVElim* create_la(const int v, GV::values_t values,
                          const OP::LBRT bt,
                          GC::Brancher& b,
                          VVElim::assignment_t a) {
    assert(not values.empty());
    switch (bt) {
    case OP::LBRT::bin : {
      assert(values.size() == 1);
      return new VVElim(b, {v, values[0]}, std::move(a));
    }
    case OP::LBRT::enu : {
      assert(values.size() >= 2);
      return new VVElim(b, append(v, std::move(values), true),
                        std::move(a));
    }
    default : throw std::runtime_error("LookaheadBranching::create_la: "
                                       "non-handled LBRT");}
  }

  struct laStats {
    typedef GenStats::GStdStats<BranchingStatistics::num_stats> stats_t;
  private :
    rlaStats rla_;
    stats_t S;
  public :

    laStats(std::ostream* const log, const EC::EncCond* const enc,
            const ListStoppingData& st) noexcept
      : rla_(log, enc, st) {}

    const rlaStats& rla() const noexcept { return rla_; }
    const stats_t& stats() const noexcept { return S; }

    void add(LR::ReductionStatistics& s0) noexcept {
      rla_.add(s0);
    }
    void add(LR::ReductionStatistics& s0,
             const BranchingStatistics& s1) noexcept {
      rla_.add(s0);
      S += s1.extract();
    }

  };


  typedef FP::float80 muld_t;
  constexpr size_t N_muld = 100;
  static_assert(N_muld >= 10);
  typedef std::array<muld_t, N_muld+1> given_muld_t;
  consteval given_muld_t init_muld() noexcept {
    static_assert(std::is_same_v<muld_t, FP::float80>);
    given_muld_t res; res[0] = FP::minfinity;
    for (size_t i = 2; i <= N_muld; ++i) res[i] = FP::log2(i);
    return res;
  }
  constexpr given_muld_t given_muld = init_muld();
  static_assert(given_muld.size() == N_muld+1);
  static_assert(given_muld[0] == FP::minfinity);
  static_assert(given_muld[1] == 0); static_assert(given_muld[2] == 1);
  static_assert(given_muld[4] == 2);
  constexpr muld_t wmuld(const size_t i) noexcept {
    static_assert(std::is_same_v<muld_t, FP::float80>);
    return i <= N_muld ? given_muld[i] : FP::log2(i);
  }
  static_assert(wmuld(0) == FP::minfinity); static_assert(wmuld(8) == 3);

  typedef std::function<float_t(const GC::IntVarArray&)> measure_t;
  typedef std::function<
    float_t(const GC::IntVarArray&,const GC::IntVarArray&)> distance_t;

  template <class SPA>
  float_t branch_distance(SPA* const m, const int v, const int val,
                         const bool equal,
                         const distance_t& d) noexcept {
    assert(v >= 0);
    const auto V = m->V;
    [[maybe_unused]] const auto size = V.size();
    assert(v < size);
    std::unique_ptr<SPA> c(static_cast<SPA*>(m->clone()));
    auto& nV = c.get()->V;
    assert(nV.size() == size);
    if (equal) GV::set_var(*c.get(), nV[v], val);
    else GV::unset_var(*c.get(), nV[v], val);
    {[[maybe_unused]] const auto status = c->status();
     assert(status == GC::SS_BRANCH);}
    return d(V,nV);
  }

  float_t wnumvars(const GC::IntVarArray& V,
                       const OP::weights_t* const w) noexcept {
    float_t sum = 0;
    for (int v = 0; v < V.size(); ++v) {
      const size_t s = tr(V[v].size(), 1);
      assert(s < w->size());
      sum += (*w)[s];
    }
    return sum;
  }
  float_t new_vars(const GC::IntVarArray& V, const GC::IntVarArray& nV,
                   const OP::weights_t* const w,
                   const size_t depth) noexcept {
    float_t sum = 0;
    const float_t w1 = FP::exp2((*w)[1] * depth);
    for (int v = 0; v < V.size(); ++v) {
      const size_t s = tr(V[v].size(), 1), sn = tr(nV[v].size(), 1);
      if (sn == s) continue;
      assert(sn < s);
      if (sn == 1) sum += w1;
      else { assert(sn < w->size()); sum += (*w)[sn]; }
    }
    return sum;
  }


  struct LaBranching : public GC::Brancher {
    const laParams P;
    typedef const OP::weights_t* weights_pt;
    const weights_pt weights;
  private :
    laStats* const S;
    inline static std::mutex stats_mutex;

    LaBranching(GC::Space& home, LaBranching& b)
      : GC::Brancher(home,b), P(b.P), weights(b.weights), S(b.S) {}
  public :
    LaBranching(const GC::Home home,
                const laParams P, laStats* const S,
                const weights_pt w = nullptr)
      : GC::Brancher(home), P(P), weights(w), S(S) {
      assert(S); assert(weights);
    }

    GC::Brancher* copy(GC::Space& home) override {
      return new (home) LaBranching(home,*this);
    }
    std::size_t dispose(GC::Space&) noexcept override { return sizeof(*this); }

    bool status(const GC::Space& s) const noexcept override {
      return not GcVariables::empty(static_cast<const CT::GenericMols0&>(s).V);
    }

    const GC::Choice* choice(GC::Space& s0) override {
      CT::GenericMols1& s = static_cast<CT::GenericMols1&>(s0);
      auto stats0 = LR::lareduction(&s, P.rt, P.lar);
      if (stats0.leafcount()) {
        if (P.parallel) {
          std::lock_guard<std::mutex> lock(stats_mutex);
          S->add(stats0);
        } else S->add(stats0);
        return new VVElim(*this, {}, {});
      }
      Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      const size_t depth = s.nodedata().depth;
      BranchingStatistics stats1(depth);

      CT::GenericMols1* const node = &s;
      for (const auto [var,val] : stats0.elims()) {
        assert(var < node->V.size()); GV::unset_var(s, node->V[var], val);
      }
      {[[maybe_unused]] const auto status = node->status();
       assert(status == GC::SS_BRANCH);
      }

      const auto& V = node->V;
      const auto n = V.size();
      int bestv = -1, bestval = -1;
      float_t opttau = FP::pinfinity;
      std::vector<float_t> optbt;
      const measure_t mu = P.d != OP::DIS::wdeltaL ?
        measure_t(GV::sumdomsizes) :
        [this](const GC::IntVarArray& V){
          return wnumvars(V, weights);};
      const float_t mu0 = mu(V); stats1.set_vals(mu0);
      const distance_t d = P.d == OP::DIS::newvars ?
        distance_t(
        [this,depth](const GC::IntVarArray& V, const GC::IntVarArray& nV){
          return new_vars(V, nV, weights, depth);}) :
        distance_t(
        [mu0,&mu](const GC::IntVarArray&, const GC::IntVarArray& nV){
          return mu0 - mu(nV);});

      for (int v = 0; v < n; ++v) {
        const auto& vo = V[v];
        if (vo.size() == 1) continue;
        const auto values = GV::values(V, v);
        if (P.bt == OP::LBRT::bin) {
          for (const int val : values) {
            const float_t a = branch_distance(node, v, val, true, d),
              b = branch_distance(node, v, val, false, d);
            assert(a > 0 and b > 0);
            const float_t tau =  Tau::ltau(a,b);
            assert(tau > 0 and tau < FP::pinfinity);
            if (tau < opttau) {
              opttau = tau; optbt = {a,b}; bestv = v; bestval = val;
            }
          }
        }
        else {
          std::vector<float_t> branchtuple;
          branchtuple.reserve(values.size());
          for (const int val : values) {
            const float_t t = branch_distance(node, v, val, true, d);
            assert(t > 0);
            branchtuple.push_back(t);
          }
          const float_t tau = Tau::ltau(branchtuple);
          assert(tau > 0 and tau < FP::pinfinity);
          if (tau < opttau) {
            opttau = tau; optbt = std::move(branchtuple); bestv = v;
          }
        }
      }

      assert(bestv >= 0);
      stats1.set_tau(opttau);
      const size_t w = optbt.size();
      stats1.set_width(w);
      assert(w >= 2);
      {GenStats::StdStats statsd;
       for (const auto d : optbt) statsd += d;
       stats1.set_dist(statsd);
      }
      auto values = bestval == -1 ? GV::values(V, bestv) : values_t{bestval};
      if (P.bt != OP::LBRT::bin) {
        assert(P.bt == OP::LBRT::enu);
        assert(bestval == -1);
        assert(w == values.size());
        if (P.bo == OP::LBRO::desc) std::ranges::reverse(values);
        else if (P.bo == OP::LBRO::ascd or P.bo == OP::LBRO::descd) {
          std::vector<std::pair<int, float_t>> valdist;
          valdist.reserve(w);
          for (size_t i = 0; i < w; ++i)
            valdist.emplace_back(values[i], optbt[i]);
          std::ranges::sort(valdist, {}, [](const auto& p){return p.second;});
          if (P.bo == OP::LBRO::ascd)
            for (size_t i = 0; i < w; ++i) values[i] = valdist[i].first;
          else
            for (size_t i = 0; i < w; ++i) values[i] = valdist[(w-1)-i].first;
        }
      }
      stats1.time(timing()-t0);
      if (P.parallel) {
        std::lock_guard<std::mutex> lock(stats_mutex);
        S->add(stats0, stats1);
      } else S->add(stats0, stats1);
      return create_la(bestv, std::move(values), P.bt, *this,
                       std::move(stats0.elims()));
    }
    const GC::Choice* choice(const GC::Space&, GC::Archive&) override {
      throw std::runtime_error("laMols::choice(Archive): not implemented.");
    }

    GC::ExecStatus commit(GC::Space& s0, const GC::Choice& c0,
                          const unsigned a) override {
      {CT::GenericMols1& s = static_cast<CT::GenericMols1&>(s0);
       s.update_clone();}
      return RlaBranching::commit0(s0, c0, a);
    }

};

}

#endif
