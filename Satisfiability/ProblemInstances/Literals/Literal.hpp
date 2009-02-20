// Oliver Kullmann, 26.2.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProblemInstances/Literals/Literal.hpp
  \brief Old, very simple implementation of boolean literals.
  \deprecated
*/

#ifndef WAECHTERLITERAL_jYtre34
#define WAECHTERLITERAL_jYtre34

#include <OKlib/Satisfiability/ProblemInstances/Variables/VarSet.hpp>


namespace Literals {

  using namespace Variables;

  class Lit {

  public :

    Lit() {};
    Lit(Var, bool);

    bool val() const;
    Var var() const;

    bool operator == (Lit) const;
    bool operator < (Lit) const;

    Lit comp() const;

  private :

    Var v;
    bool va;
    
  };

  inline Lit::Lit(Var a, bool b) {
    v = a; va = b;
  }

  inline bool Lit::val() const {
    return va;
  }

  inline Var Lit::var() const {
    return v;
  }

  inline bool Lit::operator == (Lit x) const {
    return v == x.v && va == x.va;
  }

  inline bool Lit::operator < (Lit x) const {
    return (v < x.v) || (v == x.v && va < x.va);
  }

  inline Lit Lit::comp() const {
    return Lit(v, ! va);
  }

}

#endif
