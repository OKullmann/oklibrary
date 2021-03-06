// Oliver Kullmann, 30.7.2019 (Swansea)
/* Copyright 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  - const maxN = 32

  - struct ClosedLines (members d,a,r,c; type member da_t, rc_t)
  - operators ==, !=, <, <<

  - functor HashClosedLines

  - functions used_da, used_da_inverse, used_rc, used_rc_inverse to
    construct a ClosedLines-object from the ranks for a Rooks_board
  - used_lines wraps usage of used_da, used_rc

  - scoped enum CS (caching schemes)
  - operator <<

  - typedef Count_t

  - class FullCaching_map

  - helper type-traits memman ("memory management")
  - class template FullSymCaching<MAP>
  - instantiations FullSymCaching_map, FullSymCaching_hash

*/

/* TODOS:

0. Enter the experimental results.

1. Provide restricted caching-versions
    - Easiest seems to start with caching only nodes with depth
      <= N/k for a given constant k.

3. Provide parallelisation (threads)
    - map_t (as now in FullCaching_map) should be a map from ClosedLines
      to some a pair of Count_t and a vector of iterator's into the map --
      these are the counts for cache-values which where unknown at the
      time when they were encountered. After finishing the main computation
      (the implicit dag), bottom-up all unknowns are computed, and the value
      for the root is the total count.
    - The class Backtracking::StatisticsRC then needs a variation likely
      without the count.
    - There is a todo on parallelisation in Backtracking.hpp.

*/

#ifndef CACHING_W2cK5yfaQT
#define CACHING_W2cK5yfaQT

#include <map>
#include <utility>
#include <ostream>
#include <bitset>
#include <optional>
#include <functional>

#include <cstdint>

#include "Numerics/FloatingPoint.hpp"

#include "ChessBoard.hpp"
#include "Recursion.hpp"

namespace Caching {

  constexpr ChessBoard::coord_t maxN = 32;

  // Used for fixed 1 <= N <= maxN:
  struct ClosedLines {
    typedef std::uint64_t da_t;
    typedef std::uint32_t rc_t;
    da_t d, a;
    rc_t r, c;
  };
  static_assert(is_pod(ClosedLines));
  inline constexpr bool operator ==(const ClosedLines& x, const ClosedLines& y) noexcept {
    return x.d == y.d and x.a == y.a and x.r == y.r and x.c == y.c;
  }
  inline constexpr bool operator !=(const ClosedLines& x, const ClosedLines& y) noexcept {
    return not (x == y);
  }
  inline constexpr bool operator <(const ClosedLines& x, const ClosedLines& y) noexcept {
    if (x.d < y.d) return true;
    else if (y.d < x.d) return false;
    if (x.a < y.a) return true;
    else if (y.a < x.a) return false;
    using da_t = ClosedLines::da_t;
    return ((da_t(x.r) << 32) | da_t(x.c)) < ((da_t(y.r) << 32) | da_t(y.c));
  }

  std::ostream& operator <<(std::ostream& out, const ClosedLines& cl) {
    return out << std::bitset<32>(cl.r) << " " << std::bitset<32>(cl.c) << " "
               << std::bitset<64>(cl.d) << " " << std::bitset<64>(cl.a)
               << "\n";
  }

  struct HashClosedLines {
    std::size_t operator ()(const ClosedLines& cl) const noexcept {
      const std::hash<ClosedLines::da_t> hash_da;
      const std::hash<ClosedLines::rc_t> hash_rc;
      // floor(2^64 / ((1+sqrt(5))/2)) :
      constexpr size_t magic_random = 0x9e3779b97f4a7c15;
      size_t result = hash_da(cl.d);
      result ^= hash_da(cl.a) + magic_random + (result << 6) + (result >> 2);
      result ^= hash_rc(cl.r) + magic_random + (result << 6) + (result >> 2);
      result ^= hash_rc(cl.c) + magic_random + (result << 6) + (result >> 2);
      return result;
    }
  };


