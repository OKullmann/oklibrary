// Irfansha, 21.11.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Content:

*/

#ifndef SOLUTIONS_9hpK9Frd0h
#define SOLUTIONS_9hpK9Frd0h

#include <cassert>
#include <array>
#include <bitset>

namespace Solutions {

// The indices for columns:
typedef std::size_t size_t;
typedef std::uint8_t index_t;

constexpr index_t N = 8;

// The solution type:
typedef std::array<index_t, N> solution_t;

// The diagonal type:
typedef std::bitset<2*N-1> da_t;

// Flips given array of indices and returns the array:
constexpr inline solution_t flip(solution_t S, const index_t i, const index_t j) noexcept {
  index_t first_col = S[i];
  index_t second_col = S[j];
  // Swapping two columns:
  S[i] = second_col;
  S[j] = first_col;
  return S;
}

static_assert(flip({1,2,3,4,5,6,7,8},2,3)[0] == 1);
static_assert(flip({1,2,3,4,5,6,7,8},2,3)[2] == 4);
static_assert(flip({1,2,3,4,5,6,7,8},2,3)[3] == 3);

// Checks if there are two queens are placed in same diagonal or anti-diagonal:
inline bool check(solution_t S) noexcept {
  // diagonals and anti-diagonals bitset:
  da_t d, a;
  for(size_t i = 0; i < sizeof(S); ++i) {
    //(x-y) + (N-1) for diagonal
    size_t cur_d = ((i+1) - int(S[i])) + (N-1);
    //(x+y) - 2 for antidiagonal
    size_t cur_a = (i+1) + int(S[i]) - 2;
    if (d[cur_d] == false and a[cur_a] == false) {
      d[cur_d] = true;
      a[cur_a] = true;
    }
    else return false;
  }
  return true;
}

}
#endif

