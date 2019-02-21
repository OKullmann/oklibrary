// Oliver Kullmann, 14.10.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Content:

    - integral-type rgb_t
    - array-types rgb3_t, rgb4_t
    - classes Colour3, Colour, with accompanying operators ==, !=, <<
    - auxiliary classes Colour3_v, Colour_v
    - constants of type Colour3:
     - black3, white3
     - red3, green3, blue3 for primary colours
     - yellow3, magenta3, cyan3 for secondary colours
    - functions grey, black, white, red, green, blue, yellow, magenta, cyan
      for classification

TODOS:

1. Write function to translate numbers into colours

  - Given an interval [min, max] and x in that interval, a colour-object is
    to be computed, represented the location of x in the interval.
  - min, max, x are of type Double, a template parameter, which should work
    for float, double, long double.
  - Likely there should also be a second template parameter, ColourT, since
    we might use colour-objects of length 3 or 4 (for example).
  - The colour-object is in the range from colour c0 to colour c1, which is
    understood as points in a vector space, and x sits on the section from c0
    to c1.
  - Implement it as a function-object, constructed from min, max, c0, c1,
    with argument x.
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
     integral points in the interval [c0,c1]. So that a random x (uniform
     distribution) yields a random f(x) (uniform distribution over the
     discrete values!).
   - There will be "one real number more" for c0 resp. c1, but otherwise we
     require full equality.
   - In the following we assume a < b; if a=b then we must have c0=c1.
   - Let's assume c0, c1 are just 1-dimensional (i.e., c0, c1 are real
     numbers).
   - First consider c0 <= c1.
   - In case of c0 = 0, we map [a,b] -> [0,c1] via considering the affine map
       h: [a,b] -> [-0.5, c1+0.5], h(a) = -0.5, h(b) = c1+0.5,
     and let f(x) := std::trunc(h(x)) (rounding towards zero in the 1/2-cases).
     Then f(a) = 0 = c0, f(b) = c1, and equal spacing is given, except that
     f(x) = 0 holds additionally at the border-argument after 0.
   - In case of c0 > 0, we perform in the construction a shift
       (c0,c1) -> (0,c1-c0),
     use h as above, and to the final result add c0.
   - Finally, in case of c0 > c1 we implicitly swap c0, c1, and then use the
     above.
  - Via static asserts, the various cases for correct rounding have to be
    tested (here in this file, if it isn't too long).

*/

#ifndef COLOUR_44nWupcRld
#define COLOUR_44nWupcRld

#include <array>
#include <vector>
#include <type_traits>
#include <ostream>

#include <cstdint>
#include <cassert>

namespace Colour {

  // Colours with opacity as in Tulip

  // First the number-type for 0,...,255, and the arrays of length3 and 4:
  typedef std::uint8_t rgb_t;
  typedef std::array<rgb_t,3> rgb3_t;
  static_assert(std::is_pod_v<rgb3_t>);
  typedef std::array<rgb_t,4> rgb4_t;
  static_assert(std::is_pod_v<rgb4_t>);

  /*
    Colour3: wrapper around a std::array of 3 colour-indices, according to
    the RGB-colour-model https://en.wikipedia.org/wiki/RGB_color_model

    Construction:
     - default construction is zero-initialisation
     - from colour indices a,b,c
     - from a colour-array of length 3 (which is copied).
    Copy-construction and copy-assignment as well as their move-counterparts
    are defined by default.

    == and != considers the object as vector.

    Read-only access is given by members r(), g(), b().

    Read- and write access to the array is given by * and ->.
    Index-access via [] is also provided directly.
  */
  struct Colour3 {
  private :
    rgb3_t a;
  public :
    using size_type = rgb3_t::size_type;

    Colour3() noexcept = default;
    constexpr Colour3(const rgb_t a, const rgb_t b, const rgb_t c) noexcept : a{a,b,c} {}
    constexpr Colour3(const rgb3_t a) noexcept : a(a) {}

    constexpr const rgb3_t& operator *() const noexcept { return a; }
    rgb3_t& operator *() noexcept { return a; }

    constexpr const rgb3_t* operator->() const noexcept { return &a; }
    rgb3_t* operator->() noexcept { return &a; }

    constexpr rgb_t operator[](const size_type i) const noexcept {return a[i];}
    rgb_t& operator[](const size_type i) noexcept { return a[i]; }

    constexpr rgb_t r() const noexcept { return a[0]; }
    constexpr rgb_t g() const noexcept { return a[1]; }
    constexpr rgb_t b() const noexcept { return a[2]; }
  };
  inline constexpr bool operator ==(const Colour3& lhs, const Colour3& rhs) noexcept {
    return lhs[0] == rhs[0] and lhs[1] == rhs[1] and lhs[2] == rhs[2];
  }
  inline constexpr bool operator !=(const Colour3& lhs, const Colour3& rhs) noexcept {
    return not (lhs == rhs);
  }
  static_assert(std::is_pod_v<Colour3>);
  static_assert(Colour3{}.r() == 0);
  static_assert(Colour3().g() == 0);
  static_assert(Colour3(11,22,33).b() == 33);

  inline std::ostream& operator <<(std::ostream& out, const Colour3 c) {
    return out << "(" << +c.r() << "," << +c.g() << "," << +c.b() << ")";
  }

  constexpr Colour3 black3{};
  static_assert(black3[0] + black3[1] + black3[2] == 0);
  static_assert(black3 == Colour3{});
  constexpr Colour3 white3(-1,-1,-1);
  static_assert(white3[0] + white3[1] + white3[2] == 765);
  static_assert(white3 != black3);
  // Primary colours:
  constexpr Colour3 red3(-1,0,0);
  constexpr Colour3 green3(0,-1,0);
  constexpr Colour3 blue3(0,0,-1);
  // Secondary colours:
  constexpr Colour3 yellow3(-1,-1,0);
  constexpr Colour3 magenta3(-1,0,-1);
  constexpr Colour3 cyan3(0,-1,-1);


  /* Colour: wrapper around a std::array of 4 colour-indices, according to
  RGBA-colour-model https://en.wikipedia.org/wiki/RGBA_color_space

    As Colour, but now with a four argument, and additional conversions:
     - conversion constructor from Colour3 adds value 255
     - conversion operator to Colour3 removes additional value.

  */

  struct Colour4 {
  private :
    rgb4_t arr;
  public :
    using size_type = rgb4_t::size_type;

    Colour4() noexcept = default;
    constexpr Colour4(const rgb_t r, const rgb_t g, const rgb_t b, const rgb_t a) noexcept : arr{r,g,b,a} {}
    constexpr Colour4(const rgb4_t a) noexcept : arr(a) {}
    constexpr Colour4(const Colour3 c) noexcept : arr{c[0],c[1],c[2],255} {}

    operator Colour3() noexcept { return {arr[0],arr[1],arr[2]}; }

    constexpr const rgb4_t& operator *() const noexcept { return arr; }
    rgb4_t& operator *() noexcept { return arr; }

    constexpr const rgb4_t* operator ->() const noexcept { return &arr; }
    rgb4_t* operator ->() noexcept { return &arr; }

    constexpr rgb_t operator [](const size_type i) const noexcept {
      return arr[i];
    }
    rgb_t& operator [](const size_type i) noexcept { return arr[i]; }

    constexpr rgb_t r() const noexcept { return arr[0]; }
    constexpr rgb_t g() const noexcept { return arr[1]; }
    constexpr rgb_t b() const noexcept { return arr[2]; }
    constexpr rgb_t a() const noexcept { return arr[3]; }
  };
  inline constexpr bool operator ==(const Colour4 lhs, const Colour4 rhs) noexcept {
    return lhs[0]==rhs[0] and lhs[1]==rhs[1] and lhs[2] == rhs[2] and lhs[3] == rhs[3];
  }
  inline constexpr bool operator !=(const Colour4 lhs, const Colour4 rhs) noexcept {
    return not(lhs == rhs);
  }
  static_assert(std::is_pod_v<Colour4>);

  inline std::ostream& operator <<(std::ostream& out, const Colour4 c) {
    return out << "(" << +c.r() << "," << +c.g() << "," << +c.b() << "," << +c.a() << ")";
  }

  typedef std::vector<Colour3> Colour3_v;
  typedef std::vector<Colour4> Colour4_v;


  inline constexpr bool grey(const Colour3 c) noexcept {
    return c.r() == c.g() and c.g() == c.b();
  }
  static_assert(grey(white3));
  static_assert(grey(black3));
  static_assert(not grey(red3));
  inline constexpr bool black(const Colour3 c) noexcept {
    return c == black3;
  }
  static_assert(black(black3));
  inline constexpr bool white(const Colour3 c) noexcept {
    return c == white3;
  }
  static_assert(white(white3));
  inline constexpr bool red(const Colour3 c) noexcept {
    return c.r() != 0 and c.g() == 0 and c.b() == 0;
  }
  static_assert(red(red3));
  static_assert(not red(black3));
  inline constexpr bool green(const Colour3 c) noexcept {
    return c.r() == 0 and c.g() != 0 and c.b() == 0;
  }
  static_assert(green(green3));
  static_assert(not green(black3));
  inline constexpr bool blue(const Colour3 c) noexcept {
    return c.r() == 0 and c.g() == 0 and c.b() != 0;
  }
  static_assert(blue(blue3));
  static_assert(not blue(black3));
  inline constexpr bool yellow(const Colour3 c) noexcept {
    return c.r() == c.g() and c.b() == 0;
  }
  static_assert(yellow(yellow3));
  static_assert(not yellow(red3));
  inline constexpr bool magenta(const Colour3 c) noexcept {
    return c.r() == c.b() and c.g() == 0;
  }
  static_assert(magenta(magenta3));
  static_assert(not magenta(blue3));
  inline constexpr bool cyan(const Colour3 c) noexcept {
    return c.r() == 0 and c.g() == c.b();
  }
  static_assert(cyan(cyan3));
  static_assert(not cyan(green3));

}

#endif
