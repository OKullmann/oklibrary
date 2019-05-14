// Oliver Kullmann, 21.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Statistical tests

Background:

erf and relatives:
https://en.wikipedia.org/wiki/Error_function
https://en.wikipedia.org/wiki/Q-function
https://en.wikipedia.org/wiki/68%E2%80%9395%E2%80%9399.7_rule

On tests in general:
https://en.wikipedia.org/wiki/One-_and_two-tailed_tests
https://en.wikipedia.org/wiki/Binomial_proportion_confidence_interval#Clopper–Pearson_interval

Coin flipping:
https://math.stackexchange.com/questions/78237/expected-number-of-runs-in-a-sequence-of-coin-flips
https://stats.stackexchange.com/questions/6770/expected-number-of-runs-of-length-n-in-coin-toss

The tests of NIST:
https://csrc.nist.gov/Projects/Random-Bit-Generation/Documentation-and-Software/Guide-to-the-Statistical-Tests
https://crypto.stackexchange.com/questions/67509/nist-randomness-test-p-values

On the Runs-Test:
https://itl.nist.gov/div898/handbook/eda/section3/eda35d.htm
https://en.wikipedia.org/wiki/Wald%E2%80%93Wolfowitz_runs_test
https://projecteuclid.org/euclid.aoms/1177731909

Longest runs:
https://experts.umn.edu/en/publications/long-repetitive-patterns-in-random-sequences

Basic distributions:
https://en.wikipedia.org/wiki/Bernoulli_trial
https://en.wikipedia.org/wiki/Bernoulli_distribution
https://en.wikipedia.org/wiki/Bernoulli_process
https://en.wikipedia.org/wiki/Binomial_distribution
https://en.wikipedia.org/wiki/Chi-squared_distribution

Kolmogorov-Smirnov test:
https://en.wikipedia.org/wiki/Kolmogorov%E2%80%93Smirnov_test
https://en.wikipedia.org/wiki/Nonparametric_statistics
https://en.wikipedia.org/wiki/Goodness_of_fit


TODOS

-1. Clean up this file.

0. Expand the above background

   From the NIST-report, the block-tests for frequencies and runs
   would be good to have, but the incomplete gamma-functions are
   not in the standard, and so likely we refrain from that.

   But we should run a serious of full tests, say 1000 x N=1e9
   for seeds 0,...,999, and then to feed the data (with the p-values)
   into R.

   Would be good to have also the precise calculations for
   longestrunheads:
   https://math.stackexchange.com/a/1409485/420244

1. Testing a fair coin-flip

   https://en.wikipedia.org/wiki/Checking_whether_a_coin_is_fair
   gives some background.

   The numerical example there is N=10 with 7 heads.
   That yields for them the "probability" of p in [0.45, 0.55] of only ~ 13%.

      Via confidence intervals we get
> confprop(65,7/10,10)
[1] 0.5018940 0.8518588
> confprop(66, 7/10, 10)
[1] 0.4988532 0.8538131
> confprop(95, 7/10, 10)
[1] 0.3475471 0.9332605

      So if we go for a 65%-confidence interval, then 0.5 is just excluded.
      Which would yield a "35% probability" of 0.5 being included, considerably
      higher than 13%.
      Perhaps their consideration is "one-sided", while our's is "two-sided"?

      While prop.test yields
> prop.test(7,10,0.5)
X-squared = 0.9, df = 1, p-value = 0.3428
95 percent confidence interval:
 0.3536707 0.9190522
sample estimates:
  p
0.7

The p-value seems consistent with the above rough 35%-probability.

Apparently for these small n-values the corrections built into prop.test
matter:

monobit(m,n) := erfc(abs(2*m-n)/sqrt(n)/sqrt(2));
float(monobit(7, 10));
.20590321073206821

The Wikipedia-page contains an example for confidence-intervals, where
the numerical values given there "0.4766 < r < 0.5170" are compatible with
our method:
> confprop(99.999, 5961/12000, 12000)
[1] 0.4765595 0.5169478

