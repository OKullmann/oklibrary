// Oliver Kullmann, 21.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/BicliquePartitions/plans/Transformations.hpp
  \brief Plans regarding transforming the problem of finding biclique partitions into other problems


  \todo Parameters for encoding
  <ul>
   <li> The most basic parameter is an upper bound on the number of
   biclique, that is, an upper bound on the number of variables. </li>
   <li> More specifically, we could have a list of biclique sizes,
   that is, for every variable we specify the number of occurrences. </li>
   <li> When can also specify the number of literal occurrences, by fixing
   the sizes of the sides of the bicliques. </li>
   <li> The size of a clause corresponding to a vertex is between 1 and the
   degree of the vertex (we should consider here only connected graphs), namely
   it is exactly the number of bicliques the vertex participates. </li>



  \todo Edge-centered SAT encoding
  <ul>
   <li> See TransformationsBiclique.hpp and
   OKlib/OKsolver/plans/general.hpp. </li>
  </ul>


  \todo Vertex-centred SAT encoding
  <ul>
   <li> A biclique-partition of a multigraph G, using at most k
   parts, can be naturally encoded into a mapping from V to
   {-k, ..., k} - {0}, assigning to each vertex its biclique-number
   together with the "side". </li>
   <li> However, if the degree of a vertex v is d(v), then we must have
   1 <= l_i <= d(v) "copies" of v (since v will participate in exactly d(v)
   bicliques).
    <ol>
     <li> l_i is the length of the clause corresponding to this vertex. </li>
     <li> So the strength of this model is that the clause-lengths can naturally
     be prescribed. </li>
     <li> Of course, this is also a weakness; apparently unavoidable. </li>
    </ol>
   </li>
   <li> No two copies of a vertex can be contain in the same biclique. </li>
   <li> In general we need to ensure that all edges have been covered (while
   the partitioning condition is automatically given now):
    <ol>
     <li> Natural is to use for each vertex v a cardinality constraint
     requiring that together for all the copies we have exactly d(v) edges.
     </li>
     <li> One could also specify that the total number of edges is as
     required. </li>
     <li> On the other end, one could specify for each copy of v the number
     of neighbours. </li>
     <li> So literal regularity of the corresponding clause-set is easy to
     require. </li>
     <li>
    </ol>
   </li>
   <li> For each 1 <= i <= k the vertices with numbers -i,+i must possibly
   form a complete bipartite graph, that is, from every vertex
   with -i to every vertex with +i there is an edge. </li>
   <li> That we have a biclique-partition is then automatically
   achieved, since each of the copies participate in a biclique! </li>
   <li> This looks quite nice.
    <ol>
     <li> A natural application is when searching for a uniform clause-set.
     </li>
     <li> Variable degrees for variable 1 <= i <= k can be naturalled specified by
     cardinality constraints on the number of copies of vertices which get values
     -i or +i. </li>
     <li> And also literal degrees can be easily prescribed (by cardinality
     constraints), again just counting copies of vertices; see also above for
     the issue of ensuring that enough edges are there. </li>
     <li> It would be nice if one could apply it to search for ABDs, however
     therefore we must also know the conflict matrix; see
     ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/AssociativeBlockDesigns.hpp
     </li>
    </ol>
   </li>
  </ul>


  \todo Transformations
  <ul>
   <li> Find reductions of the problem of finding biclique
   partitions to other problems (than SAT). </li>
   <li> Perhaps there are more closely related problems, where the
   translation is more direct. </li>
  </ul>

*/

