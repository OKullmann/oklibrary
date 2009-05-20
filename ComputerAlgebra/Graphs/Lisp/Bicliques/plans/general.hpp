// Oliver Kullmann, 16.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/Bicliques/plans/general.hpp
  \brief Plans regarding bicliques of (general) graphs


  \todo Directed graphs
  <ul>
   <li> Above we only considered undirected graphs, however it seems that
   perhaps directed graphs are somewhat more fundamental here (at least
   computationally). </li>
   <li> The point is that there are no problems with the sides of the
   biclique (note that a biclique now requires all edges to go from
   one side to the other); this is especially important for clause-sets
   where literals always have a polarity. </li>
   <li> For combinatorial matrices we consider the adjacency matrices
   of directed graphs, which are nonnegative (but not symmetric). </li>
  </ul>


  \todo Maximum bicliques
  <ul>
   <li> There are two ways of measuring the size of a biclique: the number of
   edges and the number of vertices. </li>
   <li> So let's speak of "maximum-vertices bicliques" and "maximum-edges
   bicliques". </li>
   <li> The maximum-edges biclique decision problem (for a given lower bound
   on the number of edges):
    <ol>
     <li> In [Sperschneider, Bioinformatics, Springer 2008], Theorem 5.42, this
     is shown to be NP-complete, even if the graph is just bipartite. </li>
     <li> From this proof a reduction of 3-SAT to this problem can be derived;
     we should implement this. </li>
     <li> On the other hand, we should also have algorithms and reductions for
     solving this problem. </li>
     <li> If the number of edges is given as a parameter: Is this problem then
     fixed-parameter tractable? </li>
    </ol>
   </li>
   <li> The maximum-vertices biclique decision problem:
    <ol>
     <li> Is this also NP-complete? </li>
     <li> And with the number of vertices as parameter: Is this problem then
     fixed-parameter tractable? </li>
    </ol>
   </li>
   <li> See "Bioinformatics" in ComputerAlgebra/plans/general.hpp for an
   application. </li>
  </ul>

*/

