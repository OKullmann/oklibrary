// Oleg Zaikin, 31.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

Look-ahead branching for the Gecode library.

BUGS:

TODOS:

0. A Gecode brancher for Binary Lookahead.

1. A Gecode brancher for Enumerative Lookahead.

*/

#ifndef LOOKAHEADBRANCHING_wXJWMxXz3R
#define LOOKAHEADBRANCHING_wXJWMxXz3R

#include <vector>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <Numerics/FloatingPoint.hpp>
#include <Numerics/Tau.hpp>

#include "LookaheadReduction.hpp"

namespace LookaheadBranching {

  namespace FP = FloatingPoint;
  namespace GC = Gecode;
  namespace LR = LookaheadReduction;

  typedef std::uint64_t count_t;
  typedef std::vector<int> values_t;
  typedef std::vector<bool> binvalues_t;
  typedef FP::float80 float_t;
  // A branching tuple, i.e. a tuple of distances:
  typedef std::vector<float_t> bt_t;

  // Array of values of an integer variable:
  typedef GC::Int::IntView IntView;
  // Array of array of values of Gecode integer variables:
  typedef GC::ViewArray<IntView> IntViewArray;
  // Value iterator for an integer variable:
  typedef GC::IntVarValues IntVarValues;

  // A node in the backtracking tree. All classes that describe problems
  // (like TwoMOLS) should be inherited from this class.
  class Node : public GC::Space {
    // Node's id:
    count_t ndid;
    // Parent node's id. For the root node, id is 1, while parent id == 0.
    count_t prntid;
    // Node's depth in the backtracking tree:
    count_t dpth;

  public:
    Node() : ndid(1), prntid(0), dpth(0) { assert(valid()); }

    count_t depth() const noexcept { assert(valid()); return dpth; }
    count_t id() const noexcept { assert(valid()); return ndid; }
    count_t parentid() const noexcept { assert(valid()); return prntid; }

    void update_id(const count_t id, const count_t pid) noexcept {
      ndid = id;
      prntid = pid;
      assert(valid());
    }
    void increment_depth() noexcept { ++dpth; assert(valid()); }

    // Root node is a special case: id == 1, parent id == 0:
    bool valid() const noexcept {
      return (ndid > prntid) and
              ( (ndid == 1 and prntid == 0) or (ndid > 1 and prntid > 0) );
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
      return s >= 0 and valid(x) and s < x.size();
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

    // Update start - the first unassigned variable:
    virtual bool status(const GC::Space&) const noexcept {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    template <class ModSpace>
    GC::ExecStatus commit(GC::Space& home, const GC::Choice&,
                          const unsigned) noexcept {
      ModSpace* m = &(static_cast<ModSpace&>(home));
      m->increment_depth();
      return GC::ES_OK;
    }

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


}

#endif
