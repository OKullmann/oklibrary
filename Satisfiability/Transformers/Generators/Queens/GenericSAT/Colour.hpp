// Oliver Kullmann, 14.10.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Content:

    - type rgba_index
    - class Colour, Colour_v
    - functions grey, black, white, red, green, blue for classification.

*/

#ifndef COLOUR_44nWupcRld
#define COLOUR_44nWupcRld

#include <vector>
#include <type_traits>
#include <ostream>

#include <cstdint>
#include <cassert>

namespace Colour {

  // Colours with opacity as in Tulip

  typedef std::uint8_t rgba_index;
  struct Colour {
    rgba_index r, g, b, a=255;
  };
  static_assert(not std::is_pod_v<Colour>);

  typedef std::vector<Colour> Colour_v;

  inline constexpr bool operator ==(const Colour lhs, const Colour rhs) noexcept {
    return lhs.r == rhs.r and lhs.g == rhs.g and lhs.b == rhs.b and lhs.a == rhs.a;
  }
  inline constexpr bool operator !=(const Colour lhs, const Colour rhs) noexcept {
    return lhs.r != rhs.r or lhs.g != rhs.g or lhs.b != rhs.b or lhs.a != rhs.a;
  }

  inline std::ostream& operator <<(std::ostream& out, const Colour c) {
    return out << "(" << +c.r << "," << +c.g << "," << +c.b << "," << +c.a << ")";
  }

  inline constexpr bool grey(const Colour c) noexcept {
    return c.r == c.g and c.g == c.b;
  }
  inline constexpr bool black(const Colour c) noexcept {
    return c.r == 0 and c.g == 0 and c.b == 0;
  }
  inline constexpr bool white(const Colour c) noexcept {
    return c.r == 0xFF and c.g == 0xFF and c.b == 0xFF;
  }
  inline constexpr bool red(const Colour c) noexcept {
    return c.r != 0 and c.g == 0 and c.b == 0;
  }
  inline constexpr bool green(const Colour c) noexcept {
    return c.r == 0 and c.g != 0 and c.b == 0;
  }
  inline constexpr bool blue(const Colour c) noexcept {
    return c.r == 0 and c.g == 0 and c.b != 0;
  }

}

#endif
