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

TODOS:

1. Implement "-D" for rand_clauselist
   (a) How should the dependencies be output? Extending the quantifier-
       structure seems natural. First the usual QBF-output, ignoring
       the variables with subtracted dependencies, and then the d-lines
       for the altered variables.
   (b) Better to have the d-line of a variable directly placed
       after the corresponding e-block: it does not depend on a-variables
       after that.
   (c) The translate-function should be generalised, to take the
       dependency-form into account. For the "-D"-form, i.e., DepOp::subtract,
       the dependency-pairs are then the pairs to be *subtracted*.
   (d) And for +D the pairs are to be added. Here perhaps the output of the
       d-lines of the affected variables comes after all a/e-lines.

*/

#ifndef DQCLAUSESETS_nMH1OTZkKa
#define DQCLAUSESETS_nMH1OTZkKa

#include <utility>
#include <string>
#include <algorithm>
#include <vector>
#include <tuple>
#include <queue>
#include <functional>

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


  // The original interval and the shifted interval:
  typedef std::pair<VarInterval, VarInterval> orig_new_pair;
  typedef std::vector<orig_new_pair> ablock_v;

  // Translating the non-contiguous a-intervals into contiguous intervals
  // (for index-transformation):
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

  // Initialised with the blocks of variables, via operator [v] translate the
  // index 1 <= v <= na into the original variable, while (v) does the reverse:
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

  // Translating x into a dependency, an edge beetween a universal and
  // an existential variable, represented as indices:
  typedef std::pair<gen_uint_t, gen_uint_t> ae_pair;
  inline constexpr ae_pair extract_ae(const gen_uint_t x, const gen_uint_t na, [[maybe_unused]] const gen_uint_t ne) noexcept {
    assert(x < na*ne);
    return {x % na, x / na};
  }
  static_assert((extract_ae(0,1,1) == ae_pair{0,0}));
  static_assert((extract_ae(0,3,4) == ae_pair{0,0}));
  static_assert((extract_ae(1,3,4) == ae_pair{1,0}));
  static_assert((extract_ae(2,3,4) == ae_pair{2,0}));
  static_assert((extract_ae(3,3,4) == ae_pair{0,1}));
  static_assert((extract_ae(11,3,4) == ae_pair{2,3}));
  static_assert((extract_ae(8,2,5) == ae_pair{0,4}));

  typedef std::vector<ae_pair> dep_edges;
  // Translating a vector rdep of dependency-indices into ae_pairs (a,e),
  // where a is the original variable, while e is the index (0 <= e < ne):
  dep_edges translate(const vec_eseed_t& rdep, const gen_uint_t na, const gen_uint_t ne, const block_v& bv, const DepOp op) {
    assert((extract_numvars(bv) == ae_numvars{na,ne}));
    dep_edges res; res.reserve(rdep.size());
    AccessA aa(bv);
    switch (op) {
    case DepOp::from_scratch: {
      for (const gen_uint_t x : rdep) {
        const ae_pair ae = extract_ae(x, na, ne);
        res.emplace_back(aa[1+ae.first], ae.second);
      }
      break;
    }
    case DepOp::subtract: {
      gen_uint_t current_na = 0, current_ne = 0,
                 old_sum_ne = 0, sum_ne = 0,
                 old_sum_deps = 0, sum_deps = 0;
      auto it_bv = bv.cbegin(); ++it_bv;
      for (const gen_uint_t x : rdep) {
        while (x >= sum_deps) {
          assert(it_bv != bv.cend());
          const auto b = *it_bv++;
          assert(b.q == Q::ex or b.q == Q::fa);
          if (b.q == Q::fa) current_na += b.v.size();
          else {
            current_ne = b.v.size();
            old_sum_ne = sum_ne;
            sum_ne += current_ne;
            old_sum_deps = sum_deps;
            sum_deps += current_ne * current_na;
          }
        }
        assert(x >= old_sum_deps);
        const ae_pair ae = extract_ae(x-old_sum_deps, current_na, current_ne);
        assert(ae.first < na);
        assert(ae.second + old_sum_ne < ne);
        res.emplace_back(aa[1+ae.first], ae.second + old_sum_ne);
      }
      break;
    }
    case DepOp::add: {
      gen_uint_t left_na = 0, right_na = na,
                 old_sum_ne = 0,
                 old_sum_deps = 0;
      auto it_bv = bv.cbegin(); ++it_bv;
      while (it_bv->q != Q::ex) {
        const gen_uint_t s = it_bv++->v.size();
        left_na += s; right_na -= s;
      }
      assert(it_bv->q == Q::ex);
      gen_uint_t current_ne = it_bv++->v.size(),
                 sum_ne = current_ne,
                 sum_deps = current_ne * right_na;
      for (const gen_uint_t x : rdep) {
        while (x >= sum_deps) {
          assert(it_bv != bv.cend());
          const auto b = *it_bv++;
          assert(b.q == Q::ex or b.q == Q::fa);
          if (b.q == Q::fa) {const auto s=b.v.size(); left_na+=s; right_na-=s;}
          else {
            current_ne = b.v.size();
            old_sum_ne = sum_ne;
            sum_ne += current_ne;
            old_sum_deps = sum_deps;
            sum_deps += current_ne * right_na;
          }
        }
        assert(x >= old_sum_deps);
        const ae_pair ae = extract_ae(x-old_sum_deps, right_na, current_ne);
        assert(ae.first + left_na < na);
        assert(ae.second + old_sum_ne < ne);
        res.emplace_back(aa[1+ae.first+left_na], ae.second + old_sum_ne);
      }
      break;
    }
    default:;
    }
    assert(res.size() == rdep.size());
    return res;
  }


  void rand_clauselist(std::ostream& out, RandGen_t& g, const rparam_v& par, const block_v& bv, const gen_uint_t na, const gen_uint_t ne, const dep_par_t deppar) {
    assert(valid(bv));
    assert(bv[0].v.b() == na+ne);
    {const auto dp = extract_parameters(par);
     assert(dp.first <= na+ne);
     out << dimacs_pars{bv[0].v.b(), dp.second};
    }

    assert(ne != 0);
    switch (deppar.second) {
    case DepOp::from_scratch: {
      const dep_edges rdep = translate(choose_kn(deppar.first, na*ne, g, true), na, ne, bv, deppar.second);
      assert(rdep.size() == deppar.first);
      out << Q::fa;
      for (const auto b : bv) if (b.q == Q::fa)
        for (const gen_uint_t v : b.v) out << " " << v;
      out << " 0\n";
      gen_uint_t ei = 0; // current index of existential variable
      auto dep_it = rdep.cbegin();
      const auto end = rdep.cend();
      for (block_v::size_type index = 1; index < bv.size(); ++index) {
        const auto& b = bv[index];
        if (b.q == Q::ex) {
          for (const gen_uint_t v : b.v) {
            out << "d " << v;
            while (dep_it != end and dep_it->second == ei) {
              out << " " << dep_it->first;
              ++dep_it;
            }
            out << " 0\n";
            ++ei;
          }
        }
      }
      assert(ei == ne);
      assert(dep_it == end);
      break;
    }
    case DepOp::subtract: {
      const dep_edges rdep = translate(choose_kn(deppar.first, num_dependencies(bv), g, true), na, ne, bv, deppar.second);
      assert(rdep.size() == deppar.first);
      gen_uint_t ei = 0;
      auto dep_it = rdep.cbegin();
      const auto end = rdep.cend();
      for (block_v::size_type index = 1; index < bv.size(); ++index) {
        const auto& b = bv[index];
        if (b.q == Q::fa) {
          out << Q::fa;
          for (const gen_uint_t v : b.v) out << " " << v;
          out << " 0\n";
        }
        else {
          assert(b.q == Q::ex);
          if (dep_it != end and dep_it->second < ei+b.v.size()) {
            bool out_ex = false;
            typedef std::pair<gen_uint_t,std::vector<gen_uint_t>> deplist;
            std::vector<deplist> removed_deps;
            for (gen_uint_t v = b.v.a(); v <= b.v.b(); ++v, ++ei) {
              if (dep_it != end and dep_it->second == ei) {
                deplist dl{v,{}};
                do dl.second.push_back(dep_it->first);
                while (++dep_it != end and dep_it->second == ei);
                removed_deps.push_back(std::move(dl));
              }
              else
                if (out_ex) out << " " << v;
                else {out_ex = true; out << Q::ex << " " << v;}
            }
            if (out_ex) out << " 0\n";
            assert(not removed_deps.empty());
            for (const auto& d : removed_deps) {
              out << "d " << d.first;
              const auto& del = d.second;
              auto rem_it = del.cbegin();
              const auto end = del.cend();
              assert(rem_it != end);
              for (block_v::size_type j = 1; j < index; ++j) {
                const auto& bj = bv[j];
                if (bj.q == Q::ex) continue;
                for (const gen_uint_t v : bj.v)
                  if (rem_it != end and v == *rem_it) ++rem_it;
                  else out << " " << v;
              }
              assert(rem_it == end);
              out << " 0\n";
            }
          }
          else {
            out << Q::ex;
            for (gen_uint_t v = b.v.a(); v <= b.v.b(); ++v, ++ei) out<<" "<<v;
            out << " 0\n";
          }
        }
      }
      assert(ei == ne);
      assert(dep_it == end);
      break;
    }
    case DepOp::add: {
      const dep_edges rdep = translate(choose_kn(deppar.first, na*ne - num_dependencies(bv), g, true), na, ne, bv, deppar.second);
      assert(rdep.size() == deppar.first);
      gen_uint_t ei = 0;
      auto dep_it = rdep.cbegin();
      const auto end = rdep.cend();
      typedef std::tuple<gen_uint_t, block_v::size_type, std::vector<gen_uint_t>> deplist;
      typedef std::priority_queue<deplist, std::vector<deplist>,
        std::function<bool(const deplist&, const deplist&)>> pqueue_t;
      pqueue_t pqueue([](const deplist& a, const deplist& b) noexcept {
        return std::get<2>(a).back() > std::get<2>(b).back(); });
      for (block_v::size_type index = 1; index < bv.size(); ++index) {
        const auto& b = bv[index];
        if (b.q == Q::fa) {
          out << Q::fa;
          for (const auto v : b.v) out << " " << v;
          out << " 0\n";
          while (not pqueue.empty() and std::get<2>(pqueue.top()).back() <= b.v.b()) {
            const auto& t = pqueue.top();
            const gen_uint_t v = std::get<0>(t);
            const auto i = std::get<1>(t);
            const auto& vec = std::get<2>(t);
            out << "d " << v;
            for (block_v::size_type j = 1; j < i; ++j) if (bv[j].q == Q::fa)
              for (const gen_uint_t w : bv[j].v) out << " " << w;
            for (const gen_uint_t w : vec) out << " " << w;
            out << " 0\n";
            pqueue.pop();
          }
        }
        else {
          assert(b.q == Q::ex);
          if (dep_it != end and dep_it->second < ei+b.v.size()) {
            bool out_ex = false;
            // XXX
          }
          else {
            out << Q::ex;
            for (gen_uint_t v = b.v.a(); v <= b.v.b(); ++v, ++ei) out<<" "<<v;
            out << " 0\n";
          }
        }
      }
      assert(ei == ne);
      assert(dep_it == end);
      break;
    }
    default:
     out << "UNKNOWN DEPENDENCY-FORM.\n";
     return;
    }

    rand_clauselist_core(out, g, par);
  }


  enum class DQError {
    nane_prod = 110,
    too_many_deps = 111,
    too_few_deps = 112,
    overflow = 113,
    too_much_added = 114,
    bad_deps = 115,
  };

}

#endif