  // Diagonals/antidiagonals are "used" (closed) iff no open field anymore;
  // coding rs[i] into bit i of the da_t-return-value:
  inline ClosedLines::da_t used_da(const ChessBoard::Rooks_Board::Ranks& rs) noexcept {
    assert(not rs.empty() and rs.size() <= 64);
    ClosedLines::da_t mask = 1;
    ClosedLines::da_t result = 0;
    for (const auto r : rs) {
      if (r.o == 0) result |= mask;
      mask <<= 1;
    }
    return result;
  }
  // Now reversing the order of bits, considering 2N-1 diagonals/antidiagonals:
  inline ClosedLines::da_t used_da_inverse(const ChessBoard::Rooks_Board::Ranks& rs, const ChessBoard::coord_t N) noexcept {
    assert(N >= 1 and N <= 32);
    const ChessBoard::coord_t numd = 2*N-1;
    assert(rs.size() == numd);
    ClosedLines::da_t mask = 1; mask <<= numd-1;
    ClosedLines::da_t result = 0;
    for (const auto r : rs) {
      if (r.o == 0) result |= mask;
      mask >>= 1;
    }
    return result;
  }
  // Rows/columns are "used" (closed) iff at least one placement is in it;
  // rs[i] goes into bit i-1 of the rc_t-return-value (here we have i >= 1):
  inline ClosedLines::rc_t used_rc(const ChessBoard::Rooks_Board::Ranks& rs) noexcept {
    assert(rs.size() >= 2 and rs.size() <= 33);
    ClosedLines::rc_t result = 0;
    ClosedLines::rc_t mask = 1;
    for (ChessBoard::coord_t i = 1; i < rs.size(); ++i, mask <<= 1)
      if (rs[i].p != 0) result |= mask;
    return result;
  }
  // Now reversing order of bits, considering N rows/columns:
  inline ClosedLines::rc_t used_rc_inverse(const ChessBoard::Rooks_Board::Ranks& rs, const ChessBoard::coord_t N) noexcept {
    assert(N >= 1 and N <= 32);
    assert(rs.size() == N+1);
    ClosedLines::rc_t result = 0;
    ClosedLines::rc_t mask = 1; mask <<= N-1;
    for (ChessBoard::coord_t i = 1; i < rs.size(); ++i, mask >>= 1)
      if (rs[i].p != 0) result |= mask;
    return result;
  }

  inline ClosedLines used_lines(const ChessBoard::Board& B) noexcept {
    assert(B.N <= maxN);
    return {
      used_da(B.d_rank()), used_da(B.ad_rank()),
      used_rc(B.r_rank()), used_rc(B.c_rank())};
  }


  // Caching schemes:
  enum class CS {
    none = 0,
    full_ordered = 1,
    fullsym_ordered = 2,
    fullsym_unordered = 3
  };
  // "full" : all internal nodes
  // "ordered": using std::map
  // "unordered": using std::unordered_map
  // "sym": using the 8 symmetries

  constexpr int maxCS = int(CS::fullsym_unordered);
  constexpr int maxCachOptions = Caching::maxCS;

  std::ostream& operator <<(std::ostream& out, const CS cs) {
    switch(cs) {
      case CS::none : return out << "none";
      case CS::full_ordered : return out << "full_ordered";
      case CS::fullsym_ordered : return out << "fullsym_ordered";
      case CS::fullsym_unordered : return out << "fullsym_unordered";
      default : return out << "CS_uncovered:" << int(cs);
    }
  }

  typedef ChessBoard::Count_t Count_t;

