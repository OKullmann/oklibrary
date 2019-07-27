// Oliver Kullmann, 9.6.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Components for random quantified CNF


*/

#ifndef QCLAUSESETS_Z7vpC0rD5U
#define QCLAUSESETS_Z7vpC0rD5U

#include <vector>
#include <string>
#include <optional>
#include <ostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
// Guaranteed to be included:
#include "ClauseSets.hpp"

namespace RandGen {

  enum class Q : gen_uint_t { fa=0, ex=1, both=2 };
  std::ostream& operator <<(std::ostream& out, const Q q) {
    switch (q) {
    case Q::fa : return out << "a";
    case Q::ex : return out << "e";
    case Q::both : return out << "ae";}
    return out;
  }

  constexpr std::optional<Q> read_Q(const char c) noexcept {
    switch (c) {
    case 'a' : return Q::fa;
    case 'e' : return Q::ex;
    default : return {};}
  }
  static_assert(read_Q('a') == Q::fa);
  static_assert(read_Q('e') == Q::ex);
  static_assert(not read_Q('A'));
  static_assert(not read_Q('E'));
  static_assert(not read_Q(' '));

  struct VarBlock {
    VarInterval v;
    Q q;
  };
  inline constexpr bool operator ==(const VarBlock& lhs, const VarBlock& rhs) noexcept {
    return lhs.v == rhs.v and lhs.q == rhs.q;
  }
  inline constexpr bool operator !=(const VarBlock& lhs, const VarBlock& rhs) noexcept {
  return not (lhs == rhs);
  }
  std::ostream& operator <<(std::ostream& out, const VarBlock& v) {
    return out << v.q << v.v;
  }

  // The quantifier-blocks, at indices starting with 1, while the first block
  // is the summary:
  typedef std::vector<VarBlock> block_v;
  bool valid(const block_v& vb) noexcept {
    const auto size = vb.size();
    if (size < 2) return false;
    if (vb[0].v.a() != 1) return false;
    if (vb[0].v.b() != vb.back().v.b()) return false;
    if (vb.back().q != Q::ex) return false;
    VarInterval oldi = vb[1].v;
    if (oldi.a() != 1) return false;
    bool founda = vb[1].q == Q::fa;
    for (block_v::size_type i = 2; i < size; ++i) {
      const auto& b{vb[i]};
      if (b.q == Q::fa) founda = true;
      const VarInterval newi = b.v;
      if (oldi.b()+1 != newi.a()) return false;
      oldi = newi;
    }
    if (founda) return vb[0].q == Q::both;
    else return vb[0].q == Q::ex;
  }
  // Reading the quantifier-blocks, interpreting the blocks as intervals of
  // variable-indices:
  block_v read_block_v(const std::string& s) {
    block_v bv;
    const auto sp = Environment::split(Environment::transform_spaces(s), ' ');
    const auto size = sp.size();
    bv.reserve(size+1);
    bv.push_back({1,Q::ex});
    if (size == 0) {bv.push_back({1,Q::ex}); return bv;}
    gen_uint_t nextvar = 1;
    bool founda = false;
    for (decltype(+size) i = 0; i < size - 1; ++i) {
      const std::string& b = sp[i];
      if (b.size() < 2) throw std::domain_error("read_block_v: quantifier-block-specifier \"" + b + "\" ill-formed.");
      const auto q = read_Q(b.front());
      if (not q) throw std::domain_error("read_block_v: quantifier-letter \"" + b.substr(0,1) + "\" must be \"e\" or \"a\".");
      if (*q == Q::fa) founda = true;
      const gen_uint_t n = to_gen_uint_t(b.substr(1), false);
      if (n == 0) throw std::domain_error("read_block_v: the number of variables in a quantifier-block must be at least one, which is violated for the block \"" + b + "\".");
      bv.push_back({{nextvar,nextvar+(n-1)}, *q});
      nextvar += n;
    }
    const gen_uint_t n = to_gen_uint_t(sp.back(), false);
    if (n == 0) throw std::domain_error("read_block_v: the number of variables in a quantifier-block must be at least one, which is violated for the final block.");
    const gen_uint_t finalvar = nextvar+(n-1);
    bv.push_back({{nextvar,finalvar}, Q::ex});
    bv[0] = {{1,finalvar}, founda ? Q::both : Q::ex};
    assert(valid(bv));
    return bv;
  }
  // Output of the e/a-lines (corresponding to the quantifier-blocks):
  void output_qblocks(std::ostream& out, const block_v& bv, const rename_info_t& R) {
    const bool use_max = R.first != 0;
    const bool use_renaming = not R.second.empty();
    using size_type = block_v::size_type;
    const size_type size = bv.size();
    assert(size >= 2);
    size_type curri = 1;
    bool end_reached = false;
    do { // output q-block with index curri (and following blocks, if same quantifier)
      assert(curri < bv.size());
      const Q currq = bv[curri].q;
      const gen_uint_t curr_a = bv[curri].v.a();
      while (++curri < size and bv[curri].q == currq);
      const gen_uint_t curr_b = bv[curri-1].v.b();
      gen_uint_t count = 0;
      for (gen_uint_t i = curr_a; i <= curr_b; ++i)
        if (not use_renaming) {
          if (use_max and i > R.first) {
            if (count == 0) return;
            end_reached = true; break;
          }
          else {
            if (count == 0) out << currq << " ";
            ++count; out << i << " ";
          }
        }
        else {
          assert(use_max);
          if (i >= R.second.size()) {
            if (count == 0) return;
            end_reached = true; break;
          }
          const gen_uint_t r = R.second[i];
          if (r > R.first) {
            if (count == 0) return;
            end_reached = true; break;
          }
          if (r == 0) continue;
          if (count == 0) out << currq << " ";
          ++count; out << r << " ";
        }
      if (count != 0) out << "0\n";
    } while (curri < size and not end_reached);
  }

