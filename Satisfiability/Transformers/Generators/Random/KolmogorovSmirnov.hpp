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

  Kolmogorov-Smirnov test

  https://www.itl.nist.gov/div898/handbook/eda/section3/eda35g.htm

  The D-value lies in [0,1], the greater the larger the distance from
  the uniform distribution (depending on the size of the vector).

  Our main interest is in ks_P(x), which is the "complemented cdf":
  the probability that for a vector x of values from [0,1] these are
  independent and uniform random values.


TODOS:

0. The Marsaglia-et-al implementation and algorithm has been superseeded by
   https://www.jstatsoft.org/article/view/v039i11
   Computing the Two-Sided Kolmogorov-Smirnov Distribution
   Richard Simard, Pierre L'Ecuyer (2011)

   Software available at
   http://www.iro.umontreal.ca/~simardr/ksdir

   This is available here as
   KolmogorovSmirnovDist.c, KolmogorovSmirnovDist.h.
   This needs to be integrated; it includes the Marsaglia-et-al implementation,
   but uses other algorithms where that is very slow or numerically weak.

     ks_P(, d) = KSfbar (n, d).


1. The literature always assumes that ties in x do not exist (for
   function ks_D_value):
    - How to overcome this?
    - Is the current treatment appropriate?
    - https://openaccess.city.ac.uk/id/eprint/18541/8/Dimitrova%252C%20Kaishev%252C%20Tan%20%25282017%2529%20KSr1.pdf
      Computing the Kolmogorov-Smirnov Distribution when the Underlying Cdf is
      Purely Discrete, Mixed or Continuous (2017)
      Dimitrina S. Dimitrova, Vladimir K. Kaishev, Senren Tan

      seems to provide a solution handling also the discrete case.
      There is a C++ implementation, however much more complicated than ours.
      Their Table 6 lists values of ks.test for given n and "D_n=d_n",
      however the numerical values differ from ours.
      Our function should be the one discussed in Subsection 3.3. of the
      paper.

  Todos related to the special algorithms are below.

*/

#ifndef KolmogorovSmirnov_vNEAv5poFe
#define KolmogorovSmirnov_vNEAv5poFe

#include <vector>
#include <algorithm>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>

namespace KolSmir {

  namespace FP = FloatingPoint;


  /* The Marsaglia-Tsang-Wong implementation

     Transferred from k.c, based on
     http://dx.doi.org/10.18637/jss.v008.i18
     George Marsaglia, Wai Wan Tsang, Jingbo Wang
     Evaluating Kolmogorov's Distribution (2003)

     This computes the "uncomplemented" distribution.

     TODOS:

     0. The implementation of Simard and Ecuyer includes this implementation,
     with some "small changes".
       - We need to compare this with our adaptation.

     1. Comparison with values given in tables from [Dimitrova, Kaishev, Tan]:

      Appendix B: B. Computing the cdf of D n when F (x) is continuous:
      numerical analysis and comparisons
      The values tabulated in Table 8 of the paper are in principle
      = 1 - ks_P(n, 1/n), however since theses values are very small, from
      n=60 on our values are (necessarily) = 0 (we aren't interested in
      these values, since only small ks_P-values of interest for us).
      The values from Table 9, 10, 11, 12, 13 are reproducible with
      full precision (always using 1 - ks_P).
      For Table 14 there is less precision for n=10^5.
      Similar for Table 15 with n >= 10^4, while Table 16 is mostly precise
      (but takes a long time).

      Appendix C. Computing the complementary cdf when F(x) is continuous:
      numerical analysis and comparisons
      This now is exactly our function ks_P.
      Values in Table 18, 19, 20 are reproduced exactly.
      For Table 21 there is a smallest discrepancy in the last digit (unclear
      what is the precise result).
      For Table 22 slightly bigger discrepancies (Carvalho seems most
      correct).

      2. The original value of ks_scaling_exp was 140, now 1000, due to using
      float80 -- is this appropriate?
      - Also using FloatingPoint::ldexp seems better for doing the scaling.
      (then by powers of 10).

      3. DONE (not really correct: the values for D<0.5, though
      "unobservable", are different from the literature -- and for d=0 the
      p-value should be 1)
      The handling of n=1 simulates the values computed by R: check this.

  */

  typedef std::vector<FP::float80> fvec_t;

  namespace detail {

    void ks_mMultiply(const fvec_t& A, const fvec_t& B, fvec_t& C, const FP::UInt_t m) noexcept {
      for (FP::UInt_t i=0; i<m; ++i) {
        const FP::UInt_t im = i*m;
        for (FP::UInt_t j=0; j<m; ++j) {
          FloatingPoint::float80 s=0;
          for (FP::UInt_t k=0; k<m; ++k) s += A[im+k] * B[k*m+j];
          C[im+j]=s;
        }
      }
    }

    constexpr FP::UInt_t ks_scaling_exp = 1000;
    constexpr FloatingPoint::float80 ks_too_big = FloatingPoint::pow(10,ks_scaling_exp);
    static_assert(ks_too_big == 1e1000L);
    constexpr FloatingPoint::float80 ks_scaling_factor = 1/ks_too_big;
    static_assert(ks_scaling_factor == 1e-1000L);

    void ks_mPower(const fvec_t& A, const FP::UInt_t eA, fvec_t& V, FP::UInt_t& eV, const FP::UInt_t m, const FP::UInt_t n) {
      if(n == 1) {
        for (FP::UInt_t i = 0; i < m*m; ++i) V[i]=A[i];
        eV = eA;
        return;
      }

      ks_mPower(A, eA, V, eV, m, n/2);
      fvec_t B(m*m);
      ks_mMultiply(V, V, B, m);
      FP::UInt_t eB = 2*eV;
      if (n % 2 == 0) {
        for (FP::UInt_t i=0; i < m*m; ++i) V[i]=B[i];
        eV = eB;
      }
      else {
        ks_mMultiply(A, B, V, m);
        eV = eA+eB;
      }
      if (V[(m/2)*m + m/2] > ks_too_big) {
        for (FP::UInt_t i=0; i < m*m; ++i) V[i] *= ks_scaling_factor;
        eV += ks_scaling_exp;
      }
    }

  } // end namespace detail

