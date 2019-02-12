// Oliver Kullmann, 12.2.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Components related to clauses and partial assignmentss */

#ifndef CLAUSESETS_w3jcQND1oa
#define CLAUSESETS_w3jcQND1oa

#include "VarLit.hpp"
#include "Generics.hpp"

namespace ClauseSets {

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

  typedef std::uint_fast64_t Count_t;

  enum class VT { und=0, fa, fe, a, e }; // variable types, with "f" for "formal"
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
  inline constexpr bool at(const VT t) noexcept {return t==VT::fa or t==VT::a;}

  typedef std::set<VarLit::Var> Varset;
  typedef Varset AVarset;
  typedef Varset EVarset;
  typedef std::set<AVarset> VarSetsystem;
  typedef VarSetsystem::const_iterator Dependency;
  typedef std::vector<Dependency> Dvector;
  typedef VarSetsystem::const_pointer Dependency_p;
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

  typedef std::pair<AClause,EClause> PairClause; // all-exists
  struct DClause {
    PairClause P; // A-E
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
    VarSetsystem dep_sets; // the occurring d-sets
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
  typedef std::set<Pass> PassSet;

}

#endif
