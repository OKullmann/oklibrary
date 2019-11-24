// Irfansha Shaik, 21.11.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Content:


TODOS:

1. Improved algorithms for determining the 1- and 2-neighbours
   1. For a flip, only positions i, j changed, and thus testing for being
      a solution can be done more efficiently.
   2. When running through all 2-neighbours, via flipping i,j, k,l,
      w.l.o.g. it should be possible to assume i <= k.

*/

#ifndef SOLUTIONS_9hpK9Frd0h
#define SOLUTIONS_9hpK9Frd0h

#include <limits>
#include <array>
#include <algorithm>
#include <vector>
#include <utility>
#include <map>
#include <ostream>
#include <stack>

#include <cassert>

#include "ChessBoard.hpp"

namespace Solutions {

  // The indices for columns:
  typedef std::uint8_t lines_t;

  constexpr lines_t maxN = std::numeric_limits<lines_t>::max() / 2 - 1;

  // The solution type:
  template <lines_t N>
    using solution_t = std::array<lines_t, N>;

  template <lines_t N>
  solution_t<N> extract(const ChessBoard::Rooks_Board b) noexcept {
    assert(N == b.N);
    solution_t<N> S;
    using ChessBoard::coord_t;
    for (coord_t i = 1; i <= N; ++i) {
      coord_t j = 1;
      for (; j <= N and b({i,j}) != ChessBoard::State::placed; ++j);
      assert(1 <= j and j <= N);
      S[i-1] = j-1;
    }
    return S;
  }

  // Flip the potential solution S at positions i, j:
  template <lines_t N>
  inline constexpr solution_t<N> flip(solution_t<N> S, const lines_t i, const lines_t j) noexcept {
    assert(i < N);
    assert(j < N);
    assert(i != j);
    {const auto t = S[i]; S[i] = S[j]; S[j] = t;}
    // std::swap(S[i], S[j]); // with C++20
    return S;
  }
  // static_assert(flip<3>(solution_t<3>{0,1,2}, 0, 2) == solution_t<3>{2,1,0}); // with C++20

  // Checks if there are two queens placed in same diagonal or anti-diagonal:
  template <lines_t N>
  inline constexpr bool valid_da(const solution_t<N>& S) noexcept {
    std::array<bool, 2*N-1> occurred_d{}, occurred_a{};
    for (lines_t i = 0; i < N; ++i) {
      assert(S[i] < N);
      const lines_t d = (N-1) + i - S[i];
      const lines_t a = i + S[i];
      if (occurred_d[d] or occurred_a[a]) return false;
      else { occurred_d[d] = true; occurred_a[a] = true; }
    }
    return true;
  }
  static_assert(valid_da<4>(solution_t<4>{0,0,0,0}));
  static_assert(not valid_da<2>(solution_t<2>{0,1}));

  template <lines_t N>
  inline constexpr bool valid_rc(const solution_t<N>& S) noexcept {
    std::array<bool,N> occurred_c{};
    for (lines_t i = 0; i < N; ++i) {
      assert(S[i] < N);
      if (occurred_c[S[i]]) return false;
        occurred_c[S[i]] = true;
    }
    return true;
  }
  static_assert(valid_rc<1>(solution_t<1>{}));
  static_assert(valid_rc<2>(solution_t<2>{0,1}));
  static_assert(not valid_rc<2>(solution_t<2>{0,0}));

  template <lines_t N>
  inline constexpr bool valid(const solution_t<N>& S) noexcept {
    return valid_rc<N>(S) and valid_da<N>(S);
  }

  template <lines_t N>
   using solution_vector = std::vector<solution_t<N>>;
  template <lines_t N>
  inline bool valid(const solution_vector<N>& V) noexcept {
    for (const auto S : V) if (not valid<N>(S)) return false;
    return true;
  }


  typedef std::vector<ChessBoard::Count_t> solution_ccs;
  typedef std::pair<solution_ccs, ChessBoard::Count_t> Solution_ccs;

  template <lines_t N>
  Solution_ccs determine_ccs(const solution_vector<N>& V, const lines_t level) {
    if (level == 0) return {};
    assert(std::is_sorted(V.begin(), V.end()));
    Solution_ccs res{V.size(), 0};
    std::vector<bool> visited(V.size());
    using ChessBoard::Count_t;
    std::stack<Count_t> buffer;
    for (Count_t i = 0; i < V.size(); ++i) {
      if (visited[i]) continue;
      buffer.push(i);
      while (not buffer.empty()) {
        const Count_t v = buffer.top(); buffer.pop();
        assert(v < V.size());
        if (visited[v]) continue;
        visited[v] = true;
        res.first[v] = res.second;
        for (lines_t i = 0; i < N-1; ++i)
          for (lines_t j = i+1; j < N; ++j)
            if (const solution_t<N> S1 = flip<N>(V[v], i, j);
                valid_da<N>(S1)) {
              const auto it = std::lower_bound(V.begin(), V.end(), S1);
              assert(it != V.end());
              const Count_t w = it - V.begin();
              if (visited[w]) continue;
              buffer.push(w);
            }
            else if (level >= 2)
              for (lines_t k = 0; k < N-1; ++k)
                for (lines_t l = k+1; l < N; ++l)
                  if (const solution_t<N> S2 = flip<N>(S1, k, l);
                      valid_da<N>(S2)) {
                    const auto it = std::lower_bound(V.begin(), V.end(), S2);
                    assert(it != V.end());
                    const Count_t w = it - V.begin();
                    if (visited[w]) continue;
                    buffer.push(w);
                  }
      }
      ++res.second;
    }
    return res;
  }


  // Map from size of connected component to absolute frequency:
  typedef std::map<ChessBoard::Count_t, ChessBoard::Count_t> freq_ccs;
  typedef std::pair<freq_ccs, ChessBoard::Count_t> Freq_ccs;

  Freq_ccs frequencies(const Solution_ccs& sc) {
    Freq_ccs res{{}, sc.second};
    solution_ccs M(sc.second);
    using ChessBoard::Count_t;
    for (const Count_t c : sc.first) ++M[c];
    for (const Count_t c : M) ++res.first[c];
    return res;
  }

  std::ostream& operator <<(std::ostream& out, const Freq_ccs& fr) {
    out << fr.second << ":";
    for (const auto p :  fr.first) out << " " << p.first << "," << p.second;
    return out;
  }

}
#endif

