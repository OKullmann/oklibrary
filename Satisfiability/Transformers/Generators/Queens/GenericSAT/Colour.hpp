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

TODOS:

1. Write function to translate numbers into colours

  - Given an interval [min, max] and x in that interval, a colour-object is
    to be computed, represented the location of x in the interval.
  - The colour-object is in the range from colour c0 to colour c1, which is understood
    as points in a vector space, and x sits on the section from c0 to c1.
  - Implement it as a function-object, constructed from min, max, c0, c1, with argument
    x.
  - Naming possibilities:
   - LinearMap, AffineMap
   - LinearTranslation, AffineTranslation.
  - Usage example:
      Creation:
        AffineMap f(min,max,c0,c1);
      Usage:
        Colour t = f(x);
      No default-construction; copying, assignment, automatic moves.
      ==, !=; no linear ordering.
      Output-streaming in natural sense, no input-streaming.
  - Rounding: the colour-object is discrete, so rounding takes place.
   - Goal: the interval [min,max] is equally spread out to appropriate
     integral points in the interval [c0,c1]. So that a random x (uniform distribution)
     yields a random f(x) (uniform distribution over the discrete values!).
   - There will be "one real number more" for c0 resp. c1, but otherwise we require
     full equality.
   - In the following we assume a < b; if a=b then we must have c0=c1.
   - Let's assume c0, c1 are just 1-dimensional (i.e., c0, c1 are real numbers).
   - First consider c0 <= c1.
   - In case of c0 = 0, we map [a,b] -> [0,c1] via considering the affine map
       h: [a,b] -> [-0.5, c1+0.5], h(a) = -0.5, h(b) = c1+0.5,
     and let f(x) := std::trunc(h(x)) (rounding towards zero in the 1/2-cases).
     Then f(a) = 0 = c0, f(b) = c1, and equal spacing is given, except that
     f(x) = 0 holds additionally at the border-argument after 0.
   - In case of c0 > 0, we perform in the construction a shift (c0,c1) -> (0,c1-c0),
     use h as above, and to the final result add c0.
   - Finally, in case of c0 > c1 we implicitly swap c0, c1, and then use the above.
  - Via static asserts, the various cases for correct rounding have to be tested
    (here in this file, if it isn't too long).

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