Their interpretation "Hence, 99.999% of the time, the interval above
would contain r which is the true value of obtaining heads in a single toss."
does not say what it means: When chosing p uniformly
from [0,1] and performing 12000 coin tosses with probability p, conditioning
on obtaining 5961 heads, then the probability of p being in the given
(symmetric) interval is (rather exactly) 99.999%."
That is, the single experiment is "choose p, perform 12000 coin flips with
probability p", and the probability we measure is the conditional propability
of p being in the interval, conditioned on the coin flips resulting
in (exactly) 5961 heads.
(One needs to check whether this is indeed what confprob in Allgemeines.R
computes (updating there the documentation accordingly)).


2. Runstest for general p

It seems that for N large enough, the distribution of runs for arbitrary
probability p converges against the normal distribution, and thus
the standard approach for p-values, using the formula

  p-value = erfc( abs(X-mu) / sigma / sqrt(2)

can be used.

 - The NIST-function (p=1/2) seems to use an approximation of mu and sigma
   using for p the found frequency in the given experiment. Unclear why they
   do this (ask them?).
 - For large N at least the difference to use the standard approach (with
   exact mu and sigma) seems negligible, but perhaps not for small N.
 - But the direct generalisation to arbitrary p, taking then also the
   measured frequency plus the guard for 4*sigma, does not work, yields
   an average of p-values higher than 0.5. (This was the old, first,
   implementation.))
 - The plain standard-approach however seems to work well.
 - Currently it uses Prob64 as type of p, to make the signature distinct
   from the p=12-runstest-function (with just three float80 as arguments):
  - In general this restriction is not needed, but a float80 can be used.
  - With monobit, one has one more argument for the general version, but now
    we have the same number of arguments, since N=n isn't used anymore.
  - It also corresponds somewhat to the meaning of p as "theoretical
    probability".

*/

#ifndef TESTS_OrkjQP7aug
#define TESTS_OrkjQP7aug

#include <utility>
#include <algorithm>
#include <tuple>
#include <vector>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>

#include "Numbers.hpp"

namespace RandGen {

  using float80 = FloatingPoint::float80;


  /* The context of the Binomial distributions:
     frequency of "heads" ("true")
  */

  inline constexpr float80 mean_Binomial(const float80 N) noexcept {
    return 0.5*N;
  }
  inline constexpr float80 mean_Binomial(const float80 N, const float80 p) noexcept {
    return p*N;
  }
  static_assert(mean_Binomial(0) == 0);
  static_assert(mean_Binomial(10) == 5);
  static_assert(mean_Binomial(30, 0.3L) == 9);

  inline constexpr float80 sigma_Binomial(const float80 N) noexcept {
    return 0.5 * FloatingPoint::sqrt(N);
  }
  inline constexpr float80 sigma_Binomial(const float80 N, const float80 p) noexcept {
    return FloatingPoint::sqrt(p * (1-p) * N);
  }
  static_assert(sigma_Binomial(0) == 0);
  static_assert(sigma_Binomial(100) == 5);
  static_assert(sigma_Binomial(100, 0.2L) == 4);

  /* Using the normal approximation to the Binomial distribution, the
     probability to obtain in a run of n fair coinflips a deviation from
     the true probability 0.5 which is at last as large as abs(0.5 - m/n).
     This is test
       2.1 Frequency (Monobit) Test
     in
       NIST Special Publication 800-22 Revision 1a
       A Statistical Test Suite for Random and Pseudorandom Number Generators
       for Cryptographic Applications
       Revised: April 2010
       https://csrc.nist.gov/publications/detail/sp/800-22/rev-1a/final
     the most basic test in the test-suite; the Decision Rule (at the 1% level)
     is (Section 2.1.5 there):
       If the computed P-value is < 0.01, then conclude that the sequence is
       non-random. Otherwise, conclude that the sequence is random.
     They recommend n >= 100.

  */
  inline constexpr float80 monobit(const float80 m, const float80 n) noexcept {
    assert(m <= n);
    return FloatingPoint::erfc(FloatingPoint::abs(2*m-n) / FloatingPoint::sqrt(n) / FloatingPoint::Sqr2);
  }
  static_assert(monobit(1,2) == 1);
  static_assert(monobit(50,100) == 1);
  static_assert(FloatingPoint::abs(monobit(6,10) - 0.5270892568655380851L) < 1e-19L);
  static_assert(FloatingPoint::abs(monobit(42,100) - 0.109598583399115988L) < 1e-19);

