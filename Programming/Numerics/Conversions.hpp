// Oliver Kullmann, 19.12.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Conversions between numeric types

    - isUInt(float80 x) tests whether x is integral within the range of UInt_t
    - toUInt(float80 x) converts (every) x >= 0 to UIint_t
    - touint(x) same for uint_t

*/

#ifndef CONVERSIONS_7SoZrKQ2nw
#define CONVERSIONS_7SoZrKQ2nw

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
    return std::floor(x); // ERROR with gcc 10.2: std::floorl not available
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
    return std::ceil(x); // ERROR with gcc 10.2: std::ceill not available
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

}

#endif
