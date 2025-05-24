// Oliver Kullmann, 20.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  - eqp<X>(X lhs, X rhs) -> bool

  - makelist<F,UINT>(F f, UINT a, UINT b) -> std::vector

  - all_read(istream) -> bool

*/

#ifndef TESTTOOLS_wcuguWCYfm
#define TESTTOOLS_wcuguWCYfm

#include <ranges>
#include <vector>
#include <istream>
#include <type_traits>

#include <cstdint>

namespace TestTools {

  template <class X>
  constexpr bool eqp(const X& lhs,
                     const std::type_identity_t<X>& rhs) noexcept {
    return lhs == rhs;
  }

  template<typename F, typename UINT = std::uint64_t>
  constexpr auto makelist(const F f, const UINT start, const UINT end) {
    return std::views::iota(start, end + 1) |
      std::views::transform(f) | std::ranges::to<std::vector>();
  }


  // Everthing has been read (by "overreading"), but no error:
  bool all_read(std::istream& s) noexcept {
    return not s.good() and not s.bad() and not s.fail() and s.eof() and
      s.peek() == std::char_traits<char>::eof();
  }
  // Now by "reading beyond":
  bool all_read_beyond(std::istream& s) noexcept {
    return not s.good() and not s.bad() and s.fail() and s.eof() and
      s.peek() == std::char_traits<char>::eof();
  }

}

#endif
