// Oleg Zaikin, 31.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Look-ahead branching for the Gecode library

  Namespace LookaheadBranching, abbreviated "LB".

  Simulating Gecode-branching:

    - using size_t
    - using values_t, a vector of int
    - using float_t, defined as float80

    - typedef vec_t for vector of float_t


    - ValVec, derived from GC::Choice, containing an object
      of values_t (vector of int)

    - helper function append(int, values_t, bool) : adds the int
      to the front of the vector, where the vector is either taken
      in order or reversed
    - create(int v, values_t values, BRT bt, GBO bo, GC::Brancher& b)
        -> const ValVec*
      creates a branching

    - class GcBranching, derived from GC::Brancher
      for (just) simulating Gecode-branching


    - VVElim, derived from ValVec, adds a member for the "assignment"
      found in la-reduction (the eliminated values)

    - create_la: similar to create above, but now producing a const VVElim*


    For rlaMols, and handling of statistics for la-reduction:

     - struct rlaParams for the parameters needed for la-reduction

     - struct StoppingData<STO> for one stopping-data item with value;
         instances: here LRStoppingData = StoppingData<OP::LRST>,
         in Solvers.hpp: GcStoppingData = StoppingData<OP::STO>
     - ListStoppingData, a wrapper for a vector of LRStoppingData

     - class rlaStats: contains
       - an array S of size 2 of the extracted data from
         LR::ReductionStatistics (for inner nodes and leaves)
       - a member of type ListStoppingData
       - an atomic_bool (set by member-function activate_abort, read
         by the Gecode-DFS-handler)
       - the list of solutions, and the counter of solutions
       - a pointer to the log-stream
       - a pointer to the Encoding-object.
      The main member function is add(LR::ReductionStatistics).
      Solvers::rlasolver creates one global (singleton) rlaStats-object,
      to aggregate the statistics (and handle abort).

    - class RlaBranching, derived from GC::Brancher:
       - choice calls the add-function of rlaStats (possibly locked)
       - commit calls commit0
       - commit0 calles update_clone() for the current node, and
         eliminates all values from the la-reduction.


    For laMols:


BUGS:

TODOS:

-1. Implement sorting of branchings for binary branching.

0. Provide documentation.

1. Provide and use better statistics-functions for the branches
    - There needs to be a general facility, which takes as input
      as vector of values, and provides, using the most exact
      computations, all basic statistics.
    - Likely this should include the median.

2. Measure the variation of considered branchings for one node:
    - Just the basic statistics for (l)tau, over all branchings
      considered.
    - But the range, max-tau / min-tau, seems most informative,
      perhaps multiplied by 100 (as a percentage).
      This also provides a standardisation.
    - This should replace the current ltau-value (which isn't very
      informative).

