// Oliver Kullmann, 2.1.2019 (Swansea)
/* Copyright 2019, 2020, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for floating-point computations, providing float80/64/32

  The functions
    - nextafter
    - fma

    - log, log1p, log10, log2, ilogb
    - harmonic, coupcollprob (own functions)
    - exp, expm1, pow, exp2, ldexp
    - sin, cos, tan, asin, acos, atan, atan2
    - sq, cb, qa (own functions)
    - sqrt, cbrt, hypot, qart (own function)
    - midpoint, lerp
    - abs, copysign, signbit
    - round, floor, trunc, ceil, antitrunc (own function)
    - erf, erfc
    - stold

  are provided as wrappers, to make sure they work with float80.

  The function
    - accuracy (plus accuracy_64 for float64, and accuracyg<FlOAT> for generic
      types, and accuracyv<VEC>, accuracymax<VEC> for vectors)

  measures the distance in steps from the "precise" value.

  The following constants of type float80 are defined (only defined for
  float64 if explicitly mentioned):

    - Log2 (= log(2))
    - euler, eulerm1
    - Sqr2 = sqrt(2), Cbr2 = cbrt(2), Qar2 = qart(2)
    - golden_ratio, log_golden_ratio, log_golden_ratio64
    - max_binom
    - pi, Stirling_factor (= sqrt(2*pi)), lStirling_factor (= log(2*pi)/2)
    - euler_mascheroni

  Concerning factorial-type functions, we have
    - factorial, lfactorial, Sfactorial, lSfactorial
    - binomial_coeff, fbinomial_coeff, lbinomial_coeff.

  Furthermore there is
    lambertW0l_lb, lambertW0_lb, lambertW0l_ub, lambertW0_ub.
  The first two functions also exists in a double-version, called
    lambertW0l_lb_64, lambertW0_lb_64.

TODOS:

-1. Currently we have UInt_t and uint_t operations also implemented here.
     - One might put that into a different module.
     - On the other hand, floating-point operations are important for us,
       and we care about precision, and thus we want to "live in one world"
       (not fractured)>
     - Another aspect here is the issue with static_assert's for floating-
       point operations.

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

#include <algorithm>
#include <numeric>
#include <initializer_list>
#include <numbers>

#include <cassert>
#include <cmath>
#include <cstddef>

// Guaranteed to be included:
#include "NumTypes.hpp"
#include "Conversions.hpp"
#include "NumInOut.hpp"


namespace FloatingPoint {

  inline CONSTEXPR float80 abs(const float80 x) noexcept {
    return std::fabs(x); // ERROR with gcc 10.1: std::fabsl not available
  }
  STATIC_ASSERT(abs(0) == 0);
  STATIC_ASSERT(abs(1) == 1);
  STATIC_ASSERT(abs(-1) == 1);

  inline CONSTEXPR float80 copysign(const float80 x, const float80 y) noexcept {
    return std::copysign(x,y); // ERROR with gcc 10.1: std::copysignl not available
  }
  STATIC_ASSERT(copysign(3,-2) == -3);
  STATIC_ASSERT(copysign(1,-0.0) == -1);
  STATIC_ASSERT(copysign(-3,2) == 3);

  inline CONSTEXPR float80 signbit(const float80 x) noexcept {
    return std::signbit(x);
  }
  STATIC_ASSERT(not signbit(1));
  STATIC_ASSERT(not signbit(0));
  STATIC_ASSERT(signbit(-0.0));
  STATIC_ASSERT(signbit(-1));


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
    else {
      const float80 pres = exact < x ?
        (x - exact) / (nextafter(exact,x) - exact) :
        (exact - x) / (exact - nextafter(exact,x));
      if (pres > 10) return pres;
      unsigned count = 1;
      for (float80 xp=nextafter(exact,x); xp != x;
           xp=nextafter(xp,x), ++count);
      return count;
    }
  }
  STATIC_ASSERT(accuracy(NaN,NaN) == pinfinity);
  STATIC_ASSERT(accuracy(pinfinity,pinfinity) == 0);
  STATIC_ASSERT(accuracy(minfinity,minfinity) == 0);
  STATIC_ASSERT(accuracy(0,0) == 0);


  /* Import of numeric functions from the standard library */

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
  STATIC_ASSERT(P264m1 == pow(2,64) - 1);
  STATIC_ASSERT(P232m1 == pow(2,32) - 1);
  STATIC_ASSERT(UInt_t(P232m1)*P232m1 == pow(2,64) - pow(2,33) + 1);
  STATIC_ASSERT(P264 == pow(2,64));
  STATIC_ASSERT(P232 == pow(2,32));

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
  STATIC_ASSERT(ldexp(ldexp(P264m1,10000),-10000) == P264m1);
  STATIC_ASSERT(ldexp(ldexp(P264m1,-10000),10000) == P264m1);

  inline CONSTEXPR float80 sin(const float80 x) noexcept {
    return std::sin(x); // ERROR with gcc 10.2: std::sinl not available
  }
  STATIC_ASSERT(sin(0) == 0);
  inline CONSTEXPR float80 cos(const float80 x) noexcept {
    return std::cos(x); // ERROR with gcc 10.2: std::cosl not available
  }
  STATIC_ASSERT(cos(0) == 1);
  inline CONSTEXPR float80 tan(const float80 x) noexcept {
    return std::tan(x); // ERROR with gcc 10.2: std::tanl not available
  }
  STATIC_ASSERT(tan(0) == 0);
  inline CONSTEXPR float80 asin(const float80 x) noexcept {
    return std::asin(x); // ERROR with gcc 10.2: std::asinl not available
  }
  STATIC_ASSERT(asin(0) == 0);
  inline CONSTEXPR float80 acos(const float80 x) noexcept {
    return std::acos(x); // ERROR with gcc 10.2: std::acosl not available
  }
  STATIC_ASSERT(acos(1) == 0);
  inline CONSTEXPR float80 atan(const float80 x) noexcept {
    return std::atan(x); // ERROR with gcc 10.2: std::acosl not available
  }
  STATIC_ASSERT(atan(0) == 0);
  inline CONSTEXPR float80 atan2(const float80 x, const float80 y) noexcept {
    return std::atan2(x,y); // ERROR with gcc 10.2: std::atan2l not available
  }
  STATIC_ASSERT(atan2(0,0) == 0);
  STATIC_ASSERT(atan2(-1,0) == -acos(0));
  STATIC_ASSERT(atan2(1,0) == acos(0));

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
  STATIC_ASSERT(sqrt(P264) == pow(2,32));
  STATIC_ASSERT(sqrt(sqrt(P264)) == pow(2,16));

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

  inline constexpr float80 qa(const float80 x) noexcept {
    return sq(sq(x));
  }
  static_assert(qa(0) == 0);
  static_assert(qa(1) == 1);
  static_assert(qa(2) == 16);
  static_assert(qa(-1) == 1);

  inline CONSTEXPR float80 qart(const float80 x) noexcept {
    return sqrt(sqrt(x));
  }
  STATIC_ASSERT(qart(81) == 3);
  STATIC_ASSERT(qart(1e4) == 1e1L);
  STATIC_ASSERT(qart(qa(8)) == 8);

  constexpr float80 Cbr2 = 1.259921049894873164767210607L;
  STATIC_ASSERT(Cbr2 == cbrt(2));
  constexpr float80 Qar2 = 1.18920711500272106671749997056L;
  STATIC_ASSERT(Qar2 == qart(2));

  inline CONSTEXPR float80 hypot(const float80 x, const float80 y) noexcept {
    return std::hypot(x,y);
  }
  STATIC_ASSERT(hypot(3,4) == 5);

  inline constexpr float80 midpoint(const float80 a, const float80 b) noexcept {
    return std::midpoint(a,b);
  }
  static_assert(midpoint(-1, 1) == 0);

  inline constexpr float80 lerp(const float80 a, const float80 b, const float80 t) noexcept {
    return std::lerp(a,b,t);
  }
  static_assert(lerp(0, 4, 0.25L) == 1);



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
  STATIC_ASSERT(pi == FloatingPoint::acos(-1));
  STATIC_ASSERT(pi/2 == FloatingPoint::acos(0));
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
    else {
      const float64 pres = exact < x ?
        (x - exact) / (std::nextafter(exact,x) - exact) :
        (exact - x) / (exact - std::nextafter(exact,x));
      if (pres > 10) return pres;
      unsigned count = 1;
      for (float64 xp=std::nextafter(exact,x); xp != x;
           xp=std::nextafter(xp,x), ++count);
      return count;
    }
  }
  STATIC_ASSERT(accuracy_64(NaN64,NaN64) == pinfinity64);
  STATIC_ASSERT(accuracy_64(pinfinity64,pinfinity64) == 0);
  STATIC_ASSERT(accuracy_64(minfinity64,minfinity64) == 0);
  STATIC_ASSERT(accuracy_64(0,0) == 0);

  template <typename FL>
  inline CONSTEXPR FL accuracyg(const FL exact, const FL x, const PrecZ pz = PrecZ::denorm) noexcept {
    if constexpr (std::is_same_v<FL,float80>) return accuracy(exact, x, pz);
    if constexpr (std::is_same_v<FL,float64>) return accuracy_64(exact, x, pz);
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
    else {
      const FL pres = exact < x ?
        (x - exact) / (std::nextafter(exact,x) - exact) :
        (exact - x) / (exact - std::nextafter(exact,x));
      if (pres > 10) return pres;
      unsigned count = 1;
      for (FL xp=std::nextafter(exact,x); xp != x;
           xp=std::nextafter(xp,x), ++count);
      return count;
    }
  }
  STATIC_ASSERT(accuracyg(NaN,NaN) == pinfinity);
  STATIC_ASSERT(accuracyg(pinfinity,pinfinity) == 0);
  STATIC_ASSERT(accuracyg(minfinity,minfinity) == 0);
  STATIC_ASSERT(accuracyg(0.0L,0.0L) == 0);

  template <typename VEC1, typename VEC2 = VEC1, typename VEC3 = VEC1>
  inline VEC3 accuracyv(const VEC1& vex, const VEC2& v, const PrecZ pz = PrecZ::denorm) {
    typedef typename VEC1::value_type float_t;
    typedef typename VEC1::size_type size_t;
    const size_t size = std::min(vex.size(), v.size());
    VEC3 res(size);
    for (size_t i = 0; i < size; ++i)
      res[i] = accuracyg<float_t>(vex[i], v[i], pz);
    return res;
  }

  template <typename VEC1, typename VEC2 = VEC1>
  inline auto accuracymax(const VEC1& vex, const VEC2& v, const PrecZ pz = PrecZ::denorm) noexcept {
    typedef typename VEC1::value_type float_t;
    float_t res = -1;
    typedef typename VEC1::size_type size_t;
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

}

#endif
