// Oliver Kullmann, 27.2.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
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
  using namespace Clauses;

  class Cls {

  public :

    Cls () {}
    Cls (const std::set<Cl>&);

    Cls& read(Var_Set&); // input from standard input in 
    // "generalised DIMACS format";
    // arbitrary (non-empty) strings are allowed as names except 
    // of "p" and "c"
    

    std::set<Var> var() const;
    Litset lit() const;

    bool is_empty() const;
    bool empty_clause() const;

    set_length n() const;
    set_length c() const;
    set_length c(set_length) const;
    unsigned long int l() const;
    set_length pmin() const;
    set_length pmax() const;

    Cls& add(const Cl&);
    Cls& remove(const Cl&);

    bool operator == (const Cls&) const;

    const Cls operator + (const Cls&) const;
    const Cls operator - (const Cls&) const;
    const Cls operator & (const Cls&) const;

    std::set<Cl> cls;
  };

  inline Cls::Cls(const std::set<Cl>& F) {
    cls = F;
  }

  inline Cls& Cls::read(Var_Set& V) {
    Litset C;
    std::string s;
    char c;

    while (std::cin >> s) {
      if (s == "c" || s == "p") {
        while (std::cin.get(c))
          if (c == '\n')
            break;
        if (! std::cin)
          break;
        else
          continue;
      }
      else if (s == "0") {
        if (! C.is_tautological())
          cls.insert(C);
        C = Litset();
      }
      else if (s[0] == '-')
        C.add(Lit(V.get_var(s.substr(1)), true));
      else
        C.add(Lit(V.get_var(s), false));
    }
    return *this;
  }

  inline const Litset& acc_plus(Litset& A, const Cl& B) {
    return A.add_ls((B));
  }

  inline Litset Cls::lit() const {
    return accumulate(cls.begin(), cls.end(), Litset(), acc_plus);
  }

  inline std::set<Var> Cls::var() const {
    return this -> lit().var();
  }

  inline bool Cls::is_empty() const {
    return cls.empty();
  }

  inline bool Cls::empty_clause() const {
    if (cls.empty())
      return false;
    return cls.begin() -> is_empty();
    // setzt voraus, dass die leere Klausel ganz vorne in cls abgelegt wird
  }

  inline set_length Cls::n() const {
    return this -> var().size();
  }
  
  inline set_length Cls::c() const {
    return cls.size();
  }

  inline bool has_length(const Cl& C, set_length k = 0) {
    static set_length kfixed;
    if (k != 0) {
      kfixed = k;
      return false;
    }
    else
      return C.length() == kfixed;
  }

  inline set_length Cls::c(set_length k) const {
    has_length(Cl(), k);
    return count_if(cls.begin(), cls.end(), has_length);
  }

  inline set_length length_plus(const set_length a, const Cl& B) {
    return a + B.length();
  }
  
  inline unsigned long int Cls::l() const {
    return accumulate(cls.begin(), cls.end(), 0, length_plus);
  }

  inline set_length Cls::pmin() const {
    set_length minl = max_length;
    for (std::set<Cl>::iterator i = cls.begin(); i != cls.end(); i++)
      minl = std::min(minl, i -> length());
    return minl;
  }

  inline set_length Cls::pmax() const {
    set_length maxl = 0;
    for (std::set<Cl>::iterator i = cls.begin(); i != cls.end(); i++)
      maxl = std::max(maxl, i -> length());
    return maxl;
  }

  inline Cls& Cls::add(const Cl& C) {
    cls.insert(C);
    return *this;
  }

  inline Cls& Cls::remove(const Cl& C) {
    cls.erase(C);
    return *this;
  }

  inline bool Cls::operator == (const Cls& F) const {
    return F.cls == cls;
  }

  inline const Cls Cls::operator + (const Cls& F) const {
    std::set<Cl> result;
    std::insert_iterator< std::set<Cl> > res_ins(result, result.begin());
    std::set_union(cls.begin(), cls.end(), F.cls.begin(), F.cls.end(), res_ins);
    return Cls(result);
  }

  inline const Cls Cls::operator - (const Cls& F) const {
    std::set<Cl> result;
    std::insert_iterator< std::set<Cl> > res_ins(result, result.begin());
    std::set_difference(cls.begin(), cls.end(), F.cls.begin(), F.cls.end(), res_ins);
    return result;
  }

  inline const Cls Cls::operator & (const Cls& F) const {
    std::set<Cl> result;
    std::insert_iterator< std::set<Cl> > res_ins(result, result.begin());
    std::set_intersection(cls.begin(), cls.end(), F.cls.begin(), F.cls.end(), res_ins);
    return result;
  }


}

#endif
