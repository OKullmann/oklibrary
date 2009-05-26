// Oliver Kullmann, 23.3.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/plans/Transversals.hpp
  \brief Plans regarding hypergraphs transversals (in Maxima/Lisp)


  \todo Connections
  <ul>
   <li> See
   Combinatorics/Hypergraphs/Transversals/plans/DirectTransversalEnumeration.hpp
   for computing the transversal hypergraph via recursive splitting. </li>
   <li> See
   Combinatorics/Hypergraphs/Transversals/plans/SizeParameter.hpp
   for the use of a parameter bounding the size of the transversals. </li>
   <li> And, more generally, see
   Combinatorics/Hypergraphs/Transversals/plans/general.hpp. </li>
   <li> See "dual_cs" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Constructions.mac, and see
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Constructions.hpp.
   </li>
  </ul>


  \todo Transfer
  <ul>
   <li> See "Rewrite hypergraph transversal functions in Maxima" in
   ComputerAlgebra/Mupad/plans/general.hpp. </li>
  </ul>


  \todo Testing
  <ul>
   <li> okltest_transversal_hyp should be a generic test for computing
   the transversal hypergraph (used for all different methods).
   Further test-cases:
    <ol>
     <li> Claw-graphs. </li>
     <li> Complete graphs (and complete r-graphs). </li>
     <li> Graphs without edges. </li>
    </ol>
    It seems superfluous to also have a dedicated test-function for the
    independence hypergraph (i.e., that check should just transfer the
    test to the transversal-test)? See below.
   </li>
  </ul>


  \todo transversal_hyp
  <ul>
   <li> Performance on complete graphs:
    <ol>
     <li> Likely this performs badly. </li>
     <li> And this likely holds for all splitting strategies. </li>
     <li> For example when adding one edge after another, starting
     with the empty edge-set and completing with all edges, there
     should be always some graph in the sequence with large transversal
     hypergraph. </li>
    </ol>
   </li>
  </ul>


  \todo The triangle of the three possibilities
  <ul>
   <li> The complementary problem is about maximal independent sets,
   the dual problem is about hyperedge coverings. </li>
   <li> Likely these problems have each at least one natural algorithm,
   different from the translations of the others. </li>
   <li> So we need to provide a general scheme for translations. </li>
  </ul>


  \todo ind_hyp(G)
  <ul>
   <li> See Combinatorics/Hypergraphs/IndependentSets/plans/general.hpp. </li>
   <li> The test-function okltest_ind_hyp should be a generic test-function,
   applicable to any function for computing the hypergraph of (maximal)
   independent subsets (the "independence hypergraph"). </li>
   <li> So, as with transversals, we must distinguish between the (many)
   possible algorithms for ind_hyp. </li>
   <li> An alternative method for computing ind_hyp is by direct
   construction:
    <ol>
     <li> Bottom-up: Start with all independent subsets of size 0.
     Then for k=0, 1, ... take the sets of size k and consider all
     possibilities to a vertex, obtain so level k+1. Those which
     cannot be extended go to the final result, while otherwise
     the processed level is removed. </li>
     <li> This algorithm is for example efficient for complete graphs. </li>
     <li> Top-down: We start with the full vertex-set at level n.
     Then for k = n,n-1, ... all subsets which are independent are
     moved to the final result, while otherwise all possible (k-1)-subsets
     are formed, which makes level k-1, and level k is removed. </li>
     <li> This algorithm is efficient for hypergraphs with no
     hyperedges. </li>
    </ol>
   </li>
   <li> Of course, a question is whether independence hypergraphs are
   really on its own, or we should standardise all considerations by
   considering transversals? Just taking the edgewise-complement is not
   a big step? But it's a step, and also the literature is different. </li>
  </ul>

*/

