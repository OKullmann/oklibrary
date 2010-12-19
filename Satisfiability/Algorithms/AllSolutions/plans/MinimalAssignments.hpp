// Oliver Kullmann, 21.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Algorithms/AllSolutions/plans/MinimalAssignments.hpp
  \brief Plans for computing the generalised "transversal hypergraph"

  Computing Tr(F) for a clause-set F, the set of all consistent minimal
  transversals of F as hypergraph (with literals as vertices).


  \todo Brute-force approach
  <ul>
   <li> The simplest thing to do, and the best thing to start with, is
   the brute-force approach. </li>
   <li> For an upper bound k, run through all partial assignments of length
   up to k, check whether they satisfy the problem instance, and if yes,
   then store them. </li>
   <li> Subsumption-elimination would then be made at the end. We should
   implement that. </li>
   <li> But likely it is more efficient to do subsumption-elimination on the
   fly. Perhaps this is most naturally done within a backtracking approach;
   see "Backtracking (DPLL)" below. </li>
  </ul>


  \todo Relation to hypergraph transversals
  <ul>
   <li> Hypergraph traversal is a special case of "clause-set
   traversal", where for a clause-set the set of minimal satisfying
   partial assignments has to be computed. </li>
   <li> Via the embedding of hypergraphs into clause-sets as positive
   clause-sets, hypergraph transversal (see
   Hypergraphs/Transversals/plans/general.hpp) can be obtained as special
   case. </li>
  </ul>


  \todo The basic algorithm
  <ul>
   <li> Compare with
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Constructions.hpp.
   </li>
   <li> And see dual prime clauses in
   ComputerAlgebra/Satisfiability/Lisp/Primality/plans/general.hpp. </li>
   <li> For a clause-set F, let's call the set of minimal satisfying
   partial assignment also "Tr(F)" (as for hypergraphs). </li>
   <li> Or, perhaps better, Tr(F) is again a set of clauses?! </li>
   <li> The fundamental formula is for computing Tr(F) is
    <center>
    Tr(F_1 union F_2) = min(Tr(F_1) * Tr(F_2))
    </center>
   where for sets P_1, P_2 of partial assignments we define
    <center>
    P_1 * P_2 = { phi_1 composition phi_2 : phi_i in P_i and
      phi_1, phi_2 compatible }
    </center>
   interpreting P_1, P_2 as clause-sets this corresponds to the
   computation of all possible unions, while suppressing tautological
   clauses). </li>
   <li> Heuristics:
    <ol>
     <li> The simplest way to apply this formula is to enumerate all
     clauses, and then adding clauses one by one. </li>
     <li> Even here the order of clauses might be important, and perhaps
     using more general splitting strategies might be worth considering. </li>
     <li> It might be also interesting to investigate storing Tr(F)
     via (variations on) OBDD's (see Laurent Simon's work). </li>
     <li> For computing min efficiently, subsumption elimination
     is crucial (see SetAlgorithms/Subsumption.hpp), especially
     using the literal-clause graph. </li>
     <li> The general strategy for choosing F_1 and F_2 is:
      <ul>
       <li> One thing is how easy it is to compute Tr(F_1) and Tr(F_2). </li>
       <li> Second, in the optimal case no combinations for * are inconsistent
       (so that no effort in computing Tr(F_1), Tr(F_2) was wasted). </li>
       <li> By the same reasoning, in the optimal case no subsumption
       elimination takes place . </li>
      </ul>
     </li>
     <li> It seems hard to predict when inconsistency elimination or
     subsumption elimination will take place (but see below), and so the
     simplest first approach is a greedy heuristics, where a set of easy
     special cases for computing Tr(F) is given, and greedily such a special
     F_1 is computed. So the formula is then
     <center>
       Tr(F_1 union F_2) = min(Tr_sp(F_1) * Tr(F_2))
     </center>
     where Sp_sp is the special method for computing Tr. </li>
    </ol>
   </li>
   <li> If F is disconnected (regarding the variable interactions), then we
   can split the computation of Tr(F) into the computation of the components,
   and need then only to take all combinations --- no subsumptions or
   inconsistencies are possible here. (It might be easy advisable here not to
   perform taking all combinations, but to compute a suitable expression).
   The formula here is
   <center>
     Tr(F_1 union F_2) = Tr(F_1) *' Tr(F_2)
   </center>
   where "*'" means that no inconsistencies can arise. </li>
   <li> Special cases for F when Tr(F) is easy to compute (in linear time
   in |E(Tr(F))|):
    <ol>
     <li> Two different clauses of F are variable-disjoint. </li>
    </ol>
   </li>
   <li> What is the known complexity of computing Tr(F) (in |E(Tr(F))|) ?
   It seems the only two question here is how good inconsistency-elimination
   and subsumption-elimination can be avoided, since churning out possibly
   inconsistent transversals is not a problem. </li>
   <li> A variation of the problem is to output one minimal transversal at a
   time, and to minimise the delays between outputs. The main problem then
   is to output *minimal* transversals. </li>
   <li> In order to compute Tr(Tr(F)) (the set of prime implicates of
   F), Tr(F) is interpreted as a clause-set (all minimal clauses clashing
   with F). </li>
  </ul>


  \todo Using some DNF representation
  <ul>
   <li> Computing Tr(F) can also be achieved by somehow computing the
   set of all satisfying assignment, and then extracting the set of minimal
   assignments from it; for example computing a DNF G equivalent to F, and 
   then computing all prime implicates. </li>
   <li> This could lead to savings for clause-sets, but not for
   hypergraphs. </li>
  </ul>


  \todo Backtracking (DPLL)
  <ul>
   <li> Another possibility is to use a "ternary DPLL", where for
   branching variable v the three branches
   {v} * F, <v -> 0> * F, <v -> 1> * F
   are explored (the first branch must be explored first if global learning
   is enabled in order to make sure that all minimal satisfying assignments
   are found; without global learning one could investigate the other branches
   first when performing subsumption elimination for the result). </li>
   <li> The main question is how to handle subsumption-elimination. </li>
   <li> See "Tree pruning for the backtracking approach" below. </li>
  </ul>


  \todo Parameterisation
  <ul>
   <li> As for the hypergraph case
   (see Hypergraphs/Transversals/Bounded/plans/general.hpp),
   the natural parameter is the size of the partial assignment. </li>
   <li> The hypergraph algorithms can be immediately generalised, only
   that now a variable v with domain size d leads to a branching of size
   d+1 (hypergraphs: d=1). </li>
   <li> Here now the backtracking approach seems more natural. </li>
   <li> For the optimisation version see
   Satisfiability/Optimisation/plans/general.hpp. </li>
   <li> See "Alternative handling of parameter-values 2" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/VanderWaerdenProblems.hpp
   for an example where the translation into a non-boolean SAT problem arises
   naturally (adding a further value to all variables with the constraint that
   this value must be used exactly B times, in this way demanding
   tau(F) <= n-B for input clause-set F; of course one can also allow that the
   values is used at least B times). </li>
   <li> For this formulation one needs to be able to say "v=val" for a variable
   v and a value val (not just "v<>val"); so monosigned literals seem to
   be the right choice here, where actually for F only positive monosigned
   literals can be handled directly. </li>
  </ul>


  \todo Tree pruning for the backtracking approach
  <ul>
   <li> Can tree pruning (minimising a satisfying assignment found, and
   then cutting off unneeded branches) be understood as a special case
   of normal tree pruning for SAT solving? </li>
   <li> If we actually store the satisfying partial assignment found
   (here it would make sense to keep it always subsumption-free, which is not
   too costly, since new assignments cannot strictly subsume old assignments,
   since they are already minimal), then this tree-pruning could be realised by
   finding out that the current partial assignment phi is subsumed by one of
   the assignments already stored, i.e., phi falsifies the negation of the
   stored DNF. </li>
   <li> Here one could use more powerful methods to find an inconsistency?!
   This should further cut the tree, and establish a kind of true dual
   method here. </li>
  </ul>


  \todo Apply it to compute all MUS's, given all MSS's.

*/

