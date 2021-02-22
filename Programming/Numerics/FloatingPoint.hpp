// Oliver Kullmann, 2.1.2019 (Swansea)
/* Copyright 2019, 2020, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for floating-point computations, providing float80/64/32

  For float80, the basic type, see
  https://en.wikipedia.org/wiki/Extended_precision#x86_extended_precision_format

  Delivers the fundamental floating-type float80 and the underlying
  64-bit unsigned UInt_t and the 32-bit unsigned uint_t.

  Wrap(x) for float80 x just wraps it; output-streaming of such an object
  with maximal precision; similar with Wrap64(x).

  The functions
    - isinf, isnan
    - nextafter
    - max, min
    - fma

    - log, log1p, log10, log2, ilogb
    - harmonic, coupcollprob (own functions)
    - exp, expm1, pow, exp2, ldexp
    - sin, cos
    - sq, cb (own functions)
    - sqrt, cbrt
    - abs
    - round, floor, trunc, ceil, antitrunc (own function)
    - erf, erfc
    - stold

  are provided as wrappers, to make sure they work with float80.
  The function
    - accuracy (plus accuracy_64 for float64, and accuracyg<FlOAT> for generic
      types, and accuracyv<VEC> for vectors)

  measures the distance in steps from the "precise" value.
  The following constants of type float80 are defined:

    - pinfinity, minfinity (positive and negative infinity)
    - NaN
    - min_value (smallest normal value > 0), max_value
    - denorm_min_value
    - epsilon

    - these constants are also defined for float64, with suffix "64";
      the following constants are only defined for float64 if explicitly
      mentioned:

    - Log2 (= log(2))
    - euler, eulerm1
    - Sqr2 = sqrt(2)
    - golden_ratio, log_golden_ratio, log_golden_ratio64
    - P264 (= 2^64), P232 (= 2^32)
    - max_binom
    - pi, Stirling_factor (= sqrt(2*pi)), lStirling_factor (= log(2*pi)/2)
    - euler_mascheroni

  The type limitfloat resp. limitfloat64 abbreviates the
  corresponding limits-type. Additionally the constants P264m1 = 2^64-1
  of type UInt_t and P232m1 = 2^32-1 of type uint_t are defined.

  Concerning factorial-type functions, we have
    - factorial, lfactorial, Sfactorial, lSfactorial
    - binomial_coeff, fbinomial_coeff, lbinomial_coeff.

  Furthermore there is
    lambertW0l_lb, lambertW0_lb, lambertW0l_ub, lambertW0_ub.
  The first two functions also exists in a double-version, called
    lambertW0l_lb_64, lambertW0_lb_64.

  Finally there are conversion functions:
    - isUInt(float80 x) tests whether x is integral within the range of UInt_t
    - toUInt(float80 x) converts (every) x >= 0 to UIint_t
    - touint(x) same for uint_t
    - to_float80(std::string s) converts s to float80
    - toUInt(std::string s) converts every string, which is convertible
      to float80, to UInt_t
    - touint(std::string s) converts every string convertible to float80
      to uint_t.

  Output-helper-functions:
    - fullprec_float80(std::ostream&) sets the maximum precision
    - similarly fullprec_float64, fullprec_float32, fullprec_floatg<FLOAT>.

  And the following macros are provided:
    - CONSTEXPR (disappears for non-gcc-compilation)
    - STATIC_ASSERT(X) (the same)
    - is_pod(X)

TODOS:

0.  See "Consolidate functions for bit-operatorions with integers" in
    SimpleBacktracking/Queens_RUCP_ct.cpp.

1.  Start using the numeric constants in <numbers> (C++20).
     - Unclear whether we should replace some of the existing constants.
     - Even just implementing some of the existing constants by the help of
       <numbers> seems to have little benefit -- these constants are
       eternal.

2.  We need also to provide double-versions; perhaps here we do not
    duplicate the standard C++-functions, but only provide additionally
    to special functions the double-versions.
     - Add documentation for the existing functions.
     - And this perhaps only for the functions which we need fast, that is,
       the functions around lambert_W.
     - Introduced float64, related constant with added suffix "64", and
       some functions with suffix "_64".

3.  It seems that long double is indeed a fundamental type, since it fully
    contains 64-bit integer arithmetic. We should have helper classes
    to use this as a fundamental counting type (for solutions).

     - Output of such x, where x is integral and -P264 < x < P264, as integer
       (with full precision).

*/

#ifndef FLOATINGPOINT_rbc6mhfmAG
#define FLOATINGPOINT_rbc6mhfmAG

#include <limits>
#include <type_traits>
#include <ostream>
#include <algorithm>
#include <string>
#include <numeric>
#include <initializer_list>
#include <numbers>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstddef>

namespace FloatingPoint {

// Uncomment for testing non-gcc-compilers:
//#undef __GNUC__

#if defined __GNUC__ && ! defined __clang__
#  define CONSTEXPR constexpr
#  define STATIC_ASSERT(X) static_assert(X)
#else
#  define CONSTEXPR
#  define STATIC_ASSERT(X) static_assert(0 == 0)
#endif

#define is_pod(X) std::is_standard_layout_v<X> and std::is_trivial_v<X>

