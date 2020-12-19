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
   - tau_gmeaneqLW
   - wtau_ge1_ub
   - lowerupper_0/1, elowerupper_0/1
   - lower_better_upper
   - wtau

   - WithCounting
   - wtau_ge1_c(x)
   - wtau_ge1_ub_c
   - wtau_c

TODOS:

1. Clean-up
    - At least names should be updated.
    - Do we need the various versions, or do we only keep wtau(x) ?
    - And what about the counting-forms?

2. Provide double-versions.
    - Duplication, or templates?
    - And what about the different forms?
    - Likely best to at least look at the graph for the number of iterations,
      in comparison with float80.

3. Provide versions via MPFR, with arbitrary precision.

4. Check accurracy of computations.

5. Provide the other functions (see BranchingTuples.hpp)

*/

#ifndef TAU_RKly55jp2p
#define TAU_RKly55jp2p

#include <numbers>

#include <cassert>

// Guaranteed to be included:
#include "FloatingPoint.hpp"

namespace Tau {

  namespace FP = FloatingPoint;

  // Where the arithmetic-mean-lower-bound equals the Lambert-W-lower-bound:
  constexpr FP::float80 tau_meaneqLW = 2.8811206627473383049862597L;

  inline CONSTEXPR FP::float80 wtau_ge1(const FP::float80 a) noexcept {
    assert(a >= 1);
    if (FP::isinf(a)) return FP::pinfinity;
    if (a == 1) return FP::Log2;
    const FP::float80 ra = 1 /a;
    FP::float80 x0 =
      a <= tau_meaneqLW ? FP::log(4) / (1+ra) : FP::lambertW0_lb(a);
    while (true) {
      const FP::float80 A = FP::exp(-x0), B = FP::expm1(-ra * x0), N = A+B;
      if (N <= 0) return x0;
      const FP::float80 D = 1 / (FP::fma(ra, B, ra) + A);
      assert(D > 0);
      const FP::float80 x1 = FP::fma(N, D, x0);
      assert(x1 >= x0);
      if (x1 == x0) return x0;
      x0 = x1;
    }
  }
  STATIC_ASSERT(wtau_ge1(FP::pinfinity) == FP::pinfinity);
  STATIC_ASSERT(wtau_ge1(1) == FP::Log2);
  STATIC_ASSERT(wtau_ge1(2) == 2 * FP::log_golden_ratio);
  STATIC_ASSERT(wtau_ge1(3) == 1.14673525752010692398807549755L);


  // Where the geometric-mean-lower-bound equals the Lambert-W-upper-bound:
  constexpr FP::float80 tau_gmeaneqLW = 5.29941064547678261500450985L;
  STATIC_ASSERT(FP::log(2) * FP::sqrt(tau_gmeaneqLW) == FP::log(tau_gmeaneqLW / FP::lambertW0_lb(tau_gmeaneqLW) + 1));

  inline CONSTEXPR FP::float80 wtau_ge1_ub(const FP::float80 a) noexcept {
    assert(a >= 1);
    if (FP::isinf(a)) return FP::pinfinity;
    if (a == 1) return FP::Log2;
    const FP::float80 ra = 1 /a;
    FP::float80 x0 =
      a <= tau_gmeaneqLW ? FP::log(2) * FP::sqrt(a) :
                           FP::log(a / FP::lambertW0_lb(a) + 1);
    {const FP::float80 A = FP::exp(-x0), B = FP::expm1(-ra * x0), N = A+B;
     if (N >= 0) return x0;
     const FP::float80 D = 1 / (FP::fma(ra, B, ra) + A);
     assert(D > 0);
     const FP::float80 x1 = FP::fma(N, D, x0);
     assert(x1 <= x0);
     if (x1 == x0) return x0;
     x0 = x1;
    }
    while (true) {
      const FP::float80 A = FP::exp(-x0), B = FP::expm1(-ra * x0), N = A+B;
      if (N <= 0) return x0;
      const FP::float80 D = 1 / (FP::fma(ra, B, ra) + A);
      assert(D > 0);
      const FP::float80 x1 = FP::fma(N, D, x0);
      assert(x1 >= x0);
      if (x1 == x0) return x0;
      x0 = x1;
    }
  }
  STATIC_ASSERT(wtau_ge1_ub(FP::pinfinity) == FP::pinfinity);
  STATIC_ASSERT(wtau_ge1_ub(1) == FP::Log2);
  STATIC_ASSERT(wtau_ge1_ub(2) == 2 * FP::log_golden_ratio);


