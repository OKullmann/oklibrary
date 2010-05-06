// Oliver Kullmann, 13.8.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/general.hpp
  \brief Plans regarding pseudo-boolean constraints


  \todo The notion of a "pseudo-boolean constraint"
  <ul>
   <li> Given literals x_1, ..., x_k and integers l, a_1, ..., a_k, u,
   a pseudo-boolean constraint expresses l <= sum_{i=1}^k a_i * x_i <= u,
   where a satisfied literal means integer 1, a falsified literal means
   integer 0, and where l, u may also be inf. </li>
   <li> Compare "Representing cardinality constraints" in
   Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp
   for the special case where all a_i = 1. </li>
   <li> As boolean functions, these are threshold functions as considered in
   Satisfiability/Lisp/FiniteFunctions/plans/Thresholds.hpp. </li>
   <li> Where for cardinality constraint we just had a set L of literals, now
   we have a list L of literals and a list A of the same length of integers
   (containing the coefficients). </li>
   <li> It just remains the question on the precise composition of the list
   which represents a pseudo-boolean constraint. </li>
   <li> One could use lists [l,A,L,u]. </li>
   <li> The major abbreviation for "pseudo-boolean constraint" should be "pbc".
   </li>
  </ul>


  \todo Application of partial assignments
  <ul>
   <li> For a partial assignment and a pbc P=[a,A,L,b] let phi*P be
   [a',A',L',b], where A', L' are obtained from A resp. L by removing
   the position corresponding to variables in phi, while a', b' are obtained
   by arithmetic substitution (and bringing the new constants either to the
   left or to the right side of the inequality). </li>
  </ul>


  \todo Pseudo-boolean constraints as active clauses
  <ul>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/plans/Conditions.hpp
   and
   ComputerAlgebra/Satisfiability/Lisp/plans/SatisfactionProblems.hpp for
   general plans. </li>
   <li> See "Pseudo-boolean constraints as active clauses" in
   ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp
   for the special case of cardinality constraints. </li>
   <li> Pseudo-boolean constraints, and especially cardinality constraint seem
   good candidates for first "active clauses":
    <ol>
     <li> Determining satisfiability and forced assignments w.r.t. a partial
     assignment should be relatively easy (it is definitely so for cardinality
     constraints; for pseudo-boolean constraints one needs to determine
     satisfiability of a single integer inequality with {0,1}-variables
     resp. to determine whether some variables have forced values). </li>
     <li> Actually, for pbc's we have a knapsack-problem, so the problem here
     is actually not that easy --- we have here an active clause-set! </li>
     <li> At least for cardinality constraints we can always determine
     the corresponding prime implicates representation, and in this way
     really have "virtual clauses" --- via determining the sizes of this
     representation we can completely emulate the in a sense optimal
     representation by prime implicates without actually having to create
     them! </li>
    </ol>
   </li>
  </ul>

  
  \todo Add milestones

*/