  // Interpretes the elements of vpar, translating block-indices to
  // variable-indices:
  rparam_v interprete(const rparam_v& vpar, const block_v& bpar) {
    rparam_v res; res.reserve(vpar.size());
    for (const auto par : vpar) {
      const auto& cps{par.cps};
      clausepart_v cps_new; cps_new.reserve(cps.size());
      for (const auto& cp : cps) {
        cps_new.push_back({{bpar[cp.n.a()].v.a(), bpar[cp.n.b()].v.b()}, cp.k, cp.p});
      }
      res.push_back({std::move(cps_new), par.c});
    }
    return res;
  }
  Q quantifier(const Var v, const block_v& bpar) noexcept {
    assert(not bpar.empty());
    for (rparam_v::size_type i = 1; i < bpar.size(); ++i)
      if (bpar[i].v.element(v.v)) return bpar[i].q;
    return Q::both;
  }


  vec_eseed_t seeds(const Param& par, const block_v& vblock) {
    const auto first = size_type_eseed;
    const auto second = first + 1 + 2 * vblock.size();
    vec_eseed_t v; v.reserve(second);

    v.push_back(gen_uint_t(MainType::block_uniform_qcnf));
    v.push_back(gen_uint_t(int(par.gp)));
    v.push_back(par.vp.size());
    v.push_back(default_thread_index);
    assert(v.size() == first);

    v.push_back(vblock.size());
    for (const auto& b : vblock) {
      v.push_back(b.v.size());
      v.push_back(gen_uint_t(b.q));
    }
    assert(v.size() == second);

    for (const auto p : par.vp) add_seeds(p,v);
    return v;
  }


  void rand_clauselist(std::ostream& out, RandGen_t& g, const rparam_v& par, const block_v& bv) {
    assert(bv.size() >= 2);
    const auto dp = extract_parameters(par);
    out << dimacs_pars{bv[0].v.b(), dp.second};
    output_qblocks(out, bv, {});
    rand_clauselist_core(out, g, par);
  }

  // Similar to rand_clauseset(g, par, r), but now without Rename-parameter,
  // and rejecting clauses with formal universal variables:
  RDimacsClauseList rand_qclauseset(RandGen_t& g, const rparam_v& par, const block_v& vblock) {
    if (par.empty()) return {{{0,0},{}}, {}};
    ClauseSet F;
    const auto [n,c] = extract_parameters(par);
    // Testing whether there is enough memory (temporary solution; better
    // to use a custome-allocator, which actually uses the allocated memory):
    F.get_allocator().deallocate(F.get_allocator().allocate(c), c);
    for (const RParam& pa : par)
      for (gen_uint_t i = 0; i < pa.c; ++i) {
        Clause C; C.reserve(size(pa.cps));
        for (;;) {
          C.clear();
          for (const ClausePart& cp : pa.cps)
            rand_clause(g, C, cp.n, cp.k, cp.p);
          std::sort(C.begin(), C.end());
          if (F.find(C) != F.end()) continue;
          assert(not C.empty());
          const Q q = quantifier(C.back().v, vblock);
          assert(q != Q::both);
          if (q == Q::ex) { F.insert(std::move(C)); break; }
        }
      }
    assert(F.size() == c);
    ClauseList F2;
    // Remark: no F2.reserve(c), to minimise memory-duplication:
    for (auto it = F.begin(); it != F.end(); )
      F2.push_back(std::move(F.extract(it++).value()));
    // That F is now empty, is not guaranteed by the standard, but is
    // reasonable to expect:
    assert(F.empty() and F2.size() == c);
    const auto R = rename_clauselist(F2,true);
    return {{{R.first,c}, F2}, R};
  }


  enum class QError {
    qblock_index = 80,
    empty_clause_block = 81,
    empty_clause = 82,
  };

}

#endif
