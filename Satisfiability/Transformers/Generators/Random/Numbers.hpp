// Oliver Kullmann, 6.7.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Components for random numbers

    - randgen_t is the type of our standard 64-bit random engine
    - gen_uint_t is the type of the generated unsigned 64-bit integers
    - constants randgen_max = 2^64-1, max_half_p1 = 2^63.

    - Function bernoulli for a random bool
    - class Bernoulli for random bool with dyadic probability (here we
      can guarantee well-definedness, and this with exactly one
      generator-call).

    - Class UniformRange for generation of uniform random numbers from
      0, ..., n-1. Also well-defined, but for n which are not powers of
      two, possibly more than one generator-call is needed.

    - Helper functions for gen_uint_t:
     - iexp2(e) = 2^e
     - ildexp(x, e) = x * 2^e
     - powerof2(x) is true iff x is an integer power of 2.

    - seed_t, vec_seed_t for seeding with a sequence of 32-bit unsigned
      integers.

    - Algorithm shuffle for shuffling a sequence.

   Originally adapted from the file TwoCNF/RandGen.hpp (created 2017 by OK),
   and then further developed as Queens/GenericSAT/RandGen.hpp.

TODOS:

1. Create programs "TimingXXX.cpp", which are compiled with optimisation, and
   can run without arguments, where then the command-line "time" gives an
   an idea on speed.
    - DONE: Start with TimingDiscard.cpp, which by default performs
      g.discard(10^9) say 1000 times.
    - DONE: These two parameters can be set on the command-line, using
      FloatingPoint::toUInt(std::string) for conversion.
    - DONE: Any of these Timing-programs should output the Git-ID and
      compilation-information.
    - DONE: Next is TimingBernoulli, with two further inputs.
    - Next is TimingUniformRange.
    - Having many Timing-programs, updating the version after an update of a
      general library-function is awkward; should there be some more general
      management?
    - All the above need to be applied with the possibility of appending
      a seed-sequence to the end, to check different (valid) seeds.
     - Not needed for TimingDiscard, TimingGeneration, TimingInitialisation.
     - DONE: TimingBernoulli12
     - TimingBernoulli
    - The final (current) results should go into a subdirectories timings.
    - Likely we need a script/program, which puts the data into timings,
      so that from time to time one can automatically check the results,
      wheter something has changed.
    - Best to develop a model for each Timing-program, obtained by linear
      regression in a standardised way, for each computer.

2. Testing the function bernoulli
    - There is an R function for computing the probability for an observed
      probability (interval).
    - This should then be generalised to test class Bernoulli.
    - Running experiments:

4 new experiments on csverify:
Random$ /usr/bin/time --output=Out2e13_e --append ./TimingBernoulli12 2e13 > Out2e13_e &
0.2.1 14.3.2019 TimingBernoulli12 8e4bd32eb285402856d475e6235515b94f51bcd7
csverify g++ 8.2.0 Mar 14 2019 21:41:23
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
20000000000000
()
10000003316339 0.50000016581694999999
2e+13
39780.27user 0.00system 11:03:00elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+54minor)pagefaults 0swaps

> confprop(99.9, 0.50000016581694999999, 2e13)
[1] 0.4999998 0.5000005
> confprop(69, 0.50000016581694999999, 2e13)
[1] 0.5000001 0.5000003
> confprop(70, 0.50000016581694999999, 2e13)
[1] 0.5000000 0.5000003

This is quite a high deviation; so we run it longer:
Random$ /usr/bin/time --output=Out1e14_e --append ./TimingBernoulli12 1e14 > Out2e13_e &
XXX

Random$ /usr/bin/time --output=Out2e13_0 --append ./TimingBernoulli12 2e13 0 > Out2e13_0 &
0.2.1 14.3.2019 TimingBernoulli12 8e4bd32eb285402856d475e6235515b94f51bcd7
csverify g++ 8.2.0 Mar 14 2019 21:41:23
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
20000000000000 0
(0,0)
9999999968549 0.49999999842745000001
2e+13
39897.78user 0.00system 11:04:57elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+54minor)pagefaults 0swaps

> confprop(99.9, 0.49999999842745000001, 2e13)
[1] 0.4999996 0.5000004
Within R-precision, not possible to deviate from 0.5:
> confprop(1e-10, 0.49999999842745000001, 2e13)
[1] 0.5 0.5

