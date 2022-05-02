// Oliver Kullmann, 8.4.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Verifying solutions

  Namespace Verification, abbreviated "VR".

  Using
   - size_t = Conditions::size_t.
   - ls_row_t = BS::ls_row_t
   - ls_t = BS::ls_t

  Helper functions (taking a range RAN):

   - constant(const RAN& v) : whether all elements of v are equal.

  Extracting diagonal elements (arbitrary ls_t):

   - extract_diagonal(ls_t) -> ls_row_t
   - extract_antidiagonal(ls_t) -> ls_row_t.

  Various properties of ls_t:

   - diagonal, antidiagonal: whether the existing elements on the (anti-)
     diagonal fulfil alldiff (no restrictions on S)
   - unipotent, antiunipotent: whether the existing elements on the (anti-)
     diagonal are all equal (no restrictions on S).

   - idempotent(S): for N the number of rows of S, check whether all N diagonal
     elements are there and have the values 0, ..., N-1
   - antiidempotent(S): same condition, but the columns start at the right end
     (not at the left end, like for the diagonal).
     Again, no restrictions on S are made.

   - rreduced(S): whether the first row of S is standardised.
   - credcued(S): whether the first column is standardised
     (which may not exist)
   - reduced(S): both conditions together.
     No restrictions on S are made.

   - symmetric(S): false for not square-shaped S
   - antisymmetric(S): false for not square-shaped S

   - orthogonal(S1, S2): checks whether the superposition of S1, S2 has
     all entries different; false of S1, S2 are not of the same shape.

  Testing solutions:

   - valid(AConditions, PSquares): formal fitting

*/

#ifndef VERIFICATION_CewvYixXoa
#define VERIFICATION_CewvYixXoa

#include <map>
#include <utility>
#include <algorithm>

#include <cassert>

#include <ProgramOptions/Strings.hpp>
#include <Transformers/Generators/Random/LatinSquares.hpp>

#include "Conditions.hpp"
#include "PartialSquares.hpp"
#include "BasicLatinSquares.hpp"

namespace Verification {

  namespace CD = Conditions;
  namespace PS = PartialSquares;
  namespace BS = BasicLatinSquares;

  typedef CD::size_t size_t;
  typedef BS::ls_row_t ls_row_t;
  typedef BS::ls_t ls_t;


  template <class RAN>
  bool constant(const RAN& v) noexcept {
    auto it = v.begin();
    const auto end = v.end();
    if (it == end) return true;
    const auto& val = *it;
    return std::find_if(++it, end, [&val](const auto& x){return x != val;})
      == end;
  }


  ls_row_t extract_diagonal(const ls_t& S) {
    const size_t N = S.size();
    ls_row_t d;
    for (size_t i = 0; i < N; ++i) {
      const ls_row_t& r = S[i];
      if (i < r.size()) d.push_back(r[i]);
    }
    return d;
  }
  ls_row_t extract_antidiagonal(const ls_t& S) {
    if (S.empty()) return {};
    const size_t N = S.size();
    ls_row_t ad;
    for (size_t i = 0, j = N-1; i < N; ++i, --j) {
      const ls_row_t& r = S[i];
      if (j < r.size()) ad.push_back(r[j]);
    }
    return ad;
  }

  bool diagonal(const ls_t& S) {
    return BS::alldiffelem(extract_diagonal(S));
  }
  bool antidiagonal(const ls_t& S) {
    return BS::alldiffelem(extract_antidiagonal(S));
  }
  bool unipotent(const ls_t& S) {
    return constant(extract_diagonal(S));
  }
  bool antiunipotent(const ls_t& S) {
    return constant(extract_antidiagonal(S));
  }

  bool idempotent(const ls_t& S) {
    const size_t N = S.size();
    for (size_t i = 0; i < N; ++i) {
      const ls_row_t& r = S[i];
      if (i >= r.size() or r[i] != i) return false;
    }
    return true;
  }
  bool antiidempotent(const ls_t& S) {
    const size_t N = S.size();
    for (size_t i = 0, j = N-1; i < N; ++i, --j) {
      const ls_row_t& r = S[i];
      if (j >= r.size() or r[j] != i) return false;
    }
    return true;
  }

  bool rreduced(const ls_t& S) noexcept {
    if (S.empty()) return false;
    const auto& r = S[0];
    const size_t N = r.size();
    for (size_t i = 0; i < N; ++i)
      if (r[i] != i) return false;
    return true;
  }
  bool creduced(const ls_t& S) noexcept {
    const size_t N = S.size();
    for (size_t i = 0; i < N; ++i) {
      const auto& r = S[i];
      if (r.empty() or r[0] != i) return false;
    }
    return true;
  }
  bool reduced(const ls_t& S) noexcept {
    return rreduced(S) and creduced(S);
  }


  bool symmetric(const ls_t& S) noexcept {
    if (not BS::sqshape(S)) return false;
    const size_t N = S.size();
    if (N <= 1) return true;
    for (size_t i = 0; i < N-1; ++i) {
      const auto& r = S[i];
      for (size_t j = i+1; j < N; ++j)
        if (r[j] != S[j][i]) return false;
    }
    return true;
  }
  bool antisymmetric(const ls_t& S) noexcept {
    if (not BS::sqshape(S)) return false;
    const size_t N = S.size();
    if (N <= 1) return true;
    for (size_t i = 0; i < N-1; ++i)
      for (size_t j = 0; j < BS::refl(i, N); ++j)
        if (BS::apply(S, {i,j}) != BS::apply(S, BS::refl_ad({i,j}, N)))
          return false;
    return true;
  }


  bool orthogonal(const ls_t& S1, const ls_t& S2) {
    const size_t N = S1.size();
    if (S2.size() != N) return false;
    typedef std::pair<size_t,size_t> p_t;
    typedef std::map<p_t, size_t> m_t;
    m_t m;
    for (size_t i = 0; i < N; ++i) {
      const ls_row_t& row1 = S1[i];
      if (row1.size() != N) return false;
      const ls_row_t& row2 = S2[i];
      if (row2.size() != N) return false;
      for (size_t j = 0; j < N; ++j)
        if (++m[{row1[j], row2[j]}] == 2) return false;
    }
    return true;
  }


  // Testing whether the format of the alleged solution is correct:
  bool valid(const CD::AConditions& ac, const PS::PSquares& sol) noexcept {
    assert(sol.valid());
    const size_t K = ac.num_squares();
    if (sol.size() != K) return false;
    if (not sol.unit()) return false;
    for (size_t i = 0; i < K; ++i) {
      const PS::PSquare& sq = sol.psqs[i];
      const CD::Square s = sq.s;
      assert(ac.valid(s));
      assert(is_main_rep(s.v));
      if (ac.index(s) != i) return false;
    }
    return true;
  }

}

#endif