  /* Basic concepts */

  typedef long double float80;
  typedef double float64;
  typedef float float32;

  using limitfloat = std::numeric_limits<float80>;

  // float80 fully includes 64-bit integer arithmetic:
  typedef std::uint64_t UInt_t;
  typedef std::uint32_t uint_t;

  static_assert(limitfloat::is_iec559);
  static_assert(limitfloat::round_style == std::round_to_nearest);
  static_assert(limitfloat::digits == 64);
  static_assert(limitfloat::radix == 2);
  static_assert(limitfloat::digits10 == 18);
  constexpr bool fp_fast_fmal =
#ifdef FP_FAST_FMAL
  true
#else
  false
#endif
;
  // static_assert(fp_fast_fmal); // not given in gcc 10.2, at least not for OK's laptop; http://www.cplusplus.com/reference/cmath/fma/

  struct Wrap {
    float80 x;
    Wrap() = default;
    Wrap(const float80 x) noexcept : x(x) {}
  };
  static_assert(is_pod(Wrap));
  auto fullprec_float80(std::ostream& out) noexcept {
    return out.precision(limitfloat::digits10 + 2);
  }
  // Slow output:
  std::ostream& operator <<(std::ostream& out, const Wrap x) {
    const auto prec = fullprec_float80(out);
    out << x.x;
    out.precision(prec);
    return out;
  }

  constexpr float80 pinfinity = limitfloat::infinity();
  static_assert(pinfinity > 0);
  static_assert(pinfinity > limitfloat::max());
  static_assert(-pinfinity < limitfloat::lowest());
  constexpr float80 minfinity = -pinfinity;
  static_assert(minfinity == -pinfinity);
  static_assert(-minfinity == pinfinity);
  static_assert(minfinity < pinfinity);
  static_assert(minfinity != pinfinity);
  static_assert(minfinity < limitfloat::lowest());

  inline CONSTEXPR bool isinf(const float80 x) noexcept {
    return std::isinf(x);
  }
  STATIC_ASSERT(isinf(pinfinity));
  STATIC_ASSERT(not isinf(limitfloat::max()));
  STATIC_ASSERT(isinf(-pinfinity));
  STATIC_ASSERT(isinf(minfinity));
  STATIC_ASSERT(not isinf(limitfloat::lowest()));

  constexpr float80 NaN = limitfloat::quiet_NaN();
  inline CONSTEXPR bool isnan(const float80 x) noexcept {
    return std::isnan(x);
  }
  STATIC_ASSERT(isnan(limitfloat::quiet_NaN()));
  STATIC_ASSERT(isnan(NaN));

  constexpr float80 epsilon = limitfloat::epsilon();
  static_assert(1 - epsilon < 1);
  static_assert(1 + epsilon > 1);
  static_assert(1 + epsilon/2 == 1);
  static_assert(epsilon < 1.1e-19L);

  constexpr float80 min_value = limitfloat::min();
  constexpr float80 denorm_min_value = limitfloat::denorm_min();
  static_assert(min_value > 0);
  static_assert(min_value < 3.4e-4932L);
  static_assert(denorm_min_value < 1e-4950L);
  static_assert(denorm_min_value > 0);
  static_assert(denorm_min_value / 2 == 0, "Higher precision than usual for denorm_min.");
  constexpr float80 max_value = limitfloat::max();
  static_assert(max_value < pinfinity);
  static_assert(1/max_value > 0);
  static_assert(1/min_value < max_value);
  static_assert(1/max_value < min_value);
  static_assert(max_value > 1.1e4932L);
  static_assert(limitfloat::lowest() == -max_value);


  inline CONSTEXPR float80 abs(const float80 x) noexcept {
    return std::fabs(x);
  }
  STATIC_ASSERT(abs(0) == 0);
  STATIC_ASSERT(abs(1) == 1);
  STATIC_ASSERT(abs(-1) == 1);

  /* Accuracy testing */

  inline CONSTEXPR float80 nextafter(const float80 from, const float80 to) noexcept {
    return std::nextafterl(from, to);
  }

  // The relative error of x compared with the exact value, in multiples of
  // the smallest possible difference (0 iff we have equality, 1 iff the
  // difference is minimal nonzero).
  enum class PrecZ { denorm=0, min=1, eps=2 };
  inline CONSTEXPR float80 accuracy(const float80 exact, const float80 x, const PrecZ pz = PrecZ::denorm) noexcept {
    if (isnan(exact)) return pinfinity;
    if (exact == pinfinity)
      if (x == pinfinity) return 0;
      else return pinfinity;
    if (exact == minfinity)
      if (x == minfinity) return 0;
      else return pinfinity;
    if (exact == x) return 0;
    if (exact == 0) {
      const float80 ax = FloatingPoint::abs(x);
      switch (pz) {
      case PrecZ::denorm : return ax / denorm_min_value;
      case PrecZ::min : return ax / min_value;
      default : return ax / epsilon; }
    }
    else if (exact < x)
      return (x - exact) / (nextafter(exact,x) - exact);
    else
      return (exact - x) / (exact - nextafter(exact,x));
  }
  STATIC_ASSERT(accuracy(NaN,NaN) == pinfinity);
  STATIC_ASSERT(accuracy(pinfinity,pinfinity) == 0);
  STATIC_ASSERT(accuracy(minfinity,minfinity) == 0);
  STATIC_ASSERT(accuracy(0,0) == 0);


