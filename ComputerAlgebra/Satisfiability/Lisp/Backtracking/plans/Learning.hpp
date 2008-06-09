// Oliver Kullmann, 9.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/Learning.hpp
  \brief Plans regarding (pure) learning algorithms


  \todo Connections
  <ul>
   <li> See Satisfiability/Algorithms/Learning/plans/general.hpp. </li>
  </ul>


  \todo basic_learning
  <ul>
   <li> Write a version of basic_learning which uses the most basic
   conflict analysis:
    <ol>
     <li> In some order go through phi and remove assignments which
     are not needed to produce the conflict. </li>
     <li> For that, phi must become a list (with new decision variables
     appended to the end). </li>
     <li> The two most natural orders are the given one and the reverse
     order. </li>
     <li> Since we are always doing a complete restart, it seems best
     to use the given order, so that we try hardest to remove the
     first decision variable (in order to get a different new beginning).
     </li>
     <li> We might use a different (stronger) reduction here. </li>
     <li> If one would do "intelligent backtracking", then it seems
     sensible to backtrack to the first decision variable in the given
     order which is not used in the conflict clause. </li>
    </ol>
   </li>
   <li> The basic statistics is the number of learned clauses. </li>
   <li> A variation of the algorithm returns the set of learned clauses. </li>
   <li> We should run experiments on heuristics:
    <ol>
     <li> The goal is to minimise the number of learned clauses. </li>
     <li> A first guess is satisfiability-orientation. </li>
     <li> On the other hand, like for ordinary DLL-algorithms, one can
     seek to maximise the number of new clauses (for stronger reductions).
     </li>
     <li> This seems sensible, since it (greedily) tries to make the
     learned clause as short as possible (by having more inferred literals).
     </li>
     <li> In the current framework the heuristics just sees the total
     clause-sets, without knowing which clauses have been learned. </li>
     <li> We should first see what can be gained within this restricted
     framework. </li>
    </ol>
   </li>
   <li> A variation of this algorithm does not stop with the first satisfying
   assignment found, but continues (and outputs all satisfying (partial)
   assignments found).
    <ol>
     <li> This realises again a CNF -> DNF translation, but this time
     in general not producing a hitting DNF. </li>
     <li> The satisfying assignment is phi together with the enforced
     assignments. </li>
     <li> Outputting only phi yields a somewhat reduced representation, but
     likely this doesn't matter much, since the dominant factor is the number
     of phi's returned. </li>
     <li> Of course, from the produced satisfying assignment one can remove
     assignments which are not needed for the satisfaction. Removal of
     inferred variables should be only sensible if before the corresponding
     decision variable has been removed. </li>
     <li> A DNF-clause-set G is equivalent to a CNF-clause-set F iff every
     clause of G is a transversal of F and if union(F,comp_cs(G)) is
     unsatisfiable.
      <ul>
       <li> The first condition is easily checkable, and for
       DLL-algorithms also the second condition is easily checkable, since
       F can be replaced by a hitting clause-set F' which is easily
       checked to be implied, and then union(F',G) is an unsatisfiable
       hitting clause-set. </li>
       <li> Now let G be the DNF-clause-set obtain by basic_learning;
       is it now checkable in poly-time whether G is equivalent to F? </li>
       <li> For example, if F0 (as in the algorithm) is the extension of
       F by the learned clauses, is then perhaps
         {} in r(union(F0, comp_cs(G)) ??
       </li>
       <li> We need to test this. </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>


  \todo Intelligent conflict-analysis
  <ul>
   <li> Perhaps we make it the responsibility of an augmented
   reduction to deliver with each derived assignment the decision
   variables responsible for it. </li>
   <li> This is "eager conflict-analysis". </li>
   <li> Then from the falsified clause we directly obtain the conflict-clause
   containing only the responsible decision variables. </li>
   <li> A "cheaper variant" delivers for the set of derived assignments
   (by the reduction) only an implication-dag, and then, when the
   conflict occurs, we need to analyse it further. </li>
   <li> This is "lazy conflict-analysis" as used currently in practice. </li>
  </ul>

*/

