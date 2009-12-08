// Oliver Kullmann, 27.7.2003 (Swansea)
/* Copyright 2003 - 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/OKsolver/Experimental/AllgKlassen200203/Clauses.cpp
  \brief Experimental code for clauses
  \deprecated Old code
*/

#ifndef CLAUSESWAECHTER_anCq9iy
#define CLAUSESWAECHTER_anCq9iy

#include <set>
#include <sstream>

#include <boost/concept_check.hpp>

#include <OKlib/General/ErrorHandling.hpp>

#include <OKlib/OKsolver/Experimental/AllgKlassen200203/ConceptDefinitions.hpp>

namespace Clauses {

  struct Error_Clauses : ErrorHandling::Error {
    Error_Clauses(const std::string& what) : ErrorHandling::Error(what) {}
  };
  
  template <class Literal, class Comp = std::less<Literal> >
  class ClAsSets {

    // BOOST_CLASS_REQUIRE(Literal, ConceptDefinitions, GeneralLiteral_concept);
    BOOST_CLASS_REQUIRE3(Comp, Literal, Literal, boost, BinaryPredicateConcept);
  
    typedef std::set<Literal, Comp> set_of_literals;
    set_of_literals sl;

  public :
    
    typedef Literal Lit;
    typedef typename set_of_literals::size_type size_type;
    typedef typename set_of_literals::const_iterator const_iterator;
    typedef Comp literal_compare;
    typedef typename Lit::Var Var;

    struct Tautological_clause : Error_Clauses {
      Tautological_clause(const std::string& what) : Error_Clauses(what) {}
    };

    ClAsSets() {}
    ClAsSets(const ClAsSets& C) : sl(C.sl) {}
    ClAsSets& operator =(const ClAsSets& C) { sl = C.sl; }

    size_type size() const { return sl.size(); }
    bool empty() const { return sl.empty(); }
    void clear() { sl.clear(); }

    const_iterator find(Lit x) const { return sl.find(x); }

    enum Options {checked_ignore, checked_throw, unchecked};
    ClAsSets& insert(const Lit x, const Options o = checked_throw) {
      if (o != unchecked) {
	if (sl.count(x + Values::neg) == 1)
	  switch (o) {
	    checked_ignore : return *this;
	    checked_throw : {
	      std::ostringstream message("ClAsSets::insert : Complement of literal ");
	      message << x << " already present.";
	      throw Tautological_clause(message.str());
	    }
	  }
      }
      sl.insert(x);
      return *this;
    }

    ClAsSets& erase(Lit x) {
      sl.erase(x);
      return *this;
    }
    ClAsSets& erase(const_iterator i) {
      sl.erase(i);
      return *this;
    }

    const_iterator begin() const { return sl.begin(); }
    const_iterator end() const { return sl.end(); }

    bool operator == (const ClAsSets& C) const { return sl == C.sl ;}
    bool operator < (const ClAsSets& C) const {
      return sl.size() < C.sl.size() or (sl.size() == C.sl.size() and sl < C.sl); }

  };
    
}

#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Literals.hpp>

namespace Clauses {

  typedef ClAsSets<Literals::LitIntOccString> ClAsSets_LitIntOccString;
}
#endif



