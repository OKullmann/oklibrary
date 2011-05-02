// Oliver Kullmann, 27.7.2003 (Swansea)
/* Copyright 2003 - 2007, 2008, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/Experimental/AllgKlassen200203/PartialAssignments.hpp
  \brief Models of partial assignments
  \deprecated To be moved (if appropriate) to OKlib.
*/

#ifndef PARTIALASSIGNMENTSWAECHTER
#define PARTIALASSIGNMENTSWAECHTER

#include <map>
#include <utility>

#include <OKlib/OKsolver/Experimental/AllgKlassen200203/ConceptDefinitions.hpp>

namespace PartialAssignments {

  /*!
    \class PassViaMap
    \brief PassViaMap<Var, Dom> yields partial assignments, implemented via std::maps.
  */

  template <class Variables, class Domain>
  class PassViaMap {

    BOOST_CLASS_REQUIRE(Variables, ConceptDefinitions, Variable_concept);
    BOOST_CLASS_REQUIRE(Domain, ConceptDefinitions, FiniteDomain_concept);

  public :

    typedef ConceptDefinitions::BasicPartialAssignment_tag Concept;

    typedef Variables Var;
    typedef Domain Dom;

  private :

    typedef typename Dom::iterator val_iterator;
    typedef std::map<Var, val_iterator> Pass_map;
    typedef typename Pass_map::const_iterator const_map_iterator;
    typedef typename Pass_map::iterator map_iterator;

    Pass_map pass;

  public :

    val_iterator operator[] (Var v) const {
      assert(not v.null());
      const const_map_iterator i = pass.find(v);
      if (i == pass.end()) return Dom::end();
      else return i -> second;
    }
    val_iterator& operator[] (Var v) {
      assert(not v.null());
      return pass[v];
    }

    void clear() { pass.clear(); }

  };

  /*!
    \class PassAsMaps
    \brief PassAsMaps<Literals> yields partial assignments, implemented via std::maps.
    \deprecated Replaced by PartialAssignments::PassViaMap.
  */

  template <class Literals>
  class PassAsMaps {
  public :
    typedef Literals Lit;
    typedef typename Lit::Var Var;
    typedef typename Lit::Val Val;

  private :
    typedef std::map<Var, Val> map_var_val;
    map_var_val pa;

  public :
    typedef typename map_var_val::iterator iterator;
    typedef typename map_var_val::const_iterator const_iterator;
    typedef typename map_var_val::size_type size_type;

    PassAsMaps() {}
    PassAsMaps (Lit x) { 
      pa.insert(std::make_pair(x.var(), x.val()));
    }
    template <typename Iterator>
    PassAsMaps(Iterator begin, Iterator end) : pa(begin, end) {}

    iterator begin() { return pa.begin(); }
    const_iterator begin() const { return pa.begin(); }
    iterator end() { return pa.end(); }
    const_iterator end() const { return pa.end(); }

    bool empty() const { return pa.empty() ;}
    size_type size() const { return pa.size(); }

    iterator find(Var v) {
      return pa.find(v);
    }
    const_iterator find(Var v) const {
      return pa.find(v);
    }
    Val& operator [] (Var v) {
      // Assumption: v is in the domain!
      return find(v) -> second;
    }
    Auxiliary::Pass_evaluation operator []  (Lit x) const {
      const_iterator i = find(x.var());
      if (i == pa.end())
	return Auxiliary::undefined;
      else
	if (i -> second == x.val())
	  return Auxiliary::falsified;
	else
	  return Auxiliary::satisfied;
    }

    bool operator ==(const PassAsMaps& phi) {
      return pa == phi.pa;
    }
    bool operator !=(const PassAsMaps& phi) {
      return not (*this == phi);
    }

  };

}

#include "Literals.hpp"

namespace PartialAssignments {

  typedef PassAsMaps<Literals::LitIntOccString> PassAsMaps_LitIntOccString;
}

#endif
