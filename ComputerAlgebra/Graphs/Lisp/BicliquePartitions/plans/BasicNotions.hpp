// Oliver Kullmann, 21.6.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/BicliquePartitions/plans/BasicNotions.hpp
  \brief Plans regarding notions of biclique partitions in (general) graphs


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Graphs/Lisp/Bicliques/plans/general.hpp. </li>
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
   <li> Write predicates to check for biclique-partitions:
    <ol>
     <li> From Maxima we can use bipartition(gr). </li>
     <li> For edge-bicliques of general graphs:
     \verbatim
ebcp_gg_p(B,G) := partitionp(B,G[2]) and
  every_s(lambda([b],ebc_gg_p(b,G)),B)$
     \endverbatim
     where G is a general graph, 
     </li>
    </ol>
   </li>
   <li> We have a natural hierarchy of contexts for biclique partitions:
    <ol>
     <li> Most special are biclique partitions of directed (multi-)graphs.
     </li>
     <li> Next come biclique partitions of bipartite (multi-)graphs with
     given bipartition. Directed (multi-)graphs G are covered here by the
     conversion which uses the bipartition (V(G), V(G)), where we have
     an edge from "left" to "right" iff the corresponding directed edge
     in G exists. </li>
     <li> This conversion corresponds to considering the adjacency matrix
     of G, and interpreting it as the reduced adjacency matrix of a bipartite
     graph. </li>
     <li> Finally we have biclique partitions of arbitrary (multi-)graphs.
     </li>
     <li> Note that representing graphs G by directed graphs G' (via creating
     from each edge two arcs, in both directions) does not embed the biclique
     partition problem for graphs in that for directed graphs, since in G'
     we have to find (different) bicliques for both arcs, while in G, so
     to speak, one chooses an orientation. </li>
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
     <li> Especially with multidigraphs the difference is only that for
     combinatorial matrices we have two arguments (the 2 indices), while
     for multigraphs we have one (the edge). </li>
     <li> So with combinatorial matrices we have automatically the
     bipartitions given, while this is not the case for
     (general) graphs, and this makes the difference. </li>
     <li> More pronouncedly, as discussed above a biclique partitioning
     of a graph needs to choose an orientation (corresponding to choosing
     only one of the two 1's representing the edge in the adjacency matrix).
     </li>
     <li> So representing a combinatorial matrix over NN_0 as a "sum" of
     1-constant matrices with disjoint row- and column-sets is, up to
     a different encoding, the same as a biclique partition of a bipartite
     multigraph with given bipartition. </li>
     <li> Algorithmically, graphs are "sparse representations", while
     combinatorial matrices are "dense" (i.e., with them it is natural
     to query "quickly" whether two vertices are adjacent). </li>
     <li> So we should explore both avenues. </li>
    </ol>
   </li>
  </ul>


  \todo Directed graphs
  <ul>
   <li> See "Directed graphs" in
   ComputerAlgebra/Graphs/Lisp/Bicliques/plans/general.hpp. </li>
  </ul>


  \todo Abbreviations
  <ul>
   <li> "bcp" is for "biclique partition". </li>
   <li> See "Abbreviations" in
   ComputerAlgebra/Graphs/Lisp/Bicliques/plans/general.hpp. </li>
   <li> E.g. "sbcp" would be a biclique partition by
   subgraph-bicliques. </li>
  </ul>

*/

