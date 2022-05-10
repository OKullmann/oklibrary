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

0. A customised Gecode brancher for Enumerative Lookahead.

1. Apply la-reduction in the status() function of customised branchers.
   - As a result, no redundant nodes are needed when la-reduction finds a leaf.
   - In the choice() function, where the the branching is chosen, no leafs
     are possible - and it should be checked.

2. Maintain branching-order.

3. Extend Node class.
   - Maintain node id and parent node id.
   - Check correctness of node id and parent node id.

*/

#ifndef LOOKAHEADBRANCHING_wXJWMxXz3R
#define LOOKAHEADBRANCHING_wXJWMxXz3R

#include <vector>
#include <algorithm>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <Numerics/FloatingPoint.hpp>
#include <Numerics/Tau.hpp>

#include "Conditions.hpp"
#include "Options.hpp"
#include "LookaheadReduction.hpp"

namespace LookaheadBranching {

  namespace FP = FloatingPoint;
  namespace GC = Gecode;
  namespace LR = LookaheadReduction;
  namespace OP = Options;
  namespace CD = Conditions;

  using size_t = CD::size_t;
  using signed_t = CD::signed_t;
  typedef std::vector<int> values_t;
  typedef std::vector<bool> binvalues_t;
  typedef FP::float80 float_t;
  typedef std::vector<float_t> vec_t;
  // A branching tuple, i.e. a tuple of distances:
  typedef std::vector<float_t> bt_t;
  typedef LR::BranchingStatus BrStatus;

  // Array of values of an integer variable:
  typedef GC::Int::IntView IntView;
  // Array of array of values of Gecode integer variables:
  typedef GC::ViewArray<IntView> IntViewArray;
  // Value iterator for an integer variable:
  typedef GC::IntVarValues IntVarValues;

  // size_t is used for sizes of Gecode arrays.
  // For a Gecode array, size() returns int, so the function
  // size_t tr(int size) was introduced to convert int to size_t.
  inline constexpr size_t tr(const int size, [[maybe_unused]] const size_t bound = 0) noexcept {
    assert(bound <= std::numeric_limits<int>::max());
    assert(size >= int(bound));
    return size;
  }

  // lookahead-distance.
  inline float_t distance(const GC::IntVarArray& V, const GC::IntVarArray& Vn,
                          const vec_t wghts, const size_t depth) noexcept {
    assert(not wghts.empty());
    float_t s = 0;
    const int N = V.size();
    for (int i = 0; i < N; ++i) {
      const auto ds = tr(V[i].size(), 1);
      const auto dsn = tr(Vn[i].size(), 1);
      if (dsn == ds) continue;
      assert(dsn < ds);
      if (dsn == 1) { // smaller domain has size 1, take depth into account:
        s += exp(wghts[0] * (float_t)depth);
      }
      else { // smaller domain has size >= 2:
        assert(dsn-1 < wghts.size());
        s += wghts[dsn-1];
      }
    }
    return s;
  }

  template<class CustomBranching>
  CustomBranching best_branching(
    std::vector<CustomBranching>& branchings) noexcept {
    assert(not branchings.empty());
    CustomBranching best_br;
    for (auto& br : branchings) {
      assert(br.status() == BrStatus::branching);
      br.calc_ltau();
      best_br = std::min(best_br, br);
    }
    return best_br;
  }

  template<class CustomBranching>
  std::vector<CustomBranching> best_branchings(
    std::vector<CustomBranching>& tau_brs) noexcept {
    std::vector<CustomBranching> branchings;
    assert(not tau_brs.empty());
    CustomBranching br = best_branching<CustomBranching>(tau_brs);
    branchings = {br};
    return branchings;
  }

  // A node in the backtracking tree. All classes that describe problems
  // (like TwoMOLS) should be inherited from this class.
  class Node : public GC::Space {
    // Node's id:
    //size_t ndid;
    // Parent node's id. For the root node, id is 1, while parent id == 0.
    //size_t prntid;
    // Node's depth in the backtracking tree:
    size_t dpth;

  public:
    //Node() : ndid(1), prntid(0), dpth(0) { assert(valid()); }
    Node() : dpth(0) {}

    size_t depth() const noexcept { return dpth; }
    //size_t id() const noexcept { assert(valid()); return ndid; }
    //size_t parentid() const noexcept { assert(valid()); return prntid; }

    /*
    void update_id(const size_t id, const size_t pid) noexcept {
      ndid = id;
      prntid = pid;
      assert(valid());
    }
    */
    void increment_depth() noexcept { ++dpth; }