Random$ /usr/bin/time --output=Out2e13_1 --append ./TimingBernoulli12 2e13 1 > Out2e13_1 &
0.2.1 14.3.2019 TimingBernoulli12 8e4bd32eb285402856d475e6235515b94f51bcd7
csverify g++ 8.2.0 Mar 14 2019 21:41:23
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
20000000000000 1
(1,0)
9999999846301 0.49999999231504999999
2e+13
40098.29user 0.00system 11:08:18elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+53minor)pagefaults 0swaps

> confprop(99.9, 0.49999999231504999999, 2e13)
[1] 0.4999996 0.5000004
> confprop(1e-10, 0.49999999231504999999, 2e13)
[1] 0.5 0.5

Random$ /usr/bin/time --output=Out2e13_2 --append ./TimingBernoulli12 2e13 2 > Out2e13_2 &
0.2.1 14.3.2019 TimingBernoulli12 8e4bd32eb285402856d475e6235515b94f51bcd7
csverify g++ 8.2.0 Mar 14 2019 21:41:23
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
20000000000000 2
(2,0)
10000000144468 0.5000000072234
2e+13
39775.64user 0.00system 11:02:55elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+53minor)pagefaults 0swaps

> confprop(99.9, 0.5000000072234, 2e13)
[1] 0.4999996 0.5000004
> confprop(1e-10, 0.5000000072234, 2e13)
[1] 0.5 0.5


Compared with the older timing on csverify below, this is nearly 10%
slower??
This should be investigated; it seems unlikely that running these 4 experiments
at the same time should have caused the slowdown, since the machine has
6 dual-cores, and no other processes seemed to run.
Is investigated in TimingBernoulli.hpp.
Case closed: for now it appears as random.


Older experiment on c-wsok:

Random> /usr/bin/time --output=Out2e13 --append ./TimingBernoulli12 2e13 > Out2e13 &
Random> cat Out2e13
TimingBernoulli12 0.1.2 5.3.2019 b0941a4522682080b65781457c2ad8cd3a1d639f
g++ 8.3.0 Mar  5 2019 01:50:16
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
20000000000000 9999998469810 0.4999999234905
2e+13
68655.54user 11.43system 19:05:09elapsed 99%CPU (0avgtext+0avgdata 2304maxresident)k
0inputs+8outputs (0major+173minor)pagefaults 0swaps

Roughly 291e6 generations per sec; evaluation in R:
> confprop(99.9, 0.4999999234905, 2e13)
[1] 0.4999996 0.5000003
> confprop(18, 0.4999999234905, 2e13)
[1] 0.4999999 0.4999999
> confprop(19, 0.4999999234905, 2e13)
[1] 0.4999999 0.5000000

So from this point of view, the probability for 1/2 being inconsistent with
the observed frequency is around 18%.

Such confidence intervals seem the right thing to do. The above computation
also uses the precise binomial distribution, not an approximation.

The more conventional "p-value tests" (now using the approximation by the
normal distribution, helped by the "Yates continuity correction" and by
the "Wilson score interval"):

> prop.test(9999998469810, 20000000000000, 0.5)
        1-sample proportions test with continuity correction
data:  9999998469810 out of 2e+13, null probability 0.5
X-squared = 0.4683, df = 1, p-value = 0.4938
alternative hypothesis: true p is not equal to 0.5
95 percent confidence interval:
 0.4999997 0.5000001
sample estimates:
        p
0.4999999

So the data is well consistent with the true probability being 0.5.

Checking with csverify:
Random$ cat Out2e13
TimingBernoulli12 0.1.2 5.3.2019 01fe2e32f34bada7f1b680f301b9a3c5a9561f15
g++ 8.2.0 Mar  6 2019 02:16:35
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
20000000000000 9999998469810 0.4999999234905
2e+13
37398.01user 0.00system 10:23:18elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+53minor)pagefaults 0swaps

Roughly 535e6 generations per sec.


Now the above results need to be summarised at an appropriate place.

    - https://en.wikipedia.org/wiki/Checking_whether_a_coin_is_fair
      gives a nice background.

      The numerical example there is N=10 with 7 heads.
      That yields for them the "probability" of p=0.5 of ~ 13%.

      Via confidence intervals we get
> confprop(66, 7/10, 10)
[1] 0.4988532 0.8538131
> confprop(95, 7/10, 10)
[1] 0.3475471 0.9332605

      So if we go for a 66%-confidence interval, then 0.5 is just excluded.
      Which would yield a "34% probability" of 0.5 being excluded, considerably
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

#ifndef RandGen_ABopVoYw
#define RandGen_ABopVoYw

