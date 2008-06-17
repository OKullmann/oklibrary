// Oliver Kullmann, 17.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/plans/Generators.hpp
  \brief Plans regarding generation of combinatorial graphs

  <ul>
   <li> For the sake of concreteness and specification, we also write
   generators if they already exist in another environment. </li>
   <li> Random graphs have their own sub-module; see
   ComputerAlgebra/Graphs/Lisp/plans/RandomGraphs.hpp. </li>
  </ul>


  \todo Generalisations of the Kneser graphs:
  <ul>
   <li> The Johnson graphs J(n,k,i), consisting like the Kneser
   graphs of all k-subsets of n, while we have an edge joining
   two vertices if the intersection has exactly size i. </li>
   <li> The generalised Kneser graph K(n,k,t), the union
   of J(n,k,i) for 0 <= i < t. </li>
   <li> The Kneser graph K(G) of a hypergraph G, with vertices
   the hyperedges, joined by an edge if disjoint. </li>
  </ul>


  \todo Forms of complete graphs
  <ul>
   <li> Create complete bipartite graphs. </li>
   <li> Create complete general graphs with k edges between two
   vertices. </li>
   <li> DONE Create complete graphs. </li>
  </ul>

*/

