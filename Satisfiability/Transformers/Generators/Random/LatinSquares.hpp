// Oliver Kullmann, 18.12.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

TODOS:

1. Establish relation to LatinSquares/Mols.cpp.

2. Move general functions later to more appropriate place.

*/

#ifndef LATINSQUARES_WPlhfGMfBs
#define LATINSQUARES_WPlhfGMfBs

#include <type_traits>
#include <vector>
#include <array>
#include <ostream>
#include <algorithm>
#include <numeric>
#include <initializer_list>

#include <cassert>
#include <cstdint>

#include "Numbers.hpp"
#include "Algorithms.hpp"

namespace LatinSquares {

  namespace RG = RandGen;

  typedef std::uint32_t ls_dim_t;
  static_assert(std::is_same_v<ls_dim_t, std::uint32_t>);
  constexpr ls_dim_t max_dim = ls_dim_t(-1);
  static_assert(max_dim == RG::iexp2(32)-1);

  // Counts:
  constexpr ls_dim_t max64_N_all_ls = 7;
  constexpr std::array<std::uint64_t, max64_N_all_ls+1> c_all_ls
    {1, 1, 2, 12, 576, 161280, 812851200, 61479419904000};

  constexpr ls_dim_t max64_N_all_reduced_ls = 9;
  constexpr std::array<std::uint64_t, max64_N_all_reduced_ls+1>
  c_all_reduced_ls
    {1, 1, 1, 1, 4, 56, 9408, 16942080, 535281401856, 377597570964258816};


  typedef std::vector<ls_dim_t> ls_row_t;
  typedef std::vector<ls_row_t> ls_t;

  constexpr ls_dim_t max_N_list_reduced_ls = 4;
  // Listing all reduced latin squares of very small order:
  std::vector<ls_t> all_reduced_ls(const ls_dim_t N) noexcept {
    switch(N) {
    case 0 : return {{}};
    case 1 : return {{{0}}};
    case 2 : return {{{0,1},{1,0}}};
    case 3 : return {{{0,1,2},{1,2,0},{2,0,1}}};
    case 4 : return {{{0,1,2,3},{1,2,3,0},{2,3,0,1},{3,0,1,2}},
                     {{0,1,2,3},{1,0,3,2},{2,3,1,0},{3,2,0,1}},
                     {{0,1,2,3},{1,3,0,2},{2,0,3,1},{3,2,1,0}},
                     {{0,1,2,3,},{1,0,3,2},{2,3,0,1},{3,2,1,0}}};
    default : return {};
    }
  }


  /* Valid latin squares: */

  // Valid dimension:
  inline constexpr bool valid(const ls_dim_t N) noexcept {
    return N != 0 and N < max_dim;
  }
  static_assert(not valid(0));
  static_assert(valid(1));
  static_assert(valid(max_dim-1));
  static_assert(not valid(max_dim));


  // Valid value:
  inline constexpr bool valid(const ls_dim_t v, const ls_dim_t N) noexcept {
    return v < N;
  }
  static_assert(not valid(0,0));
  static_assert(valid(0,1));
  inline constexpr bool singular(const ls_dim_t v, const ls_dim_t N) noexcept {
    return v == N;
  }
  static_assert(singular(0,0));
  static_assert(singular(1,1));
  static_assert(not singular(0,1));
  inline constexpr bool valid_partial(const ls_dim_t v, const ls_dim_t N) noexcept {
    return v <= N;
  }
  static_assert(valid_partial(0,0));
  static_assert(valid_partial(0,1));
  static_assert(valid_partial(1,1));
  static_assert(not valid_partial(2,1));


  // Valid complete row:
  inline bool valid_basic(const ls_row_t& r, const ls_dim_t N) noexcept {
    if (not valid(N) or r.size() != N) return false;
    for (const ls_dim_t x : r) if (not valid(x,N)) return false;
    return true;
  }
  // Testing whether all entries are different; for computing
  // invalidating indices see find_first_duplication(r):
  inline bool all_different(const ls_row_t& r) {
    const auto N = r.size();
    assert(valid_basic(r, N));
    std::vector<bool> found(N);
    for (ls_dim_t x : r)
      if (found[x]) return false;
      else found[x] = true;
    return true;
  }
  inline bool valid(const ls_row_t& r, const ls_dim_t N) noexcept {
    return valid_basic(r, N) and all_different(r);
  }

