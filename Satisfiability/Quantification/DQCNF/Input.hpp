// Oliver Kullmann, 12.2.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Components related to DIMACS input

   Class template ReadDimacs<ErrOut>:
    - construction stores an input stream and its level
    - operator () then returns a const-reference to the constructed
      ClauseSets::DClauseSet.

*/

#ifndef INPUT_4uaSDpRpoG
#define INPUT_4uaSDpRpoG

#include <string>
#include <sstream>
#include <istream>
#include <new>
#include <algorithm>

#include <cstdlib>
#include <cassert>

#include "Generics.hpp"
#include "InOut.hpp"
#include "VarLit.hpp"
#include "ClauseSets.hpp"

namespace Input {

  template <class ErrOut>
  class ReadDimacs {

  using ConformityLevel = InOut::ConformityLevel;
  using Error = InOut::Error;

  std::istream& in;
  ErrOut& errout;
  ClauseSets::DClauseSet F;
  const ConformityLevel conlev;
  ClauseSets::Count_t current_line_number = 0; // starting with 1; final value the last dependency (or p-line, if no dependencies)
  ClauseSets::Count_t current_clause_index = 0; // starting with 1

  /* Reads header-data from in into F:
      - setting F.n_pl, F.c_pl
      - aborts via std::exit in case of input-errors, with error-message on
        errout.
  */
  void read_header() noexcept {
    assert(in.exceptions() == 0);
    assert(in.good());
    std::string line;
    while (true) {
      std::getline(in, line); ++current_line_number;
      const char c = line[0];
      if (c == '\0') {
        errout << "Line" << current_line_number << "Empty line (no p-line found).";
        std::exit(Generics::code(Error::empty_line));
      }
      assert(not line.empty());
      if (c == 'p') break;
      if (c != 'c') {
        errout << "Line" << current_line_number << "Comment lines must start with \"c\".";
        std::exit(Generics::code(Error::bad_comment));
      }
    }
    assert(line[0] == 'p');
    std::stringstream s(line);
    {std::string inp; s >> inp;
     if (inp != "p") {
       errout << "Line" << current_line_number << "Syntax error in parameter line (\"p\" not followed by space).";
       std::exit(Generics::code(Error::file_pline));
     }
     assert(s);
     if (s.eof()) {
       errout << "Line" << current_line_number << "Syntax error in parameter line (p-line ends after \"p\").";
       std::exit(Generics::code(Error::file_pline));
     }
     s >> inp;
     if (not s or inp != "cnf") {
       errout << "Line" << current_line_number << "Syntax error in parameter line (no \"cnf\").";
       std::exit(Generics::code(Error::file_pline));
     }
    }
    s >> F.n_pl;
    if (not s) {
      errout << "Line" << current_line_number << "Reading error with parameter maximal-variable-index "
        "(too big or not-a-number).";
      std::exit(Generics::code(Error::file_pline));
    }
    if (not VarLit::valid(F.n_pl)) {
      errout << "Line" << current_line_number << "Parameter maximal-variable-index n=" << F.n_pl <<
        " is too big for numeric_limits<Lit_int>::max=" << VarLit::max_lit;
      std::exit(Generics::code(Error::num_vars));
    }
    s >> F.c_pl;
    if (not s) {
      errout << "Line" << current_line_number << "Reading error with parameter number-of-clauses "
        "(too big or not-a-number).";
      std::exit(Generics::code(Error::num_cls));
    }
    if (not s.eof()) {
      errout << "Line" << current_line_number << "Syntax error in parameter line (something after c-parameter).";
      std::exit(Generics::code(Error::file_pline));
    }
  }