#include <random>
#include <limits>
#include <vector>
#include <utility>
#include <ostream>
#include <type_traits>
#include <algorithm>

#include <cstdint>
#include <cassert>

namespace RandGen {

  // The type of the random-engine:
  typedef std::mt19937_64 randgen_t;
  // The type of the random unsigned integers produced by randgen_t:
  typedef randgen_t::result_type gen_uint_t;
  static_assert(std::is_same_v<gen_uint_t, std::uint64_t>);

  constexpr gen_uint_t randgen_max{randgen_t::max()};
  constexpr gen_uint_t max_half_p1{randgen_max / 2 + 1};

  static_assert(randgen_t::min() == 0);
  static_assert(randgen_max == std::numeric_limits<std::uint64_t>::max());
  static_assert(randgen_max == 0xFFFF'FFFF'FFFF'FFFFL);
  static_assert(randgen_max == gen_uint_t(-1));
  static_assert(max_half_p1 == 0x8000'0000'0000'0000L);


  // Two auxiliary functions for special unsigned comparisons:

  // Testing x < max_half_p1 = 2^63:
  inline constexpr bool lessP263(const gen_uint_t x) noexcept {
    return not (x >> 63);
  }
  static_assert(lessP263(0));
  static_assert(not lessP263(randgen_max));
  static_assert(lessP263(max_half_p1 -1));
  static_assert(not lessP263(max_half_p1));

  inline constexpr bool lessP232(const gen_uint_t x) noexcept {
    return not (x >> 32);
  }
  static_assert(lessP232(0));
  static_assert(not lessP232(randgen_max));


  // Auxiliary function, computing integral binary powers:
  inline constexpr gen_uint_t iexp2(const gen_uint_t e) noexcept {
    assert(e < 64);
    return gen_uint_t(1) << e;
  }
  static_assert(iexp2(0) == 1ULL);
  static_assert(iexp2(1) == 2ULL);
  static_assert(iexp2(2) == 4ULL);
  static_assert(iexp2(63) == max_half_p1);
  static_assert(lessP232(iexp2(32)-1));
  static_assert(not lessP232(iexp2(32)));

  inline constexpr gen_uint_t ildexp(const gen_uint_t x, const gen_uint_t e) noexcept {
    return x << e;
  }
  static_assert(ildexp(1,13) == iexp2(13));
  static_assert(ildexp(0,10) == 0);
  static_assert(ildexp(777,0) == 777);
  static_assert(ildexp(3,2) == 12);


  // Seeding with a sequence of values

  // The type of a single value:
  typedef std::uint32_t seed_t;
  // The type of the sequence of values:
  typedef std::vector<seed_t> vec_seed_t;
  // Extended seed-sequences:
  typedef std::vector<gen_uint_t> vec_eseed_t;

  // Checking whether an vec_eseed_t is indeed a vec_seed_t:
  inline bool is_seed_t(const vec_eseed_t& v) noexcept {
    return std::find_if_not(v.begin(), v.end(), lessP232) == v.end();
  }

  typedef std::pair<seed_t,seed_t> pair_seed_t;
  inline constexpr pair_seed_t split(const gen_uint_t x) noexcept {
    return {x, x >> 32};
  }
  static_assert(split(0) == pair_seed_t{0,0});
  static_assert(split(1) == pair_seed_t{1,0});
  static_assert(split(iexp2(32)) == pair_seed_t{0,1});
  static_assert(split(iexp2(63)) == pair_seed_t{0, iexp2(31)});
  static_assert(split(randgen_max) == pair_seed_t{seed_t(-1),seed_t(-1)});

  // Split-Policy class:
  enum class SP {trunc, split, check};
  // trunc: just keep the lower 32-bit
  // split: use split on the elements
  // check: uses is_seed_t to test

  // Transform, using truncation or splitting:
  inline vec_seed_t transform(const vec_eseed_t& v, const SP p = SP::split) {
    switch (p) {
    case SP::trunc : {
      vec_seed_t res; res.resize(v.size());
      std::copy(v.begin(),v.end(), res.begin());
      return res;}
    case SP::check :
      return (is_seed_t(v) ? transform(v,SP::trunc) : transform(v,SP::split));
    default : {
      vec_seed_t res; res.reserve(2*v.size());
      for (const auto x : v) {
        const auto [first,second] = split(x);
        res.push_back(first); res.push_back(second);
      }
      return res;}
    }
  }
  // Embed-Policy class:
  enum class EP { four, one };
  // four: four chars in one seed_t
  // one: one char in one seed_t
  // Checking whether a string uses only ascii-codes, and thus the
  // transformed integer-sequence is platform-independent:
  inline bool valid_ascii(const std::string& s) noexcept {
    return std::all_of(s.begin(), s.end(), [](const unsigned char x){return x <= 127;});
  }
  inline vec_seed_t transform(const std::string& s, const EP p) {
    switch (p) {
    case EP::one :
      return vec_seed_t(s.begin(),s.end());
    default : {
      vec_seed_t res; res.reserve((s.size()+3) / 4);
      gen_uint_t value = 0;
      unsigned int exp = 0;
      for (const unsigned char x : s) {
        value += ildexp(x, exp);
        exp += 8;
        if (exp == 32) {
          exp = 0;
          res.push_back(value);
          value = 0;
        }
      }
      if (exp != 0) res.push_back(value);
      return res;}
    }
  }

  inline randgen_t init(const vec_seed_t& v) {
    std::seed_seq s(v.begin(), v.end());
    return randgen_t(s);
  }


  /* Wrapper around random-generator g, providing initialisation with
     a sequence of seeds only: It seems besides default-initialisation of g,
     only this form of initialisation should be used.
  */
  class RandGen_t {
    randgen_t g_;
  public :
    static constexpr gen_uint_t min() noexcept {return 0;}
    static constexpr gen_uint_t max() noexcept {return randgen_max;}
    void discard(unsigned long long z) noexcept {g_.discard(z);}
    gen_uint_t operator ()() noexcept { return g_(); }

    randgen_t extract() const noexcept { return g_; }

    explicit RandGen_t() noexcept : g_(init({})) {};
    explicit RandGen_t(const vec_seed_t& v) : g_(init(v)) {}

    explicit RandGen_t(const RandGen_t&) noexcept = default;
    explicit RandGen_t(RandGen_t&&) noexcept = default;
    RandGen_t& operator =(const RandGen_t&) noexcept = default;
    RandGen_t& operator =(RandGen_t&&) noexcept = default;

    friend bool operator ==(const RandGen_t& lhs, const RandGen_t& rhs) noexcept {
      return lhs.g_ == rhs.g_;
    }
    friend bool operator !=(const RandGen_t& l, const RandGen_t& r) noexcept {
      return not (l == r);
    }
    friend std::ostream& operator <<(std::ostream& out, const RandGen_t& g) {
      return out << g.g_;
    }

  };
  static_assert(RandGen_t::min() == randgen_t::min());
  static_assert(RandGen_t::max() == randgen_t::max());


  // Returns true/false with probability 1/2, using exactly one call of g:
  inline bool bernoulli(randgen_t& g) noexcept { return lessP263(g()); }
  inline bool bernoulli(RandGen_t& g) noexcept { return lessP263(g()); }


  /* Class Bernoulli, generalising bernoulli(g) for dyadic p

     Here the propability is given by p = x / 2^y for returning tree
     (using real-number arithmetic for the mathematical specification,
     and exact handling in the implementation), now employing a functor:
      - y is integer with 0 <= y <= 63
      - x is integer with 0 <= x <= 2^y.
      - Construct by
          Bernoulli b(g, x, y);
        (const or not).
      - Call as
          b()
      - This class can be simulated by UniformRange (see Test.cpp), but is
        more efficient.
      - The generator g is called exactly once for each b(), except of the
        cases with probabilities 0 or 1 (where there are no calls).
  */
  class Bernoulli {
    randgen_t& g;
  public :
    enum class S {c0, c1, nc }; // constant 0/1, or non-constant
    const S s;
    const gen_uint_t threshold;
    Bernoulli(randgen_t& g, const gen_uint_t x, const gen_uint_t y) noexcept :
      g(g), s(set_S(x,y)), threshold(set_t(x,y,s)) {}
    Bernoulli(const Bernoulli& b) = delete;
    bool operator ()() const noexcept {
      switch (s) {
      case S::c0 : return false;
      case S::c1 : return true;
      default : return g() < threshold;
      }
    }

    static constexpr S set_S(const gen_uint_t x, const gen_uint_t y) noexcept {
      if (x == 0) return S::c0;
      assert(y < 64);
      assert(x <= iexp2(y));
      if (iexp2(y) == x) return S::c1; else return S::nc;
    }
    static constexpr gen_uint_t set_t(const gen_uint_t x, const gen_uint_t y, const S s) noexcept {
      if (s != S::nc) return 0; else return ildexp(x, 64-y);
    }
  };


  // Auxiliary function, checking whether n >= 1 is a power of 2:
  inline constexpr bool powerof2(const gen_uint_t x) noexcept {
    assert(x >= 1);
    return not (x & (x-1));
  }
  static_assert(powerof2(1) and powerof2(2) and not powerof2(3) and
    powerof2(4) and powerof2(iexp2(63)) and not powerof2(5) and
    not powerof2(-1));


  /* Replacement of std::uniform_int_distribution (in order to obtain
     well-defined behaviour); while with the standard we have the usage
       std::uniform_int_distribution<result_type> d(a,b);
       result_type r = d(g);
     for a random number in {a,...,b}, where g is the underlying random number
     generator, here now the type of g is standardised to the 64-bit Mersenne
     twister, result_type is gen_uint_t, and the usage is
       UniformRange U(g, n, start);
       gen_uint_t random = U();
     creating random numbers from {start, ..., n+start-1}.

     If using only U(g,n) (with start=0 by default), then the "range"
     (i.e. half-open interval) [0,n) (including 0, excluding n) is sampled;
     this is the usual treatment of "ranges" in C++, but deviates from
     std::uniform_int_distribution.

     Every use of U() advances the state of g at least once except of the
     trivial case n=1; if n is a power of 2, then g is used exactly once.

     Operators: only <<.
  */
  class UniformRange {
    randgen_t& g;
  public :
    const gen_uint_t n; // size of sampling interval
    const gen_uint_t s; // start of sampling interval
    const bool trivial; // whether n == 1
    const bool p2; // whether n is a power of 2
    const gen_uint_t size_region; // how often the sampling-interval fits
    const gen_uint_t last_regions; // before start of trailing broken interval
    /* The numbers 0,...,randgen_max generated by g() are partioned into n
       consecutive maximum- and equal-sized intervals ("regions") for outcomes
         s, s+1, ..., s+n-1,
       with a trailing region of lesser size beginning after last_regions.
       E.g. let randgen_max := 15 (so we have 4 bits). Now for n=3, s=0:
       size_region = 5, last_regions = 14.
       So 0,...,4 -> 0; 5,...,9 -> 1; 10,...,14 -> 2; 15 -> disposed.
       And for n=2: size_region = 8, last_regions = 15, and so
       0,...,7 -> 0; 8,...,15 -> 1; nothing disposed.
       We have size_region = total_size / n (integer arithmetic), where
       total_size = randgen_max + 1.
       If n is not a divisor of total_size, i.e, n is not a power of 2,
       then size_region = randgen_max / n.
    */
    UniformRange(randgen_t& g, const gen_uint_t n, const gen_uint_t start=0)
      noexcept : g(g), n(n), s(start),
      trivial(n == 1), p2(trivial or powerof2(n)),
      size_region(randgen_max/n + p2),
      last_regions((p2) ? randgen_max : n*size_region - 1) {
      assert(n != 0);
      assert(s <= randgen_max - (n-1));
      assert(not trivial or size_region == 0);
      assert(size_region >= 1 or trivial);
      assert(not p2 or last_regions == randgen_max);
      assert(last_regions != randgen_max or p2);
    }
    UniformRange(const UniformRange& u) = delete;
    gen_uint_t operator ()() const noexcept {
      if (trivial) return s;
      assert(n >= 2);
      gen_uint_t result;
      do result = g(); while (result > last_regions);
      return result/size_region + s;
    }

    friend std::ostream& operator <<(std::ostream& out, const UniformRange& u) {
      return out << u.n << "," << u.s << "," << u.trivial << ","
        << u.p2 << "," << u.size_region << "," << u.last_regions;
    }

  };


  // As std::shuffle, but now fixing the algorithm and the random-generator:
  template <class RandomAccessIterator>
  inline void shuffle(const RandomAccessIterator begin, const RandomAccessIterator end, randgen_t& g) noexcept {
    for (auto i = (end - begin) - 1; i > 0; --i) {
      using std::swap;
      swap(begin[i], begin[UniformRange(g, gen_uint_t(i+1))()]);
    }
  }
  template <class RAI>
  inline void shuffle(const RAI begin, const RAI end, randgen_t&& g) noexcept {
    RandGen::shuffle(begin, end, g);
  }
  // Remark: If randgen_t would also be a template parameter, then just one
  // version would be sufficient, due to "perfect forwarding". Here however
  // we want to be sure that exactly type randgen_t is used.

}

#endif
