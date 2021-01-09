// Oliver Kullmann, 9.11.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Adaptation of KolmogorovSmirnovDist.h/c for C++ and float80

*/

#ifndef KolmogorovSmirnov_vNEAv5poFe
#define KolmogorovSmirnov_vNEAv5poFe

#include <vector>

#include <Numerics/FloatingPoint.hpp>

namespace KolSmir {

  namespace FP = FloatingPoint;

  /* For x close to 0 or 1, we use the exact formulae of Ruben-Gambino in all
     cases. For n <= NEXACT, we use exact algorithms: the Durbin matrix and
     the Pomeranz algorithms. For n > NEXACT, we use asymptotic methods
     except for x close to 0 where we still use the method of Durbin
     for n <= NKOLMO. For n > NKOLMO, we use asymptotic methods only and
     so the precision is less for x close to 0.
     We could increase the limit NKOLMO to 10^6 to get better precision
     for x close to 0, but at the price of a slower speed.
  */
  constexpr FP::UInt_t nexact = 500;
  constexpr FP::UInt_t nkolmo = 100000;

  constexpr FP::UInt_t mfact = 1000;
  typedef std::array<FP::float80, mfact+1> LnFactorial_t;
  consteval LnFactorial_t init_lf() noexcept {
    LnFactorial_t res;
    res[0] = 0;
    for (FP::UInt_t n = 1; n <= mfact; ++n) res[n] = FP::lfactorial(n);
    return res;
  }
  constexpr LnFactorial_t LnFactorial = init_lf();
  static_assert(LnFactorial[2] == FP::Log2);

  // The original program uses some approximation:
  constexpr FP::float80 getLogFactorial(const FP::UInt_t n) noexcept {
    if (n <= mfact) return LnFactorial[n];
    assert(n < FP::P264m1);
    FP::float80 sum = LnFactorial[mfact];
    for (FP::UInt_t i = mfact+1; i <= n; ++i) sum += FP::log(i);
    return sum;
  }
  static_assert(getLogFactorial(0) == 0);
  static_assert(getLogFactorial(1) == 0);
  static_assert(getLogFactorial(2) == FP::Log2);

  // n! / n^n;
  constexpr FP::float80 rapfac(const FP::UInt_t n) noexcept {
    assert(n >= 1 and n < FP::P264m1);
    FP::float80 res = 1.0L / n;
    for (FP::UInt_t i = 2; i <= n; ++i) res *= FP::float80(i) / n;
    return res;
  }
  static_assert(rapfac(1) == 1);
  static_assert(rapfac(2) == 0.5L);

  typedef std::vector<FP::float80> row_t;
  typedef std::vector<row_t> matrix_t;

  matrix_t create_matrix(const FP::UInt_t N, const FP::UInt_t M) {
    return matrix_t{N, row_t(M)};
  }


  /* The probability of the KS+ distribution using a simple asymptotic
     formula: */
  constexpr FP::float80 KSPlusbarAsymp(const FP::UInt_t n, const FP::float80 x) noexcept {
    assert(x >= 0 and x <= 1);
    const FP::float80 t = 6.0L * n * x + 1;
    const FP::float80 z = t * t / (18.0L * n);
    FP::float80 v = 1.0L - (2 * z * z - 4 * z - 1) / (18.0L * n);
    if (v <= 0) return 0;
    v = v * FP::exp(-z);
    if (v >= 1) return 1;
    return v;
  }

}

#endif
