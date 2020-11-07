// Oliver Kullmann, 7.11.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing the tau-function and variations
*/

#include <cassert>

#include "FloatingPoint.hpp"

#ifndef TAU_RKly55jp2p
#define TAU_RKly55jp2p

namespace Tau {

  namespace FP = FloatingPoint;

  // Where the mean-lower-bound equals the Lambert-W-lower-bound:
  constexpr FP::float80 tau_meaneqLW = 2.8811206627473383049862597L;

  inline constexpr FP::float80 wtau_ge1(const FP::float80 a) noexcept {
    assert(a >= 1);
    if (FP::isinf(a)) return FP::pinfinity;
    if (a == 1) return FP::Log2;
    FP::float80 ra = 1 /a, x0 =
      a <= tau_meaneqLW ? FP::log(4) / (1 + ra) : FP::lambertW0_lb(a);
    while (true) {
      const FP::float80 A = FP::expm1(-x0), B = FP::exp(-ra * x0), N = A+B;
      if (N <= 0) return x0;
      const FP::float80 D = 1 / (FP::fma(ra, B, A) + 1);
      assert(D > 0);
      const FP::float80 x1 = FP::fma(N, D, x0);
      assert(x1 >= x0);
      if (x1 == x0) return x0;
      x0 = x1;
    }
  }
  static_assert(wtau_ge1(1) == FP::Log2);
  static_assert(FP::abs(wtau_ge1(2) - 2 * FP::log_golden_ratio) < FP::epsilon);
  static_assert(FP::abs(wtau_ge1(3) - 1.14673525752010692398807549755L) < 2*FP::epsilon);
  static_assert(wtau_ge1(FP::max_value) < FP::max_value);
  static_assert(wtau_ge1(FP::pinfinity) == FP::pinfinity);
  

  inline constexpr FP::float80 wtau_le1(const FP::float80 a) noexcept {
    assert(a <= 1);
    if (a == 0) return 0;
    if (a == 1) return FP::Log2;
    const FP::float80 ra = 1 / a;
    FP::float80 x0 = FP::log(4) / (1 + ra);
    while (true) {
      const FP::float80 A = FP::expm1(-x0), B = FP::exp(-ra * x0), N = A+B;
      if (N <= 0) return x0;
      const FP::float80 D = 1 / (FP::fma(ra, B, A) + 1);
      assert(D > 0);
      const FP::float80 x1 = FP::fma(N, D, x0);
      assert(x1 >= x0);
      if (x1 == x0) return x0;
      x0 = x1;
    }
  }
  static_assert(wtau_le1(0) == 0);
  static_assert(wtau_le1(FP::min_value) > 0);
  static_assert(FP::abs(wtau_le1(1 / 3.0L) - 0.38224508584003564132935849918L) < FP::epsilon);
  static_assert(wtau_le1(0.5L) == FP::log_golden_ratio);
  static_assert(wtau_le1(1) == FP::Log2);
  /* It holds
     wtau_le1(x) = x * wtau_ge1(1/x) and wtau_ge1(x) = x * wtau_le1(1/x).
  */
  static_assert(wtau_le1(0.1L) == 0.1L * wtau_ge1(10));
  static_assert(FP::abs(wtau_ge1(10L) - 10L * wtau_le1(0.1L)) < 2*FP::epsilon);


  /* Counting completed Newton-iterations: */

  struct WithCounting {
    FP::float80 r;
    FP::UInt_t c;
  };
  inline constexpr bool operator ==(const WithCounting a, const WithCounting b) noexcept {
    return a.r == b.r and a.c == b.c;
  }

  inline constexpr WithCounting wtau_ge1_c(const FP::float80 a) noexcept {
    assert(a >= 1);
    if (FP::isinf(a)) return {FP::pinfinity, 0};
    if (a == 1) return {FP::Log2, 0};
    FP::float80 ra = 1 /a, x0 =
      a <= tau_meaneqLW ? FP::log(4) / (1 + ra) : FP::lambertW0_lb(a);
    for (FP::UInt_t count = 0; true; ++count) {
      const FP::float80 A = FP::expm1(-x0), B = FP::exp(-ra * x0), N = A+B;
      if (N <= 0) return {x0, count};
      const FP::float80 D = 1 / (FP::fma(ra, B, A) + 1);
      assert(D > 0);
      const FP::float80 x1 = FP::fma(N, D, x0);
      assert(x1 >= x0);
      if (x1 == x0) return {x0, count};
      x0 = x1;
    }
  }
  static_assert(wtau_ge1_c(1) == WithCounting{wtau_ge1(1), 0});
  static_assert(wtau_ge1_c(2) == WithCounting{wtau_ge1(2), 4});
  static_assert(wtau_ge1_c(3) == WithCounting{wtau_ge1(3), 4});
  static_assert(wtau_ge1_c(FP::max_value) == WithCounting{wtau_ge1(FP::max_value), 0});
  static_assert(wtau_ge1_c(FP::pinfinity) == WithCounting{FP::pinfinity, 0});

  inline constexpr WithCounting wtau_le1_c(const FP::float80 a) noexcept {
    assert(a <= 1);
    if (a == 0) return {0, 0};
    if (a == 1) return {FP::Log2, 0};
    const FP::float80 ra = 1 / a;
    FP::float80 x0 = FP::log(4) / (1 + ra);
    for (FP::UInt_t count = 0; true; ++count) {
      const FP::float80 A = FP::expm1(-x0), B = FP::exp(-ra * x0), N = A+B;
      if (N <= 0) return {x0, count};
      const FP::float80 D = 1 / (FP::fma(ra, B, A) + 1);
      assert(D > 0);
      const FP::float80 x1 = FP::fma(N, D, x0);
      assert(x1 >= x0);
      if (x1 == x0) return {x0, count};
      x0 = x1;
    }
  }
  static_assert(wtau_le1_c(0) == WithCounting{0,0});
  static_assert(wtau_le1_c(FP::min_value) == WithCounting{wtau_le1(FP::min_value), 11350});
  static_assert(wtau_le1_c(1 / 3.0L) == WithCounting{wtau_le1(1 / 3.0L), 5});
  static_assert(wtau_le1_c(0.5L) == WithCounting{wtau_le1(0.5L), 4});
  static_assert(wtau_le1_c(1) == WithCounting{FP::Log2, 0});


}
#endif
