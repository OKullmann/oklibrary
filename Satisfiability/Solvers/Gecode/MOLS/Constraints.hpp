// Oliver Kullmann, 16.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  The Gecode-constraint-classes

*/

#ifndef CONSTRAINTS_hBa0Xe3nKA
#define CONSTRAINTS_hBa0Xe3nKA

#include <gecode/int.hh>
#include <gecode/search.hh>

#include "Encoding.hpp"
#include "LookaheadBranching.hpp"

namespace Constraints {

  namespace GC = Gecode;
  namespace EC = Encoding;
  namespace LAB = LookaheadBranching;

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
  class LookaheadMols : public LAB::Node {
    typedef GC::IntVarArray VarVec;
    typedef GC::IntVar Var;
    VarVec V;
    LAB::vec_t wghts;
    LookaheadMols(LookaheadMols& gm) : LAB::Node(gm), V(gm.V), wghts(gm.wghts) {
      V.update(*this, gm.V);
    }
    GC::Space* copy() { return new LookaheadMols(*this); }
  public :
    LookaheadMols(const EC::EncCond& enc, const LAB::vec_t wghts_) :
      wghts(wghts_) {
      V = enc.post<VarVec, Var>(this);
    }
    GC::IntVar var(const size_t i) const noexcept { return V[i]; }
    GC::IntVarArray var() const noexcept { return V; }
    LAB::vec_t weights() const noexcept { return wghts; }
  };

}

#endif