  // Valid partial row:
  inline bool valid_basic_partial(const ls_row_t& r, const ls_dim_t N) noexcept {
    if (not valid(N) or r.size() != N) return false;
    for (const ls_dim_t x : r)
      if (not valid_partial(x,N)) return false;
    return true;
  }
  inline bool all_different_partial(const ls_row_t& r) {
    const auto N = r.size();
    assert(valid_basic_partial(r, N));
    std::vector<bool> found(N);
    for (ls_dim_t x : r)
      if (x != N)
        if (found[x]) return false;
        else found[x] = true;
    return true;
  }
  inline bool valid_partial(const ls_row_t& r, const ls_dim_t N) noexcept {
    return valid_basic_partial(r, N) and all_different_partial(r);
  }

  // Valid complete latin square:
  inline bool valid_basic(const ls_t& L) noexcept {
    const auto N = L.size();
    if (not valid(N)) return false;
    for (const ls_row_t& r : L) if (not valid_basic(r,N)) return false;
    return true;
  }
  inline bool all_different_rows(const ls_t& L) {
    assert(valid_basic(L));
    for (const ls_row_t& r : L) if (not all_different(r)) return false;
    return true;
  }
  inline bool all_different_columns(const ls_t& L) {
    assert(valid_basic(L));
    const auto N = L.size();
    for (ls_dim_t j = 0; j < N; ++j) {
      ls_row_t r((N));
      for (ls_dim_t i = 0; i < N; ++i) r[i] = L[i][j];
      if (not all_different(r)) return false;
    }
    return true;
  }
  inline bool valid(const ls_t& L) {
    return valid_basic(L) and all_different_rows(L) and
      all_different_columns(L);
  }

  // Valid partial latin square:
  inline bool valid_basic_partial(const ls_t& L) noexcept {
    const auto N = L.size();
    if (not valid(N)) return false;
    for (const ls_row_t& r : L) if (not valid_basic_partial(r,N)) return false;
    return true;
  }
  inline bool all_different_rows_partial(const ls_t& L) {
    assert(valid_basic_partial(L));
    for (const ls_row_t& r : L) if (not all_different_partial(r)) return false;
    return true;
  }
  inline bool all_different_columns_partial(const ls_t& L) {
    assert(valid_basic_partial(L));
    const auto N = L.size();
    for (ls_dim_t j = 0; j < N; ++j) {
      ls_row_t r((N));
      for (ls_dim_t i = 0; i < N; ++i) r[i] = L[i][j];
      if (not all_different_partial(r)) return false;
    }
    return true;
  }
  inline bool valid_partial(const ls_t& L) {
    return valid_basic_partial(L) and all_different_rows_partial(L) and
      all_different_columns_partial(L);
  }


  // Enumeration of basic matrices (with valid_basic(L)) in lexicographical
  // order:
  inline ls_t first_basic(const ls_dim_t N) {
    return ls_t(N, ls_row_t(N));
  }
  inline bool next_basic(ls_t& L) noexcept {
    assert(valid_basic(L));
    const ls_dim_t N = L.size();
    ls_dim_t i = 0, j = 0;
    while (i < N) {
      if (L[i][j] < N-1) break;
      if (++j == N) {j = 0; ++i; }
    }
    if (i == N) return false;
    ++L[i][j];
    for (ls_dim_t ip = 0; ip < i; ++ip)
      for (ls_dim_t j = 0; j < N; ++j)
        L[ip][j] = 0;
    for (ls_dim_t jp = 0; jp < j; ++jp)
      L[i][jp] = 0;
    return true;
  }


  // Most trivial counting of all latin squares:
  std::uint64_t trivial_count_all_ls(const ls_dim_t N) {
    assert(valid(N));
    ls_t L = first_basic(N);
    std::uint64_t count = 0;
    do count += valid(L);
    while (next_basic(L));
    return count;
  }


  /* Generation of standard objects */

  // Standardised rows:
  ls_row_t standard(const ls_dim_t N) {
    ls_row_t res(N);
    std::iota(res.begin(), res.end(), 0);
    return res;
  }

  // Empty latin squares:
  ls_t empty_ls(const ls_dim_t N) {
    return ls_t(N, ls_row_t(N,N));
  }

  // The cyclic latin square of order N (the multiplication table of the
  // cyclic group):
  ls_t cyclic_ls(const ls_dim_t N) {
    assert(valid(N));
    ls_t L(N, ls_row_t(N));
    for (ls_dim_t i = 0; i < N; ++i)
      for (ls_dim_t j = 0; j < N; ++j)
        L[i][j] = (i + j) % N;
    assert(valid(L));
    return L;
  }


