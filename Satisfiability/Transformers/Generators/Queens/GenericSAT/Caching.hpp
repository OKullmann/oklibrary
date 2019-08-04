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

#include <cstdint>

#include "ChessBoard.hpp"

namespace Caching {

  constexpr ChessBoard::coord_t maxN = 32;
  // Used for fixed 1 <= N <= maxN:
  struct ClosedLines {
    typedef std::uint32_t rc_t;
    typedef std::uint64_t da_t;
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

  ClosedLines used_lines(const ChessBoard::Board& B) noexcept {
    assert(B.N <= maxN);
    ClosedLines L{};
    using rc_t = ClosedLines::rc_t;
    {rc_t mask = 1;
     for (const auto rr : B.r_rank()) {
       if (rr.p != 0) L.r |= mask;
       mask <<= 1;
     }
    }
    {rc_t mask = 1;
     for (const auto cr : B.c_rank()) {
       if (cr.p != 0) L.c |= mask;
       mask <<= 1;
     }
    }
    using da_t = ClosedLines::da_t;
    {da_t mask = 1;
     for (const auto dr : B.d_rank()) {
       if (dr.p != 0) L.d |= mask;
       mask <<= 1;
     }
    }
    {da_t mask = 1;
     for (const auto adr : B.ad_rank()) {
       if (adr.p != 0) L.a |= mask;
       mask <<= 1;
     }
    }
    return L;
  }

  // Caching schemes:
  enum class CS { none = 0, full_ordered = 1 };
  std::ostream& operator <<(std::ostream& out, const CS cs) {
    switch(cs) {
      case CS::none : return out << "none";
      case CS::full_ordered : return out << "full_ordered";
      default : return out << "CS_uncovered:" << int(cs);
    }
  }

  class FullCaching_map {
    typedef ChessBoard::Count_t Count_t;
    typedef std::map<ClosedLines, Count_t> map_t;
    typedef map_t::iterator iterator;
    static map_t M;
  public :
    typedef std::pair<iterator, bool> return_t;
    static return_t find(const ChessBoard::Board& B) noexcept {
      const auto [it, inserted] = M.insert({used_lines(B), 0});
      return {it, not inserted};
    }
  };
  FullCaching_map::map_t FullCaching_map::M;

}

#endif
