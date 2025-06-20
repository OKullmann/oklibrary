// Oliver Kullmann, 19.12.2021 (Swansea)
/* Copyright 2021, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Conversions between numeric types

    - round, floor, trunc, ceil with float80-arguments (just wrapping
      the functions from the standard library)
    - additionally antitrunc

    - toUInt(float80 x) converts (every) x >= 0 to UIint_t
    - touint(x) same for uint_t

    - struct RepFloat80 represents float80 via integral mantissa, int exponent,
      and booleans for negative, nan, and inf.


TODOS:

1. How precise is RepFloat80 ?
  - Does for *every* float80 x hold: fulleq(x, float80(RepFloat80(x))) ?
    This is currently asserted in the constructor.
  - Then also fulleq(x, y) <=> RepFloat80(x) == RepFloat80(y)
    should hold.

*/

#ifndef CONVERSIONS_7SoZrKQ2nw
#define CONVERSIONS_7SoZrKQ2nw

#include <ostream>

#include "NumTypes.hpp"

namespace FloatingPoint {

  inline CONSTEXPR float80 round(const float80 x) noexcept {
    return std::roundl(x);
  }
  STATIC_ASSERT(round(0.4) == 0);
  STATIC_ASSERT(round(0.5) == 1);
  STATIC_ASSERT(round(0.6) == 1);
  STATIC_ASSERT(round(1.5) == 2);
  STATIC_ASSERT(round(2.5) == 3);
  STATIC_ASSERT(round(-0.5) == -1);
  STATIC_ASSERT(round(-1.5) == -2);
  STATIC_ASSERT(round(P264m1) == P264m1);

  inline CONSTEXPR float80 floor(const float80 x) noexcept {
    return std::floorl(x);
  }
  STATIC_ASSERT(floor(0.0) == 0);
  STATIC_ASSERT(floor(0.1) == 0);
  STATIC_ASSERT(floor(0.9) == 0);
  STATIC_ASSERT(floor(1) == 1);
  STATIC_ASSERT(floor(-0.1) == -1);
  STATIC_ASSERT(floor(-1) == -1);

  inline CONSTEXPR float80 trunc(const float80 x) noexcept {
    return std::truncl(x);
  }
  STATIC_ASSERT(trunc(0.0) == 0);
  STATIC_ASSERT(trunc(0.1) == 0);
  STATIC_ASSERT(trunc(0.9) == 0);
  STATIC_ASSERT(trunc(1) == 1);
  STATIC_ASSERT(trunc(-0.1) == 0);
  STATIC_ASSERT(trunc(-0.9) == 0);
  STATIC_ASSERT(trunc(-1) == -1);

  inline CONSTEXPR float80 ceil(const float80 x) noexcept {
    return std::ceill(x);
  }
  STATIC_ASSERT(ceil(0.0) == 0);
  STATIC_ASSERT(ceil(0.1) == 1);
  STATIC_ASSERT(ceil(0.9) == 1);
  STATIC_ASSERT(ceil(1) == 1);
  STATIC_ASSERT(ceil(-0.1) == 0);
  STATIC_ASSERT(ceil(-0.9) == 0);
  STATIC_ASSERT(ceil(-1) == -1);

  // For completeness (seems missing in standard library):
  inline CONSTEXPR float80 antitrunc(const float80 x) noexcept {
    return (x >= 0) ? ceil(x) : floor(x);
  }
  STATIC_ASSERT(antitrunc(0.0) == 0);
  STATIC_ASSERT(antitrunc(0.1) == 1);
  STATIC_ASSERT(antitrunc(0.9) == 1);
  STATIC_ASSERT(antitrunc(1) == 1);
  STATIC_ASSERT(antitrunc(-0.1) == -1);
  STATIC_ASSERT(antitrunc(-0.9) == -1);
  STATIC_ASSERT(antitrunc(-1) == -1);


