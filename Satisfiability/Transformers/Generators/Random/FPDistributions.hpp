// Oliver Kullmann, 16.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Distributions for random floating-point numbers


   See Distributions.hpp for integer-distributions.


TODOS:

1. Can the inprecision of Uniform80Range be repaired?
    - In TestFPDistributions.hpp we see that for
        a = 1, b = 1 + epsilon
      we can get output b (by internal rounding).
    - Can this be repaired?
    - If b is the next float80-number of a, then we could always
      output a.
    - And, more generally, if b is just a small number of steps from a,
      then one could use directly a discrete uniform distributions on these
      steps.

*/

#ifndef FPDISTRIBUTIONS_T77fC5Vr08
#define FPDISTRIBUTIONS_T77fC5Vr08

#include <type_traits>

#include <cassert>

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

     If a, b are very close together, then x == b is possible (and so then
     the inclusive form Uniform80RangeI might be more appropriate).

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
      const float80 r = float80(g()) / FloatingPoint::P264;
      assert(0 <= r and r < 1);
      return FloatingPoint::fma(r, d, a);
    }

  };

  // The above creates random r with a <= r < b, now we have also r <= b:
  template <class RG>
  class Uniform80RangeI {
    static_assert(std::is_same_v<RG,RandGen_t>
                  or std::is_same_v<RG, randgen_t>);
    RG& g;
  public :
    typedef RG rg_t;
    typedef FloatingPoint::float80 float80;
    const float80 a, b, d;

    explicit Uniform80RangeI(rg_t& g, const float80 a=0, const float80 b=1)
      noexcept : g(g), a(a), b(b), d(b-a) {
        assert((a < b and d > 0) or (a == b and d == 0));
      }

    float80 operator ()() const noexcept {
      if (d == 0) return a;
      const auto r0 = g();
      if (r0 == 0) return a;
      else if (r0 == FloatingPoint::P264m1) return b;
      const float80 r = float80(r0) / FloatingPoint::P264m1;
      assert(0 < r and r < 1);
      return FloatingPoint::fma(r, d, a);
    }

  };

}

#endif
