// Oliver Kullmann, 31.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/StrongRegularity/plans/general.hpp
  \brief Plans on strongly regular graphs


  \todo Concepts
  <ul>
   <li> An "srg-parameter-tuple" is a quadruple [n,k,a,c], where
   "n" is the number of vertices, "k" the (constant) vertex degree, "a"
   the (constant) number of common neighbours of adjacent vertices,
   and "c" the (constant) number of common neighbours of different
   non-adjacent vertices. </li>
   <li> First we need a predicate "srg_p(G,t)", which checks whether
   graph G is of the type given by the srg-parameter-tuple t.
    <ol>
     <li> This test should use "g2pghg" (see "Graphs as hypergraphs"
     in ComputerAlgebra/Graphs/Lisp/plans/general.hpp. </li>
     <li> And the neighbour-sets should be computed when creating
     this hypergraph; see "Memoisation for general graphs and multigraphs" in
     ComputerAlgebra/Graphs/Lisp/plans/general.hpp. </li>
    </ol>
   </li>
   <li> A "design graph" is a strongly regular graph with a=c; we speak of
   "k-design graph" if a=c=k. </li>
   <li> We also need to consider the more general notion of "distance-regular"
   graphs (strongly regular graphs are exactly the distance-regular graphs of
   diameter 2). </li>
   <li> It seems that only graphs are considered in the literature; but we need
   to include graphs with loops (see "Exactly one common neighbour" below).
    <ol>
     <li> And, additionally to potentially allowing loops, we should also
     consider "weak" forms of such notions, where for example regularity is not
     required. </li>
     <li> Perhaps we speak of "strongly regular graphs with loops" and "design
     graphs with loops", if we ask for regularity (where a loop is counted
     only once!) and that every two distinct(!) vertices have exactly a resp. c
     common neighbours (where "neighbours" are the adjacent vertices (including
     the vertex itself iff it has a loop)). </li>
     <li> These considerations are motivated by considering "polar incidence
     structures" (compare "Graphs as hypergraphs"
     in ComputerAlgebra/Graphs/Lisp/plans/general.hpp). </li>
     <li> And we add "weak" if we drop the regularity condition. </li>
    </ol>
   </li>
  </ul>


  \todo Exactly one common neighbour
  <ul>
   <li> We consider here graphs where every two distinct vertices have exactly
   one common neighbour, motivated by Exercise 1J in [A Course in
   Combinatorics] (see
   ComputerAlgebra/docus/CourseCombinatorics_LintWilson/Chapter01.hpp). </li>
   <li> This subject is further developed in Exercise 21Q there (see
   CourseCombinatorics_LintWilson/Chapter21.hpp). </li>
   <li> Actually, considering graphs *with loops* is relevant here. </li>
   <li> So we want to determine (if possible) the weak 1-design graphs with
   loops (see above). </li>
   <li> It is currently (31.7.2008) an exercise for MG,ML to determine the
   structure of such graphs with loops.
    <ol>
     <li> We know already that if there is no vertex v_0 neighbour to all other
     vertices, and there are no loops, then such graphs are regular. </li>
     <li> If there is such v_0, what are the possible graphs? </li>
    </ol>
   </li>
   <li> Translating the search for such graphs into generalised SAT problems
   yields a hopefully useful generator:
    <ol>
     <li> n is the parameter, and the boolean variables are (naturally) the
     edges. </li>
     <li> Translating the conditions "at most one common neighbour"
     and "at least one common neighbour" naturally yields propositional
     formulas. </li>
     <li> These can be solved directly, or translated to CNF. </li>
     <li> The condition about the non-existence of v_0 (see above) yields
     easily (long) clauses. </li>
    </ol>
   </li>
  </ul>
  
*/