    // Root node is a special case: id == 1, parent id == 0:
    /*bool valid() const noexcept {
      return dpth > 0;
      //return (ndid > prntid) and
      //        ( (ndid == 1 and prntid == 0) or (ndid > 1 and prntid > 0) );
    }*/
  };

  // Binary branching: for a given variable var at most two branches of the
  // kind var==val and var!=val.
  //  - var : variable.
  //  - value : variable value.
  //  - binvalues : a Boolean array of branches: true means var==value, false
  //    means var!=value. Possible arrays are: {}, {false}, {true},
  //    {false, true}.
  //  - tuple : branching tuple, where each element corresponds to the
  //    branch-distance.
  //  - ltau :  value of the ltau function for the branching tuple.
  struct BinBranching {
    int var;
    int value;
    binvalues_t binvalues;
    bt_t tuple;
    float_t ltau;
    LR::BranchingStatus brstatus;

    BinBranching(const int v=0, const int val=0,
                const binvalues_t binvls={}, const bt_t tpl={})
      : var(v), value(val), binvalues(binvls), tuple(tpl),
        ltau(FP::pinfinity) {
        assert(valid());
        // If branching of width 0, the problem is UNSAT:
        if (tuple.empty() and binvalues.empty()) {
          brstatus = LR::BranchingStatus::unsat;
        }
        // If at least one child-node problem is satisfiable:
        else if (tuple.size() != binvalues.size()) {
          brstatus = LR::BranchingStatus::sat;
        }
        // If branching of width 1:
        else if (tuple.size() == 1 and binvalues.size() == 1) {
          brstatus = LR::BranchingStatus::single;
        }
        // Two branches, neither unsat or sat:
        else brstatus = LR::BranchingStatus::branching;
        assert(valid());
    }

   bool valid() const noexcept {
      return var >= 0 and binvalues.size() <= 2 and ltau >= 0 and
      (binvalues.empty() or binvalues.size() == 1 or
       binvalues[0] != binvalues[1]) and
      tuple.size() <= binvalues.size();
    }

    bool operator <(const BinBranching& a) const noexcept {
      return ltau < a.ltau;
    }

    LR::BranchingStatus status() const noexcept { return brstatus; }

    void calc_ltau() noexcept {
      assert(valid());
      assert(not tuple.empty());
      ltau = Tau::ltau(tuple);
      assert(valid());
    }

    size_t branches_num() const noexcept {
      assert(valid());
      if (brstatus == LR::BranchingStatus::unsat) return 1;
      else return binvalues.size();
    }

  };

  template <class CustomisedBinBrancher>
  struct BinBranchingChoice : public GC::Choice {
    BinBranching br;
    size_t parentid;
    bool valid() const noexcept { return br.valid(); }
    BinBranchingChoice(const CustomisedBinBrancher& b,
                       const BinBranching& br = BinBranching(),
                       const size_t parentid = 0)
      : GC::Choice(b, br.branches_num()), br(br), parentid(parentid) {
        assert(valid());
      }
  };


  // A base customised brancher inherited from Gecode::Brancher.
  // It contains common functionality to reduce code duplications.
  // All lookahead customised branchers should be inherited from it.
  class BaseBrancher : public GC::Brancher {
  protected:
    // Array of variables:
    IntViewArray x;
    // Index of the first unassigned variable:
    mutable int start;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size() and x[s].size() >= 2;
    }
  public:
    bool valid() const noexcept { return valid(start, x); }

    BaseBrancher(const GC::Home home, const IntViewArray& x)
      : GC::Brancher(home), x(x), start(0) { assert(valid(start, x)); }
    BaseBrancher(GC::Space& home, BaseBrancher& b)
      : GC::Brancher(home,b), start(b.start) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    virtual bool status(const GC::Space&) const noexcept {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) noexcept {
      return new BinBranchingChoice<BaseBrancher>(*this);
    }