  class FullCaching_map {
    typedef ChessBoard::Board Board;
    typedef std::map<ClosedLines, Count_t> map_t;
    typedef map_t::const_iterator iterator;
    static map_t M;
  public :
    static void init(const ChessBoard::coord_t) noexcept {}
    typedef ClosedLines cache_t;
    typedef map_t::size_type size_t;
    static size_t size() noexcept { return M.size(); }
    static double load_factor() noexcept { return 1; }
    typedef std::optional<Count_t> return_t;
    static cache_t hash(const Board& B) noexcept {
      return used_lines(B);
    }
    static return_t find(const cache_t& h, const Board&) noexcept {
      if (const iterator found = M.find(h); found == M.end()) return {};
      else return found->second;
    }
    static bool insert(const cache_t& h, const Count_t c) {
      return M.emplace(h, c).second;
    }
  };
  FullCaching_map::map_t FullCaching_map::M;

  // Whether the MAP-type has memory-management:
  template <class T> struct memman { static constexpr bool v = false; };
  template <class K, class T, class H, class E, class A>
  struct memman<std::unordered_map<K,T,H,E,A>> {static constexpr bool v=true;};

  template <class MAP>
  class FullSymCaching {
    typedef ChessBoard::Board Board;
    typedef MAP map_t;
    typedef typename map_t::iterator iterator;
    static map_t M;
  public :
    static constexpr bool has_memory_management = memman<map_t>::v;
    // Anticipated quotient cache_size / number_solutions:
    static constexpr FloatingPoint::float80 quot = 2;
    static constexpr FloatingPoint::float80 max_load_factor = 0.5;
    static void init([[maybe_unused]] const ChessBoard::coord_t N) {
      if constexpr (has_memory_management) {
        M.max_load_factor(max_load_factor);
        M.reserve(FloatingPoint::toUInt(quot*Recursion::strong_conjecture(N)));
      }
    }
    typedef ClosedLines cache_t;
    typedef typename map_t::size_type size_t;
    static size_t size() noexcept { return M.size(); }
    static double load_factor() noexcept {
      if constexpr (has_memory_management)
        return M.load_factor();
      else return 1;
    }

    static cache_t hash(const Board& B) noexcept {
      return used_lines(B);
    }
    typedef std::optional<Count_t> return_t;
    static return_t find(const cache_t& h, const Board& B) noexcept {
      const iterator end = M.end();
      const auto [D,A,R,C] = h;

      if (const auto it = M.find({D,A,R,C}); it != end) return it->second;

      const auto iD = used_da_inverse(B.d_rank(), B.N);
      // reflexion main-diagonal:
      if (const auto it = M.find({iD,A,C,R}); it != end) return it->second;

      const auto iR = used_rc_inverse(B.r_rank(), B.N);
      // reflexion horizontal:
      if (const auto it = M.find({A,D,iR,C}); it != end) return it->second;

      const auto iC = used_rc_inverse(B.c_rank(), B.N);
      // rotation 270:
      if (const auto it = M.find({A,iD,iC,R}); it != end) return it->second;

      const auto iA = used_da_inverse(B.ad_rank(), B.N);
      // rotation 180 (point-symmetry):
      if (const auto it = M.find({iD,iA,iR,iC}); it != end) return it->second;
      // rotation 90:
      if (const auto it = M.find({iA,D,C,iR}); it != end) return it->second;
      // reflexion main-anti-diagonal:
      if (const auto it = M.find({D,iA,iC,iR}); it != end) return it->second;
      // reflexion vertical:
      if (const auto it = M.find({iA,iD,R,iC}); it != end) return it->second;
      return {};
    }
    static bool insert(const cache_t& h, const Count_t c) {
      return M.emplace(h, c).second;
    }
  };
  template <class MAP>
  typename FullSymCaching<MAP>::map_t FullSymCaching<MAP>::M;

  typedef FullSymCaching<std::map<ClosedLines, Count_t>> FullSymCaching_map;
  typedef FullSymCaching<std::unordered_map<ClosedLines, Count_t, HashClosedLines>> FullSymCaching_hash;
  static_assert(FullSymCaching_hash::has_memory_management);

}

#endif
