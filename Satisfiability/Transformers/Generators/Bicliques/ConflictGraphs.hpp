// Oliver Kullmann, 27.2.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Conflict graphs of clause-sets etc.

   - Import from Random/ClauseSets.hpp:
    - var_t : typedef for gen_uint_t
      size_t = var_t
    - Var, Lit, Clause, ClauseList, dimacs_pars, DimacsClauseList
    - varlist_t
    - indexlist_t = std::vector<size_t>

   General algorithmic tools:
    - list2map(vector<X>) -> map<x,size_t>
    - filter_rename(indexlist_t, std::map) -> indexlist_t :
        remove elements not in the map, and rename the rest.

   General tools for clause-sets:
    - ewcompl(Clause) -> Clause (elementwise complementation)
    - ewcompl(ClauseList) -> ClauseList


   The conflict-graph:

    - conflictgraph_bydef(DimacsClauseList) -> AdjVecUInt
        by pairwise comparison

    - More efficient approach by occurrence-lists:
     - struct OccVar : containing an array o of size 2 of vectors of
       literal-occurrence-indices (lit_occ_t)
       - default constructor
       - constructor from occurrence-lists
       - operator [bool], [Lit x] for accessing the occurrence-lists
         (const and non-const)
       - conflicts(Lit x) -> -> const lit_occ_t&
       - test-functions trivial, pure, singular, osingular, nonosingular,
         nonsingular
       - operator ==
     - struct AllOcc : contains a vector of OccVar (for every variable its
       occurrences)
     - allocc(DimacsClauseList) -> AllOcc
     - conflictgraph(var_t c, AllOcc) -> AdjVecUInt
         (works by adding bicliques)
     - conflictgraph(indexlist_t, varlist_t, AllOcc) :
         more generally, for a list of clauses and variables
     - conflictgraph(DimacsClauseList) -> AdjVecUInt

    - cc_by_dfs(DimacsClauseList) -> CCbyIndices
        returns the vector stating for every clause-index to which connected
        component of the global-conflict graph it belongs;
        algorithm the same as GraphTraversal::cc_by_dfs, however not
        explicitly creating the conflict-graph but using the occurrence-lists
        to access the adjacent vertices.

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
#include <stack>
#include <map>

#include <cassert>

#include <Transformers/Generators/Random/ClauseSets.hpp>

#include "Graphs.hpp"
#include "GraphTraversal.hpp"
#include "DimacsTools.hpp"
#include "Algorithms.hpp"

namespace ConflictGraphs {

  typedef RandGen::gen_uint_t var_t;
  typedef RandGen::Var Var;
  typedef RandGen::Lit Lit;
  typedef RandGen::Clause Clause;
  typedef RandGen::ClauseList ClauseList;
  typedef RandGen::dimacs_pars dimacs_pars;
  typedef RandGen::DimacsClauseList DimacsClauseList;
  typedef DimacsTools::varlist_t varlist_t;

  typedef var_t size_t;
  typedef std::vector<size_t> indexlist_t;



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


  // Does not assume the elements of F to be sorted, but sorts them:
  Graphs::AdjVecUInt conflictgraph_bydef(DimacsClauseList F) {
    assert(valid(F));
    DimacsTools::msort_elements(F);
    const auto c = F.first.c;
    Graphs::AdjVecUInt G(Graphs::GT::und, c);
    if (c <= 1) return G;
    Graphs::AdjVecUInt::adjlist_t A(c);
    const ClauseList FC = ewcompl(F.second);
    for (size_t i = 0; i < c-1; ++i) {
      const Clause& C = F.second[i];
      if (C.empty()) continue;
      auto& neighbours = A[i];
      for (size_t j = i+1; j < c; ++j)
        if (not Algorithms::empty_intersection(C, FC[j])) {
          neighbours.push_back(j);
          A[j].push_back(i);
        }
    }
    for (auto& v : A) {
      std::ranges::sort(v);
      assert(std::ranges::adjacent_find(v) == v.end());
    }
    G.set(std::move(A));
    assert(A.empty());
    return G;
  }


  // The occurrences of a variable:
  struct OccVar {
    typedef std::vector<var_t> lit_occ_t; // clause-index
    typedef std::array<lit_occ_t, 2> var_occ_t; // index 0 -> false (negative)
    var_occ_t o;

    OccVar() noexcept = default;
    OccVar(lit_occ_t l, lit_occ_t r) : o({l,r}) {}

    lit_occ_t& operator[](const bool s) noexcept { return o[s]; }
    const lit_occ_t& operator[](const bool s) const noexcept { return o[s]; }
    lit_occ_t& operator[](const Lit x) noexcept { return o[x.s]; }
    const lit_occ_t& operator[](const Lit x) const noexcept { return o[x.s]; }

    const lit_occ_t& conflicts(const Lit x) const noexcept {
      return o[not x.s];
    }

    bool trivial() const noexcept {
      return o[0].empty() and o[1].empty();
    }
    bool pure() const noexcept {
      return not trivial() and (o[0].empty() or o[1].empty());
    }
    bool singular() const noexcept {
      return not pure() and (o[0].size() == 1 or o[1].size() == 1);
    }
    bool osingular() const noexcept {
      return o[0].size() == 1 and o[1].size() == 1;
    }
    bool nonosingular() const noexcept {
      return (o[0].size() == 1 and o[1].size() >= 2)
        or (o[1].size() == 1 and o[0].size() >= 2);
    }
    bool nonsingular() const noexcept {
      return o[0].size() >= 2 and o[1].size() >= 2;
    }

