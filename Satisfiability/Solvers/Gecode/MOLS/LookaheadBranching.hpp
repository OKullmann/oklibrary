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
#include <memory>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

namespace LookaheadBranching {

  namespace FP = FloatingPoint;
  namespace GC = Gecode;

  typedef std::uint64_t count_t;

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


}

#endif
