// Oliver Kullmann, 16.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
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


TODOS:

1. Using UniformRange U(g), then by U() we do not create all
   possible float80-values x with 0 <= x < 1.

   At https://github.com/DiscreteLogarithm/canonical-random-float
   an algorithm is implemented (in C++) to fix that.
   The implementation
   https://github.com/DiscreteLogarithm/canonical-random-float/blob/master/canonical_float_random.hpp
   is indeed rather simple, so we could mimic that.

*/

#ifndef FPDISTRIBUTIONS_T77fC5Vr08
#define FPDISTRIBUTIONS_T77fC5Vr08

#include <type_traits>

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
      noexcept : g(g), a(a), b(b), d(b-a) { assert(a < b and d > 0); }

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

}

#endif
