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
 2. In decreasing order of generality; starting with the "type" of seeding.

    Best two values: main type (e.g., constant-density clause-sets with one
    block) and sub-type (e.g., could be the global options).
    Called type-seed.
 3. Using extended seed-values (64 bits).
 4. The user-provided seeds (arbitrarily many, including none) are appended.
 5. For the standard model after the first seed-pair (always the "type")
    come (in brackets the number of related seed-values):
     1. n (2)
     2. k (1)
     3. c (1)
     4. p (2)

II Blocks of parameters

One block of parameter describes the construction of one clause-set.
One can have several such blocks (at least one), where the clause-sets
are appended, and the Dimacs-parameter-value reflect the whole construct.

The type-seed contains the number of blocks.
Then the blocks each have their parameters (n,k,c,p) as seeds.

Perhaps the type-seed has three values:
 - the main type
 - the subtypes
 - the number of blocks.


III Global options

 - One global option is to have to sort the clauses, and remove duplicated
   clauses.
 - Another global option is to rename the variables, so that gaps and
   non-occurring variables are eliminated.
 - This yields 2*2 = 4 possibilities; actually independent dimensions are
  - sorting
  - removing duplicated clauses
  - rename variables to eliminate formal variables.
 - This makes 8 possibilities. One could use std::bitset<3>.
 - Though removing duplicates without sorting is costly, while sorting without
   removing duplicates is no problem.
 - So perhaps activating "remove duplicates" also activates "sort".
   But is this effort needed? Would just "sort", which includes "remove
   duplicates", be enough?
 - One could have an option, where duplicated clauses are rejected.
   A check would be needed, whether the parameters are realisable.


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


IX The CDRCLS-object

 - Class-name perhaps CDRCLS (constant-density random cls).
   Or CoDeRCLS.
 - For one parameter-block we have a class RParam.
 - DONE Besides Prob64 we introduce VarInterval, which is just a pair of
   64-bit uints (a,b) with 1 <= a <= b.
 - The constructor then takes a vector of RParam's.
 - Plus the global options; perhaps also packaged.
 - Accepting the empty list: the empty clause-set (0,0,0,1/2).
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

#include "Distributions.hpp"

namespace RandGen {

  struct VarInterval {
    const gen_uint_t a, b;

    constexpr VarInterval(const gen_uint_t n) : VarInterval(1,n) {
      if (n == 0) throw std::domain_error("VarInterval(gen_uint_t): n = 0");
    }
    constexpr VarInterval(const int n) : VarInterval(1,n) {
      if (n < 0) throw std::domain_error("VarInterval(int): n < 0");
    }
    VarInterval(FloatingPoint::float80) = delete;
    VarInterval(double) = delete;
    VarInterval(float) = delete;

    constexpr VarInterval(const gen_uint_t a, gen_uint_t b) : a(a), b(b) {
      if (a > b) throw std::domain_error("VarInterval(gen_uint_t,gen_uint_t): a > b");
      if (a == 0) throw std::domain_error("VarInterval(gen_uint_t,gen_uint_t): a = 0");
    }
    explicit constexpr VarInterval(const pair64 p) : VarInterval(p.first, p.second) {}

    constexpr VarInterval(const VarInterval&) noexcept = default;

    explicit constexpr operator pair64() const noexcept { return {a,b}; }
  };
  static_assert(VarInterval(1,2).a == 1 and VarInterval(1,2).b == 2);

  inline constexpr bool operator ==(const VarInterval lhs, const VarInterval rhs) noexcept {
    return lhs.a == rhs.a and lhs.b == rhs.b;
  }
  inline constexpr bool operator !=(const VarInterval lhs, const VarInterval rhs) noexcept {
    return not(lhs == rhs);
  }
  static_assert(VarInterval(10) == VarInterval(1,10));
  static_assert(VarInterval(5,8) == VarInterval(pair64{5,8}));
  static_assert(pair64(VarInterval(11)) == pair64(1,11));


  struct RParam {
    const VarInterval n;
    const gen_uint_t k;
    const gen_uint_t c;
    const Prob64 p{1,2};
  };

}

#endif
