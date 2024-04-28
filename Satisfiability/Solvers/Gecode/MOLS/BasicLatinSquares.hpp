// Oliver Kullmann, 1.5.2022 (Swansea)
/* Copyright 2022, 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Components around std::vector<std::vector<size_t>>

  Namespace BasicLatinSquares, abbreviated "BS".

  The fundamental types are
   - ls_row_t : vector of size_t
   - ls_t     : vector of ls_row_t

  These three types are "concrete alias types" for the whole directory
  Gecode/MOLS:
   - all values allowed in general
   - these are by definitions typedefs, and thus don't need to be
     imported, but can be redefined.

  As far as it is relatively easily done, the various functions treat these
  types according to their most general form, as concrete types, without
  further conditions: "ls" just announces the most important application.

  Via
   - out(ostream&, ls_row_t,string  sep)
   - out(ostream&, ls_t, string sep)
  one has simple line-oriented output, with "sep" separating the row-entries.
  Also out for vector<ls_t> and vector<vector<ls_t>>
  Via
   - in_strictls(istream&, char sep) -> ls_t
  one has simple line-oriented input, with empty lines result in ls_t{}.

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

  Helper functions (taking a range RAN):

   - alldiffelement(RAN v) : whether all elements of v are different

  Basic properties:

   - alldiffrows(const ls_t& S) : whether all rows of S fulfil alldiff
     (no restriction on S).
   - alldiffcols(const ls_t& S) : whether all (ragged) columns fulfil
     alldiff (again no restriction on S).
   - alldiffsq(const ls_t& S) : both conditions together.

   - sqshape(const ls_t& S): whether all rows of S have size N, where N
     is the number of rows.
   - rcshape(S): more generally, whether S's shape is rectangular.
   - sqval(const ls_t& S): whether all values are < N, where N is (again)
     the number of rows.
   - sqprop(const ls_t& S): both conditions together.

   - rcyclic(const ls_t& S) : whether all rows can be obtained by cyclic shifts
     of the first row, using the first possible match of the first row
     in the other rows.

     All these tests make no assumptions on S.

   - ccylcic(const ls_t& S) : translating columns into rows via gtransposition
     (below, assuming rcshape(S)), and applying rcyclic
   - dcyclic(const ls_t& S) : cyclic on the diagonals (assuming sqshape(S))
   - adcyclic(const ls_t& S) : cyclic on the antidiagonals
     (assuming sqshape(S))
   - typedef cyclicity_t = array of 4 bools (for r/c/d/ad-cyclicity)
   - cyclicity(const ls_t& S) -> cyclicity_t.

  Three versions of latin squares, where without sqprop(S) false is returned:

   - rls: all rows are permutations
   - cls: all columns are permutations
   - ls: rls and cls
   - pandiagonal: ls, and also all modular diagonals and antidiagonals are
     permutations.

  Coordinate handling:

   - struct coord, holding x, y of size_t
   - apply(ls_t, coord), applym(ls_t&, coord)

   - refl(i, N) : horizontal (i is row) resp. vertical reflextion (i is column)
   - refl_d(v) : reflection at diagonal for coordinate-vector v
   - refl_ad(v, N) : reflection at antidiagonal.

  Transformations:

   - transpositionm(ls_t& S) for sqshape(S)
   - transposition(ls_t) -> ls_t (using transpositionm)
   - gtransposition(ls_t) -> ls_t, assuming only rcshape(S)
   - antitranspositionm(ls_t&) for sqshape(S)
   - antitransposition(ls_t) -> ls_t for sqshape(S).

     The following six transformations assume sqshape(S):

     Two helper transformations to reduce the problem of being cyclic in
     the diagonals or antidiagonals to the cyclicity of rows:
   - moddiags2rows(ls_t) -> ls_t (writing the broken diagonals into the rows)
   - modantidiags2rows(ls_t) -> ls_t (writing the broken antidiagonals into
     the rows)

     The symmetries lf the square which map {rows,columns,diagonals,
     antidiagonals} to itself:
   - shift(ls_t, size_t, size_t) -> ls_t
   - negationj(ls_t) -> ls_t (reflection at the first column)
   - sumdiff(ls_t) -> ls_t (swapping rows with diagonals, and columns with
     antidiagonals)
   - scaling(ls_t, size_t) -> ls_t.

   - rstandardisem(ls_t& S) for sqval(S)
   - rstandardise(ls_t) -> ls_t.

   Algebra:

    - rproduct(ls_t, ls_t) : row-product; assumes sqprop(A, B)
    - cproduct(ls_t, ls_t) : column-product; assumes sqprop(A, B)
    - rinverse(ls_t): row-inverse; assumes rls(A)
    - cinverse(ls_t): column-inverse; assumes cls(A).

TODOS:

*/

