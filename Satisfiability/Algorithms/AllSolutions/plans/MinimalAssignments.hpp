// Oliver Kullmann, 21.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file AllSolutions/plans/MinimalAssignments.hpp
  \brief Plans for computing the generalised "transversal hypergraph"


  \todo The basic algorithm:
  <ul>
   <li> Hypergraph traversal is a special case of "clause-set
   traversal", where for a clause-set the set of minimal satisfying
   partial assignments has to be computed. </li>
   <li> Via the embedding of hypergraphs into clause-sets as positive clause-sets,
   hypergraph transversal (see HypergraphTransversals/plans/general.hpp)
   can be obtained as special case. </li>
   <li> For a clause-set F, let's call the set of minimal satisfying
   partial assignment also "Tr(F)". </li>
   <li> Computation of Tr(F) for clause-sets F. The fundamental formula is
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
   <li> The simplest way to apply this formula is to enumerate all
   clauses, and then adding clauses one by one. Even here the
   order of clauses might be important, and perhaps using more
   general splitting strategies might be worth considering.
   It might be also interesting to investigate storing Tr(F)
   via (variations on) OBDD's (see Laurent Simon's work).
   For computing min efficiently, subsumption elimination
   is crucial (see SetAlgorithms/Subsumption.hpp), especially
   using the literal-clause graph.
   </li>
   <li> In order to compute Tr(Tr(F)) (the set of prime implicates of
   F), Tr(F) is interpreted as a clause-set (all minimal clauses clashing
   with F). </li>
  </ul>


  \todo Computing Tr(F) can also be achieved by somehow computing the
  set of all satisfying assignment,
  and then extracting the set of minimal assignments from it;
  for example computing a DNF G equivalent to F, and then computing
  all prime implicates.


  \todo Another possibility is to use a "ternary DPLL", where for
  branching variable v the three branches
  {v} * F, <v -> 0> * F, <v -> 1> * F
  are explored (the first branch must be explored first if global learning
  is enabled in order to make sure that all minimal satisfying assignments
  are found; without global learning one could investigate the other branches
  first when performing subsumption elimination for the result).


  \todo Parameterisation
  <ul>
   <li> As for the hypergraph case
   (see Hypergraphs/Transversals/plans/SizeParameter.hpp),
   the natural parameter is the size of the partial assignment. </li>
   <li> The hypergraph algorithms can be immediately generalised, only
   that now a variable v with domain size d leads to a branching of size
   d+1 (hypergraphs: d=1). </li>
  </ul>


  \todo Tree pruning
  <ul>
   <li> Can tree pruning (minimising a satisfying assignment found, and
   then cutting off unneeded branches) be understood as a special case
   of normal tree pruning for SAT solving? </li>
   <li> If we actually store the satisfying partial assignment found
   (here it would make sense to keep it always subsumption-free, which is not
   too costly, since new assignments cannot strictly subsume old assignments,
   since they are already minimal), then this tree-pruning could be realised by
   finding out that the current partial assignment phi is subsumed by one of the
   assignments already stored, i.e., phi falsifies the negation of the stored DNF. </li>
   <li> Here one could use more powerful methods to find a inconsistency?!
   This should further cut the tree, and establish a kind of true dual
   method here. </li>
  </ul>


  \todo Apply it to compute all MUS's, given all MSS's.

*/