  FloatingPoint::float80 ks_mtw(const FP::UInt_t n, const FloatingPoint::float80 d) {
    assert(n >= 1);
    assert(d >= 0 and d <= 1);
    if (n == 1)
      if (d <= 0.5L) return 0;
      else return 2*(d-0.5L);
    if (d == 0) return 0;
    using FloatingPoint::float80;

  //OMIT NEXT LINE IF YOU REQUIRE >7 DIGIT ACCURACY IN THE RIGHT TAIL
  // s=d*d*n; if(s>7.24||(s>3.76&&n>99)) return 1-2*exp(-(2.000071+.331/sqrt(n)+1.409/n)*s);

    const FP::UInt_t k = FP::UInt_t(n*d) + 1;
    assert(k >= 1);
    const FP::UInt_t m = 2*k - 1;
    assert(m >= 1);
    const float80 h = k - n*d;

    fvec_t H(m*m), Q(m*m);

    for (FP::UInt_t i=0; i<m; ++i)
      for (FP::UInt_t j=0; j <= std::min(i+1, m-1); ++j)
        H[i*m+j] = 1;
    for (FP::UInt_t i=0; i<m; ++i) {
      H[i*m] -= FloatingPoint::pow(h,i+1);
      H[(m-1)*m+i] -= FloatingPoint::pow(h,m-i);
    }
    H[(m-1)*m] += 2*h-1 > 0 ? FloatingPoint::pow(2*h-1,m) : 0;
    for (FP::UInt_t i=0; i<m; ++i)
      for (FP::UInt_t j=0; j < std::min(i+1, m); ++j)
        for (FP::UInt_t g=1; g <= (i+1)-j; ++g) H[i*m+j] /= g;

    const FP::UInt_t eH = 0;
    FP::UInt_t eQ;
    detail::ks_mPower(H,eH,  Q,eQ,  m,n);

    float80 s = Q[(k-1)*m+k-1];
    for (FP::UInt_t i = 1; i <= n; ++i) {
      s *= float80(i)/n;
      if (s < detail::ks_scaling_factor) {
        s *= detail::ks_too_big;
        eQ -= detail::ks_scaling_exp;
      }
    }
    s *= FloatingPoint::pow(10, eQ);
    return s;
  }



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
      - Pomeranz: We tried
         - eps = 1.0e-16L (originally EPS = 1.0e-15)
         - eno = 1000 (originally ENO = 350)
         - minsum < 1.0e-900L (originally "minsum < 1.0e-280")
         which was reset due to assertion-failure.
      - cdfSpecial: the magical constant "n * x * x >= 18" was left unchanged.
      - fbarSpecial: thw two magical constants "w >= 370.0" and
        "w <= 0.0274" were left unchanged.

      These constants should be updated, so that we use the higher precision
      of float80.

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

  typedef std::vector<fvec_t> matrix_t;

  matrix_t create_matrix(const FP::UInt_t N, const FP::UInt_t M) {
    return matrix_t{N, fvec_t(M)};
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
         fvec_t& A,               // A_i
         fvec_t& Atflo,           // floor (A_i - t)
         fvec_t& Atcei            // ceiling (A_i + t)
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
    const FP::float80 eps = 1.0e-15L;
    const FP::UInt_t eno = 350;

    const FP::float80 reno = FP::ldexp(1, eno); // for renormalization of V
    const FP::float80 t = n * x;

    fvec_t A(2*n+3), Atflo(2*n+3), Atcei(2*n+3);
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
    {const FP::float80 w = (1 - 2 * A[2]) / n;
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

      if (minsum < 1.0e-280L) {
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
    if (w >= 370) return 0; // needs update for float80 XXX
    if (w <= 0.0274L) return 1; // needs update for float80 XXX
    if (x <= 1.0L / n) {
      const FP::float80 t = 2 * x * n - 1;
      if (n <= nexact) return 1 - rapfac(n) * FP::pow(t, n);
      return 1 - FP::exp(getLogFactorial(n) + n * FP::log(t / n));
    }
    if (x >= 1.0 - 1.0L / n) return 2 * FP::pow(1 - x, n);
    return -1;
  }


  FP::float80 KSfbar(FP::UInt_t, FP::float80);
  FP::float80 KScdf(const FP::UInt_t n, const FP::float80 x) {
    const FP::float80 w = n * x * x;
    {const FP::float80 u = cdfSpecial(n, x); if (u >= 0) return u;}
    if (n <= nexact) {
      if (w < 0.754693L) return ks_mtw(n, x);
      if (w < 4) return Pomeranz(n, x);
      return 1 - KSfbar (n, x);
    }
    if (w * x * n <= 7 and n <= nkolmo) return ks_mtw(n, x);
    return Pelz (n, x);
  }
  FP::float80 KSfbar(const FP::UInt_t n, const FP::float80 x) {
    const FP::float80 w = n * x * x;
    {const FP::float80 v = fbarSpecial(n, x); if (v >= 0) return v;}
    if (n <= nexact) {
      if (w < 4) return 1 - KScdf(n, x);
      else return 2 * KSPlusbarUpper(n, x);
    }
    if (w >= 2.65L) return 2 * KSPlusbarUpper(n, x);
    return 1 - KScdf (n, x);
  }

}

#endif
