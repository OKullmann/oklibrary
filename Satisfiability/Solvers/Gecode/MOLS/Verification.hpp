// Oliver Kullmann, 8.4.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Verifying solutions

  Using size_t = Conditions::size_t.

  The fundamental types are
   - ls_row_t : vector of size_t
   - ls_t     : vector of ls_row_t

  As far as it is relatively easily done, the various functions treat these
  types according to their most general form, as concrete types, without
  further conditions: "ls" just announces the most important application.

  Helper function (taking a range RAN):

   - alldiffelement(RAN v) : whether all elements of v are different
   - constant(const RAN& v) : whether all elements of v are equal

     Extracting diagonal elements (arbitrary ls_t):
   - extract_(anti)diagonal(ls_t) -> ls_row_t

  Various properties of ls_t:

   - alldiffrows(const ls_t& S) : whether all rows of S fulfil alldiff
     (no restriction on S).
   - alldiffcols(const ls_t& S) : whether all (ragged) columns fulfil
     alldiff (again no restriction on S).
   - alldiffsq(const ls_t& S) : both conditions together.

   - sqshape(const ls_t& S): whether all rows of S have size N, where N
     is the number of rows.
   - sqval(const ls_t& S): whether all values are < N, where N is (again)
     the number of rows.
   - sqprop(const ls_t& S): both conditions together.
     All three tests make no assumptions on S.

   - diagonal, antidiagonal: whether the existing elements on the (anti-)
     diagonal fulfil alldiff (no restrictions on S)
   - unipotent, antiunipotent: whether the existing elements on the (anti-)
     diagonal are all equal (no restrictions on S).

   - idempotent(S): for N the number of rows of S, check whether all N diagonal
     elements are there and have the values 0, ..., N-1
   - antiidempotent(S): same condition, but the columns start at the right end
     (not at the left end, like for the diagonal).
     Again, no restrictions on S are made.

*/

#ifndef VERIFICATION_CewvYixXoa
#define VERIFICATION_CewvYixXoa

#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include <cassert>

#include <Transformers/Generators/Random/LatinSquares.hpp>

#include "Conditions.hpp"

namespace Verification {

  namespace RG = RandGen;
  namespace CD = Conditions;

  typedef Conditions::size_t size_t;

  typedef std::vector<size_t> ls_row_t;
  typedef std::vector<ls_row_t> ls_t;


  template <class RAN>
  bool alldiffelem(RAN v) {
    std::ranges::sort(v);
    return std::ranges::adjacent_find(v) == v.end();
  }
  template <class RAN>
  bool constant(const RAN& v) noexcept {
    auto it = v.begin();
    const auto end = v.end();
    if (it == end) return true;
    const auto& val = *it;
    return std::find_if(++it, end, [&val](const auto& x){return x != val;})
      == end;
  }


  bool alldiffrows(const ls_t& S) {
    return std::ranges::all_of(S,[](const auto& r){return alldiffelem(r);});
  }
  bool alldiffcols(const ls_t& S) {
    if (S.empty()) return true;
    const size_t N = std::ranges::max_element(S, {},
      [](const auto& r){return r.size();}) -> size();
    for (size_t j = 0; j < N; ++j) {
      ls_row_t col;
      for (const ls_row_t& r : S)
        if (j < r.size()) col.push_back(r[j]);
      if (not alldiffelem(std::move(col))) return false;
    }
    return true;
  }
  bool alldiffsq(const ls_t& S) {
    return alldiffrows(S) and alldiffcols(S);
  }


  bool sqshape(const ls_t& S) noexcept {
    const size_t N = S.size();
    return std::ranges::all_of(S, [&N](const auto& r){return r.size()==N;});
  }
  bool sqval(const ls_t& S) noexcept {
    const size_t N = S.size();
    return std::ranges::all_of(S, [&N](const auto& r){
      return std::ranges::all_of(r, [&N](const size_t x){return x < N;});});
  }
  bool sqprop(const ls_t& S) noexcept {
    return sqshape(S) and sqval(S);
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
    return alldiffelem(extract_diagonal(S));
  }
  bool antidiagonal(const ls_t& S) {
    return alldiffelem(extract_antidiagonal(S));
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
    if (S.empty()) return true;
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


  bool rls(const ls_t& S) noexcept {
    return sqprop(S) and alldiffrows(S);
  }
  bool cls(const ls_t& S) noexcept {
    return sqprop(S) and alldiffcols(S);
  }
  bool ls(const ls_t& S) noexcept {
    return sqprop(S) and alldiffsq(S);
  }


  bool symmetric(const ls_t& S) noexcept {
    const size_t N = S.size();
    if (N == 0) return true;
    if (not sqshape(S)) return false;
    if (N == 1) return true;
    for (size_t i = 0; i < N-1; ++i) {
      const auto& r = S[i];
      for (size_t j = i+1; j < N; ++j)
        if (r[j] != S[j][i]) return false;
    }
    return true;
  }


  void transpositionm(ls_t& S) noexcept {
    assert(sqshape(S));
    const size_t N = S.size();
    if (N <= 1) return;
    for (size_t i = 0; i < N-1; ++i) {
      auto& r = S[i];
      for (size_t j = i+1; j < N; ++j) std::swap(r[j], S[j][i]);
    }
  }
  ls_t transposition(ls_t S) {
    transpositionm(S);
    return S;
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


  // A * B row-wise (first B, then A):
  ls_t rproduct(const ls_t& A, const ls_t& B) {
    assert(rls(A) and rls(B));
    const size_t N = A.size();
    ls_t res(N, ls_row_t(N));
    for (size_t i = 0; i < N; ++i) {
      ls_row_t& resr = res[i], Ar = A[i], Br = B[i];
      for (size_t j = 0; j < N; ++j) resr[j] = Ar[Br[j]];
    }
    return res;
  }

  ls_t rinverse(const ls_t& A) {
    assert(rls(A));
    const size_t N = A.size();
    ls_t res(N, ls_row_t(N));
    for (size_t i = 0; i < N; ++i) {
      ls_row_t& resr = res[i], Ar = A[i];
      for (size_t j = 0; j < N; ++j) resr[Ar[j]] = j;
    }
    return res;
  }

}

#endif
