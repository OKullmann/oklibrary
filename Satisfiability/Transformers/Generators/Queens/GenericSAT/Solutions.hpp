// Irfansha Shaik, 21.11.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Content:


TODOS:

0. Create list of contents

1. Improved algorithms for determining the 1- and 2-neighbours
   1. DONE For a flip, only positions i, j changed, and thus testing for being
      a solution can be done more efficiently.
     - The two new diagonals/antidiagonals must be new.
     - So one can compute the (direct-addressing) hash-table for all elements
       once at the beginning, and for each i,j-iteration needs only to
       check these two values.
   2. That's for the 1-neighbourhoods; what about the 2-neighbourhoods?
     - Strangely, using the same approach as above here, for the first
       flip, impairs runtime by 10% (on csltok)??
   3. DONE When running through all 2-neighbours, via flipping i,j, k,l,
      w.l.o.g. we can assume i <= k.

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
  inline solution_t<N> extract(const ChessBoard::Rooks_Board b) noexcept {
    assert(N == b.N);
    solution_t<N> S;
    for (lines_t i = 0; i < N; ++i) {
      assert(b.r_rank()[i+1].p == 1);
      const auto& row = b()[i+1];
      const auto b = row.begin() + 1;
      S[i] = std::find(b, row.end(), ChessBoard::State::placed) - b;
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

  template <lines_t N>
  inline constexpr lines_t diagonal(const lines_t r, const lines_t c) noexcept {
    return (N-1) + r - c;
  }
  static_assert(diagonal<4>(0,3) == 0);
  static_assert(diagonal<4>(0,0) == 3);
  static_assert(diagonal<4>(3,0) == 6);
  template <lines_t N>
  inline constexpr lines_t antidiagonal(const lines_t r, const lines_t c) noexcept {
    return r + c;
  }
  static_assert(antidiagonal<4>(0,0) == 0);
  static_assert(antidiagonal<4>(0,3) == 3);
  static_assert(antidiagonal<4>(3,3) == 6);

  template <lines_t N>
    using hash_d_t = std::array<lines_t, 2*N-1>;
  template <lines_t N>
    using hash_a_t = std::array<lines_t, 2*N-1>;
  template <lines_t N>
    using hash_da_t = std::pair<hash_d_t<N>, hash_a_t<N>>;

  template <lines_t N>
  inline constexpr hash_da_t<N> hash_da(const solution_t<N>& S) noexcept {
    hash_da_t<N> res{};
    for (lines_t i = 0; i < N; ++i) {
      res.first[diagonal<N>(i, S[i])] = true;
      res.second[antidiagonal<N>(i, S[i])] = true;
    }
    return res;
  }
  // static_assert(hash_da<4>({1,3,0,2}) == hash_da_t<4>{{false,true,true,false,true,true,false}, {false,true,true,false,true,true,false}}); // C++20

  // Checks if there are two queens placed in same diagonal or anti-diagonal:
  template <lines_t N>
  inline constexpr bool valid_da(const solution_t<N>& S) noexcept {
    hash_d_t<N> occurred_d{};
    hash_a_t<N> occurred_a{};
    for (lines_t i = 0; i < N; ++i) {
      assert(S[i] < N);
      const lines_t d = diagonal<N>(i, S[i]);
      const lines_t a = antidiagonal<N>(i, S[i]);
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


  struct Stats {
    typedef ChessBoard::Count_t Count_t;
    Count_t num_cc;
    Count_t sum_deg;
  };
  std::ostream& operator <<(std::ostream& out, const Stats& s) {
    return out << s.num_cc;
  }

  typedef std::vector<ChessBoard::Count_t> solution_ccs;
  struct Solution_ccs {
    solution_ccs vec_cc;
    Stats s;
    explicit Solution_ccs(const solution_ccs::size_type s)
      : vec_cc(s), s({}) {}
    // For debugging:
    Solution_ccs(solution_ccs v, ChessBoard::Count_t c)
      : vec_cc(v), s({c,0}) {}
  };

  template <lines_t N>
  Solution_ccs determine_ccs1(const solution_vector<N>& V) {
    assert(std::is_sorted(V.begin(), V.end()));
    Solution_ccs res(V.size());
    std::vector<bool> visited(V.size());
    using ChessBoard::Count_t;
    std::stack<Count_t> buffer;
    const auto begin = V.begin(), end = V.end();
    for (Count_t x = 0; x < V.size(); ++x) {
      if (visited[x]) continue;
      buffer.push(x); visited[x] = true;
      do {
        assert(not buffer.empty());
        const Count_t v = buffer.top(); buffer.pop();
        assert(v < V.size());
        assert(visited[v]);
        res.vec_cc[v] = res.s.num_cc;
        const solution_t<N>& S0 = V[v];
        const auto [occursd, occursa] = hash_da<N>(S0);
        for (lines_t i = 0; i < N-1; ++i)
          for (lines_t j = i+1; j < N; ++j) {
            if (not occursd[diagonal<N>(i,S0[j])] and
                not occursa[antidiagonal<N>(i,S0[j])] and
                not occursd[diagonal<N>(j,S0[i])] and
                not occursa[antidiagonal<N>(j,S0[i])]) {
              ++res.s.sum_deg;
              const solution_t<N> S1 = flip<N>(S0, i, j);
              assert(valid_da<N>(S1));
              const auto it = std::lower_bound(begin, end, S1);
              assert(it != end);
              const Count_t w = it - begin;
              if (visited[w]) continue;
              buffer.push(w); visited[w] = true;
            }
          }
      } while (not buffer.empty());
      ++res.s.num_cc;
    }
    return res;
  }

  template <lines_t N>
  Solution_ccs determine_ccs2(const solution_vector<N>& V) {
    assert(std::is_sorted(V.begin(), V.end()));
    Solution_ccs res(V.size());
    std::vector<bool> visited(V.size());
    using ChessBoard::Count_t;
    std::stack<Count_t> buffer;
    const auto begin = V.begin(), end = V.end();
    for (Count_t x = 0; x < V.size(); ++x) {
      if (visited[x]) continue;
      buffer.push(x); visited[x] = true;
      do {
        assert(not buffer.empty());
        const Count_t v = buffer.top(); buffer.pop();
        assert(v < V.size());
        assert(visited[v]);
        visited[v] = true;
        res.vec_cc[v] = res.s.num_cc;
        const solution_t<N>& S0 = V[v];
        for (lines_t i = 0; i < N-1; ++i)
          for (lines_t j = i+1; j < N; ++j) {
            const solution_t<N> S1 = flip<N>(S0, i, j);
            if (valid_da<N>(S1)) {
              assert(valid_da<N>(S1));
              const auto it = std::lower_bound(begin, end, S1);
              assert(it != end);
              if (const Count_t w = it - begin; not visited[w])
                { buffer.push(w); visited[w] = true; }
            }
            else {
              assert(not valid_da<N>(S1));
              for (lines_t k = i; k < N-1; ++k)
                for (lines_t l = k+1; l < N; ++l) {
                  if (const solution_t<N> S2 = flip<N>(S1, k, l);
                      valid_da<N>(S2)) {
                    const auto it = std::lower_bound(begin, end, S2);
                    assert(it != end);
                    if (const Count_t w = it - begin; not visited[w])
                      { buffer.push(w); visited[w] = true; }
                  }
                }
            }
          }
      } while (not buffer.empty());
      ++res.s.num_cc;
    }
    return res;
  }


  // Map from size of connected component to absolute frequency:
  typedef std::map<ChessBoard::Count_t, ChessBoard::Count_t> freq_ccs;
  struct Freq_ccs {
    freq_ccs freq;
    Stats s;
  };
  bool operator ==(const Freq_ccs& lhs, const Freq_ccs& rhs) noexcept {
    return lhs.freq == rhs.freq;
  }
  std::ostream& operator <<(std::ostream& out, const Freq_ccs& fr) {
    out << fr.s << ":";
    for (const auto p :  fr.freq) out << " " << p.first << "," << p.second;
    return out;
  }

  Freq_ccs frequencies(const Solution_ccs& sc) {
    Freq_ccs res{{}, sc.s};
    solution_ccs M(sc.s.num_cc);
    using ChessBoard::Count_t;
    for (const Count_t c : sc.vec_cc) ++M[c];
    for (const Count_t c : M) ++res.freq[c];
    return res;
  }

}
#endif

