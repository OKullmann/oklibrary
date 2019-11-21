// Irfansha Shaik, 21.11.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Content:


TODO:

1. Make check_da constexpr.

*/

#ifndef SOLUTIONS_9hpK9Frd0h
#define SOLUTIONS_9hpK9Frd0h

#include <limits>
#include <array>
#include <bitset>
#include <utility>

#include <cassert>

namespace Solutions {

  // The indices for columns:
  typedef std::uint8_t lines_t;

  constexpr lines_t maxN = std::numeric_limits<lines_t>::max() / 2 - 1;
  constexpr lines_t N = 8;
  static_assert(N <= maxN);

  // The solution type:
  typedef std::array<lines_t, N> solution_t;

  // Hashing of diagonals/antidiagonals:
  typedef std::bitset<2*N-1> hash_da;

  // Flip the potential solution S as positions i, j:
  inline constexpr solution_t flip(solution_t S, const lines_t i, const lines_t j) noexcept {
    assert(i < N);
    assert(j < N);
    assert(i != j);
    const lines_t first_col = S[i];
    S[i] = S[j]; S[j] = first_col;
    // std::swap(S[i], S[j]); // with C++20
    return S;
  }

  // Checks if there are two queens are placed in same diagonal or anti-diagonal:
  inline bool valid_da(solution_t S) noexcept {
    hash_da occurred_d, occurred_a;
    for (lines_t i = 0; i < N; ++i) {
      assert(S[i] < N);
      const lines_t d = (N-1) + i - S[i];
      const lines_t a = i + S[i];
      if (not occurred_d[d] and not occurred_a[a]) {
        occurred_d[d] = true; occurred_a[a] = true;
      }
      else return false;
    }
    return true;
  }

}
#endif

