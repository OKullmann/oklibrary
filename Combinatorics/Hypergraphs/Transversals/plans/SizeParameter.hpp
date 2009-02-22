// Oliver Kullmann, 22.9.2007 (Swansea)
/* Copyright 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Hypergraphs/Transversals/plans/SizeParameter.hpp
  \brief Plans for the module on computing hypergraph transversals
  of size at most k


  \todo Trivial algorithm for input G, k
  <ul>
   <li> The task is to find a transversal T with |T| <= k. </li>
   <li> Outline of the algorithm:
    <ol>
     <li> Another input is a vertex set T of size <= k, to be
     extended to a transversal of size at most k. </li>
     <li> If all hyperedges are covered, return T. </li>
     <li> Otherwise, if |T| is of size = k, then return "impossible". </li>
     <li> Otherwise choose some hyperedge H which is not hit by T, and for all
     v in H try recursively T + {v}. </li>
    </ol>
   </li>
   <li> If a minimal transversal T is needed (w.r.t. set-inclusion),
   then by some heuristics unnecessary vertices in T are removed
   (iteratively). </li>
   <li> For the choice of H the simplest heuristics is to choose a
   smallest H.
    <ol>
     <li> Simplest to order the hyperedges once at the beginning. </li>
     <li> Stronger if the hypergraph datastructure gives access to the
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


  \todo An "eager, binary" variation of the trivial algorithm
  <ul>
   <li> The algorithms transversals_be, transversals_bes in
   ComputerAlgebra/Hypergraphs/Lisp/Transversals.mac implement the
   above algorithm, but with the following two differences:
    <ol>
     <li> Above we use "clause branching", while these algorithms use
     binary branching. </li>
     <li> Above the handling of the hypergraph is "lazy" (not performing
     the assignment), while these algorithms are "eager" (actually performing
     the assignment). </li>
    </ol>
   </li>
  </ul>


  \todo Enumerating all transversals T with |T| <= k
  <ul>
   <li> The above trivial algorithm also enumerates all transversals
   T with |T| <= k, when just continuing the process until the end. </li>
   <li> However now we really only want to find minimal transversals (w.r.t.
   inclusion). This can be achieved by computing all and then applying
   subsumption elimination, but we need also more efficient solutions. </li>
   <li> It seems natural, as above for a transversal T found just to minimise
   it (trivially), and then to prune parts of the tree which cannot contribute
   a minimal transversal anymore. This process is similar to tree pruning
   ("intelligent backtracking") for SAT. </li>
   <li> Of course, if k is at most the transversal number, then no further
   steps are needed; this situation arises naturally when either searching for
   a minimum transversal from below, or when computing transversal numbers
   for monotonically increasing sequences of hypergraphs (at one new vertex
   at each step); see
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/Transversals.hpp.
   </li>
  </ul>


  \todo Overview on more sophisticated algorithms

*/

