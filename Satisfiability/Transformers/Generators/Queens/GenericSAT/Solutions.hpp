// Irfansha Shaik, 21.11.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Content:


TODO:

1. Enable dependency on N, via template-parameters, to have
   static_asserts possible.

*/

#ifndef SOLUTIONS_9hpK9Frd0h
#define SOLUTIONS_9hpK9Frd0h

#include <limits>
#include <array>
#include <bitset>
#include <algorithm>
#include <vector>

#include <cassert>

#include "ChessBoard.hpp"

namespace Solutions {

  // The indices for columns:
  typedef std::uint8_t lines_t;

  constexpr lines_t maxN = std::numeric_limits<lines_t>::max() / 2 - 1;
  constexpr lines_t N = 8;
  static_assert(N >= 1 and N <= maxN);

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
  inline constexpr bool valid_da(solution_t S) noexcept {
    hash_da occurred_d, occurred_a;
    for (lines_t i = 0; i < N; ++i) {
      assert(S[i] < N);
      const lines_t d = (N-1) + i - S[i];
      const lines_t a = i + S[i];
      if (not ((const hash_da)(occurred_d))[d] and not ((const hash_da)(occurred_a))[a]) {
        occurred_d[d] = true; occurred_a[a] = true;
      }
      else return false;
    }
    return true;
  }

  typedef std::vector<solution_t> solution_vector;
  typedef std::vector<ChessBoard::Count_t> solution_ccs;

  void dfs_visit(const ChessBoard::Count_t v, const solution_vector& V, std::vector<bool>& visited, const ChessBoard::Count_t current_cc, solution_ccs& res) noexcept {
    assert(v < V.size());
    assert(visited.size() == V.size());
    assert(res.size() == V.size());
    assert(not visited[v]);
    visited[v] = true;
    res[v] = current_cc;
    using ChessBoard::Count_t;
    for (lines_t i = 0; i < N-1; ++i) {
      for (lines_t j = i+1; j < N; ++i) {
        const solution_t S = flip(V[v], i, j);
	if (valid_da(S)) {
          const auto it = std::lower_bound(V.begin(), V.end(), S);
	  assert(it != V.end());
          const Count_t w = it - V.begin();
	  if (visited[w]) continue;
	  dfs_visit(w, V, visited, current_cc, res);
	}
      }
    }
  }

  solution_ccs determine_1ccs(const solution_vector& V) {
    assert(std::is_sorted(V.begin(), V.end()));
    solution_ccs res(V.size());
    using ChessBoard::Count_t;
    Count_t current_cc = 0;
    std::vector<bool> visited(V.size());
    for (Count_t i = 0; i < V.size(); ++i) {
      if (visited[i]) continue;
      dfs_visit(i, V, visited, current_cc, res);
      ++current_cc;
    }
    return res;
  }

}
#endif

