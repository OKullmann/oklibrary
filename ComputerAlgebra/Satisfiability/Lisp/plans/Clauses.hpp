// Oliver Kullmann, 29.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/plans/Clauses.hpp
  \brief Plans for clause-constructions


  \todo Boolean clauses, and more general clauses
  <ul>
   <li> We have a notion of boolean clauses in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac,
   and this seems sufficient. </li>
   <li> Also clauses of more general literals are all just
   plain sets. </li>
   <li> With more general literals we get the problem of "standardising"
   clauses. </li>
   <li> We also need to provide promotions (just promoting the literals).
   </li>
  </ul>


  \todo Signed clauses as conditions


  \todo Sets of conditions? (Clause-sets as special cases?)
  Big And resp. big Or ?!

*/

