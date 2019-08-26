// Oliver Kullmann, 17.7.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Random dependency-quantified CNF

DESIGN:

At least initially, only one additional parameter to QBRG, in three forms:
  D, +D, -D
which alters the dependencies as given by the quantifier-blocks:

 - +D adds ("to the right") dependencies uniformly.

   The number of open dependencies is
     d := na*ne - #D(quants),
   where #D(qunats) is the number of dependencies given by quantifiers.
   We must have D <= d, and the random choice is
     choose D elements from {1, ..., d},
   which are then mapped to the dependency-pairs, and added
 - -D subtracts ("to the left") dependencies uniformly.

   Here we must have D <= #D(quants), and the random choice is
     choose D elements from {1, ..., #D(quants)},
   which are mapped to the dependency-pairs, and removed.
 - D just deletes all dependencies, and chooses D dependencies uniformly
   (treating all existential variables and all universal variables equally).

   The random choice is
     choose D elements from {1, ..., na*ne},
   and we must have D <= na*ne.

 1. The handling of the dependency-sets should be unified with
    DQCNF/AutarkiesL1.cpp.
 2. Filtering of clauses (rejecting clauses with formal universal
    variables) as with QBRG.

*/

#ifndef DQCLAUSESETS_nMH1OTZkKa
#define DQCLAUSESETS_nMH1OTZkKa

#include <utility>
#include <string>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Distributions.hpp"
// Guaranteed to be included:
#include "QClauseSets.hpp"

namespace RandGen {

  // Pair na=#universal variables, ne=#existential variables:
  typedef std::pair<gen_uint_t, gen_uint_t> ae_numvars;

  // Whether na+ne and na*ne do not overflow, and ne >= 1:
  inline constexpr bool valid(const ae_numvars& n) noexcept {
    if (n.second == 0) return false;
    {const gen_uint_t sum = n.first + n.second;
     if (sum < n.first or sum < n.second) return false;
    }
    {const gen_uint_t prod = n.first * n.second;
     if (prod / n.second != n.first) return false;
    }
    return true;
  }
  static_assert(valid(ae_numvars{0,1}));
  static_assert(not valid(ae_numvars{1,0}));
  static_assert(not valid(ae_numvars{0,0}));
  static_assert(not valid(ae_numvars{gen_uint_t(-1),1}));
  static_assert(not valid(ae_numvars{1,gen_uint_t(-1)}));
  static_assert(valid(ae_numvars{gen_uint_t(-1)-1,1}));
  static_assert(valid(ae_numvars{1,gen_uint_t(-1)-1}));
  static_assert(valid(ae_numvars{0x1'0000'0000ULL, 0x1'0000'0000ULL-1}));
  static_assert(valid(ae_numvars{0x1'0000'0000ULL-1, 0x1'0000'0000ULL}));
  static_assert(not valid(ae_numvars{0x1'0000'0000ULL, 0x1'0000'0000ULL}));
  static_assert(not valid(ae_numvars{iexp2(63),2}));
  static_assert(not valid(ae_numvars{2,iexp2(63)}));

  // Compute na, ne for bv:
  ae_numvars extract_numvars(const block_v& bv) noexcept {
    assert(valid(bv));
    ae_numvars n{};
    for (gen_uint_t i = 1; i < bv.size(); ++i) {
      const auto& b = bv[i];
      if (b.q == Q::fa) n.first += b.v.size();
      else n.second += b.v.size();
    }
    assert(bv[0].v.size() == n.first + n.second);
    return n; // might not be valid regarding product-overflow
  }

  gen_uint_t num_dependencies(const block_v& bv) noexcept {
    assert(valid(extract_numvars(bv)));
    gen_uint_t sum = 0;
    gen_uint_t current_a = 0;
    for (gen_uint_t i = 1; i < bv.size(); ++i) {
      const auto& b = bv[i];
      if (b.q == Q::ex) sum += b.v.size() * current_a;
      else current_a += b.v.size();
    }
    return sum;
  }

  enum class DepOp { from_scratch = 0, subtract = 1, add = 2 };

  typedef std::pair<gen_uint_t, DepOp> dep_par_t;
  dep_par_t read_dep_par(const std::string& p) {
    if (p.empty()) return {};
    if (p.front() == '+')
      return {FloatingPoint::toUInt(p.substr(1)), DepOp::add};
    else if (p.front() == '-')
      return {FloatingPoint::toUInt(p.substr(1)), DepOp::subtract};
    else
      return {FloatingPoint::toUInt(p), DepOp::from_scratch};
  }


  vec_eseed_t seeds(const Param& par, const block_v& vblock, const dep_par_t& dep) {
    const auto first = size_type_eseed;
    const auto second = first + (1 + 1) + (1 + 2 * vblock.size());
    vec_eseed_t v; v.reserve(second);

    v.push_back(gen_uint_t(MainType::block_uniform_dqcnf));
    v.push_back(gen_uint_t(int(par.gp)));
    v.push_back(par.vp.size());
    v.push_back(default_thread_index);
    assert(v.size() == first);

    v.push_back(gen_uint_t(int(dep.second)));
    v.push_back(dep.first);

    v.push_back(vblock.size());
    for (const auto& b : vblock) {
      v.push_back(b.v.size());
      v.push_back(gen_uint_t(b.q));
    }
    assert(v.size() == second);

    for (const auto p : par.vp) add_seeds(p,v);
    return v;
  }


  enum class DQError {
    nane_prod = 110,
    too_many_deps = 111,
    too_few_deps = 112,
    overflow = 113,
    too_much_added = 114,
  };

}

#endif
