// Oliver Kullmann, 27.2.2002 (Swansea)
/* Copyright 2002 - 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProblemInstances/ClauseSets/ClauseSet.hpp
  \brief Old, very simple implementation of boolean clause-sets.
  \deprecated
*/

#ifndef WAECHTERCLAUSESET_hbew90
#define WAECHTERCLAUSESET_hbew90

#include <set>
#include <numeric>
#include <iostream>
#include <iterator>
#include <algorithm>

#include <OKlib/Satisfiability/ProblemInstances/Variables/VarSet.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/Literal.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Clauses/Clause.hpp>

namespace Clausesets {

  using namespace Variables;
  using namespace Literals;

  class Cls {
    std::set<Clauses::Cl> cls;
  public :
    Cls () {}
    Cls (const std::set<Clauses::Cl>&);
    Cls& read(Var_Set&); // input from standard input in 
    // "generalised DIMACS format";
    // arbitrary (non-empty) strings are allowed as names except 
    // of "p" and "c"
    std::set<Var> var() const;
    Clauses::Litset lit() const;
    bool is_empty() const;
    bool empty_clause() const;
    Clauses::size_type n() const;
    Clauses::size_type c() const;
    Clauses::size_type c(Clauses::size_type) const;
    unsigned long int l() const;
    Clauses::size_type pmin() const;
    Clauses::size_type pmax() const;
    Cls& add(const Clauses::Cl&);
    Cls& remove(const Clauses::Cl&);
    bool operator ==(const Cls&) const;
    const Cls operator +(const Cls&) const;
    const Cls operator -(const Cls&) const;
    const Cls operator &(const Cls&) const;
    const std::set<Clauses::Cl>& clauseset() const { return cls; }
  };

  inline Cls::Cls(const std::set<Clauses::Cl>& F) { cls = F; }

  inline Cls& Cls::read(Var_Set& V) {
    Clauses::Litset C;
    std::string s;
    char c;

    while (std::cin >> s) {
      if (s == "c" || s == "p") {
        while (std::cin.get(c)) if (c == '\n') break;
        if (! std::cin) break;
        else continue;
      }
      else if (s == "0") {
        if (! C.tautological()) cls.insert(C);
        C = Clauses::Litset();
      }
      else if (s[0] == '-')
        C.add(Lit(V.get_var(s.substr(1)), true));
      else
        C.add(Lit(V.get_var(s), false));
    }
    return *this;
  }

  inline const Clauses::Litset& acc_plus(Clauses::Litset& A, const Clauses::Cl& B) {
    return A.add_ls((B));
  }
  inline Clauses::Litset Cls::lit() const {
    return accumulate(cls.begin(), cls.end(), Clauses::Litset(), acc_plus);
  }
  inline std::set<Var> Cls::var() const { return this -> lit().var(); }
  inline bool Cls::is_empty() const { return cls.empty(); }
  inline bool Cls::empty_clause() const {
    if (cls.empty()) return false;
    return cls.begin() -> empty();
    // setzt voraus, dass die leere Klausel ganz vorne in cls abgelegt wird
  }
  inline Clauses::size_type Cls::n() const { return this -> var().size(); }
  inline Clauses::size_type Cls::c() const { return cls.size(); }
  inline bool has_length(const Clauses::Cl& C, Clauses::size_type k = 0) {
    static Clauses::size_type kfixed;
    if (k != 0) {
      kfixed = k;
      return false;
    }
    else
      return C.size() == kfixed;
  }
  inline Clauses::size_type Cls::c(const Clauses::size_type k) const {
    has_length(Clauses::Cl(), k);
    return count_if(cls.begin(), cls.end(), has_length);
  }
  inline Clauses::size_type length_plus(const Clauses::size_type a, const Clauses::Cl& B) {
    return a + B.size();
  }
  inline unsigned long int Cls::l() const {
    return accumulate(cls.begin(), cls.end(), 0, length_plus);
  }
  inline Clauses::size_type Cls::pmin() const {
    Clauses::size_type minl = Clauses::max_length;
    for (std::set<Clauses::Cl>::iterator i = cls.begin(); i != cls.end(); ++i)
      minl = std::min(minl, i->size());
    return minl;
  }
  inline Clauses::size_type Cls::pmax() const {
    Clauses::size_type maxl = 0;
    for (std::set<Clauses::Cl>::iterator i = cls.begin(); i != cls.end(); ++i)
      maxl = std::max(maxl, i->size());
    return maxl;
  }
  inline Cls& Cls::add(const Clauses::Cl& C) {
    cls.insert(C);
    return *this;
  }
  inline Cls& Cls::remove(const Clauses::Cl& C) {
    cls.erase(C);
    return *this;
  }
  inline bool Cls::operator ==(const Cls& F) const { return F.cls == cls; }
  inline const Cls Cls::operator +(const Cls& F) const {
    std::set<Clauses::Cl> result;
    std::insert_iterator< std::set<Clauses::Cl> > res_ins(result, result.begin());
    std::set_union(cls.begin(), cls.end(), F.cls.begin(), F.cls.end(), res_ins);
    return Cls(result);
  }
  inline const Cls Cls::operator -(const Cls& F) const {
    std::set<Clauses::Cl> result;
    std::insert_iterator< std::set<Clauses::Cl> > res_ins(result, result.begin());
    std::set_difference(cls.begin(), cls.end(), F.cls.begin(), F.cls.end(), res_ins);
    return result;
  }
  inline const Cls Cls::operator & (const Cls& F) const {
    std::set<Clauses::Cl> result;
    std::insert_iterator< std::set<Clauses::Cl> > res_ins(result, result.begin());
    std::set_intersection(cls.begin(), cls.end(), F.cls.begin(), F.cls.end(), res_ins);
    return result;
  }

}

#endif
