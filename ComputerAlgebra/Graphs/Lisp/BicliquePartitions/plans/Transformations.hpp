// Oliver Kullmann, 21.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/BicliquePartitions/plans/Transformations.hpp
  \brief Plans regarding transforming the problem of finding biclique partitions into other problems


  \todo Finding specific biclique partitions via (generalised) SAT
  <ul>
   <li> See TransformationsBiclique.hpp and
   Transitional/OKsolver/plans/general.hpp. </li>
   <li> We must also give an overview on the space of possible
   generalised SAT-Translations.
    <ol>
     <li> The most basic parameter is an upper bound on the number of
     biclique. </li>
     <li> More specifically, we could have a list of biclique sizes. </li>
    </ol>
   </li>
  </ul>


  \todo Transformations
  <ul>
   <li> Find reductions of the problem of finding biclique
   partitions to other problems. </li>
   <li> See "Finding specific biclique partitions via SAT" above. </li>
   <li> Perhaps there are more closely related problems, where the
   translation is more direct. </li>
  </ul>


  \todo Vertex-centred SAT encoding
  <ul>
   <li> A biclique-partition of a multigraph G, using at most k
   parts, can be naturally encoded into a mapping from V to
   {-k, ..., k} - {0}, assigning to each vertex its biclique-number
   together with the "side". </li>
   <li> However, if the degree of a vertex v is m, then we must have
   m "copies" of v (since v will participate in exactly m bicliques). </li>
   <li> For each of such copies, a biclique must contain at most one
   of them. </li>
   <li> For each 1 <= i <= k the vertices with numbers -i,+i must possibly
   form a complete bipartite graph, that is, from every vertex
   with -i to every vertex with +i there is an edge. </li>
   <li> That we have a biclique-partition is then automatically
   achieved, since each of the copies participate in a biclique! </li>
   <li> This looks quite nice. </li>
  </ul>

*/

