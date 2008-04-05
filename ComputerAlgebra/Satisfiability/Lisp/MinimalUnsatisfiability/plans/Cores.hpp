// Oliver Kullmann, 2.4.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/Cores.hpp
  \brief Plans related to irredundant cores


  \todo Organisation
  <ul>
   <li> Create sub-modules "Cores.mac". </li>
   <li> From
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/Basics.mac
   the following functions should belong to it:
    <ol>
     <li> min_size_mus_bydef(F, S) </li>
     <li> contains_us_bydef </li>
     <li> first_mus_fcs </li>
     <li> first_irr_fcs </li>
     <li> all_irr_cores_bydef </li>
     <li> all_forced_irr_cores_bydef </li>
    </ol>
   </li>
   <li> We should group these functions better together. </li>
   <li> DONE We should use an "impliesp(F,C,S)" predicate. </li>
  </ul>


  \todo all_irr_cores_bydef
  <ul>
   <li> We could have a randomised version of all_forced_irr_cores_bydef,
   not using backtracking but just exploring one path.
    <ol>
     <li> Where are Maxima random generators? </li>
     <li> More generally, we ask for a "random sample" of N
     (if existent) irredundant cores. </li>
     <li> A parameter p controls the probability of going left
     or right in the recursive splitting step. </li>
     <li> Then p=0 and p=1 correspond to the existing version
     and the alternative version (with switched branches) mentioned
     below. Using N=inf we obtain then all irredundant cores. </li>
    </ol>
   </li>
   <li> If we monitor the computation and decide that the currently
   obtained clause-sets are enough --- is there any way to stop the
   computation, keeping the current results?? Ask on Maxima mailing
   list. </li>
   <li> A variation of all_forced_irr_cores_bydef would postpone the
   irredundancy check just before "return({Forced})". However, I (OK)
   cannot imagine this to be more efficient. </li>
   <li> DONE (the corrected version has on the first branch on deletions)
   An alternative version of all_forced_irr_cores_bydef would
   make the recursive branching with switched branches, delaying
   as much as possible the inclusion of clauses. Perhaps this is
   handled by the randomised version above. </li>
   <li> We could write a version of all_forced_irr_cores_bydef, which
   only works on unsatisfiable clause-sets. On the other hand, not much
   advantage seems to follow from this additional knowledge? </li>
  </ul>


  \todo Finding an irredundant core of minimum size
  <ul>
   <li> First we concentrate on measure "number of literal occurrences". </li>
   <li> Similar to optimal_splitting_tree (in
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/SplittingTrees.mac)
   we need a branch-and-bound implementation. </li>
  </ul>

*/

