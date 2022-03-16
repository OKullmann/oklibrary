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

#include "../Lookahead.hpp"

#include "Conditions.hpp"
#include "Encoding.hpp"

namespace Constraints {

  namespace GC = Gecode;
  namespace LA = Lookahead;
  namespace CD = Conditions;
  namespace EC = Encoding;

  typedef EC::size_t size_t;


  class GenericMols : public LA::Node {

    GC::IntVarArray V;

  public :

    explicit GenericMols(const CD::AConditions ac, const size_t N,
                         const GC::IntPropLevel pl) noexcept {
      const EC::EncCond ec(ac, N, pl, this);
      V = ec.post();
    }

  };

}

#endif
