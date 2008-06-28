// Oliver Kullmann, 21.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/BicliquePartitions/plans/Bicliques.hpp
  \brief Plans regarding biclique in (general) graphs


  \todo The notion of a "biclique"
  <ul>
   <li> The "full" mathematical notion of a biclique B in a (general/multi-)
   graph G is that of a sub-graph of G, which itself is a complete bipartite
   graph. </li>
   <li> Since the edges determine the sub-graph B, it is tempting to represent
   B by a set B' of edges. </li>
   <li> However then we have the problem, that this notion is not stable
   under sub-set-formation, i.e., a subset of B' in general is not again
   a biclique. </li>
   <li> So for multigraphs and graphs G it seems more appropriate to use
   sets of *vertices* B' such that we can find edges in B' making it
   a complete biclique. </li>
   <li> For graphs this is the subgraph induced by B' </li>
   <li> While for multigraphs this is a "numerical condition", which seems
   perfectly appropriate since multigraphs are close to combinatorial matrices.
   </li>
   <li> Only for general graphs, where the edges have an identity, we need to
   know precisely which edges are part of the biclique. </li>
   <li> So we could speak of a "subgraph-biclique", "edge-set-biclique" and
   "vertex-set-biclique". </li>
   <li> The vertex-bicliques then form a hereditary hypergraph (vertices
   the vertices of the given graph; "hereditary" means that for every
   hyperedge also every subset is a hyperedge, where here we must
   exclude subsets of size 0 or 1). </li>
   <li> A sub-graph biclique can be refined to a subgraph with (given)
   bipartition. For a vertex-set-biclique this corresponds to a partition
   of this vertex set according to the two sides (fixing the sides!). </li>
  </ul>


  \todo The notion of a biclique partition
  <ul>
   <li> For each of the above notions of bicliques, we obtain a notion
   of biclique partition. </li>
   <li> Most interesting seems the vertex-oriented notions for (multi-)
   graphs. </li>
   <li> This corresponds to a representation of the adjacency matrix of the
   multigraph as a sum of sub-matrices which are constant 1. </li>
   <li> Considering the edge-oriented notion, we get the notion of an
   "edge-biclique-partition" of a general graph as a partition of the
   edge set such that each part constitutes a complete bipartite graph.
    <ol>
     <li> We take "partition" here in the formal sense, that is, a set of
     subsets, none empty, union the whole, pairwise disjoint. </li>
     <li> As mentioned, we also need "polarised" versions where the parts of
     the biclique are also specified. </li>
     <li> Here we need, given an edge-subset of a general graph, to
     construct the "induced" general sub-graph, whose vertex set is
     just the vertices used by those edges. </li>
     <li> Then we need a check for simplicity of a general graph. </li>
     <li> And then we need a bipartiteness test for general graphs
     (using the Maxima function). </li>
    </ol>
   </li>
   <li> Write predicate "bicliquepartp" to check for this property.
    <ol>
     <li> From Maxima we can use bipartition(gr). </li>
    </ol>
   </li>
  </ul>


  \todo Combinatorial matrices
  <ul>
   <li> Likely the more algebraic aspects of graph theory are handled
   better by combinatorial matrices; see
   ComputerAlgebra/CombinatorialMatrices/Lisp/plans/general.hpp. </li>
   <li> Also for translating biclique partition into clause-sets we
   do most work at the level of combinatorial matrices. </li>
   <li> In ComputerAlgebra/CombinatorialMatrices/Lisp/plans/general.hpp
   we find some plans on doing biclique partitioning for combinatorial
   matrices:
    <ol>
     <li> The question is, what is the right place, here or there? </li>
     <li> Especially with multigraphs the difference is only that for
     combinatorial matrices we have two arguments (the 2 indices), while
     for multigraphs we have one (the edge). </li>
     <li> So with combinatorial matrices we have automatically the
     bipartitions given, while this is not the case for
     (general) graphs, and this makes the difference. </li>
     <li> Algorithmically, graphs are "sparse representations", while
     combinatorial matrices are "dense" (i.e., with them it is natural
     to query "quickly" whether two vertices are adjacent). </li>
     <li> So likely we should explore both avenues. </li>
    </ol>
   </li>
  </ul>


  \todo Directed graphs
  <ul>
   <li> Above we only considered undirected graphs, however it seems that
   perhaps directed graphs are somewhat more fundamental here (at least
   computationally). </li>
   <li> The point is that there are no problems with the sides of the
   biclique (note that a biclique now requires all edges to go from
   one side to the other); this is especially important for clause-sets
   where literals always have a polarity. </li>
   <li> For combinatorial matrices then we consider the adjacency matrices
   of directed graphs, which are still nonnegative, but which are no longer
   symmetric. </li>
  </ul>

*/

