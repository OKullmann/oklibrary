// Oliver Kullmann, 12.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Encoding the LS-MOLS-conditions, for Gecode

*/

#ifndef ENCODING_HqEmYk6s0p
#define ENCODING_HqEmYk6s0p

#include <gecode/int.hh>

#include "Conditions.hpp"

namespace Encoding {

  namespace GC = Gecode;

  namespace CD = Conditions;

  typedef CD::size_t size_t;


  struct EncCond {
    const CD::AConditions ac;
    const size_t N;
    const size_t num_vars;

    static bool valid(const size_t N) noexcept {
      return N >= 2 and N <= 10'000;
    }

    explicit EncCond(CD::AConditions ac, const size_t N) noexcept
      : ac(ac), N(N), num_vars(ac.num_squares() * N^2) {
        assert(valid(N));
      }


    bool valid(GC::Home* const p, const GC::IntVarArray v) const noexcept {
      return p and v.size() >= 0 and size_t(v.size()) == num_vars;
    }
    void update(GC::Home* const p, const GC::IntVarArray v) noexcept {
      assert(valid(p,v));
      m = p; x = v;
    }

    void post() const {
      assert(valid(m,x));
      // XXX
    }

  private :

    GC::Home* m = nullptr;
    GC::IntVarArray x;
  };

}

#endif
