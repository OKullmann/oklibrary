// Oliver Kullmann, 23.5.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/plans/BicliquePartitions.hpp
  \brief Plans regarding biclique partitions of (general) graphs


  \todo Basic concepts
  <ul>
   <li> A "biclique partition" of a general graph is a partition of the
   edge set such that each part constitutes a complete bipartite graph. </li>
   <li> Write predicate "bicliquepartp" to check for this property. </li>
   <li> Likely this more algebraic aspects of graph theory is handled
   better by combinatorial matrices; see
   ComputerAlgebra/CombinatorialMatrices/Lisp/plans/general.hpp. </li>
   <li> Also for translating "biclique partition into clause-sets we
   do most work at the level of combinatorial matrices. </li>
  </ul>


  \todo Tools for graphs
  <ul>
   <li> From the Maxima graphs-library:
    <ol>
     <li> complete_bipartite_graph (n, m) </li>
     <li> bipartition (gr) </li>
     <li> is_bipartite (gr) </li>
    </ol>
   </li>
  </ul>


  \todo The conjecture of [Galesi, Kullmann]
  <ul>
   <li> Using the (deprecated) Mupad-functions, the conjecture is
   that if ist_exakt_transversal(M) is true, then
   ist_speziell_exakt_transversal(M) is true. </li>
   <li> This should be equivalent to the statement, that every minimally
   unsatisfiable clause-set F, whose conflict multigraph is a complete
   bipartite graph, has a read-once resolution refutation. </li>
  </ul>

*/

