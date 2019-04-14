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



TODOS

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
computes (updating there the documentation accordingly).


*/

#ifndef TESTS_OrkjQP7aug
#define TESTS_OrkjQP7aug

#include <utility>
#include <algorithm>
#include <tuple>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>

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
    return FloatingPoint::erfc(FloatingPoint::abs(m-n*p) / FloatingPoint::sqrt(2*n*p*(1-p)));
  }
  static_assert(monobit(1,2,0.5) == 1);
  static_assert(monobit(50,100,0.5) == 1);
  static_assert(FloatingPoint::abs(monobit(6,10,0.5) - 0.5270892568655380851L) < 1e-19L);
  static_assert(FloatingPoint::abs(monobit(42,100,0.5) - 0.109598583399115988L) < 1e-19);
  static_assert(FloatingPoint::abs(monobit(20,100,0.1) - 8.5812066639367588e-4L) < 1e-19);
  static_assert(FloatingPoint::abs(monobit(80,100,0.9) - 8.5812066639367314e-4L) < 2e-18);

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
    if (abs(p - 0.5) >= 2 / sqn) return -FloatingPoint::pinfinity; // 4*sigma
    const float80 q = 1 - p;
    return FloatingPoint::erfc(abs(r - 2*n*p*q) / (2*Sqr2*sqn*p*q));
  }
  static_assert(FloatingPoint::abs(runstest(6,10,7) - 0.14723225536366556485L) < 1e-19L);

  // Now for arbitrary true probability p0 (likely not using approximations here):
  inline constexpr float80 runstest(const float80 m, const float80 n, const float80 r, const float80 p0) noexcept {
    assert(n >= 1);
    assert(m <= n);
    assert(1 <= r and r <= n);
    assert(0 <= p0 and p0 <= 1);
    using FloatingPoint::abs;
    {const float80 diff0 = abs(m - mean_Binomial(n, p0));
     if (diff0 > 0 and diff0 >= 4 * sigma_Binomial(n, p0))
       return -FloatingPoint::pinfinity;}
    const float80 p = m / n;
    const float80 diff = abs(r - mean_numruns(n, p));
    if (diff == 0) return 1;
    const float80 stand = diff / sigma_numruns(n, p);
    return FloatingPoint::erfc(stand / FloatingPoint::Sqr2);
  }
  static_assert(runstest(1,1,1,0) == -FloatingPoint::pinfinity);
  static_assert(runstest(1,1,1,1) == 1);


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

}

#endif
