// Oliver Kullmann, 12.2.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Components related to clauses and partial assignments

  - unsigned integral type Count_t
  - enumeration VT, auxiliary VTvector, classification functions et, at
  - classes Varset, AVarset, EVarset, AVarSetsystem
  - clases Dependency (iterator to element of AVarSetsystem), Dependency_p
    (pointer form)
  - class Dvector
  - class DepCounts (map from Dependency_p to Count_t)
  - classes Clause, AClause, EClause
  - function output_clause
  - class PairClause
  - class DClause, extending PairClause to a proper concept
  - class Degree_vec (vector of Count_t)
  - class CLS (vector of Clause)
  - class DCLS (set of DClause)
  - dclause_it (iterator to element of DCLS)

  - DClauseSet : extends DCLS to a proper concept

  - class Pass (map from EVar to Litc)
  - class PassSet

*/

#ifndef CLAUSESETS_w3jcQND1oa
#define CLAUSESETS_w3jcQND1oa

#include <ostream>
#include <set>
#include <vector>
#include <map>
#include <utility>
#include <bitset>
#include <tuple>
#include <limits>

#include <cstdint>

#include "VarLit.hpp"

namespace ClauseSets {

  // TEMPORARY:
  template <typename T>
  std::ostream& operator <<(std::ostream& out, const std::set<T>& S) noexcept {
    for (const T& x : S) out << " " << x;
    return out;
  }

  typedef std::uint_fast64_t Count_t;
  static_assert(std::numeric_limits<Count_t>::digits >= 64);
  static_assert(std::numeric_limits<Count_t>::is_integer);
  static_assert(not std::numeric_limits<Count_t>::is_signed);

  enum class VT { und=0, fa, fe, a, e }; // variable types, with "f" for "formal"
  static_assert(static_cast<int>(VT::und) == 0);
  std::ostream& operator <<(std::ostream& out, const VT t) noexcept {
    switch (t) {
    case VT::fa : return out << "fa";
    case VT::fe : return out << "fe";
    case VT::a : return out << "a";
    case VT::e : return out << "e";
    default : return out << "und";
    }
  }
  typedef std::vector<VT> VTvector;
  inline constexpr bool et(const VT t) noexcept {return t==VT::fe or t==VT::e;}
  static_assert(et(VT::e) and et(VT::fe));
  inline constexpr bool at(const VT t) noexcept {return t==VT::fa or t==VT::a;}
  static_assert(at(VT::a) and at(VT::fa));

  typedef std::set<VarLit::Var> Varset;
  typedef Varset AVarset;
  typedef Varset EVarset;
  typedef std::set<AVarset> AVarSetsystem;
  typedef AVarSetsystem::const_iterator Dependency;
  typedef std::vector<Dependency> Dvector;
  typedef AVarSetsystem::const_pointer Dependency_p;
  typedef std::map<Dependency_p, Count_t> DepCounts;

  typedef std::set<VarLit::Lit> Clause;
  typedef Clause AClause;
  typedef Clause EClause;
  // Special output, comma-separated:
  void output_clause(std::ostream& out, const Clause& C) noexcept {
    const auto begin = C.begin(), end = C.end();
    for (auto i = begin; i != end; ++i)
      if (i != begin) out << "," << *i; else out << *i;
  }

  typedef std::pair<AClause,EClause> PairClause;
  // Wrapper around PairClause, additionally with index (valid iff >= 1)
  // of clause in the input:
  struct DClause {
    PairClause P;
    const Count_t index = 0;
    void clear() noexcept {P.first.clear(); P.second.clear();}
    bool pseudoempty() const noexcept {return P.second.empty();}
    bool empty() const noexcept {return P.first.empty() and P.second.empty();}
    bool operator ==(const DClause C) const noexcept {return P == C.P;}
    bool operator !=(const DClause C) const noexcept {return P != C.P;}
    friend bool operator <(const DClause& C, const DClause& D) noexcept {
      return C.P < D.P;
    }
    friend std::ostream& operator <<(std::ostream& out, const DClause& C) noexcept {
      out << "E={";
      output_clause(out, C.P.second);
      out << "} A={";
      output_clause(out, C.P.first);
      return out <<"} " << C.index;
    }
  };

  typedef std::vector<Count_t> Degree_vec;

  typedef std::vector<Clause> CLS;

