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
#include <algorithm>
#include <vector>

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
  std::ostream& operator <<(std::ostream& out, const DepOp d) {
    switch (d) {
    case DepOp::from_scratch : return out << "ignore_given";
    case DepOp::subtract : return out << "subtract_from_given";
    case DepOp::add : return out << "add_to_given";
    default : return out << "DepOp:" << int(d);
    }
  }

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


  typedef std::pair<VarInterval, VarInterval> orig_new_pair;
  typedef std::vector<orig_new_pair> ablock_v;

  ablock_v extract(const block_v& bv) {
    assert(valid(bv));
    ablock_v res;
    {gen_uint_t next = 1;
     for (const VarBlock& b : bv) if(b.q == Q::fa) {
       const gen_uint_t diff = b.v.b() - b.v.a(), old = next;
       res.emplace_back(b.v, VarInterval(old, next+=diff));
       ++next;
     }
    }
    return res;
  }

  class AccessA {
    const ablock_v abv;
    typedef ablock_v::const_iterator iterator;
    const iterator begin, end;
  public :
    const gen_uint_t max;
    const gen_uint_t na;
    explicit AccessA(const block_v& bv) noexcept : abv(extract(bv)), begin(abv.begin()), end(abv.end()), max(abv.empty() ? 0 : abv.back().first.b()), na(abv.empty() ? 0 : abv.back().second.b()) {}
    gen_uint_t operator()(const gen_uint_t v) const noexcept {
      assert(v <= max);
      if (v == 0) return na;
      const auto it = std::lower_bound(begin, end, v,
        [](const orig_new_pair& w, const gen_uint_t v) noexcept {
          return w.first.b() < v; });
      assert(it->first.element(v));
      return it->second[v - it->first.a()];
    }
    gen_uint_t operator[](const gen_uint_t v) const noexcept {
      assert(v <= na);
      if (v == 0) return max;
      const auto it = std::lower_bound(begin, end, v,
        [](const orig_new_pair& w, const gen_uint_t v) noexcept {
          return w.second.b() < v; });
      assert(it->second.element(v));
      return it->first[v - it->second.a()];
    }
  };

  void rand_clauselist(std::ostream& out, RandGen_t& g, const rparam_v& par, const block_v& bv, const gen_uint_t na, const gen_uint_t ne, const dep_par_t deppar) {
    assert(valid(bv));
    assert(bv[0].v.b() == na+ne);
    const auto dp = extract_parameters(par);
    out << dimacs_pars{bv[0].v.b(), dp.second};

    out << Q::fa;
    for (const auto b : bv)
      if (b.q == Q::fa)
        for (gen_uint_t v = b.v.a(); v <= b.v.b(); ++v)
          out << " " << v;
    out << " 0\n";

    if (deppar.second != DepOp::from_scratch) {
      out << "NOT IMPLEMENTED YET.\n";
      return;
    }
    const auto rdep = choose_kn(dp.first, na*ne, g, true);
    assert(rdep.size() == dp.first);
    AccessA aa(bv);
    for (const auto b : bv) if (b.q == Q::ex)
      for (gen_uint_t v = b.v.a(); v <= b.v.b(); ++v) {
        out << Q::ex << " " << v;
        // XXX
        out << " 0\n";
      }

    rand_clauselist_core(out, g, par);
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
