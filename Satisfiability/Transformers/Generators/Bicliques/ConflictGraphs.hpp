// Oliver Kullmann, 27.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Conflict graphs of clause-sets etc.

*/

#ifndef CONFLICTGRAPHS_ipM62kMu5w
#define CONFLICTGRAPHS_ipM62kMu5w

#include <istream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <cassert>

#include <Transformers/Generators/Random/ClauseSets.hpp>

#include "Graphs.hpp"

namespace ConflictGraphs {

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


  /* Reading strict Dimacs
      - for the parameters n, c: n is an upper bound for the occurring
        var-indices, c is the exact number of clauses.
  */
  typedef RandGen::gen_uint_t var_t;
  typedef RandGen::Var Var;
  typedef RandGen::Lit Lit;
  typedef RandGen::Clause Clause;
  typedef RandGen::ClauseList ClauseList;
  typedef RandGen::dimacs_pars dimacs_pars;
  typedef RandGen::DimacsClauseList DimacsClauseList;

  dimacs_pars read_strict_dimacs_pars(std::istream& in) noexcept {
    std::string line;
    do {
      std::getline(in, line); assert(in and not line.empty());
    } while (line.front() == 'c');
    assert(in and not line.empty() and line.starts_with("p cnf "));
    std::stringstream s(line.substr(6));
    var_t n; s >> n; var_t c; s >> c; assert(s);
    return {n,c};
  }

  Lit read_strict_literal(std::istream& in) noexcept {
    std::string s;
    in >> s;
    assert(in); assert(not s.empty());
    if (s.starts_with('-')) return {std::stoull(s.substr(1)), -1};
    else return {std::stoull(s), +1};
  }
  Clause read_strict_clause(std::istream& in) noexcept {
    assert(in);
    Clause res;
    Lit x;
    while ((x = read_strict_literal(in)).v != Var{0}) res.push_back(x);
    return res;
  }
  DimacsClauseList read_strict_Dimacs(std::istream& in) noexcept {
    assert(in);
    DimacsClauseList res;
    res.first = read_strict_dimacs_pars(in); assert(in);
    var_t c = res.first.c;
    res.second.reserve(c);
    for (; c != 0; --c) res.second.push_back(read_strict_clause(in));
    return res;
  }


  // Element-wise complementation:
  inline Clause ewcompl(const Clause& C) {
    Clause res(C);
    std::for_each(res.begin(), res.end(), [](Lit& x){x.sign = -x.sign;});
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

}

#endif
