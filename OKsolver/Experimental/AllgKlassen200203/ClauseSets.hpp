// Oliver Kullmann, 27.7.2003 (Swansea)
/* Copyright 2003 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef CLAUSESETSWAECHTER

#define CLAUSESETSWAECHTER

#include <set>
#include <functional>

#include <Transitional/OKsolver/Experimental/AllgKlassen200203/Auxiliary.hpp>

namespace ClauseSets {

  template <class Clauses, class Comp = std::less<Clauses> >
  class ClsAsSets {

    // Concept: Constraint_Collection

    typedef typename std::set<Clauses, Comp> set_of_clauses;
    set_of_clauses sc;

  public:

    typedef Clauses Cl;
    typedef typename set_of_clauses::size_type size_type;
    typedef typename set_of_clauses::const_iterator const_iterator;
    typedef Comp clause_compare;

    typedef typename Cl::Lit Lit;
    typedef typename Cl::Var Var;
    
    ClsAsSets() {}
    ClsAsSets(const ClsAsSets& F) : sc(F.sc) {}
    ClsAsSets& operator =(const ClsAsSets& F) { sc = F.sc; }
    
    size_type size() const { return sc.size(); }
    bool empty() const { return sc.empty(); }
    Auxiliary::Sat_status unsat() const {
      // Assumption: For clauses C1, C2 we have
      // C1.size() < C2.size() -> C1 < C2.
      if (! sc.empty() and sc.begin() -> empty())
	return Auxiliary::unsatisfiable;
      else
	return Auxiliary::unknown;
    }
    Auxiliary::Sat_status sat() const {
      if (sc.empty())
	return Auxiliary::satisfiable;
      else
	return Auxiliary::unknown;
    }

    ClsAsSets& insert(const Cl& C) {
      sc.insert(C);
      return *this;
    }

    ClsAsSets& erase(const Cl& C) {
      sc.erase(C);
      return *this;
    }
    ClsAsSets& erase(const_iterator Ci) {
      sc.erase(Ci);
      return *this;
    }

    const_iterator begin() const { return sc.begin(); }
    const_iterator end() const { return sc.end(); }

    bool operator == (const ClsAsSets& F) const { return sc == F.sc; }
    bool operator != (const ClsAsSets& F) const { return not (*this == F); }
    bool operator < (const ClsAsSets& F) const { return sc < F.sc; }

    template <typename Pass>
    ClsAsSets& apply(const Pass& pa) {
      for (const_iterator Ci = begin(); Ci != end();) {
	bool clause_changed = false;
	Cl C(*Ci);
	for (typename Cl::const_iterator xi = C.begin(); xi != C.end();) {
	  switch (pa[*xi]) {
	  case Auxiliary::satisfied : erase(Ci++); goto skip_C;
	  case Auxiliary::falsified :
	    C.erase(xi++); clause_changed = true; goto skip_x;
	  }
	  ++xi;
	  skip_x : ;
	}
	if (clause_changed) {
	  erase(Ci++); insert(C); goto skip_C;
	  // here the above order assumption is crucial!
	}
	else
	  ++Ci;
	skip_C : ;
      }
    }
  };

}
  
#include "Clauses.hpp"

namespace ClauseSets {

  typedef ClsAsSets<Clauses::ClAsSets_LitIntOccString> ClsAsSets_ClAsSets_LitIntOccString;
}

#endif
