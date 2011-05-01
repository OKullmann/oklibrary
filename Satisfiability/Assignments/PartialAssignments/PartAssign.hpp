// Oliver Kullmann, 1.3.2002 (Swansea)
/* Copyright 2002 - 2007, 2009 Oliver Kullmann
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

  using namespace Variables;
  using namespace Literals;
  using namespace Clauses;
  using namespace Clausesets;

  namespace Error {
    struct not_in_domain {};
    struct inconsistent_extension {};
  }

  class Pass {
    typedef std::map<Var, bool> map_type;
    map_type pa;
  public :
    typedef map_type::size_type size_type;
    typedef Var var_type;
    typedef Lit literal_type;
    typedef std::set<Var> varset_type;
    typedef Cl clause_type;
    typedef Cls clauseset_type;
    Pass () {};
    Pass (Lit);
    Pass (const Cl&);
    bool empty() const;
    size_type size() const;
    std::set<Var> var() const;
    bool in_domain(Var) const;
    bool val(Var) const;
    Pass& composition_left(const Pass&);
    // Attention: the argument is composed from the *left*
    Pass& restrict(const std::set<Var>&);
    Cl clause() const;
    Cls operator* (const Cls&) const;
  };

  inline Pass::Pass(const Lit x) {
    pa = Pass::map_type();
    pa[x.var()] = x.val();
  }
  inline Pass::Pass (const Cl& C) {
    pa = Pass::map_type();
    for (std::set<Lit>::const_iterator i = C.ls.begin(); i != C.ls.end(); ++i)
      pa[i -> var()] = i -> val();
  }
  inline bool Pass::empty() const { return pa.empty(); }
  inline std::set<Var> Pass::var() const {
    std::set<Var> V;
    for (std::map<Var,bool>::const_iterator i = pa.begin(); i != pa.end(); ++i)
      V.insert(i -> first);
    return V;
  }
  inline Pass::size_type Pass::size() const { return pa.size(); }
  inline bool Pass::in_domain(const Var v) const {
    return pa.find(v) != pa.end();
  }
  inline bool Pass::val(const Var v) const {
    std::map<Var,bool>::const_iterator p = pa.find(v);
    if (p == pa.end()) throw Error::not_in_domain();
    return p -> second;
  }
  inline Pass& Pass::composition_left(const Pass& phi) {
    pa.insert(phi.pa.begin(), phi.pa.end());
    return *this;
  }
  inline Pass& Pass::restrict(const std::set<Var>& V) {
    for (std::set<Var>::const_iterator i = V.begin(); i != V.end(); ++i)
      pa.erase(*i);
    return *this;
  }
  inline Cl Pass::clause() const {
    Cl C;
    for (std::map<Var,bool>::const_iterator i = pa.begin(); i != pa.end(); ++i)
      C.ls.insert(Lit(i -> first, i -> second));
    return C;
  }
  inline Cls Pass::operator *(const Cls& F) const {
    Cls G;
    for (std::set<Cl>::iterator i = F.clauseset().begin(); i != F.clauseset().end(); ++i)
      if ((*i & this->clause().comp()).empty()) G.add(*i - this->clause());
    return G;
  }

}

#endif
