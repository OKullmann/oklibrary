// Oliver Kullmann, 22.9.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Hypergraphs/Transversals/plans/SizeParameter.hpp
  \brief Plans for the module on computing hypergraph transversals
  of size at most k


  \todo Trivial algorithm for input G, k
  <ul>
   <li> Outline of the algorithm:
    <ol>
     <li> Another input is a vertex set V of size <= k, to be
     extended to a transversal of size at most k. </li>
     <li> If all hyperedges are covered, return V. </li>
     <li> Otherwise, if |V| is of size = k, then return "impossible". </li>
     <li> Otherwise choose hyperedge H which is not hit by V, and for v in H
     try recursively V + {v}. </li>
    </ol>
   </li>
   <li> For the choice of H the simplest heuristics is to choose a
   smallest H.
    <ol>
     <li> Simplest to order the hyperedges once at the beginning. </li>
     <li> Stronger if the hypergraph datastructure gives acces to the
     current smallest hyperedge; but this is not enough, we need the
     smallest *uncovered* hyperedge (one whose size has not changed
     since the beginning). </li>
    </ol>
   </li>
   <li> For the order of vertices, the simplest heuristics is to order
   them in descending degree. </li>
   <li> Special services needed from the hypergraph:
    <ol>
     <li> Necessary is a largest hyperedge. </li>
     <li> For heuristical purposes a smallest hyperedge (strongest:
     amongst the untouched hyperedges). </li>
     <li> For heuristical purposes a vertex of largest degree. </li>
    </ol>
   </li>
  </ul>


  \todo Overview on more sophisticated algorithms

*/