  /* Read dependency-data from in into F:
      - setting F.vt, F.D, F.dep_sets, F.na_d, F.ne_d;
      - aborts via std::exit in case of input-errors, with error-message on
        errout.
  */
  void read_dependencies() noexcept {
    assert(in.exceptions() == 0);
    assert(in.good());
    try { F.vt.resize(F.n_pl+1); F.D.resize(F.n_pl+1); }
    catch (const std::bad_alloc&) {
      errout << "Allocation error for vt- + dependency-vector of size " << F.n_pl;
      std::exit(Generics::code(Error::allocation));
    }
    struct Finish { // marking unset variables as fe with empty domain
      ClauseSets::DClauseSet& F0;
      Finish(ClauseSets::DClauseSet& F) noexcept : F0(F) {}
      ~Finish() noexcept {
        const ClauseSets::Dependency emptyset = F0.dep_sets.find(ClauseSets::Varset());
        assert(emptyset != F0.dep_sets.end());
        for (VarLit::Var v = 1; v <= F0.n_pl; ++v)
          if (F0.vt[v] == ClauseSets::VT::und) {
            F0.vt[v] = ClauseSets::VT::fe; ++F0.ne_d;
            F0.D[v] = emptyset;
          }
        assert(F0.n_pl == F0.na_d + F0.ne_d);
      }
    } finish(F);
    ClauseSets::AVarset A;
    ClauseSets::Dependency dep;
    try { dep = F.dep_sets.insert(A).first; }
    catch (const std::bad_alloc&) {
      errout << "Allocation error for first (empty) dependency-set.";
      std::exit(Generics::code(Error::allocation));
    }
    std::string line;
    enum class lt { begin, e, a, d }; // line type
    lt last_line = lt::begin;
    if (in.eof()) return;
    while (true) {
      const int peek = in.peek();
      if (peek == std::char_traits<char>::eof()) return;
      ++current_line_number;
      if (peek == 'c')
        if (conlev == ConformityLevel::general) {
          std::getline(in, line);
          continue;
        }
        else {
          errout << "Line" << current_line_number << "Comment after p-line.";
          std::exit(Generics::code(Error::illegal_comment));
        }
      if (peek != 'a' and peek != 'e' and peek != 'd') return;
      std::getline(in, line);
      std::stringstream s(line);
      {std::string skip; s >> skip;}
      if (peek == 'a') {
        if (conlev != ConformityLevel::general and last_line == lt::a) {
          errout << "Line" << current_line_number << "Repeated a-line.";
          std::exit(Generics::code(Error::a_rep_line));
        }
        ClauseSets::Count_t num_a = 0;
        do {
          VarLit::Var v;
          if (not (s >> v)) {
            errout << "Line" << current_line_number << "Bad a-read.";
            std::exit(Generics::code(Error::a_read));
          }
          if (v > F.n_pl) {
            errout << "Line" << current_line_number << "a-variable" << v << "contradicts n=" << F.n_pl;
            std::exit(Generics::code(Error::variable_value));
          }
          if (v == 0) break;
          if (F.vt[v] != ClauseSets::VT::und) {
            errout << "Line" << current_line_number << "Repeated a-variable-read" << v;
            std::exit(Generics::code(Error::a_rep));
          }
          F.vt[v] = ClauseSets::VT::fa;
          A.insert(v); ++num_a; ++F.na_d;
        } while (true);
        if (not s) {
          errout << "Line" << current_line_number << "Bad a-line read (file corrupted?).";
          std::exit(Generics::code(Error::a_line_read));
        }
        if (not s.eof()) {
          errout << "Line" << current_line_number << "Syntax error in a-line (trailing characters).";
          std::exit(Generics::code(Error::a_line_trail));
        }
        if (num_a != 0) {
          last_line = lt::a;
          try {
            const auto insert = F.dep_sets.insert(A);
            assert(insert.second);
            dep = insert.first;
          }
          catch (const std::bad_alloc&) {
            errout << "Line" << current_line_number << "Allocation error for dependency-set.";
            std::exit(Generics::code(Error::allocation));
          }
        } else {
          if (conlev != ConformityLevel::general) {
            errout << "Line" << current_line_number << "Empty a-line.";
            std::exit(Generics::code(Error::a_empty));
          }
        }
      } else if (peek == 'e') {
        if (conlev != ConformityLevel::general and last_line == lt::e) {
          errout << "Line" << current_line_number << "Repeated e-line.";
          std::exit(Generics::code(Error::e_rep_line));
        }
        ClauseSets::Count_t num_e = 0;
        do {
          VarLit::Var v;
          if (not (s >> v)) {
            errout << "Line" << current_line_number << "Bad e-read.";
            std::exit(Generics::code(Error::e_read));
          }
          if (v > F.n_pl) {
            errout << "Line" << current_line_number << "e-variable" << v << "contradicts n=" << F.n_pl;
            std::exit(Generics::code(Error::variable_value));
          }
          if (v == 0) break;
          if (F.vt[v] != ClauseSets::VT::und) {
            errout << "Line" << current_line_number << "Repeated e-variable-read.";
            std::exit(Generics::code(Error::e_rep));
          }
          F.vt[v] = ClauseSets::VT::fe;
          F.D[v] = dep; ++num_e; ++F.ne_d;
        } while (true);
        if (not s) {
          errout << "Line" << current_line_number << "Bad e-line read (file corrupted?).";
          std::exit(Generics::code(Error::e_line_read));
        }
        if (not s.eof()) {
          errout << "Line" << current_line_number << "Syntax error in e-line (trailing characters).";
          std::exit(Generics::code(Error::e_line_trail));
        }
        if (conlev != ConformityLevel::general and num_e == 0) {
          errout << "Line" << current_line_number << "Empty e-line.";
          std::exit(Generics::code(Error::e_empty));
        }
        last_line = lt::e;
      } else {
        assert(peek == 'd');
        VarLit::Var v;
        if (not (s >> v)) {
          errout << "Line" << current_line_number << "Bad e-variable read in d-line.";
          std::exit(Generics::code(Error::e_read_dline));
        }
        if (v > F.n_pl) {
          errout << "Line" << current_line_number << "e-variable" << v << "contradicts n=" << F.n_pl;
          std::exit(Generics::code(Error::variable_value));
        }
        if (v == 0) {
          errout << "Line" << current_line_number << "Empty d-line.";
          std::exit(Generics::code(Error::d_empty));
        }
        if (F.vt[v] != ClauseSets::VT::und) {
          errout << "Line" << current_line_number << "Repeated e-read" << v << "in d-line.";
          std::exit(Generics::code(Error::e_rep_dline));
        }
        F.vt[v] = ClauseSets::VT::fe;
        ++F.ne_d;
        ClauseSets::Varset A;
        do {
          VarLit::Var w;
          if (not (s >> w)) {
            errout << "Line" << current_line_number << "Bad a-read in d-line.";
            std::exit(Generics::code(Error::a_read_dline));
          }
          if (w > F.n_pl) {
            errout << "Line" << current_line_number << "a-variable" << w << " in d-line contradicts n=" << F.n_pl;
            std::exit(Generics::code(Error::variable_value));
          }
          if (w == 0) break;
          if (F.vt[w] != ClauseSets::VT::fa) {
            errout << "Line" << current_line_number << "Not a-variable" << w << "in d-line.";
            std::exit(Generics::code(Error::d_bada));
          }
          try {
            const auto insert = A.insert(w);
            if (not insert.second) {
              errout << "Line" << current_line_number << "Repeated a-variable-read in d-line.";
              std::exit(Generics::code(Error::a_rep_dline));
            }
          }
          catch (const std::bad_alloc&) {
            errout << "Line" << current_line_number << "Allocation error for insertion into dependency-set.";
            std::exit(Generics::code(Error::allocation));
          }
        } while (true);
        if (not s.eof()) {
          errout << "Line" << current_line_number << "Syntax error in d-line (trailing characters).";
          std::exit(Generics::code(Error::d_line_trail));
        }
        try { F.D[v] = F.dep_sets.insert(A).first; }
        catch (const std::bad_alloc&) {
          errout << "Line" << current_line_number << "Allocation error for insertion of dependency-set.";
          std::exit(Generics::code(Error::allocation));
        }
        last_line = lt::d;
      } // end of main if-then-else
    } // main loop
  }