  /* Import of numeric functions from the standard library */

  inline CONSTEXPR float80 max(const float80 x, const float80 y) noexcept {
    return std::fmax(x,y);
  }
  inline CONSTEXPR float80 min(const float80 x, const float80 y) noexcept {
    return std::fmin(x,y);
  }
  STATIC_ASSERT(max(1.23, -1.09) == 1.23);
  STATIC_ASSERT(min(44.123, 55.88) == 44.123);

  // x* y + z:
  inline CONSTEXPR float80 fma(const float80 x, const float80 y, const float80 z) noexcept {
    return std::fmal(x,y,z);
  }
  STATIC_ASSERT(fma(2,3,4) == 10);

  inline CONSTEXPR float80 log(const float80 x) noexcept {
    return std::log(x); // ERROR with gcc 10.2: std::logl not available
  }
  STATIC_ASSERT(log(1) == 0);
  STATIC_ASSERT(log(4) == 2*log(2));
  STATIC_ASSERT(log(0.5) == -log(2));
  constexpr float80 Log2 = 0.693147180559945309417232121458L;
  STATIC_ASSERT(Log2 == log(2));
  STATIC_ASSERT(float64(Log2) == std::numbers::ln2);
  // STATIC_ASSERT(log(pinfinity) == pinfinity); // bug with gcc 10.2
  // STATIC_ASSERT(log(0) == -pinfinity); // bug with gcc 10.2

  // log(1+x):
  inline CONSTEXPR float80 log1p(const float80 x) noexcept {
    return std::log1pl(x);
  }
  STATIC_ASSERT(log1p(0) == 0);
  STATIC_ASSERT(log1p(1e-1000L) == 1e-1000L);

  inline CONSTEXPR float80 log10(const float80 x) noexcept {
    return std::log10(x); // ERROR with gcc 10.2: std::log10l not available
  }
  STATIC_ASSERT(log10(10) == 1);

  inline CONSTEXPR float80 log2(const float80 x) noexcept {
    return std::log2l(x);
  }
  STATIC_ASSERT(log2(64) == 6);
  STATIC_ASSERT(log2(0.125) == -3);

  inline CONSTEXPR int ilogb(const float80 x) noexcept {
    return std::ilogbl(x);
  }
  STATIC_ASSERT(ilogb(8) == 3);
  STATIC_ASSERT(ilogb(9) == 3);
  STATIC_ASSERT(ilogb(16) == 4);
  STATIC_ASSERT(ilogb(1) == 0);
  STATIC_ASSERT(ilogb(0.9) == -1);
  STATIC_ASSERT(ilogb(0.5) == -1);
  STATIC_ASSERT(ilogb(0.4) == -2);

  inline constexpr float80 harmonic(const UInt_t n) noexcept {
    float80 sum = 0;
    for (UInt_t i = n; i != 0; --i) sum += float80(1) / i;
    return sum;
  }
  static_assert(harmonic(0) == 0);
  static_assert(harmonic(1) == 1);
  static_assert(harmonic(2) == 1.5L);
  static_assert(harmonic(3) == 11.0L / 6);
  inline constexpr float80 coupcollprob(const UInt_t n) noexcept {
    return n * harmonic(n);
  }
  static_assert(coupcollprob(0) == 0);
  static_assert(coupcollprob(1) == 1);
  static_assert(coupcollprob(2) == 3);
  static_assert(coupcollprob(3) == 5.5L);


  inline CONSTEXPR float80 exp(const float80 x) noexcept {
    return std::exp(x); // ERROR with gcc 10.2: std::expl not available
  }
  STATIC_ASSERT(exp(0) == 1);
  STATIC_ASSERT(exp(2) == exp(1)*exp(1));
  STATIC_ASSERT(log(exp(1)) == 1);
  constexpr float80 euler = 2.718281828459045235360287471352662497757L;
  STATIC_ASSERT(euler == exp(1));
  STATIC_ASSERT(log(euler) == 1);
  constexpr float80 eulerm1 = 1.718281828459045235360287471352662497757L;

  // exp(x) - 1:
  inline CONSTEXPR float80 expm1(const float80 x) noexcept {
    return std::expm1l(x);
  }
  STATIC_ASSERT(expm1(0) == 0);
  STATIC_ASSERT(expm1(1e-1000L) == 1e-1000L);
  STATIC_ASSERT(expm1(1) == eulerm1);