  inline constexpr float80 monobit(const float80 m, const float80 n, const float80 p) noexcept {
    assert(m <= n);
    assert(0 <= p && p <= 1);
    const float80 diff = FloatingPoint::abs(m - mean_Binomial(n, p));
    if (diff == 0) return 1;
    const float80 sigma = sigma_Binomial(n, p);
    if (sigma == 0) return 0;
    const float80 stand = diff / sigma;
    return FloatingPoint::erfc(stand / FloatingPoint::Sqr2);
  }
  static_assert(monobit(1,1,1) == 1);
  static_assert(monobit(1,1,0) == 0);
  static_assert(monobit(0,1,1) == 0);
  static_assert(monobit(0,1,0) == 1);
  static_assert(monobit(1,2,0.5) == 1);
  static_assert(monobit(50,100,0.5) == 1);
  static_assert(FloatingPoint::abs(monobit(6,10,0.5) - 0.5270892568655380851L) < 1e-19L);
  static_assert(FloatingPoint::abs(monobit(42,100,0.5) - 0.109598583399115988L) < 2e-19);
  static_assert(FloatingPoint::abs(monobit(20,100,0.1) - 8.5812066639367588e-4L) < 1e-19);
  static_assert(FloatingPoint::abs(monobit(80,100,0.9) - 8.5812066639367314e-4L) < 2e-18);


  /* The log of the binomial probability binomial(n,m)*p^n*(1-p)^(n-m)
     Returns -infinity if the probability is 0.
     The exact value is computed by the Maxima-function binomial_prop(m, n, p)
     in RandomClauseSets.mac.
  */
  inline constexpr float80 l_binomial_prob(const float80 m, const float80 n, const float80 p) noexcept {
    if (m > n or (p == 0 and m >= 1) or (p == 1 and m < n)) return FloatingPoint::minfinity;
    if (p == 0 or p == 1) return 1;
    assert(0 < p and p < 1);
    using FloatingPoint::log;
    if (m == 0) return n * log(1-p);
    if (m == n) return m * log(p);
    return m * log(p) + (n - m) * log(1 - p) + FloatingPoint::lbinomial_coeff(n,m);
  }
  static_assert(l_binomial_prob(0,1,0.7L) == FloatingPoint::log(0.3L));
  static_assert(l_binomial_prob(1,1,0.3L) == FloatingPoint::log(0.3L));
  static_assert(l_binomial_prob(0,1,1) == FloatingPoint::minfinity);
  static_assert(l_binomial_prob(1,1e100,0) == FloatingPoint::minfinity);
  static_assert(FloatingPoint::abs(l_binomial_prob(2,6,0.3L) - FloatingPoint::log(0.324134999999999839914721633249428L)) < 1e-15L);
  static_assert(FloatingPoint::abs(l_binomial_prob(2,10,0.1L) - FloatingPoint::log(387420489.0L/2000000000)) < 1e-18L);


  /* The tailed binomial-test, the sum of binomial_prob(m',n,p) for
     m <= m' <= n.

     Assuming that the outcome m/n is greater
     than the given p, which is small.

     Should give the same results as the R-function
       binom.test(m,n,p, alternative="greater")
     and as the Maxima-computation
       1 - cdf_binomial(m-1, n, p).

     The exact value is computed by the Maxima-function
       tailed_binomial_test(m, n, p)
     in RandomClauseSets.mac.

TODOS:

1. Also for the two-sided binomial-test a function is needed.

2. Also for the runstest the exact (discrete) distribution can be determined,
   and exact one-sided and two-sided test should be provided.

  */