  /* Helper functions */

  // Find first duplication in r, returning their indices (sorted):
  typedef std::array<ls_dim_t,2> index_pair_t;
  index_pair_t find_first_duplication(const ls_row_t& r) {
    assert(valid_basic(r, r.size()));
    const ls_dim_t N = r.size();
    std::vector<ls_dim_t> found(N,N);
    for (ls_dim_t i = 0; i < N; ++i)
      if (found[r[i]] != N) return {found[r[i]], i};
      else found[r[i]] = i;
    return {N,N};
  }


  /* Basic operations */

  ls_t transpose(const ls_t& L) {
    const ls_dim_t N = L.size();
    ls_t res(N,ls_row_t(N));
    for (ls_dim_t i = 0; i < N; ++i) {
      assert(L[i].size() == N);
      for (ls_dim_t j = 0; j < N; ++j)
        res[j][i] = L[i][j];
    }
    return res;
  }

  bool has_standardised_first_column(const ls_t& L) noexcept {
    for (ls_dim_t i = 0; i < L.size(); ++i) {
      assert(not L[i].empty());
      if (L[i][0] != i) return false;
    }
    return true;
  }
  ls_t standardise_first_column(const ls_t& L) {
    const ls_dim_t N = L.size();
    ls_t res(N);
    for (ls_dim_t i = 0; i < N; ++i) {
      assert(not L[i].empty() and L[i][0] < N);
      res[L[i][0]] = L[i];
    }
    return res;
  }

  bool has_standardised_first_row(const ls_t& L) noexcept {
    if (L.empty()) return true;
    const ls_dim_t N = L[0].size();
    for (ls_dim_t i = 0; i < N; ++i) if (L[0][i] != i) return false;
    return true;
  }
  ls_t standardise_first_row(const ls_t& L) {
    return transpose(standardise_first_column(transpose(L)));
  }

  bool is_standardised(const ls_t& L) noexcept {
    return has_standardised_first_row(L) and
      has_standardised_first_column(L);
  }
  ls_t standardise(const ls_t& L) {
    return standardise_first_column(standardise_first_row(L));
  }


  /* Basic output */

  std::ostream& operator <<(std::ostream& out, const ls_row_t& r) {
    assert(valid_basic_partial(r, r.size()));
    if (r[0] != r.size()) out << r[0];
    else out << "*";
    for (ls_dim_t i = 1; i < r.size(); ++i)
      if (r[i] != r.size()) out << " " << r[i];
      else out << " *";
    return out;
  }
  std::ostream& operator <<(std::ostream& out, const ls_t& L) {
    assert(valid_basic_partial(L));
    for (const auto& r : L) out << r << "\n";
    return out;
  }
  std::ostream& operator <<(std::ostream& out, const index_pair_t& p) {
    return out << p[0] << "," << p[1];
  }


  /* Computing SDRs (systems of distinct representatives) */

  /* The set-systems used here use the integers 0, ..., N-1
     as indices as well as for values, where N is the number of
     indices, given by the size of the vector of values.
     Thus here only cases are handled where the number of values is
     at most the number of indices, and where the values are directly
     used as indices.
  */

  typedef std::vector<ls_dim_t> set_t;
  struct Set {
    set_t s;
    ls_dim_t size() const noexcept { return s.size(); }
    bool contains(const ls_dim_t x) const noexcept {
      return std::binary_search(s.begin(), s.end(), x);
    }
    void remove(const ls_dim_t x) noexcept {
      const auto it = std::lower_bound(s.begin(),s.end(),x);
      assert(it != s.end());
      s.erase(it);
    }
    bool operator ==(const Set& rhs) const noexcept = default;
  };
  bool valid_basic(const Set& s, const ls_dim_t N) noexcept {
    if ( N >= max_dim) return false;
    for (const ls_dim_t x : s.s) if (not valid(x,N)) return false;
    return true;
  }
  bool valid(const Set& s, const ls_dim_t N) noexcept {
    return valid_basic(s,N) and std::is_sorted(s.s.begin(), s.s.end())
      and std::adjacent_find(s.s.begin(),s.s.end()) == s.s.end();
  }

