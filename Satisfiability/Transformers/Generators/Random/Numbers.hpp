// Oliver Kullmann, 6.7.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Basic components for random numbers

    - Basic types fixing the 64-bit Mersenne Twister:

     - randgen_t is the type of our standard 64-bit random engine
     - gen_uint_t is the type of the generated unsigned 64-bit integers.

    - Basic constants:
     - randgen_max = 2^64-1, max_half_p1 = 2^63
     - specval, specseed are the special generated values as specified by
       the C++ standard
     - valempty_1/2/3/10001/20001/30001 are generated values when seeding
       with the empty seed-sequence.

    To have a large and safe seed-space, seed-sequences should be used
    (not single-valued seeds), which is facilitates by the wrapper
    RandGen_t below.

    - Helper functions for gen_uint_t (all fulfil constexpr):

     - lessP263(x) : x < 2^63 ?
     - lessP232(x) : x < 2^32
     - iexp2(e) = 2^e
     - ildexp(x, e) = x * 2^e
     - powerof2(x) is true iff x is an integer power of 2
     - ilogp2(x): the binary logarithm of integer powers of 2

    - Helper functions for seeding the generator:

     - seed_t is the unsigned 32-bit integer type for a single seed
     - vec_seed_t is a sequence of seeds

     - vec_eseed_t is a sequence of unsigned 64-bit values for seeding
     - is_seed_t(vec_eseed_t v) : returns true iff all values are < 2^32

     - pair_seed_t is a std::pair of seed_t
     - split(gen_uint_t x) splits x into a pair_seed_t (s0,s1)
     - split(x, s0, s1) changes (s0,s1)
     - inc(x,y) for seed_t x, y increments (x,y) as if incrementing the
       underlying gen_uint_t z with split(z) = (x,y).

     - SP is the Split-policy scoped enum
     - transform(vec_eseed_t v, SP p) returns vec_seed_t according to policy

     - EP is the Embed-policy scoped enum
     - transform(std::string s, EP p) returns vec_seed_t according to policy,
       interpreting the characters as integers; via vald_ascii(s) one
       can check whether the codes are platform-independent

     - init(vec_seed_t v) returns a randgen_t initialised with v

     - SW{vec_seed_t} is a wrapper for output-streaming of a vec_seed_t,
       enclosing the numbers in "()" and separating by commas.

    - RandGen_t is a wrapper around randgen_t, allowing only initialisation
      via the above init: the direct initialisation with a single seed
      we better avoid (to avoid unknowlingly duplicated runs).

    - Prob64 is a simple type for precise probabilities, based on fractions of
      unsigned 64-bit integers:
     - toProb64(string_view) creates std::optional(Prob64)
     - pair64 is convertible to and from Prob64 (the latter explicitly; there
       is a non-implicit conversion to FloatingPoint80).

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
    - DONE: Next is TimingBernoulli2, with two further inputs.
    - DONE: Next is TimingBernoulli.
    - Then TimingUniformRange.
    - Having many Timing-programs, updating the version after an update of a
      general library-function is awkward; should there be some more general
      management?
    - All the above need to be applied with the possibility of appending
      a seed-sequence to the end, to check different (valid) seeds.
     - Not needed for TimingDiscard, TimingGeneration, TimingInitialisation.
     - DONE: TimingBernoulli12
     - DONE: TimingBernoulli
    - The final (current) results should go into a subdirectories timings.
    - Likely we need a script/program, which puts the data into timings,
      so that from time to time one can automatically check the results,
      wheter something has changed.
    - Best to develop a model for each Timing-program, obtained by linear
      regression in a standardised way, for each computer.

*/

#ifndef RandGen_ABopVoYw
#define RandGen_ABopVoYw

#include <random>
#include <type_traits>
#include <limits>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <ostream>
#include <string_view>
#include <optional>
#include <numeric>
#include <limits>

#include <cstdint>
#include <cassert>
#include <cmath>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

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

  // The type of a single seed-value:
  typedef std::uint32_t seed_t;

