// Oliver Kullmann, 10.1.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/plans/PartialAssignments.hpp
  \brief Plans regarding partial assignments


  \todo Connections
  <ul>
   <li> See
   Satisfiability/Assignments/PartialAssignments/concepts/plans/general.hpp
   for the C++ level. </li>
  </ul>


  \todo Boolean partial assignments
  <ul>
   <li> We have a notion of a "partial assignment" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac. </li>
   <li> Syntactically a partial assignment is just a set of literals. </li>
   <li> Semantically a partial assignment (as a set of total assignments)
   is the same as a DNF-clause. </li>
   <li> We have the relation "phi <= psi", meaning that the set of total
   assignments for phi is a superset for that of psi. This is the same
   as the implication-relation for DNF-clauses. </li>
   <li> However there are further operations associated with partial
   assignments:
    <ol>
     <li> composition </li>
     <li> application phi * F </li>
     <li> evaluation phi(v), phi(x), phi(F) </li>
    </ol>
   </li>
   <li> Shall we make a notational difference between "partial assignments"
   and "partial multiassignments", or is it just depending on the underlying
   literal-type? </li>
  </ul>

*/

