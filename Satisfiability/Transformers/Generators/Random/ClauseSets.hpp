// Oliver Kullmann, 17.4.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Random clause-sets

DESIGN:

I Seeding

Accidentally using the same seeds should be avoided.

 1. So all parameters go into the seed-sequence.
 2. Using always extended seed-values (64 bits).
 3. In decreasing order of generality; starting with the "type" of seeding.

    Using four values:
     - main type (e.g., constant-density clause-sets),
     - sub-type (e.g., could be the global options)
     - number of blocks
     - thread-index (default: 0).

    Called "type-seed".

 4. The user-provided seeds (arbitrarily many, including none) are appended.
 5. For the standard model after the first type-seed come (in brackets
    the number of related seed-values):
     1. n (2)
     2. k (1)
     3. c (1)
     4. p (2)
    This is one block(n,k,c,p).
 6. Should this also accommodate the extension to DQCNF? Then one also needs
    the number of quantifier blocks (1 for CNF). NO:
    And there are the dependency-specifications. Perhaps this generator is
    conceptually similar to the DQCNF, but actually the instances should be
    independent (already guaranteed by the type-seed), and thus there is no
    need to anticipate here the bigger seed-space for the DQCNF-versions.


II Blocks of parameters

One block of parameter describes the construction of one clause-set.
One can have several such blocks (at least one), where the clause-sets
are appended, and the Dimacs-parameter-value reflect the whole construct
(summing up the number of clauses, and taking the maximum over the sets of
variables).

The blocks each have their parameters (n,k,c,p) as seeds.
"n", as discussed below, can be an interval.

Actually one can allow zero blocks (for the empty clause-set).



III Global options

 - We have two dimensions:
  - "sorted" (with removal of duplications), stronger "sorted with rejection"
    (making sure that the numbers c of clauses are reached)
  - rename (eliminating gaps and non-occurring variables).
 - This makes 3*2=6 possibilities. Could be realised as a pair of
   enum-classes.
  - In order for that to work with the system in ProgramOptions::Environment,
    one needed to wrap that pair in a class which makes it look like an
    enum-class. This means that in can be constructed from an int, and
    converted to an int.


IV Variables

Either parameter n: 1, ..., n
or n1,n2: n1, ..., n2.

As seed just always a pair n1, n2 (with n1=1 in the first case).

Allowed is the full range for 64-bit; literals are pairs (var,sign).
A comment specifies the smallest bit-number from 8,16,32,64,128 which
safely contains n+1 (- - (n+1) == n+1).


V Sign probabilities

A Prob64 value p, by default 1/2, for the probability of a positive literal.


VI The number of clauses

One 64-bit value c.


VII The clause-length

One 64-bit value k.


VIII The random-number generators

Using one RandGen_t g1 for the construction of the clauses.
And one RandGen_t g2 for the signs (which uses bernoulli_low in the case
p = 1/2).
 - Is having two generators really useful? It seems to only complicate things.
 - For parallelisation via threads, the result will depend (in a well-defined
   way) on the number of threads, and for each thread one generator is used
   (with its own seed).


IX The CDRCLS-object

 - Class-name perhaps CDRCLS (constant-density random cls).
   Or CoDeRCLS.
 - For one parameter-block we have a class RParam.
 - DONE Besides Prob64 we introduce VarInterval, which is just a pair of
   64-bit uints (a,b) with 1 <= a <= b.
 - The constructor then takes a vector of RParam's.
 - Accepting the empty list: the empty clause-set (0,0,0,1/2).
 - Plus the global options; perhaps also packaged.
 - One can then get the comments as strings, and the parameter-line.
 - And the seed-sequence (as 64-bit).
 - Yet nothing constructed.
 - If one of the global options is on, then the computation creates
   a clause-set held as data-member; otherwise in memory there are only
   single clauses.
 - There is an output-function on an output-stream.
 - There is also an output-iterator available for the clauses (as sorted
   vectors of literals), creating the clauses on-the-fly.
 - Only one of the output-facilities should be chosen; how to arrange that?
   One could have a guard-variable, and throwing an exception in the
   error-case.
 - If the clause-set is available in memory, then perhaps it should be
   created at construction, and then the two output-facilities (output-stream
   and iterator) are both available. But otherwise only one of the two
   is available, and can only be used once.

*/

#ifndef CLAUSESETS_UUIoaKXj2K
#define CLAUSESETS_UUIoaKXj2K

#include <stdexcept>
#include <utility>

#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"

namespace RandGen {

  class VarInterval {
    gen_uint_t a_, b_;
  public :
    constexpr gen_uint_t a() const noexcept { return a_; }
    constexpr gen_uint_t b() const noexcept { return b_; }

    constexpr VarInterval(const gen_uint_t n) : VarInterval(1,n) {
      if (n == 0) throw std::domain_error("VarInterval(gen_uint_t): n = 0");
    }
    constexpr VarInterval(const int n) : VarInterval(1,n) {
      if (n < 0) throw std::domain_error("VarInterval(int): n < 0");
    }
    VarInterval(FloatingPoint::float80) = delete;
    VarInterval(double) = delete;
    VarInterval(float) = delete;