// Special generated values:

  // The numerical values as specified by the C++ standard:
  constexpr gen_uint_t specval = 9981545732273789042ULL; // 10'000th value
  static_assert(specval == 0b1000101010000101100100101111010110000001011111101101100001110010);
  constexpr seed_t specseed = 5489u;

  // The ith generated values using the empty seed-sequence:
  constexpr gen_uint_t valempty_1 = 835052665647855778ULL;
  static_assert(valempty_1 ==     0b0000101110010110101100111111100110100000101011101111100010100010);
  constexpr gen_uint_t valempty_2 = 3190053552572815828ULL;
  static_assert(valempty_2 ==     0b0010110001000101010110001100110111101111100100110101100111010100);
  constexpr gen_uint_t valempty_3 = 4634633302865102305ULL;
  static_assert(valempty_3 ==     0b0100000001010001100001100110111100000001011110000100010111100001);
  constexpr gen_uint_t valempty_10001 = 12990417185246102803ULL;
  static_assert(valempty_10001 == 0b1011010001000111001110111001101010010010101110010100010100010011);
  constexpr gen_uint_t valempty_20001 = 17222733215482709862ULL;
  static_assert(valempty_20001 == 0b1110111100000011011100001001110000001010001010110000011101100110);
  constexpr gen_uint_t valempty_30001 = 3712653940750131205ULL;
  static_assert(valempty_30001 == 0b0011001110000101111111110001011010110000010111011010000000000101);


  // Auxiliary functions for integral/bit operations

  // Testing x < max_half_p1 = 2^63:
  inline constexpr bool lessP263(const gen_uint_t x) noexcept {
    return not (x >> 63);
  }
  static_assert(lessP263(0));
  static_assert(not lessP263(randgen_max));
  static_assert(lessP263(max_half_p1 -1));
  static_assert(not lessP263(max_half_p1));

  // Testing x < 2^32:
  inline constexpr bool lessP232(const gen_uint_t x) noexcept {
    return not (x >> 32);
  }
  static_assert(lessP232(0));
  static_assert(not lessP232(randgen_max));

  // Integral binary powers:
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

  // Checking whether n >= 1 is a power of 2:
  inline constexpr bool powerof2(const gen_uint_t x) noexcept {
    assert(x >= 1);
    return not (x & (x-1));
  }
  static_assert(powerof2(1) and powerof2(2) and not powerof2(3) and
    powerof2(4) and powerof2(iexp2(63)) and not powerof2(5) and
    not powerof2(-1));

  // The binary logarithm of a binary power:
  inline constexpr gen_uint_t ilogp2(const gen_uint_t x) noexcept {
    assert(powerof2(x));
    return std::ilogb(x);
    // If constexpr is not needed, then
    //   return std::bitset<64>(x-1).count();
    // is faster (see Timingilogp2.cpp).
  }
  static_assert(ilogp2(1) == 0);
  static_assert(ilogp2(2) == 1);
  static_assert(ilogp2(iexp2(63)) == 63);


  // Seeding with a sequence of values

  // The type of the sequence of values:
  typedef std::vector<seed_t> vec_seed_t;
  // Extended seed-sequences:
  typedef std::vector<gen_uint_t> vec_eseed_t;

  // Checking whether an vec_eseed_t is indeed a vec_seed_t:
  inline bool is_seed_t(const vec_eseed_t& v) noexcept {
    return std::find_if_not(v.begin(), v.end(), lessP232) == v.end();
  }

  // Splitting a 64-bit seed into two 32-bits seeds, as a pair, with
  // first as the lower-order part:
  typedef std::pair<seed_t,seed_t> pair_seed_t;
  inline constexpr pair_seed_t split(const gen_uint_t x) noexcept {
    return {x, x >> 32};
  }
  static_assert(split(0) == pair_seed_t{0,0});
  static_assert(split(1) == pair_seed_t{1,0});
  static_assert(split(iexp2(32)) == pair_seed_t{0,1});
  static_assert(split(iexp2(63)) == pair_seed_t{0, iexp2(31)});
  static_assert(split(randgen_max) == pair_seed_t{seed_t(-1),seed_t(-1)});
  inline void split(const gen_uint_t x, seed_t& s0, seed_t& s1) noexcept {
    s0 = x; s1 = x >> 32;
  }

  // Incrementing a pair of seeds, taking s0 as the low-order part:
  inline void inc(seed_t& s0, seed_t& s1) noexcept {
    if (s0 != FloatingPoint::P232m1) ++s0;
    else { s0 = 0; ++s1; }
  }

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
  // "four": four chars in one seed_t
  // "one" : one char in one seed_t
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

  inline gen_uint_t to_eseed(const std::string& s) noexcept {
    if (s == "r") {
      typedef unsigned int rand_t;
      const rand_t n{std::random_device()()};
      if constexpr(std::numeric_limits<rand_t>::digits < 64) {
        const rand_t n2{std::random_device()()};
        return n + ildexp(n2,32);
      }
      else return n;
    }
    else {
      typedef unsigned long long rand_t;
      const rand_t n {
        s=="t" ? Environment::CurrentTime::timestamp() : std::stoull(s)
      };
      constexpr rand_t P264{rand_t(randgen_max) + 1};
      if constexpr(P264 == 0) return n;
      else {
        const auto [div, mod] = std::lldiv(n, P264);
        return gen_uint_t(div) ^ gen_uint_t(mod);
      }
    }
  }


  inline randgen_t init(const vec_seed_t& v) {
    std::seed_seq s(v.begin(), v.end());
    return randgen_t(s);
  }

  // Seed-Wrap:
  struct SW { const vec_seed_t& v; };
  std::ostream& operator <<(std::ostream& out, const SW& sw) {
    const auto& v{sw.v};
    if (v.empty()) return out << "()";
    auto i = v.begin();
    out << "(" << *i++;
    while (i != v.end()) out << "," << *i++;
    return out << ")";
  }


  /* Wrapper around random-generator g, providing initialisation with
     a sequence of seeds only.
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


  // Probabilities as 64-bit fractions

  typedef std::pair<gen_uint_t, gen_uint_t> pair64;

  class Prob64 {
    gen_uint_t nom_, den_; // nom <= den, den >= 1, gcd(nom,den) = 1
  public :
    constexpr gen_uint_t nom() const noexcept { return nom_; }
    constexpr gen_uint_t den() const noexcept { return den_; }

    constexpr bool dyadic() const noexcept { return powerof2(den_); }
    constexpr bool zero() const noexcept { return nom_ == 0; }
    constexpr bool one() const noexcept { return nom_ == den_; }
    constexpr bool constant() const noexcept { return den_ == 1; }

    constexpr Prob64(const gen_uint_t n, const gen_uint_t d) noexcept :
      nom_(n / std::gcd(n,d)), den_(d / std::gcd(n,d)) {
      assert(d >= 1 and n <= d);
      assert(den_ >= 1);
      assert(nom_ <= den_);
    }
    constexpr Prob64(const pair64 p) noexcept : Prob64(p.first, p.second) {}

    explicit constexpr operator pair64() const noexcept { return {nom_,den_}; }

    constexpr operator FloatingPoint::float80() const noexcept {
      return FloatingPoint::float80(nom_) / den_;
    }

    friend constexpr bool operator ==(const Prob64 lhs, const Prob64 rhs) noexcept {
      return lhs.nom_ == rhs.nom_ and lhs.den_ == rhs.den_;
    }
    friend constexpr bool operator !=(const Prob64 l, const Prob64 r) noexcept {
      return not (l == r);
    }

  };
  static_assert(pair64(Prob64(4,16)) == pair64(1,4));
  static_assert(pair64(Prob64(10,20)) == pair64{1,2});
  static_assert(pair64(Prob64(14,60)) == pair64{7,30});
  static_assert(double(Prob64(1,8)) == 0.125);
  static_assert(FloatingPoint::float80(Prob64(1,4)) == 0.25);
  static_assert(Prob64(0,1).dyadic());
  static_assert(Prob64(1,1).dyadic());
  static_assert(Prob64(15,60).dyadic());
  static_assert(not Prob64(14,60).dyadic());
  static_assert(Prob64(0,2).zero());
  static_assert(Prob64(0,100).zero());
  static_assert(not Prob64(1,randgen_max).zero());
  static_assert(Prob64(randgen_max, randgen_max).one());
  static_assert(not Prob64(randgen_max-1, randgen_max).one());
  static_assert(not Prob64(1,2).one());
  static_assert(Prob64(0,1).constant());
  static_assert(Prob64(0,4).constant());
  static_assert(Prob64(4,4).constant());
  static_assert(Prob64(1,1).constant());
  static_assert(not Prob64(1,2).constant());
  static_assert(Prob64(4,8) == Prob64(1,2));

  inline std::ostream& operator <<(std::ostream& out, const Prob64 p) {
    return out << p.nom() << "/" << p.den();
  }

  static_assert(Prob64(0,1) == Prob64(0,2));
  static_assert(Prob64(1,1) == Prob64(3,3));
  static_assert(Prob64(0,1) != Prob64(1,1));

  /* Constructing a Prob64 p from a string-view s:
      - s must be of the form "nom/den";
      - in case of missing "/" returns empty optional;
      - nom, den must be interpretable as FloatingPoint::float80 values,
        otherwise exceptions are thrown by std::stold;
      - uses FloatingPoint::toUInt, which interpretes every too-big float80
        as 2^64-1, and every negative float80 is 0, and rounds otherwise;
      - returns empty optional if den = 0 or nom > den.
  */
  std::optional<Prob64> toProb64(const std::string_view s) {
    std::optional<Prob64> res;
    constexpr auto npos = std::string_view::npos;
    const auto posdiv = s.find_first_of('/');
    if (posdiv == npos or posdiv == s.size()-1) return res;
    using FloatingPoint::toUInt;
    const auto nom = toUInt(std::string(s.substr(0,posdiv)));
    const auto den = toUInt(std::string(s.substr(posdiv+1)));
    if (den == 0 or nom > den) return res;
    res.emplace(Prob64{nom,den});
    return res;
  }

}

#endif
