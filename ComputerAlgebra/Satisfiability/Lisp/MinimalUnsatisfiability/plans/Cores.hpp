// Oliver Kullmann, 2.4.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/Cores.hpp
  \brief Plans related to irredundant cores


  \todo all_irr_cores_bydef
  <ul>
   <li> Tests: Cover all branches of the function. </li>
   <li> Write documentation:
    <ol>
     <li> The algorithm (and the underlying reasoning) is not completely
     trivial. </li>
     <li> So needs to be explained properly, with examples. </li>
     <li> Also a proof of correctness is needed. </li>
    </ol>
   </li>
   <li> Yet we choose just an arbitrary clause for the branching:
    <ol>
     <li> A reasonable heuristic would be to choose a clause of
     minimal length. </li>
     <li> The target would be to get the smallest irredundant cores
     at the beginning. </li>
     <li> Otherwise actually it doesn't seem to have any influence. </li>
     <li> However, we first remove the clause! So perhaps we should choose
     a clause of maximal length? </li>
     <li> One would guess that choosing a clause of maximal length
     would tend to first produce a core of small size, while choosing
     a clause of minimal length would quicker lead to the first
     core, since we get more forced clauses. </li>
     <li> These considerations should be more relevant to the sampling
     version below. </li>
     <li> Perhaps we supply the heuristics as a parameter (selecting
     a clause C from Possible). </li>
     <li> Should this heuristics also have access to Forced? Likely yes.
     </li>
    </ol>
   </li>
   <li> If we monitor the computation and decide that the currently
   obtained clause-sets are enough --- is there any way to stop the
   computation, keeping the current results?? Ask on Maxima mailing
   list.
    <ol>
     <li> One possibility would be to push the results into a global
     variable. </li>
     <li> Or, better, to provide a variable into which these results
     are pushed. So we need to provide the *name* of the variable? </li>
    </ol>
   </li>
   <li> Monitoring:
    <ol>
     <li> Variable counter_irr is only relevant for monitoring, so
     perhaps should not be defined as if of general relevance. </li>
     <li> It would be informative to print out also the path
     to the current node (at the entry), as list of 1s and 2s.
      <ul>
       <li> Perhaps this list is a bit long; printing it out as
       string saves space. </li>
       <li> We could also send all monitoring output to a file. </li>
       <li> Perhaps we shold work on general methods and standards for
       monitoring. Where to put it? Perhaps a new supermodule Programming? </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> A variation of all_forced_irr_cores_bydef would postpone the
   irredundancy check just before "return({Forced})". However, I (OK)
   cannot imagine this to be more efficient. </li>
   <li> We could write a version of all_forced_irr_cores_bydef, which
   only works on unsatisfiable clause-sets. On the other hand, not much
   advantage seems to follow from this additional knowledge? </li>
   <li> DONE (the corrected version has on the first branch only deletions)
   An alternative version of all_forced_irr_cores_bydef would
   make the recursive branching with switched branches, delaying
   as much as possible the inclusion of clauses. Perhaps this is
   handled by the randomised version sample_irr_cores. </li>
  </ul>


  \todo sample_irr_cores
  <ul>
   <li> The task is to obtain a random sample of N (if existent)
   irredundant cores. </li>
   <li> A problem is that these samples will stay close to the first
   completed path (due to the backtracking process). </li>
   <li> So perhaps we create another version, which after each
   completed sample starts a complete new path. </li>
   <li> The problem with that is that it takes endless to go down
   the path, just to find one instance. </li>
   <li> So perhaps here we just go down the path, only checking
   the clause selected, whether it is necessary or unusable, or
   whether it is a choice clause. </li>
   <li> And only at the end do we check whether the obtained clause-set
   is actually irredundant --- if not then we just restart. </li>
   <li> For testing we then use a function "irredundant_corep"
   (for checking whether a clause-set is an irredundant core of
   another clause-set). </li>
   <li> DONE Where are Maxima random generators? </li>
   <li> DONE A parameter p controls the probability of going left
   or right in the recursive splitting step. </li>
   <li> DONE Then p=0 and p=1 correspond to the existing version
   and the alternative version (with switched branches) mentioned
   below. Using N=inf we obtain then all irredundant cores. </li>
  </ul>


  \todo Finding an irredundant core of minimum size
  <ul>
   <li> First we concentrate on measure "number of literal occurrences". </li>
   <li> Similar to optimal_splitting_tree (in
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/SplittingTrees.mac)
   we need a branch-and-bound implementation. </li>
  </ul>


  \todo Organisation : DONE
  <ul>
   <li> DONE Create sub-modules "Cores.mac". </li>
   <li> DONE From
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/Basics.mac
   the following functions should belong here:
    <ol>
     <li> min_size_mus_bydef(F, S) </li>
     <li> contains_us_bydef </li>
     <li> first_mus_fcs </li>
     <li> first_irr_fcs </li>
     <li> all_irr_cores_bydef </li>
     <li> all_forced_irr_cores_bydef </li>
    </ol>
   </li>
   <li> DONE We should group these functions better together. </li>
   <li> DONE We should use an "impliesp(F,C,S)" predicate. </li>
  </ul>

*/

