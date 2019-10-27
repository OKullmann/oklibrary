// Oliver Kullmann, 17.7.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Random dependency-quantified CNF

DESIGN:

 1. The handling of the dependency-sets should be unified with
    DQCNF/AutarkiesL1.cpp.
 2. Filtering of clauses (rejecting clauses with formal universal
    variables) as with QBRG.

TODOS:

1. Merging repeated a/e-lines
    - QBRG in all modes merges neighbouring q-blocks of the same type (by
      calling output_qblocks)
    - Currently rand_clauselist in DQClauseSets does not do this; likely it
      should. DONE: no, too expensive here, due to the possibility of
      e-lines being removed. So we leave this, for the o,u-mode only, as
      a special feature of this mode.
    - Moreover, this functions handles output of quantifier-information itself.
    - Due to the need for direct output, it seems this is necessary: the
      dependencies are created on the fly, and are not stored.
    - Possibly this should be outsorced to another function. On the other hand,
      the handling of the clauses is just one line.
    - Output_dqblocks for the other forms (not the -1-forms, i.e., "u,o"),
      needs the dependencies as created before creating the clauses (the
      former influencing the latter in the {}-form). And thus here likely
      it needs to be a different function.
    - Perhaps it just uses the dep_vector, not the original vblock's.

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
     assert(dp.first <= na+ne);
     out << dimacs_pars{bv[0].v.b(), dp.second};
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

  auto num_dependencies(const Dvector& D) noexcept {
    AVarset::size_type sum = 0;
    for (const auto d : D) if (d) sum += d->size();
    return sum;
  }

  std::pair<AVarSetsystem, Dvector> create_dependencies(const dep_edges& rdep, const block_v& bv, const DepOp dpo) {
    assert(valid(bv));
    const gen_uint_t n = bv[0].v.b();
    std::pair<AVarSetsystem, Dvector> R{{},n+1};
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

  std::pair<AVarSetsystem, Dvector> create_dependencies(RandGen_t& g, const block_v& bv, const gen_uint_t na, const gen_uint_t ne, const dep_par_t deppar) {
    assert(valid(bv));
    assert(deppar.second==DepOp::from_scratch or deppar.second==DepOp::subtract or deppar.second==DepOp::add);
    return create_dependencies(translate(choose_kn(deppar.first,
      deppar.second==DepOp::from_scratch ? na*ne
        : (deppar.second==DepOp::subtract ? num_dependencies(bv) : na*ne-num_dependencies(bv)),
      g, true), na, ne, bv, deppar.second), bv, deppar.second);
  }


  // Similar to output_qblocks(out, bv, R) in QClauseSets.hpp:
  void output_dqblocks(std::ostream& out, const Dvector& dv, const rename_info_t& R, const DepOp dpo) {
    const bool use_max = R.first != 0;
    const bool use_renaming = not R.second.empty();
    assert(not use_renaming or use_max);
    typedef Dvector::size_type size_t;
    const size_t size = dv.size();
    assert(size >= 2);
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

      }
      else {

      }
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
