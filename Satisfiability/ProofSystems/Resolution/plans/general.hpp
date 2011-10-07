// Oliver Kullmann, 16.2.2007 (Swansea)
/* Copyright 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProofSystems/Resolution/plans/general.hpp
  \brief Plans for the modules providing components for resolution handling


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/general.hpp
   for the Maxima-level. </li>
  </ul>


  \todo Update namespaces.


  \todo Update milestones.


  \todo Update todos.


  \todo Whatever remains of module DPv shall move here
  <ul>
   <li> It seems that a dedicated module on Davis-Putnam-resolution
   is needed, but likely this is best handled as submodule of
   the resolution module. </li>
  </ul>


  \todo The basic problems
  <ul>
   <li> Overview on the different forms of resolutions (for non-boolean
   variables). </li>
   <li> Overview on datastructures and ADT's. </li>
  </ul>


  \todo Write docus
  <ul>
   <li> Link to the resolution facilities in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets. </li>
  </ul>


  \todo A Resolvent-class
  <ul>
   <li> The basic problem is that the resolution operation is partial. </li>
   <li> A concept which offers combined services:
   \code
template <class Clauses>
class Resolvent {
  public :
  Resolvent(const Clauses& C, const Clauses& D);
  bool decided_resolvability() const;
  bool computed_resolution_literal() const;
  bool computed_resolvent() const;
  typedef Clauses clause_type;
  typedef typename Clauses::literal_type literal_type;
  bool resolvable();
  const literal_type& resolution_literal();
  const clause_type& resolvent();
}
   \endcode
   </li>
   <li> After initialisation decided_resolvability,
   computed_resolution_literal, computed_resolvent are false. </li>
   <li> If computed_resolution_literal is true then decided_resolvability
   is true; if computed_resolvent is true, then computed_resolution_literal
   is true. </li>
   <li> After calling resolvable, decided_resolvability is true; the
   call is constant-time if decided_resolvability is true. </li>
   <li> After calling resolution_literal, computed_resolution_literal is
   true; the call is constant-time if computed_resolution_literal is
   true. The returned literal is singular iff the clauses are not
   resolvable. </li>
   <li> After calling resolvent, computed_resolvent is true; the
   call is constant-time if computed_resolvent is true. If the
   clauses are not resolvable, then the returned clause is unchanged
   (i.e., still default-constructed). </li>
  </ul>

*/

/*!
  \namespace OKlib::Resolution
  \brief Components for resolution handling
*/

namespace OKlib {
  namespace Resolution {
  }
}

