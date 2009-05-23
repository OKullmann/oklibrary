// Oliver Kullmann, 17.6.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
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


  \todo Generalisations of the Kneser graphs
  <ul>
   <li> The Johnson graphs J(n,k,i), consisting like the Kneser
   graphs of all k-subsets of n, while we have an edge joining
   two vertices if the intersection has exactly size i. </li>
   <li> The generalised Kneser graph K(n,k,t), the union
   of J(n,k,i) for 0 <= i < t. </li>
   <li> The Kneser graph K(G) of a hypergraph G, with vertices
   the hyperedges, joined by an edge if disjoint. </li>
   <li> DONE (the graphs there are not the Kneser-graphs; see
   "Maxima package graphs" in ComputerAlgebra/Graphs/Lisp/plans/general.hpp)
   In the Maxima-graphs-documentation, the Knesergraph K(n,m)
   is called "Petersen graph(N,m)", which is not right --- tell the
   mailing list. </li>
  </ul>


  \todo Parameters of the Kneser graphs
  <ul>
   <li> Likely all relevant parameters regarding some generator should
   be made available where the generator is defined. </li>
   <li> Most famous for the Kneser graphs is the chromatic number. </li>
   <li> And then we have the automorphism group:
    <ol>
     <li> Order of this group. </li>
     <li> Explicit enumeration. </li>
     <li> A fundamental question here is whether, like for the Petersen
     graph, always the full permutation group of {1,...,n} induces
     all automorphisms of kneser_g(n,m).
      <ul>
       <li> This is not the case for example for n=5, m=3, where there are
       no edges (and thus every permutation of powerset(setn(5),3) is
       an automorphism. </li>
      </ul>
     </li>
     <li> Abstract representation through relations. </li>
     <li> And also realising the generators through concrete
     automorphisms. </li>
    </ol>
   </li>
   <li> Is a formula known for the girth? </li>
   <li> And for the precise degree of arc-transitivity? This would imply
   the order of the automorphism group. </li>
   <li> See http://en.wikipedia.org/wiki/Kneser_graph. </li>
   <li> We should somewhere present a sub-module with all the
   information available on the Petersen-graph (for example
   the isomorphism from S_5 to its automorphism group, nice
   representations etc.; see above). As a part of the general
   tools for investigating Kneser graphs. </li>
  </ul>


  \todo Forms of complete graphs
  <ul>
   <li> Create complete bipartite graphs. </li>
   <li> Create complete general graphs with k edges between two
   vertices. </li>
   <li> DONE Create complete graphs. </li>
  </ul>

*/

