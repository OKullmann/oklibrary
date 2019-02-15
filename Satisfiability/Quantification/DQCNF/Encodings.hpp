// Oliver Kullmann, 15.2.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Components related to encodings (of variables) */

#ifndef ENCODINGS_0kFHsYMfXJ
#define ENCODINGS_0kFHsYMfXJ

#include <vector>
#include <set>
#include <map>

#include "VarLit.hpp"
#include "ClauseSets.hpp"

namespace Encodings {

  // TEMPORARY:
  template <typename T>
  std::ostream& operator <<(std::ostream& out, const std::set<T>& S) noexcept {
    for (const T& x : S) out << " " << x;
    return out;
  }
  template <typename T>
  std::ostream& operator <<(std::ostream& out, const std::vector<T>& v) noexcept {
    for (const T& x : v) out << " " << x;
    return out;
  }
  template <typename A, typename V>
  std::ostream& operator <<(std::ostream& out, const std::map<A,V>& M) noexcept {
    for (const auto& p : M) out << " (" << p.first << "," << p.second << ")";
    return out;
  }

  struct Encoding {

    using Var = VarLit::Var;
    using EVar = VarLit::EVar;
    using AVar = VarLit::AVar;
    using Lit = VarLit::Lit;
    using ELit = VarLit::ELit;
    using ALit = VarLit::ALit;
    using Litc = VarLit::Litc;

    const ClauseSets::DClauseSet& F;

    // Vector of existential variables:
    typedef std::vector<EVar> Evar_vec;
    const Evar_vec E;
    // For each e-var its index in E;
    const Evar_vec E_index;

    // Vector of dependencies as sorted vectors:
    typedef std::vector<AVar> Avar_vec;
    typedef std::vector<Avar_vec> Dep_vec;
    const Dep_vec dep;

    // Vector of Dclause-iterators for e-var-indices:
    typedef std::vector<ClauseSets::dclause_it> Dclause_vec;
    typedef Dclause_vec::size_type clause_index_t;
    const Dclause_vec dclauses;

    // Vector of bf-indices (for given e-index the starting point, plus at the
    // last position the "past-the-end"-index):
    typedef std::vector<Var> Var_vec;
    const Var_vec bfvar_indices;

    // The set of all occurring partial assignments (satisfying clauses):
    typedef ClauseSets::PassSet::const_pointer Pass_p;
    typedef std::set<Pass_p> Solution_set; // yields the pa-variables
    typedef std::vector<Solution_set> Solset_vec; // for each clause-index the set of pointers to solution-pass's
    typedef std::pair<ClauseSets::PassSet, Solset_vec> All_solutions;
    const All_solutions all_solutions;

    const Var ncs, nbf, npa, n;

    typedef std::map<Pass_p,Var> EncodingPass;
    const EncodingPass enc_pass;

    Encoding(const ClauseSets::DClauseSet& F) :
      F(F), E(extract_evar()), E_index(extract_eindices()), dep(convert_dependencies()), dclauses(list_iterators()), bfvar_indices(set_bfvar_indices()), all_solutions(set_all_solutions()), ncs(F.c), nbf(bfvar_indices.back() - F.c - 1), npa(all_solutions.first.size()), n(ncs+nbf+npa), enc_pass(set_pass_encoding()) {
  #if not NDEBUG
      for (auto it = all_solutions.first.begin(); it != all_solutions.first.end(); ++it)
        assert(enc_pass.find(&*it) != enc_pass.end());
  #endif
    }

    Var csvar(const clause_index_t C) const noexcept {
      assert(C < F.c);
      return C+1;
    }

    Var bfvar(const EVar v, const Litc f) const noexcept {
      assert(v >= 1);
      assert(v <= F.max_e_index);
      assert(not f.sing());
      const Var i = E_index[v];
      assert(i < F.ne);
      const ALit x{f};
      const AVar w = var(x);
      assert(w <= F.max_a_index);
      assert(w == 0 or F.vt[w] == ClauseSets::VT::a or F.vt[w] == ClauseSets::VT::fa);
      assert(w == 0 or F.D[v]->find(w) != F.D[v]->end());
      const Var base_index = bfvar_indices[i];
      if (w == 0) {
        assert(f.constant());
        return (f == VarLit::bf(false)) ? base_index : base_index + 1;
      }
      const auto w_it = std::lower_bound(dep[i].begin(), dep[i].end(), w);
      assert(*w_it == w);
      const Var j = base_index + 2 + 2 * (w_it - dep[i].begin());
      return (x.negi()) ? j : j+1;
    }

    Var pavar(const Pass_p& phi) const noexcept {
      const auto find = enc_pass.find(phi);
      assert(find != enc_pass.end());
      return find->second;
    }