  inline CONSTEXPR float80 pow(const float80 x, const float80 y) noexcept {
    return std::pow(x,y); // ERROR with gcc 10.2: std::powl not available
  }
  STATIC_ASSERT(pow(0,0) == 1);
  STATIC_ASSERT(pow(2,-1) == 0.5);
  STATIC_ASSERT(pow(2,16) == 65536);

  // 2^x:
  inline CONSTEXPR float80 exp2(const float80 x) noexcept {
    return std::exp2l(x);
  }
  STATIC_ASSERT(exp2(64) == pow(2,64));
  STATIC_ASSERT(exp2(-1) == 0.5);

  // x * 2^x:
  inline CONSTEXPR float80 ldexp(const float80 x, const int exp) noexcept {
    return std::ldexp(x, exp); // ERROR with gcc 10.2: std::ldexpl not available
  }
  STATIC_ASSERT(ldexp(1,-1000) == pow(2,-1000));

  inline CONSTEXPR float80 sin(const float80 x) noexcept {
    return std::sin(x); // ERROR with gcc 10.2: std::sinl not available
  }
  STATIC_ASSERT(sin(0) == 0);
  inline CONSTEXPR float80 cos(const float80 x) noexcept {
    return std::cos(x); // ERROR with gcc 10.2: std::cosl not available
  }
  STATIC_ASSERT(cos(0) == 1);

  inline constexpr float80 sq(const float80 x) noexcept {
    return x*x;
  }
  static_assert(sq(0) == 0);
  static_assert(sq(1) == 1);
  static_assert(sq(2) == 4);
  static_assert(sq(-1) == 1);

  inline CONSTEXPR float80 sqrt(const float80 x) noexcept {
    return std::sqrt(x); // ERROR with gcc 10.2: std::sqrtl not available
  }
  STATIC_ASSERT(sqrt(0) == 0);
  STATIC_ASSERT(sqrt(1) == 1);
  STATIC_ASSERT(sqrt(4) == 2);
  STATIC_ASSERT(sqrt(3*3+4*4) == 5);
  constexpr float80 Sqr2 = 1.4142135623730950488016887242L;
  STATIC_ASSERT(Sqr2 == sqrt(2));
  // STATIC_ASSERT(isnan(sqrt(-1))); // bug with gcc 10.2

  constexpr float80 golden_ratio = 1.6180339887498948482045868L;
  STATIC_ASSERT(golden_ratio == (1+sqrt(5))/2);
  STATIC_ASSERT(fma(golden_ratio,golden_ratio,-golden_ratio) == 1);
  STATIC_ASSERT(float64(golden_ratio) == std::numbers::phi);
  constexpr float80 log_golden_ratio = 0.4812118250596034474977589L;
  STATIC_ASSERT(log_golden_ratio == log(golden_ratio));
  STATIC_ASSERT(exp(log_golden_ratio) == golden_ratio);

  inline constexpr float80 cb(const float80 x) noexcept {
    return x*x*x;
  }
  static_assert(cb(0) == 0);
  static_assert(cb(1) == 1);
  static_assert(cb(2) == 8);
  static_assert(cb(-1) == -1);

  inline CONSTEXPR float80 cbrt(const float80 x) noexcept {
    return std::cbrtl(x);
  }
  STATIC_ASSERT(cbrt(27) == 3);
  STATIC_ASSERT(cbrt(1e3) == 1e1L);
  STATIC_ASSERT(cb(cbrt(8)) == 8);

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

  inline CONSTEXPR float80 erf(const float80 x) noexcept {
    return std::erfl(x);
  }
  STATIC_ASSERT(erf(0) == 0);
  STATIC_ASSERT(erf(1) == 0.8427007929497148693412L);
  STATIC_ASSERT(erf(-1) == -0.8427007929497148693412L);

  inline CONSTEXPR float80 erfc(const float80 x) noexcept {
    return std::erfcl(x);
  }
  STATIC_ASSERT(erfc(0) == 1);
  STATIC_ASSERT(erfc(1) == 0.15729920705028513066L);
  STATIC_ASSERT(erfc(-1) == 1.8427007929497148693L);


  /* Connection with integral types */

  static_assert(0.3e1L == 3L);

  constexpr UInt_t P264m1 = std::numeric_limits<UInt_t>::max();
  static_assert(P264m1 == UInt_t(-1));
  STATIC_ASSERT(P264m1 == pow(2,64) - 1);
  static_assert(P264m1 == UInt_t(1.8446744073709551615e19L));
  static_assert(P264m1 + 1 == 0);
  static_assert(UInt_t(float80(P264m1)) == P264m1);
  constexpr uint_t P232m1 = std::numeric_limits<uint_t>::max();
  STATIC_ASSERT(P232m1 == pow(2,32) - 1);
  static_assert(P232m1 + 1 == 0);
  STATIC_ASSERT(UInt_t(P232m1)*P232m1 == pow(2,64) - pow(2,33) + 1);
  static_assert(UInt_t(P232m1)*P232m1 == P264m1 - 2*(UInt_t(P232m1)+1) + 2);
  STATIC_ASSERT(ldexp(ldexp(P264m1,10000),-10000) == P264m1);
  STATIC_ASSERT(ldexp(ldexp(P264m1,-10000),10000) == P264m1);
  STATIC_ASSERT(round(P264m1) == P264m1);

