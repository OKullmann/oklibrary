// Oliver Kullmann, 9.11.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Adaptation of
   - k.c
   - KolmogorovSmirnovDist.h/c

  for C++ and float80.

  See todos below.

*/

#ifndef KolmogorovSmirnov_vNEAv5poFe
#define KolmogorovSmirnov_vNEAv5poFe

#include <vector>
#include <algorithm>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>

namespace KolSmir {

  namespace FP = FloatingPoint;


  /* The Simard-Ecuyer implementation

     Transferred from KolmogorovSmirnovDist.h/c.

     TODOS:

     1. Specical constants and their adaptation:

      - nexact = 1000 was originally  NEXACT=500.
      - nkolmo = 1000000 was originally NKOLMO=100000.
      - mfact = 1000 was originally MFACT=30.
      - getLogFactorial(n) originally used for n>MFACT an approximation.
      - KSPlusbarUpper:
         - epsilon = 1.0e-13L was originally EPSILON = 1.0E-12
         - there are two magical constants, which were left unchanged:
           200000, 3000.
      - Pelz:
         - jmax = 22 was originally JMAX = 20
         - eps = 1.0e-11L was originally EPS = 1.0e-10.
      - Pomeranz:
         - eps = 1.0e-16L was originally EPS = 1.0e-15
         - eno = 1000 was originally ENO = 350
         - there is one magical constant "minsum < 1.0e-900L", whas was
           originally "minsum < 1.0e-280".
      - cdfSpecial: the magical constant "n * x * x >= 18" was left unchanged.
      - fbarSpecial: thw two magical constants "w >= 370.0" and
        "w <= 0.0274" were left unchanged.

  */

  /* For x close to 0 or 1, we use the exact formulae of Ruben-Gambino in all
     cases. For n <= NEXACT, we use exact algorithms: the Durbin matrix and
     the Pomeranz algorithms. For n > NEXACT, we use asymptotic methods
     except for x close to 0 where we still use the method of Durbin
     for n <= NKOLMO. For n > NKOLMO, we use asymptotic methods only and
     so the precision is less for x close to 0.
     We could increase the limit NKOLMO to 10^6 to get better precision
     for x close to 0, but at the price of a slower speed.
  */
  constexpr FP::UInt_t nexact = 1000;
  constexpr FP::UInt_t nkolmo = 1000000;

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
    constexpr FP::UInt_t jmax = 22;
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


  /* Precompute A_i, floors, and ceilings for limits of sums in the Pomeranz
     algorithm: */
  inline void CalcFloorCeil (
         const FP::UInt_t n,     // sample size
         const FP::float80 t,    // = nx
         row_t& A,               // A_i
         row_t& Atflo,           // floor (A_i - t)
         row_t& Atcei            // ceiling (A_i + t)
  ) {
    assert(t >= 0);
    assert(A.size() == 2*n+3);
    assert(Atflo.size() == A.size() and Atcei.size() == A.size());
    const FP::UInt_t last = 2*n + 2;
    assert(t < FP::P264m1);
    const FP::UInt_t ell = t;         // floor(t)
    const FP::float80 z = t - ell;    // t - floor(t)
    const FP::float80 w = FP::ceil(t) - t;

    const auto h = [](const FP::UInt_t n){return FP::float80(n/2);};
    if (z > 0.5) {
      for (FP::UInt_t i = 2; i <= last; i += 2) Atflo[i] = h(i)-2-ell;
      for (FP::UInt_t i = 1; i <= last; i += 2) Atflo[i] = h(i)-1-ell;
      for (FP::UInt_t i = 2; i <= last; i += 2) Atcei[i] = i/2+ell;
      for (FP::UInt_t i = 1; i <= last; i += 2) Atcei[i] = i/2+1+ell;
    } else if (z > 0.0) {
      for (FP::UInt_t i = 1; i <= last; ++i) Atflo[i] = h(i)-1-ell;
      for (FP::UInt_t i = 2; i <= last; ++i) Atcei[i] = i/2+ell;
      Atcei[1] = 1 + ell;
    } else {
      assert(z == 0);
      for (FP::UInt_t i = 2; i <= last; i += 2) Atflo[i] = h(i)-1-ell;
      for (FP::UInt_t i = 1; i <= last; i += 2) Atflo[i] = h(i)-ell;
      for (FP::UInt_t i = 2; i <= last; i += 2) Atcei[i] = i/2-1+ell;
      for (FP::UInt_t i = 1; i <= last; i += 2) Atcei[i] = i/2+ell;
    }

    A[0] = A[1] = 0; A[2] = w < z ? w : z; A[3] = 1 - A[2];
    for (FP::UInt_t i = 4; i <= last-1; ++i) A[i] = A[i - 2] + 1;
    A[last] = n;
  }


