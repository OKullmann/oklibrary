// Oliver Kullmann, 7.10.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/Search.hpp
  \brief Plans regarding searching for short resolution proofs


  \todo Finding short tree-resolution proofs
  <ul>
   <li> See optimal_splitting_tree in
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/SplittingTrees.mac for
   a backtracking approach. </li>
   <li> Implement the dynamic-programming algorithm for computing
   the tree resolution complexity for all possible clauses
   over var(F) for a given clause-set F:
    <ol>
     <li> Let trc(C) be the tree-resolution complexity of C w.r.t. F,
     that is, the minimal number of nodes in a tree resolution
     derivation of (exactly) C from F. </li>
     <li> So trc(C) is an odd natural number. </li>
     <li> At the beginning trc(C) is "open" for
     all C over F. </li>
     <li> Then in round i = 1, 2, ... we determine
     all clauses C with trc(C) = 2i-1 (by marking exactly those
     clauses with 2i-1). </li>
     <li> In round 1 exactly the clauses of F are marked with 1. </li>
     <li> In round i > 1 for a yet unmarked clause C one checks whether
     it is the resolvent of marked clauses C',C'' such that
     trc(C') + trc(C'') + 1 = 2i-1 in which case clause C gets marked. </li>
    </ol>
   </li>
   <li> For extracting an optimal tree resolution refutation for the
   empty clause, when marking a clause one should also store
   pointers to the parent clauses. </li>
   <li> Can further restrictions be handled for boolean resolution? </li>
   <li> For non-boolean clause-sets there is still only one form
   of resolution, however if signed literals are allowed, then
   more forms are possible. </li>
   <li> If only interested in the empty clause, then we can stop
   as soon as we found it (and the reached level then is
   exactly the tree-resolution complexity of F). </li>
   <li> Are there other strategies to make the procedure a bit more
   efficient?
    <ol>
     <li> One possibility is to restrict the maximal clause-size
     considered (for 2-CNF it is actually sufficient to consider
     only clauses of length at most 2). </li>
    </ol>
   </li>
  </ul>


  \todo Regular-resolution complexity
  <ul>
   <li> One could handle it like resolution complexity, only with the
   additional restriction on the paths that no resolution variable may
   occur twice. </li>
  </ul>


  \todo How to compute the minimal DP-resolution complexity?
  <ul>
   <li> Is it necessary to run through all permutations of the set
   of variables ? </li>
   <li> The question here is about the notion of complexity:
    <ol>
     <li> One could consider ordinary resolution complexity, only that
     the order of variables on all paths is the same. </li>
     <li> Or one considers the sum of the sizes of the levels of the
     DP-procedure. </li>
    </ol>
   </li>
   <li> In case one considers resolution complexity, then one can
   use the methods for (full) resolution proofs (or for regular
   resolution proofs). </li>
   <li> When considering the sum of sizes, then perhaps one has to
   run through all permutations.
    <ol>
     <li> Via a greedy heuristics we start with a "promising" ordering. </li>
     <li> Then we can stop any other order as soon that the sum is bigger than
     what we have already achieved. </li>
     <li> Likely subsumption elimination should be applied throughout. </li>
     <li> If an ordering v_1, ..., v_k was ruled out (a partial ordering, only
     specifying an initial segment), then actually all orderings which use
     a permutation of these v_1, ..., v_k as an initial a segment are ruled
     out as well! </li>
     <li> One needs a nice way of implementing this, but it should achieve
     a substantial shortcut. </li>
    </ol>
   </li>
  </ul>


  \todo Resolution complexity
  <ul>
   <li> First we should concentrate on the decision problem, whether
   input F has a resolution refutation of length at most k. </li>
   <li> One approach is to translate the search for a resolution
   refutation into a (generalised) SAT problem: We have k variables for
   the k steps in the proof (presented as a sequence), and values are
   either clauses from F or a pair of indices for a resolution step.
   See "Finding short resolution proofs via SAT" below. </li>
   <li> Preprocessing:
    <ol>
     <li> Subsumption elimination should be performed first. </li>
     <li> And then the learn kernel is to be computed (removing all clauses
     which cannot participate in a resolution refutation). </li>
     <li> We must also find out the necessary clauses (since they have to be
     placed). </li>
    </ol>
   </li>
   <li> Backtracking approach:
    <ol>
     <li> A question is whether we should enumerate all unsatisfiable
     sub-clause-sets, and take each as the basis for a search (in a
     related search those clauses must then all be used). </li>
     <li> An easy criterion to see that the goal can't be reached is
     given by the observation that each resolution step can only remove
     at most one literal. So clauses which are too long to reach the empty
     clause in the given number of steps can be removed. </li>
     <li> And when reusing a clause C, then it must be smallest
     w.r.t. subsumption. </li>
    </ol>
   </li>
  </ul>


  \todo 2-CNF
  <ul>
   <li> Implement the Buresh-Oppenheim/Mitchell algorithm
   for computing the minimal resolution complexity of F in
   boolean 2-CNF in polynomial time. </li>
  </ul>
  

  \todo Finding short resolution proofs via SAT

*/