  constexpr float80 P264 = 18446744073709551616.0L;
  constexpr float80 P232 = 4294967296.0L;
  static_assert(P264 == 1.8446744073709551616e19L);
  static_assert(P232 == 4.294967296e9L);
  STATIC_ASSERT(P264 == pow(2,64));
  STATIC_ASSERT(P232 == pow(2,32));
  STATIC_ASSERT(sqrt(P264) == pow(2,32));
  STATIC_ASSERT(sqrt(sqrt(P264)) == pow(2,16));
  static_assert(-(-P264) == P264);

  // Exactly the integers in the interval [-P264, +P264] are exactly represented by float80:
  static_assert(P264-1 == P264m1);
  static_assert(-P264 == -float80(P264m1) - 1);
  static_assert(P264+1 == P264);
  static_assert(-P264-1 == -P264);
  static_assert(P232-1 == P232m1);
  static_assert(-P232 == -float80(P232m1) - 1);


  /* Computations related to the factorial function */

  inline constexpr float80 factorial(const uint_t N) noexcept {
    float80 prod = 1;
    for (uint_t i = 1; i < N; ++i) prod *= i+1;
    return prod;
  }
  static_assert(factorial(0) == 1);
  static_assert(factorial(20) == 2432902008176640000ULL);
  static_assert(factorial(1754) < pinfinity);
  inline CONSTEXPR float80 lfactorial(const UInt_t N) noexcept {
    float80 sum = 0;
    for (UInt_t i = 1; i < N; ++i) sum += log1p(i);
    return sum;
  }
  STATIC_ASSERT(lfactorial(0) == 0);
  STATIC_ASSERT(lfactorial(1) == 0);
  STATIC_ASSERT(lfactorial(2) == Log2);
  STATIC_ASSERT(exp(lfactorial(10)) == factorial(10));
  STATIC_ASSERT(round(exp(lfactorial(19))) == factorial(19));

  // The Stirling approximation:

  constexpr float80 pi = 3.141592653589793238462643383279502884L;
  STATIC_ASSERT(pi == std::acos(float80(-1)));
  STATIC_ASSERT(std::cos(pi) == -1);
  STATIC_ASSERT(abs(std::sin(pi)) < epsilon);

  constexpr float80 Stirling_factor = 2.506628274631000502415765L;
  STATIC_ASSERT(Stirling_factor == sqrt(2*pi));
  inline CONSTEXPR float80 Sfactorial(const uint_t N) noexcept {
    return Stirling_factor * sqrt(N) * pow(N/euler,N);
  }
  STATIC_ASSERT(Sfactorial(0) == 0);
  STATIC_ASSERT(Sfactorial(1) == Stirling_factor/euler);
  STATIC_ASSERT(Sfactorial(1754) < factorial(1754));
  STATIC_ASSERT(factorial(1754) / Sfactorial(1754) < 1.00005);
  constexpr float80 lStirling_factor = 0.91893853320467274178032973640561763986L;
  STATIC_ASSERT(lStirling_factor == log(Stirling_factor));
  inline CONSTEXPR float80 lSfactorial(const UInt_t N) noexcept {
    assert(N != 0);
    return fma(log(N), N+0.5, lStirling_factor-N);
  }
  STATIC_ASSERT(lSfactorial(1) == lStirling_factor - 1);

  // Binomial coefficients:

  // binomial(n,k) < 2^64 for all k iff n <= max_binom:
  constexpr UInt_t max_binom = 67;
  // The binomial-coefficient "choose k from n" for all results < 2^64:
  inline constexpr UInt_t binomial_coeff(const UInt_t n, const UInt_t k) noexcept {
    if (k > n) return 0;
    if (k == 0 or k == n) return 1;
    if (k == 1 or k == n-1) return n;
    if (k > n/2) return binomial_coeff(n, n-k);
    const UInt_t g = std::gcd(n,k), n2 = n/g, k2 = k/g;
    const UInt_t b = binomial_coeff(n-1,k-1);
    assert(b % k2 == 0);
    return n2 * (b / k2);
  }
  static_assert(binomial_coeff(0,1) == 0);
  static_assert(binomial_coeff(0,0) == 1);
  static_assert(binomial_coeff(10,0) == 1);
  static_assert(binomial_coeff(10,10) == 1);
  static_assert(binomial_coeff(5,3) == 10);
  static_assert(binomial_coeff(60,30) == 118264581564861424ULL);
  static_assert(binomial_coeff(80,21) == 10100903263463355200ULL);
  static_assert(binomial_coeff(70,27) == 18208558839321176480ULL);
  static_assert(binomial_coeff(100,83) == 6650134872937201800ULL);
  static_assert(binomial_coeff(67,33) == 14226520737620288370ULL);
  static_assert(binomial_coeff(67,34) == 14226520737620288370ULL);
  static_assert(binomial_coeff(6074001000,2) == 18446744070963499500ULL);

