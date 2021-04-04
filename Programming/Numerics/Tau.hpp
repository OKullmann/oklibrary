// Oliver Kullmann, 7.11.2020 (Swansea)
/* Copyright 2020, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing the tau-function and variations

  The function ltau is the "main" function; tau should only be used as an
  interface (the computation is much less precise).

  For float80:

   - tau_meaneqLW
   - wtau_elem_lb
   - wtau_ge1(x)
   - tau_gmeaneqLW
   - wtau_ge1_ub
   - lowerupper_0/1, elowerupper_0/1
   - lower_better_upper
   - wtau(x)

   - ltau(a,b)
   - mtau(a,b), ktau(x)
   - tau(a,b)

   - probdist_t
   - is_probdist_basic
   - is_probdist_precise
   - is_lprobdist_basic
   - lptau(a,b)
   - ptau(a,b)

   - WithCounting
   - wtau_ge1_c(x)
   - wtau_ge1_ub_c
   - wtau_c

   - pmean(a,b,p), kpmean(x,p)


  For float64:

   - tau_meaneqLW_64
   - wtau_elem_lb_64
   - wtau_64(x)

   - ltau_64(a,b)
   - mtau_64(a,b)

TODOS:

1. Clean-up
    - At least names should be updated.
    - Do we need the various versions, or do we only keep wtau(x) ?
    - And what about the counting-forms?

2. Provide versions via MPFR, with arbitrary precision.

3. Check accurracy of computations.

4. Provide the other functions (see BranchingTuples.hpp)

5. Implement the standard form of comparison (see TawSolver/Tau.hpp).
    - Then develop the "step-wise" form as developed by Donald Knuth.

*/

#ifndef TAU_RKly55jp2p
#define TAU_RKly55jp2p

#include <numbers>
#include <utility>
#include <vector>
#include <numeric>

#include <cassert>

// Guaranteed to be included:
#include "FloatingPoint.hpp"

namespace Tau {

  namespace FP = FloatingPoint;

  // Where the arithmetic-mean-lower-bound equals the Lambert-W-lower-bound:
  constexpr FP::float80 tau_meaneqLW = 2.8811206627473383049862597L;

  inline CONSTEXPR FP::float80 wtau_elem_lb(const FP::float80 ra) noexcept {
    return FP::log(4) / (1+ra);
  }