  // The two x-values where the elementary lower-bound + 1 Newton-iteration
  // equals the elementary upper-bound + 1 Newton-iteration:
  constexpr FP::float80 lowerupper_0 = 3.88657252378295887884606176871L;
  constexpr FP::float80 lowerupper_1 = 28.84297132043715060543685463152L;
  // Thus from lowerupper_0 to lowerupper_1 the lower bound is better.
  // Looking at the plots suggests to extend this range:
  constexpr FP::float80 elowerupper_0 = 3.5;
  constexpr FP::float80 elowerupper_1 = 55;
  constexpr bool lower_better_upper(const FP::float80 a) noexcept {
    return a >= elowerupper_0 and a <= elowerupper_1;
  }

  inline CONSTEXPR FP::float80 wtau(const FP::float80 a) noexcept {
    assert(a >= 1);
    if (lower_better_upper(a)) return wtau_ge1(a);
    else return wtau_ge1_ub(a);
  }


  /* Counting completed Newton-iterations: */

  struct WithCounting {
    FP::float80 r;
    FP::UInt_t c;
    bool operator ==(const WithCounting& rhs) const noexcept = default;
  };
  static_assert(WithCounting{} == WithCounting{});
  static_assert(WithCounting{1} != WithCounting{});

  inline CONSTEXPR WithCounting wtau_ge1_c(const FP::float80 a) noexcept {
    assert(a >= 1);
    if (FP::isinf(a)) return {FP::pinfinity, 0};
    if (a == 1) return {FP::Log2, 0};
    const FP::float80 ra = 1 /a;
    FP::float80 x0 =
      a <= tau_meaneqLW ? FP::log(4) / (1 + ra) : FP::lambertW0_lb(a);
    for (FP::UInt_t count = 0; true; ++count) {
      const FP::float80 A = FP::exp(-x0), B = FP::expm1(-ra * x0), N = A+B;
      if (N <= 0) return {x0, count};
      const FP::float80 D = 1 / (FP::fma(ra, B, ra) + A);
      assert(D > 0);
      const FP::float80 x1 = FP::fma(N, D, x0);
      assert(x1 >= x0);
      if (x1 == x0) return {x0, count};
      x0 = x1;
    }
  }
  STATIC_ASSERT((wtau_ge1_c(FP::pinfinity) == WithCounting{FP::pinfinity, 0}));
  STATIC_ASSERT((wtau_ge1_c(FP::max_value) == WithCounting{wtau_ge1(FP::max_value), 2}));
  STATIC_ASSERT((wtau_ge1_c(1) == WithCounting{wtau_ge1(1), 0}));
  STATIC_ASSERT((wtau_ge1_c(2) == WithCounting{wtau_ge1(2), 4}));
  STATIC_ASSERT((wtau_ge1_c(3) == WithCounting{wtau_ge1(3), 4}));

  inline CONSTEXPR WithCounting wtau_ge1_ub_c(const FP::float80 a) noexcept {
    assert(a >= 1);
    if (FP::isinf(a)) return {FP::pinfinity, 0};
    if (a == 1) return {FP::Log2, 0};
    const FP::float80 ra = 1 /a;
    FP::float80 x0 =
      a <= tau_gmeaneqLW ? FP::log(2) * FP::sqrt(a) :
                           FP::log(a / FP::lambertW0_lb(a) + 1);
    {const FP::float80 A = FP::exp(-x0), B = FP::expm1(-ra * x0), N = A+B;
     if (N >= 0) return {x0, 0};
     const FP::float80 D = 1 / (FP::fma(ra, B, ra) + A);
     assert(D > 0);
     const FP::float80 x1 = FP::fma(N, D, x0);
     assert(x1 <= x0);
     if (x1 == x0) return {x0, 0};
     x0 = x1;
    }
    for (FP::UInt_t count = 1; true; ++count) {
      const FP::float80 A = FP::exp(-x0), B = FP::expm1(-ra * x0), N = A+B;
      if (N <= 0) return {x0, count};
      const FP::float80 D = 1 / (FP::fma(ra, B, ra) + A);
      assert(D > 0);
      const FP::float80 x1 = FP::fma(N, D, x0);
      assert(x1 >= x0);
      if (x1 == x0) return {x0, count};
      x0 = x1;
    }
  }
  STATIC_ASSERT((wtau_ge1_ub_c(FP::pinfinity) == WithCounting{FP::pinfinity, 0}));
  STATIC_ASSERT((wtau_ge1_ub_c(FP::max_value) == WithCounting{wtau_ge1_ub(FP::max_value), 1}));
  STATIC_ASSERT((wtau_ge1_ub_c(1) == WithCounting{wtau_ge1_ub(1), 0}));
  STATIC_ASSERT((wtau_ge1_ub_c(2) == WithCounting{wtau_ge1_ub(2), 4}));
  STATIC_ASSERT((wtau_ge1_ub_c(3) == WithCounting{wtau_ge1_ub(3), 4}));

