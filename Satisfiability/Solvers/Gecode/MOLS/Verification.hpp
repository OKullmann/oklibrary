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
   - size_t = Conditions::size_t, signed_t = Conditions::signed_t
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

   - rreduced(S): whether the first row of S is standardised
   - orreduced(S): whether the last row of S is standardised
   - credcued(S): whether the first column is standardised
     (which may not exist)
   - ocredcued(S): whether the N-column is standardised, where N is
     the number of rows
   - reduced(S): both conditions together
   - oreduced(S): both o-forms together
   - wcreduced(S): whether the first column is "weakly standardised", i.e.,
     has in row i values {0, ..., i+1}
     No restrictions on S are made.

   - helper-functions for equivalence-classes of cells:
     - mdiff_equiv, msum_equiv
     - diff_equiv, sum_equiv
   - moddiagonal(S), modantidiagonal(S): where all modular diagonals resp.
     antidiagonals fulfil alldiff
   - queendiagonal(S), queenantidiagonal(S): same for ordinary diagonals
     resp. antidiagonals.

   - helper-function box_index(i,j,N)
   - is_boxed(S): whether all boxes fulfil alldiff.

   - symmetric(S): false for not square-shaped S
   - antisymmetric(S): false for not square-shaped S

   - orthogonal(S1, S2): checks whether the superposition of S1, S2 has
     all entries different; false of S1, S2 are not of the same shape.

  Testing solutions:

   - valid(AConditions, PSquares): formal fitting
   - extract(AConditions, PSquares, Square): compute all squares
   - correct(AConditions, PSquares): checking the conditions (asserting
     formal fitting)
   - lcorrect(AConditions, vector<PSquares>): calls "correct" on each element
     of the vector, and additionally checks that all elements of the vector
     are different.

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
#include "OrthogonalArrays.hpp"

namespace Verification {

  namespace CD = Conditions;
  namespace PS = PartialSquares;
  namespace BS = BasicLatinSquares;
  namespace OA = OrthogonalArrays;