  inline CONSTEXPR FP::float80 wtau_ge1(const FP::float80 a) noexcept {
    assert(a >= 1);
    if (FP::isinf(a)) return FP::pinfinity;
    if (a == 1) return FP::Log2;
    const FP::float80 ra = 1 /a;
    FP::float80 x0 =
      a <= tau_meaneqLW ? wtau_elem_lb(ra) : FP::lambertW0_lb(a);
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


  inline CONSTEXPR FP::float80 pmean(FP::float80 a, FP::float80 b, const FP::float80 p) noexcept {
    assert(not FP::isnan(p));
    assert(a >= 0 and b >= 0);
    if (a == b) return a;
    if (a > b) std::swap(a,b);
    if (p == FP::minfinity) return a;
    else if (p == FP::pinfinity) return b;
    else if (a == 0)
       if (p < 0) return 0;
       else if (p == 0)
         if (b != FP::pinfinity) return 0;
         else return FP::NaN;
       else if (p == 0.5L) return b / 4;
       else if (p == 1) return b / 2;
       else if (p == 2) return b / FP::Sqr2;
       else if (p == 3) return b / FP::Cbr2;
       else if (p == 4) return b / FP::Qar2;
       else return b * FP::pow(0.5L, 1/p);
    else if (b == FP::pinfinity)
      if (p < 0) return a * FP::pow(0.5L, 1/p);
      else return FP::pinfinity;
    else if (p == -1) return 1 / FP::midpoint(1/a, 1/b);
    else if (p == 0) return FP::sqrt(a*b);
    else if (p == 1 / 3.0L) return FP::cb(FP::midpoint(FP::cbrt(a), FP::cbrt(b)));
    else if (p == 0.5L) return FP::sq(FP::midpoint(FP::sqrt(a), FP::sqrt(b)));
    else if (p == 1) return FP::midpoint(a,b);
    else if (p == 2) return FP::sqrt(FP::midpoint(FP::sq(a), FP::sq(b)));
    else if (p == 3) return FP::cbrt(FP::midpoint(FP::cb(a), FP::cb(b)));
    else if (p == 4) return FP::qart(FP::midpoint(FP::qa(a), FP::qa(b)));
    else return FP::pow(FP::midpoint(FP::pow(a,p), FP::pow(b,p)), 1/p);
  }

  inline CONSTEXPR FP::float80 kpmean(FP::float80 x, const FP::float80 p) noexcept {
    assert(x >= 1);
    return pmean(1,x,p);
  }
  STATIC_ASSERT(kpmean(FP::pinfinity, -1) == 2);
  STATIC_ASSERT(kpmean(FP::pinfinity, 0) == FP::pinfinity);
  STATIC_ASSERT(kpmean(FP::pinfinity, 1) == FP::pinfinity);
  STATIC_ASSERT(kpmean(1, -1) == 1);
  STATIC_ASSERT(kpmean(1, 0) == 1);
  STATIC_ASSERT(kpmean(1, 1) == 1);
  STATIC_ASSERT(kpmean(2, -1) == 4.0L/3);
  STATIC_ASSERT(kpmean(2, 0) == FP::Sqr2);
  STATIC_ASSERT(kpmean(2, 1) == 1.5);


  /* The versions for double */

  constexpr FP::float64 tau_meaneqLW_64 = tau_meaneqLW;
  inline CONSTEXPR FP::float64 wtau_elem_lb_64(const FP::float64 ra) noexcept {
    return std::log(4) / (1+ra);
  }
  inline CONSTEXPR FP::float64 wtau_64(const FP::float64 a) noexcept {
    assert(a >= 1);
    if (std::isinf(a)) return FP::pinfinity64;
    if (a == 1) return std::numbers::ln2;
    const FP::float64 ra = 1 /a;
    FP::float64 x0 =
      a <= tau_meaneqLW_64 ? wtau_elem_lb_64(ra) : FP::lambertW0_lb_64(a);
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
  STATIC_ASSERT(wtau_64(FP::pinfinity64) == FP::pinfinity64);
  STATIC_ASSERT(wtau_64(1) == std::numbers::ln2);
  STATIC_ASSERT(wtau_64(2) == 2 * std::log(std::numbers::phi));
  STATIC_ASSERT(wtau_64(3) == 1.14673525752010692398807549755);


  /* Binary ltau, tau, and probability-variations */

  inline CONSTEXPR FP::float80 ltau(FP::float80 a, FP::float80 b) noexcept {
    assert(a >= 0);
    assert(b >= 0);
    if (a > b) std::swap(a, b);
    if (a == 0)
      if (b == FP::pinfinity) return FP::NaN;
      else return FP::pinfinity;
    if (b == FP::pinfinity) return 0;
    if (a == b) return FP::Log2 / a;
    return wtau(b / a) / b;
  }
  STATIC_ASSERT(ltau(0,0) == FP::pinfinity);
  STATIC_ASSERT(ltau(FP::pinfinity,FP::pinfinity) == 0);
  STATIC_ASSERT(FP::isnan(ltau(0,FP::pinfinity)));
  STATIC_ASSERT(FP::isnan(ltau(FP::pinfinity,0)));
  STATIC_ASSERT(ltau(1,1) == FP::Log2);
  STATIC_ASSERT(ltau(1,0) == FP::pinfinity);
  STATIC_ASSERT(ltau(0,1) == FP::pinfinity);
  STATIC_ASSERT(ltau(1,FP::pinfinity) == 0);
  STATIC_ASSERT(ltau(FP::pinfinity,1) == 0);
  STATIC_ASSERT(ltau(2,2) == FP::Log2/2);
  STATIC_ASSERT(ltau(3,3) == FP::Log2/3);
  STATIC_ASSERT(ltau(1000,1000) == FP::Log2/1000);
  STATIC_ASSERT(ltau(1e+1000L, 1e+1000L) == FP::Log2 * 1e-1000L);
  STATIC_ASSERT(ltau(1e-1000L, 1e-1000L) == FP::Log2 * 1e1000L);
  STATIC_ASSERT(ltau(FP::max_value,FP::max_value) == FP::Log2/FP::max_value);
  STATIC_ASSERT(ltau(FP::min_value,FP::min_value) == FP::Log2/FP::min_value);
  STATIC_ASSERT(ltau(1,2) == FP::log_golden_ratio);
  STATIC_ASSERT(ltau(2,4) == FP::log_golden_ratio / 2);
  STATIC_ASSERT(ltau(3,6) == FP::log_golden_ratio / 3);
  STATIC_ASSERT(ltau(1e1000L,2e1000L) == FP::log_golden_ratio * 1e-1000L);
  STATIC_ASSERT(ltau(1e-1000L,2e-1000L) == FP::log_golden_ratio * 1e1000L);


  inline CONSTEXPR FP::float80 mtau(FP::float80 a, FP::float80 b) noexcept {
    if (a == b) return a;
    if (a > b) std::swap(a, b);
    assert(a >= 0);
    if (a == 0)
      if (b == FP::pinfinity) return FP::NaN;
      else return 0;
    if (b == FP::pinfinity) return FP::pinfinity;
    return FP::Log2 / ltau(a,b);
  }
  STATIC_ASSERT(FP::isnan(mtau(0,FP::pinfinity)));
  STATIC_ASSERT(FP::isnan(mtau(FP::pinfinity,0)));
  STATIC_ASSERT(mtau(FP::minfinity,FP::minfinity) == FP::minfinity);
  STATIC_ASSERT(mtau(-1,-1) == -1);
  STATIC_ASSERT(mtau(0,0) == 0);
  STATIC_ASSERT(mtau(2,2) == 2);
  STATIC_ASSERT(mtau(FP::pinfinity,FP::pinfinity) == FP::pinfinity);
  STATIC_ASSERT(mtau(0,2) == 0);
  STATIC_ASSERT(mtau(2,0) == 0);
  STATIC_ASSERT(mtau(FP::pinfinity,1) == FP::pinfinity);
  STATIC_ASSERT(mtau(1,2) == FP::Log2 / FP::log_golden_ratio);
  STATIC_ASSERT(mtau(2,4) == FP::log(4) / FP::log_golden_ratio);

  inline CONSTEXPR FP::float80 ktau(FP::float80 x) noexcept {
    assert(x >= 1);
    return mtau(1,x);
  }
  STATIC_ASSERT(ktau(1) == 1);
  STATIC_ASSERT(ktau(FP::pinfinity) == FP::pinfinity);
  STATIC_ASSERT(ktau(2) == FP::Log2 / FP::log_golden_ratio);


  inline CONSTEXPR FP::float80 tau(FP::float80 a, FP::float80 b) noexcept {
    assert(a >= 0);
    assert(b >= 0);
    if (a > b) std::swap(a, b);
    if (a == 0)
      if (b == FP::pinfinity) return FP::NaN;
      else return FP::pinfinity;
    if (b == FP::pinfinity) return 1;
    if (a == b) return FP::pow(2, 1/a);
    return FP::exp(ltau(a,b));
  }
  STATIC_ASSERT(tau(0,0) == FP::pinfinity);
  STATIC_ASSERT(tau(FP::pinfinity,FP::pinfinity) == 1);
  STATIC_ASSERT(FP::isnan(tau(0,FP::pinfinity)));
  STATIC_ASSERT(FP::isnan(tau(FP::pinfinity,0)));
  STATIC_ASSERT(tau(1,1) == 2);
  STATIC_ASSERT(tau(1,0) == FP::pinfinity);
  STATIC_ASSERT(tau(0,1) == FP::pinfinity);
  STATIC_ASSERT(tau(1,FP::pinfinity) == 1);
  STATIC_ASSERT(tau(FP::pinfinity,1) == 1);
  STATIC_ASSERT(tau(2,2) == FP::sqrt(2));
  STATIC_ASSERT(tau(3,3) == FP::cbrt(2));
  STATIC_ASSERT(tau(1000,1000) == FP::pow(2,FP::float80(1)/1000));
  STATIC_ASSERT(tau(1e+1000L, 1e+1000L) == 1);
  STATIC_ASSERT(tau(0.01L, 0.01L) == FP::pow(2,100));
  STATIC_ASSERT(tau(1,2) == FP::golden_ratio);
  STATIC_ASSERT(tau(2,4) == FP::sqrt(FP::golden_ratio));
  STATIC_ASSERT(tau(3,6) == FP::cbrt(FP::golden_ratio));


  typedef std::vector<FP::float80> probdist_t;
  bool is_probdist_basic(const probdist_t& v) noexcept {
    if (v.empty()) return false;
    for (const FP::float80 x : v) {
      if (FP::isnan(x) or x < 0 or x > 1) return false;
    }
    return true;
  }
  bool is_probdist_precise(const probdist_t& v) noexcept {
    if (not is_probdist_basic(v)) return false;
    return std::accumulate(v.begin(), v.end(), FP::float80(0)) == 1;
  }
  bool is_lprobdist_basic(const probdist_t& v) noexcept {
    if (v.empty()) return false;
    for (const FP::float80 x : v) {
      if (FP::isnan(x) or x > 0) return false;
    }
    return true;
  }

  probdist_t lptau(const FP::float80 a, const FP::float80 b) noexcept {
    assert(a >= 0);
    assert(b >= 0);
    if (a == 0)
      if (b == FP::pinfinity) return {0,FP::minfinity};
      else return {};
    if (b == 0)
      if (a == FP::pinfinity) return {FP::minfinity,0};
      else return {};
    if (a == FP::pinfinity)
      if (b == FP::pinfinity) return {};
      else return {FP::minfinity,0};
    if (b == FP::pinfinity)
      if (a == FP::pinfinity) return {};
      else return {0,FP::minfinity};
    const FP::float80 lt = ltau(a,b);
    return {-a * lt, -b * lt};
  }

  probdist_t ptau(const FP::float80 a, const FP::float80 b) noexcept {
    const auto p = lptau(a,b);
    if (p.empty()) return p;
    assert(is_lprobdist_basic(p));
    return {FP::exp(p[0]), FP::exp(p[1])};
  }


  /* The versions for double */

  inline CONSTEXPR FP::float64 ltau_64(FP::float64 a, FP::float64 b) noexcept {
    assert(a >= 0);
    assert(b >= 0);
    if (a > b) std::swap(a, b);
    if (a == 0)
      if (b == FP::pinfinity64) return FP::NaN64;
      else return FP::pinfinity64;
    if (b == FP::pinfinity64) return 0;
    if (a == b) return std::numbers::ln2 / a;
    return wtau_64(b / a) / b;
  }
  STATIC_ASSERT(ltau_64(0,0) == FP::pinfinity);
  STATIC_ASSERT(ltau_64(FP::pinfinity,FP::pinfinity) == 0);
  STATIC_ASSERT(FP::isnan(ltau_64(0,FP::pinfinity)));
  STATIC_ASSERT(FP::isnan(ltau_64(FP::pinfinity,0)));
  STATIC_ASSERT(ltau_64(1,1) == std::numbers::ln2);
  STATIC_ASSERT(ltau_64(1,0) == FP::pinfinity);
  STATIC_ASSERT(ltau_64(0,1) == FP::pinfinity);
  STATIC_ASSERT(ltau_64(1,FP::pinfinity) == 0);
  STATIC_ASSERT(ltau_64(FP::pinfinity,1) == 0);
  STATIC_ASSERT(ltau_64(2,2) == std::numbers::ln2/2);
  STATIC_ASSERT(ltau_64(3,3) == std::numbers::ln2/3);
  STATIC_ASSERT(ltau_64(1000,1000) == std::numbers::ln2/1000);
  STATIC_ASSERT(ltau_64(1e+200, 1e+200) == std::numbers::ln2 * 1e-200);
  STATIC_ASSERT(ltau_64(1e-100, 1e-100) == std::numbers::ln2 * 1e100);
  STATIC_ASSERT(ltau_64(FP::max_value64,FP::max_value64) == std::numbers::ln2/FP::max_value64);
  STATIC_ASSERT(ltau_64(FP::min_value64,FP::min_value64) == std::numbers::ln2/FP::min_value64);
  STATIC_ASSERT(ltau_64(1,2) == FP::log_golden_ratio64);
  STATIC_ASSERT(ltau_64(2,4) == FP::log_golden_ratio64 / 2);
  STATIC_ASSERT(ltau_64(3,6) == FP::log_golden_ratio64 / 3);
  STATIC_ASSERT(ltau_64(1e200,2e200) == FP::log_golden_ratio64 * 1e-200);
  STATIC_ASSERT(ltau_64(1e-200,2e-200) == FP::log_golden_ratio64 * 1e200);


  inline CONSTEXPR FP::float64 mtau_64(FP::float64 a, FP::float64 b) noexcept {
    if (a == b) return a;
    if (a > b) std::swap(a, b);
    assert(a >= 0);
    if (a == 0)
      if (b == FP::pinfinity64) return FP::NaN64;
      else return 0;
    if (b == FP::pinfinity64) return FP::pinfinity64;
    return std::numbers::ln2 / ltau_64(a,b);
  }
  STATIC_ASSERT(FP::isnan(mtau_64(0,FP::pinfinity64)));
  STATIC_ASSERT(FP::isnan(mtau_64(FP::pinfinity64,0)));
  STATIC_ASSERT(mtau_64(FP::minfinity64,FP::minfinity64) == FP::minfinity64);
  STATIC_ASSERT(mtau_64(-1,-1) == -1);
  STATIC_ASSERT(mtau_64(0,0) == 0);
  STATIC_ASSERT(mtau_64(2,2) == 2);
  STATIC_ASSERT(mtau_64(FP::pinfinity64,FP::pinfinity64) == FP::pinfinity64);
  STATIC_ASSERT(mtau_64(0,2) == 0);
  STATIC_ASSERT(mtau_64(1,FP::pinfinity64) == FP::pinfinity64);
  STATIC_ASSERT(mtau_64(2,0) == 0);
  STATIC_ASSERT(mtau_64(FP::pinfinity64,1) == FP::pinfinity64);
  STATIC_ASSERT(mtau_64(1,2) == std::numbers::ln2 / FP::log_golden_ratio64);
  STATIC_ASSERT(mtau_64(2,4) == std::log(4) / FP::log_golden_ratio64);

}
#endif