  typedef std::set<DClause> DCLS;
  typedef DCLS::const_iterator dclause_it;

  struct DClauseSet {
    DCLS F;
    VTvector vt; // for each variable its type
    AVarSetsystem dep_sets; // the occurring d-sets
    Dvector D; // for each variable its d-set
    DepCounts dc; // map dep-pointer -> how often each d-set occurs

    // Statistics:
    //   from the parameter line:
    VarLit::Var n_pl;
    Count_t c_pl;
    //   from dependency-specification:
    VarLit::Var na_d = 0, ne_d = 0;
    //   actually occurring in clauses (with tautological clauses removed):
    Degree_vec vardeg;
    VarLit::Var max_a_index=0, max_e_index=0, max_index=0; // maximal occurring variable-index
    VarLit::Var na=0, ne=0, n=0; // number occurring e/a/both variables
    VarLit::Var max_a_length=0, max_e_length=0, max_c_length=0; // max number of a/e/both literals in clauses
    VarLit::Var max_s_dep=0, min_s_dep=VarLit::max_lit, count_dep=0;
    Count_t c=0; // number of clauses (without tautologies or repetitions)
    Count_t la=0, le=0, l=0, lrep=0; // number of a/e/both/repeated literal occurrences
    Count_t t=0, empty=0, pempty=0, repeated=0; // number of tautological/empty/pseudoempty/repeated clauses

    friend std::ostream& operator <<(std::ostream& out, const DClauseSet& F) noexcept {
      out << "c  List of variables:\nc   ";
      for (VarLit::Var v = 1; v < F.vt.size(); ++v)
        out << " " << v << ":" << F.vt[v];
      out << "\nc  p cnf " << F.max_index << " " << F.c << "\n";
      for (VarLit::Var v = 1; v < F.vt.size(); ++v)
        if (F.vt[v] == VT::e) out << "c  d " << v << *F.D[v] << " 0\n";
      Count_t i = 0;
      for (const auto& C : F.F) out << "c  C[" << ++i << "]: " << C << "\n";
      return out;
    }
  };

  typedef std::map<VarLit::EVar, VarLit::Litc> Pass;
  inline VarLit::Litc eval(const Pass& phi, const VarLit::ELit x) noexcept {
    const VarLit::Var v = var(x);
    const auto it = phi.find(v);
    if (it == phi.end()) return {};
    const VarLit::Litc val = it->second;
    return (x.posi()) ? val : -val;
  }

  typedef std::set<Pass> PassSet;

  /* Testing whether phi fits F: for the result res holds
      - res[0] is true iff there exists a variable outside of F,
      - res[1] is true if there exists a variable of F, with phi(v) not
        according to dependency-specification.
     Uses F.vt, F.D, F.max_e_index.
  */
  std::bitset<2> check(const Pass& phi, const DClauseSet& F) noexcept {
    std::bitset<2> res;
    for (const auto [v,val] : phi) {
      if (v <= F.max_e_index and F.vt[v] == VT::e) {
        const VarLit::Var w = var(VarLit::ALit(val));
        if (bool(w) and F.D[v]->find(w) == F.D[v]->end()) res.set(1);
      }
      else res.set(0);
      if (res.all()) break;
    }
    return res;
  }

  /* Assumes that F does not contain tautological clauses
     Returns the number of touched, satisfied, and
     untouched clauses.

  */
  typedef std::tuple<VarLit::EVar, VarLit::EVar, VarLit::EVar> Count3;
  Count3 eval(const Pass& phi, const DClauseSet& F) noexcept {
    Count3 res{};
    for (const DClause& C : F.F) {
      bool touched = false, satisfied = false;
      const AClause& A = C.P.first, E = C.P.second;
      const auto Aend = A.end();
      for (const VarLit::ELit x : E) {
        const VarLit::Litc val = eval(phi,x);
        if (val.sing()) continue;
        touched = true;
        if (val.constant()) {
          if (val == VarLit::bf(true)) {satisfied=true; break;}
          else continue;
        }
        if (A.find(-VarLit::Lit(val)) != Aend) {satisfied=true; break;}
      }
      if (touched) {
        ++std::get<0>(res);
        std::get<1>(res) += satisfied;
      }
      else ++std::get<2>(res);
    }
    return res;
  }

}

#endif