  /* Now returning a float80:
      - all results < 2^64 are correct (as above)
      - otherwise result >= P264 is guaranteed
      - these larger results are "approximations".
  */
  inline constexpr float80 fbinomial_coeff(const UInt_t n, const UInt_t k) noexcept {
    if (k > n) return 0;
    if (k == 0 or k == n) return 1;
    if (k == 1 or k == n-1) return n;
    if (k > n/2) return fbinomial_coeff(n, n-k);
    const UInt_t g = std::gcd(n,k);
    return (n/g) * (fbinomial_coeff(n-1,k-1) / (k/g));
  }
  static_assert(fbinomial_coeff(0,1) == 0);
  static_assert(fbinomial_coeff(0,2) == 0);
  static_assert(fbinomial_coeff(1,2) == 0);
  static_assert(fbinomial_coeff(0,0) == 1);
  static_assert(fbinomial_coeff(10,0) == 1);
  static_assert(fbinomial_coeff(2,2) == 1);
  static_assert(fbinomial_coeff(10,10) == 1);
  static_assert(fbinomial_coeff(5,3) == 10);
  static_assert(fbinomial_coeff(60,30) == 118264581564861424ULL);
  static_assert(fbinomial_coeff(80,21) == 10100903263463355200ULL);
  static_assert(fbinomial_coeff(70,27) == 18208558839321176480ULL);
  static_assert(fbinomial_coeff(100,83) == 6650134872937201800ULL);
  static_assert(fbinomial_coeff(67,33) == 14226520737620288370ULL);
  static_assert(fbinomial_coeff(67,34) == 14226520737620288370ULL);
  static_assert(fbinomial_coeff(6074001000,2) == 18446744070963499500ULL);
  static_assert(fbinomial_coeff(6074001001,2) > P264);
  static_assert(fbinomial_coeff(6074001001,2) == 1.8446744077037500500e19L);
  static_assert(fbinomial_coeff(68,34) > P264);
  static_assert(fbinomial_coeff(68,34) == 2.8453041475240576740e19L);

  // The log of binomial_coeff:
  inline constexpr float80 lbinomial_coeff(const UInt_t n, const UInt_t k) noexcept {
    if (k > n) return minfinity;
    if (k == 0 or k == n) return 0;
    if (k == 1 or k == n-1) return log(n);
    if (k > n/2) return lbinomial_coeff(n, n-k);
    float80 sum = 0;
    for (UInt_t i = n; i > n-k; --i) sum += log(i);
    for (UInt_t i = 2; i <= k; ++i) sum -= log(i);
    return sum;
  }
  STATIC_ASSERT(lbinomial_coeff(0,1) == minfinity);
  STATIC_ASSERT(lbinomial_coeff(0,0) == 0);
  STATIC_ASSERT(lbinomial_coeff(10,0) == 0);
  STATIC_ASSERT(lbinomial_coeff(10,10) == 0);
  STATIC_ASSERT(lbinomial_coeff(5,3) == log(10));
  STATIC_ASSERT(lbinomial_coeff(100,83) == log(binomial_coeff(100,83)));


  /* Computations related to Lambert-W
     https://en.wikipedia.org/wiki/Lambert_W_function
     W(x) >= 0 for x >= 0 is the unique real number with W(x)*exp(W(x)) = x.
     We have W(0) = 0, and W is concave and strictly monotonically increasing,
     with limit infinity.
  */

  /* The lower bound for W(x), first taking log(x) as argument:
       [Theorem 2.5, Hoorfar and Hassani, 2008]
       W(x) >= (ln(x)-ln(ln(x))+1)*ln(x)/(1+ln(x))
             = (1 - ln(ln(x))/(1+ln(x))) * ln(x)
       for x > 1.
     This bound is better for all x > e than the bound from Theorem 2.7 of
     the same paper (W(x) >= ln(x)-ln(ln(x))+1/2*ln(ln(x))/ln(x) for x >= e).
  */
  inline CONSTEXPR float80 lambertW0l_lb(const float80 l) noexcept {
    assert(l > 0);
    const float80 ll = log(l);
    return fma(-ll, l/(l+1), l);
  }
  STATIC_ASSERT(lambertW0l_lb(1) == 1);
  inline CONSTEXPR float80 lambertW0_lb(const float80 x) noexcept {
    assert(x > 1);
    return lambertW0l_lb(log(x));
  }
  STATIC_ASSERT(lambertW0_lb(euler) == 1);

  /* The upper bound
       W(x) <= ln(x) - ln(ln(x)) + e/(e-1) * ln(ln(x)) / ln(x),
     again first taking log(x) as argument:
  */
 inline CONSTEXPR float80 lambertW0l_ub(const float80 l) noexcept {
    assert(l >= 1);
    const float80 ll = log(l);
    return fma(ll/l, euler/eulerm1, l-ll);
  }
  STATIC_ASSERT(lambertW0l_ub(1) == 1);
  STATIC_ASSERT(lambertW0l_ub(euler) == 1/eulerm1 + eulerm1);
  inline CONSTEXPR float80 lambertW0_ub(const float80 x) noexcept {
    assert(x >= euler);
    return lambertW0l_ub(log(x));
  }
  STATIC_ASSERT(lambertW0_ub(euler) == 1);

