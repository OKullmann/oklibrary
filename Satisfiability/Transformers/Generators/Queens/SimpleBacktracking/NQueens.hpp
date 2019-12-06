// Oliver Kullmann, 20.7.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Basic definitions for simple backtracking algorithms

*/

#ifndef NQUEENS_KLwLMEuU0U
#define NQUEENS_KLwLMEuU0U

#include <limits>

#include <cstdint>

namespace Queens {

  typedef std::uint8_t input_t; // type of N
  typedef std::uint_fast64_t count_t; // counting solutions

  // Definition of queen_t, the bits representing the N columns:
#ifndef NMAX
# define NMAX 64
#endif
  constexpr input_t maxN = NMAX;
  static_assert(maxN == 32 or maxN == 64, "Max value for N: 32 or 64.");
#if maxN == 32
  typedef std::uint_fast32_t queen_t;
#else
 typedef std::uint_fast64_t queen_t;
#endif
  static_assert(std::numeric_limits<queen_t>::digits >= maxN, "Problem with queen_t.");
  // For 64 < N <= 128, use queen_t = std::uint_fast128_t (and appropriate count_t).

  // Three helper functions for bit-operations:

  // N 1's from the right, the rest 0:
  constexpr queen_t setrightmostbits(const input_t N) noexcept {
    return (N>=maxN) ? queen_t(-1) : (queen_t(1) << N) - 1;
  }
  static_assert(setrightmostbits(0) == 0);
  static_assert(setrightmostbits(1) == 1);
  static_assert(setrightmostbits(2) == 3);
  static_assert(setrightmostbits(3) == 7);

  // 1 at position+1 (from right), the rest 0:
  constexpr queen_t one(const input_t position) noexcept {
    return queen_t(1) << position;
  }
  static_assert(one(0) == 1);
  static_assert(one(1) == 2);
  static_assert(one(2) == 4);
  static_assert(one(3) == 8);

  // Set all bits in x to 0 except of rightmost one (if exists):
  inline constexpr queen_t keeprightmostbit(const queen_t x) noexcept {
    return -x & x;
  }
  static_assert(keeprightmostbit(0) == 0);
  static_assert(keeprightmostbit(1) == 1);
  static_assert(keeprightmostbit(2) == 2);
  static_assert(keeprightmostbit(3) == 1);
  static_assert(keeprightmostbit(4) == 4);
  static_assert(keeprightmostbit(5) == 1);
  static_assert(keeprightmostbit(6) == 2);
  static_assert(keeprightmostbit(7) == 1);

}

#endif