  inline CONSTEXPR WithCounting wtau_c(const FP::float80 a) noexcept {
    assert(a >= 1);
    if (lower_better_upper(a)) return wtau_ge1_c(a);
    else return wtau_ge1_ub_c(a);
  }



  /* Duplication of functionality for double */

  constexpr FP::float64 tau_meaneqLW_64 = tau_meaneqLW;

  inline CONSTEXPR FP::float64 wtau_ge1_64(const FP::float64 a) noexcept {
    assert(a >= 1);
    if (std::isinf(a)) return FP::pinfinity64;
    if (a == 1) return std::numbers::log2e;
    const FP::float64 ra = 1 /a;
    FP::float64 x0 =
      a <= tau_meaneqLW_64 ? std::log(4) / (1+ra) : FP::lambertW0_lb_64(a);
    while (true) {
      const FP::float64 A = std::exp(-x0), B = std::expm1(-ra * x0), N = A+B;
      if (N <= 0) return x0;
      const FP::float64 D = 1 / (std::fma(ra, B, ra) + A);
      assert(D > 0);
      const FP::float64 x1 = std::fma(N, D, x0);
      assert(x1 >= x0);
      if (x1 == x0) return x0;
      x0 = x1;
    }
  }
  STATIC_ASSERT(wtau_ge1_64(FP::pinfinity64) == FP::pinfinity64);
  STATIC_ASSERT(wtau_ge1_64(1) == std::numbers::log2e);
  STATIC_ASSERT(wtau_ge1_64(2) == 2 * std::log(std::numbers::phi));
  STATIC_ASSERT(wtau_ge1_64(3) == 1.14673525752010692398807549755);


  constexpr FP::float64 tau_gmeaneqLW_64 = tau_gmeaneqLW;

  inline CONSTEXPR FP::float64 wtau_ge1_ub_64(const FP::float64 a) noexcept {
    assert(a >= 1);
    if (std::isinf(a)) return FP::pinfinity64;
    if (a == 1) return std::numbers::log2e;
    const FP::float64 ra = 1 /a;
    FP::float64 x0 =
      a <= tau_gmeaneqLW_64 ? std::log(2) * std::sqrt(a) :
                              std::log(a / FP::lambertW0_lb_64(a) + 1);
    {const FP::float64 A = std::exp(-x0), B = std::expm1(-ra * x0), N = A+B;
     if (N >= 0) return x0;
     const FP::float64 D = 1 / (std::fma(ra, B, ra) + A);
     assert(D > 0);
     const FP::float64 x1 = std::fma(N, D, x0);
     assert(x1 <= x0);
     if (x1 == x0) return x0;
     x0 = x1;
    }
    while (true) {
      const FP::float64 A = std::exp(-x0), B = std::expm1(-ra * x0), N = A+B;
      if (N <= 0) return x0;
      const FP::float64 D = 1 / (std::fma(ra, B, ra) + A);
      assert(D > 0);
      const FP::float64 x1 = std::fma(N, D, x0);
      assert(x1 >= x0);
      if (x1 == x0) return x0;
      x0 = x1;
    }
  }
  STATIC_ASSERT(wtau_ge1_ub_64(FP::pinfinity64) == FP::pinfinity64);
  STATIC_ASSERT(wtau_ge1_ub_64(1) == std::numbers::log2e);
  STATIC_ASSERT(wtau_ge1_ub_64(2) == 2 *std::log(std::numbers::phi));


  constexpr bool lower_better_upper_64(const FP::float64 a) noexcept {
    return a >= elowerupper_0 and a <= elowerupper_1;
  }

  inline CONSTEXPR FP::float64 wtau_64(const FP::float64 a) noexcept {
    assert(a >= 1);
    if (lower_better_upper_64(a)) return wtau_ge1_64(a);
    else return wtau_ge1_ub_64(a);
  }

