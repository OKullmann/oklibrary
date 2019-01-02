// Oliver Kullmann, 2.1.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for floating-point computations, based on "long double"

  Delivers the fundamental floating-type floating_t and the underlying
  64-bit unsigned UInt_t and the 32-bit unsigned uint_t.
  The functions
    log, exp, sqrt, abs, expm1, pow, round
  are imported, to make sure they work with floating_t.
  The constants
    pinfinity, euler, pi
  of type floating_t are defined.

  Concerning factorial-type functions, we have
    factorial, lfactorial, Sfactorial, lSfactorial.

*/

#ifndef FLOATINGPOINT_rbc6mhfmAG
#define FLOATINGPOINT_rbc6mhfmAG

#include <limits>

#include <cassert>
#include <cmath>
#include <cstdint>

namespace FloatingPoint {

  typedef long double floating_t;
  using limitfloat = std::numeric_limits<floating_t>;
  static_assert(limitfloat::is_iec559);

  constexpr floating_t pinfinity = limitfloat::infinity();
  static_assert(pinfinity > 0);
  static_assert(pinfinity > limitfloat::max());
  static_assert(std::isinf(pinfinity));

  constexpr floating_t epsilon = limitfloat::epsilon();
  static_assert(1 - epsilon < 1);
  static_assert(1 + epsilon > 1);
  static_assert(1 + epsilon/2 == 1);


  inline constexpr floating_t log(const floating_t x) noexcept {
    return std::log(x);
  }
  static_assert(log(1) == 0);
  static_assert(log(4) == 2*log(2));

  inline constexpr floating_t exp(const floating_t x) noexcept {
    return std::exp(x);
  }
  static_assert(exp(0) == 1);
  static_assert(exp(2) == exp(1)*exp(1));
  static_assert(log(exp(1)) == 1);
  constexpr floating_t euler = exp(1);
  static_assert(log(euler) == 1);

  inline constexpr floating_t sqrt(const floating_t x) noexcept {
    return std::sqrt(x);
  }
  static_assert(sqrt(1) == 1);
  static_assert(sqrt(4) == 2);

  inline constexpr floating_t abs(const floating_t x) noexcept {
    return std::abs(x);
  }
  static_assert(abs(sqrt(2)*sqrt(2) - 2) < 2*epsilon);
  static_assert(abs(log(sqrt(2)) - log(2)/2) < epsilon);

  inline constexpr floating_t expm1(const floating_t x) noexcept {
    return std::expm1(x);
  }
  static_assert(expm1(0) == 0);
  static_assert(expm1(1e-1000L) == 1e-1000L);
  static_assert(abs(expm1(1) - (euler - 1)) < 2*epsilon);

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
  static_assert(P264m1 + 1 == 0);
  static_assert(UInt_t(floating_t(P264m1)) == P264m1);
  constexpr floating_t P264 = pow(2,64);
  static_assert(sqrt(P264) == pow(2,32));
  static_assert(sqrt(sqrt(P264)) == pow(2,16));
  // Exactly the integers in the interval [-P264, +P264] are exactly represented by floating_t:
  static_assert(P264-1 == floating_t(P264m1));
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
    for (UInt_t i = 1; i < N; ++i) sum += log(i+1);
    return sum;
  }
  static_assert(lfactorial(0) == 0);
  static_assert(lfactorial(1) == 0);
  static_assert(lfactorial(2) == log(2));
  static_assert(exp(lfactorial(10)) == factorial(10));
  static_assert(round(exp(lfactorial(19))) == factorial(19));

  // The Stirling approximation:

  constexpr floating_t pi = std::acos(floating_t(-1));
  static_assert(std::cos(pi) == -1);
  static_assert(abs(std::sin(pi)) < epsilon);

  inline constexpr floating_t Sfactorial(const uint_t N) noexcept {
    return sqrt(2*pi*N) * pow(N/euler,N);
  }
  static_assert(Sfactorial(0) == 0);
  static_assert(Sfactorial(1) == sqrt(2*pi)/euler);
  static_assert(Sfactorial(1754) < factorial(1754));
  static_assert(factorial(1754) / Sfactorial(1754) < 1.00005);
  constexpr floating_t lStirling_factor = log(2*pi)/2;
  inline constexpr floating_t lSfactorial(const UInt_t N) noexcept {
    assert(N != 0);
    return log(N) * (N + 0.5) - N + lStirling_factor;
  }
  static_assert(lSfactorial(1) == lStirling_factor - 1);
  static_assert(abs(lSfactorial(10) - log(Sfactorial(10))) < epsilon);

}

#endif
