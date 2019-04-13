// Oliver Kullmann, 2.1.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for floating-point computations, based on "long double"
  https://en.wikipedia.org/wiki/Extended_precision#x86_extended_precision_format

  Delivers the fundamental floating-type float80 and the underlying
  64-bit unsigned UInt_t and the 32-bit unsigned uint_t.
  Wrap(x) for float80 x just wraps it; output-streaming of such an object
  with maximal precision.

  The functions
    - isinf, isnan
    - max, min
    - fma
    - log, log1p, log10, log2, ilogb
    - exp, expm1, pow, exp2, ldexp
    - sq (own function), sqrt, cbrt
    - abs
    - round, floor, trunc, ceil, antitrunc (own function)
    - erf, erfc

  are provided as wrappers, to make sure they work with float80.
  The constants

    - pinfinity, min_value, max_value, epsilon,
    - Log2 (= log(2))
    - euler, eulerm1,
    - Sqr2 = sqrt(2), golden_ratio, log_golden_ratio,
    - P264 (= 2^64),
    - pi, Stirling_factor (= sqrt(2*pi)), lStirling_factor (= log(2*pi)/2)
    - euler_mascheroni

  of type float80 are defined. The type limitfloat abbreviates the
  corresponding limits-type. Additionally the constants P264m1 = 2^64-1
  of type UInt_t and P232m1 = 2^32-1 of type uint_t are defined.

  Concerning factorial-type functions, we have
    factorial, lfactorial, Sfactorial, lSfactorial.

  Furthermore there is
    lambertW0l_lb, lambertW0_lb, lambertW0l_ub, lambertW0_ub.
  The first function also exists in a double-version, called
    lambertW0l_lb_d.

  Finally there are conversion functions:
    - toUInt(float80 x) converts (every) x >= 0 to UIint_t
    - stold(const std::string& s) converts s to float80
    - toUInt(const std::string& s) converts every string, which is convertible
      to float80, to UInt_t.

TODOS:

0.  We need also to provide double-versions; perhaps here we do not
    duplicate the standard C++-functions, but only provide additionally
    to special functions the double-versions.
     - And this perhaps only for the functions which we need fast, that is,
       the functions around lambert_W.
     - Shall we rely on the argument type of the function (using overloading)?
     - Perhaps floating_t should be renamed to "float80" ? DONE

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
#include <string>

#include <cassert>
#include <cmath>
#include <cstdint>

namespace FloatingPoint {

  /* Basic concepts */

  typedef long double float80;
  using limitfloat = std::numeric_limits<float80>;

  static_assert(limitfloat::is_iec559);
  static_assert(limitfloat::round_style == std::round_to_nearest);
  static_assert(limitfloat::digits >= 64);
  static_assert(limitfloat::radix == 2);
  static_assert(limitfloat::digits10 >= 18);
  constexpr bool fp_fast_fmal =
#ifdef FP_FAST_FMAL
  true
#else
  false
#endif
;
  // static_assert(fp_fast_fmal); // not given in gcc 8.3, at least not for OK's laptop; http://www.cplusplus.com/reference/cmath/fma/

  struct Wrap {
    float80 x;
    Wrap() = default;
    Wrap(const float80 x) noexcept : x(x) {}
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

  constexpr float80 pinfinity = limitfloat::infinity();
  static_assert(pinfinity > 0);
  static_assert(pinfinity > limitfloat::max());
  static_assert(-pinfinity < limitfloat::lowest());

  inline constexpr bool isinf(const float80 x) noexcept {
    return std::isinf(x);
  }
  static_assert(isinf(pinfinity));
  static_assert(not isinf(limitfloat::max()));
  static_assert(isinf(-pinfinity));
  static_assert(not isinf(limitfloat::lowest()));

  inline constexpr bool isnan(const float80 x) noexcept {
    return std::isnan(x);
  }
  static_assert(isnan(limitfloat::quiet_NaN()));

  constexpr float80 epsilon = limitfloat::epsilon();
  static_assert(1 - epsilon < 1);
  static_assert(1 + epsilon > 1);
  static_assert(1 + epsilon/2 == 1);
  static_assert(epsilon < 1.1e-19L);

  constexpr float80 min_value = limitfloat::min();
  static_assert(min_value > 0);
  static_assert(min_value < 3.4e-4932L);
  static_assert(limitfloat::denorm_min() < 1e-4950L);
  static_assert(limitfloat::denorm_min() > 0);
  static_assert(limitfloat::denorm_min() / 2 == 0, "Higher precision than usual for denorm_min.");
  constexpr float80 max_value = limitfloat::max();
  static_assert(max_value < pinfinity);
  static_assert(1/max_value > 0);
  static_assert(1/min_value < max_value);
  static_assert(1/max_value < min_value);
  static_assert(max_value > 1.1e4932L);
  static_assert(limitfloat::lowest() == -max_value);


