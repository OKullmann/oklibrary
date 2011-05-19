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

  typedef size_t size_type; // should by "size_type"
  const size_type max_length = std::numeric_limits<size_type>::max();

  inline const std::set<Variables::Var> operator +(const std::set<Variables::Var>& A, const std::set<Variables::Var>& B) {
    std::set<Variables::Var> result;
    std::insert_iterator<std::set<Variables::Var> > res_ins(result, result.begin());
    std::set_union(A.begin(), A.end(), B.begin(), B.end(), res_ins);
    return result;
  }

  
  class Litset {
  public :
    typedef Literals::Lit literal_type;
    typedef std::set<literal_type> litset_t;
  public :
    litset_t ls; // should be protected
  public :
    typedef Variables::Var var_type;
    typedef litset_t::const_iterator iterator;
    typedef iterator const_iterator;
    typedef std::set<var_type> varset_type;

    Litset() {}
    Litset (const litset_t& L) : ls(L) {}

    // uses the special order of literals
    bool tautological() const {
      iterator a0 = ls.begin(), a = a0;
      const iterator ls_end = ls.end();
      if (a0 == ls_end) return false;
      ++a;
      while (a != ls.end()) {
        if (a->comp() == *a0) return true;
        ++a; ++a0;
      }
      return false;
    }

    bool empty() const { return ls.empty(); }
    size_type size() const { return ls.size(); }
    bool contains(const literal_type x) const {
      return ! (ls.find(x) == ls.end());
    }

    static literal_type comp_(const literal_type x) { return x.comp(); }
    Litset comp() const {
      litset_t result;
      std::insert_iterator<litset_t> res_ins(result, result.begin());
      transform(ls.begin(), ls.end(), res_ins, comp_);
      return result;
    }

    static var_type var_(const literal_type x) { return x.var(); }
    varset_type var() const {
      varset_type result;
      std::insert_iterator<varset_type> res_ins(result, result.begin());
      transform(ls.begin(), ls.end(), res_ins, var_);
      return result;
    }

    Litset& add(const literal_type x) { ls.insert(x); return *this; }
    Litset& remove(const literal_type x) { ls.erase(x); return *this; }
    Litset& add_ls(const Litset& L) {
      ls.insert(L.ls.begin(), L.ls.end());
      return *this;
    }
    Litset& remove_ls(const Litset& L) {
      ls.erase(L.ls.begin(), L.ls.end());
      return *this;
    }

    friend bool operator ==(const Litset& lhs, const Litset& rhs) {
      return lhs.ls == rhs.ls;
    }
    friend bool operator <(const Litset& lhs, const Litset& rhs) {
      return lhs.ls < rhs.ls;
    }

    friend Litset operator +(const Litset& lhs, const Litset& rhs) {
      litset_t result;
      std::insert_iterator<litset_t> res_ins(result, result.begin());
      std::set_union(lhs.ls.begin(), lhs.ls.end(), rhs.ls.begin(), rhs.ls.end(), res_ins);
      return result;
    }
    friend Litset operator -(const Litset& lhs, const Litset& rhs) {
      litset_t result;
      std::insert_iterator<litset_t> res_ins(result, result.begin());
      std::set_difference(lhs.ls.begin(), lhs.ls.end(), rhs.ls.begin(), rhs.ls.end(), res_ins);
      return result;
    }
    friend Litset operator &(const Litset& lhs, const Litset& rhs) {
      litset_t result;
      std::insert_iterator<litset_t> res_ins(result, result.begin());
      std::set_intersection(lhs.ls.begin(), lhs.ls.end(), rhs.ls.begin(), rhs.ls.end(), res_ins);
      return result;
    }
  };


  namespace Error {
    struct not_a_clause {};
  }

  struct Cl : Litset {
    Cl() {}
    Cl(const litset_t& L) : Litset(L) {
      if (this->tautological()) throw Error::not_a_clause();
    }
    Cl(const Litset& L) : Litset(L) {
      if (this->tautological()) throw Error::not_a_clause();
    }

    Cl& add(const literal_type x) {
      if (this->contains(x.comp())) throw Error::not_a_clause();
      this -> add(x);
      return *this;
    }
    friend Cl operator +(const Cl& lhs, const Cl& rhs) {
      Cl D = Litset(lhs) + Litset(rhs);
      if (D.tautological()) throw Error::not_a_clause();
      return D;
    }

  };




}

#endif
