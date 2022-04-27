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

  These three types are "concrete alias types" for the whole directory
  Gecode/MOLS:
   - all values allowed in general
   - these are by definitions typedefs, and thus don't need to be
     imported, but can be redefined.

  Via
   - out(ostream&, ls_row_t, sep)
   - out(ostream&, ls_t, sep)
  one has simple line-oriented output, with "sep" separating the row-entries.

  Conversions:

   - sls2bls translates from LatinSquares::ls_t to ls_t
   - the extract-functions translate from partial squares, first producing
     (single) ls_t:
    - extract(PS::Cell)
    - extract(PS::prow_t)
    - extract(PS::psquare_t)
    - extract(PS::PSquare)
   - producing a std::vector<ls_t> from a "solution":
    - extract(PS::PSquares)
   - finally a std::vector<std::vector<ls_t>> (a list of "solutions"):
    - extract(std::vector<PS::PSquares>).

  Generators:

   - random_sq(N, RandGen_t&) produces a random NxN square with uniformly
     distributed entries from 0, ..., N-1
   - neutral_rls(N) has 0, ..., N-1 in its rows
   - neutral_cls(N) has 0, ..., N-1 in its columns
   - random_rls(N, RandGen_t&) produces a random row-ls's, with each
     row-permutation uniformly distributed
   - random_cls(N, RandGen_t&) accordingly for column-ls's
   - random_ls(N, RG::RandGen_t&, LS::StRLS) imports the main function
     from module LatinSquares, with LS::StRLS offering "row-reduced"
     and "reduced"
   - random_ortho_rls(N, g) produces a pair of random orthogonal rls's
   - random_ortho_cls(N, g) produces a pair of random orthogonal cls's.


  As far as it is relatively easily done, the various functions treat these
  types according to their most general form, as concrete types, without
  further conditions: "ls" just announces the most important application.

  Helper functions (taking a range RAN):

   - alldiffelement(RAN v) : whether all elements of v are different
   - constant(const RAN& v) : whether all elements of v are equal.

  Extracting diagonal elements (arbitrary ls_t):

   - extract_diagonal(ls_t) -> ls_row_t
   - extract_antidiagonal(ls_t) -> ls_row_t.

  Coordinate handling:

   - refl(i, N) : horizontal (i is row) resp. vertical reflextion (i is column)
   - struct coord, holding x, y of size_t
   - apply(ls_t, coord), applym(ls_t&, coord)
   - refl_d(v) : reflection at diagonal for coordinate-vector v
   - refl_ad(v, N) : reflection at antidiagonal.

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

   - rreduced(S): whether the first row of S is standardised.
   - whether the first column is standardised (which may not exist)
   - reduced(S): both conditions together.
     No restrictions on S are made.

   - symmetric(S): false for not square-shaped S
   - antisymmetric(S): false for not square-shaped S

   - orthogonal(S1, S2): checks whether the superposition of S1, S2 has
     all entries different; false of S1, S2 are not of the same shape.

   Three versions of latin squares, where without sqprop(S) false is returned:

   - rls: all rows are permutations
   - cls: all columns are permutations
   - ls: rls and cls.

   Transformations:

    - transpositionm(ls_t&)
    - transpositionm(ls_t) -> ls_t
    - antitranspositionm(ls_t&)
    - antitransposition(ls_t) -> ls_t.

   Algebra:

    - rproduct(ls_t, ls_t) : row-product; assumes sqprop(A, B)
    - cproduct(ls_t, ls_t) : column-product; assumes sqprop(A, B)
    - rinverse(ls_t): row-inverse; assumes rls(A)
    - cinverse(ls_t): column-inverse; assumes cls(A).

*/

#ifndef VERIFICATION_CewvYixXoa
#define VERIFICATION_CewvYixXoa

#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include <cassert>

#include <ProgramOptions/Strings.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>
#include <Transformers/Generators/Random/Distributions.hpp>
#include <Transformers/Generators/Random/Algorithms.hpp>
#include <Transformers/Generators/Random/LatinSquares.hpp>
#include <Transformers/Generators/Random/LSRG.hpp>

#include "Conditions.hpp"
#include "PartialSquares.hpp"

namespace Verification {

  namespace RG = RandGen;
  namespace LS = LatinSquares;

  namespace CD = Conditions;
  namespace PS = PartialSquares;

  typedef Conditions::size_t size_t;

  typedef std::vector<size_t> ls_row_t;
  typedef std::vector<ls_row_t> ls_t;

  // Conversion from LS::ls_t to ls_t, that is,
  // "small" ls_t to big ls_t (LS::ls_t uses 32-bits for the cells):
  ls_t sls2bls(const LS::ls_t& S) {
    const size_t N = S.size();
    ls_t res(N);
    for (size_t i = 0; i < N ; ++i)
      res[i].assign(S[i].begin(), S[i].end());
    return res;
  }

  // Conversion from partial squares to ls_t:
  size_t extract(const PS::Cell& c) noexcept {
    return c.first();
  }
  ls_row_t extract(const PS::prow_t& r) {
    ls_row_t res; res.reserve(r.size());
    for (const PS::Cell& c : r) res.push_back(extract(c));
    return res;
  }
  ls_t extract(const PS::psquare_t& s) {
    ls_t res; res.reserve(s.size());
    for (const PS::prow_t& r : s) res.push_back(extract(r));
    return res;
  }
  ls_t extract(const PS::PSquare& s) {
    return extract(s.ps);
  }
  std::vector<ls_t> extract(const PS::PSquares& S) {
    std::vector<ls_t> res; res.reserve(S.psqs.size());
    for (const PS::PSquare& s : S.psqs) res.push_back(extract(s));
    return res;
  }
  std::vector<std::vector<ls_t>> extract(const std::vector<PS::PSquares>& V) {
    std::vector<std::vector<ls_t>> res; res.reserve(V.size());
    for (const PS::PSquares& S : V) res.push_back(extract(S));
    return res;
  }


