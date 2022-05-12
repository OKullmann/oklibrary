// Oliver Kullmann, 16.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  The Gecode-constraint-classes

  Namespace Constraints, abbreviated "CT".

TODOS:

0. Remove LookaheadMols
    - This module does not include any look-ahead stuff (it is the other way
      around!).

*/

#ifndef CONSTRAINTS_hBa0Xe3nKA
#define CONSTRAINTS_hBa0Xe3nKA

#include <gecode/int.hh>
#include <gecode/search.hh>

#include "Encoding.hpp"
#include "Options.hpp"
#include "LookaheadBranching.hpp"

namespace Constraints {

  namespace GC = Gecode;
  namespace EC = Encoding;
  namespace OP = Options;
  namespace LB = LookaheadBranching;

  typedef EC::size_t size_t;


  // Pure Gecode-version:
  struct GenericMols0 : GC::Space {
    typedef GC::IntVarArray VarVec;
    typedef GC::IntVar Var;
    VarVec V;
    GenericMols0(const EC::EncCond& enc) {
      V = enc.post<VarVec, Var>(this);
    }
  protected :
    GenericMols0(GenericMols0& gm) : Space(gm), V(gm.V) {
      V.update(*this, gm.V);
    }
    // Pure virtual function inherited from GC::Space:
    GC::Space* copy() { return new GenericMols0(*this); }
  };

  // Lookahead-version:
  struct [[deprecated]] LookaheadMols : LB::Node {
    typedef GC::IntVarArray VarVec;
    typedef GC::IntVar Var;
    VarVec V;
    const OP::RT rt;
    const OP::GBO gbo;
    const OP::LAR lar;
    const LB::vec_t wghts;
    GC::IntPropLevel pl;
    LookaheadMols(const EC::EncCond& enc,
                  const OP::RT rt_,
                  const OP::GBO gbo_,
                  const OP::LAR lar_,
                  const LB::vec_t wghts_) :
      rt(rt_), gbo(gbo_), lar(lar_), wghts(wghts_) {
      assert(wghts.size() == enc.N-1);
      V = enc.post<VarVec, Var>(this);
      pl = enc.pl;
      assert(valid());
    }
    bool valid() const noexcept { return V.size() > 0 and not wghts.empty(); }
    bool valid(const size_t i) const noexcept {
      assert(valid());
      return i<LB::tr(V.size());
    }
    GC::IntVar var(const size_t i) const noexcept {
      assert(valid()); return V[i];
    }
    GC::IntVarArray var() const noexcept { assert(valid()); return V; }
    OP::RT runtype() const noexcept { assert(valid()); return rt; }
    GC::IntPropLevel proplevel() const noexcept { assert(valid()); return pl; }
    OP::GBO brorder() const noexcept { assert(valid()); return gbo; }
    OP::LAR laredtype() const noexcept { assert(valid()); return lar; }
    LB::vec_t weights() const noexcept { assert(valid()); return wghts; }
  protected :
    LookaheadMols(LookaheadMols& gm) : LB::Node(gm), V(gm.V), rt(gm.rt),
      gbo(gm.gbo), lar(gm.lar), wghts(gm.wghts) {
      V.update(*this, gm.V);
      assert(valid());
    }
    GC::Space* copy() { return new LookaheadMols(*this); }
  };

}

#endif
