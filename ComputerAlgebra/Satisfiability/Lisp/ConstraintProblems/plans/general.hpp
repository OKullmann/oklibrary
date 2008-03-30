// Oliver Kullmann, 2.12.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/plans/general.hpp
  \brief Plans for Maxima-components regarding constraint satisfaction problems


  \todo Create milestones


  \todo Write tests


  \todo Write docus


  \todo Relations to other modules
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/general.hpp.
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
   seems rather succesful (at least regarding the number of nodes;
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
   <li> With a C++ implementation one could investiate larger n. </li>
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

*/

