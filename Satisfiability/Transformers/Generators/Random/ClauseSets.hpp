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

 1. So all parameters go into the seed-sequence. DONE
 2. Using always extended seed-values (64 bits). DONE
 3. DONE
    In decreasing order of generality; starting with the "type" of seeding.

    Using four values:
     - main type (e.g., constant-density clause-sets),
     - sub-type (e.g., could be the global options)
     - number of blocks
     - thread-index (default: 0).

    Called "type-seed".

 4. The user-provided seeds (arbitrarily many, including none) are appended.
     - Via "t" one gets a timestamp (as many as asked, without any further
       editing).
     - Via "r" one gets a random value via std::random_device().
 5. DONE
    For the standard model after the first type-seed come (in brackets
    the number of related seed-values):
     1. n (2)
     2. k (1)
     3. c (1)
     4. p (2)
    This is one block(n,k,c,p).
 6. DONE
    Should this also accommodate the extension to DQCNF? Then one also needs
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



III Global options DONE

 - We have two dimensions:
  - "sorted" (with removal of duplications), stronger "sorted with rejection"
    (making sure that the numbers c of clauses are reached)
  - rename (eliminating gaps and non-occurring variables).
 - This makes 3*2=6 possibilities, realised as a pair of
   enum-classes, in class GParam.
  - In order for that to work with the system in ProgramOptions::Environment,
    one needed to wrap that pair in a class which makes it look like an
    enum-class. This means that in can be constructed from an int, and
    converted to an int.


IV Variables

For a clause-block: Either parameter n: 1, ..., n, or n1,n2: n1, ..., n2.
Represented as a pair n1, n2 (with n1=1 in the first case).

Allowed is the full range for 64-bit; literals are pairs (var,sign).
A comment specifies the smallest bit-number from 8,16,32,64,128 which
safely contains n+1 (- - (n+1) == n+1).


V Sign probabilities

A Prob64 value p, by default 1/2, for the probability of a positive literal.
The input of probabilities is always as a quotient, also for 0 = 0/1 and
1 = 1/1.

Alternatively one can use an integer s, which if >= 0 means the number
of positive literals in a clause, and if negative, then -s means the
number of negative literals in a clause:
 - Likely best to immediately translated this into the number of positive
   literals, so that the internal representation is unsigned.
 - DONE The current parameter handling in RParam needs to be generalised.
 - DONE One could use std::variant.
 - DONE For RParam::add_seeds one can represent s as the pair (0,s), which
   only for s=1 can be produced by a probability (namely p=1), in which
   case one could use (2,1).
 - One motivation here is to have the possibility to specify mixed binary
   clauses (with s=1).


VI The number of clauses

One 64-bit value c.


VII The clause-length

One 64-bit value k.


VIII The random-number generators

Using one RandGen_t g1 for the construction of the clauses.
And one RandGen_t g2 for the signs (which uses bernoulli_low in the case
p = 1/2).
 - Is having two generators really useful? It seems to only complicate things.
 - One argument is the use of bernoulli_low: it generates a continuous stream
   of bits, and this is more efficient when not interrupted inbetween clauses.
 - Is it really worth, to use bernoulli_low?
 - For parallelisation via threads, the result will depend (in a well-defined
   way) on the number of threads, and for each thread one generator is used
   (with its own seed).


IX The CDRCLS-object

 - Class-name perhaps CDRCLS (constant-density random cls).
   Or CoDeRCLS. Or CRCNF.
 - DONE For one parameter-block we have a class RParam.
 - DONE
   While class Param contains the global parameters and a vector of RParam's.
 - DONE Besides Prob64 we introduce VarInterval, which is just a pair of
   64-bit uints (a,b) with 1 <= a <= b.
 - The constructor then takes an Param-object.
 - Accepting the empty list: the empty clause-list (0,0,0,1/2).
 - Additional parameter is the comments as strings.
 - And the seed-sequence (as 64-bit).
 - While the parameter-line of the output is computed by the class.
 - Yet nothing constructed.
 - If one of the global options is on, then the computation creates
   a clause-list held as data-member; otherwise in memory there are only
   single clauses.
 - There is an output-function on an output-stream.
 - There is also an output-iterator available for the clauses (as sorted
   vectors of literals), creating the clauses on-the-fly.
  - Perhaps easier to just have a member-function next_clause() ?
  - If there is no further clause, then the empty clause (empty vector)
    is returned, while every created clause is non-empty.
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
#include <variant>

