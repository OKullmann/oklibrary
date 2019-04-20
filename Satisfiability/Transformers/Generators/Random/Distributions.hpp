// Oliver Kullmann, 15.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Distributions for random numbers

   Bernoulli distributions:

    - Random bools with probability 1/2:
     - Function bernoulli(g) taking a randgen_t or a RandGen_t by reference.
     - Class bernoulliS is a convenience wrapper for handling the generator.
     - function bernoulli_high(g) uses all bits from the call of g() (not just
       the highest-order bit as above), to produce one bool
     - Class bernoulli_low(g) produces one bool per bit (storing the 64-bit
       random numbers and the current position).

    - Class Bernoulli2 for random bool with dyadic probability (here we
      can still guarantee well-definedness, and this with exactly one
      generator-call).

    - Class Bernoulli for arbitrary probabilities p = nom/den, with
      nom, den unsigned 64-bit integers: again well-defined, and coinciding
      for nom, which are powers of 2, with Bernoulli2 (thus here exactly
      one generator-call per generation).

      While class Bernoulli takes a RandGen_t by reference, the class
      BernoulliS takes a seed-sequence instead, and creates the generator
      internally.

      Either bernoulli or Bernoulli(S) should normally be used.

    Uniform distributions:

    - Class UniformRange for generation of uniform random numbers from
      0, ..., n-1. Also well-defined, but for n which are not powers of
      two, possibly more than one generator-call is needed.

TODOS:

*/

#ifndef DISTRIBUTIONS_6S09j6DxLm
#define DISTRIBUTIONS_6S09j6DxLm

#include <bitset>
#include <ostream>
#include <type_traits>

#include <cassert>
#include <cstdint>
#include <cinttypes>

// Guaranteed to be included:
#include "Numbers.hpp"

namespace RandGen {

  // Returns true/false with probability 1/2, using exactly one call of g:
  inline bool bernoulli(randgen_t& g) noexcept { return lessP263(g()); }
  inline bool bernoulli(RandGen_t& g) noexcept { return lessP263(g()); }
  // Convenience wrapper:
  struct bernoulliS {
    RandGen_t g;
    bernoulliS() = default;
    explicit bernoulliS(const vec_seed_t& seed) noexcept : g(seed) {}
    bool operator ()() noexcept { return bernoulli(g); }
  };

  // The above uses exactly one bit of g(), the negation of the highest-order
  // one; now using the xor of all bits:
  template <class RG>
  inline bool bernoulli_high(RG& g) noexcept {
    static_assert(std::is_same_v<RG,RandGen_t> or std::is_same_v<RG,randgen_t>);
    return std::bitset<64>(g()).count() % 2 == 1;
  }

  // The other extreme, using all bits for 64 outputs:
  template <class RG>
  class bernoulli_low {
    static_assert(std::is_same_v<RG,RandGen_t> or std::is_same_v<RG,randgen_t>);
    RG& g;
    typedef std::uint_fast8_t index_t;
    index_t i = 0; // next index (0 <= i < 64)
    std::bitset<64> bv;
  public :
    typedef RG rg_t;
    explicit bernoulli_low(rg_t& g) noexcept : g(g), bv(g()) {}

    bool operator ()() noexcept {
      assert(i < 64);
      const bool res = bv[i];
      if (++i == 64) { i = 0; bv = g(); }
      return res;
    }

    void discard(const unsigned long long z) noexcept {
      assert(i < 64);
      const auto d = std::lldiv(z, 64);
      const index_t rem = d.rem + i;
      if (rem < 64) {
        if (d.quot != 0) {
          g.discard(d.quot-1);
          bv = g();
        }
        i = rem;
      }
      else {
        g.discard(d.quot);
        bv = g();
        i = rem - 64;
      }
    }

  };
  struct bernoulli_lowS {
    RandGen_t g;
    bernoulli_low<RandGen_t> b;

    bernoulli_lowS() noexcept : b(g) {}
    explicit bernoulli_lowS(const vec_seed_t& seed) noexcept : g(seed), b(g) {}