    size_t deg() const noexcept { return o[0].size() + o[1].size(); }
    size_t muldeg() const noexcept { return o[0].size() * o[1].size(); }
    size_t maxdeg() const noexcept { return std::max(o[0].size(), o[1].size()); }
    size_t mindeg() const noexcept { return std::min(o[0].size(), o[1].size()); }
    size_t deg(const bool s) const noexcept { return o[s].size(); }
    size_t deg(const Lit x) const noexcept { return o[x.s].size(); }

    bool operator ==(const OccVar&) const noexcept = default;
  };

  // All occurrences of variables:
  struct AllOcc {
    typedef std::vector<OccVar> occ_t;
    occ_t O;

    AllOcc() noexcept = default;
    explicit AllOcc(const var_t n) : O(n) {}
    AllOcc(occ_t O) : O(O) {}

    var_t size() const noexcept {
      return O.size();
    }

    const OccVar& operator[](const Var v) const noexcept {
      const var_t v0 = v.v;
      assert(1 <= v0 and v0 <= O.size());
      return O[v0-1];
    }

    typedef OccVar::lit_occ_t lit_occ_t;

    lit_occ_t& operator[](const Lit x) noexcept {
      const var_t v = x.v.v;
      assert(1 <= v and v <= O.size());
      return O[v-1][x.s];
    }
    const lit_occ_t& operator[](const Lit x) const noexcept {
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


  Graphs::AdjVecUInt conflictgraph(const var_t c, const AllOcc& O) {
    Graphs::AdjVecUInt G(Graphs::GT::und, c);
    if (c <= 1) return G;
    Graphs::AdjVecUInt::adjlist_t A(c);
    const var_t n = O.size();
    for (var_t v0 = 0; v0 < n; ++v0) {
      const var_t v = v0+1;
      Graphs::add_biclique(A, Graphs::GT::und, O[Lit(v)], O[-Lit(v)]);
    }
    for (auto& v : A) {
      std::ranges::sort(v);
      const auto dup = std::ranges::unique(v);
      v.erase(dup.begin(), dup.end());
    }
    G.set(std::move(A));
    assert(A.empty());
    return G;
  }
  Graphs::AdjVecUInt conflictgraph(const DimacsClauseList& F) {
    return conflictgraph(F.first.c, allocc(F));
  }

  // Translate list (x_1, ..., x_n) to map x_i -> i :
  template <typename X>
  std::map<X, size_t> list2map(const std::vector<X>& v) {
    std::map<X, size_t> res;
    for (size_t i = 0; i < v.size(); ++i) res.insert({v[i], i});
    return res;
  }
  indexlist_t filter_rename(const indexlist_t& L,
                            const std::map<size_t, size_t>& m) {
    indexlist_t res;
    const auto end = m.end();
    for (const size_t x : L) {
      const auto f = m.find(x);
      if (f != end) res.push_back(f->second);
    }
    return res;
  }

  // More generally, use a list of clause-indices and a list of variables:
  Graphs::AdjVecUInt conflictgraph(const indexlist_t& F, const varlist_t& V,
                                   const AllOcc& O) {
    const auto c = F.size(), n = V.size();
    Graphs::AdjVecUInt G(Graphs::GT::und, c);
    if (c <= 1 or n == 0) return G;
    Graphs::AdjVecUInt::adjlist_t A(c);
    const auto map = list2map(F);
    for (size_t i = 0; i < n; ++i) {
      const Var v = V[i];
      Graphs::add_biclique(A, Graphs::GT::und,
        filter_rename(O[Lit(v)], map), filter_rename(O[-Lit(v)], map));
    }
    for (auto& v : A) {
      std::ranges::sort(v);
      const auto dup = std::ranges::unique(v);
      v.erase(dup.begin(), dup.end());
    }
    G.set(std::move(A));
    assert(A.empty());
    return G;
  }


  GraphTraversal::CCbyIndices cc_by_dfs(const DimacsClauseList& F,
                                        const AllOcc& O) {
    assert(valid(F));
    const var_t c = F.first.c;
    GraphTraversal::CCbyIndices res(c);
    for (var_t v = 0; v < c; ++v) {
      if (res.cv[v] != 0) continue;
      res.cv[v] = ++res.numcc;
      std::stack<id_t> S;
      for (const Lit x : F.second[v])
        for (const var_t w : O.conflicts(x))
          if (res.cv[w] == 0) S.push(w);
      while (not S.empty()) {
        const id_t v = S.top(); S.pop();
        if (res.cv[v] != 0) continue;
        res.cv[v] = res.numcc;
        for (const Lit x : F.second[v])
          for (const var_t w : O.conflicts(x))
            if (res.cv[w] == 0) S.push(w);
      }
    }
    assert(valid(res));
    assert(res.canonical());
    return res;
  }
  GraphTraversal::CCbyIndices cc_by_dfs(const DimacsClauseList& F) {
    return cc_by_dfs(F, allocc(F));
  }

}

#endif