#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Algorithms.hpp"

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
      if (a > b)
        throw std::domain_error("VarInterval(gen_uint_t,gen_uint_t): a > b");
      if (a == 0)
        throw std::domain_error("VarInterval(gen_uint_t,gen_uint_t): a = 0");
    }
    explicit constexpr VarInterval(const pair64 p) : VarInterval(p.first, p.second) {}

    explicit constexpr operator pair64() const noexcept { return {a_,b_}; }

    constexpr gen_uint_t size() const noexcept { return (b_ - a_) + 1; }

    constexpr bool element(const gen_uint_t x) const noexcept {
      return x >= a_ and x <= b_;
    }
    constexpr gen_uint_t operator[](const gen_uint_t i) const noexcept {
      assert(i < size());
      return a_ + i;
    }
    // Assuming that g(n) creates a random number in {0, ..., n-1}:
    template <class RG>
    gen_uint_t random_element(RG&& g) const noexcept { return a_ + g(size()); }

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
  static_assert(VarInterval(5,5).size() == 1);
  static_assert(VarInterval(3,10).size() == 8);
  static_assert(VarInterval(5,5).element(5));
  static_assert(not VarInterval(5,6).element(4));
  static_assert(VarInterval(77,78)[0] == 77);
  static_assert(VarInterval(100,110)[10] == 110);


  const gen_uint_t size_cblock_eseed = 2 + 1 + 1 + 2;

  // The probability of a positive sign, or the number of positive literals
  // in a clause:
  typedef std::variant<Prob64, gen_uint_t> SignDist;

  // The parameters of a clause-block:
  struct RParam {
    const VarInterval n;
    const gen_uint_t k;
    const gen_uint_t c;
    const SignDist p{Prob64{1,2}};

    void add_seeds(vec_eseed_t& v) const {
      v.reserve(v.size() + size_cblock_eseed);
      {const pair64 n_(n);
       v.push_back(n_.first); v.push_back(n_.second);}
      v.push_back(k); v.push_back(c);
      if (p.index() == 0) {
        const pair64 p_{std::get<0>(p)};
        v.push_back(p_.first); v.push_back(p_.second);
      }
      else {
        const gen_uint_t s{std::get<1>(p)};
        v.push_back(s==1 ? 2 : 0); v.push_back(s);
      }
    }

  };
  constexpr bool operator ==(const RParam& lhs, const RParam& rhs) noexcept {
    return lhs.n == rhs.n and lhs.k == rhs.k and lhs.c == rhs.c and lhs.p == rhs.p;
  }
  constexpr bool operator !=(const RParam& lhs, const RParam& rhs) noexcept {
    return not(lhs == rhs);
  }
  static_assert((RParam{10,3,20,Prob64{0,1}} != RParam{10,3,20,0}));
  constexpr bool valid(const RParam& rp) noexcept {
    return (rp.k <= rp.n.size()) and
      (rp.p.index() == 0 or std::get<1>(rp.p) <= rp.k);
  }
  static_assert(not valid({{3,5},4,5}));
  static_assert(valid({{3,6},4,5}));
  static_assert(valid({{3,6},4,5,Prob64{0,1}}));
  static_assert(not valid({{3,6},4,5,5}));
  static_assert(valid({{3,6},4,5,4}));


  // The global parameters:
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
  static_assert(GParam::size == 2*3);
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

  enum class MainType : gen_uint_t {
    block_uniform_cnf = 0,
    block_uniform_dqcnf_dimacs = 1,
    block_uniform_dqcnf_qdimacs = 2,
    block_uniform_dqcnf_dqdimacs = 3
  };

  const unsigned int default_thread_index = 0;

  const gen_uint_t size_type_eseed = 4;
  struct Param {
    GParam gp;
    typedef std::vector<RParam> rparam_v;
    rparam_v vp;

    Param(const GParam gp, const rparam_v& v) : gp(gp), vp(v) {}
    Param(const GParam gp, rparam_v&& v) noexcept : gp(gp), vp(v) {}

    vec_eseed_t seeds() const {
      using size_t = rparam_v::size_type;
      const size_t size = size_type_eseed + size_cblock_eseed * vp.size();
      vec_eseed_t v; v.reserve(size);

      v.push_back(gen_uint_t(MainType::block_uniform_cnf));
      v.push_back(gen_uint_t(int(gp)));
      v.push_back(vp.size());
      v.push_back(default_thread_index);
      assert(v.size() == size_type_eseed);

      for (const auto p : vp) p.add_seeds(v);
      assert(v.size() == size);
      return v;
    }

  };


  struct Var {
    const gen_uint_t v;
  };
  struct Lit {
    const Var v;
    const signed char sign;
  };
  inline constexpr bool valid(const Var v) noexcept { return v.v >= 1; }
  inline constexpr bool valid(const Lit x) noexcept {
    return valid(x.v) and (x.sign == -1 or x.sign == +1);
  }
  static_assert(not valid(Var{0}));
  static_assert(valid(Var{1}));
  static_assert(not valid(Lit{1,0}));
  static_assert(valid(Lit{1,-1}));
  static_assert(valid(Lit{1,1}));

  inline constexpr bool operator ==(const Var v, const Var w) noexcept {
    return v.v == w.v;
  }
  inline constexpr bool operator !=(const Var v, const Var w) noexcept {
    return not(v == w);
  }
  inline constexpr bool operator ==(const Lit x, const Lit y) noexcept {
    return x.v == y.v and x.sign == y.sign;
  }
  inline constexpr bool operator !=(const Lit x, const Lit y) noexcept {
    return not (x == y);
  }
  inline constexpr bool operator <(const Lit x, const Lit y) noexcept {
    return (x.v.v < y.v.v) or (x.v.v == y.v.v and x.sign < y.sign);
  }
  static_assert(Lit{0,-2} < Lit{0,-1});
  static_assert(Lit{1,1} < Lit{2,-1});

  std::ostream& operator <<(std::ostream& out, const Lit x) {
    if (x.sign == -1) out << "-";
    return out << x.v.v;
  }

  typedef std::vector<Lit> Clause;
  typedef std::vector<Clause> ClauseList;

  // Create a sorted random clause with k literals over the variables from n,
  // with sign-distribution given by p:
  inline Clause rand_clause(RandGen_t& g, const VarInterval n, const gen_uint_t k, const SignDist p) {
    assert(k >= 1);
    assert(k <= n.size());
    const auto varvec = choose_kn(k, n.size(), g, true);
    assert(varvec.size() == k);
    Clause C; C.reserve(k);
    if (p.index() == 0) {
      const Prob64 p0 = std::get<0>(p);
      if (p0 == Prob64{1,2})
        for (const auto i : varvec)
          C.push_back({n[i], bool2schar(bernoulli_high(g))});
      else {
        Bernoulli b(g, p0);
        for (const auto i : varvec)
          C.push_back({n[i], bool2schar(b())});
      }
    }
    else {
      const gen_uint_t s = std::get<1>(p);
      if (s == k)
        for (const auto i : varvec) C.push_back({n[i], 1});
      else if (s == 0)
        for (const auto i : varvec) C.push_back({n[i], -1});
      else {
        std::vector<signed char> signs(k, -1);
        {const auto posvec = choose_kn(s, k, g, true);
         assert(posvec.size() == s);
         for (const auto i : posvec) signs[i] = 1;
        }
        for (gen_uint_t i = 0; i < k; ++i)
          C.push_back({n[varvec[i]], signs[i]});
      }
    }
    assert(C.size() == k);
    return C;
  }

}

#endif
