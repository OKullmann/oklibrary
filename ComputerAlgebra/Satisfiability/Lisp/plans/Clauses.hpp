// Oliver Kullmann, 29.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/plans/Clauses.hpp
  \brief Plans for clause-constructions


  \todo Connections
  <ul>
   <li> See Satisfiability/ProblemInstances/Clauses/concepts/general.hpp for
   plans at C++ level. </li>
  </ul>


  \todo Boolean clauses, and more general clauses
  <ul>
   <li> We have a notion of boolean clauses in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac.
    <ol>
     <li> The meaning of literals is the same for boolean CNF-clauses
     and boolean DNF-clauses. (This is different from signed clauses.) </li>
    </ol>
   </li>
   <li> Also clauses of more general literals are all just
   plain sets. </li>
   <li> With more general literals we get the problem of "standardising"
   clauses; see
   ComputerAlgebra/Satisfiability/Lisp/plans/SatisfactionProblems.hpp.
    <ol>
     <li> For monosigned clauses "possibly degenerated clauses" contain
     pairs [v,e],[v,e'] with e <> e', while this is not the case for
     clauses. </li>
     <li> A degenerated CNF-monosigned clause is constant true, a
     degenerated DNF-monosigned clause is constant false. </li>
    </ol>
   </li>
   <li> We also need to provide promotions (just promoting the literals).
   </li>
  </ul>


  \todo Signed clauses as conditions


  \todo Sets of conditions? (Clause-sets as special cases?)
  Big And resp. big Or ?!

*/

