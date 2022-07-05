// Oliver Kullmann, 16.4.2021 (Swansea)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Distributions for random floating-point numbers

   See Distributions.hpp for integer-distributions.

   Components:

    - Via
        Uniform80Range U(g, a, b);
      one obtains a generator U() producing float80-numbers x in the interval
      a <= x < b.
    - Via
        Uniform80RangeI U(g, a, b);
      one obtains a generator U() producing float80-numbers x in the interval
      a <= x <= b.
   The values are quite well equally distributed, but not all possible
   values are obtained (the denominator is 2^-64).

    - function cummulative_probabilities(s) for a probability-distribution s
      computes the vector of cummulative probabilities of the same length;
    - Discrete D(g, s) then yields a generator D() for random number from
      {0, ..., size(s)-1} according to the distribution s (by the simplest
      algorithm, the "roulette wheel").


TODOS:

1. Using UniformRange U(g), then by U() we do not create all
   possible float80-values x with 0 <= x < 1.

   At https://github.com/DiscreteLogarithm/canonical-random-float
   an algorithm is implemented (in C++) to fix that.
   The implementation
   https://github.com/DiscreteLogarithm/canonical-random-float/blob/master/canonical_float_random.hpp
   is indeed rather simple, so we could mimic that.

2. Improve function cummulative_probabilities
    - The kbn-algorithm as in Generators/Statistics.hpp is used here.
    - One could also sort the probabilities.
    - And one could work from both sides (which should be better than the
      current version, which works only from the left, and then "nails"
      the right end).
    - For more time-efficient algorithms (when calling the generator many
      times) apparently Vose's algorithm seems best; see
      https://www.keithschwarz.com/darts-dice-coins/
      for on overview.

*/

#ifndef FPDISTRIBUTIONS_T77fC5Vr08
#define FPDISTRIBUTIONS_T77fC5Vr08

#include <type_traits>
#include <vector>

#include <cassert>
#include <cfenv>

// Guaranteed to be included:
#include "Numbers.hpp"

#include <Numerics/FloatingPoint.hpp>

namespace RandGen {

  /* Uniform float80 distribution

     Replacement of std::uniform_real_distribution:
      - With the C++-standard we have the usage
          std::uniform_real_distribution<RealType> d(a,b);
          RealType r = d(g);
        for a random number x with a <= x < b, where g is the underlying random
        number generator.
      - Here now the type of g is standardised to the 64-bit Mersenne
        twister, result_type is FP::float80, and the usage is
          Uniform80Range U(g, a, b);
          FP::float80 random = U().

     It should indeed always hold a <= x < b, for all a < b.

    */

  template <class RG>
  class Uniform80Range {
    static_assert(std::is_same_v<RG,RandGen_t>
                  or std::is_same_v<RG, randgen_t>);
    RG& g;
  public :
    typedef RG rg_t;
    typedef FloatingPoint::float80 float80;
    const float80 a, b, d;

    explicit Uniform80Range(rg_t& g, const float80 a=0, const float80 b=1)
      noexcept : g(g), a(a), b(b), d(b-a) {
        assert(a < b and d > 0);
        assert(d < FloatingPoint::pinfinity);
      }

    float80 operator ()() const noexcept {
      const int mode = std::fegetround();
      std::fesetround(FE_DOWNWARD);
      const float80 r = float80(g()) / FloatingPoint::P264;
      assert(0 <= r and r < 1);
      const float80 res = FloatingPoint::fma(r, d, a);
      std::fesetround(mode);
      return res;
    }

  };

  // The above creates random r with a <= r < b, now we have also r <= b:
  template <class RG>
  class Uniform80RangeI {
    static_assert(std::is_same_v<RG,RandGen_t>
                  or std::is_same_v<RG, randgen_t>);
  public :
    typedef FloatingPoint::float80 float80;
    Uniform80Range<RG> U;

    explicit Uniform80RangeI(RG& g, const float80 a=0, const float80 b=1)
      noexcept :
      U(g, a, FloatingPoint::nextafter(b, FloatingPoint::pinfinity)) {}

    float80 operator ()() const noexcept {
      if (U.a == U.b) return U.a;
      return U();
    }

  };


  // More precise summation (as in RandGen::sum_kbn):
  template <class VEC>
  std::vector<FloatingPoint::float80> cummulative_probabilities(const VEC& s) {
    typedef typename VEC::size_type size_t;
    const size_t size = s.size();
    assert(size > 0);
    std::vector<FloatingPoint::float80> res(size);
    res[0] = s[0]; res[size-1] = 1;
    FloatingPoint::float80 sum = s[0], c = 0;
    for (size_t i = 1; i < size-1; ++i) {
      const FloatingPoint::float80 x = s[i];
      assert(x >= 0 and x <= 1);
      const auto t = sum + x;
      if (sum >= x) c += (sum - t) + x;
      else c += (x - t) + sum;
      sum = t;
      res[i] = sum + c;
    }
    return res;
  }

  /* Construction with vector s of size N > 0, return an element
     of 0, ..., N-1 according to the probabilities in s, by operator ().
  */
  template <class RG>
  class Discrete {
    static_assert(std::is_same_v<RG,RandGen_t>
                  or std::is_same_v<RG, randgen_t>);
    RG& g;
  public :
    typedef RG rg_t;
    typedef FloatingPoint::float80 float80;
    typedef std::vector<float80> vec_t;
    const vec_t cump;
    const gen_uint_t N;

    template <class VEC>
    Discrete(rg_t& g, const VEC& s) noexcept :
      g(g), cump(cummulative_probabilities(s)), N(s.size()) { assert(N > 0); }
    Discrete(rg_t& g, const vec_t& s) noexcept :
      g(g), cump(cummulative_probabilities(s)), N(s.size()) { assert(N > 0); }

    gen_uint_t operator ()() const noexcept {
      const float80 r = Uniform80Range(g)();
      assert(r >= 0 and r < 1);
      gen_uint_t i = 0;
      while (r >= cump[i]) ++i;
      return i;
    }
  };

}

#endif