  enum class RS { clause, none, empty, tautology, pseudoempty }; // read-status

  /* Attempting to read a single clause from in:
      - reference-parameter C is empty iff none or a tautological clause or
        an empty clause was found;
      - updating current_clause_index
      - uses F.vt, F.n_pl
      - updates F.lrep
      - aborts via std::exit in case of input-errors, with error-message on
        errout.
  */
  RS read_clause(ClauseSets::DClause& C) noexcept {
    assert(in.exceptions() == 0);
    assert(in.good());
    C.clear();
    VarLit::Lit x;
    in >> x;
    if (in.eof()) return RS::none;
    ++current_clause_index;
    ClauseSets::AClause CA; ClauseSets::EClause CE; // complemented clauses
    ClauseSets::Count_t lrep = 0;
    while (true) { // reading literals into C
      if (not in) {
        errout << "Clause" << current_clause_index << "Invalid literal-read at beginning of clause.";
        std::exit(Generics::code(Error::literal_read));
      }
      assert(in.good());
      if (not x) break; // end of clause
      const VarLit::Var v = var(x);
      if (v > F.n_pl) {
        errout << "Clause" << current_clause_index << "Literal" << x << "contradicts n=" << F.n_pl;
        std::exit(Generics::code(Error::variable_value));
      }
      if (at(F.vt[v]))
        if (CA.find(x) != CA.end()) { // tautology via universal literals
          C.clear();
          do
            if (not (in >> x)) {
              errout << "Clause" << current_clause_index << "Invalid literal-read in a-tautological clause.";
              std::exit(Generics::code(Error::literal_read));
            }
          while (x);
          return RS::tautology;
        }
        else {
          lrep += not C.P.first.insert(x).second;
          CA.insert(-x);
        }
      else {
        assert(et(F.vt[v]));
        if (CE.find(x) != CE.end()) { // tautology via existential literals
          C.clear();
          do
            if (not (in >> x)) {
              errout << "Clause" << current_clause_index << "Invalid literal-read in e-tautological clause.";
              std::exit(Generics::code(Error::literal_read));
            }
          while (x);
          return RS::tautology;
        }
        else {
          lrep += not C.P.second.insert(x).second;
          CE.insert(-x);
        }
      }
      in >> x;
    }
    F.lrep += lrep;
    switch (conlev) {
    case ConformityLevel::general :
      if (C.empty()) return RS::empty;
      if (C.pseudoempty()) return RS::pseudoempty;
      break;
    case ConformityLevel::normal :
      if (C.empty()) {
        errout << "Clause" << current_clause_index << "Empty clause.";
        std::exit(Generics::code(Error::empty_clause));
      }
      if (C.pseudoempty()) return RS::pseudoempty;
      break;
    default :
      if (C.empty()) {
        errout << "Clause" << current_clause_index << "Empty clause.";
        std::exit(Generics::code(Error::empty_clause));
      }
      if (C.pseudoempty()) {
        errout << "Clause" << current_clause_index << "Clause without existential variables.";
        std::exit(Generics::code(Error::pseudoempty_clause));
      }
    }
    return RS::clause;
  }