  constexpr float80 euler_mascheroni = 0.57721566490153286060651209L;


  /* Conversion functions */

  inline CONSTEXPR bool isUInt(const float80 x) noexcept {
    if (isnan(x)) return false;
    if (x < 0) return false;
    if (x > P264m1) return false;
    if (UInt_t(x) != x) return false;
    return true;
  }
  STATIC_ASSERT(isUInt(0)); STATIC_ASSERT(not isUInt(-1));
  STATIC_ASSERT(isUInt(P264m1)); STATIC_ASSERT(not isUInt(NaN));
  STATIC_ASSERT(not isUInt(pinfinity)); STATIC_ASSERT(not isUInt(minfinity));
  STATIC_ASSERT(not isUInt(0.5)); STATIC_ASSERT(not isUInt(P264m1 + 0.5L));
  inline CONSTEXPR bool isUInt(const std::initializer_list<float80> X) noexcept {
    for (const float80 x : X) if (not isUInt(x)) return false;
    return true;
  }
  STATIC_ASSERT(isUInt({0,1,2,P264m1}));
  STATIC_ASSERT(not isUInt({0,1,2,P264m1,1.1}));


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

  inline float80 stold(const std::string& s, std::size_t* const pos = 0) {
    return std::stold(s, pos);
  }

  // Improving stold, by improving error-messages in exceptions, and
  // furthermore throwing std:domain_error in case of trailing characters:
  inline float80 to_float80(const std::string& s) {
    std::size_t converted;
    long double x;
    try { x = FloatingPoint::stold(s,&converted); }
    catch(const std::invalid_argument& e) {
      throw std::invalid_argument("FloatingPoint::to_float80(string), failed"
        " for \"" + s + "\"");
    }
    catch(const std::out_of_range& e) {
      throw std::out_of_range("FloatingPoint::to_float80(string), \""
        + s + "\"");
    }
    if (converted != s.size())
      throw std::domain_error("FloatingPoint::to_float80(string), trailing: \""
        + s.substr(converted) + "\" in \"" + s + "\"");
    return x;
  }

  // Succeeds for every s convertible to float80, interpreting negative x
  // as zero, too big x as the maximal value, and applying rounding otherwise:
  inline UInt_t toUInt(const std::string& s) {
    const float80 x = to_float80(s);
    if (not (x >= 0)) return 0;
    else return toUInt(x);
  }
  inline uint_t touint(const std::string& s) {
    const float80 x = to_float80(s);
    if (not (x >= 0)) return 0;
    else return touint(x);
  }


  /* Basic definitions for float64 = double */

  using limitfloat64 = std::numeric_limits<float64>;
  using limitfloat32 = std::numeric_limits<float32>;

  static_assert(limitfloat64::is_iec559);
  static_assert(limitfloat64::round_style == std::round_to_nearest);
  static_assert(limitfloat64::digits == 53);
  static_assert(limitfloat64::radix == 2);
  static_assert(limitfloat64::digits10 == 15);

  constexpr float64 pinfinity64 = limitfloat64::infinity();
  static_assert(pinfinity64 > 0);
  static_assert(pinfinity64 > limitfloat64::max());
  static_assert(-pinfinity64 < limitfloat64::lowest());
  constexpr float64 minfinity64 = -pinfinity64;
  static_assert(minfinity64 == -pinfinity64);
  static_assert(-minfinity64 == pinfinity64);
  static_assert(minfinity64 < pinfinity64);
  static_assert(minfinity64 != pinfinity64);
  static_assert(minfinity64 < limitfloat64::lowest());

  constexpr float64 NaN64 = limitfloat64::quiet_NaN();

  constexpr float64 epsilon64 = limitfloat64::epsilon();
  static_assert(1 - epsilon64 < 1);
  static_assert(1 + epsilon64 > 1);
  static_assert(1 + epsilon64/2 == 1);
  static_assert(epsilon64 < 3e-16L);
  constexpr float64 min_value64 = limitfloat64::min();
  constexpr float64 denorm_min_value64 = limitfloat64::denorm_min();
  static_assert(min_value64 > 0);
  static_assert(min_value64 < 2.3e-308);
  static_assert(denorm_min_value64 < 1e-323);
  static_assert(denorm_min_value64 > 0);
  static_assert(denorm_min_value64 / 2 == 0, "Higher precision than usual for denorm_min.");
  constexpr float64 max_value64 = limitfloat64::max();
  static_assert(max_value64 < pinfinity64);
  static_assert(1/max_value64 > 0);
  static_assert(1/min_value64 < max_value64);
  static_assert(1/max_value64 < min_value64);
  static_assert(max_value64 > 1.7e308);
  static_assert(limitfloat64::lowest() == -max_value64);

