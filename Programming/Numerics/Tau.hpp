// Oliver Kullmann, 7.11.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing the tau-function and variations

   - tau_meaneqLW
   - wtau_ge1(x)
   - wtau_le1(x)
   - WithCounting
   - wtau_ge1_c(x)
   - wtau_le1_c(x)

*/

#include <cassert>

#include "FloatingPoint.hpp"

#ifndef TAU_RKly55jp2p
#define TAU_RKly55jp2p

namespace Tau {

  namespace FP = FloatingPoint;

  // Where the arithmetic-mean-lower-bound equals the Lambert-W-lower-bound:
  constexpr FP::float80 tau_meaneqLW = 2.8811206627473383049862597L;

  inline constexpr FP::float80 wtau_ge1(const FP::float80 a) noexcept {
    assert(a >= 1);
    if (FP::isinf(a)) return FP::pinfinity;
    if (a == 1) return FP::Log2;
    const FP::float80 ra = 1 /a;
    FP::float80 x0 =
      a <= tau_meaneqLW ? FP::log(4) / (1+ra) : FP::lambertW0_lb(a);
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
  static_assert(wtau_ge1(FP::pinfinity) == FP::pinfinity);
  static_assert(wtau_ge1(FP::max_value) < FP::max_value);
  static_assert(wtau_ge1(1) == FP::Log2);
  static_assert(FP::abs(wtau_ge1(2) - 2 * FP::log_golden_ratio) < FP::epsilon);
  static_assert(FP::abs(wtau_ge1(3) - 1.14673525752010692398807549755L) < 2*FP::epsilon);

  inline constexpr FP::float80 wtau_ge1_ub(const FP::float80 a) noexcept {
    assert(a >= 1);
    if (FP::isinf(a)) return FP::pinfinity;
    if (a == 1) return FP::Log2;
    const FP::float80 ra = 1 /a;
    FP::float80 x0;
    if (a <= tau_meaneqLW) x0 = FP::log(4) / (1+ra); // XXX
    else {
      x0 = FP::log(a / FP::lambertW0_lb(a) + 1);
      const FP::float80 A = FP::expm1(-x0), B = FP::exp(-ra * x0), N = A+B;
      if (N >= 0) return x0;
      const FP::float80 D = 1 / (FP::fma(ra, B, A) + 1);
      assert(D > 0);
      const FP::float80 x1 = FP::fma(N, D, x0);
      assert(x1 <= x0);
      if (x1 == x0) return x0;
      x0 = x1;
    }
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
  static_assert(wtau_ge1_ub(FP::pinfinity) == FP::pinfinity);
  static_assert(wtau_ge1_ub(FP::max_value) < FP::max_value);
  static_assert(wtau_ge1_ub(1) == FP::Log2);
  static_assert(FP::abs(wtau_ge1_ub(2) - 2 * FP::log_golden_ratio) < FP::epsilon);
  static_assert(FP::abs(wtau_ge1_ub(3) - 1.14673525752010692398807549755L) < FP::epsilon);


  /* Counting completed Newton-iterations: */

  struct WithCounting {
    FP::float80 r;
    FP::UInt_t c;
    bool operator ==(const WithCounting& rhs) const noexcept = default;
  };
  static_assert(WithCounting{} == WithCounting{});
  static_assert(WithCounting{1} != WithCounting{});

  inline constexpr WithCounting wtau_ge1_c(const FP::float80 a) noexcept {
    assert(a >= 1);
    if (FP::isinf(a)) return {FP::pinfinity, 0};
    if (a == 1) return {FP::Log2, 0};
    const FP::float80 ra = 1 /a;
    FP::float80 x0 =
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
  static_assert(wtau_ge1_c(FP::pinfinity) == WithCounting{FP::pinfinity, 0});
  static_assert(wtau_ge1_c(FP::max_value) == WithCounting{wtau_ge1(FP::max_value), 0});
  static_assert(wtau_ge1_c(1) == WithCounting{wtau_ge1(1), 0});
  static_assert(wtau_ge1_c(2) == WithCounting{wtau_ge1(2), 4});
  static_assert(wtau_ge1_c(3) == WithCounting{wtau_ge1(3), 4});

  inline constexpr WithCounting wtau_ge1_ub_c(const FP::float80 a) noexcept {
    assert(a >= 1);
    if (FP::isinf(a)) return {FP::pinfinity, 0};
    if (a == 1) return {FP::Log2, 0};
    const FP::float80 ra = 1 /a;
    FP::float80 x0;
    FP::UInt_t count;
    if (a <= tau_meaneqLW) {
      x0 = FP::log(4) / (1+ra); // XXX
      count = 0;
    }
    else {
      x0 = FP::log1p(a / FP::lambertW0_lb(a));
      const FP::float80 A = FP::expm1(-x0), B = FP::exp(-ra * x0), N = A+B;
      if (N >= 0) return {x0, 0};
      const FP::float80 D = 1 / (FP::fma(ra, B, A) + 1);
      assert(D > 0);
      const FP::float80 x1 = FP::fma(N, D, x0);
      assert(x1 <= x0);
      if (x1 == x0) return {x0, 0};
      x0 = x1;
      count = 1;
    }
    for (; true; ++count) {
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
  static_assert(wtau_ge1_ub_c(FP::pinfinity) == WithCounting{FP::pinfinity, 0});
  static_assert(wtau_ge1_ub_c(FP::max_value) == WithCounting{wtau_ge1_ub(FP::max_value), 0});
  static_assert(wtau_ge1_ub_c(1) == WithCounting{wtau_ge1_ub(1), 0});
  static_assert(wtau_ge1_ub_c(2) == WithCounting{wtau_ge1_ub(2), 4});
  static_assert(wtau_ge1_ub_c(3) == WithCounting{wtau_ge1_ub(3), 5});

}
#endif
