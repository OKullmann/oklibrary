// Oleg Zaikin, 7.4.2021 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A trivial class IntArr inherited from Gecode::Space.

  For arguments (sz, a, b) an object carrying an integer-array V of size sz and
  with values in {a,...,b} is created.


  TODOS:

  0. Documentation is needed (with references to the appropriate parts of
     the documentation.

  1. DONE Namespace needed.

  2. The copy-constructor is faulty.
    - If this misuse of a copy-constructor is part of the Gecode-library,
      one needs to see how to work around that.

  3. Leaking memory
    - A concept of ownership is needed.

*/

#ifndef TRIVIAL_UeAozKZjBa
#define TRIVIAL_UeAozKZjBa

#include <cstdint>

#include <gecode/int.hh>

#include "Lookahead.hpp"

namespace Trivial {

  namespace LA = Lookahead;

  class IntArr : public Gecode::Space {
  protected:
    Gecode::IntVarArray V;
  public:

    IntArr(const std::uint64_t sz, const std::uint64_t a, const std::uint64_t b) : V(*this, sz, a, b) {}

    IntArr(IntArr& s) : Gecode::Space(s) {
      V.update(*this, s.V);
    }

    virtual Gecode::Space* copy() {
      return new IntArr(*this);
    }
    void print() const {
      std::cout << V << "\n";
    }
    std::uint64_t size() const noexcept {
      return V.size();
    }

    LA::sz_t mu0() {
      return LA::mu0(V);
    }

  };

}

#endif