#ifndef BASICLATINSQUARES_MRVxVvSSRs
#define BASICLATINSQUARES_MRVxVvSSRs

#include <vector>
#include <ostream>
#include <string>
#include <algorithm>
#include <utility>
#include <istream>
#include <map>

#include <ProgramOptions/Strings.hpp>
#include <Numerics/NumInOut.hpp>

#include <Transformers/Generators/Random/Numbers.hpp>
#include <Transformers/Generators/Random/Distributions.hpp>
#include <Transformers/Generators/Random/Algorithms.hpp>

#include <Transformers/Generators/Random/LatinSquares.hpp>
#include <Transformers/Generators/Random/LSRG.hpp>

#include "Conditions.hpp"
#include "PartialSquares.hpp"

namespace BasicLatinSquares {

  namespace CD = Conditions;
  namespace LS = LatinSquares;
  namespace PS = PartialSquares;
  namespace RG = RandGen;

  using size_t = CD::size_t;
  using coord = PS::coord;

  typedef std::vector<size_t> ls_row_t;
  typedef std::vector<ls_row_t> ls_t;


  void out(std::ostream& out, const ls_row_t& r, const std::string& sep=" ") {
    Environment::out_line(out, r, sep);
  }
  void out(std::ostream& out, const ls_t& S, const std::string& sep = " ") {
    Environment::out_lines(out, S, "\n", sep);
  }
  void out(std::ostream& o, const std::vector<ls_t>& S,
           const std::string& sep = " ") {
    for (const ls_t& L : S) out(o, L, sep);
  }
  void out(std::ostream& o, const std::vector<std::vector<ls_t>>& V,
           const std::string& sep = " ") {
    for (const auto& S : V) {out(o, S, sep); o << "\n";}
  }

  // Assuming each row on its own line, but allowing for a spaces-only line
  // (possibly empty, also possibly without eol), which results in
  // the empty ls_t; in.eof() at end iff last line not concluded by eol;
  // the number of rows is equal to the number of entries on the first line:
  ls_t in_strictls(std::istream& in, const char sep = ' ') {
    assert(in.good());
    std::string line;
    std::getline(in, line);
    ls_t res;
    res.push_back(FloatingPoint::to_vec_unsigned<size_t>(line, sep));
    size_t N = res[0].size();
    if (N == 0) return {};
    res.reserve(N);
    while (--N != 0) {
      std::getline(in, line);
      res.push_back(FloatingPoint::to_vec_unsigned<size_t>(line, sep));
    }
    return res;
  }


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