  inline constexpr float80 tailed_binomial_test(const gen_uint_t m, const gen_uint_t n, const float80 p) noexcept {
    assert(n >= 1);
    assert(0 <= p and p <= 1);
    if (m > n) return 0;
    if (p == 1 or m == 0) return 1;
    if (p == 0) return 0;
    if (m == n) return FloatingPoint::pow(p,n);
    const float80 q = 1-p;
    if (m > n/2) {
      float80 prob = n > 60 ?
        FloatingPoint::exp(l_binomial_prob(m, n, p)) :
        FloatingPoint::binomial_coeff(n,m) * FloatingPoint::pow(p,m) * FloatingPoint::pow(q,n-m);
        float80 sum = prob;
        for (gen_uint_t i = m+1; i <= n; ++i) {
          prob *= p * (n-i+1) / q / i;
          sum += prob;
        }
        return sum;
    }
    else {
      float80 prob = FloatingPoint::pow(q,n);
      float80 sum = prob;
      for (gen_uint_t i = 1; i < m; ++i) {
        prob *= p * (n-i+1) / q / i;
        sum += prob;
      }
      return 1-sum;
    }
  }
  static_assert(tailed_binomial_test(0,1,0) == 1);
  static_assert(tailed_binomial_test(1,1,0) == 0);
  static_assert(tailed_binomial_test(0,1,1) == 1);
  static_assert(tailed_binomial_test(1,1,1) == 1);
  static_assert(tailed_binomial_test(1,1,0.5L) == 0.5L);
  static_assert(tailed_binomial_test(0,1,0.5L) == 1);
  static_assert(tailed_binomial_test(0,1,0.1L) == 1);
  static_assert(tailed_binomial_test(1,1,0.7L) == 0.7L);
  static_assert(tailed_binomial_test(1,1,1e-18L) == 1e-18L);
  static_assert(FloatingPoint::abs(tailed_binomial_test(1,10,0.1L) - 0.6513215599L) < 1e-18L);
  static_assert(tailed_binomial_test(3,27,0.3L) == 0.994937465960768532499453575L);
  static_assert(FloatingPoint::abs(tailed_binomial_test(28,33,0.2L) - 2.180686743085252608e-15L) < 1e-33L);
  static_assert(FloatingPoint::abs(tailed_binomial_test(51,235,1.0L/6) - 0.02654424571169092029019118952683548417L) < 1e-17L);
  static_assert(FloatingPoint::abs(tailed_binomial_test(200,257,0.8L) - 0.829711351875466539993028918769L) < 1e-16L);


  class Count_true {
  public :
    using UInt_t = FloatingPoint::UInt_t;
    void operator ()(const bool b) noexcept { c += b; }
    constexpr UInt_t operator *() const noexcept { return c; }
  private :
    UInt_t c = 0;
  };
  static_assert(*Count_true() == 0);


  /* Runs and their analysis */

  inline constexpr float80 mean_numruns(const float80 N) noexcept {
    assert(N >= 0);
    return 0.5 * (N + 1);
  }
  static_assert(mean_numruns(1) == 1);
  static_assert(mean_numruns(2) == 1.5);
  inline constexpr float80 mean_numruns(const float80 N, const float80 p) noexcept {
    assert(N >= 0);
    return 1 + 2 * (N-1) * p * (1-p);
  }
  static_assert(mean_numruns(1,0) == 1);
  static_assert(mean_numruns(1,0.6) == 1);
  static_assert(FloatingPoint::abs(mean_numruns(2, 0.3L) - 1.42L) < 1e-18L);

  /* Standard deviation, computed as in
     https://math.stackexchange.com/a/750221/420244
     and obtained the same result as in
     https://math.stackexchange.com/a/805454/420244
     corrected for N=1:
  */
  inline constexpr float80 sigma_numruns(const float80 N) noexcept {
    assert(N >= 1);
    return 0.5 * FloatingPoint::sqrt(N - 1);
  }
  static_assert(sigma_numruns(1) == 0);
  static_assert(sigma_numruns(2) == 0.5);
  inline constexpr float80 sigma_numruns(const float80 N, const float80 p) noexcept {
    assert(N >= 1);
    if (N == 1) return 0;
    const float80 prod = 2 * p * (1-p);
    return FloatingPoint::sqrt(prod * (2*N - 3 - (3*N - 5) * prod));
  }
  static_assert(sigma_numruns(1,0) == 0);
  static_assert(sigma_numruns(1,0.3L) == 0);
  static_assert(sigma_numruns(1,1) == 0);
  static_assert(sigma_numruns(2,0.5) == 0.5);