    constexpr VarInterval(const gen_uint_t a, gen_uint_t b) : a_(a), b_(b) {
      if (a > b) throw std::domain_error("VarInterval(gen_uint_t,gen_uint_t): a > b");
      if (a == 0) throw std::domain_error("VarInterval(gen_uint_t,gen_uint_t): a = 0");
    }
    explicit constexpr VarInterval(const pair64 p) : VarInterval(p.first, p.second) {}

    explicit constexpr operator pair64() const noexcept { return {a_,b_}; }

    friend constexpr bool operator ==(const VarInterval lhs, const VarInterval rhs) noexcept {
      return lhs.a_ == rhs.a_ and lhs.b_ == rhs.b_;
    }
    friend constexpr bool operator !=(const VarInterval lhs, const VarInterval rhs) noexcept {
      return not(lhs == rhs);
    }
  };
  static_assert(VarInterval(1,2).a() == 1 and VarInterval(1,2).b() == 2);
  static_assert(VarInterval(10) == VarInterval(1,10));
  static_assert(VarInterval(5,8) == VarInterval(pair64{5,8}));
  static_assert(pair64(VarInterval(11)) == pair64(1,11));


  struct RParam {
    const VarInterval n;
    const gen_uint_t k;
    const gen_uint_t c;
    const Prob64 p{1,2};

    void add_seeds(vec_eseed_t& v) const {
      v.reserve(v.size() + 6);
      {const pair64 n_(n);
       v.push_back(n_.first); v.push_back(n_.second);}
      v.push_back(k); v.push_back(c);
      {const pair64 p_{p};
       v.push_back(p_.first); v.push_back(p_.second); }
    }

  };


  enum class SortO { unsorted=0, sorted=1, rejectdup=2 }; // u, s, r
  enum class RenameO { original=0, renamed=1 }; // o, r

  class GParam {
    SortO s_;
    RenameO r_;
  public :
    constexpr static int size_s = 3;
    constexpr static int size_r = 2;
    constexpr static int size = size_s * size_r;
    constexpr SortO s() const noexcept { return s_; }
    constexpr RenameO r() const noexcept { return r_; }

    GParam() noexcept = default;
    constexpr GParam(const SortO s, const RenameO r) noexcept : s_(s), r_(r) {}
    explicit constexpr GParam(const int i) : s_(SortO(i % size_s)), r_(RenameO(i / size_s)) {
      if (i < 0) throw std::domain_error("GParam(int): i < 0");
      if (i >= size) throw std::domain_error("GParam(int): i >= size");
    }

    explicit constexpr operator int() const noexcept {
      return int(s_) + int(r_) * size_s;
    }

    typedef std::pair<SortO,RenameO> pair_t;
    constexpr operator pair_t() const noexcept { return {s_,r_}; }

    friend constexpr bool operator ==(const GParam lhs, const GParam rhs) noexcept {
      return lhs.s_ == rhs.s_ and lhs.r_ == rhs.r_;
    }
    friend constexpr bool operator !=(const GParam lhs, const GParam rhs) noexcept {
      return not(lhs == rhs);
    }

  };
  static_assert(GParam::size == 6);
  static_assert(int(GParam(0)) == 0);
  static_assert(int(GParam(1)) == 1);
  static_assert(int(GParam(2)) == 2);
  static_assert(int(GParam(3)) == 3);
  static_assert(int(GParam(4)) == 4);
  static_assert(int(GParam(5)) == 5);
  static_assert(GParam() == GParam(SortO::unsorted, RenameO::original));
  static_assert(GParam(0) == GParam());
}

namespace Environment {
  template <>
  struct RegistrationPolicies<RandGen::GParam> {
    static constexpr int size = RandGen::GParam::size;
    static constexpr std::array<const char*, size> string
      {"u|o", "s|o", "r|o", "u|r", "s|r", "r|r"};
  };
}

namespace RandGen {

  std::ostream& operator <<(std::ostream& out, const RandGen::GParam p) {
    return out << Environment::RegistrationPolicies<RandGen::GParam>::string[int(p)];
  }

  enum class MainType : gen_uint_t { const_density = 0 };


  struct Param {
    GParam gp;
    typedef std::vector<RParam> rparam_v;
    rparam_v vp;

    Param(const GParam gp, const rparam_v& v) : gp(gp), vp(v) {}
    Param(const GParam gp, rparam_v&& v) noexcept : gp(gp), vp(v) {}

    vec_eseed_t seeds() const {
      using size_t = rparam_v::size_type;
      const size_t size = 3 + 6 * vp.size();
      vec_eseed_t v; v.reserve(size);
      v.push_back(gen_uint_t(MainType::const_density));
      v.push_back(gen_uint_t(int(gp)));
      v.push_back(vp.size());
      for (const auto p : vp) p.add_seeds(v);
      assert(v.size() == size);
      return v;
    }

  };

}

#endif
