// Oliver Kullmann, 15.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/Trees/plans/general.hpp
  \brief Plans regarding trees (as combinatorial objects)


  \todo Concepts
  <ul>
   <li> The "rooted ordered trees" are the trees (rt) as in
   ComputerAlgebra/Trees/Lisp/Basics.mac. </li>
   <li> Are there other types of "directed trees" than "oriented
   trees" (directed graphs without antiparallel edges, such that the
   underlying graph is a tree)?
    <ol>
     <li> We have "arborescences", which are rooted oriented trees such that
     all edges point to the root. They are the same as rooted trees, when
     considered on their own, but their use is as subgraphs of digraphs. </li>
    </ol>
   </li>
  </ul>


  \todo Isomorphism testing
  <ul>
   <li> We need a poly-time test for the isomorphism of trees. </li>
   <li> We need also to detect whether a graph is a pathgraph or
   a star. </li>
   <li> This seems to belong to Graphs/Lisp/Isomorphisms/Trees.mac. </li>
   <li> Or should we place it in this module? </li>
   <li> And the automorphism group needs to be determined. </li>
  </ul>


  \todo Spanning trees
  <ul>
   <li> It seems they also belong to here, but they also belong to
   GraphTraversal (see ComputerAlgebra/Graphs/Lisp/plans/general.hpp)? </li>
   <li> Likely in GraphTraversal we should have the special algorithms
   obtained by this approach, while other algorithms and other questions
   are handled in Trees/SpanningTrees. </li>
  </ul>

*/

