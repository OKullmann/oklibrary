// Oliver Kullmann, 6.7.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Basic components for random numbers

    - randgen_t is the type of our standard 64-bit random engine
    - gen_uint_t is the type of the generated unsigned 64-bit integers
    - constants randgen_max = 2^64-1, max_half_p1 = 2^63.

    - Helper functions for gen_uint_t:
     - iexp2(e) = 2^e
     - ildexp(x, e) = x * 2^e
     - powerof2(x) is true iff x is an integer power of 2.

    - seed_t, vec_seed_t for seeding with a sequence of 32-bit unsigned
      integers.

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

}

#endif