  void out(std::ostream& out, const ls_row_t& r, const std::string& sep=" ") {
    Environment::out_line(out, r, sep);
  }
  void out(std::ostream& out, const ls_t& S, const std::string& sep = " ") {
    Environment::out_lines(out, S, "\n", sep);
  }


  ls_t random_sq(const size_t N, RG::RandGen_t& g) {
    ls_t S(N, ls_row_t(N));
    RandGen::UniformRange u(g, N);
    for (size_t i = 0; i < N; ++i) {
      ls_row_t& r = S[i];
      for (size_t& x : r) x = u();
    }
    return S;
  }
  ls_t neutral_rls(const size_t N) {
    ls_t S(N, ls_row_t(N));
    for (size_t i = 0; i < N; ++i) {
      ls_row_t& r = S[i];
      for (size_t j = 1; j < N; ++j) r[j] = j;
    }
    return S;
  }
  ls_t neutral_cls(const size_t N) {
    ls_t S(N, ls_row_t(N));
    for (size_t i = 1; i < N; ++i) {
      ls_row_t& r = S[i];
      for (size_t j = 0; j < N; ++j) r[j] = i;
    }
    return S;
  }
  ls_t random_rls(const size_t N, RG::RandGen_t& g) {
    ls_t S = neutral_rls(N);
    for (ls_row_t& r : S) RG::shuffle(r.begin(), r.end(), g);
    return S;
  }
  ls_t random_cls(const size_t, RG::RandGen_t&);

  ls_t random_ls(const size_t N, RG::RandGen_t& g,
                 const LS::StRLS s = LS::StRLS::none) {
    return sls2bls(LSRG::random_ls(N, N, LSRG::GenO::majm, s, g));
  }


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


  bool rls(const ls_t& S) noexcept {
    return sqprop(S) and alldiffrows(S);
  }
  bool cls(const ls_t& S) noexcept {
    return sqprop(S) and alldiffcols(S);
  }
  bool ls(const ls_t& S) noexcept {
    return sqprop(S) and alldiffsq(S);
  }


  struct coord {
    size_t x, y;
    bool operator ==(const coord&) const noexcept = default;
    auto operator <=>(const coord&) const noexcept = default;
  };
  size_t apply(const ls_t& S, const coord v) noexcept {
    assert(v.x < S.size()); assert(v.y < S[v.x].size());
    return S[v.x][v.y];
  }
  size_t& applym(ls_t& S, const coord v) noexcept {
    assert(v.x < S.size()); assert(v.y < S[v.x].size());
    return S[v.x][v.y];
  }

  // Horizontal resp. vertical reflection:
  constexpr size_t refl(const size_t i, const size_t N) noexcept {
    assert(i < N);
    return (N-1) - i;
  }
  constexpr coord refl_d(const coord v) noexcept {
    return {v.y, v.x};
  }
  constexpr coord refl_ad(const coord v, const size_t N) noexcept {
    return {refl(v.y, N), refl(v.x, N)};
  }


  bool symmetric(const ls_t& S) noexcept {
    if (not sqshape(S)) return false;
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
    if (not sqshape(S)) return false;
    const size_t N = S.size();
    if (N <= 1) return true;
    for (size_t i = 0; i < N-1; ++i)
      for (size_t j = 0; j < refl(i, N); ++j)
        if (apply(S, {i,j}) != apply(S, refl_ad({i,j}, N))) return false;
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
  void antitranspositionm(ls_t& S) noexcept {
    assert(sqshape(S));
    const size_t N = S.size();
    if (N <= 1) return;
    for (size_t i = 0; i < N-1; ++i)
      for (size_t j = 0; j < refl(i, N); ++j)
        std::swap(applym(S, {i,j}), applym(S, refl_ad({i,j}, N)));
  }
  ls_t antitransposition(ls_t S) {
    antitranspositionm(S);
    return S;
  }


  ls_t random_cls(const size_t N, RG::RandGen_t& g) {
    return transposition(random_rls(N,g));
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
    assert(sqprop(A) and sqprop(B));
    const size_t N = A.size();
    assert(B.size() == N);
    ls_t res(N, ls_row_t(N));
    for (size_t i = 0; i < N; ++i) {
      ls_row_t& resr = res[i], Ar = A[i], Br = B[i];
      for (size_t j = 0; j < N; ++j) resr[j] = Ar[Br[j]];
    }
    return res;
  }
  // A * B column-wise (first B, then A):
  ls_t cproduct(const ls_t& A, const ls_t& B) {
    assert(sqprop(A) and sqprop(B));
    const size_t N = A.size();
    assert(B.size() == N);
    ls_t res(N, ls_row_t(N));
    for (size_t j = 0; j < N; ++j)
      for (size_t i = 0; i < N; ++i) res[i][j] = A[B[i][j]][j];
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
  ls_t cinverse(const ls_t& A) {
    assert(cls(A));
    const size_t N = A.size();
    ls_t res(N, ls_row_t(N));
    for (size_t j = 0; j < N; ++j)
      for (size_t i = 0; i < N; ++i) res[A[i][j]][j] = i;
    return res;
  }

  std::pair<ls_t,ls_t> random_ortho_rls(const size_t N, RG::RandGen_t& g) {
    const ls_t A = random_rls(N,g);
    return {A, rproduct(random_ls(N,g), A)};
  }
  std::pair<ls_t,ls_t> random_ortho_cls(const size_t N, RG::RandGen_t& g) {
    const ls_t A = random_cls(N,g);
    return {A, cproduct(random_ls(N,g), A)};
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