  struct WithCounting64 {
    FP::float64 r;
    FP::UInt_t c;
    bool operator ==(const WithCounting64& rhs) const noexcept = default;
  };
  static_assert(WithCounting64{} == WithCounting64{});
  static_assert(WithCounting64{1} != WithCounting64{});

  inline CONSTEXPR WithCounting64 wtau_ge1_c_64(const FP::float64 a) noexcept {
    assert(a >= 1);
    if (std::isinf(a)) return {FP::pinfinity64, 0};
    if (a == 1) return {std::numbers::log2e, 0};
    const FP::float64 ra = 1 /a;
    FP::float64 x0 =
      a <= tau_meaneqLW_64 ? std::log(4) / (1 + ra) : FP::lambertW0_lb_64(a);
    for (FP::UInt_t count = 0; true; ++count) {
      const FP::float64 A = std::exp(-x0), B = std::expm1(-ra * x0), N = A+B;
      if (N <= 0) return {x0, count};
      const FP::float64 D = 1 / (std::fma(ra, B, ra) + A);
      assert(D > 0);
      const FP::float64 x1 = std::fma(N, D, x0);
      assert(x1 >= x0);
      if (x1 == x0) return {x0, count};
      x0 = x1;
    }
  }
  STATIC_ASSERT((wtau_ge1_c_64(FP::pinfinity64) == WithCounting64{FP::pinfinity64, 0}));
  STATIC_ASSERT((wtau_ge1_c_64(FP::max_value64) == WithCounting64{wtau_ge1_64(FP::max_value64), 2}));
  STATIC_ASSERT((wtau_ge1_c_64(1) == WithCounting64{wtau_ge1_64(1), 0}));
  STATIC_ASSERT((wtau_ge1_c_64(2) == WithCounting64{wtau_ge1_64(2), 4}));
  STATIC_ASSERT((wtau_ge1_c_64(3) == WithCounting64{wtau_ge1_64(3), 4}));

  inline CONSTEXPR WithCounting64 wtau_ge1_ub_c_64(const FP::float64 a) noexcept {
    assert(a >= 1);
    if (std::isinf(a)) return {FP::pinfinity64, 0};
    if (a == 1) return {std::numbers::log2e, 0};
    const FP::float64 ra = 1 /a;
    FP::float64 x0 =
      a <= tau_gmeaneqLW_64 ? std::log(2) * std::sqrt(a) :
                              std::log(a / FP::lambertW0_lb_64(a) + 1);
    {const FP::float64 A = std::exp(-x0), B = std::expm1(-ra * x0), N = A+B;
     if (N >= 0) return {x0, 0};
     const FP::float64 D = 1 / (std::fma(ra, B, ra) + A);
     assert(D > 0);
     const FP::float64 x1 = std::fma(N, D, x0);
     assert(x1 <= x0);
     if (x1 == x0) return {x0, 0};
     x0 = x1;
    }
    for (FP::UInt_t count = 1; true; ++count) {
      const FP::float64 A = std::exp(-x0), B = std::expm1(-ra * x0), N = A+B;
      if (N <= 0) return {x0, count};
      const FP::float64 D = 1 / (std::fma(ra, B, ra) + A);
      assert(D > 0);
      const FP::float64 x1 = std::fma(N, D, x0);
      assert(x1 >= x0);
      if (x1 == x0) return {x0, count};
      x0 = x1;
    }
  }
  STATIC_ASSERT((wtau_ge1_ub_c_64(FP::pinfinity64) == WithCounting64{FP::pinfinity64, 0}));
  STATIC_ASSERT((wtau_ge1_ub_c_64(FP::max_value64) == WithCounting64{wtau_ge1_ub_64(FP::max_value64), 1}));
  STATIC_ASSERT((wtau_ge1_ub_c_64(1) == WithCounting64{wtau_ge1_ub_64(1), 0}));
  STATIC_ASSERT((wtau_ge1_ub_c_64(2) == WithCounting64{wtau_ge1_ub_64(2), 3}));
  STATIC_ASSERT((wtau_ge1_ub_c_64(3) == WithCounting64{wtau_ge1_ub_64(3), 4}));

  inline CONSTEXPR WithCounting64 wtau_c_64(const FP::float64 a) noexcept {
    assert(a >= 1);
    if (lower_better_upper_64(a)) return wtau_ge1_c_64(a);
    else return wtau_ge1_ub_c_64(a);
  }

}
#endif
