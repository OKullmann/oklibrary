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

#include <gecode/int.hh>

#include "Lookahead.hpp"

namespace Trivial {

  namespace LA = Lookahead;

  bool operator==(const Gecode::IntVarArray& lhs, const Gecode::IntVarArray& rhs) noexcept {
    if (lhs.size() != rhs.size()) return false;
    const auto size = LA::tr(lhs.size());
    for (LA::size_t i = 0; i < size; ++i) {
      if (lhs[i].size() != rhs[i].size()) return false;
      for (Gecode::IntVarValues jl(lhs[i]), jr(rhs[i]); jl(); ++jl, ++jr)
        if (jl.val() != jr.val()) return false;
    }
    return true;
  }

  class IntArr : public Gecode::Space {
  protected:
    Gecode::IntVarArray V;
  public:

    const LA::size_t sz, a, b;

    IntArr(const LA::size_t sz, const LA::size_t a, const LA::size_t b) :
      V(*this, sz, a, b), sz(sz), a(a), b(b) {}

    IntArr(IntArr& s) : Gecode::Space(s), sz(s.sz), a(s.a), b(s.b) {
      V.update(*this, s.V);
    }
    IntArr(const IntArr& s) : IntArr(s.sz, s.a, s.b) {}
    void update() { V.update(*this, V); }

    virtual Gecode::Space* copy() {
      return new IntArr(*this);
    }
    void print() const {
      std::cout << V << "\n";
    }
    LA::size_t size() const noexcept {
      return V.size();
    }

    double mu0() {
      return LA::mu0(V);
    }
    double mu1() {
      return LA::mu1(V);
    }

    friend bool operator ==(const IntArr& lhs, const IntArr& rhs) noexcept {
      return lhs.V == rhs.V;
    }
    friend bool operator !=(const IntArr& lhs, const IntArr& rhs) noexcept {
      return not (lhs.V == rhs.V);
    }

  };

}

#endif