  bool rcshape(const ls_t& S) noexcept {
    if (S.empty()) return true;
    const size_t N = S[0].size();
    return std::all_of(S.begin()+1, S.end(),
                       [&N](const auto& r){return r.size()==N;});
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


  bool rls(const ls_t& S) noexcept {
    return sqprop(S) and alldiffrows(S);
  }
  bool rlsm1(const ls_t& S) noexcept {
    if (not sqprop(S)) return false;
    const size_t size = S.size();
    if (size <= 1) return true;
    for (size_t i = 0; i < size-1; ++i)
        if (not alldiffelem(S[i])) return false;
    return true;
  }
  bool rlsm2(const ls_t& S) noexcept {
    if (not sqprop(S)) return false;
    const size_t size = S.size();
    if (size <= 2) return true;
    for (size_t i = 0; i < size-2; ++i)
        if (not alldiffelem(S[i])) return false;
    return true;
  }
  bool cls(const ls_t& S) noexcept {
    return sqprop(S) and alldiffcols(S);
  }
  bool clsm1(const ls_t& S) noexcept {
    if (not sqprop(S)) return false;
    if (S.empty()) return true;
    const size_t N = std::ranges::max_element(S, {},
      [](const auto& r){return r.size();}) -> size();
    if (N <= 1) return true;
    for (size_t j = 0; j < N-1; ++j) {
      ls_row_t col;
      for (const ls_row_t& r : S)
        if (j < r.size()) col.push_back(r[j]);
      if (not alldiffelem(std::move(col))) return false;
    }
    return true;
  }
  bool clsm2(const ls_t& S) noexcept {
    if (not sqprop(S)) return false;
    if (S.empty()) return true;
    const size_t N = std::ranges::max_element(S, {},
      [](const auto& r){return r.size();}) -> size();
    if (N <= 2) return true;
    for (size_t j = 0; j < N-2; ++j) {
      ls_row_t col;
      for (const ls_row_t& r : S)
        if (j < r.size()) col.push_back(r[j]);
      if (not alldiffelem(std::move(col))) return false;
    }
    return true;
  }
  bool ls(const ls_t& S) noexcept {
    return sqprop(S) and alldiffsq(S);
  }


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
  // Transposition for rectangular shapges:
  ls_t gtransposition(const ls_t& S) {
    assert(rcshape(S));
    if (S.empty()) return {};
    const size_t M = S[0].size();
    const size_t N = S.size();
    ls_t res(M, ls_row_t(N));
    for (size_t i = 0; i < N; ++i) {
      const ls_row_t& r = S[i];
      for (size_t j = 0; j < M; ++j)
        res[j][i] = r[j];
    }
    return res;
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

  ls_t moddiags2rows(const ls_t& S) {
    assert(sqshape(S));
    const size_t N = S.size();
    ls_t res(N, ls_row_t(N));
    for (size_t d = 0; d < N; ++d) {
      ls_row_t& R = res[d]; // diagonal d -> R
      for (size_t i = 0; i < N; ++i) { // j - i = d mod N
        const size_t j = (d + i) % N;
        R[i] = S[i][j];
      }
    }
    return res;
  }
  ls_t modantidiags2rows(const ls_t& S) {
    assert(sqshape(S));
    const size_t N = S.size();
    ls_t res(N, ls_row_t(N));
    for (size_t s = 0; s < N; ++s) {
      ls_row_t& R = res[(N-1) - s]; // antidiagonal s -> R
      for (size_t i = 0; i < N; ++i) { // i + j = s mod N
        const size_t j = (s + (N - i)) % N;
        R[i] = S[i][j];
      }
    }
    return res;
  }

  ls_t shift(const ls_t& S, const size_t x, const size_t y) {
    assert(sqshape(S));
    const size_t N = S.size();
    ls_t res(N, ls_row_t(N));
    for (size_t i = 0; i < N; ++i) {
      const ls_row_t& R0 = S[i];
      const size_t i2 = (i + x) % N;
      ls_row_t& R = res[i2];
      for (size_t j = 0; j < N; ++j) {
        const size_t j2 = (j + y) % N;
        R[j2] = R0[j];
      }
    }
    return res;
  }
  ls_t negationj(const ls_t& S) {
    assert(sqshape(S));
    const size_t N = S.size();
    ls_t res(N, ls_row_t(N));
    for (size_t i = 0; i < N; ++i) {
      const ls_row_t& R0 = S[i];
      ls_row_t& R = res[i];
      for (size_t j = 0; j < N; ++j) {
        const size_t nj = (N - j) % N;
        R[nj] = R0[j];
      }
    }
    return res;
  }
  // Remark: For transposition we have transposition(ls_t) above.
  // A coordinate-permutation only for odd N; with "second-first"
  // (to obtain "first-second" combine with negationj):
  ls_t sumdiff(const ls_t& S) {
    assert(sqshape(S));
    const size_t N = S.size();
    ls_t res(N, ls_row_t(N));
    for (size_t i = 0; i < N; ++i) {
      const ls_row_t& R0 = S[i];
      for (size_t j = 0; j < N; ++j) {
        const size_t sum = (i + j) % N;
        const size_t diff = ((N+j) - i) % N;
        res[sum][diff] = R0[j];
      }
    }
    return res;
  }
  // A coordinate-permutation only for gcd(k,N) = 1:
  ls_t scaling(const ls_t& S, const size_t k) {
    assert(sqshape(S));
    const size_t N = S.size();
    ls_t res(N, ls_row_t(N));
    for (size_t i = 0; i < N; ++i) {
      const ls_row_t& R0 = S[i];
      const size_t i2 = (i * k) % N;
      ls_row_t& R = res[i2];
      for (size_t j = 0; j < N; ++j) {
        const size_t j2 = (j * k) % N;
        R[j2] = R0[j];
      }
    }
    return res;
  }

  bool qsqshape(const ls_row_t& Q) noexcept  {
    const size_t N = Q.size();
    for (const size_t& x : Q) if (x >= N) return false;
    return true;
  }
  // The above permutations of the square now for queens-solutions,
  // which are vectors Q of length N, with Q(i) being the column of
  // the queen in row i:
  ls_row_t qtransposition(const ls_row_t& Q) {
    assert(qsqshape(Q));
    const size_t N = Q.size();
    ls_row_t res(N);
    for (size_t i = 0; i < N; ++i) res[Q[i]] = i;
    return res;
  }
  ls_row_t qnegationj(const ls_row_t& Q) {
    const size_t N = Q.size();
    ls_row_t res(N);
    for (size_t i = 0; i < N; ++i) res[i] = (N - Q[i]) % N;
    return res;
  }
  ls_row_t qsumdiff(const ls_row_t& Q) {
    const size_t N = Q.size();
    ls_row_t res(N);
    for (size_t i = 0; i < N; ++i) {
      const size_t j = Q[i], sum = (i + j) % N, diff = ((N+j) - i) % N;
      res[sum] = diff;
    }
    return res;
  }
  ls_row_t qscaling(const ls_row_t& Q, const size_t k) {
    const size_t N = Q.size();
    ls_row_t res(N);
    for (size_t i = 0; i < N; ++i) {
      const size_t i2 = (i * k) % N, j = Q[i], j2 = (j * k) % N;
      res[i2] = j2;
    }
    return res;
  }


  bool pandiagonal(const ls_t& S) noexcept {
    return ls(S) and
      rls(moddiags2rows(S)) and rls(modantidiags2rows(S));
  }


  bool rcyclic(const ls_t& S) noexcept {
    const size_t N = S.size();
    if (N <= 1) return true;
    const ls_row_t& R0 = S[0];
    const size_t N0 = R0.size();
    const auto next = S.begin()+1, end = S.end();
    if (N0 == 0)
      return std::all_of(next, end, [](const auto& r){return r.empty();});
    const size_t val0 = R0[0];
    for (auto it = next; it != end; ++it) {
      const ls_row_t& R1 = *it;
      const size_t N1 = R1.size();
      if (N1 == 0) continue;
      size_t i = 0;
      while (i < N1 and R1[i] != val0) ++i;
      if (i == N1) return false;
      size_t count = N1 - 1; i = (i+1) % N1; size_t j = 1 % N0;
      while (count != 0)
        if (R1[i] != R0[j]) return false;
        else { i = (i+1) % N1; j = (j+1) % N0; --count; }
    }
    return true;
  }
  bool ccyclic(const ls_t& S) noexcept {
    return rcyclic(gtransposition(S));
  }
  bool dcyclic(const ls_t& S) noexcept {
    return rcyclic(moddiags2rows(S));
  }
  bool adcyclic(const ls_t& S) noexcept {
    return rcyclic(modantidiags2rows(S));
  }
  typedef std::array<bool, 4> cyclicity_t;
  cyclicity_t cyclicity(const ls_t& S) noexcept {
    return {rcyclic(S), ccyclic(S), dcyclic(S), adcyclic(S)};
  }


  void rstandardisem(ls_t& S) noexcept {
    assert(sqval(S));
    const size_t N = S.size();
    if (N <= 1) return;
    const ls_row_t r =
      [N, &S]{ls_row_t r(N);
              for (size_t i = 0; i < N; ++i) r[S[0][i]] = i;
              return r;}();
    for (ls_row_t& R : S)
      for (size_t& x : R)
        x = r[x];
  }
  ls_t rstandardise(ls_t S) {
    rstandardisem(S);
    return S;
  }


  // "Modular directions":
  struct ModDir {
    size_t a, b;
    auto operator <=>(const ModDir&) const noexcept = default;
    friend std::ostream& operator <<(std::ostream& out, const ModDir& d) {
      return out << "(" << d.a << "," << d.b << ")";
    }
  };

  // Expanding the queens-solution pi into direction d, returning
  // {} if pi is empty or the expansion doesn't cover the square
  // (based on [Atkins, Hay, Larson; 1983, Comp. & Maths. with Appls. 9(2)]):
  ls_t expand_queenssols(const ls_row_t& pi, const ModDir& d) {
    const size_t N = pi.size();
    if (N == 0) return {};
    ls_t res(N, ls_row_t(N));
    for (size_t x = 0; x < N; ++x) {
      const size_t a = (x * d.a) % N, b = (x * d.b) % N;
      for (size_t i = 0; i < N; ++i) {
        const size_t i2 = (i + (N - a))% N;
        const size_t j = (pi[i2] + b) % N;
        size_t& oldval = res[i][j];
        if (oldval != 0) return {};
        else oldval = x+1;
      }
    }
    for (size_t i = 0; i < N; ++i) {
      ls_row_t& R = res[i];
      for (size_t j = 0; j < N; ++j) --R[j];
    }
    return res;
  }

  typedef std::map<ls_t, ModDir> expansions_t;
  // For each equivalence-class of an expansion (with representative
  // the row-standardised element) the lexicographical smallest direction:
  expansions_t all_expansions(const ls_row_t& pi) {
    const size_t N = pi.size();
    if (N == 0) return {};
    expansions_t res;
    for (size_t a = 0; a < N; ++a)
      for (size_t b = 0; b < N; ++b) {
        const ModDir d{a,b};
        ls_t L = rstandardise(expand_queenssols(pi, d));
        if (L.empty()) continue;
        const auto find = res.find(L);
        if (find == res.end()) res.insert({std::move(L), d});
        else if (d < find->second) find->second = d;
      }
    return res;
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


}

#endif
