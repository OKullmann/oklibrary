// Oliver Kullmann, 20.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/Bicliques/plans/BasicNotions.hpp
  \brief Plans regarding notions of bicliques


  \todo The notion of a "biclique"
  <ul>
   <li> General discussion:
    <ol>
     <li> The "full" mathematical notion of a biclique B in a (general/multi-)
     graph G is that of a sub-graph of G, which itself is a complete bipartite
     graph. </li>
     <li> Since the edges determine the sub-graph B, it is natural to represent
     B by a set B' of edges. </li>
     <li> However then we have the problem, that this notion is not stable
     under sub-set-formation, i.e., a subset of B' in general is not again
     a biclique. </li>
     <li> So for multigraphs and graphs G alternatively we can use
     sets of *vertices* B' such that we can find edges in B' making it
     a complete biclique. </li>
     <li> For graphs this is the subgraph induced by B' </li>
     <li> However, then it is needed to specify the bipartitioning, since in
     general there are many ways that a set of vertices could form a biclique.
     </li>
    </ol>
   </li>
   <li> For multigraphs a biclique partition is a "numerical condition",
   which seems perfectly appropriate since multigraphs are close to
   combinatorial matrices. </li>
   <li> Only for general graphs, where the edges have an identity, we need to
   know precisely which edges are part of the biclique. </li>
   <li> So we could speak of a "subgraph-biclique", "edge-set-biclique" and
   "vertex-set-biclique" (where the latter is a pair of vertex-sets). </li>
   <li> For the ordered graph versions instead a vertex-sets we have
   vertex-lists, and their order must conform with the given total
   order; the abbreviation then is "ovbc". </li>
   <li> DONE (since we need two subsets, it's not really a hypergraph)
   The vertex-bicliques then form a hereditary hypergraph (vertices
   the vertices of the given graph; "hereditary" means that for every
   hyperedge also every subset is a hyperedge, where here we must
   exclude subsets of size 0 or 1). </li>
   <li> DONE (solved by using pairs)
   Likely for (general) digraphs for a given vertex-biclique there are
   several possibilities to interprete it as a edge-biclique, but otherwise
   there should be no ambiguities. </li>
   <li> A subgraph-biclique as well as an edge-biclique can be refined by
   specifying an associated vertex-biclique (which fixes the bipartitioning).
   </li>
  </ul>


  \todo Abbreviations
  <ul>
   <li> "bc" for "biclique". </li>
   <li> Bicliques exist as "sbc", "ebc" and "vbc" (subgraphs, edge-sets
   or vertex-sets). </li>
   <li> A test would be named e.g. "sbc_ogdg_p". </li>
  </ul>


  \todo Maximal biclique
  <ul>
   <li> We need tests for checking whether a biclique is maximal (cannot
   be extended). </li>
   <li> We need maximal_bc_ogl. </li>
   <li> We need a version of maximal_bc_ogl allowing randomisation.
    <ol>
     <li> One control parameter is the given order of the vertices. </li>
     <li> And then we have 0 <= p <= 1, which controls the probability
     that the left or right side is considered first. </li>
    </ol>
   </li>
   <li> For an algorithm finding all maximal bicliques see
   ComputerAlgebra/Graphs/Lisp/Bicliques/plans/Consensus.hpp. </li>
   <li> And regarding *maximum* bicliques see "Maximum bicliques" in
   ComputerAlgebra/Graphs/Lisp/Bicliques/plans/general.hpp. </li>
  </ul>


*/

