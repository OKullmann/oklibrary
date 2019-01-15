// Oliver Kullmann, 2.1.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for floating-point computations, based on "long double"
  https://en.wikipedia.org/wiki/Extended_precision#x86_extended_precision_format

  Delivers the fundamental floating-type floating_t and the underlying
  64-bit unsigned UInt_t and the 32-bit unsigned uint_t.
  Wrap(x) for floating_t x just wraps it; output-streaming of such an object
  with maximal precision.

  The functions
    isinf, isnan, max, min, fma, log, exp, sqrt, abs, expm1, log1p, pow, round,

  are provided as wrappers, to make sure they work with floating_t.
  The constants

    pinfinity, min_value, max_value, epsilon,
    euler, eulerm1, golden_ratio, log_golden_ratio, P264 (= 2^64),
    pi, Stirling_factor (= sqrt(2*pi)), lStirling_factor (= log(2*pi)/2)

  of type floating_t are defined. The type limitfloat abbreviates the
  corresponding limits-type. Additionally the constants P264m1 = 2^64-1
  of type UInt_t and P232m1 = 2^32-1 of type uint_t are defined.

  Concerning factorial-type functions, we have
    factorial, lfactorial, Sfactorial, lSfactorial.

  Furthermore there is
    lambertW0l_lb, lambertW0_lb, lambertW0l_ub, lambertW0_ub.

TODOS:

1.  It seems that long double is indeed a fundamental type, since it fully
    contains 64-bit integer arithmetic. We should have helper classes
    to use this as a fundamental counting type (for solutions).

     - Output of such x, where x is integral and -P264 < x < P264, as integer
       (with full precision).
     - Otherwise via precision(FP::limitfloat::digits10 + 2).

*/

#ifndef FLOATINGPOINT_rbc6mhfmAG
#define FLOATINGPOINT_rbc6mhfmAG

#include <limits>
#include <type_traits>
#include <ostream>
#include <algorithm>

#include <cassert>
#include <cmath>
#include <cstdint>

namespace FloatingPoint {

  typedef long double floating_t;
  using limitfloat = std::numeric_limits<floating_t>;

  static_assert(limitfloat::is_iec559);
  static_assert(limitfloat::round_style == std::round_to_nearest);
  static_assert(limitfloat::digits >= 64);
  static_assert(limitfloat::radix == 2);
  static_assert(limitfloat::digits10 >= 18);
  // static_assert(FP_FAST_FMAL); not available in gcc 7.4

  struct Wrap {
    floating_t x;
    Wrap() = default;
    Wrap(const floating_t x) noexcept : x(x) {}
  };
  static_assert(std::is_pod_v<Wrap>);
  // Slow output:
  std::ostream& operator <<(std::ostream& out, const Wrap x) {
    const auto prec = out.precision();
    out.precision(limitfloat::digits10 + 2);
    out << x.x;
    out.precision(prec);
    return out;
  }

  constexpr floating_t pinfinity = limitfloat::infinity();
  static_assert(pinfinity > 0);
  static_assert(pinfinity > limitfloat::max());
  static_assert(-pinfinity < limitfloat::lowest());

  inline constexpr bool isinf(const floating_t x) noexcept {
    return std::isinf(x);
  }
  static_assert(isinf(pinfinity));
  static_assert(not isinf(limitfloat::max()));

  inline constexpr bool isnan(const floating_t x) noexcept {
    return std::isnan(x);
  }
  static_assert(isnan(limitfloat::quiet_NaN()));

  constexpr floating_t epsilon = limitfloat::epsilon();
  static_assert(1 - epsilon < 1);
  static_assert(1 + epsilon > 1);
  static_assert(1 + epsilon/2 == 1);
  static_assert(epsilon < 1.1e-19L);

  constexpr floating_t min_value = limitfloat::min();
  static_assert(min_value > 0);
  static_assert(min_value < 3.4e-4932L);
  static_assert(limitfloat::denorm_min() < 1e-4950L);
  static_assert(limitfloat::denorm_min() > 0);
  static_assert(limitfloat::denorm_min() / 2 == 0, "Higher precision than usual for denorm_min.");
  constexpr floating_t max_value = limitfloat::max();
  static_assert(max_value < pinfinity);
  static_assert(1/max_value > 0);
  static_assert(1/min_value < max_value);
  static_assert(1/max_value < min_value);
  static_assert(max_value > 1.1e4932L);
  static_assert(limitfloat::lowest() == -max_value);