    bool operator ()() noexcept { return b(); }

    friend bool operator ==(const bernoulli_lowS& lhs, const bernoulli_lowS& rhs) noexcept {
      return lhs.g == rhs.g;
    }
    friend bool operator !=(const bernoulli_lowS& lhs, const bernoulli_lowS& rhs) noexcept {
      return not(lhs == rhs);
    }

  };


  /* Class Bernoulli2, generalising bernoulli(g) for dyadic p

     Here the propability is given by p = x / 2^y for returning tree
     (using real-number arithmetic for the mathematical specification,
     and exact handling in the implementation), now employing a functor:
      - y is integer with 0 <= y <= 63
      - x is integer with 0 <= x <= 2^y.
      - Construct by
          Bernoulli2 b(g, x, y);
        (const or not).
      - Call as
          b()
      - This class can be simulated by UniformRange (see Test.cpp), but is
        more efficient.
      - The generator g is called exactly once for each b(), except of the
        cases with probabilities 0 or 1 (where there are no calls).
  */
  class Bernoulli2 {
    randgen_t& g;
  public :
    enum class S {c0, c1, nc }; // constant 0/1, or non-constant
    const S s;
    const gen_uint_t threshold;
    Bernoulli2(randgen_t& g, const gen_uint_t x, const gen_uint_t y) noexcept :
      g(g), s(set_S(x,y)), threshold(set_t(x,y,s)) {}
    Bernoulli2(const Bernoulli2& b) = delete;
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
      assert(s == set_S(x,y));
      if (s != S::nc) return 0; else return ildexp(x, 64-y);
    }
  };
  static_assert(Bernoulli2::set_S(0,0) == Bernoulli2::S::c0);
  static_assert(Bernoulli2::set_S(1,0) == Bernoulli2::S::c1);
  static_assert(Bernoulli2::set_S(0,1) == Bernoulli2::S::c0);
  static_assert(Bernoulli2::set_S(1,1) == Bernoulli2::S::nc);
  static_assert(Bernoulli2::set_S(2,1) == Bernoulli2::S::c1);
  static_assert(Bernoulli2::set_t(1,1,Bernoulli2::S::nc) == iexp2(63));


  /* Generalising Bernoulli2, now allowing arbitrary 64-bit fractions p

     The size of the "broken interval" is, for p = m/n (m. n relatively prime)
       2^64 % n = randgen_max - last_valid,
     where last_valid = set_l(p, s), using s for the classification of p
     as 0, 1, n prime-power, other; s = set_S(p).
     The probability of a discarded generator-call for p = m/n (m, n being
     relatively prime), i.e., that the generatored random number falls into
     the broken interval, thus is (as real number)
       (2^64 % n) / 2^64,
     which is maximal for n = 2^63 + 1, where then the probability is
     (2^63-1) / 2^64 ~ 0.5.
  */
  class Bernoulli {
    RandGen_t& g;
    gen_uint_t last_generated = 0;
    gen_uint_t rejected_ = 0;
  public :
    enum class S {c0=0, c1=1, dy=2, o=3 }; // constant 0/1, dyadic, other
    static constexpr bool constant(const S s) noexcept {
      return s==S::c0 or s==S::c1;
    }

    const S s;
    const gen_uint_t threshold;
    const gen_uint_t last_valid;

    Bernoulli(RandGen_t& g, const Prob64 p) noexcept :
      g(g), s(set_S(p)), threshold(set_t(p,s)), last_valid(set_l(p,s)) {}
    Bernoulli(const Bernoulli& b) = delete;

    bool operator ()() noexcept {
      switch (s) {
      case S::c0 : return false;
      case S::c1 : return true;
      case S::dy : return g() < threshold;
      default :
        while ((last_generated = g()) > last_valid) ++rejected_;
        return last_generated < threshold;
      }
    }
    gen_uint_t last() const noexcept { return last_generated; }
    gen_uint_t rejected() const noexcept { return rejected_; }

    static constexpr S set_S(const Prob64 p) noexcept {
      if (p.zero()) return S::c0;
      if (p.one()) return S::c1;
      if (p.dyadic()) return S::dy;
      return S::o;
    }
    static constexpr gen_uint_t set_t(const Prob64 p, const S s) noexcept {
      assert(s == set_S(p));
      if (constant(s)) return 0;
      if (s == S::dy) return ildexp(p.nom, 64 - ilogp2(p.den));
      return p.nom * (randgen_max / p.den);
    }
    static constexpr gen_uint_t set_l(const Prob64 p, const S s) noexcept {
      assert(s == set_S(p));
      if (s != S::o) return randgen_max;
      return p.den * (randgen_max / p.den) - 1;
    }
  };
  // (2^64-1) - last_valid = 2^64 % p.nom :
  static_assert(randgen_max - Bernoulli::set_l({1,3},Bernoulli::S::o) == 1);
  static_assert(randgen_max - Bernoulli::set_l({2,6},Bernoulli::S::o) == 1);
  static_assert(randgen_max - Bernoulli::set_l({1,5},Bernoulli::S::o) == 1);
  static_assert(randgen_max - Bernoulli::set_l({2,10},Bernoulli::S::o) == 1);
  static_assert(randgen_max - Bernoulli::set_l({1,6},Bernoulli::S::o) == 4);
  static_assert(randgen_max - Bernoulli::set_l({77,1007},Bernoulli::S::o) == 492);
  static_assert(randgen_max - Bernoulli::set_l({777212,10000000019ULL},Bernoulli::S::o) == 8660737959ULL);

  // Convenience wrapper, handling the generator:
  struct BernoulliS {
    RandGen_t g;
    Bernoulli b;
    BernoulliS(const Prob64 p, const vec_seed_t& seed) noexcept
      : g(seed), b(g,p) {}
    bool operator ()() noexcept { return b(); }
  };


  /* Uniform integer distribution

     Replacement of std::uniform_int_distribution:
      - With the C++-standard we have the usage
          std::uniform_int_distribution<result_type> d(a,b);
          result_type r = d(g);
        for a random number in {a,...,b}, where g is the underlying random
        number generator.
      - Here now the type of g is standardised to the 64-bit Mersenne
        twister, result_type is gen_uint_t, and the usage is
          UniformRange U(g, n, start);
          gen_uint_t random = U();
        creating random numbers from {start, ..., n+start-1}.

     If using only U(g,n) (with start=0 by default), then the "range"
     (i.e. half-open interval) [0,n) (including 0, excluding n) is sampled;
     this is the usual treatment of "ranges" in C++, but deviates from
     std::uniform_int_distribution.

     Every use of U() advances the state of g at least once except of the
     trivial case n=1, where nothing happens; if n>1 is a power of 2, then
     g is used exactly once. The probability of a discarded generator-call
     is (as real number)
       (2^64 % n) / 2^64,
     which is maximal for n = 2^63 + 1, where then the probability is
     (2^63-1) / 2^64 ~ 0.5.

     Operators: only <<.
  */
  template <class RG>
  class UniformRange {
    static_assert(std::is_same_v<RG,RandGen_t> or std::is_same_v<RG, randgen_t>);
    RG& g;
  public :
    typedef RG rg_t;
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
       While last_regions = randgen_max - 2^64 % n.
       The size of the "broken interval" is
         2^64 % n (= randgen_max - last_regions),
       which is the same as for Bernoulli(1,n) (and thus the sequence of the
       calls of the generator g will be exactly the same).
    */
    UniformRange(rg_t& g, const gen_uint_t n, const gen_uint_t start=0)
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

  struct UniformRangeS {
    RandGen_t g;
    UniformRange<RandGen_t> r;
    UniformRangeS(const gen_uint_t n, const vec_seed_t& seed, const gen_uint_t start = 0) noexcept
      : g(seed), r(g, n, start) {}
    gen_uint_t operator ()() const noexcept { return r(); }
  };

}

#endif
