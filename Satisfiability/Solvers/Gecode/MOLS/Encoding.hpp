// Oliver Kullmann, 12.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Encoding the LS-MOLS-conditions, for Gecode

1. Complete the encoding of the unary conditions:

  The following Gecode constraints are enough for encoding all
  LS-MOLS-conditions parsed in Conditions.hpp:
  - distinct(IntVarArray X) : the all-different constraint for the array X.
  - element(IntVarArray X, IntVar y, IntVar z) : z is the y-th element of the
      array X, i.e. X_y = z.
  - rel(IntVar x, IntRelType irt, int val) : if irt is IRT_EQ, then x=val;
      if irt is IRT_NQ, then x!=val.

  Question OZ: Tip 4.8 in MPG seems not relevant for us, since apparently
  it concerns only the effort in posting the constraints, which is
  negligible?

  Consider a Latin square A of order N.

    B = c132 A
  means
    B(i,j) = k <=> A(i,k) = j,
  and thus the equality is equivalent to the N^2 constraints
    element(A[i,*], B[i,j], j)
  for all 0 <= i,j < N.
  Alternatively one can use
    element(B[i,*], A[i,k], k).

    B = c312 A
  means
    B(i,j) = k <=> A(k,i) = j
  equivalent to
    element(A[*,i], B[i,j], j).
  Alternatively
    element(B[i,*], A[k,i], k).

    B = c231 A
  means
    B(i,j) = k <=> A(j,k) = i
  equivalent to
    element(A[j,*], B[i,j], i).
  Alternatively
    element(B[*,j], A[j,k], k).

    B = c321 A
  means
    B(i,j) = k <=> A(k,j) = i
  equivalent to
    element(A[*,j], B[i,j], i).
  Alternatively
    element(B[*,j], A[k,j], j).

  Perhaps these two versions are triggered by either using say
    B = c321 A
  or
    c321 A = B
  ?!


  It is be possible to use only 2 squares instead of 4 (as above),
  by reading off the other 2 by some permutation of the cells.
  Example:

    B1 = c132 A, B2 = c312 A => B2 = c312 c132^-1 B1 = c312 c132 B1
    = c321 B1, which is not a simple permutation.

    B1 = c132 A, B2 = c231 A => B2 = c231 c132^-1 B1 = c231 c132 B1
    = c213 B1 = B1^t.

    B1 = c312 A, B2 = c321 A => B2 = c321 c312^-1 A = c321 c231 A
    = c213 B1 = B1^t.

   So we have three groups: {c123, c213=c123^t},
                            {c132, c213=c132^t},
                            {c312, c321=c312^t}.


