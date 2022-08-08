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
    - In the choice-function the id is set, in commit then the clone
      is updated (obtaining the pid, depth, and branch-number).
    - DONE The root has id=1, and pid=0 (NIL).

*/

#ifndef CONSTRAINTS_hBa0Xe3nKA
#define CONSTRAINTS_hBa0Xe3nKA

#include <gecode/int.hh>
#include <gecode/search.hh>

#include "Encoding.hpp"
#include "Measures.hpp"

namespace Constraints {

  namespace GC = Gecode;
  namespace EC = Encoding;

  using size_t = EC::size_t;
  using float_t = Measures::float_t;


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
  std::ostream& operator <<(std::ostream& out, const NodeData& d) {
    return out << d.id << " " << d.pid << " " << d.depth;
  }

  struct GenericMols1 : GenericMols0 {
    using GenericMols0::VarVec;
    using GenericMols0::Var;
    using GenericMols0::V;

    GenericMols1(const EC::EncCond& enc) : GenericMols0(enc) {}
    NodeData nodedata() const noexcept { return nd; }
    size_t& idref() noexcept { return nd.id; }

    // To be called in commit:
    void update_clone() noexcept { nd.pid = nd.id; ++nd.depth; }

  protected :
    GenericMols1(GenericMols1& gm) : GenericMols0(gm), nd(gm.nd) {}
    GC::Space* copy() override { return new GenericMols1(*this); }

  private :
    NodeData nd;

  };


  struct NodeMeasures {
    float_t lestlvs;
    constexpr NodeMeasures() noexcept : lestlvs(0) {};
    constexpr bool operator ==(const NodeMeasures&) const noexcept = default;
  };
 std::ostream& operator <<(std::ostream& out, const NodeMeasures& d) {
    return out << d.lestlvs;
  }

  struct GenericMols2 : GenericMols1 {
    using GenericMols1::VarVec;
    using GenericMols1::Var;
    using GenericMols1::V;

    GenericMols2(const EC::EncCond& enc) : GenericMols1(enc) {}
    NodeMeasures nodemeasures() const noexcept { return nm; }

    // Assumes that GenericMols1::update_clone() is called separately:
    void update_clone(const float_t d) noexcept {
      nm.lestlvs += d;
    }

  protected :
    GenericMols2(GenericMols2& gm) : GenericMols1(gm), nm(gm.nm) {}
    GC::Space* copy() override { return new GenericMols2(*this); }

  private :
    NodeMeasures nm;

  };

}

#endif
