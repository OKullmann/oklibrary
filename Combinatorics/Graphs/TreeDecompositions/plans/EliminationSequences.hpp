// Oliver Kullmann, 19.12.2007 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Graphs/TreeDecompositions/plans/EliminationSequences.hpp
  \brief Plans tree decompositions via elimination sequences


  \todo GraphDecomposition::Treewidth_by_enumerating_elimination_sequences
  <ul>
   <li> Write tests. </li>
   <li> Generalise it by using strategy objects. </li>
  </ul>


  \todo GraphDecomposition::Width_elimination_sequence
  <ul>
   <li> Extend it by computing also the tree decompositon. </li>
   <li>  Here it seems necessary that we make a local copy of the input graph,
   where each vertex in the copy has as property its associated vertex in the
   original graph (in this way we can change the copy, while the tree
   decomposition obtained referes to the original (unchanged) graph). </li>
  </ul>


  \todo Greedy strategy
  <ul>
   <li> Implement the greedy strategy for computing an elimination
   sequence. </li>
   <li> Likely we achieve better performance when not going to the eye of the
   needle represented by the elimination sequence, but just taking as input
   a graph g with vertices and edges removable, and asking an algorithm
   visitor for the next vertex from the (remaining) graph --- the greedy
   algorithm then is realised by an algorithm visitor returning a vertex with
   minimum degree. </li>
  </ul>


*/

