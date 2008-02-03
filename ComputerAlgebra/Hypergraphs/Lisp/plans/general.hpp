// Oliver Kullmann, 30.11.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/plans/general.hpp
  \brief Plans for Maxima-components for hypergraphs


  \todo Create milestones


  \todo Write tests


  \todo Write docus
  <ul>
   <li> We should have a list of all available functions. </li>
   <li> Seems that this list needs to be maintained manually? </li>
  </ul>


  \todo Hypergraphs/Lisp/Generators.mac
  <ul>
   <li> Create complete r-graphs. </li>
   <li> Create Sudoko-hypergraphs (such that the Sudoko-problems can be
   expressed as strong colouring problems).
    <ol>
     <li> For box dimension p the vertex set should be {1,...,p}^2. </li>
     <li> The 3N hyperedges (N=p^2) just correspond to the rows, columns
     and boxes (the hypergraph is thus N-uniform). </li>
    </ol>
   </li>
   <li> Create Ramsey graphs etc. </li>
  </ul>


  \todo Hypergraph transversals
  <ul>
   <li> Likely this should become its own module. </li>
   <li> See Combinatorics/Hypergraphs/Transversals/plans/DirectTransversalEnumeration.hpp. </li>
   <li> See "Rewrite hypergraph transversal functions in Maxima" in
   ComputerAlgebra/Mupad/plans/general.hpp. </li>
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
   <li> transversal_hyp:
    <ol>
     <li> Performance on complete graphs:
      <ul>
       <li> Likely this performs badly. </li>
       <li> And this likely holds for all splitting strategies. </li>
       <li> For example when adding one edge after another, starting
       with the empty edge-set and completing with all edges, there
       should be always some graph in the sequence with large transversal
       hypergraph. </li>
      </ul>
    </ol>
   </li>
   <li> The triangle of the three possibilities:
    <ol>
     <li> The complementary problem is about maximal independent sets,
     the dual problem is about hyperedge coverings. </li>
     <li> Likely these problems have each at least one natural algorithm,
     different from the translations of the others. </li>
     <li> So we need to provide a general scheme for translations. </li>
    </ol>
   </li>
   <li> ind_hyp(G)
    <ol>
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
    </ol>
   </li>
  </ul>

*/

