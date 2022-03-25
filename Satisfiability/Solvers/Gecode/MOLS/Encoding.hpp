// Oliver Kullmann, 12.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Encoding the LS-MOLS-conditions, for Gecode

  The following Gecode constraints are enough for encoding all
  LS-MOLS-conditions parsed in Conditions.hpp:
  - distinct(IntVarArray X) : the all-different constraint for the array X.
  - element(IntVarArray X, IntVar y, IntVar z) : z is the y-th element of the
      array X, i.e. X_y = z.
  - rel(IntVar x, IntRelType irt, int val) : if irt is IRT_EQ, then x=val;
      if irt is IRT_NQ, then x!=val.

  Consider a Latin square A of order N.
  These three constraints can be used to encode that A is of special type:
  1. rls       : N distinct conditions - one for each row.
  2. cls       : N distinct conditions - one for each column.
  3. ls        : 2*N distinct conditions - one for each row and column.
  4. diag      : 1 distinct condition for the main diagonal.
  5. antidiag  : 1 distinct condition for the main antidiagonal.
  6. uni       : N rel-eq conditions on the main diagonal's elements.
  7. antiuni   : N rel-eq conditions on the main antidiagonal's elements.
  8. idem      : N rel-eq conditions on the main diagonal's elements.
  9. antiidem  : N rel-eq conditions on the main antidiagonal's elements.
  10. rred     : N rel-eq conditions on the first row's elements.
  11. orred    : N rel-eq conditions on the first row's elements.
  12. cred     : N rel-eq conditions on the first columns's elements.
  13. ocred    : N rel-eq conditions on the first columns's elements.
  14. symm     : 5 * N^2 element conditions.
                 for i,j=1,...N do
                   element(A[i], A[i,j], j) // equality to (1,3,2)-conjugate
                   element(A[j], i, A[i,j]) // equality to (2,1,3)-conjugate
                   element(A[j], A[i,j], i) // equality to (2,3,1)-conjugate
                   element(A[A[i,j]], i, j) // equality to (3,1,2)-conjugate
                   element(A[A[i,j]], j, i) // equality to (3,2,1)-conjugate
  15. antisymm : XXX

*/

#ifndef ENCODING_HqEmYk6s0p
#define ENCODING_HqEmYk6s0p

#include <vector>
#include <istream>

#include <gecode/int.hh>

#include "Conditions.hpp"
#include "PartialSquares.hpp"

namespace Encoding {

  namespace GC = Gecode;
  namespace CD = Conditions;
  namespace PS = PartialSquares;

  typedef CD::size_t size_t;


  struct EncCond {

    const CD::AConditions ac;
    const PS::PSquares ps;
    const size_t N;
    const size_t N2;
    const size_t num_vars;
    const GC::IntPropLevel pl;

    static bool valid(const size_t N) noexcept {
      return N >= 2 and N <= 10000;
    }

    EncCond(const CD::AConditions& ac,
            const PS::PSquares& ps,
            const GC::IntPropLevel pl = {}) noexcept
      : ac(ac), ps(ps), N(ps.N), N2(N*N), num_vars(ac.num_squares() * N2),
        pl(pl) { assert(valid(N)); }

    typedef GC::IntVarArray VA;
    typedef std::vector<GC::IntVar> vv_t;

    typedef GC::Space* SP;

    void post_unary(const VA& va, const SP s) const {
      assert(s);
      // Compilation-tests:
      vv_t vv;
      vv.push_back(va[0]);
      GC::distinct(*s, vv, pl); // just a compilation-test
      // XXX
    }
    void post_equations(const VA& va, const SP s) const {
      assert(s);
      // XXX
    }
    void post_prod_equations(const VA& va, const SP s) const {
      assert(s);
      // XXX
    }
    void post_psquares(const VA& va, const SP s) const {
      assert(s);
      // XXX
    }


    // The VA is default-constructed in the calling-class, and updated
    // by the result obtained from post:
    VA post(const SP s) const {
      assert(s);
      VA va(*s, num_vars, 0, N-1);
      post_unary(va, s);
      post_equations(va, s);
      post_prod_equations(va, s);
      post_psquares(va, s);
      return va;
    }


    typedef CD::Square Square;
    size_t index(const Square s, size_t i, size_t j) const noexcept {
      assert(ac.valid(s));
      assert(i < N and j < N);
      return ac.index(s) * N2 + i * N + j;
    }


    // The initial domain-restriction:
    PS::PSquares initdomrestr() const {
      PS::PSquares::psquares_t P; P.reserve(ac.num_squares());
      const auto f = PS::full_psquare(N);
      for (size_t i = 0; i < ac.versions().size(); ++i)
        for (const CD::VS v : ac.versions()[i].choices())
          P.emplace_back(f, Square{i,v});
      assert(P.size() == ac.num_squares());
      return {N, P};
    }

  };

}

#endif