  typedef std::vector<Set> setsystem_t;
  struct SetSystem {
    setsystem_t S;
    SetSystem(std::initializer_list<Set> l) : S(l) {}
    SetSystem(const ls_dim_t N) {
      S.reserve(N);
      for (ls_dim_t i = 0; i < N; ++i) S.emplace_back(standard(N));
    }
    auto size() const noexcept { return S.size(); }
    bool empty() const noexcept { return S.empty(); }
  };
  bool valid(const SetSystem& S) noexcept {
    const auto N = S.size();
    if (N >= max_dim) return false;
    for (const Set& s : S.S) if (not valid(s,N)) return false;
    return true;
  }

  /* Due to the restricted nature of set-systems here, a system of
     distinct representatives is exactly a valid row of a latin square
     of size N = number of sets, such that each element of the row is
     in the corresponding set:
  */
  bool is_sdr(const ls_row_t f, const SetSystem& S) noexcept {
    assert(f.size() == S.size());
    const ls_dim_t N = S.size();
    if (N == 0) return true;
    assert(valid(f,N));
    assert(valid(S));
    for (ls_dim_t i = 0; i < N; ++i)
      if (not S.S[i].contains(f[i])) return false;
    return true;
  }
  bool is_psdr(const ls_row_t f, const SetSystem& S) noexcept {
    assert(f.size() == S.size());
    const ls_dim_t N = S.size();
    if (N == 0) return true;
    assert(valid_partial(f,N));
    assert(valid(S));
    for (ls_dim_t i = 0; i < N; ++i)
      if (f[i] != N and not S.S[i].contains(f[i])) return false;
    return true;
  }


  // Partial bijection between indices and values:
  class PBij {
    ls_row_t f, b; // forward, backward
    ls_dim_t s = 0;
    ls_dim_t N;
  public :
    PBij(const ls_dim_t N) : f(N,N), b(N,N), N(N) {
      assert(valid(N));
    }

    ls_dim_t size() const noexcept { return s; }
    ls_dim_t total_size() const noexcept { return N; }
    bool empty() const noexcept { return s == 0; }
    bool total() const noexcept { return s == N; }

    ls_dim_t operator()(const ls_dim_t i) const noexcept {
      assert(i < N);
      return f[i];
    }
    ls_dim_t operator[](const ls_dim_t v) const noexcept {
      assert(v < N);
      return b[v];
    }

    bool set(const ls_dim_t i, const ls_dim_t v) noexcept {
      assert(i < N);
      assert(v < N);
      if (f[i] != N or b[v] != N) return false;
      f[i] = v; b[v] = i; ++s;
      return true;
    }

    const ls_row_t& r() const noexcept { return f; }

    bool operator ==(const PBij& rhs) const noexcept = default;

    friend std::ostream& operator <<(std::ostream& out, const PBij& pb) {
      return out << pb.f << ";" << pb.b;
    }

  };


  // Randomly choose assignments, computing a maximal psdr; no unit-clause
  // propagation, since used here only on regular bipartite graphs:
  PBij random_psdr(const SetSystem& S, RG::randgen_t& g) {
    assert(valid(S));
    const ls_dim_t N = S.size();
    PBij res(N);
    if (S.empty()) return res;
    ls_row_t order = standard(N);
    RG::shuffle(order.begin(), order.end(), g);
    for (const ls_dim_t x : order) {
      auto values = S.S[x].s;
      RG::shuffle(values.begin(), values.end(), g);
      for (const ls_dim_t y : values) if (res.set(x,y)) break;
    }
    return res;
  }

  void remove_psdr(const PBij& p, SetSystem& S) noexcept {
    assert(is_psdr(p.r(),S));
    const ls_dim_t N = S.size();
    for (ls_dim_t i = 0; i < N; ++i) {
      ls_dim_t v = p(i);
      if (v != N) S.S[i].remove(v);
    }
  }

  struct PartiallyFilled {
    ls_t L;
    ls_dim_t rows_completed;
    SetSystem A; // available values
    PBij next; // next row
  };

  std::ostream& operator <<(std::ostream& out, const PartiallyFilled& P) {
    return out << P.L << P.rows_completed << ":\n" << P.next.r() << "\n";
  }

  PartiallyFilled random_pls(const ls_dim_t N, RG::randgen_t& g) {
    assert(valid(N));
    PartiallyFilled res{empty_ls(N), 0, N, N};
    do {
      res.next = random_psdr(res.A, g);
      if (res.next.total()) {
        for (ls_dim_t i = 0; i < N; ++i)
          res.L[res.rows_completed][i] = res.next(i);
        remove_psdr(res.next, res.A);
        ++res.rows_completed;
      }
      else return res;
    } while (res.rows_completed < N);
    return res;
  }

}

#endif