  inline constexpr floating_t max(const floating_t x, const floating_t y) noexcept {
    return std::fmax(x,y);
  }
  inline constexpr floating_t min(const floating_t x, const floating_t y) noexcept {
    return std::fmin(x,y);
  }
  static_assert(max(1.23, -1.09) == 1.23);
  static_assert(min(44.123, 55.88) == 44.123);

  inline constexpr floating_t fma(const floating_t x, const floating_t y, const floating_t z) noexcept {
    return std::fma(x,y,z);
  }
  static_assert(fma(2,3,4) == 10);

  inline constexpr floating_t log(const floating_t x) noexcept {
    return std::log(x);
  }
  static_assert(log(1) == 0);
  static_assert(log(4) == 2*log(2));
  static_assert(log(0.5) == -log(2));
  // static_assert(log(pinfinity) == pinfinity); bug with gcc 7.4
  // static_assert(log(0) == -pinfinity); bug with gcc 7.4

  inline constexpr floating_t exp(const floating_t x) noexcept {
    return std::exp(x);
  }
  static_assert(exp(0) == 1);
  static_assert(exp(2) == exp(1)*exp(1));
  static_assert(log(exp(1)) == 1);
  constexpr floating_t euler = 2.718281828459045235360287471352662497757L;
  static_assert(euler == exp(1));
  static_assert(log(euler) == 1);
  constexpr floating_t eulerm1 = 1.718281828459045235360287471352662497757L;
  static_assert(abs(eulerm1 - (euler-1)) < epsilon);

  inline constexpr floating_t sqrt(const floating_t x) noexcept {
    return std::sqrt(x);
  }
  static_assert(sqrt(0) == 0);
  static_assert(sqrt(1) == 1);
  static_assert(sqrt(4) == 2);
  static_assert(sqrt(3*3+4*4) == 5);
  // static_assert(isnan(sqrt(-1))); bug with gcc 7.4

  constexpr floating_t golden_ratio = 1.6180339887498948482045868L;
  static_assert(golden_ratio == (1+sqrt(5))/2);
  static_assert(fma(golden_ratio,golden_ratio,-golden_ratio) == 1);
  constexpr floating_t log_golden_ratio = 0.4812118250596034474977589L;
  static_assert(log_golden_ratio == log(golden_ratio));
  static_assert(exp(log_golden_ratio) == golden_ratio);

  inline constexpr floating_t abs(const floating_t x) noexcept {
    return std::fabs(x);
  }
  static_assert(abs(sqrt(2)*sqrt(2) - 2) < 2*epsilon);
  static_assert(abs(log(sqrt(2)) - log(2)/2) < epsilon);

  inline constexpr floating_t expm1(const floating_t x) noexcept {
    return std::expm1(x);
  }
  static_assert(expm1(0) == 0);
  static_assert(expm1(1e-1000L) == 1e-1000L);
  static_assert(expm1(1) == eulerm1);

  inline constexpr floating_t log1p(const floating_t x) noexcept {
    return std::log1p(x);
  }
  static_assert(log1p(0) == 0);
  static_assert(log1p(1e-1000L) == 1e-1000L);

  inline constexpr floating_t pow(const floating_t x, const floating_t y) noexcept {
    return std::pow(x,y);
  }
  static_assert(pow(0,0) == 1);
  static_assert(pow(2,-1) == 0.5);
  static_assert(pow(2,16) == 65536);

  inline constexpr floating_t round(const floating_t x) noexcept {
    return std::round(x);
  }
  static_assert(round(0.5) == 1);
  static_assert(round(1.5) == 2);
  static_assert(round(2.5) == 3);
  static_assert(round(-0.5) == -1);
  static_assert(round(-1.5) == -2);


  // floating_t fully includes 64-bit integer arithmetic:
  typedef std::uint64_t UInt_t;
  typedef std::uint32_t uint_t;

  constexpr UInt_t P264m1 = std::numeric_limits<UInt_t>::max();
  static_assert(P264m1 == pow(2,64) - 1);
  static_assert(P264m1 + 1 == 0);
  static_assert(UInt_t(floating_t(P264m1)) == P264m1);
  constexpr uint_t P232m1 = std::numeric_limits<uint_t>::max();
  static_assert(P232m1 == pow(2,32) - 1);
  static_assert(P232m1 + 1 == 0);
  static_assert(UInt_t(P232m1)*P232m1 == pow(2,64) - pow(2,33) + 1);
  static_assert(UInt_t(P232m1)*P232m1 == P264m1 - 2*(UInt_t(P232m1)+1) + 2);

