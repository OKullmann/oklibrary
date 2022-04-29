// Oliver Kullmann, 2.1.2019 (Swansea)
/* Copyright 2019, 2020, 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for floating-point computations, providing float80/64/32

    - harmonic(n), coupcollprob(n)

  Factorial-type functions:

    - factorial(n), lfactorial(n)
    - Sfactorial(n), lSfactorial(n)
    - binomial_coeff(n,k), fbinomial_coeff(n,k), lbinomial_coeff(n,k)
      (computation of all k-subsets of {0,...,n-1} is done in
       Satisfiability/Solvers/Gecode/MOLS/OrthogonalArrays::subsets(n,k))
    - cbinomial_coeff(n), fcbinomial_coeff(n), inv_fcbinomial_coeff(n)
      (central binomial coefficient and inverse).


  Lambert-W:

    - lambertW0l_lb(x), lambertW0_lb(x)
    - lambertW0l_lb_64, lambertW0_lb_64
    - lambertW0l_ub(x), lambertW0_ub(x).


  The following constants of type float80 are defined (only defined for
  float64 if explicitly mentioned):

    - max_binom
    - Stirling_factor (= sqrt(2*pi)), lStirling_factor (= log(2*pi)/2)
    - euler_mascheroni.


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

#include <numeric>

#include <cassert>
#include <cmath>

// Guaranteed to be included:
#include "NumTypes.hpp"
#include "NumBasicFunctions.hpp"
#include "NumPrecise.hpp"
#include "Conversions.hpp"
#include "NumInOut.hpp"


namespace FloatingPoint {

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

  // The central binomial-coefficient "choose n from n/2" for results < 2^64
  // (exact iff n <= max_binom = 67):
  inline constexpr UInt_t cbinomial_coeff(const UInt_t n) noexcept {
    return binomial_coeff(n, n/2);
  }
  static_assert(cbinomial_coeff(0) == 1);
  static_assert(cbinomial_coeff(1) == 1);
  static_assert(cbinomial_coeff(2) == 2);
  static_assert(cbinomial_coeff(3) == 3);
  static_assert(cbinomial_coeff(4) == 6);
  static_assert(cbinomial_coeff(67) == 14226520737620288370ULL);

  /* Now returning a float80:
      - all results < 2^64 are correct (as above)
      - otherwise result >= P264 is guaranteed
      - these larger results are "approximations".
  */
  inline constexpr float80 fcbinomial_coeff(const UInt_t n) noexcept {
    return fbinomial_coeff(n, n/2);
  }
  static_assert(fcbinomial_coeff(67) == 14226520737620288370UL);
  static_assert(fcbinomial_coeff(70) == 112186277816662845432.0L);
  inline constexpr UInt_t inv_fcbinomial_coeff(const UInt_t k) noexcept {
    UInt_t n = 0;
    while (fcbinomial_coeff(n) < k) ++n;
    return n;
  }
  static_assert(inv_fcbinomial_coeff(0) == 0);
  static_assert(inv_fcbinomial_coeff(1) == 0);
  static_assert(inv_fcbinomial_coeff(2) == 2);
  static_assert(inv_fcbinomial_coeff(14226520737620288370UL) == 67);
  static_assert(inv_fcbinomial_coeff(14226520737620288371UL) == 68);


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