  /* Import of numeric functions from the standard library */

  inline constexpr float80 max(const float80 x, const float80 y) noexcept {
    return std::fmax(x,y);
  }
  inline constexpr float80 min(const float80 x, const float80 y) noexcept {
    return std::fmin(x,y);
  }
  static_assert(max(1.23, -1.09) == 1.23);
  static_assert(min(44.123, 55.88) == 44.123);

  inline constexpr float80 fma(const float80 x, const float80 y, const float80 z) noexcept {
    return std::fma(x,y,z);
  }
  static_assert(fma(2,3,4) == 10);

  inline constexpr float80 log(const float80 x) noexcept {
    return std::log(x);
  }
  static_assert(log(1) == 0);
  static_assert(log(4) == 2*log(2));
  static_assert(log(0.5) == -log(2));
  constexpr float80 Log2 = 0.693147180559945309417232121458L;
  static_assert(Log2 == log(2));
  // static_assert(log(pinfinity) == pinfinity); // bug with gcc 8.3
  // static_assert(log(0) == -pinfinity); // bug with gcc 8.3

  inline constexpr float80 log1p(const float80 x) noexcept {
    return std::log1p(x);
  }
  static_assert(log1p(0) == 0);
  static_assert(log1p(1e-1000L) == 1e-1000L);

  inline constexpr float80 log10(const float80 x) noexcept {
    return std::log10(x);
  }
  static_assert(log10(10) == 1);

  inline constexpr float80 log2(const float80 x) noexcept {
    return std::log2(x);
  }
  static_assert(log2(64) == 6);
  static_assert(log2(0.125) == -3);

  inline constexpr int ilogb(const float80 x) noexcept {
    return std::ilogb(x);
  }
  static_assert(ilogb(8) == 3);
  static_assert(ilogb(9) == 3);
  static_assert(ilogb(16) == 4);
  static_assert(ilogb(1) == 0);
  static_assert(ilogb(0.9) == -1);
  static_assert(ilogb(0.5) == -1);
  static_assert(ilogb(0.4) == -2);

  inline constexpr float80 exp(const float80 x) noexcept {
    return std::exp(x);
  }
  static_assert(exp(0) == 1);
  static_assert(exp(2) == exp(1)*exp(1));
  static_assert(log(exp(1)) == 1);
  constexpr float80 euler = 2.718281828459045235360287471352662497757L;
  static_assert(euler == exp(1));
  static_assert(log(euler) == 1);
  constexpr float80 eulerm1 = 1.718281828459045235360287471352662497757L;
  static_assert(abs(eulerm1 - (euler-1)) < 2*epsilon);

  inline constexpr float80 expm1(const float80 x) noexcept {
    return std::expm1(x);
  }
  static_assert(expm1(0) == 0);
  static_assert(expm1(1e-1000L) == 1e-1000L);
  static_assert(expm1(1) == eulerm1);

  inline constexpr float80 pow(const float80 x, const float80 y) noexcept {
    return std::pow(x,y);
  }
  static_assert(pow(0,0) == 1);
  static_assert(pow(2,-1) == 0.5);
  static_assert(pow(2,16) == 65536);

  inline constexpr float80 exp2(const float80 x) noexcept {
    return std::exp2(x);
  }
  static_assert(exp2(64) == pow(2,64));
  static_assert(exp2(-1) == 0.5);

  inline constexpr float80 ldexp(const float80 x, const int exp) noexcept {
    return std::ldexp(x, exp);
  }
  static_assert(ldexp(1,-1000) == pow(2,-1000));

  inline constexpr float80 sq(const float80 x) noexcept {
    return x*x;
  }
  static_assert(sq(0) == 0);
  static_assert(sq(1) == 1);
  static_assert(sq(2) == 4);

  inline constexpr float80 sqrt(const float80 x) noexcept {
    return std::sqrt(x);
  }
  static_assert(sqrt(0) == 0);
  static_assert(sqrt(1) == 1);
  static_assert(sqrt(4) == 2);
  static_assert(sqrt(3*3+4*4) == 5);
  constexpr float80 Sqr2 = 1.4142135623730950488016887242L;
  static_assert(Sqr2 == sqrt(2));
  // static_assert(isnan(sqrt(-1))); // bug with gcc 8.3

  constexpr float80 golden_ratio = 1.6180339887498948482045868L;
  static_assert(golden_ratio == (1+sqrt(5))/2);
  static_assert(fma(golden_ratio,golden_ratio,-golden_ratio) == 1);
  constexpr float80 log_golden_ratio = 0.4812118250596034474977589L;
  static_assert(log_golden_ratio == log(golden_ratio));
  static_assert(exp(log_golden_ratio) == golden_ratio);

