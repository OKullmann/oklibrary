// Oliver Kullmann, 1.3.2002 (Swansea)
/* Copyright 2002 - 2007, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Assignments/PartialAssignments/PartAssign.hpp
  \brief Old, very simple implementation of partial boolean assignments.
  \deprecated
*/

#ifndef WAECHTERPARTASSIGN_9Yfvce67
#define WAECHTERPARTASSIGN_9Yfvce67

#include <map>

#include <OKlib/Satisfiability/ProblemInstances/Variables/VarSet.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/Literal.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Clauses/Clause.hpp>
#include <OKlib/Satisfiability/ProblemInstances/ClauseSets/ClauseSet.hpp>

namespace PartAssignments {

  namespace Error {
    struct not_in_domain {};
    struct inconsistent_extension {};
  }

  class Pass {
  public :
    typedef Variables::Var var_type;
  private :
    typedef std::map<var_type, bool> map_type;
    typedef map_type::const_iterator iterator;
    map_type pa;
  public :
    typedef Literals::Lit literal_type;
    typedef map_type::size_type size_type;
    typedef std::set<var_type> varset_type;
    typedef Clauses::Cl clause_type;
    typedef Clausesets::Cls clauseset_type;

    Pass() {}
    Pass(const literal_type x) : pa(map_type()) { pa[x.var()] = x.val(); }
    Pass (const clause_type& C) : pa(map_type()) {
      typedef clause_type::litset_t::const_iterator literator;
      const literator C_end = C.ls.end();
      for (literator i = C.ls.begin(); i != C_end; ++i)
        pa[i->var()] = i->val();
    }

    friend bool operator ==(const Pass& lhs, const Pass& rhs) {
      return lhs.pa == rhs.pa;
    }
    bool empty() const { return pa.empty(); }
    bool in_domain(const var_type v) const { return pa.find(v) != pa.end(); }

    size_type size() const  { return pa.size(); }
    varset_type var() const {
      varset_type V;
      const iterator pa_end = pa.end();
      for (iterator i = pa.begin(); i != pa_end; ++i) V.insert(i->first);
      return V;
    }

    bool val(const var_type v) const {
      const iterator p = pa.find(v);
      if (p == pa.end()) throw Error::not_in_domain();
      return p -> second;
    }

    //! conversion to clause
    clause_type clause() const {
      clause_type C;
      const iterator pa_end = pa.end();
      for (iterator i = pa.begin(); i != pa_end; ++i)
        C.ls.insert(literal_type(i->first, i->second));
      return C;
    }
    //! applying the partial assignment to a clause-set
    clauseset_type operator *(const clauseset_type& F) const {
      clauseset_type G;
      typedef clauseset_type::const_iterator citerator;
      const citerator F_end = F.clauseset().end();
      for (citerator i = F.clauseset().begin(); i != F_end; ++i)
        if ((*i & this->clause().comp()).empty()) G.add(*i - this->clause());
      return G;
    }

    //! computing (phi composition this)
    Pass& composition_left(const Pass& phi) {
      pa.insert(phi.pa.begin(), phi.pa.end());
      return *this;
    }
    //! remove assignments for variables in V
    Pass& restrict(const varset_type& V) {
      typedef varset_type::const_iterator viterator;
      const viterator V_end = V.end();
      for (viterator i = V.begin(); i != V_end; ++i) pa.erase(*i);
      return *this;
    }
  };


}

#endif
