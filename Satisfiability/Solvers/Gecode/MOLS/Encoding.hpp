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

#include <vector>

#include <gecode/int.hh>

#include "Conditions.hpp"

namespace Encoding {

  namespace GC = Gecode;
  namespace CD = Conditions;

  typedef CD::size_t size_t;


  struct EncCond {
    const CD::AConditions& ac;
    const size_t N;
    const size_t num_vars;
    const GC::IntPropLevel pl;
    typedef GC::Space* SP;
    const SP s;

    static bool valid(const size_t N) noexcept {
      return N >= 2 and N <= 10'000;
    }

    EncCond(const CD::AConditions& ac,
            const size_t N, const GC::IntPropLevel pl,
            const SP s) noexcept
      : ac(ac), N(N), num_vars(ac.num_squares() * N^2), pl(pl), s(s) {
      assert(valid(N)); assert(s);
    }


    typedef GC::IntVarArray VA;
    typedef std::vector<GC::IntVar> vv_t;

    void post_unary(const VA& va) const {

      // Compilation-tests:
      vv_t vv;
      vv.push_back(va[0]);
      GC::distinct(*s, vv, pl); // just a compilation-test

    }
    void post_equations(const VA& va) const {

    }
    void post_prod_equations(const VA& va) const {

    }


    // the VA is default-constructed in the calling-class, and updated
    // by the result obtained from post:
    VA post() const {
      VA va(*s, num_vars, 0, N-1);
      post_unary(va);
      post_equations(va);
      post_prod_equations(va);
      return va;
    }


    typedef CD::Square Square;
    size_t index(const Square s, size_t i, size_t j) const noexcept {
      assert(ac.valid(s));
      assert(i < N and j < N);
      // XXX
    }

  };

}

#endif
