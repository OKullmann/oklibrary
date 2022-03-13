// Oliver Kullmann, 27.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Conflict graphs of clause-sets etc.

   - Import from Random/ClauseSets.hpp
    - var_t Var, Lit, Clause, ClauseList, dimacs_pars, DimacsClauseList

   General algorithmic tools:
    - empty_intersection(RAN r1, RAN r2)

   General tools for clause-sets:
    - ewcompl(Clause) (elementwise complementation)
    - ewcompl(ClauseList)

   The cnflict-graph:
    - conflictgraph_bydef(DimacsClauseList)

*/

#ifndef CONFLICTGRAPHS_ipM62kMu5w
#define CONFLICTGRAPHS_ipM62kMu5w

#include <istream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <array>

#include <cassert>

#include <Transformers/Generators/Random/ClauseSets.hpp>

#include "Graphs.hpp"

namespace ConflictGraphs {

  typedef RandGen::gen_uint_t var_t;
  typedef RandGen::Var Var;
  typedef RandGen::Lit Lit;
  typedef RandGen::Clause Clause;
  typedef RandGen::ClauseList ClauseList;
  typedef RandGen::dimacs_pars dimacs_pars;
  typedef RandGen::DimacsClauseList DimacsClauseList;


  // For sorted ranges decide whether their intersection is empty:
  template <class RAN>
  inline bool empty_intersection(const RAN& r1, const RAN& r2) noexcept {
    if (r1.empty() or r2.empty()) return true;
    const auto e1 = r1.end(); const auto e2 = r2.end();
    auto i1 = r1.begin(); auto i2 = r2.begin();
    if (r1.size() <= r2.size()) {
      for (;;) {
        while (*i1 < *i2) { ++i1; if (i1 == e1) return true; }
        if (*i1 == *i2) return false;
        do { ++i2; if (i2 == e2) return true; } while  (*i2 < *i1);
        if (*i2 == *i1) return false;
      }
    }
    else {
      for (;;) {
        while (*i2 < *i1) { ++i2; if (i2 == e2) return true; }
        if (*i2 == *i1) return false;
        do { ++i1; if (i1 == e1) return true; } while (*i1 < *i2);
        if (*i1 == *i2) return false;
      }
    }
    return true;
  }


  // Element-wise complementation:
  inline Clause ewcompl(const Clause& C) {
    Clause res(C);
    std::for_each(res.begin(), res.end(), [](Lit& x){x.neg();});
    return res;
  }
  ClauseList ewcompl(const ClauseList& F) {
    ClauseList res; res.reserve(F.size());
    std::transform(F.begin(), F.end(), std::back_inserter(res),
                   [](const Clause& C){return ewcompl(C);});
    return res;
  }


  Graphs::AdjVecUInt conflictgraph_bydef(const DimacsClauseList& F) {
    Graphs::AdjVecUInt G(Graphs::GT::und, F.first.c);
    if (F.first.c <= 1) return G;
    Graphs::AdjVecUInt::adjlist_t A(F.first.c);
    const ClauseList FC = ewcompl(F.second);
    for (size_t i = 0; i < F.first.c-1; ++i) {
      const Clause& C = F.second[i];
      if (C.empty()) continue;
      auto& neighbours = A[i];
      for (size_t j = i+1; j < F.first.c; ++j)
        if (not empty_intersection(C, FC[j])) {
          neighbours.push_back(j);
          A[j].push_back(i);
        }
    }
    for (auto& v : A) std::ranges::sort(v);
    G.set(std::move(A));
    assert(A.empty());
    return G;
  }


  // The occurrences of a variable:
  struct OccVar {
    typedef std::vector<var_t> lit_occ_t;
    typedef std::array<lit_occ_t, 2> var_occ_t; // 0 = false
    var_occ_t o;

    OccVar() noexcept = default;
    OccVar(lit_occ_t l, lit_occ_t r) : o({l,r}) {}

    lit_occ_t& operator[](const bool s) { return o[s]; }
    const lit_occ_t& operator[](const bool s) const { return o[s]; }
    lit_occ_t& operator[](const Lit x) { return o[x.s]; }
    const lit_occ_t& operator[](const Lit x) const { return o[x.s]; }

    const lit_occ_t& conflicts(const Lit x) const noexcept {
      return o[not x.s];
    }
    bool operator ==(const OccVar&) const noexcept = default;
  };

  // All occurrences of variables:
  struct AllOcc {
    typedef std::vector<OccVar> occ_t;
    occ_t O;

    AllOcc() noexcept = default;
    explicit AllOcc(const var_t n) : O(n) {}
    AllOcc(occ_t O) : O(O) {}

    typedef OccVar::lit_occ_t lit_occ_t;

    lit_occ_t& operator[](const Lit x) {
      const var_t v = x.v.v;
      assert(1 <= v and v <= O.size());
      return O[v-1][x.s];
    }
    const lit_occ_t& operator[](const Lit x) const {
      const var_t v = x.v.v;
      assert(1 <= v and v <= O.size());
      return O[v-1][x.s];
    }

    const lit_occ_t& conflicts(const Lit x) const noexcept {
      const var_t v = x.v.v;
      assert(1 <= v and v <= O.size());
      return O[v-1].conflicts(x);
    }
    bool operator ==(const AllOcc&) const noexcept = default;
  };

  AllOcc allocc(const DimacsClauseList& F) {
    assert(valid(F));
    AllOcc res(F.first.n);
    const var_t c = F.first.c;
    for (var_t i = 0; i < c; ++i)
      for (const Lit x : F.second[i])
        res[x].push_back(i);
    return res;
  }

}

#endif