  // m is the number of true's, n the total number of trials,
  // r the number of runs (of true's or false's);
  // assuming the true probability is 1/2:
  inline constexpr float80 runstest(const float80 m, const float80 n, const float80 r) noexcept {
    assert(n >= 1);
    assert(m <= n);
    assert(1 <= r and r <= n);
    using FloatingPoint::abs;
    using FloatingPoint::Sqr2;
    const float80 p = m / n;
    const float80 sqn = FloatingPoint::sqrt(n);
    if (abs(p - 0.5) >= 2 / sqn) return FloatingPoint::minfinity; // 4*sigma
    const float80 q = 1 - p;
    return FloatingPoint::erfc(abs(r - 2*n*p*q) / (2*Sqr2*sqn*p*q));
  }
  static_assert(runstest(0,100,1) == FloatingPoint::minfinity);
  static_assert(FloatingPoint::abs(runstest(6,10,7) - 0.14723225536366556485L) < 1e-19L);

  // Now for arbitrary true probability p, not using m anymore, but just
  // the total number n of trials, the number r of runs, and the assumed
  // probability p of "true" (or "false" --- doesn't matter, since a "run"
  // counts both types of runs):
  inline constexpr float80 runstest_gen(const float80 n, const float80 r, const Prob64 p) noexcept {
    assert(n >= 1);
    assert(1 <= r and r <= n);
    assert(0 <= p and p <= 1);
    const float80 diff = FloatingPoint::abs(r - mean_numruns(n, p));
    if (diff == 0) return 1;
    const float80 sigma = sigma_numruns(n, p);
    if (sigma == 0) return 0;
    const float80 stand = diff / sigma;
    return FloatingPoint::erfc(stand / FloatingPoint::Sqr2);
  }
  static_assert((runstest_gen(1,1,{0,1}) == 1));
  static_assert((runstest_gen(1,1,{1,1}) == 1));
  static_assert((runstest_gen(2,2,{0,1}) == 0));
  static_assert((runstest_gen(2,2,{1,1}) == 0));
  static_assert((runstest_gen(1e10, 2.187445784e9, {1,8}) == runstest_gen(1e10, 2.187445784e9, {7,8})));


  class CountRuns {
  public :
    using UInt_t = FloatingPoint::UInt_t;
    typedef std::pair<UInt_t, UInt_t> res_t;
    explicit constexpr CountRuns(const bool init) noexcept :
    current(init), r(1), c(init) {}
    void operator ()(const bool b) noexcept {
      c += b;
      if (b != current) { ++r; current = b; }
    }
    constexpr res_t operator *() const noexcept { return {r,c}; }
  private :
    UInt_t current, r, c;
  };
  static_assert(*CountRuns(true) == CountRuns::res_t{1,1});
  static_assert(*CountRuns(false) == CountRuns::res_t{1,0});


  /* Longest runs */

  // According to
  // https://math.stackexchange.com/questions/1409372/what-is-the-expected-length-of-the-largest-run-of-heads-if-we-make-1-000-flips :
  inline constexpr float80 meanasym_longestrunheads(const float80 n) noexcept {
    constexpr float80 special = FloatingPoint::euler_mascheroni / FloatingPoint::Log2 - 1.5;
    static_assert(special == -0.66725382272313284935L);
    return FloatingPoint::log2(n) + special;
  }
  static_assert(meanasym_longestrunheads(1000) == 9.298530461938954194L);

  inline constexpr float80 sigmaasym_longestrunheads() noexcept {
    constexpr float80 pilog2 = FloatingPoint::pi / FloatingPoint::Log2;
    using FloatingPoint::sqrt;
    return 1/sqrt(6) * sqrt(pilog2*pilog2 + 0.5L);
  }
  static_assert(FloatingPoint::abs(sigmaasym_longestrunheads() - 1.87271142354358396508039441905133441L) < 1e-18);

  class LongestRun {
  public :
    using UInt_t = FloatingPoint::UInt_t;
    typedef std::tuple<UInt_t, UInt_t, UInt_t, UInt_t> res_t;
    explicit constexpr LongestRun(const bool init) noexcept :
    current(init), r(1), c(init), l(1), lt(init), lf(not init) {}
    void operator ()(const bool b) noexcept {
      if (b) {
        ++c;
        if (current) ++l;
        else { ++r; current = true; lf = std::max(l,lf); l=1; }
      }
      else {
        if (current) { ++r; current = false; lt = std::max(l,lt); l=1; }
        else ++l;
      }
    }
    constexpr res_t operator *() noexcept {
      if (current) lt = std::max(l,lt);
      else lf = std::max(l,lf);
      return {lt,lf,r,c};
    }
  private :
    UInt_t current, r, c, l, lt, lf;
  };
  static_assert(*LongestRun(true) == LongestRun::res_t{1,0,1,1});
  static_assert(*LongestRun(false) == LongestRun::res_t{0,1,1,0});