  inline constexpr float80 cbrt(const float80 x) noexcept {
    return std::cbrt(x);
  }
  static_assert(cbrt(27) == 3);
  static_assert(cbrt(1e3) == 1e1L);

  inline constexpr float80 abs(const float80 x) noexcept {
    return std::fabs(x);
  }
  static_assert(abs(sq(Sqr2) - 2) < 2*epsilon);
  static_assert(abs(log(sqrt(2)) - Log2/2) < epsilon);

  inline constexpr float80 round(const float80 x) noexcept {
    return std::round(x);
  }
  static_assert(round(0.4) == 0);
  static_assert(round(0.5) == 1);
  static_assert(round(0.6) == 1);
  static_assert(round(1.5) == 2);
  static_assert(round(2.5) == 3);
  static_assert(round(-0.5) == -1);
  static_assert(round(-1.5) == -2);

  inline constexpr float80 floor(const float80 x) noexcept {
    return std::floor(x);
  }
  static_assert(floor(0.0) == 0);
  static_assert(floor(0.1) == 0);
  static_assert(floor(0.9) == 0);
  static_assert(floor(1) == 1);
  static_assert(floor(-0.1) == -1);
  static_assert(floor(-1) == -1);

  inline constexpr float80 trunc(const float80 x) noexcept {
    return std::trunc(x);
  }
  static_assert(trunc(0.0) == 0);
  static_assert(trunc(0.1) == 0);
  static_assert(trunc(0.9) == 0);
  static_assert(trunc(1) == 1);
  static_assert(trunc(-0.1) == 0);
  static_assert(trunc(-0.9) == 0);
  static_assert(trunc(-1) == -1);

  inline constexpr float80 ceil(const float80 x) noexcept {
    return std::ceil(x);
  }
  static_assert(ceil(0.0) == 0);
  static_assert(ceil(0.1) == 1);
  static_assert(ceil(0.9) == 1);
  static_assert(ceil(1) == 1);
  static_assert(ceil(-0.1) == 0);
  static_assert(ceil(-0.9) == 0);
  static_assert(ceil(-1) == -1);

  // For completeness (seems missing in standard library):
  inline constexpr float80 antitrunc(const float80 x) noexcept {
    return (x >= 0) ? ceil(x) : floor(x);
  }
  static_assert(antitrunc(0.0) == 0);
  static_assert(antitrunc(0.1) == 1);
  static_assert(antitrunc(0.9) == 1);
  static_assert(antitrunc(1) == 1);
  static_assert(antitrunc(-0.1) == -1);
  static_assert(antitrunc(-0.9) == -1);
  static_assert(antitrunc(-1) == -1);

  inline constexpr float80 erf(const float80 x) noexcept {
    return std::erf(x);
  }
  static_assert(erf(0) == 0);
  static_assert(erf(1) == 0.8427007929497148693412L);
  static_assert(erf(-1) == -0.8427007929497148693412L);

  inline constexpr float80 erfc(const float80 x) noexcept {
    return std::erfc(x);
  }
  static_assert(erfc(0) == 1);
  static_assert(erfc(1) == 0.15729920705028513066L);
  static_assert(erfc(-1) == 1.8427007929497148693L);


  /* Connection with integral types */

  // float80 fully includes 64-bit integer arithmetic:
  typedef std::uint64_t UInt_t;
  typedef std::uint32_t uint_t;

  static_assert(0.3e1L == 3L);

  constexpr UInt_t P264m1 = std::numeric_limits<UInt_t>::max();
  static_assert(P264m1 == UInt_t(-1));
  static_assert(P264m1 == pow(2,64) - 1);
  static_assert(P264m1 == UInt_t(1.8446744073709551615e19L));
  static_assert(P264m1 + 1 == 0);
  static_assert(UInt_t(float80(P264m1)) == P264m1);
  constexpr uint_t P232m1 = std::numeric_limits<uint_t>::max();
  static_assert(P232m1 == pow(2,32) - 1);
  static_assert(P232m1 + 1 == 0);
  static_assert(UInt_t(P232m1)*P232m1 == pow(2,64) - pow(2,33) + 1);
  static_assert(UInt_t(P232m1)*P232m1 == P264m1 - 2*(UInt_t(P232m1)+1) + 2);
  static_assert(ldexp(ldexp(P264m1,10000),-10000) == P264m1);
  static_assert(ldexp(ldexp(P264m1,-10000),10000) == P264m1);

  constexpr float80 P264 = 18446744073709551616.0L;
  static_assert(P264 == 1.8446744073709551616e19L);
  static_assert(P264 == pow(2,64));
  static_assert(sqrt(P264) == pow(2,32));
  static_assert(sqrt(sqrt(P264)) == pow(2,16));
  static_assert(-(-P264) == P264);

