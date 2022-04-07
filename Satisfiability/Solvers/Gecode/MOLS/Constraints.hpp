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

namespace Constraints {

  namespace GC = Gecode;
  namespace EC = Encoding;

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
  struct LookaheadMols : GC::Space {
    typedef GC::IntVarArray VarVec;
    typedef GC::IntVar Var;
    VarVec V;
    LookaheadMols(const EC::EncCond& enc) {
      V = enc.post<VarVec, Var>(this);
    }
    GC::IntVar at(const size_t i) const noexcept { return V[i]; }
    GC::IntVarArray at() const noexcept { return V; }
  protected :
    LookaheadMols(LookaheadMols& gm) : Space(gm), V(gm.V) {
      V.update(*this, gm.V);
    }
    GC::Space* copy() { return new LookaheadMols(*this); }
  };

}

#endif