    template <class ModSpace>
    GC::ExecStatus commit(GC::Space& home, const GC::Choice&,
                          const unsigned) noexcept {
      ModSpace* m = &(static_cast<ModSpace&>(home));
      m->increment_depth();
      return GC::ES_OK;
    }

  };

  // A binary Loookahead brancher. For a variable var and its value val,
  // branching is formed by two branches: var==val and var!=val. The best
  // branching is chosen via the tau-function.
  template <class ModSpace>
  class BinLookahead : public BaseBrancher {
  public:

    using BaseBrancher::BaseBrancher;

    static void post(GC::Home home, const IntViewArray& x) noexcept {
      new (home) BinLookahead(home, x);
    }
    virtual GC::Brancher* copy(GC::Space& home) noexcept {
      return new (home) BinLookahead(home, *this);
    }

    // Apply lookahead-reduction:
    virtual bool status(GC::Space& home) noexcept {
      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->valid());
      assert(m->status() == GC::SS_BRANCH);
      assert(valid(start, x));
      // Find the first unassigned variable, if exists:
      bool issolved = true;
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; issolved = false; break; }
      // If all variables are assigned, no branching is needed:
      if (issolved) return false;
      // La-reduction parameters:
      const OP::RT rt = m->runtype();
      const GC::IntPropLevel pl = m->proplevel();
      const OP::LAR lar = m->laredtype();
      // Lookahead-reduction:
      LR::ReductionStatistics reduct_stat =
        LR::lareduction<ModSpace>(home, rt, pl, lar);
      // A leaf was found during the la-reduction, so no branching is needed:
      if (reduct_stat.leafcount() > 0) return false;
      // At least one unassigned variable, la-reduction hasn't found a leaf:
      return true;
    }

    virtual GC::Choice* choice(GC::Space& home) noexcept {
      ModSpace* m = &(static_cast<ModSpace&>(home));
      // Since la-reduction was called beforehand in the status() function,
      // no leaves can be found here, so only branching is chosen:
      assert(m->status() == GC::SS_BRANCH);
      const GC::IntPropLevel pl = m->proplevel();
      const vec_t wghts = m->weights();
      const size_t dpth = m->depth();
      std::vector<BinBranching> tau_brs;
      BinBranching best_br;
      // Form all branchings:
      assert(valid(start, x));
      for (signed_t var = start; var < x.size(); ++var) {
        const IntView view = x[var];
        if (view.assigned()) continue;
        assert(view.size() >= 2);
        for (IntVarValues j(view); j(); ++j) {
          const signed_t val = j.val();
          const auto subm_eq =
            LR::child_node<ModSpace>(m, var, val, pl, true);
          [[maybe_unused]] const auto subm_eq_st = subm_eq->status();
          // A leaf (SAT or UNSAT) is impossible here:
          assert(subm_eq_st == GC::SS_BRANCH);
          const float_t dist1 = distance(m->var(), subm_eq->var(), wghts, dpth);
          assert(dist1 > 0);
          const auto subm_neq =
            LR::child_node<ModSpace>(m, var, val, pl, false);
          [[maybe_unused]] const auto subm_neq_st = subm_neq->status();
          assert(subm_neq_st == GC::SS_BRANCH);
          const float_t dist2 = distance(m->var(), subm_neq->var(), wghts, dpth);
          assert(dist2 > 0);
          BinBranching br(var, val, {true,false}, {dist1,dist2});
          assert(br.status() == BrStatus::branching);
          tau_brs.push_back(br);
        }
      }
      assert(not tau_brs.empty());
      // Choose the best branchibg:
      best_br = best_branching<BinBranching>(tau_brs);

      [[maybe_unused]] const auto var = best_br.var;
      assert(var >= 0 and var >= start);
      assert(not x[var].assigned() or m->status() == GC::SS_FAILED);
      return new BinBranchingChoice<BinLookahead>(*this, best_br);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) noexcept {
      BaseBrancher::commit<ModSpace>(home, c, branch);
      [[maybe_unused]] ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      typedef BinBranchingChoice<BinLookahead> BrChoice;
      const BrChoice& brc = static_cast<const BrChoice&>(c);
      const BinBranching& br = brc.br;
      if (br.status() == BrStatus::unsat) return GC::ES_FAILED;
      const auto var = br.var;
      const auto& val = br.value;
      const auto& binvalues = br.binvalues;
      assert(var >= 0);
      assert(binvalues.size() == 1 or binvalues.size() == 2);
      assert(branch == 0 or branch == 1);
      assert(branch < binvalues.size());
      if ( (binvalues[branch] == true and GC::me_failed(x[var].eq(home, val))) or
           (binvalues[branch] == false and GC::me_failed(x[var].nq(home, val))) ) {
        return GC::ES_FAILED;
      }
      return GC::ES_OK;
    }

  };

  template <class ModSpace>
  inline void post_la_branching(ModSpace& s, GC::IntVarArgs V,
                                const OP::BRT brt) noexcept {
    GC::Home home = s;
    assert(not home.failed());
    const IntViewArray x(home, V);
    if (brt == OP::BRT::binbr) {
      BinLookahead<ModSpace>::post(home, x);
    }
    else if (brt == OP::BRT::enumbr) {
      // XXX
    }
  }

}

#endif