  typedef CD::size_t size_t;
  typedef CD::signed_t signed_t;
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
    const auto& r = S.front();
    const size_t N = r.size();
    for (size_t i = 0; i < N; ++i)
      if (r[i] != i) return false;
    return true;
  }
  bool orreduced(const ls_t& S) noexcept {
    if (S.empty()) return false;
    const auto& r = S.back();
    const size_t N = r.size();
    for (size_t i = 0; i < N; ++i)
      if (r[i] != i) return false;
    return true;
  }
  bool creduced(const ls_t& S) noexcept {
    const size_t N = S.size();
    for (size_t i = 0; i < N; ++i) {
      const auto& r = S[i];
      if (r.empty() or r.front() != i) return false;
    }
    return true;
  }
  bool ocreduced(const ls_t& S) noexcept {
    const size_t N = S.size();
    for (size_t i = 0; i < N; ++i) {
      const auto& r = S[i];
      if (r.size() < N or r.back() != i) return false;
    }
    return true;
  }
  bool reduced(const ls_t& S) noexcept {
    return rreduced(S) and creduced(S);
  }
  bool oreduced(const ls_t& S) noexcept {
    return orreduced(S) and ocreduced(S);
  }
  bool wcreduced(const ls_t& S) noexcept {
    const size_t N = S.size();
    for (size_t i = 0; i < N; ++i) {
      const auto& r = S[i];
      if (not r.empty() and r[0] > i+1) return false;
    }
    return true;
  }


  size_t mdiff_equiv(const size_t i, const size_t j, const size_t N) {
    assert(i < N and j < N);
    return ((N + i) - j) % N;
  }
  size_t msum_equiv(const size_t i, const size_t j, const size_t N) {
    assert(i < N and j < N);
    return (i + j) % N;
  }
  bool moddiagonal(const ls_t& S) {
    std::map<size_t, std::set<size_t>> equiv_classes;
    const size_t N = S.size();
    for (size_t i = 0; i < N; ++i) {
      const ls_row_t& r = S[i];
      for (size_t j = 0; j < r.size(); ++j)
        if (not equiv_classes[mdiff_equiv(i,j,N)].insert(r[j]).second)
          return false;
    }
    return true;
  }
  bool modantidiagonal(const ls_t& S) {
    std::map<size_t, std::set<size_t>> equiv_classes;
    const size_t N = S.size();
    for (size_t i = 0; i < N; ++i) {
      const ls_row_t& r = S[i];
      for (size_t j = 0; j < r.size(); ++j)
        if (not equiv_classes[msum_equiv(i,j,N)].insert(r[j]).second)
          return false;
    }
    return true;
  }
  signed_t diff_equiv(const size_t i, const size_t j) {
    return signed_t(i) - signed_t(j);
  }
  size_t sum_equiv(const size_t i, const size_t j) {
    return i + j;
  }
  bool queendiagonal(const ls_t& S) {
    std::map<size_t, std::set<signed_t>> equiv_classes;
    for (size_t i = 0; i < S.size(); ++i) {
      const ls_row_t& r = S[i];
      for (size_t j = 0; j < r.size(); ++j)
        if (not equiv_classes[diff_equiv(i,j)].insert(r[j]).second)
          return false;
    }
    return true;
  }
  bool queenantidiagonal(const ls_t& S) {
    std::map<size_t, std::set<size_t>> equiv_classes;
    for (size_t i = 0; i < S.size(); ++i) {
      const ls_row_t& r = S[i];
      for (size_t j = 0; j < r.size(); ++j)
        if (not equiv_classes[sum_equiv(i,j)].insert(r[j]).second)
          return false;
    }
    return true;
  }

  std::array<size_t, 2> box_index(const size_t i, const size_t j,
                                  const size_t N) noexcept {
    const size_t b = std::sqrt(N);
    return {i / b, j / b};
  }
  bool is_boxed(const ls_t& S) {
    std::map<std::array<size_t,2>, std::set<size_t>> equiv_classes;
    const size_t N = S.size();
    for (size_t i = 0; i < N; ++i) {
      const ls_row_t& r = S[i];
      for (size_t j = 0; j < r.size(); ++j)
        if (not equiv_classes[box_index(i,j,N)].insert(r[j]).second)
          return false;
    }
    return true;
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

  ls_t extract(const CD::AConditions& ac, const PS::PSquares& sol,
               const CD::Square s) {
    assert(valid(ac, sol));
    assert(ac.valid(s));
    ls_t L = BS::extract(sol.psqs[ac.index(s)]);
    const CD::VS v = s.v;
    if (with_t(v)) BS::transpositionm(L);
    if (with_at(v)) BS::antitranspositionm(L);
    return L;
  }

  bool correct(const CD::AConditions& ac, const PS::PSquares& sol) {
    assert(valid(ac, sol));
    using CD::Square;
    for (size_t i = 0; i < ac.k; ++i) {
      for (CD::VS v : ac.versions()[i].choices()) {
        assert(CD::is_main_rep(v));
        if (v == CD::VS::c231) {
          if (extract(ac,sol,{i,v}) !=
              OA::conjugate_ls(extract(ac,sol,{i,CD::VS::id}), {2,3,1}, 1))
            return false;
        }
        else if (v == CD::VS::c312) {
          if (extract(ac,sol,{i,v}) !=
              OA::conjugate_ls(extract(ac,sol,{i,CD::VS::id}), {3,1,2}, 1))
            return false;
        }
      }
    }
    for (const auto& [uc, Sqs] : ac.map()) {
      for (const Square s : Sqs.sqs()) {
        const ls_t L = extract(ac, sol, s);
        switch (uc) {
        case CD::UC::rls : if (not BS::rls(L)) return false; break;
        case CD::UC::cls : if (not BS::cls(L)) return false; break;
        case CD::UC::ls : if (not BS::ls(L)) return false; break;
        case CD::UC::diag : if (not diagonal(L)) return false; break;
        case CD::UC::antidiag : if (not antidiagonal(L)) return false; break;
        case CD::UC::uni : if (not unipotent(L)) return false; break;
        case CD::UC::antiuni : if (not antiunipotent(L)) return false; break;
        case CD::UC::idem : if (not idempotent(L)) return false; break;
        case CD::UC::antiidem : if (not antiidempotent(L)) return false; break;
        case CD::UC::moddiag : if (not moddiagonal(L)) return false; break;
        case CD::UC::modantidiag :
          if (not modantidiagonal(L)) return false; else break;
        case CD::UC::queendiag :
          if (not queendiagonal(L)) return false; else break;
        case CD::UC::queenantidiag :
          if (not queenantidiagonal(L)) return false; else break;
        case CD::UC::rred : if (not rreduced(L)) return false; break;
        case CD::UC::orred : if (not orreduced(L)) return false; break;
        case CD::UC::cred : if (not creduced(L)) return false; break;
        case CD::UC::ocred : if (not ocreduced(L)) return false; break;
        case CD::UC::red : if (not reduced(L)) return false; break;
        case CD::UC::ored : if (not oreduced(L)) return false; break;
        case CD::UC::wcred : if (not wcreduced(L)) return false; break;
        case CD::UC::box : if (not is_boxed(L)) return false; break;
        case CD::UC::symm : if (not symmetric(L)) return false; break;
        case CD::UC::antisymm : if (not antisymmetric(L)) return false; break;
        case CD::UC::mention : break;
        case CD::UC::rlsm1 : if (not BS::rlsm1(L)) return false; break;
        case CD::UC::clsm1 : if (not BS::clsm1(L)) return false; break;
        case CD::UC::rlsm2 : if (not BS::rlsm2(L)) return false; break;
        case CD::UC::clsm2 : if (not BS::clsm2(L)) return false; break;
        default : throw std::runtime_error("Verification::correct: unknown"
                                           " uc=" + std::to_string(int(uc)));
        }
      }
    }
    for (const auto& E : ac.eq()) {
      const Square l = E.lhs(), r = E.rhs();
      if (extract(ac, sol, l) != extract(ac, sol, r)) return false;
    }
    for (const auto& P : ac.peq()) {
      const Square r = P.r(), f1 = P.f1(), f2 = P.f2();
      const auto pt = P.pt();
      if (pt == CD::PT::rprod) {
        if (extract(ac,sol,r)
            != BS::rproduct(extract(ac,sol,f2), extract(ac,sol,f1)))
          return false;
      }
      else {
        if (extract(ac,sol,r)
            != BS::cproduct(extract(ac,sol,f2), extract(ac,sol,f1)))
          return false;
      }
    }
    return true;
  }

  bool lcorrect(const CD::AConditions& ac,
                std::vector<PS::PSquares> Lsol) {
    return std::ranges::all_of(Lsol, [&ac](const PS::PSquares& sol){
                                 return correct(ac, sol);})
      and BS::alldiffelem(std::move(Lsol));
  }

}

#endif