  /* Converting float80 to UInt_t for x >= 0, using rounding, except
     x is too big, in which case P264m1 is returned:
  */
  inline CONSTEXPR UInt_t toUInt(const float80 x) noexcept {
    assert(x >= 0);
    if (x >= P264m1) return P264m1;
    else return round(x);
  }
  STATIC_ASSERT(toUInt(0) == 0);
  STATIC_ASSERT(toUInt(0.5) == 1);
  STATIC_ASSERT(toUInt(exp2(32)) == UInt_t(P232m1) + 1);
  STATIC_ASSERT(toUInt(P264) == P264m1);
  STATIC_ASSERT(toUInt((P264m1-1) + 0.5000000000000000001L) == P264m1);
  STATIC_ASSERT(toUInt(pinfinity) == P264m1);
  STATIC_ASSERT(toUInt(1e100) == P264m1);
  STATIC_ASSERT(toUInt(1e+28) == P264m1);

  inline CONSTEXPR uint_t touint(const float80 x) noexcept {
    assert(x >= 0);
    if (x == pinfinity) return P232m1;
    else return round(min(x, P232m1));
  }
  STATIC_ASSERT(touint(0) == 0);
  STATIC_ASSERT(touint(0.5) == 1);
  STATIC_ASSERT(touint(exp2(16)) == 65536);
  STATIC_ASSERT(touint(P232) == P232m1);
  STATIC_ASSERT(touint((P232m1-1) + 0.5000000000000000001L) == P232m1);
  STATIC_ASSERT(touint(pinfinity) == P232m1);


  struct ApproxBin {
    const int exp;
    const float80 base, low, mid, up; // base * 2^-exp is the original number
    explicit ApproxBin(const float80 x, const int exp)
      : exp(exp), base(std::ldexp(x, exp)),
        low(std::ldexp(std::floor(base), -exp)),
        mid(std::ldexp(std::roundl(base), -exp)),
        up(std::ldexp(std::ceil(base), -exp)) {}
    bool operator ==(const ApproxBin&) const noexcept = default;
  };


  struct RepFloat80 {
    UInt_t m = 0; // mantissa
    int be = 0; // binary exponent
    bool neg = false, nan = false, inf=false;

    constexpr RepFloat80() noexcept = default;
    constexpr RepFloat80(const float80 x) noexcept :
    neg(std::signbit(x)), nan(isnan(x)), inf(isinf(x)) {
      if (not nan and not inf and x != 0) {
        m = std::ldexpl(std::frexpl(std::fabsl(x), &be), 64);
        be -= 64;
      }
      assert(fulleq(x, operator float80()));
      assert(valid());
    }

    // Might create invalid objects:
    constexpr RepFloat80(const UInt_t m, const int be, const bool neg,
                         const bool nan, const bool inf) noexcept :
    m(m), be(be), neg(neg), nan(nan), inf(inf) {}

    // The range of be is unchecked (except for be=0 if m=0; so a valid object
    // could use a larger range for be):
    constexpr bool valid() const noexcept {
      if (nan and inf) return false;
      if (nan and (m != 0 or be != 0)) return false;
      if (inf and (m != 0 or be != 0)) return false;
      if (m == 0 and be != 0) return false;
      if (m != 0 and m < P263) return false;
      return true;
    }

    constexpr int negf() const noexcept { return neg ? -1 : +1; }
    explicit constexpr operator float80() const noexcept {
      if (nan) return neg ? -NaN : NaN;
      else if (inf) return neg ? minfinity : pinfinity;
      else if (m == 0) return neg ? -0.0L : 0;
      else return negf() * std::ldexp(float80(m), be);
    }

    constexpr bool operator ==(const RepFloat80&) const noexcept = default;
  };
  static_assert(RepFloat80().valid());
  static_assert(RepFloat80() == RepFloat80{0,0,false,false,false});

  std::ostream& operator <<(std::ostream& out, const RepFloat80& r) {
    return out << "(" << r.m << "," << r.be << "," << r.neg << ","
               << r.nan << "," << r.inf << ")";
  }
}

#endif
