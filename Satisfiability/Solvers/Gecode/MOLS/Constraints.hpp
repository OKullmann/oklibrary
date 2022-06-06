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
      This is GenericMols1.

    - This is done so in Lookahead::Node, containing also the node-id and
      the parent-node-it.
    - However that class doesn't use the copy-mechanism, but relies
      on manual updates in the commit-function.
      The point here seems to be that the commit-function already acts on the
      new "clone" which is the new node, while the copy-function in general
      is used for several purposes (we use it for the lookahead).

    - The depth should also be a reported statistics (per node).
    - Such additional node-data likely should not need a destructor, and
      should be packaged into one structure.
    - This is NodeData: the rood has id=1, and pid=0 (NIL).
    - Since we use Gecode-parallelism, we can not simply take the current
      node-count as id, without further interfering with parallelism.
    - The path is a valid id, which can be extended for each branch by
      branching-variable and index of branch, but that is non-elementary
      data which we try to avoid.
    - Though having an atomic running index should be harmless; only
      increment and reading is needed, which can be handled lock-free.
    - One could also say that when submitting the statistics stats0
      (for the la-reduction, which contains the node-count), it is cheap
      to return the current total node-count, which can be used for the id.
      This happens in the choice-function, and needed then, similar to
      the assignment found in the la-reduction, be put into the
      branching-object (created by create_la).
      The problem here is the barrier between choice and commit.

*/

#ifndef CONSTRAINTS_hBa0Xe3nKA
#define CONSTRAINTS_hBa0Xe3nKA

#include <gecode/int.hh>
#include <gecode/search.hh>

#include "Encoding.hpp"

namespace Constraints {

  namespace GC = Gecode;
  namespace EC = Encoding;

  using size_t = EC::size_t;


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


  struct NodeData {
    size_t id, pid, depth;
    constexpr NodeData() noexcept : id(1), pid(0), depth(0) {};
    constexpr bool operator ==(const NodeData&) const noexcept = default;
  };

  struct GenericMols1 : GenericMols0 {
    using GenericMols0::VarVec;
    using GenericMols0::Var;
    using GenericMols0::V;
    NodeData nd;

    GenericMols1(const EC::EncCond& enc) : GenericMols0(enc) {}

  protected :
    GenericMols1(GenericMols1& gm) : GenericMols0(gm), nd(gm.nd) {}
    GC::Space* copy() override { return new GenericMols1(*this); }

  };

}

#endif
