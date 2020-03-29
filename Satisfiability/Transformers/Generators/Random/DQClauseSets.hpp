// Oliver Kullmann, 17.7.2019 (Swansea)
/* Copyright 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Random dependency-quantified CNF

   Components:

   Parameter-handling:

    - typedef ae_numvars: the pair (na, ne)
    - bool valid(ae_numvars): valid iff sum and product don't overflow
    - ae_numvars extract_numvars(block_v)
    - gen_uint_t num_dependencies(block_v)

   Option-handling:

    - enum class DepOp (from_scratch, subtract, add)
    - typedef dep_par_t: pair (gen_uint_t, DepOp) for the dependency-form
      and the number associated with it
    - dep_par_t read_dep_par(std::string)

   Seed-handling:

     - vec_eseed_t seeds(Param, block_v, dep_par_t)

   Tools for the generation-process:

     - typedef orig_new_pair: pair of VarInterval's, for old and shifted
       interval
     - typedef ablock_v: vector of orig_new_pair, for translating the
       fragemented a-intervals into contigous intervals
     - ablock_v extract(block_v): computing the contiguous a-intervals
       (shifted)
     - functor-class AccessA: constructed as AccessA aa(block_v),
       aa[v] translates 1 <= v <= na into the original variable, while
       aa(v) computes the inverse.

     - typedef ae_pair: pair of gen_uint_t, representing a dependency-edge
       (via indices or variables)
     - ae_pair extract_ae(gen_uint_t x, gen_uint_t na): translating
       dependency-index x into a dependency-edge
     - typedef dep_edges: vector of ae_pair's
     - dep_edges translate(vec_eseed_t, gen_uint_t na, ne, block_v, DepOp)
       computes the dependency graph as "set" of ae_pair's (a,e), where
       a is a variable, while e is an index.

     - typedef AVarset: set of gen_uint_t
     - typedef AVarSetsystem: set of AVarset
     - typedef Dependency: pointer into AVarSetsystem
     - typedef Dvector: vector of Dependency's
     - typedef FullDependencies: pair of AVarSetsystem and Dvector
     - AVarset::size_type num_dependencies(Dvector)
     - FullDependencies create_dependencies(dep_edges, block_v, DepOp)
       (a helper function for the following overload)
     - FullDependencies create_dependencies(RandGen_t, block_v, gen_uint_t na, ne, dep_par_t)

   Generation:

     - void rand_clauselist(std::ostream, RandGen_t, rparam_v, block_v, gen_uint_t na, ne, dep_par_t)
     - void output_dqblocks(std::ostream, Dvector, AVarSetsystem, rename_info_t R, DepOp)
       (implementation not complete)

DESIGN:

 1. The handling of the dependency-sets should be unified with
    DQCNF/AutarkiesL1.cpp.
 2. Filtering of clauses (rejecting clauses with formal universal
    variables) as with QBRG.

TODOS:

0. Complete implementation of output_dqblocks

1. Merging repeated a/e-lines
    - QBRG in all modes merges neighbouring q-blocks of the same type (by
      calling output_qblocks)
    - Currently rand_clauselist in DQClauseSets does not do this; likely it
      should. DONE: no, too expensive here, due to the possibility of
      e-lines being removed. So we leave this, for the o,u-mode only, as
      a special feature of this mode.
    - DONE
      Moreover, this functions handles output of quantifier-information itself.
    - DONE Due to the need for direct output, it seems this is necessary: the
      dependencies are created on the fly, and are not stored.
    - DONE
      Possibly this should be outsorced to another function. On the other hand,
      the handling of the clauses is just one line.
    - DONE output_dqblocks for the other forms (not the -1-forms, i.e., "u,o"),
      needs the dependencies as created before creating the clauses (the
      former influencing the latter in the {}-form). And thus here likely
      it needs to be a different function.
    - DONE Perhaps it just uses the dep_vector, not the original vblock's.

    A problem here is with renaming: for a complete handling of clean
    a/e-lines (no repetitions, and always using an e-line if possible),
    the dependencies needed to be updated (for their output)
    according to renaming (and non-occurrences).
    Shall this be done? Would be more consistent.

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
#include <set>

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
     assert(dp.n <= na+ne);
     out << dimacs_pars{bv[0].v.b(), dp.c};
    }

    assert(ne != 0);
    switch (deppar.second) {
    case DepOp::from_scratch: {
      const dep_edges rdep = translate(choose_kn(deppar.first, na*ne, g, true), na, ne, bv, DepOp::from_scratch);
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
      const dep_edges rdep = translate(choose_kn(deppar.first, num_dependencies(bv), g, true), na, ne, bv, DepOp::subtract);
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
            for (const gen_uint_t v : b.v) {
              if (dep_it != end and dep_it->second == ei) {
                deplist dl{v,{}};
                do dl.second.push_back(dep_it->first);
                while (++dep_it != end and dep_it->second == ei);
                removed_deps.push_back(std::move(dl));
              }
              else
                if (out_ex) out << " " << v;
                else {out_ex = true; out << Q::ex << " " << v;}
              ++ei;
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
            for (const gen_uint_t v : b.v) {out << " " << v; ++ei;}
            out << " 0\n";
          }
        }
      }
      assert(ei == ne);
      assert(dep_it == end);
      break;
    }
    case DepOp::add: {
      const dep_edges rdep = translate(choose_kn(deppar.first, na*ne - num_dependencies(bv), g, true), na, ne, bv, DepOp::add);
      assert(rdep.size() == deppar.first);
      gen_uint_t ei = 0;
      auto dep_it = rdep.cbegin();
      const auto end = rdep.cend();
      typedef std::tuple<gen_uint_t, block_v::size_type, std::vector<gen_uint_t>, std::pair<gen_uint_t,dep_edges::size_type>> deplist;
      typedef std::priority_queue<deplist, std::vector<deplist>,
        std::function<bool(const deplist&, const deplist&)>> pqueue_t;
      pqueue_t added_deps([](const deplist& a, const deplist& b) noexcept {
        return std::get<3>(a) > std::get<3>(b); });
      dep_edges::size_type counter = 0;
      for (block_v::size_type index = 1; index < bv.size(); ++index) {
        const auto& b = bv[index];
        if (b.q == Q::fa) {
          out << Q::fa;
          for (const auto v : b.v) out << " " << v;
          out << " 0\n";
          while (not added_deps.empty() and std::get<3>(added_deps.top()).first <= b.v.b()) {
            const auto& t = added_deps.top();
            const gen_uint_t v = std::get<0>(t);
            const auto i = std::get<1>(t);
            const auto& vec = std::get<2>(t);
            out << "d " << v;
            for (block_v::size_type j = 1; j < i; ++j) if (bv[j].q == Q::fa)
              for (const gen_uint_t w : bv[j].v) out << " " << w;
            for (const gen_uint_t w : vec) out << " " << w;
            out << " 0\n";
            added_deps.pop();
          }
        }
        else {
          assert(b.q == Q::ex);
          if (dep_it != end and dep_it->second < ei+b.v.size()) {
            bool out_ex = false;
            for (const gen_uint_t v : b.v) {
              if (dep_it != end and dep_it->second == ei) {
                deplist dl{v,index,{},{}};
                do std::get<2>(dl).push_back(dep_it->first);
                while (++dep_it != end and dep_it->second == ei);
                std::get<3>(dl) = {std::get<2>(dl).back(), counter};
                assert(not std::get<2>(dl).empty());
                counter += std::get<2>(dl).size();
                added_deps.push(std::move(dl));
              }
              else
                if (out_ex) out << " " << v;
                else {out_ex = true; out << Q::ex << " " << v;}
              ++ei;
            }
            if (out_ex) out << " 0\n";
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
      assert(counter == rdep.size());
      break;
    }
    default:
     out << "UNKNOWN DEPENDENCY-FORM.\n";
     return;
    }

    rand_clauselist_core(out, g, par);
  }


  typedef std::set<gen_uint_t> AVarset;
  typedef std::set<AVarset> AVarSetsystem;
  typedef AVarSetsystem::const_pointer Dependency;
  // nullptr means universal variable:
  typedef std::vector<Dependency> Dvector;
  typedef std::pair<AVarSetsystem, Dvector> FullDependencies;

  AVarset::size_type num_dependencies(const Dvector& D) noexcept {
    AVarset::size_type sum = 0;
    for (const auto d : D) if (d) sum += d->size();
    return sum;
  }

  FullDependencies create_dependencies(const dep_edges& rdep, const block_v& bv, const DepOp dpo) {
    assert(valid(bv));
    const gen_uint_t n = bv[0].v.b();
    FullDependencies R{{},n+1};
    if (dpo == DepOp::from_scratch) {
      gen_uint_t ei = 0;
      auto dep_it = rdep.cbegin();
      const auto end = rdep.cend();
      for (block_v::size_type index = 1; index < bv.size(); ++index) {
        const auto& b = bv[index];
        if (b.q == Q::ex) {
          for (const gen_uint_t v : b.v) {
            AVarset V;
            auto hint = V.cbegin();
#ifndef NDEBUG
            const auto old_it = dep_it;
#endif
            while (dep_it != end and dep_it->second == ei) {
              hint = V.insert(hint, dep_it->first);
              ++dep_it;
            }
#ifndef NDEBUG
            assert(Dvector::size_type(dep_it - old_it) == V.size());
#endif
            R.second[v] = &*R.first.insert(std::move(V)).first;
            ++ei;
          }
        }
      }
      assert(dep_it == end);
    }
    else {
      assert(dpo == DepOp::subtract or dpo == DepOp::add);
      gen_uint_t ei = 0;
      auto dep_it = rdep.cbegin();
      const auto end = rdep.cend();
      AVarset VA;
      for (block_v::size_type index = 1; index < bv.size(); ++index) {
        const auto& b = bv[index];
        if (b.q == Q::fa) {
          auto hint = VA.end();
          for (const gen_uint_t v : b.v) hint = VA.insert(hint,v);
        }
        else {
          assert(b.q == Q::ex);
          for (const gen_uint_t v : b.v) {
            AVarset V(VA);
#ifndef NDEBUG
            const auto old_it = dep_it;
#endif
            while (dep_it != end and dep_it->second == ei) {
              if (dpo == DepOp::subtract) V.erase(dep_it->first);
              else V.insert(dep_it->first);
              ++dep_it;
            }
#ifndef NDEBUG
            assert(Dvector::size_type(dep_it-old_it) == (dpo==DepOp::add ?
              V.size()-VA.size() : VA.size() - V.size()));
#endif
            R.second[v] = &*R.first.insert(std::move(V)).first;
            ++ei;
          }
        }
      }
      assert(dep_it == end);
    }
    return R;
  }

  FullDependencies create_dependencies(RandGen_t& g, const block_v& bv, const gen_uint_t na, const gen_uint_t ne, const dep_par_t deppar) {
    assert(valid(bv));
    assert(deppar.second==DepOp::from_scratch or deppar.second==DepOp::subtract or deppar.second==DepOp::add);
    return create_dependencies(translate(choose_kn(deppar.first,
      deppar.second==DepOp::from_scratch ? na*ne
        : (deppar.second==DepOp::subtract ? num_dependencies(bv) : na*ne-num_dependencies(bv)),
      g, true), na, ne, bv, deppar.second), bv, deppar.second);
  }

  FullDependencies rename_dependencies(const Dvector& dv, const AVarSetsystem& ds, const rename_info_t& R) {
    assert(not ds.empty());
    [[maybe_unused]] const auto size = dv.size();
    assert(ds.size() <= size);
    assert(size >= 2);
    const auto size2 = R.second.size();
    assert(size2 <= size);
    const auto max = R.first;
    assert(max >= 1);
    assert(size2 >= max);

    FullDependencies res{{}, max+1};
    std::map<Dependency, Dependency> old2new;
    for (const AVarset& V : ds) {
      const Dependency olddep = &V;
      AVarset Vr;
      auto hint = Vr.end();
      for (const gen_uint_t v : V) {
        assert(v < size and dv[v] == nullptr);
        if (v < size2 and R.second[v] != 0) hint = Vr.insert(hint, v);
      }
      const Dependency newdep = &*res.first.insert(std::move(Vr)).first;
      old2new[olddep] = newdep;
    }
    for (gen_uint_t v = 1; v < size2; ++v) {
      const gen_uint_t vr = R.second[v];
      if (vr == 0) continue;
      assert(vr <= max);
      if (dv[v] != nullptr) res.second[vr] = old2new[dv[v]];
    }
    return res;
  }

  // Similar to output_qblocks(out, bv, R) in QClauseSets.hpp:
  void output_dqblocks(std::ostream& out, const Dvector& dv, const AVarSetsystem& ds, const rename_info_t& R, const DepOp dpo) {
    const bool use_max = R.first != 0;
    const bool use_renaming = not R.second.empty();
    assert(not use_renaming or use_max);
    typedef Dvector::size_type size_t;
    const size_t size = dv.size();
    assert(size >= 2);
    const size_t size2 = R.second.size();
    assert(size2 <= size);
    switch (dpo) {

    case DepOp::from_scratch: {
      if (not use_max) {
        out << Q::fa;
        for (size_t i = 1; i < size; ++i)
          if (dv[i] == nullptr) out << " " << i;
        out << " 0\n";
        for (size_t i = 1; i < size; ++i)
          if (dv[i] != nullptr) {
            out << "d " << i;
            for (const auto v : *dv[i]) out << " " << v;
            out << " 0\n";
          }
      }
      else if (use_renaming) {
        out << Q::fa;
        for (size_t i = 1; i < size2; ++i)
          if (dv[i] == nullptr and R.second[i] != 0)
            out << " " << R.second[i];
        out << " 0\n";
        for (size_t i = 1; i < size2; ++i)
          if (dv[i] != nullptr and R.second[i] != 0) {
            out << "d " << R.second[i];
            for (const auto j : *dv[i]) {
              assert(dv[j] == nullptr);
              if (j >= size2) break;
              if (R.second[j] != 0) out << " " << R.second[j];
            }
            out << " 0\n";
          }
      }
      else {
        out << Q::fa;
        for (size_t i = 1; i <= R.first; ++i)
          if (dv[i] == nullptr) out << " " << i;
        out << " 0\n";
        for (size_t i = 1; i <= R.first; ++i)
          if (dv[i] != nullptr) {
            out << "d " << i;
            for (const auto v : *dv[i])
              if (v > R.first) break;
              else out << " " << v;
            out << " 0\n";
          }
      }
      break;}

    case DepOp::subtract: {
      if (not use_max) {
        AVarset VA;
        auto hint = VA.end();
        for (size_t i = 1; i < size;) {
          if (dv[i] == nullptr) {
            SUB_NOTMAX_FA:
            out << Q::fa << " " << i; hint = VA.insert(hint, i++);
            while (i < size and dv[i] == nullptr) {
              out << " " << i;
              hint = VA.insert(hint, i++);
            }
            out << " 0\n";
            if (i < size) goto SUB_NOTMAX_EX;
            else goto SUB_EXIT;
          }
          else {
            SUB_NOTMAX_EX: bool found_ex = false;
            const auto find = ds.find(VA);
            const Dependency va = find == ds.end() ? nullptr : &*find;
            std::vector<gen_uint_t> d_cases;
            do {
              if (dv[i] == va) {
                if (not found_ex) {
                  out << Q::ex << " " << i;
                  found_ex = true;
                }
                else out << " " << i;
              }
              else d_cases.push_back(i);
            } while (++i < size and dv[i] != nullptr);
            if (found_ex) out << " 0\n";
            if (not d_cases.empty()) {
              for (const gen_uint_t v : d_cases) {
                out << "d " << v;
                for (const gen_uint_t w : *dv[v]) out << " " << w;
                out << " 0\n";
              }
            }
            if (i < size) goto SUB_NOTMAX_FA;
            else goto SUB_EXIT;
          }
        }
      }
      else if (use_renaming) {
        AVarset VA;
        for (size_t i = 1; i < size2; ++i) {
          if (dv[i] == nullptr) {
            SUB_RENAME_FA:
            bool found_fa = false;
            do
              if (const auto ri = R.second[i]; ri != 0) {
                assert(dv[i] == nullptr);
                VA.insert(i);
                if (not found_fa) {
                  out << Q::fa << " " << ri;
                  found_fa = true;
                }
                else out << " " << ri;
              }
              else if (dv[i] == nullptr) VA.insert(i);
            while (++i < size2 and (dv[i] == nullptr or R.second[i] == 0));
            if (found_fa) out << " 0\n";
            if (i < size2) goto SUB_RENAME_EX;
            else goto SUB_EXIT;
          }
          else {
            SUB_RENAME_EX: assert(i < size2 and dv[i] != nullptr);
            bool found_ex = false;
            const auto find = ds.find(VA);
            const Dependency va = find == ds.end() ? nullptr : &*find;
            std::vector<gen_uint_t> d_cases;
            /* Note: the above do-while-loop for fa does grab all
               immediately following non-occurring ex-variables, while this
               do-while-loop is not doing that for the fa-variables, since
               they need to be included into VA.
            */
            do if (const auto ri = R.second[i]; ri != 0) {
              if (dv[i] == va) {
                if (not found_ex) {
                  out << Q::ex << " " << ri;
                  found_ex = true;
                }
                else out << " " << ri;
              }
              else d_cases.push_back(i);
            } while (++i < size2 and dv[i] != nullptr);
            if (found_ex) out << " 0\n";
            for (const gen_uint_t v : d_cases) {
              assert(v < size2 and dv[v] != nullptr and R.second[v] != 0);
              out << "d " << R.second[v];
              for (const gen_uint_t w : *dv[v])
                if (w < size2 and R.second[w] != 0)
                  out << " " << R.second[w];
              out << " 0\n";
            }
            if (i < size2) goto SUB_RENAME_FA;
            else goto SUB_EXIT;
          }
        }
      }
      else {
        [[ unimplemented ]];
      }
      SUB_EXIT: break;}

    case DepOp::add : {
      [[ unimplemented ]];
      break;}

    }
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