  /* Add non-tautological C to F (if not already existent):
      - insertions into F.F, update of F.vt
      - updates F.c, F.la, F.le, F.na, F.ne,
                F.max_a/e/c_length, F.vardeg,
                F.max_a/e_index, F.repeated
      - aborts via std::exit in case of out-of-memory.
  */
  void add_clause(const ClauseSets::DClause& C) noexcept {
    try {
      if (F.F.insert({C.P, current_clause_index}).second) {
        ++F.c;
        const VarLit::Var sa = C.P.first.size(), se = C.P.second.size();
        F.la += sa; F.le += se;
        F.max_a_length = std::max(sa, F.max_a_length);
        F.max_e_length = std::max(se, F.max_e_length);
        F.max_c_length = std::max(sa+se, F.max_c_length);
        for (const VarLit::ALit x : C.P.first) {
          const VarLit::Var v = var(x);
          ++F.vardeg[v];
          if (F.vt[v] != ClauseSets::VT::fa) continue;
          F.max_a_index = std::max(v, F.max_a_index);
          F.vt[v] = ClauseSets::VT::a; ++F.na;
        }
        for (const VarLit::ELit x : C.P.second) {
          const VarLit::Var v = var(x);
          ++F.vardeg[v];
          if (F.vt[v] != ClauseSets::VT::fe) continue;
          F.max_e_index = std::max(v, F.max_e_index);
          F.vt[v] = ClauseSets::VT::e; ++F.ne;
        }
      }
      else ++F.repeated;
    }
    catch (const std::bad_alloc&) {
      errout << "Clause" << current_clause_index << "Allocation error for insertion of clause.";
      std::exit(Generics::code(Error::allocation));
    }
  }

