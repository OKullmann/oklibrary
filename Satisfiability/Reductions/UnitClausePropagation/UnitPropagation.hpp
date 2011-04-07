// Oliver Kullmann, 7.3.2003 (Swansea)
/* Copyright 2003 - 2007, 2008, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Reductions/UnitClausePropagation/UnitPropagation.hpp
  \brief Experimental code for unit-propagation (boolean clause-sets)
  \deprecated Old code, needs a full update and redesign. Has also never been
  put into action.

  \todo Update namespaces
*/

#ifndef UNITPROPAGATIONWAECHTER_93Rt3
#define UNITPROPAGATIONWAECHTER_93Rt3

#include <algorithm>
#include <functional>

#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Auxiliary.hpp>

namespace UnitPropagation {

  /*!
    \brief Generic function for UCP, applying just iterated linear search for unit-clauses

    \todo Make the concept for Cls explicit
    <ul>
     <li> F.unsat() should just check for the empty clause. </li>
     <li> F.sat() should just check for the empty clause-set. </li>
     <li> F.apply(phi) applies the partial assignment in-place. </li>
    </ul>
  */

  template <class Cls>
  Auxiliary::Sat_status unit_propagation_simple(Cls& F) {
    // Return values Auxiliary::unsatisfiable and Auxiliary::satisfiable
    // report that Cls::unsat() resp. Cls::sat() yielded true.
    for (typename Cls::const_iterator Ci;;) {
      if (F.unsat() == Auxiliary::unsatisfiable) return Auxiliary::unsatisfiable;
      if (F.sat() == Auxiliary::satisfiable) return Auxiliary::satisfiable;
      if ((Ci = std::find_if(F.begin(), F.end(), std::bind2nd(std::bind1st(std::equal_to<typename Cls::size_type>(), 1), std::mem_fun(& Cls::Cl::size)))) == F.end())
	return Auxiliary::unknown;
      else
	F.apply(typename Cls::Pass(*Ci, Cls::Pass::complemented));
    }
  }
}

namespace UnitPropagation {

  /*!
    \brief Generic implementation of the basic linear-time algorithm for UCP

    \todo Make the underlying concepts explicit
    <ul>
     <li> For a literal x, [x.begin,x.end()] is the range of all clauses in
     which x occurs. </li>
    </ul>
  */

  template <class Cls>
  Auxiliary::Sat_status unit_propagation_improved(const Cls& F, typename Cls::Pass& phi) {
    // Return value Auxiliary::satisfiable is not used.
    // If r_1(F) does not yield unsatisfiable, then phi * F = r_1(F),
    // while otherwise phi * F contains the empty clause, and phi
    // corresponds to partial unit-clause elimination on F.

    // Initialisation
    for (typename Cls::const_iterator Ci = F.begin(); Ci != F.end(); ++Ci) {
      const typename Cls::size_type s = *Ci.size();
      switch (s) {
      case 0 : return Auxiliary::unsatisfiable;
      case 1 :
	if (not phi.push_back(*Ci, Cls::Pass::complemented))
	  return Auxiliary::unsatisfiable;
	break;
      default : (**Ci).current_size = s;
      }
    }

    for (typename Cls::Pass::const_iterator phii = phi.begin(); phii != phi.end(); ++phii) {
      const typename Cls::Lit x = *phii.lit();
      for (typename Cls::Lit::InfoValueType::const_iterator xi = *x.begin(); xi != *x.end(); ++xi) {
	// assertion : *(*xi.cl()).current_size >= 2;
	if ((-- (*(*xi.cl()).current_size)) == 1)
	  for (typename Cls::Cl::const_iterator yi = *xi.next(); ; ++yi) {
	    // iterates through the literal occurrences of *xi.cl()
	    // assertion : there is a satisfied or undefined literal
	    const typename Cls::Lit y = *yi.lit();
	    switch (phi(y)) {
	    case Auxiliary::satisfied : goto clause_finished;
	    case Auxiliary::undefined :
	      if (not phi.push_back(y, Cls::Pass::complemented))
		return Auxiliary::unsatisfiable;
	      else goto clause_finished;
	    }
	  }
	clause_finished : ;
      } // all occurrences of x processed
    } // all unit eliminations processed

    return Auxiliary::unknown;
  }
}

#endif
