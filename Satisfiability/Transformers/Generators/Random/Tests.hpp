// Oliver Kullmann, 21.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Statistical tests

TODOS

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

#include <cassert>

#include <Numerics/FloatingPoint.hpp>

namespace RandGen {

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
  inline constexpr FloatingPoint::float80 monobit(const FloatingPoint::float80 m, const FloatingPoint::float80 n) noexcept {
    assert(m <= n);
    return FloatingPoint::erfc(FloatingPoint::abs(2*m-n) / FloatingPoint::sqrt(n) / FloatingPoint::sqrt(2));
  }
  static_assert(monobit(1,2) == 1);
  static_assert(monobit(50,100) == 1);
  static_assert(FloatingPoint::abs(monobit(6,10) - 0.5270892568655380851L) < 1e-19L);
  static_assert(FloatingPoint::abs(monobit(42,100) - 0.109598583399115988L) < 1e-19);

  inline constexpr FloatingPoint::float80 monobit(const FloatingPoint::float80 m, const FloatingPoint::float80 n, const FloatingPoint::float80 p) noexcept {
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

}

#endif