  constexpr floating_t P264 = 18446744073709551616.0L;
  static_assert(P264 == pow(2,64));
  static_assert(sqrt(P264) == pow(2,32));
  static_assert(sqrt(sqrt(P264)) == pow(2,16));
  static_assert(-(-P264) == P264);

  // Exactly the integers in the interval [-P264, +P264] are exactly represented by floating_t:
  static_assert(P264-1 == P264m1);
  static_assert(-P264 == -floating_t(P264m1) - 1);
  static_assert(P264+1 == P264);
  static_assert(-P264-1 == -P264);


  /* Computations related to the factorial function: */

  inline constexpr floating_t factorial(const uint_t N) noexcept {
    floating_t prod = 1;
    for (uint_t i = 1; i < N; ++i) prod *= i+1;
    return prod;
  }
  static_assert(factorial(0) == 1);
  static_assert(factorial(20) == 2432902008176640000ULL);
  static_assert(factorial(1754) < pinfinity);
  inline constexpr floating_t lfactorial(const UInt_t N) noexcept {
    floating_t sum = 0;
    for (UInt_t i = 1; i < N; ++i) sum += log1p(i);
    return sum;
  }
  static_assert(lfactorial(0) == 0);
  static_assert(lfactorial(1) == 0);
  static_assert(lfactorial(2) == log(2));
  static_assert(exp(lfactorial(10)) == factorial(10));
  static_assert(round(exp(lfactorial(19))) == factorial(19));

  // The Stirling approximation:

  constexpr floating_t pi = 3.141592653589793238462643383279502884L;
  static_assert(pi == std::acos(floating_t(-1)));
  static_assert(std::cos(pi) == -1);
  static_assert(abs(std::sin(pi)) < epsilon);

  constexpr floating_t Stirling_factor = 2.506628274631000502415765L;
  static_assert(Stirling_factor == sqrt(2*pi));
  inline constexpr floating_t Sfactorial(const uint_t N) noexcept {
    return Stirling_factor * sqrt(N) * pow(N/euler,N);
  }
  static_assert(Sfactorial(0) == 0);
  static_assert(Sfactorial(1) == Stirling_factor/euler);
  static_assert(Sfactorial(1754) < factorial(1754));
  static_assert(factorial(1754) / Sfactorial(1754) < 1.00005);
  constexpr floating_t lStirling_factor = 0.91893853320467274178032973640561763986L;
  static_assert(lStirling_factor == log(Stirling_factor));
  inline constexpr floating_t lSfactorial(const UInt_t N) noexcept {
    assert(N != 0);
    return fma(log(N), N+0.5, lStirling_factor-N);
  }
  static_assert(lSfactorial(1) == lStirling_factor - 1);
  static_assert(abs(lSfactorial(10) - log(Sfactorial(10))) < epsilon);


  /* Computations related to Lambert-W
     https://en.wikipedia.org/wiki/Lambert_W_function
     W(x) >= 0 for x >= 0 is the unique real number with W(x)*exp(W(x)) = x.
     We have W(0) = 0, and W is concave and strictly monotonically increasing,
     with limit infinity.
  */

  /* The lower bound for W(x), first taking log(x) as argument: */
  inline constexpr floating_t lambertW0l_lb(const floating_t l) noexcept {
    assert(l >= 1);
    const floating_t ll = log(l);
    return fma(ll/l, 0.5L, l-ll);
  }
  static_assert(lambertW0l_lb(1) == 1);
  static_assert(abs(lambertW0l_lb(euler) - fma(1/euler, 0.5L, eulerm1)) < 2*epsilon);
  inline constexpr floating_t lambertW0_lb(const floating_t x) noexcept {
    assert(x >= euler);
    return lambertW0l_lb(log(x));
  }
  static_assert(lambertW0_lb(euler) == 1);

  /* The upper bound for W(x), again first taking log(x) as argument: */
 inline constexpr floating_t lambertW0l_ub(const floating_t l) noexcept {
    assert(l >= 1);
    const floating_t ll = log(l);
    return fma(ll/l, euler/eulerm1, l-ll);
  }
  static_assert(lambertW0l_ub(1) == 1);
  static_assert(lambertW0l_ub(euler) == 1/eulerm1 + eulerm1);
  inline constexpr floating_t lambertW0_ub(const floating_t x) noexcept {
    assert(x >= euler);
    return lambertW0l_ub(log(x));
  }
  static_assert(lambertW0_ub(euler) == 1);

}

#endif