3. Update member "vals_" of BranchingStatistics
    - More general, it is the "measure".
    - What to do when only the distances are available?
    - One could use the sum of the distances along the path
      from the root: this would be kind of an "inverted measure".
    - The statistics-output currently uses "mu", which could also
      be used for that "inverted measure".
    - It seems best to have the reduction statistics besides depth
      also report exp(sum of distances * ltau), the random variable
      predicting the leaf-count.
      So the node besides depth needs also to have this sum.
    - Then for BranchingStatistics we don't report otherwise on tau;
      the measure-component "vals_" should be replaced by the relative
      change in total-open-assignment-count (as achieved by this node).

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
#include <Numerics/NumBasicFunctions.hpp>
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
#include "Measures.hpp"

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
  namespace MS = Measures;

  using size_t = CD::size_t;
  using values_t = GV::values_t;
  using float_t = MS::float_t;

  typedef std::vector<float_t> vec_t;

  using MS::tr;


  /*
    Simulating Gecode-branching
  */

  struct ValVec : GC::Choice {
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
  const ValVec* create(const int v, values_t values,
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
    const size_t val; // if this is time, then in seconds
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
    typedef GenStats::GStdStats<LR::ReductionStatistics::num_stats> stats0_t;
    typedef std::array<stats0_t, 2> stats_t;
    typedef LR::sollist_t sollist_t;

    typedef ListStoppingData::list_t st_t;
    const st_t st;
    inline static std::atomic_bool abort;

    rlaStats(std::ostream* const log, const EC::EncCond* const enc,
             const ListStoppingData& st) noexcept :
      st(st.list()), sol_counter(0), log(log), enc(enc) {
      assert(not enc or log);
      abort = false;
      std::signal(SIGINT, activate_abort);
    }
    rlaStats(const rlaStats&) = delete;

    size_t sol_count() const noexcept { return sol_counter; }
    size_t inds() const noexcept { return S[0].N(); }
    size_t lvs() const noexcept { return S[1].N(); }
    size_t nds() const noexcept { return S[0].N() + S[1].N(); }

    const stats_t& stats() const noexcept { return S; }

    const sollist_t& sols() const noexcept { return sols_; }

    static void activate_abort([[maybe_unused]]int dummy=0) noexcept {
      abort.store(true, std::memory_order_relaxed);
    }
    void handle_abort() const noexcept {
      for (const auto [s, val] : st)
        switch (s) {
        case OP::LRST::nds :
          if (nds() > val) {activate_abort(); return;} else break;
        case OP::LRST::lvs :
          if (lvs() > val) {activate_abort(); return;} else break;
        case OP::LRST::inds :
          if (inds() > val) {activate_abort(); return;} else break;
        case OP::LRST::satc :
          if (sol_counter > val) {activate_abort(); return;} else break;
        case OP::LRST::none : assert(false); }
    }

    void add(LR::ReductionStatistics& s) noexcept {
      S[s.leaf()] += s.extract();
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
    stats_t S; // statistics for inner nodes (S[0]) resp. leaves (S[1])
    size_t sol_counter;
    std::ostream* const log;
    const EC::EncCond* const enc;
  };


  struct VVElim : ValVec {
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
      CT::GenericMols1& s = static_cast<CT::GenericMols1&>(s0);
      auto stats = LR::lareduction(&s, P.rt, P.lar);
      if (P.parallel) {
        std::lock_guard<std::mutex> lock(stats_mutex);
        S->add(stats);
      }
      else S->add(stats);
      if (stats.leaf()) return new VVElim(*this, {}, {});
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
      CT::GenericMols1* const node = &(static_cast<CT::GenericMols1&>(s));
      node->update_clone();
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
    float_t minp_, meanp_, maxp_, sdd_; // the branch-probabilities
    Timing::Time_point time_; // total time for the branching construction

  public :

    BranchingStatistics() noexcept = default;

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
      minp_ = s.min(); meanp_ = s.amean(); maxp_ = s.max();
      sdd_ = s.sd_population();
    }

    BranchingStatistics& time(const Timing::Time_point t) noexcept {
      time_ = t; return *this;
    }
    Timing::Time_point time() const noexcept { return time_; }

    static constexpr size_t num_stats = 8;
    typedef std::array<float_t, num_stats> export_t;
    export_t extract() const noexcept {
      export_t res;
      res[0] = vals_; res[1] = width_; res[2] = ltau_;
      res[3] = minp_; res[4] = meanp_; res[5] = maxp_; res[6] = sdd_;
      res[num_stats-1] = time_;
      return res;
    }
    static std::vector<std::string> stats_header() noexcept {
      return {"mu1", "w", "ltau",
          "minp", "meanp", "maxp", "sdd",
          "tb"};
    }
    static size_t index(const std::string& s) {
      const auto words = stats_header();
      const auto find = std::ranges::find(words, s);
      if (find != words.end()) return find - words.begin();
      throw std::runtime_error("ERROR[LB::index]: unknown variable \""
                               + s + "\"\n");
    }

    bool operator ==(const BranchingStatistics&) const noexcept = default;
    friend bool eqwt(BranchingStatistics lhs, BranchingStatistics rhs)
      noexcept {
      return lhs.time(0) == rhs.time(0);
    }
  };


  struct VVEMeasure : VVElim {
    using VVElim::br;
    using VVElim::elim;
    vec_t m;

    VVEMeasure(const GC::Brancher& b, values_t br, assignment_t elim,
               vec_t m) noexcept : VVElim(b,br,elim), m(m) {}
  };

  const VVEMeasure* create_la(const int v, GV::values_t values,
                              const OP::LBRT bt,
                              GC::Brancher& b,
                              VVElim::assignment_t a,
                              vec_t m) {
    assert(not values.empty());
    switch (bt) {
    case OP::LBRT::bin : {
      assert(values.size() == 1);
      return new VVEMeasure(b, {v, values[0]}, std::move(a), std::move(m));
    }
    case OP::LBRT::enu : {
      assert(values.size() >= 2);
      return new VVEMeasure(b, append(v, std::move(values), true),
                            std::move(a), std::move(m));
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
      if (stats0.leaf()) {
        if (P.parallel) {
          std::lock_guard<std::mutex> lock(stats_mutex);
          S->add(stats0);
        } else S->add(stats0);
        return new VVElim(*this, {}, {});
      }
      Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      const size_t depth = s.nodedata().depth;
      BranchingStatistics stats1;

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
      const measure_t mu = P.d != OP::DIS::wdeltaL ? MS::muld :
        measure_t([this](const GC::IntVarArray& V){
                    return MS::wnumvars(V, weights);});
      const float_t mu0 = mu(V); stats1.set_vals(mu0);
      const distance_t d = P.d == OP::DIS::newvars ?
        distance_t(
        [this,depth](const GC::IntVarArray& V, const GC::IntVarArray& nV){
          return MS::new_vars(V, nV, weights, depth);}) :
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
      vec_t mv; mv.reserve(w);
      {GenStats::StdStats statsd;
        for (const auto d : optbt) {
          const float_t m = opttau * d;
          mv.push_back(m);
          statsd += FP::exp(-m);
        }
        stats1.set_dist(statsd);
      }
      auto values = bestval == -1 ? GV::values(V, bestv) : values_t{bestval};
      if (P.bt != OP::LBRT::bin) {
        assert(P.bt == OP::LBRT::enu);
        assert(bestval == -1);
        assert(w == values.size());
        if (P.bo == OP::LBRO::desc) {
          std::ranges::reverse(values);
          std::ranges::reverse(mv);
        }
        else if (P.bo == OP::LBRO::ascd or P.bo == OP::LBRO::descd) {
          std::vector<std::tuple<int, float_t, float_t>> valdist;
          valdist.reserve(w);
          for (size_t i = 0; i < w; ++i)
            valdist.emplace_back(values[i], optbt[i], mv[i]);
          std::ranges::sort(valdist, {}, [](const auto& p){
                              return std::get<1>(p);});
          for (size_t i = 0; i < w; ++i) {
            const size_t ip = P.bo == OP::LBRO::ascd ? i : (w-1)-i;
            values[i] = std::get<0>(valdist[ip]);
            mv[i] = std::get<2>(valdist[ip]);
          }
        }
      }
      stats1.time(timing()-t0);
      if (P.parallel) {
        std::lock_guard<std::mutex> lock(stats_mutex);
        S->add(stats0, stats1);
      } else S->add(stats0, stats1);
      return create_la(bestv, std::move(values), P.bt, *this,
                       std::move(stats0.elims()), std::move(mv));
    }
    const GC::Choice* choice(const GC::Space&, GC::Archive&) override {
      throw std::runtime_error("laMols::choice(Archive): not implemented.");
    }

    GC::ExecStatus commit(GC::Space& s, const GC::Choice& c0,
                          const unsigned a) override {
      const VVEMeasure& c = static_cast<const VVEMeasure&>(c0);
      const size_t w = c.br.size();
      if (w == 0) return GC::ExecStatus::ES_FAILED;
      assert(a < w); assert(c.m.size() == w);
      CT::GenericMols2* const node = &(static_cast<CT::GenericMols2&>(s));
      node->update_clone(c.m[a]);
      return RlaBranching::commit0(s, c0, a);
    }

  };

}

#endif
