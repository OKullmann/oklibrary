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

  /* The probability of the KS+ distribution in the upper tail using
     Smirnov's stable formula: */
  constexpr FP::float80 KSPlusbarUpper(const FP::UInt_t n, const FP::float80 x) noexcept {
    if (n > 200000) return KSPlusbarAsymp(n, x);
    assert(x >= 0 and x <= 1);
    const FP::float80 epsilon = 1.0e-13L;
    FP::float80 Sum = 0;
    FP::UInt_t jmax = n * (1 - x);
    if (1.0L - x - FP::float80(jmax)/n <= 0)
      --jmax; // avoid log(0) for j = jmax and q ~ 1.0
    const FP::UInt_t jdiv = n > 3000 ? 2 : 3;
    FP::UInt_t j = jmax / jdiv + 1;
    const FP::float80 logjmax = getLogFactorial(n) - getLogFactorial(j)
      - getLogFactorial (n-j);

    for (FP::float80 LogCom = logjmax; j <= jmax; ++j) {
      const FP::float80 q = FP::float80(j) / n + x;
      const FP::float80 term = LogCom + (j-1)*FP::log(q) + (n-j)*FP::log1p(-q);
      const FP::float80 t = FP::exp(term);
      Sum += t;
      LogCom += FP::log(FP::float80(n - j) / (j + 1));
      if (t <= Sum * epsilon) break;
    }

    j = jmax / jdiv;
    for (FP::float80 LogCom = logjmax + FP::log(FP::float80(j + 1) / (n - j));
         j > 0; --j) {
      const FP::float80 q = FP::float80(j) / n + x;
      const FP::float80 term = LogCom + (j-1)*FP::log(q) + (n-j)*FP::log1p(-q);
      const FP::float80 t = FP::exp(term);
      Sum += t;
      LogCom += FP::log(FP::float80(j) / (n - j + 1));
      if (t <= Sum * epsilon) break;
    }

    Sum *= x;
    Sum += FP::exp(n * FP::log1p(-x)); // add the term j = 0
    return Sum;
  }

   /* Approximating the Lower Tail-Areas of the Kolmogorov-Smirnov One-Sample
      Statistic, according to
        Wolfgang Pelz and I. J. Good,
        Journal of the Royal Statistical Society, Series B.
        Vol. 38, No. 2 (1976), pp. 152-156:
   */
  constexpr FP::float80 Pelz(const FP::UInt_t n, const FP::float80 x) noexcept {
    constexpr FP::UInt_t jmax = 20;
    constexpr FP::float80 eps = 1.0e-11L;
    constexpr FP::float80 C = FP::sqrt(2*FP::pi);
    constexpr FP::float80 C2 = FP::sqrt(FP::pi/2);
    constexpr FP::float80 pi2 = FP::pi * FP::pi;
    constexpr FP::float80 pi4 = pi2 * pi2;

    const FP::float80 racn = FP::sqrt(n);
    const FP::float80 z = racn * x;
    const FP::float80 z2 = z * z;
    const FP::float80 z4 = z2 * z2;
    const FP::float80 z6 = z4 * z2;
    const FP::float80 w = pi2 / (2 * z2);

    FP::float80 sum = 0;

    {FP::float80 term = 1;
     for (FP::UInt_t j=0; j <= jmax and term > eps * sum; ++j) {
       const FP::float80 ti = j + 0.5L;
       term = FP::exp(-ti * ti * w);
       sum += term;
     }
     sum *= C / z;
    }

    {FP::float80 term = 1;
     FP::float80 tom = 0;
     for (FP::UInt_t j=0; j <= jmax and FP::abs(term) > eps*FP::abs(tom); ++j) {
       const FP::float80 ti = j + 0.5L;
       term = (pi2 * ti * ti - z2) * FP::exp(-ti * ti * w);
       tom += term;
     }
     sum += tom * C2 / (racn * 3 * z4);
    }

    {FP::float80 term = 1;
     FP::float80 tom = 0;
     for (FP::UInt_t j=0; j <= jmax and FP::abs(term) > eps*FP::abs(tom); ++j) {
       const FP::float80 ti = j + 0.5L;
       term = 6 * z6 + 2 * z4 + pi2 * (2 * z4 - 5 * z2) * ti * ti +
         pi4 * (1 - 2 * z2) * ti * ti * ti * ti;
       term *= FP::exp(-ti * ti * w);
       tom += term;
     }
     sum += tom * C2 / (n * 36 * z * z6);
    }

    {FP::float80 term = 1;
     FP::float80 tom = 0;
     for (FP::UInt_t j = 1; j <= jmax and term > eps * tom; ++j) {
       const FP::float80 ti = j;
       term = pi2 * ti * ti * FP::exp(-ti * ti * w);
       tom += term;
     }
     sum -= tom * C2 / (n * 18 * z * z2);
    }

    {FP::float80 term = 1;
     FP::float80 tom = 0;
     for (FP::UInt_t j=0; j <= jmax and FP::abs(term) > eps*FP::abs(tom); ++j) {
       FP::float80 ti = j + 0.5L;
       ti *= ti;
       term = -30 * z6 - 90 * z6 * z2 + pi2 * (135 * z4 - 96 * z6) * ti +
         pi4 * (212 * z4 - 60 * z2) * ti * ti + pi2 * pi4 * ti * ti * ti *
           (5 - 30 * z2);
       term *= FP::exp(-ti * w);
       tom += term;
     }
     sum += tom * C2 / (racn * n * 3240 * z4 * z6);
    }

    {FP::float80 term = 1;
     FP::float80 tom = 0;
     for (FP::UInt_t j=1; j <= jmax and FP::abs(term) > eps*FP::abs(tom); ++j) {
       const FP::float80 ti = FP::float80(j) * j;
       term = (3 * pi2 * ti * z2 - pi4 * ti * ti) * FP::exp(-ti * w);
       tom += term;
     }
     sum += tom * C2 / (racn * n * 108 * z6);
    }

    return sum;
  }

}

#endif