  template <typename FL>
  inline CONSTEXPR FL accuracyg(const FL exact, const FL x, const PrecZ pz = PrecZ::denorm) noexcept {
    using limitfloatg = std::numeric_limits<FL>;
    constexpr FL pinfinityg = limitfloatg::infinity();
    constexpr FL minfinityg = -pinfinityg;
    if (std::isnan(exact)) return pinfinityg;
    if (exact == pinfinityg)
      if (x == pinfinityg) return 0;
      else return pinfinityg;
    if (exact == minfinityg)
      if (x == minfinityg) return 0;
      else return pinfinityg;
    if (exact == x) return 0;
    if (exact == 0) {
      const FL ax = std::abs(x);
      switch (pz) {
      case PrecZ::denorm : return ax / limitfloatg::denorm_min();
      case PrecZ::min : return ax / limitfloatg::min();
      default : return ax / limitfloatg::epsilon(); }
    }
    else if (exact < x)
      return (x - exact) / (std::nextafter(exact,x) - exact);
    else
      return (exact - x) / (exact - std::nextafter(exact,x));
  }
  STATIC_ASSERT(accuracyg(NaN,NaN) == pinfinity);
  STATIC_ASSERT(accuracyg(pinfinity,pinfinity) == 0);
  STATIC_ASSERT(accuracyg(minfinity,minfinity) == 0);
  STATIC_ASSERT(accuracyg(0.0L,0.0L) == 0);
  inline CONSTEXPR float64 accuracy_64(const float64 exact, const float64 x, const PrecZ pz = PrecZ::denorm) noexcept {
    if (std::isnan(exact)) return pinfinity64;
    if (exact == pinfinity64)
      if (x == pinfinity64) return 0;
      else return pinfinity64;
    if (exact == minfinity64)
      if (x == minfinity64) return 0;
      else return pinfinity64;
    if (exact == x) return 0;
    if (exact == 0) {
      const float64 ax = std::abs(x);
      switch (pz) {
      case PrecZ::denorm : return ax / denorm_min_value64;
      case PrecZ::min : return ax / min_value64;
      default : return ax / epsilon64; }
    }
    else if (exact < x)
      return (x - exact) / (std::nextafter(exact,x) - exact);
    else
      return (exact - x) / (exact - std::nextafter(exact,x));
  }
  STATIC_ASSERT(accuracy_64(NaN64,NaN64) == pinfinity64);
  STATIC_ASSERT(accuracy_64(pinfinity64,pinfinity64) == 0);
  STATIC_ASSERT(accuracy_64(minfinity64,minfinity64) == 0);
  STATIC_ASSERT(accuracy_64(0,0) == 0);

  template <typename VEC>
  inline auto accuracyv(const VEC& vex, const VEC& v, const PrecZ pz = PrecZ::denorm) noexcept {
    typedef typename VEC::value_type float_t;
    float_t res = -1;
    typedef typename VEC::size_type size_t;
    const size_t size = std::min(vex.size(), v.size());
    for (size_t i = 0; i < size; ++i)
      res = std::max(res, accuracyg<float_t>(vex[i], v[i], pz));
    return res;
  }


  constexpr float64 log_golden_ratio64 = log_golden_ratio;
  STATIC_ASSERT(log_golden_ratio64 == std::log(std::numbers::phi));


  inline CONSTEXPR float64 lambertW0l_lb_64(const float64 l) noexcept {
    assert(l > 0);
    const float64 ll = std::log(l);
    return std::fma(-ll, l/(l+1), l);
  }
  STATIC_ASSERT(lambertW0l_lb_64(1) == 1);
  inline CONSTEXPR float64 lambertW0_lb_64(const float64 x) noexcept {
    assert(x > 1);
    return lambertW0l_lb_64(std::log(x));
  }
  STATIC_ASSERT(lambertW0_lb_64(euler) == 1);


  struct Wrap64 {
    float64 x;
    Wrap64() = default;
    Wrap64(const float64 x) noexcept : x(x) {}
  };
  static_assert(is_pod(Wrap64));
  auto fullprec_float64(std::ostream& out) noexcept {
    return out.precision(limitfloat64::digits10 + 2);
  }
  // Slow output:
  std::ostream& operator <<(std::ostream& out, const Wrap64 x) {
    const auto prec = fullprec_float64(out);
    out << x.x;
    out.precision(prec);
    return out;
  }

  struct Wrap32 {
    float32 x;
    Wrap32() = default;
    Wrap32(const float32 x) noexcept : x(x) {}
  };
  static_assert(is_pod(Wrap32));
  auto fullprec_float32(std::ostream& out) noexcept {
    return out.precision(limitfloat32::digits10 + 2);
  }
  // Slow output:
  std::ostream& operator <<(std::ostream& out, const Wrap32 x) {
    const auto prec = fullprec_float32(out);
    out << x.x;
    out.precision(prec);
    return out;
  }

  template <typename FLOAT>
  std::streamsize fullprec_floatg(std::ostream& out) noexcept {
    return out.precision(std::numeric_limits<FLOAT>::digits10 + 2);
  }

}

#endif