2. New unary conditions: "box"

   Let k be maximal with k^2 <= N, that is, k = floor(sqrt(N)).
   Beginning bottom-left ((0,0)), without gap partition the whole square into
   kxk boxes, and on each box pose an all-different constraint.
   If N is a square, then these are the (big) Sudoku boxes, and so together
   with the ls-condition we get the (big) Sudoku condition.

   Nor N=8 say we get 4*4=16 2x2 boxes (complete partition of the square),
   for N=7 we have 3*3=9 2x2 boxes, and the upper and right edge-cell are not
   covered.

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
        pl(pl) {
      assert(valid(N));
      assert(PS::valid(ps, ac));
    }

    typedef GC::IntVarArray VA;
    typedef std::vector<GC::IntVar> vv_t;

    typedef GC::Space* SP;

    void post_psquares(const VA& va, const SP s) const {
      assert(s);
      for (const PS::PSquare& ps : ps.psqs) {
        size_t ind = index(ps.s);
        for (size_t i = 0; i < N; ++i) {
          const auto& row = ps.ps[i];
          for (size_t j = 0; j < N; ++j, ++ind) {
            const auto& c = row[j].c;
            for (size_t k = 0; k < N; ++k)
              if (c[k]) {
                GC::rel(*s, va[ind], GC::IRT_NQ, k, pl);
              }
          }
        }
      }
    }
    void post_versions(const VA& va, const SP s) const {
      assert(s);
      // XXX
    }

    size_t t(const size_t i) const noexcept {
      assert(i < N);
      return N-i-1;
    }
    typedef std::pair<size_t, size_t> co_t; // coordinates in a square
    // Reflection at diagonal and/or antidiagonal:
    co_t t(const co_t x, const bool diag, const bool antidiag) noexcept {
      if (diag) {
        if (antidiag) return {t(x.first), t(x.second)};
        else return {x.second, x.first};
      }
      else {
        if (antidiag) return {t(x.second), t(x.first)};
        else return x;
      }
    }

    typedef CD::Square Square;
    void post_unary(const VA& va, const SP s) const {
      assert(s);
      using CD::UC;
      for (const auto& [uc, S] : ac.map())
        for (const Square sq : S.sqs()) {
          switch(uc) {
          case UC::rls : {
            for (size_t i = 0; i < N; ++i) { vv_t vv;
              for (size_t j = 0; j < N; ++j) vv.push_back(va[index(sq,i,j)]);
              GC::distinct(*s, vv, pl);
            } break; }
          case UC::cls : {
            for (size_t j = 0; j < N; ++j) { vv_t vv;
              for (size_t i = 0; i < N; ++i) vv.push_back(va[index(sq,i,j)]);
              GC::distinct(*s, vv, pl);
            } break; }
          case UC::ls : {
            for (size_t i = 0; i < N; ++i) { vv_t vv;
              for (size_t j = 0; j < N; ++j) vv.push_back(va[index(sq,i,j)]);
              GC::distinct(*s, vv, pl);
            }
            for (size_t j = 0; j < N; ++j) { vv_t vv;
              for (size_t i = 0; i < N; ++i) vv.push_back(va[index(sq,i,j)]);
              GC::distinct(*s, vv, pl);
            } break; }
          case UC::diag : { vv_t vv;
            for (size_t i = 0; i < N; ++i) vv.push_back(va[index(sq,i,i)]);
            GC::distinct(*s, vv, pl); break; }
          case UC::antidiag : { vv_t vv;
            for (size_t i = 0; i < N; ++i) vv.push_back(va[index(sq,i,t(i))]);
            GC::distinct(*s, vv, pl); break; }
          case UC::uni : { vv_t vv;
            for (size_t i = 0; i < N; ++i) vv.push_back(va[index(sq,i,i)]);
            GC::rel(*s, vv, GC::IRT_EQ, pl); break; }
          case UC::antiuni : { vv_t vv;
            for (size_t i = 0; i < N; ++i) vv.push_back(va[index(sq,i,t(i))]);
            GC::rel(*s, vv, GC::IRT_EQ, pl); break; }
          case UC::idem : {
            for (size_t i = 0; i < N; ++i)
              GC::rel(*s, va[index(sq,i,i)], GC::IRT_EQ, i, pl);
            break; }
          case UC::antiidem : {
            for (size_t i = 0; i < N; ++i)
              GC::rel(*s, va[index(sq,i,t(i))], GC::IRT_EQ, i, pl);
            break; }
          case UC::rred : {
            for (size_t i = 0; i < N; ++i)
              GC::rel(*s, va[index(sq,0,i)], GC::IRT_EQ, i, pl);
            break; }
          case UC::orred : {
            for (size_t i = 0; i < N; ++i)
              GC::rel(*s, va[index(sq,t(0),i)], GC::IRT_EQ, i, pl);
            break; }
          case UC::cred : {
            for (size_t i = 0; i < N; ++i)
              GC::rel(*s, va[index(sq,i,0)], GC::IRT_EQ, i, pl);
            break; }
          case UC::ocred : {
            for (size_t i = 0; i < N; ++i)
              GC::rel(*s, va[index(sq,i,t(0))], GC::IRT_EQ, i, pl);
            break; }
          case UC::red : {
            for (size_t i = 0; i < N; ++i)
              GC::rel(*s, va[index(sq,0,i)], GC::IRT_EQ, i, pl);
            for (size_t i = 0; i < N; ++i)
              GC::rel(*s, va[index(sq,i,0)], GC::IRT_EQ, i, pl);
            break; }
          case UC::ored : {
            for (size_t i = 0; i < N; ++i)
              GC::rel(*s, va[index(sq,t(0),i)], GC::IRT_EQ, i, pl);
            for (size_t i = 0; i < N; ++i)
              GC::rel(*s, va[index(sq,i,t(0))], GC::IRT_EQ, i, pl);
            break; }
          case UC::symm : {
            for (size_t i = 0; i < N-1; ++i)
              for (size_t j = i+1; j < N; ++j)
                GC::rel(*s, va[index(sq,i,j)], GC::IRT_EQ,
                            va[index(sq,j,i)], pl);
            break; }
          case UC::antisymm : {
            for (size_t i = 0; i < N-1; ++i)
              for (size_t j = 0; j < N-i-1; ++j)
                GC::rel(*s, va[index(sq,i,j)], GC::IRT_EQ,
                            va[index(sq,t(j),t(i))], pl);
            break; }

          default : throw std::runtime_error("ERROR[post_unary]: UNKNOWN uc="
                                             +std::to_string(size_t(uc)));}
        }
    }
    void post_equations(const VA& va, const SP s) const {
      assert(s);
      // XXX
    }
    void post_prod_equations(const VA& va, const SP s) const {
      assert(s);
      // XXX
    }


    // The VA is default-constructed in the calling-class, and updated
    // by the result obtained from post:
    VA post(const SP s) const {
      assert(s);
      VA va(*s, num_vars, 0, N-1);
      post_psquares(va, s);
      post_unary(va, s);
      post_equations(va, s);
      post_prod_equations(va, s);
      return va;
    }


    size_t index(const Square s) const noexcept {
      assert(ac.valid(s));
      return ac.index(s) * N2;
    }
    size_t index(const Square s, size_t i, size_t j) const noexcept {
      assert(i < N and j < N);
      return index(s) + i * N + j;
    }


    // The initial domain-restriction:
    PS::PSquares full_tass() const {
      PS::PSquares::psquares_t P; P.reserve(ac.num_squares());
      const auto f = PS::full_psquare(N);
      for (size_t i = 0; i < ac.versions().size(); ++i)
        for (const CD::VS v : ac.versions()[i].choices())
          P.emplace_back(f, Square{i,v});
      assert(P.size() == ac.num_squares());
      return {N, P};
    }

    PS::PSquares decode(const VA& va) const {
      assert(size_t(va.size()) == num_vars);
      PS::PSquares res = full_tass();
      size_t v = 0; // index for va
      for (PS::PSquare& s : res.psqs)
        for (size_t i = 0; i < N; ++i) {
          PS::prow_t& row = s.ps[i];
          for (size_t j = 0; j < N; ++j, ++v) {
            PS::Cell& c = row[j];
            for (GC::IntVarValues eps(va[v]); eps(); ++eps)
              c.c[eps.val()] = 0;
          }
        }
      assert(v == num_vars);
      return res;
    }

    static bool unit(const VA& va) noexcept {
      for (size_t i = 0; i < size_t(va.size()); ++i) {
        if (va[i].size() != 1) return false;
      }
      return true;
    }

  };

}

#endif
