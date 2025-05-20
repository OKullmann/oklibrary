// Oliver Kullmann, 20.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

*/

#ifndef TESTTOOLS_wcuguWCYfm
#define TESTTOOLS_wcuguWCYfm

#include <ranges>

#include <cstdint>

namespace TestTools {

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }

  template<typename F, typename UInt_t = std::uint64_t>
  constexpr auto makelist(const F f, const UInt_t start, const UInt_t end) {
    return std::views::iota(start, end + 1) |
      std::views::transform(f) | std::ranges::to<std::vector>();
  }


}

#endif