  /* The Pomeranz algorithm to compute the KS distribution */
  FP::float80 Pomeranz(const FP::UInt_t n, const FP::float80 x) noexcept {
    const FP::float80 eps = 1.0e-16L;
    const FP::UInt_t eno = 1000;

    const FP::float80 reno = FP::ldexp(1.0, eno); // for renormalization of V
    const FP::float80 t = n * x;

    row_t A(2*n+3), Atflo(2*n+3), Atcei(2*n+3);
    matrix_t V = create_matrix(2,n+2);
    matrix_t H = create_matrix(4,n+2); // = pow(w, j) / Factorial(j)

    CalcFloorCeil(n, t, A, Atflo, Atcei);

    V[1][1] = reno;
    FP::UInt_t coreno = 1; // counter: how many renormalizations

    // Precompute H[][] = (A[j] - A[j-1]^k / k! for speed:
    H[0][0] = 1;
    {const FP::float80 w = 2 * A[2] / n;
     for (FP::UInt_t j = 1; j <= n + 1; ++j) H[0][j] = w * H[0][j - 1] / j;}
    H[1][0] = 1;
    {const FP::float80 w = (1.0L - 2 * A[2]) / n;
     for (FP::UInt_t j = 1; j <= n + 1; ++j) H[1][j] = w * H[1][j - 1] / j;}
    H[2][0] = 1;
    {const FP::float80 w = A[2] / n;
     for (FP::UInt_t j = 1; j <= n + 1; ++j) H[2][j] = w * H[2][j - 1] / j;}
    H[3][0] = 1;
    for (FP::UInt_t j = 1; j <= n + 1; ++j) H[3][j] = 0;

    FP::UInt_t r2 = 1;
    for (FP::UInt_t i = 2, r1 = 0; i <= 2 * n + 2; ++i) {
      const FP::UInt_t jlow = Atflo[i] <= -1 ? 1 :
        Atflo[i] < 1 ? 2 : 2 + FP::UInt_t(Atflo[i]);
      const FP::UInt_t jup = std::min(FP::UInt_t(Atcei[i]), n+1);
      const FP::UInt_t klow = Atflo[i - 1] <= -1 ? 1 :
        Atflo[i-1] < 1 ? 2 : 2 + FP::UInt_t(Atflo[i-1]);
      const FP::UInt_t kup0 = Atcei[i - 1];

      FP::UInt_t s = 0;
      {const FP::float80 w = (A[i] - A[i - 1]) / n;
       for (; s < 4; ++s) if (FP::abs(w - H[s][1]) <= eps) break;
       assert(s < 4);
      }

      FP::float80 minsum = reno;
      r1 = 1 - r1; r2 = 1 - r2;

      for (FP::UInt_t j = jlow; j <= jup; ++j) {
        const FP::UInt_t kup = std::min(kup0, j);
        FP::float80 sum = 0;
        for (FP::UInt_t k = kup; k >= klow; --k) sum += V[r1][k] * H[s][j - k];
        V[r2][j] = sum;
        if (sum < minsum) minsum = sum;
      }

      if (minsum < 1.0e-900L) {
        // V is too small: renormalize to avoid underflow of probabilities
        for (FP::UInt_t j = jlow; j <= jup; ++j) V[r2][j] *= reno;
        ++coreno; // keep track of log of reno
      }
    }

    const FP::float80 sum = V[r2][n + 1];
    const FP::float80 w =
      getLogFactorial(n) - coreno*eno*FP::Log2 + FP::log(sum);
    if (w >= 0) return 1;
    return FP::exp(w);
  }


  /* The KS distribution is known exactly for these cases: */
  inline constexpr FP::float80 cdfSpecial(const FP::UInt_t n, const FP::float80 x) noexcept {
    assert(n >= 1);
    assert(x >= 0);
    if (x >= 1) return 1;
    if (x <= 0.5L / n) return 0;
    if (n == 1) return 2 * x - 1;
    /* For nx^2 > 18, KSfbar(n, x) is smaller than 5e-16: */
    if (n * x * x >= 18) return 1; // needs update for float80 XXX
    if (x <= 1.0L / n) {
      const FP::float80 t = 2 * x * n - 1;
      if (n <= nexact) return rapfac(n) * FP::pow(t, n);
      return FP::exp(getLogFactorial(n) + n * FP::log(t / n));
    }
    if (x >= 1 - 1.0L / n) return 1 - 2 * FP::pow(1 - x, n);
    return -1;
  }
  inline constexpr FP::float80 fbarSpecial(const FP::UInt_t n, const FP::float80 x) noexcept {
    assert(n >= 1);
    assert(x >= 0);
    if (x >= 1) return 0;
    if (x <= 0.5L / n) return 1;
    if (n == 1) return 2 - 2 * x;
    const FP::float80 w = n * x * x;
    if (w >= 370.0) return 0; // needs update for float80 XXX
    if (w <= 0.0274) return 1; // needs update for float80 XXX
    if (x <= 1.0L / n) {
      const FP::float80 t = 2 * x * n - 1;
      if (n <= nexact) return 1 - rapfac(n) * FP::pow(t, n);
      return 1 - FP::exp(getLogFactorial(n) + n * FP::log(t / n));
    }
    if (x >= 1.0 - 1.0L / n) return 2 * FP::pow(1 - x, n);
    return -1;
  }

}

#endif