    friend std::ostream& operator <<(std::ostream& out, const Encoding& enc) noexcept {
      out << "c cs-variables and the associated clauses:\n"
             "c ncs = " << enc.ncs << "\n";
      for (clause_index_t i = 0; i < enc.ncs; ++i)
        out << "c  cs[" << enc.csvar(i) << "]: " << *enc.dclauses[i] << "\n";
      out << "c bf-variables and the associated 1-variable partial assignments:\n"
             "c nbf = " << enc.nbf << "\n";
      using ClauseSets::Pass;
      for (Var vi = 0; vi < enc.F.ne; ++vi) {
        const EVar v = enc.E[vi];
        out << "c  evar[" << v << "]: ";
        const Var begin = enc.bfvar_indices[vi], end = enc.bfvar_indices[vi+1];
        out << (end-begin) << " bfs from " << begin << " to " << (end-1) << "\nc   ";
        Var j = begin;
        out << "bf[" << j++ << "]:" << Pass{{v,VarLit::bf(false)}};
        out << ", bf[" << j++ << "]:" << Pass{{v,VarLit::bf(true)}};
        for (const AVar w : enc.dep[vi]) {
          out << ", bf[" << j++ << "]:" << Pass{{v,Litc(Lit(w,VarLit::Pol::n))}};
          out << ", bf[" << j++ << "]:" << Pass{{v,Litc(Lit(w,VarLit::Pol::p))}};
        }
        out << "\n";
      }
      out << "c pa-variables and their partial assignments:\n"
             "c npa = " << enc.npa << "\n";
      {Var v = enc.ncs+enc.nbf;
       for (const Pass& phi : enc.all_solutions.first)
         out << "c  pa[" << ++v << "]:" << phi << "\n";
      }
      out << "c The clauses (see the cs-variables) and their solution-sets (as pa-variables):\n";
      for (clause_index_t ci = 0; ci < enc.ncs; ++ci) {
        out << "c  sol[" << enc.csvar(ci) << "]:";
        const Solution_set& ss = enc.all_solutions.second[ci];
        std::vector<Var> v; v.reserve(ss.size());
        for (Pass_p p : ss) v.push_back(enc.enc_pass.find(p)->second);
        std::sort(v.begin(),v.end());
        out << v << "\n";
      }
      return out;
    }

  private :

    Evar_vec extract_evar() const {
      Evar_vec e;
      e.reserve(F.ne);
      for (Var v = 1; v <= F.max_e_index; ++v)
        if (F.vt[v] == ClauseSets::VT::e) e.push_back(v);
      assert(e.size() == F.ne);
      return e;
    }

    Evar_vec extract_eindices() const {
      Evar_vec ei;
      ei.resize(F.max_e_index+1); // superfluous zero-initialisation
      for (Var i = 0; i < F.ne; ++i) ei[E[i]] = i;
      return ei;
    }

    Dclause_vec list_iterators() const {
      Dclause_vec dc;
      dc.reserve(F.c);
      for (ClauseSets::dclause_it it = F.F.begin(); it != F.F.end(); ++it)
        dc.push_back(it);
      return dc;
    }

    Dep_vec convert_dependencies() const {
      Dep_vec d;
      d.resize(F.ne);
      for (Var i = 0; i < F.ne; ++i) {
        const EVar v = E[i];
        d[i].assign(F.D[v]->begin(), F.D[v]->end());
      }
      return d;
    }

    Var_vec set_bfvar_indices() const {
      Var_vec ind;
      ind.resize(F.ne+1);
      Var current = F.c+1;
      for (Var i = 0; i < F.ne; ++i) {
        ind[i] = current;
        current += 2*F.D[E[i]]->size() + 2;
      }
      ind[F.ne] = current;
      return ind;
    }

    All_solutions set_all_solutions() const {
      All_solutions all_sol;
      all_sol.second.resize(F.c);
      for (ClauseSets::Count_t ci = 0; ci < F.c; ++ci) {
        const ClauseSets::DClause& C(*dclauses[ci]);
        assert(ci < all_sol.second.size());
        Solution_set& pas(all_sol.second[ci]);
        for (const ELit x : C.P.second) { // setting e-literals to true
          const EVar v = var(x);
          ClauseSets::Pass pa; pa[v] = VarLit::bf(x.posi());
          pas.insert(&*all_sol.first.insert(std::move(pa)).first);
        }
        ClauseSets::AVarset V;
        for (const ALit x : C.P.first) { // e-literals as negations of a-literals
          const AVar v = var(x);
          V.insert(v);
          for (const ELit y : C.P.second) {
            const EVar w = var(y);
            if (F.D[w]->find(v) != F.D[w]->end()) {
              ClauseSets::Pass pa; pa[w] = Litc( (y.posi()) ? -x : x);
              pas.insert(&*all_sol.first.insert(std::move(pa)).first);
            }
          }
        }
        for (const ELit x : C.P.second) { // two e-literals negating each other
          const EVar v = var(x);
          const auto begx = F.D[v]->begin();
          const auto endx = F.D[v]->end();
          for (const ELit y : C.P.second) {
            if (not (x < y)) continue;
            const EVar w = var(y);
            assert(v != w);
            std::vector<AVar> I;
            std::set_intersection(begx,endx,F.D[w]->begin(),F.D[w]->end(),std::back_inserter(I));
            for (const AVar u : I) {
              if (V.find(u) != V.end()) continue;
              const Litc u1{ALit(u)}, u2{(sign(x)==sign(y))?-u1:u1};
              ClauseSets::Pass pa; pa[v]=u1, pa[w]=u2;
              pas.insert(&*all_sol.first.insert(pa).first);
              pa.clear(); pa[v]=-u1, pa[w]=-u2;
              pas.insert(&*all_sol.first.insert(std::move(pa)).first);
            }
          }
        }
      }
      return std::move(all_sol); // to emphasise the necessity (maintaining pointers)
    }

    EncodingPass set_pass_encoding() const {
      EncodingPass ep;
      Var v = ncs + nbf;
      for (auto phi_it = all_solutions.first.begin(); phi_it != all_solutions.first.end(); ++phi_it)
        ep[&*phi_it] = ++v;
      assert(v == ncs+nbf+npa);
      return ep;
    }

  };

}

#endif
