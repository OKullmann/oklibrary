// Oliver Kullmann 19.12.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/ClauseLength/TransCNF3CNF.cpp
  \brief Simple program for translation CNF -> k-CNF, mainly for teaching
  purposes

  USAGE:

> ./CNF3CNF [k=3]

  reads a DIMACS CNF from standard input,
  outputs a DIMACS max(3,k)-CNF on standard output (with comments removed).

*/

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <limits>

#include <cassert>
#include <cstdint>
#include <cstdlib>

namespace {

  const std::string version = "0.2";
  const std::string date = "20.12.2018";
  const std::string program = "CNF3CNF"
#ifndef NDEBUG
    "_debug"
#endif
;

  typedef std::uint64_t Var;
  typedef std::int64_t Lit;

  constexpr Var max_var = Var(std::numeric_limits<Lit>::max() - 1);
  constexpr Lit max_lit = Lit(max_var);
  constexpr Lit min_lit = -max_lit;
  static_assert(-max_lit == min_lit);
  static_assert(-min_lit == max_lit);
  static_assert(Lit(max_var) == max_lit);
  inline constexpr bool valid(const Var v) noexcept { return v <= max_var; }
  static_assert(valid(max_var));
  inline constexpr bool valid(const Lit x) noexcept {
    return x >= min_lit and x <= max_lit;
  }
  static_assert(valid(min_lit));
  static_assert(valid(max_lit));
  inline constexpr Var var(const Lit x) noexcept {
    assert(valid(x));
    return (x > 0) ? x : -x;
  }
  static_assert(var(min_lit) == max_var);
  static_assert(var(max_lit) == max_var);

  typedef std::uint64_t ClauseCount;
  struct Parameters {
    Var n; ClauseCount c;
  };

  typedef std::vector<Lit> Clause;
  typedef std::vector<Clause> ClauseList;

  std::ostream& operator <<(std::ostream& out, const Clause& C) {
    for (const Lit x : C) out << x << " ";
    return out << "0\n";
  }
  std::ostream& operator <<(std::ostream& out, const ClauseList F) {
    for (const Clause& C : F) out << C;
    return out;
  }

  Parameters read(std::istream& in) {
    std::string line;
    while (std::getline(in, line)) {
      if (line.empty()) std::exit(1);
      const auto first = line[0];
      if (first == 'c') continue;
      if (first == 'p') break;
      std::exit(1);
    }
    if (not in) std::exit(1);
    assert(line[0] = 'p');
    std::stringstream pline{line};
    {std::string token;
     pline >> token;
     if (token != "p") std::exit(1);
     pline >> token;
     if (token != "cnf") std::exit(1);}
    Var n; pline >> n; if (not pline) std::exit(1);
    if (not valid(n)) std::exit(1);
    ClauseCount c; pline >> c; if (not pline) std::exit(1);
    return {n,c};
  }

  ClauseList read(const Parameters p, std::istream& in) {
    ClauseList F; F.reserve(p.c);
    while (F.size() < p.c) {
      Clause C;
      for (Lit x; (in >> x) and x; C.push_back(x));
      if (not in) std::exit(1);
      if (C.empty()) std::exit(1);
      for (const Lit x : C) if (not valid(x)) std::exit(1);
      F.push_back(std::move(C));
    }
    assert(F.size() == p.c);
    return F;
  }

  Var aux_vars(const ClauseList& F, const Var target) noexcept {
    Var n = 0;
    for (const Clause& C : F) if (C.size() > target) n += C.size() - target;
    return n;
  }

  ClauseList transform(ClauseList& F, const Parameters p, const Var num_aux_var, const Var target) {
    ClauseList G; G.reserve(num_aux_var);
    Var aux = p.n;
    for (Clause& C : F)
      while (C.size() > target) {
        const Lit x = C.back(); C.pop_back();
        const Lit y = C.back(); C.pop_back();
        const Var v = ++aux;
        C.push_back(v);
        G.emplace_back(Clause{x,y,-Lit(v)});
      }
    assert(G.size() == num_aux_var);
    assert(aux - p.n == num_aux_var);
    assert(F.size() == p.c);
    return G;
  }

}

int main(const int argc, const char* const argv[]) {
  constexpr Var default_target_clause_length = 3;
  const Var target_clause_length = (argc > 1) ?
    std::max(default_target_clause_length, Var(std::stoull(argv[1]))) :
    default_target_clause_length;
  if (not valid(target_clause_length)) std::exit(1);

  const Parameters p = read(std::cin);
  ClauseList F = read(p, std::cin);

  const Var num_aux_vars = aux_vars(F, target_clause_length);
  const ClauseList G = transform(F, p, num_aux_vars, target_clause_length);

  std::cout << "p cnf " << p.n + num_aux_vars << " " << p.c + num_aux_vars << "\n";
  std::cout << F << G;
}