  /* Kolmogorov-Smirnov test

     https://www.itl.nist.gov/div898/handbook/eda/section3/eda35g.htm

     The D-value lies in [0,1], the greater the larger the distance from
     the uniform distribution (depending on the size of the vector).

     The p-value implementation is an adaptation from
     http://dx.doi.org/10.18637/jss.v008.i18
     (the value computed there is 1-p).

TODOS:

1. The original value of ks_scaling_exp was 140, now 1000, due to using
   float80 -- is this appropriate?
    - Also using FloatingPoint::ldexp seems better for doing the scaling.
      (then by powers of 10).

2. The handling of n=1 simulates the values computed by R: check this.

3. One should understand exactly the meaning of the p-value computed.

4. The literature always assumes that ties in x does not exist (for
   function ks_D_value):
    - How to overcome this?
    - Is the current treatment appropriate?

  */

  inline FloatingPoint::float80 ks_D_value(const std::vector<FloatingPoint::float80>& x) noexcept {
    assert(std::is_sorted(x.begin(), x.end()));
    using FloatingPoint::float80;
    float80 D = FloatingPoint::minfinity;
    if (x.empty()) return D;
    float80 frac = 0;
    for (std::vector<float80>::size_type i = 0; i < x.size(); ) {
      const auto val = x[i];
      assert(0 <= val and val <= 1);
      D = std::max(D, val - frac);
      frac = float80(++i) / x.size();
      D = std::max(D, frac - val);
    }
    return D;
  }

  typedef std::vector<FloatingPoint::float80> fvec_t;

  namespace detail {

  void ks_mMultiply(const fvec_t& A, const fvec_t& B, fvec_t& C, const gen_uint_t m) noexcept {
    for (gen_uint_t i=0; i<m; ++i) {
      const gen_uint_t im = i*m;
      for (gen_uint_t j=0; j<m; ++j) {
        FloatingPoint::float80 s=0;
        for (gen_uint_t k=0; k<m; ++k) s += A[im+k] * B[k*m+j];
        C[im+j]=s;
      }
    }
  }

  constexpr gen_uint_t ks_scaling_exp = 1000;
  constexpr FloatingPoint::float80 ks_too_big = FloatingPoint::pow(10,ks_scaling_exp);
  static_assert(ks_too_big == 1e1000L);
  constexpr FloatingPoint::float80 ks_scaling_factor = 1/ks_too_big;
  static_assert(ks_scaling_factor == 1e-1000L);

  void ks_mPower(const fvec_t& A, const gen_uint_t eA, fvec_t& V, gen_uint_t& eV, const gen_uint_t m, const gen_uint_t n) {
    if(n == 1) {
      for (gen_uint_t i = 0; i < m*m; ++i) V[i]=A[i];
      eV = eA;
      return;
    }

    ks_mPower(A, eA, V, eV, m, n/2);
    fvec_t B(m*m);
    ks_mMultiply(V, V, B, m);
    gen_uint_t eB = 2*eV;
    if (n % 2 == 0) {
      for (gen_uint_t i=0; i < m*m; ++i) V[i]=B[i];
      eV = eB;
    }
    else {
      ks_mMultiply(A, B, V, m);
      eV = eA+eB;
    }
    if (V[(m/2)*m + m/2] > ks_too_big) {
      for (gen_uint_t i=0; i < m*m; ++i) V[i] *= ks_scaling_factor;
      eV += ks_scaling_exp;
    }
  }

  } // end namespace detail

