// Oliver Kullmann, 8.6.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/plans/Generators.hpp
  \brief Plans regarding generators for constraint problems

  More precisely, for specific problems propagators and instances of the general backtracking algorithms are constructed.


  \todo Write tests


  \todo Concept of a "propagator"
  <ul>
   <li> The concept is specified in
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/ConstraintSatisfaction.mac
   </li>
   <li> Compare it with the literature. </li>
   <li> Write docus. </li>
   <li> Perhaps we create a new sub-module "Propagators.mac" here, and we
   leave only the solvers etc. in Backtracking/ConstraintSatisfaction.mac.
   </li>
  </ul>


  \todo "n Queens"
  <ul>
   <li> See Configurations/NQueens/plans/general.hpp. </li>
   <li> Write a generator for boolean CNF (perhaps also in this module). </li>
   <li> Write a checker for the solutions. </li>
   <li> Especially when all solutions are to be determined, symmetry
   considerations seem necessary. </li>
   <li> Application of "constraint_backtracking" (from
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/ConstraintSatisfaction.mac)
   seems rather successful (at least regarding the number of nodes;
   the following shows n -> #nodes):
    <ol>
     <li> 1,2,3 -> 1 </li>
     <li> 4 -> 2 </li>
     <li> 5 -> 3 </li>
     <li> 6 -> 2 </li>
     <li> 7 -> 3 </li>
     <li> 8 -> 5 </li>
     <li> 9 -> 6 </li>
     <li> 10 -> 6 </li>
     <li> 11 -> 6 </li>
     <li> 12 -> 8 </li>
     <li> 20 -> 15 </li>
     <li> 30 -> 25 </li>
    </ol>
    (This table should be filled out, and linear regression performed.)
   </li>
   <li> Could one show that we have actually a polytime procedure ?
   (It seems that a solution exists if n <> 2,3). </li>
   <li> With a C++ implementation one could investigate larger n. </li>
   <li> What is the performance of constraint solvers on this problem? </li>
   <li> The above good performance of "constraint_backtracking" is likely
   just due to the look-ahead and the ordering heuristics. (One should
   have a look at the trees: How much branching is there?). This will
   likely break down when finding (or counting) all solutions. </li>
   <li> Results of "constraint_backtracking_counting" (from
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/ConstraintSatisfaction.mac)
   (the number of solutions and the number of nodes):
    <ol>
     <li> 1 ->   [1,  1]  </li>
     <li> 2,3 -> [0,  1]  </li>
     <li> 4 ->   [2,  3]  </li>
     <li> 5 ->   [10, 16] </li>
     <li> 6 ->   [4,  7]  </li>
     <li> 7 ->   [40, 48] </li>
     <li> 8 ->   [92, 143] </li>
     <li> 9 ->   [352, 632] </li>
    </ol>
    (This problem has only total solutions, and thus the weakness of the
    general approach, that only total assignments can be taken into
    account, is irrelevant here.)
   </li>
   <li> Of course, one could go here for optimal splitting trees. </li>
  </ul>


  \todo Hitting clause-sets
  <ul>
   <li> Here we look at the problem of searching for (special) hitting
   clause-sets. </li>
   <li> The fundamental propagator "hittingcls_propagator(n,L)",
   where n is the number of boolean variables in the hitting-cls, and
   L is the list of (precise) clause-lengths, is composed of constraints
   imposing the clause-lengths and imposing the conflicts. </li>
   <li> The constraint variables correspond to the literal occurrences,
   and have value-set {-1,0,+1}. </li>
   <li> Constraints for exact clause-lengths:
    <ol>
     <li> Given the list L of constraint-variables and the clause-length m,
     the constraint is that exactly m of them have values <> 0. </li>
     <li> Let l be the length of L. </li>
     <li> Unsatisfiability iff more than m non-zeros exist, or more than
     l - m zeros exist. </li>
     <li> So we need the number za of zero-assignments, and the number nza
     of non-zero assignments. </li>
     <li> A total solution has nza = m and za = l - m. </li>
     <li> If za = l - m, then value zero is to be removed from the other
     variables. </li>
     <li> If nza = m, then all other variables have to be set to zero. </li>
     <li> That's it, or? </li>
     <li> Of course, these are special cases of more general constraints;
     but perhaps we first postpone them. </li>
    </ol>
   </li>
   <li> Constraints for the hitting property:
    <ol>
     <li> Given two clauses "C" and "D", a constraint specifies that we
     have at least one conflict. </li>
     <li> The clauses are given by their list of constraint-variables
     (specifying the literal occurrences). </li>
     <li> One needs to compute then the list of common boolean-variables. </li>
     <li> The constraint is unsatisfiable iff for all these boolean variables
     the domains of their constraint-variables do not contain a clash
     anymore. </li>
     <li> And the constraint is satisfied iff there exists a conflict. </li>
     <li> So we need to compute the value c of conflicts and the value
     p of open potential conflicts, the number of boolean variables for which
     the constraint-variables have still the possibility to clash (but don't
     do yet). </li>
     <li> The constraint is satisfied iff c >= 1. </li>
     <li> And the constraint is unsatisfiable iff p = 0 (and c = 0). </li>
     <li> A domain propagation happens iff p = 1 (and c = 0), in which
     case zero needs to be removed from the domains of the two
     constraint-variables involved; potentially then we have a conflict
     or both have value-set {-1,+1}. </li>
    </ol>
   </li>
  </ul>

*/

