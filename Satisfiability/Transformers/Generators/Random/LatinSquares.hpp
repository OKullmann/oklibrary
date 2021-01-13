// Oliver Kullmann, 18.12.2020 (Swansea)
/* Copyright 2020, 2021 Oliver Kullmann
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
#include <utility>
#include <optional>

#include <cassert>
#include <cstdint>
#include <cmath>

#include "Numbers.hpp"
#include "Algorithms.hpp"
#include "Distributions.hpp"


namespace LatinSquares {

  namespace RG = RandGen;


  typedef std::uint32_t ls_dim_t;
  static_assert(std::is_same_v<ls_dim_t, std::uint32_t>);
  constexpr ls_dim_t max_dim = ls_dim_t(-1);
  static_assert(max_dim == RG::iexp2(32)-1);

  typedef std::int32_t ls_sdim_t;
  static_assert(std::is_same_v<ls_sdim_t, std::int32_t>);
  constexpr ls_sdim_t max_sdim = 2147483647;


  /* Counts and representatives

      - max64_N_all_ls, max80_N_all_ls
      - c_all_ls, c80_all_ls

      - max64_N_all_reduced_ls, max80_N_all_reduced_ls
      - c_all_reduced_ls, c80_all_reduced_ls

  */

  // All ls's https://oeis.org/A002860 :
  constexpr ls_dim_t max64_N_all_ls = 7;
  constexpr std::array<std::uint64_t, max64_N_all_ls+1> c_all_ls
    {1, 1, 2, 12, 576, 161280, 812851200, 61479419904000};
  constexpr ls_dim_t max80_N_all_ls = 11;
  constexpr std::array<FloatingPoint::float80, max80_N_all_ls+1> c80_all_ls
  {1, 1, 2, 12, 576, 161280, 812851200, 61479419904000,
      108776032459082956800.0L, 5524751496156892842531225600.0L,
      9982437658213039871725064756920320000.0L,
      776966836171770144107444346734230682311065600000.0L};

  // All reduced ls's https://oeis.org/A000315 :
  constexpr ls_dim_t max64_N_all_reduced_ls = 9;
  constexpr std::array<std::uint64_t, max64_N_all_reduced_ls+1>
  c_all_reduced_ls
    {1, 1, 1, 1, 4, 56, 9408, 16942080, 535281401856, 377597570964258816};
  constexpr ls_dim_t max80_N_all_reduced_ls = 11;
  constexpr std::array<FloatingPoint::float80, max80_N_all_reduced_ls+1>
  c80_all_reduced_ls
  {1, 1, 1, 1, 4, 56, 9408, 16942080, 535281401856, 377597570964258816,
      7580721483160132811489280.0L,
      5363937773277371298119673540771840.0L};

  // All ls's with reduced first row (or column) :
  constexpr ls_dim_t max64_N_all_hreduced_ls = 8;
  constexpr std::array<std::uint64_t, max64_N_all_hreduced_ls+1>
  c_all_hreduced_ls
    {1, 1, 1, 2, 24, 1344, 1128960, 12198297600, 2697818265354240};


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
  constexpr ls_dim_t max_N_list_nonisotopic_ls = 4;
  // Listing reps of all isotopy-classes of latin squares of very small order:
  std::vector<ls_t> all_nonisotopic_ls(const ls_dim_t N) noexcept {
    switch(N) {
    case 0 : return all_reduced_ls(0);
    case 1 : return all_reduced_ls(1);
    case 2 : return all_reduced_ls(2);
    case 3 : return all_reduced_ls(3);
    case 4 : return {all_reduced_ls(4)[0], all_reduced_ls(4)[3]};
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

  inline bool is_square(const ls_t& L) noexcept {
    const ls_dim_t N = L.size();
    if (N == 0) return true;
    assert(N == L.size() and valid(N));
    for (ls_dim_t i = 0; i < N; ++i)
      if (L[i].size() != N) return false;
    return true;
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

  enum class StRLS {none=0, row=1, column=2, both=3};
  ls_t standardise(const ls_t& L, const StRLS ost) {
    switch (ost) {
    case StRLS::none   : return L;
    case StRLS::row    : return standardise_first_row(L);
    case StRLS::column : return standardise_first_column(L);
    default            : return standardise(L);
    }
  }
  constexpr bool available_counts(const ls_dim_t N, const StRLS o) noexcept {
    switch (o) {
    case StRLS::none : return N <= max64_N_all_ls;
    case StRLS::both : return N <= max64_N_all_reduced_ls;
    default : return N <= max64_N_all_hreduced_ls;
    }
  }
  constexpr std::uint64_t count_ls(const ls_dim_t N, const StRLS o) noexcept {
    assert(available_counts(N,o));
    switch (o) {
    case StRLS::none : return c_all_ls[N];
    case StRLS::both : return c_all_reduced_ls[N];
    default : return c_all_hreduced_ls[N];
    }
  }


  typedef std::array<ls_dim_t, 3> triple_t;
  typedef std::vector<triple_t> ls_array_t;

  bool valid_basic(const triple_t& t, const ls_dim_t N) noexcept {
    return t[0]<N and t[1]<N and t[2]<N;
  }
  bool valid_basic(const ls_array_t& A) noexcept {
    const auto N2 = A.size();
    if (N2 == 0) return false;
    const ls_dim_t N = std::sqrt(N2);
    if (N*N != N2) return false;
    for (const triple_t& t : A)
      if (not valid_basic(t, N)) return false;
    return true;
  }

  inline ls_array_t ls2lsa(const ls_t& L) {
    assert(valid_basic(L));
    const auto N = L.size();
    ls_array_t res; res.reserve(N*N);
    for (ls_dim_t i = 0; i < N; ++i)
      for (ls_dim_t j = 0; j < N; ++j)
        res.push_back({i,j,L[i][j]});
    return res;
  }
  inline ls_t lsa2ls(const ls_array_t& A) {
    assert(valid_basic(A));
    const ls_dim_t N = std::sqrt(A.size());
    ls_t res(N, ls_row_t(N));
    for (const triple_t& t : A) res[t[0]][t[1]] = t[2];
    return res;
  }

  inline ls_row_t shuffle_row(ls_row_t r, RG::RandGen_t& g) {
    RG::shuffle(r.begin(), r.end(), g);
    return r;
  }
  ls_array_t full_shuffle(const ls_array_t& A, RG::RandGen_t& g) {
     assert(valid_basic(A));
     const auto N2 = A.size();
     const ls_dim_t N = std::sqrt(N2);
     ls_array_t res; res.reserve(N2);
     const ls_row_t pr = shuffle_row(standard(N), g),
                    pc = shuffle_row(standard(N), g),
                    pv = shuffle_row(standard(N), g),
                    pi = shuffle_row(standard(3), g);
     for (const triple_t& t : A) {
       const triple_t pt{pr[t[0]], pc[t[1]], pv[t[2]]};
       res.push_back({pt[pi[0]], pt[pi[1]], pt[pi[2]]});
     }
     return res;
  }
  // Creating a paratopic ls:
  ls_t full_shuffle(const ls_t& L, RG::RandGen_t& g) {
    return lsa2ls(full_shuffle(ls2lsa(L), g));
  }


  typedef std::vector<ls_sdim_t> ls_srow_t;
  typedef std::vector<std::vector<ls_srow_t>> ls_ip_t;

  inline bool valid_basic(const ls_ip_t& I) noexcept {
    const ls_dim_t N = I.size();
    if (N != I.size() or not valid(N)) return false;
    for (ls_dim_t i = 0; i < N; ++i) {
      if (I[i].size() != N) return false;
      for (ls_dim_t j = 0; j < N; ++j)
        if (I[i][j].size() != N) return false;
    }
    return true;
  }
  inline bool valid_basic01(const ls_ip_t& I) noexcept {
    const ls_dim_t N = I.size();
    if (N != I.size() or not valid(N)) return false;
    for (ls_dim_t i = 0; i < N; ++i) {
      if (I[i].size() != N) return false;
      for (ls_dim_t j = 0; j < N; ++j) {
        if (I[i][j].size() != N) return false;
        for (const ls_sdim_t v : I[i][j])
          if (v < 0 or v > 1) return false;
      }
    }
    return true;
  }
  inline bool valid(const ls_ip_t& I) noexcept {
    if (not valid_basic01(I)) return false;
    const ls_dim_t N = I.size();
    for (ls_dim_t i = 0; i < N; ++i)
      for (ls_dim_t j = 0; j < N; ++j)
        if (std::accumulate(I[i][j].begin(), I[i][j].end(), 0) != 1)
          return false;
    for (ls_dim_t i = 0; i < N; ++i)
      for (ls_dim_t k = 0; k < N; ++k) {
        ls_dim_t sum = 0;
        for (ls_dim_t j = 0; j < N; ++j) sum += I[i][j][k];
        if (sum != 1) return false;
      }
    for (ls_dim_t j = 0; j < N; ++j)
      for (ls_dim_t k = 0; k < N; ++k) {
        ls_dim_t sum = 0;
        for (ls_dim_t i = 0; i < N; ++i) sum += I[i][j][k];
        if (sum != 1) return false;
      }
    return true;
  }

  inline ls_ip_t create_ip(const ls_dim_t N) {
    assert(valid(N));
    return {N, std::vector<ls_srow_t>(N, ls_srow_t(N))};
  }

  inline ls_ip_t ls2lsip(const ls_t& L) {
    assert(valid_basic(L));
    const auto N = L.size();
    ls_ip_t res(N);
    for (ls_dim_t i = 0; i < N; ++i) {
      res[i].resize(N);
      for (ls_dim_t j = 0; j < N; ++j) {
        res[i][j].resize(N);
        res[i][j][L[i][j]] = 1;
      }
    }
    return res;
  }
  inline ls_t lsip2ls(const ls_ip_t& I) {
    assert(valid_basic(I));
    const ls_dim_t N = I.size();
    ls_t res = empty_ls(N);
    for (ls_dim_t i = 0; i < N; ++i)
      for (ls_dim_t j = 0; j < N; ++j)
        for (ls_dim_t k = 0; k < N; ++k)
          if (I[i][j][k] == 1) res[i][j] = k;
    return res;
  }
    


  /* Basic output */

  std::ostream& operator <<(std::ostream& out, const ls_row_t& r) {
    const auto N = r.size();
    if (N == 0) return out;
    if (r[0] != N) out << r[0]; else out << "*";
    for (ls_dim_t i = 1; i < N; ++i)
      if (r[i] != N) out << " " << r[i]; else out << " *";
    return out;
  }
  struct LS_t {
    const ls_t& L;
  };
  std::ostream& operator <<(std::ostream& out, const ls_t& L) {
    for (const auto& r : L) out << r << "\n";
    return out;
  }
  std::ostream& operator <<(std::ostream& out, const LS_t& L) {
    for (const auto& r : L.L) out << r << "\n";
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
    Set(std::initializer_list<ls_dim_t> l) : s(l) {}
    Set(const set_t& s) : s(s) {}

    ls_dim_t size() const noexcept { return s.size(); }
    bool contains(const ls_dim_t x) const noexcept {
      return std::binary_search(s.begin(), s.end(), x);
    }
    void remove(const ls_dim_t x) noexcept {
      const auto it = std::lower_bound(s.begin(),s.end(),x);
      assert(it != s.end());
      s.erase(it);
    }

    void sort() noexcept { std::sort(s.begin(),s.end()); }

    bool operator ==(const Set& rhs) const noexcept = default;

    auto begin() noexcept {return s.begin();}
    auto begin() const noexcept {return s.cbegin();}
    auto end() noexcept {return s.end();}
    auto end() const noexcept {return s.cend();}

    friend std::ostream& operator <<(std::ostream& out, const Set& s) {
      for (const ls_dim_t x : s) out << x << ",";
      return out;
    }
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

    void sort() noexcept { for (Set& s : S) s.sort(); }

    auto begin() noexcept {return S.begin();}
    auto begin() const noexcept {return S.cbegin();}
    auto end() noexcept {return S.end();}
    auto end() const noexcept {return S.cend();}

    friend std::ostream& operator <<(std::ostream& out, const SetSystem& S) {
      for (const Set s : S) out << s << ";";
      return out;
    }
  };
  bool valid_basic(const SetSystem& S) noexcept {
    const auto N = S.size();
    if (N >= max_dim) return false;
    for (const Set& s : S.S) if (not valid_basic(s,N)) return false;
    return true;
  }
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
  bool is_psdr(const ls_row_t f, SetSystem S) noexcept {
    assert(f.size() == S.size());
    const ls_dim_t N = S.size();
    if (N == 0) return true;
    assert(valid_partial(f,N));
    S.sort();
    assert(valid(S));
    for (ls_dim_t i = 0; i < N; ++i)
      if (f[i] != N and not S.S[i].contains(f[i])) return false;
    return true;
  }


  typedef std::vector<std::pair<ls_dim_t,ls_dim_t>> ls_map_t;
  std::ostream& operator <<(std::ostream& out, const ls_map_t m) {
    for (const auto [x,y] : m) out << "(" << x << "," << y << ")";
    return out;
  }

  // Partial bijection between indices and values:
  class PBij {
    ls_row_t f, b; // forward, backward
    ls_dim_t s = 0;
    ls_dim_t N;
  public :
    PBij(const ls_dim_t N) : f(N,N), b(N,N), N(N) {
      assert(N < max_dim);
    }

    ls_dim_t insert(const ls_map_t& m) noexcept {
      ls_dim_t count = 0;
      for (const auto [x,y] : m) count += set(x,y);
      return count;
    }

    PBij(const ls_dim_t N, const ls_map_t& m) : PBij(N) {
      [[maybe_unused]] const ls_dim_t inserted = insert(m);
      assert(inserted == m.size());
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
    void unset(const ls_dim_t i) noexcept {
      assert(i < N);
      if (f[i] != N) {b[f[i]] = N; f[i] = N; --s;}
    }

    const ls_row_t& r() const noexcept { return f; }

    bool operator ==(const PBij& rhs) const noexcept = default;

    friend std::ostream& operator <<(std::ostream& out, const PBij& pb) {
      return out << pb.f << ";" << pb.b;
    }

  };


  // Randomly choose assignments, computing a maximal psdr; no unit-clause
  // propagation, since used here only on regular bipartite graphs:
  PBij random_psdr(const SetSystem& S, RG::RandGen_t& g) {
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

  PartiallyFilled random_pls(const ls_dim_t N, RG::RandGen_t& g) {
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


  inline PBij improve(const PBij& init, const SetSystem& A, const ls_row_t& back_arcs, const ls_map_t& final) {
    assert(is_psdr(init.r(), A));
    const ls_dim_t N = A.size();
    PBij res(N);
    ls_dim_t failures = 0;
    for (auto [x,y] : final) {
      assert(init[y] == N);
      [[maybe_unused]] const bool success = res.set(x,y);
      assert(success);
      while (init(x) != N) {
        y = init(x); x = back_arcs[y];
        if (not res.set(x,y)) {
          ++failures;
          do {
            x = init[y]; y = res(x);
            res.unset(x);
          } while (init[y] != N);
          goto Loop_end;
        }
      }
    Loop_end: ;
    }
    assert(failures < final.size());
    for (ls_dim_t i = 0; i < N; ++i)
      if (init(i) != N and res(i) == N) {
        [[maybe_unused]] const bool success = res.set(i, init(i));
        assert(success);
      }
    assert(res.size() == init.size() + final.size() - failures);
    return res;
  }

  /* Starting with a given partial sdr init for A, compute a maximum
     partial sdr for A, randomised:
  */
  std::optional<PBij> maximise_once(const PBij& init, SetSystem A, RG::RandGen_t& g) {
    assert(is_psdr(init.r(), A));
    const ls_dim_t N = A.size();
    assert(init.total_size() == N);
    const ls_dim_t M = init.size();
    if (M == N) return std::nullopt;
    for (ls_dim_t i = 0; i < N; ++i) {
      auto& set = A.S[i].s;
      RG::shuffle(set.begin(),set.end(),g);
    }

    ls_row_t back_arcs(N,N);
    ls_row_t next;
    for (ls_dim_t i = 0; i < N; ++i) if (init(i)==N) next.push_back(i);
    assert(next.size() <= N-M);
    RG::shuffle(next.begin(), next.end(), g);

    while (true) {
      ls_t alt_values(next.size());
      for (ls_dim_t i = 0; i < next.size(); ++i) {
        const ls_dim_t x = next[i];
        for (const ls_dim_t y : A.S[x].s)
          if (init[y] == N) { // found augmenting path
            ls_map_t final;
            std::vector<bool> used(N);
            final.push_back({x,y}); used[y] = true;
            for (++i; i < next.size(); ++i) {
              const ls_dim_t x = next[i];
              for (const ls_dim_t y : A.S[x].s)
                if (init[y] == N and not used[y]) {
                  final.push_back({x,y}); used[y] = true;
                  break;
                }
            }
            RG::shuffle(final.begin(), final.end(), g);
            return improve(init, A, back_arcs, final);
          }
          else if (back_arcs[y] == N) alt_values[i].push_back(y);
        RG::shuffle(alt_values[i].begin(), alt_values[i].end(), g);
      }
      ls_row_t new_next;
      bool values_left;
      do {
        values_left = false;
        for (ls_dim_t i = 0; i < alt_values.size(); ++i) {
          if (alt_values[i].empty()) continue;
          const ls_dim_t x = next[i];
          const ls_dim_t y = alt_values[i].back();
          if (back_arcs[y] == N) {
            back_arcs[y] = x;
            new_next.push_back(init[y]);
          }
          alt_values[i].pop_back();
          if (not alt_values[i].empty()) values_left = true;
        }
      } while (values_left);
      if (new_next.empty()) return std::nullopt;
      next = new_next;
    }
  }

  PBij maximise(PBij init, const SetSystem& A, RG::RandGen_t& g) {
    for (std::optional<PBij> res = maximise_once(init, A, g);
         res;
         res = maximise_once(init, A, g))
      init = *res;
    return init;
  }


  enum class CrRLS {with_initial_phase = 0, without_initial_phase = 1};

  ls_t random_ma_ls(const ls_dim_t N, const CrRLS ocr, RG::RandGen_t& g) {
    assert(valid(N));
    ls_t L;
    SetSystem A(N);
    ls_dim_t row = 0;

    if (ocr == CrRLS::with_initial_phase) {
      PartiallyFilled pls = random_pls(N, g);
      L = std::move(pls.L);
      row = pls.rows_completed;
      if (row == N) return L;
      A = std::move(pls.A);
      PBij next = maximise(pls.next, A, g);
      assert(next.size() == N);
      L[row++] = std::move(next.r());
      remove_psdr(next, A);
    } else L = ls_t(N);

    for (; row < N; ++row) {
      PBij next = maximise(PBij(N), A, g);
      assert(next.size() == N);
      L[row] = std::move(next.r());
      remove_psdr(next, A);
    }
    return L;
  }


  /* The Jacobson-Matthews generator */

  // The special cell is used if a current square is improper;
  // i and j are indices of the cell, while triple (a,b,c) is cell's value:
  struct SpecialCell {
    ls_dim_t i, j;
    ls_dim_t a, b, c;
    bool active;
  };
  constexpr bool valid(const SpecialCell& s, const ls_dim_t N) noexcept {
    if (not(s.i<N and s.j<N and s.a<N and s.b<N and s.c<N)) return false;
    if (not s.active) return true;
    return s.a!=s.b and s.a!=s.c and s.b!=s.c;
  }

  struct StatsJM {
    typedef RG::gen_uint_t count_t;
    const count_t initial_rounds;
    count_t additional_rounds=0, proper_ls_encountered=0;
    constexpr StatsJM(const count_t T) noexcept : initial_rounds(T) {}
  };

  class JacobsMatthews {
    const ls_dim_t N;
    ls_t L;
    SpecialCell scell;
    RG::RandGen_t& g;
    StatsJM stats;

  public:

    constexpr static ls_dim_t max_N = 2642245;
    STATIC_ASSERT(max_N == RG::gen_uint_t(std::cbrt(RG::randgen_max)));
    constexpr static bool valid(const ls_dim_t N) noexcept {
      return  N <= max_N;
    }
    constexpr static RG::gen_uint_t cb(const ls_dim_t N) noexcept {
      assert(valid(N));
      const RG::gen_uint_t n{N};
      return n*n*n;
    }

    constexpr static RG::gen_uint_t rounds(const ls_dim_t N, RG::RandGen_t& g) noexcept {
      if (N == 1) return 0;
      else if (N == 2) return RG::bernoulli(g);
      else return cb(N);
    }

    JacobsMatthews(const ls_dim_t N, RG::RandGen_t& g) noexcept :
        N(N),
        L(cyclic_ls(N)),
        scell{0,0,0,0,0,false},
        g(g),
        stats(rounds(N,g)) {
      assert(valid(N));
      if (N>1) iterate();
    }
    JacobsMatthews(ls_t L, RG::RandGen_t& g) noexcept :
        N(L.size()),
        L(L),
        scell{0,0,0,0,0,false},
        g(g),
        stats(rounds(N,g)) {
      assert(valid(N));
      assert(LatinSquares::valid(L));
      if (N>1) iterate();
    }

    const ls_t& ls() const noexcept { return L; }
    const StatsJM& statistics() const noexcept { return stats; }

  private:

    void iterate() noexcept {
      for (RG::gen_uint_t i = 0; i < stats.initial_rounds; ++i)
        perturbate_square();
      while (not LatinSquares::valid(L)) {
        perturbate_square(); ++stats.additional_rounds;
      }
    }

    // Perturbate current square:
    void perturbate_square() noexcept {
      ls_dim_t i, j,  a, b, c;

      if (not scell.active) {
        ls_dim_t i0, j0;
        // Randomly choose one cell and its new entry:
        {RG::UniformRange U(g, N);
         i0 = U(); j0 = U();
         c = L[i0][j0];
         b = RG::UniformRange(g,N-1)();
         if (b >= c) ++b;
        }
        // Determine the 3 other cells for modification:
        const ls_row_t& row = L[i0];
        {const auto it = std::find(row.begin(), row.end(), b);
         assert(it != row.end());
         j = std::distance(row.begin(), it);
         assert(j < N);
        }
        ls_row_t col(N);
        for (unsigned k = 0; k < N; ++k) col[k] = L[k][j0];
        {const auto it = std::find(col.begin(), col.end(), b);
         assert(it != col.end());
         i = std::distance(col.begin(), it);
         assert(i < N);
        }
        // Save old entry:
        a = L[i][j];
        // Update entries of the chosen cells:
        L[i0][j0] = b; L[i][j] = b;
        L[i0][j] = c; L[i][j0] = c;
      }
      else {
        // Randomly choose the special cell's entry (from the first two ones):
        const ls_dim_t v = RG::bernoulli(g) ? scell.a : scell.b;
        c = (v == scell.a) ? scell.b : scell.a;
        // Randomly choose index of one of two duplicate entries in the
        // improper row:
        const ls_row_t& row = L[scell.i];
        {const std::array<ls_dim_t,2> dup = find_first_duplication(row);
         j = dup[RG::bernoulli(g)];
         assert(j < N);
         b = L[scell.i][j];
        }
        // Randomly choose index of one of two duplicate entries in the
        // improper column:
        {ls_row_t col(N);
         for (unsigned k = 0; k < N; ++k) col[k] = L[k][scell.j];
         {const std::array<ls_dim_t,2> dup = find_first_duplication(col);
          i = dup[RG::bernoulli(g)];
          assert(i < N);
          a = L[i][j];
         }
        }
        // Update entries of the chosen cells:
        assert(L[scell.i][j] == L[i][scell.j]);
        L[scell.i][scell.j] = v;
        L[scell.i][j] = c; L[i][scell.j] = c;
        L[i][j] = b;
      }
      assert(valid_basic(L));

      if (LatinSquares::valid(L))
        {scell.active = false; ++stats.proper_ls_encountered;}
      else scell = {i,j, a,b,c, true};
    }

  };


  // Alternative implementation of JM based on 3-dimensional representation:
  inline triple_t find_zero(const ls_ip_t& I, RG::RandGen_t& g) noexcept {
    RG::UniformRange U(g, I.size());
    ls_dim_t i, j, k;
    do { i = U(); j = U(); k = U(); }
    while (I[i][j][k] != 0);
    return {i,j,k};
  }
  inline triple_t find_ones(const ls_ip_t& I, const triple_t z) noexcept {
    const auto [rx,ry,rz] = z;
    triple_t o;
    for (ls_dim_t i = 0; i < I.size(); ++i) {
      if (I[i][ry][rz] == 1) o[0] = i;
      if (I[rx][i][rz] == 1) o[1] = i;
      if (I[rx][ry][i] == 1) o[2] = i;
    }
    return o;
  }
  inline void move(ls_ip_t& I, const triple_t r, const triple_t o) noexcept {
    const auto [rx, ry, rz] = r;
    const auto [ox, oy, oz] = o;
    ++I[rx][ry][rz];
    ++I[rx][oy][oz]; ++I[ox][ry][oz]; ++I[ox][oy][rz];
    --I[ox][oy][oz]; // this might lead to -1
    --I[rx][ry][oz]; --I[rx][oy][rz]; --I[ox][ry][rz];
  }
  inline std::array<triple_t, 2> find_both_ones(const ls_ip_t& I, const triple_t z) noexcept {
    const auto [rx,ry,rz] = z;
    ls_dim_t ix=0, iy=0, iz=0;
    std::array<triple_t, 2> res;
    for (ls_dim_t i = 0; i < I.size(); ++i) {
      if (I[i][ry][rz] == 1) res[ix++][0] = i;
      if (I[rx][i][rz] == 1) res[iy++][1] = i;
      if (I[rx][ry][i] == 1) res[iz++][2] = i;
    }
    return res;
  }

  // Counts the number of intermediate creations:
  std::uint64_t jm_next(ls_ip_t& I, RG::RandGen_t& g) noexcept {
    assert(valid(I));
    const ls_dim_t N = I.size();
    if (N == 1) return 0;
    std::uint64_t count = 0;
    triple_t r = find_zero(I, g);
    triple_t o = find_ones(I, r);
    move(I, r, o);

    for (; I[o[0]][o[1]][o[2]] == -1; ++count) {
      r = o;
      const auto p = find_both_ones(I, r);
      for (ls_dim_t i = 0; i < 3; ++i) o[i] = p[bernoulli(g)][i];
      move(I, r, o);
    }
    return count;
  }

  std::uint64_t jm_rounds(const ls_dim_t N, RG::RandGen_t& g) noexcept {
    if (N == 1) return 0;
    if (N == 2) return RG::bernoulli(g);
    const std::uint64_t n = N;
    return n*n*n;
  }

  ls_ip_t jm3(const ls_dim_t N, RG::RandGen_t& g) {
    const auto rounds = jm_rounds(N, g);
    ls_ip_t I = ls2lsip(cyclic_ls(N));
    for (std::uint64_t i = 0; i < rounds; ++i) jm_next(I, g);
    return I;
  }


  // Construct selection-object by giving the number of selected rows, columns,
  // and additional single cells, stores for rows and columns the
  // number of deletions:
  struct Selection {
    const ls_dim_t N, r, c;
    const ls_dim_t remaining, s;
    const bool additional_cells;

    constexpr Selection(const ls_dim_t N, const ls_dim_t r_, const ls_dim_t c_, const ls_dim_t s) noexcept : N(N), r(N-r_), c(N-c_), remaining(N*N-r_*c_), s(s), additional_cells(s != 0) {
      assert(valid(N));
      assert(r_ <= N and c_ <= N and s <=  remaining);
    }
    constexpr Selection(const ls_dim_t N) noexcept : Selection(N,N,N,0) {}
  };


  ls_t select(ls_t L, const Selection& sel, RG::RandGen_t& g) noexcept {
    const ls_dim_t N = sel.N;
    assert(sel.N == L.size());
    assert(is_square(L));
    const auto sel_row = RG::choose_kn(sel.r, N, g, true);
    const auto sel_col = RG::choose_kn(sel.c, N, g, true);

    if (not sel.additional_cells) {
      for (ls_dim_t i = 0, i_sel = 0; i < N; ++i) {
        if (i_sel != sel.r and i == sel_row[i_sel]) {
          std::fill(L[i].begin(), L[i].end(), N);
          ++i_sel; continue;
        }
        for (ls_dim_t j=0, j_sel=0; j < N; ++j)
          if (j_sel != sel.c and j == sel_col[j_sel]) {
            L[i][j] = N; ++j_sel;
          }
      }
    }
    else {
      std::vector<std::pair<index_pair_t, ls_dim_t>> rem_cells;
      rem_cells.reserve(sel.remaining);
      for (ls_dim_t i = 0, i_sel = 0; i < N; ++i) {
        if (i_sel != sel.r and i == sel_row[i_sel]) {
          for (ls_dim_t j = 0; j < N; ++j) {
            rem_cells.push_back({{i,j}, L[i][j]});
            L[i][j] = N;
          }
          ++i_sel; continue;
        }
        for (ls_dim_t j=0, j_sel=0; j < N; ++j)
          if (j_sel != sel.c and j == sel_col[j_sel]) {
            rem_cells.push_back({{i,j}, L[i][j]});
            L[i][j] = N; ++j_sel;
          }
      }
      assert(rem_cells.size() == sel.remaining);
      const auto selection = RG::choose_kn(sel.s, sel.remaining, g);
      for (const auto i : selection) {
        const auto [p,v] = rem_cells[i];
        assert( L[p[0]][p[1]] == N);
        L[p[0]][p[1]] = v;
      }
    }
    return L;
  }

}

#endif
