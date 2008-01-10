// Oliver Kullmann, 29.6.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/plans/general.hpp
  \brief Plans for Lisp-SAT in general


  \todo Redesign
  <ul>
   <li> First we consider the simple Lisp-level. </li>
   <li> So all existing computer-algebra-SAT-components are moved
   to Satisfiability/Lisp. </li>
  </ul>


  \todo Relations to other modules
  <ul>
   <li> What is the relation to
   LogicalReflection/FormalisedProofs/plans/FundamentalADTs.hpp ? </li>
  </ul>


  \todo Update plans:
  <ul>
   <li> Satisfiability/Lisp/plans/Clauses.hpp </li>
   <li> Satisfiability/Lisp/plans/Literals.hpp </li>
   <li> Satisfiability/Lisp/plans/MinimalSatisfyingAssignments.hpp </li>
   <li> Satisfiability/Lisp/plans/SatisfactionProblems.hpp </li>
  </ul>


  \todo Symmetries
  <ul>
   <li> Compute the orbits of literals under the action of the
   automorphism group of a clause-set. </li>
   <li> Stronger one could compute simply all permutations combined with
   sign flips which leave the clause-set invariant, i.e., the full
   automorphism group in the trivial representation. </li>
   <li> Then one needs to look for more intelligent representations
   of the automorphism group. </li>
   <li> Likely, Maxima is only for simple and initial attempts towards
   symmetries, and Gap is the real tool here. </li>
  </ul>

*/

