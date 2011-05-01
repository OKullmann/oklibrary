// Oliver Kullmann, 27.2.2002 (Swansea)
/* Copyright 2002 - 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProblemInstances/Clauses/Clause.hpp
  \brief Old, very simple implementation of boolean clauses.
  \deprecated
*/

#ifndef WAECHTERCLAUSE_5454Dsw
#define WAECHTERCLAUSE_5454Dsw


#include <set>
#include <algorithm>
#include <limits>
#include <iterator>

#include <OKlib/Satisfiability/ProblemInstances/Variables/VarSet.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/Literal.hpp>

namespace Clauses {

  using namespace Variables;
  using namespace Literals;

  typedef size_t size_type; // should by "size_type"
  const size_type max_length = std::numeric_limits<size_type>::max();

  inline const std::set<Var> operator +(const std::set<Var>& A, const std::set<Var>& B) {
    std::set<Var> result;
    std::insert_iterator< std::set<Var> > res_ins(result, result.begin());
    std::set_union(A.begin(), A.end(), B.begin(), B.end(), res_ins);
    return result;
  }

  
  class Litset {
  public :
    std::set<Lit> ls; // should be private
  public :
    Litset () {}
    Litset (const std::set<Lit>&);
    bool tautological() const;
    bool empty() const;
    size_type size() const;
    bool contains(Lit) const;
    Litset comp() const;
    std::set<Var> var() const;
    Litset& add(Lit);
    Litset& remove(Lit);
    Litset& add_ls(const Litset&);
    Litset& remove_ls(const Litset&);
    bool operator == (const Litset&) const;
    bool operator < (const Litset&) const;
    const Litset operator + (const Litset&) const;
    const Litset operator - (const Litset&) const;
    const Litset operator & (const Litset&) const;
  };

  inline Litset::Litset(const std::set<Lit>& L) { ls = std::set<Lit>(L); }
  inline bool Litset::tautological() const {
    // uses the special order of literals
    std::set<Lit>::iterator a0 = ls.begin(), a = a0;
    if (a0 == ls.end()) return false;
    ++a;
    while (a != ls.end()) {
      if (a -> comp() == *a0) return true;
      ++a; ++a0;
    }
    return false;
  }
  inline bool Litset::empty() const { return ls.empty(); }
  inline size_type Litset::size() const { return ls.size(); }
  inline bool Litset::contains(const Lit x) const {
    return ! (ls.find(x) == ls.end());
  }
  inline Lit comp(const Lit x) {
    return x.comp();
  }
  inline Litset Litset::comp() const {
    std::set<Lit> result;
    std::insert_iterator< std::set<Lit> > res_ins(result, result.begin());
    transform(ls.begin(), ls.end(), res_ins, Clauses::comp);
    return Litset(result);
  }
  inline Var var(const Lit x) { return x.var(); }
  inline std::set<Var> Litset::var() const {
    std::set<Var> result;
    std::insert_iterator< std::set<Var> > res_ins(result, result.begin());
    transform(ls.begin(), ls.end(), res_ins, Clauses::var);
    return result;
  }
  inline Litset& Litset::add(const Lit x) {
    ls.insert(x);
    return *this;
  }
  inline Litset& Litset::remove(const Lit x) {
    ls.erase(x);
    return *this;
  }
  inline Litset& Litset::add_ls(const Litset& L) {
    ls.insert(L.ls.begin(), L.ls.end());
    return *this;
  }
  inline Litset& Litset::remove_ls(const Litset& L) {
    ls.erase(L.ls.begin(), L.ls.end());
    return *this;
  }
  inline bool Litset::operator ==(const Litset& L) const { return ls == L.ls; }
  inline bool Litset::operator <(const Litset& L) const { return ls < L.ls; }
  inline const Litset Litset::operator + (const Litset& L) const {
    std::set<Lit> result;
    std::insert_iterator< std::set<Lit> > res_ins(result, result.begin());
    std::set_union(ls.begin(), ls.end(), L.ls.begin(), L.ls.end(), res_ins);
    return Litset(result);
  }
  inline const Litset Litset::operator -(const Litset& L) const {
    std::set<Lit> result;
    std::insert_iterator< std::set<Lit> > res_ins(result, result.begin());
    std::set_difference(ls.begin(), ls.end(), L.ls.begin(), L.ls.end(), res_ins);
    return Litset(result);
  }
  inline const Litset Litset::operator &(const Litset& L) const {
    std::set<Lit> result;
    std::insert_iterator< std::set<Lit> > res_ins(result, result.begin());
    std::set_intersection(ls.begin(), ls.end(), L.ls.begin(), L.ls.end(), res_ins);
    return Litset(result);
  }


  namespace Error {
    struct not_a_clause {};
  }


  struct Cl : Litset {
    // for objects x of type Cl we must have ! x.tautological()
    Cl () {};
    Cl (const std::set<Lit>&);
    Cl (const Litset&);
    Cl& add(Lit);
    const Cl operator + (const Cl&) const;
  };

  inline Cl::Cl(const std::set<Lit>& L)
    : Litset(L) {
    if (this -> Litset::tautological()) throw Error::not_a_clause();
  }

  inline Cl::Cl(const Litset& L)
    : Litset(L) {
    if (this -> Litset::tautological()) throw Error::not_a_clause();
  }

  inline Cl& Cl::add(const Lit x) {
    if (this -> contains(x.comp())) throw Error::not_a_clause();
    this -> Litset::add(x);
    return *this;
  }

  inline const Cl Cl::operator +(const Cl& C) const {
    Cl D = Litset(*this) + Litset(C);
    if (D.tautological()) throw Error::not_a_clause();
    return D;
  }

}

#endif