  /* Counting dependency-sets, and removing unused ones (not used by
     e-variables occurring in proper clauses):
      - setting F.dc, updating F.dep_sets
      - using V.max_e_index, F.vt, F.D.
  */
  void count_dependencies() noexcept {
    for (VarLit::Var v = 1; v <= F.max_e_index; ++v) {
      if (F.vt[v] != ClauseSets::VT::e) continue;
      const ClauseSets::Dependency_p dp = &*F.D[v];
      ++F.dc[dp];
    }
    for (auto i = F.dep_sets.begin(); i != F.dep_sets.end();) {
      const ClauseSets::Dependency_p dp = &*i;
      const auto find = F.dc.find(dp);
      auto j = i; ++j;
      if (find == F.dc.end()) F.dep_sets.erase(i);
      i = j;
    }
  }

  // Removing such a-variables from clauses, which aren't in a dependency of
  // some e-variable in that clause; degrade a-variables to formal a-variables
  // at the end accordingly:
  void cleanup_clauses() noexcept {

  }

  // Shrink dependencies by removing formal a-variables:
  void cleanup_dependencies() noexcept {

  }

  public :

  ReadDimacs(std::istream& in, ErrOut& errout, const ConformityLevel cl) noexcept :
    in(in), errout(errout), conlev(cl) {}

  const ClauseSets::DClauseSet& operator()() noexcept {
    read_header();
    if (in.eof()) return F;
    read_dependencies();
    if (in.eof()) return F;
    try { F.vardeg.resize(F.n_pl+1); }
    catch (const std::bad_alloc&) {
      errout << "Allocation error for degree-vector of size "<< F.n_pl << ".";
      std::exit(Generics::code(Error::allocation));
    }
    {ClauseSets::DClause C; RS status;
     while ((status = read_clause(C)) != RS::none) {
       bool add = true;
       switch (status) {
       case RS::tautology   : ++F.t; add = false; break;
       case RS::empty       : ++F.empty; break;
       case RS::pseudoempty : ++F.pempty; break;
       default : ;
       }
       if (F.c + F.repeated + F.t + ((add)?1:0) > F.c_pl) {
         errout << "Clause" << current_clause_index << "More than c=" << F.c_pl << "clauses, contradicting cnf-header.";
         std::exit(Generics::code(Error::number_clauses));
       }
       if (add) add_clause(C);
     }
    }
    F.max_index = std::max(F.max_a_index, F.max_e_index);
    F.n = F.na + F.ne;
    F.l = F.la + F.le;
    assert(F.c == F.F.size());

    count_dependencies();
    F.count_dep = F.dep_sets.size();
    for (const ClauseSets::AVarset& D : F.dep_sets) {
      const VarLit::Var size = D.size();
      F.max_s_dep = std::max(size, F.max_s_dep);
      F.min_s_dep = std::min(size, F.min_s_dep);
    }

    cleanup_clauses();
    cleanup_dependencies();

    return F;
  }

  }; // class ReadDimacs

}

#endif