  // Exactly the integers in the interval [-P264, +P264] are exactly represented by float80:
  static_assert(P264-1 == P264m1);
  static_assert(-P264 == -float80(P264m1) - 1);
  static_assert(P264+1 == P264);
  static_assert(-P264-1 == -P264);


  /* Computations related to the factorial function */

  inline constexpr float80 factorial(const uint_t N) noexcept {
    float80 prod = 1;
    for (uint_t i = 1; i < N; ++i) prod *= i+1;
    return prod;
  }
  static_assert(factorial(0) == 1);
  static_assert(factorial(20) == 2432902008176640000ULL);
  static_assert(factorial(1754) < pinfinity);
  inline constexpr float80 lfactorial(const UInt_t N) noexcept {
    float80 sum = 0;
    for (UInt_t i = 1; i < N; ++i) sum += log1p(i);
    return sum;
  }
  static_assert(lfactorial(0) == 0);
  static_assert(lfactorial(1) == 0);
  static_assert(lfactorial(2) == Log2);
  static_assert(exp(lfactorial(10)) == factorial(10));
  static_assert(round(exp(lfactorial(19))) == factorial(19));

  // The Stirling approximation:

  constexpr float80 pi = 3.141592653589793238462643383279502884L;
  static_assert(pi == std::acos(float80(-1)));
  static_assert(std::cos(pi) == -1);
  static_assert(abs(std::sin(pi)) < epsilon);

  constexpr float80 Stirling_factor = 2.506628274631000502415765L;
  static_assert(Stirling_factor == sqrt(2*pi));
  inline constexpr float80 Sfactorial(const uint_t N) noexcept {
    return Stirling_factor * sqrt(N) * pow(N/euler,N);
  }
  static_assert(Sfactorial(0) == 0);
  static_assert(Sfactorial(1) == Stirling_factor/euler);
  static_assert(Sfactorial(1754) < factorial(1754));
  static_assert(factorial(1754) / Sfactorial(1754) < 1.00005);
  constexpr float80 lStirling_factor = 0.91893853320467274178032973640561763986L;
  static_assert(lStirling_factor == log(Stirling_factor));
  inline constexpr float80 lSfactorial(const UInt_t N) noexcept {
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
  inline constexpr float80 lambertW0l_lb(const float80 l) noexcept {
    assert(l >= 1);
    const float80 ll = log(l);
    return fma(ll/l, 0.5L, l-ll);
  }
  static_assert(lambertW0l_lb(1) == 1);
  static_assert(abs(lambertW0l_lb(euler) - fma(1/euler, 0.5L, eulerm1)) < 2*epsilon);
  inline constexpr float80 lambertW0_lb(const float80 x) noexcept {
    assert(x >= euler);
    return lambertW0l_lb(log(x));
  }
  static_assert(lambertW0_lb(euler) == 1);

  inline constexpr double lambertW0l_lb_d(const double l) noexcept {
    assert(l >= 1);
    const double ll = std::log(l);
    return std::fma(ll/l, 0.5, l-ll);
  }
  static_assert(lambertW0l_lb_d(1) == 1);

  /* The upper bound for W(x), again first taking log(x) as argument: */
 inline constexpr float80 lambertW0l_ub(const float80 l) noexcept {
    assert(l >= 1);
    const float80 ll = log(l);
    return fma(ll/l, euler/eulerm1, l-ll);
  }
  static_assert(lambertW0l_ub(1) == 1);
  static_assert(lambertW0l_ub(euler) == 1/eulerm1 + eulerm1);
  inline constexpr float80 lambertW0_ub(const float80 x) noexcept {
    assert(x >= euler);
    return lambertW0l_ub(log(x));
  }
  static_assert(lambertW0_ub(euler) == 1);

  constexpr float80 euler_mascheroni = 0.57721566490153286060651209L;


  /* Conversion functions */

  /* Converting float80 to UInt_t for x >= 0, using rounding, except
     x is too big, in which case P264m1 is returned:
  */
  inline constexpr UInt_t toUInt(const float80 x) noexcept {
    assert(x >= 0);
    if (x == pinfinity) return P264m1;
    else return round(min(x, P264m1));
  }
  static_assert(toUInt(0) == 0);
  static_assert(toUInt(0.5) == 1);
  static_assert(toUInt(exp2(32)) == UInt_t(P232m1) + 1);
  static_assert(toUInt(P264) == P264m1);
  static_assert(toUInt((P264m1-1) + 0.5000000000000000001L) == P264m1);
  static_assert(toUInt(pinfinity) == P264m1);

  inline float80 stold(const std::string& s) {
    return std::stold(s);
  }

  // Succeeds for every s convertible to float80, interpreting negative x
  // as zero, and too big x as the maximal value:
  inline UInt_t toUInt(const std::string& s) {
    const float80 x = FloatingPoint::stold(s);
    if (not (x >= 0)) return 0;
    else return toUInt(x);
  }

}

#endif
