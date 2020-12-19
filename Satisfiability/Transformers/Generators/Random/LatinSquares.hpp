// Oliver Kullmann, 18.12.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

TODOS:

1. Establish relation to LatinSquares/Mols.cpp.

*/

#ifndef LATINSQUARES_WPlhfGMfBs
#define LATINSQUARES_WPlhfGMfBs

#include <type_traits>
#include <vector>
#include <array>

#include <cassert>
#include <cstdint>

#include "Numbers.hpp"

namespace LatinSquares {

  namespace RG = RandGen;

  typedef std::uint32_t ls_dim_t;
  static_assert(std::is_same_v<ls_dim_t, std::uint32_t>);
  constexpr ls_dim_t max_dim = ls_dim_t(-1);
  static_assert(max_dim == RG::iexp2(32)-1);

  // Counts:
  constexpr ls_dim_t max64_N_all_ls = 7;
  constexpr std::array<std::uint64_t, max64_N_all_ls+1> all_ls
  {1, 1, 2, 12, 576, 161280, 812851200, 61479419904000};


  typedef std::vector<ls_dim_t> ls_row_t;
  typedef std::vector<ls_row_t> ls_t;


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

  // Valid complete row:
  inline bool valid_basic(const ls_row_t& r, const ls_dim_t N) noexcept {
    if (not valid(N) or r.size() != N) return false;
    for (const ls_dim_t x : r) if (not valid(x,N)) return false;
    return true;
  }
  inline bool all_different(const ls_row_t& r) {
    const auto N = r.size();
    assert(valid_basic(r, N));
    std::vector<bool> found((N));
    for (ls_dim_t x : r)
      if (found[x]) return false;
      else found[x] = true;
    return true;
  }
  inline bool valid(const ls_row_t& r, const ls_dim_t N) noexcept {
    return valid_basic(r, N) and all_different(r);
  }

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

}

#endif
