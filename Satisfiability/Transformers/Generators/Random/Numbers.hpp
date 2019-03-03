// Oliver Kullmann, 6.7.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Components for random numbers

    - randgen_t is the type of our standard 64-bit random engine
    - gen_uint_t is the type of the generated unsigned 64-bit integers

    - class Uniform for generation of uniform random numbers from 0, ..., n

    - seed_t, vec_seed_t for seeding with a sequence of 32-bit unsigned
      integers

    - algorithm shuffle for shuffling a sequence.

   Originally adapted from the file TwoCNF/RandGen (created 2017 by OK).

*/

#ifndef RandGen_ABopVoYw
#define RandGen_ABopVoYw

#include <random>
#include <limits>
#include <vector>
#include <utility>

#include <cstdint>
#include <cassert>

namespace RandGen {

  // The type of the random-engine:
  typedef std::mt19937_64 randgen_t;
  // The type of the random unsigned integers produced by randgen_t:
  typedef randgen_t::result_type gen_uint_t;

  constexpr gen_uint_t randgen_max{randgen_t::max()};
  constexpr gen_uint_t max_half_p1{randgen_max / 2 + 1};

  static_assert(randgen_t::min() == 0);
  static_assert(randgen_max == std::numeric_limits<std::uint64_t>::max());
  static_assert(randgen_max == 0xFFFF'FFFF'FFFF'FFFFL);
  static_assert(randgen_max == gen_uint_t(-1));
  static_assert(max_half_p1 == 0x8000'0000'0000'0000L);


  // Returns true/false with probability 1/2:
  inline bool bernoulli(randgen_t& g) noexcept { return g() < max_half_p1; }


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
     Every use of U() advances the state of g at least once.
  */
  class UniformRange {
    randgen_t& g;
    const gen_uint_t n;
    const gen_uint_t s;
    const gen_uint_t us;
    const gen_uint_t past;
  public :
    UniformRange(randgen_t& g, const gen_uint_t n, const gen_uint_t start=0)
      noexcept : g(g), n(n), s(start), us(randgen_max / n), past(n * us) {
      assert(n != 0);
      assert(s <= randgen_max - (n-1));
    }
    gen_uint_t operator ()() const noexcept {
      gen_uint_t result;
      do result = g(); while (result >= past);
      return result/us + s;
    }
  };


  // Seeding with a sequence of values

  // The type of a single value:
  typedef std::uint32_t seed_t;
  // The type of the sequence of values:
  typedef std::vector<seed_t> vec_seed_t;
  /* In order to create a generator g initialised by the seed sequence
     s_1, ..., s_m, m >= 0, one needs an l-value of type std::seed_seq,
     calling it seq, obtained by

       vec_seed_t v{s_1, ..., s_m};
       std::seed_seq seq(v.begin(), v.end());

     or (equivalently)
       std::seed_seq seq{s_1, ..., s_m};

     Note that in general the s_i are taken mod 2^32, which is not suitable
     in general, and instead larger s_i should be split into 2^32-portions
     (via mod and div).

     Usage:
       randgen_t g(seq);
  */


  // As std::shuffle, but now fixing the algorithm:
  template <class RandomAccessIterator>
  inline void shuffle(const RandomAccessIterator begin, const RandomAccessIterator end, randgen_t& g) noexcept {
    for (auto i = (end - begin) - 1; i > 0; --i) {
      using std::swap;
      swap(begin[i], begin[UniformRange(g, gen_uint_t(i+1))()]);
    }
  }
  template <class RAI>
  inline void shuffle(const RAI begin, const RAI end, randgen_t&& g) noexcept {
    shuffle(begin, end, g);
  }
  // Remark: If randgen_t would also be a template parameter, then just one
  // version would be sufficient, due to "perfect forwarding". Here however
  // we want to be sure that exactly type randgen_t is used.

}

#endif
