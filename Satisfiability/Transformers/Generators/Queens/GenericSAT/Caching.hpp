// Oliver Kullmann, 30.7.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* TODOS:

1. Symmetry handling

   - Write a function which for a ClosedLines-object computes all 8
     symmetric versions of it.
   - Then write a version of FullCaching_map where find checks whether
     any of the symmetric versions is available.

*/

#ifndef CACHING_W2cK5yfaQT
#define CACHING_W2cK5yfaQT

#include <map>
#include <utility>
#include <ostream>
#include <bitset>

#include <cstdint>

#include "ChessBoard.hpp"

namespace Caching {

  constexpr ChessBoard::coord_t maxN = 32;
  // Used for fixed 1 <= N <= maxN:
  struct ClosedLines {
    typedef std::uint64_t da_t;
    typedef std::uint32_t rc_t;
    da_t d, a;
    rc_t r, c;
  };
  static_assert(std::is_pod_v<ClosedLines>);
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

  inline ClosedLines::da_t used_da(const ChessBoard::Rooks_Board::Ranks& rs) noexcept {
    assert(not rs.empty() and rs.size() <= 64);
    ClosedLines::da_t mask = 1;
    ClosedLines::da_t result = 0;
    for (const auto r : rs) {
      if (r.p != 0) result |= mask;
      mask <<= 1;
    }
    return result;
  }
  inline ClosedLines::da_t used_da_inverse(const ChessBoard::Rooks_Board::Ranks& rs, const ChessBoard::coord_t N) noexcept {
    assert(N >= 1 and N <= 32);
    const ChessBoard::coord_t numd = 2*N-1;
    assert(rs.size() == numd);
    ClosedLines::da_t mask = 1; mask <<= numd-1;
    ClosedLines::da_t result = 0;
    for (const auto r : rs) {
      if (r.p != 0) result |= mask;
      mask >>= 1;
    }
    return result;
  }
  inline ClosedLines::rc_t used_rc(const ChessBoard::Rooks_Board::Ranks& rs) noexcept {
    assert(rs.size() >= 2 and rs.size() <= 32);
    ClosedLines::rc_t result = 0;
    ClosedLines::rc_t mask = 1;
    for (ChessBoard::coord_t i = 1; i < rs.size(); ++i, mask <<= 1)
      if (rs[i].p != 0) result |= mask;
    return result;
  }
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
  enum class CS { none = 0, full_ordered = 1, fullsym_ordered = 2 };
  // "full" : all internal nodes
  // "ordered": using std::map
  // "sym": using the 8 symmetries
  std::ostream& operator <<(std::ostream& out, const CS cs) {
    switch(cs) {
      case CS::none : return out << "none";
      case CS::full_ordered : return out << "full_ordered";
      case CS::fullsym_ordered : return out << "fullsym_ordered";
      default : return out << "CS_uncovered:" << int(cs);
    }
  }

  class FullCaching_map {
    typedef ChessBoard::Count_t Count_t;
    typedef std::map<ClosedLines, Count_t> map_t;
    typedef map_t::iterator iterator;
    static map_t M;
  public :
    static auto size() noexcept { return M.size(); }
    typedef std::pair<iterator, bool> return_t;
    static return_t find(const ChessBoard::Board& B) noexcept {
      const auto [it, inserted] = M.insert({used_lines(B), 0});
      return {it, not inserted};
    }
  };
  FullCaching_map::map_t FullCaching_map::M;

  class FullSymCaching_map {
    typedef ChessBoard::Count_t Count_t;
    typedef std::map<ClosedLines, Count_t> map_t;
    typedef map_t::iterator iterator;
    static map_t M;
  public :
    static auto size() noexcept { return M.size(); }
    typedef std::pair<iterator, bool> return_t;
    static return_t find(const ChessBoard::Board& B) noexcept {
      const iterator end = M.end();
      const auto D = used_da(B.d_rank());
      const auto A = used_da(B.ad_rank());
      const auto R = used_rc(B.r_rank());
      const auto C = used_rc(B.c_rank());

      iterator it = M.find({D,A,R,C});
      if (it != end) return {it, true};

      const auto iD = used_da_inverse(B.d_rank(), B.N);
      it = M.find({iD,A,C,R}); // reflexion main-diagonal
      if (it != end) return {it, true};

      const auto iR = used_rc_inverse(B.r_rank(), B.N);
      it = M.find({A,D,iR,C}); // reflexion horizontal
      if (it != end) return {it, true};

      const auto iC = used_rc_inverse(B.c_rank(), B.N);
      it = M.find({A,iD,iC,R}); // rotation 270
      if (it != end) return {it, true};

      const auto iA = used_da_inverse(B.ad_rank(), B.N);
      it = M.find({iD,iA,iR,iC}); // rotation 180 (point-symmetry)
      if (it != end) return {it, true};
      it = M.find({iA,D,C,iR}); // rotation 90
      if (it != end) return {it, true};
      it = M.find({D,iA,iC,iR}); // reflexion main-anti-diagonal
      if (it != end) return {it, true};
      it = M.find({iA,iD,R,iC}); // reflexion vertical
      if (it != end) return {it, true};
      return {M.insert({{D,A,R,C}, 0}).first, false};
    }
  };
  FullSymCaching_map::map_t FullSymCaching_map::M;

}

#endif
