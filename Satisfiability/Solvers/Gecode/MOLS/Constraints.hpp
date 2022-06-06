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

1. Provide depth-etc-facilities
    - This can be done by providing a derived class from GenericMols0,
      which has a parameter "depth" (etc.) in it, and which the (pseudo-)
      copy-constructor copies and updates (increment the depth etc.).
    - The depth should also be a reported statistics (per node).

*/

#ifndef CONSTRAINTS_hBa0Xe3nKA
#define CONSTRAINTS_hBa0Xe3nKA

#include <gecode/int.hh>
#include <gecode/search.hh>

#include "Encoding.hpp"

namespace Constraints {

  namespace GC = Gecode;
  namespace EC = Encoding;


  struct GenericMols0 : GC::Space {
    typedef GC::IntVarArray VarVec;
    typedef GC::IntVar Var;
    VarVec V;
    GenericMols0(const EC::EncCond& enc) {
      V = enc.post<VarVec, Var>(this);
    }
  protected :
    GenericMols0(GenericMols0& gm) : GC::Space(gm) {
      V.update(*this, gm.V);
    }
    GC::Space* copy() override { return new GenericMols0(*this); }
  };


}

#endif