  FloatingPoint::float80 ks_P(const gen_uint_t n, const FloatingPoint::float80 d) {
    assert(n >= 1);
    assert(d >= 0 and d <= 1);
    if (n == 1) return 1-2*FloatingPoint::abs(d-0.5L);
    using FloatingPoint::float80;

  //OMIT NEXT LINE IF YOU REQUIRE >7 DIGIT ACCURACY IN THE RIGHT TAIL
  // s=d*d*n; if(s>7.24||(s>3.76&&n>99)) return 1-2*exp(-(2.000071+.331/sqrt(n)+1.409/n)*s);

    const gen_uint_t k = gen_uint_t(n*d) + 1;
    assert(k >= 1);
    const gen_uint_t m = 2*k - 1;
    assert(m >= 1);
    const float80 h = k - n*d;

    fvec_t H(m*m), Q(m*m);

    for (gen_uint_t i=0; i<m; ++i)
      for (gen_uint_t j=0; j <= std::min(i+1, m-1); ++j)
        H[i*m+j] = 1;
    for (gen_uint_t i=0; i<m; ++i) {
      H[i*m] -= FloatingPoint::pow(h,i+1);
      H[(m-1)*m+i] -= FloatingPoint::pow(h,m-i);
    }
    H[(m-1)*m] += (2*h-1 > 0 ? FloatingPoint::pow(2*h-1,m) : 0);
    for (gen_uint_t i=0; i<m; ++i)
      for (gen_uint_t j=0; j < std::min(i+1, m); ++j)
        for (gen_uint_t g=1; g <= (i+1)-j; ++g) H[i*m+j] /= g;

    const gen_uint_t eH = 0;
    gen_uint_t eQ;
    detail::ks_mPower(H,eH,  Q,eQ,  m,n);

    float80 s = Q[(k-1)*m+k-1];
    for (gen_uint_t i = 1; i <= n; ++i) {
      s *= float80(i)/n;
      if (s < detail::ks_scaling_factor) {
        s *= detail::ks_too_big;
        eQ -= detail::ks_scaling_exp;
      }
    }
    s *= FloatingPoint::pow(10, eQ);
    return 1-s;
  }

  struct ExtremePVal {
    using float80 = FloatingPoint::float80;
    float80 level;
    gen_uint_t count;
    float80 p;
  };
  inline constexpr bool operator ==(const ExtremePVal lhs, const ExtremePVal rhs) noexcept {
    return lhs.level == rhs.level and lhs.count == rhs.count and lhs.p == rhs.p;
  }
  std::ostream& operator <<(std::ostream& out, const ExtremePVal e) {
    return out << e.level << " " << e.count << " " << FloatingPoint::Wrap(e.p);
  }

  // Determining the maximum level such that pv contains elements <= 10^-level,
  // and computing the Bernoulli-p-value for the occurrence of these p-values:
  ExtremePVal epval_prob(const fvec_t& pv) noexcept {
    assert(std::is_sorted(pv.begin(), pv.end()));
    assert(not pv.empty());
    const auto minp = pv.front();
    assert(minp >= 0);
    if (minp == 0) {
      gen_uint_t count = 1;
      for (; count < pv.size() and pv[count] == 0; ++count);
      return {FloatingPoint::pinfinity,count,0};
    }
    assert(minp <= 1);
    if (minp == 1) return {0,pv.size(),1};
    const auto lminp = FloatingPoint::floor(-FloatingPoint::log10(minp));
    assert(lminp >= 0);
    if (lminp == 0) return {0,pv.size(),1};
    const auto p = FloatingPoint::pow(10, -lminp);
    assert(minp <= p);
    gen_uint_t count = 1;
    for (; count < pv.size() and pv[count] <= p; ++count);
    return {lminp, count, tailed_binomial_test(count, pv.size(), p)};
  }

  typedef std::pair<FloatingPoint::float80, ExtremePVal> AnalysePVal;
  std::ostream& operator <<(std::ostream& out, const AnalysePVal& a) {
    return out << FloatingPoint::Wrap(a.first) << " " << a.second;
  }
  AnalysePVal analyse_pvalues(fvec_t pv) noexcept {
    assert(not pv.empty());
    std::sort(pv.begin(), pv.end());
    const auto Kp = ks_P(pv.size(), ks_D_value(pv));
    const auto Pp = epval_prob(pv);
    return {Kp, Pp};
  }
  AnalysePVal analyse_pvalues(const std::vector<AnalysePVal>& v) {
    fvec_t p;
    p.reserve(v.size());
    for (const auto& x : v) p.push_back(x.first);
    return analyse_pvalues(std::move(p));
  }
  FloatingPoint::float80 min_pvalue(const std::vector<AnalysePVal>& v) noexcept {
    FloatingPoint::float80 min = FloatingPoint::pinfinity;
    for (const auto& a : v) min = FloatingPoint::min(min, a.second.p);
    return min;
  }

}

#endif
