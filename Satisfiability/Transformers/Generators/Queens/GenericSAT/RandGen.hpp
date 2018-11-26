// Oliver Kullmann, 6.7.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef RandGen_ABopVoYw
#define RandGen_ABopVoYw

#include <random>
#include <limits>
#include <vector>

#include <cstdint>
#include <cassert>

namespace RandGen {

  typedef std::mt19937_64 randgen_t;

  static_assert(randgen_t::min() == 0, "Min-value of std::mt19937_64");
  static_assert(randgen_t::max() == std::numeric_limits<std::uint64_t>::max(), "Absolute max-value of std::mt19937_64");

  typedef randgen_t::result_type gen_uint_t;
  static_assert(randgen_t::max() == std::numeric_limits<gen_uint_t>::max(), "Max-value of std::mt19937_64");

  /* Replacement of std::uniform_int_distribution (in order to obtain
     well-defined behaviour); while with the standard we have the usage
       std::uniform_int_distribution<result_type> d(0,n);
       result_type r = d(g);
     for a random number in {0,...,n}, where g is the underlying random number
     generator, here now the type of g is standardised to the 64-bit Mersenne
     twister, result_type is gen_uint_t, and the usage is
       Uniform U(g, n);
       gen_uint_t random = U();
     Every use of U() advances the state of g.
  */
  class Uniform {
    randgen_t& g;
    const gen_uint_t n;
    const gen_uint_t us;
    const gen_uint_t past;
  public :
    Uniform(randgen_t& g, const gen_uint_t n) noexcept : g(g), n(n),
       us(std::numeric_limits<gen_uint_t>::max() / n), past(n * us) {
      assert(n != 0);
    }
    gen_uint_t operator ()() const noexcept {
      gen_uint_t result;
      do result = g(); while (result >= past);
      return result/us + 1;
    }
  };

  typedef std::uint32_t seed_t;
  typedef std::vector<seed_t> vec_seed_t;
  /* In order to create a generator g initialised by the seed sequence
     s_1, ..., s_m, m >= 0, use

       vec_seed_t v{s_1, ..., s_m};
       std::seed_seq seq(v.begin(), v.end());
       randgen_t g(seq);
  */

  template <class RandomAccessIterator>
  void shuffle(const RandomAccessIterator begin, const RandomAccessIterator end, randgen_t&& g) noexcept {
    for (auto i = (end - begin) - 1; i > 0; --i)
      std::swap(begin[i], begin[Uniform(g, gen_uint_t(i))()]);
  }

}

#endif
