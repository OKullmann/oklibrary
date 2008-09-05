// Oliver Kullmann, 4.9.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/RamseyProblems.hpp
  \brief Plans for Maxima-generators for Ramsey-problems


  \todo Relations to other modules
  <ul>
   <li> Compare "Ramsey graphs" in
   ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
   <li> See Ramsey.cpp. </li>
  </ul>


  \todo Extreme cases
  <ul>
   <li> One needs to think about the definition of the extreme cases; it seems
   there are inconsistencies in our tests. </li>
   <li> See "Ramsey hypergraphs" in
   ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
  </ul>


  \todo More efficient computation
  <ul>
   <li> For larger Ramsey problems, we need the ability to compute directly
   the standardised clause-set (which just consists of the hypergraph edges
   in two versions, positive (as they are) and negative), without further
   renaming. </li>
   <li> This can be achieved by using the standardised order as discussed in
   "Ramsey hypergraphs" in
   ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
   <li> Compare "Colouring problems" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/general.hpp. </li>
  </ul>


  \todo Symmetry breaking
  <ul>
   <li> Consider the K_n, and parameters r=2 and q. </li>
   <li> Partial assignments phi assign the edges of K_n. </li>
   <li> Given any phi, we have the following possibility for symmetry
   breaking by fixing values w.r.t. q-cliques:
    <ol>
     <li> The basic fact is that any 2-edge-labelling of K_q, which is
     not monochromatic, must contain two incident edges of different
     values (for q >= 3). </li>
     <li> By symmetry these two incident edges can be any pair of incident
     edges. </li>
     <li> So, if there is a subset T <= V(K_n) of size q, such that no edge
     for T is affected (i.e., assigned) by phi, then w.l.o.g. one can fix the
     values of 2 chosen incident edges to 1 and 2. </li>
     <li> This process can be repeated until no such T exists anymore. </li>
     <li> A question is whether there are better and worse choices for T
     and for the chosen edges? </li>
     <li> One needs to be careful here about "affected by phi": The above
     formulation is very "optimistic", while more "pessimistic" is that
     an edge is affected iff it is incident with an assigned one --- are
     there counterexample for the "optimistic" interpretation? </li>
     <li> More general than above, also for T with one affected edge one
     could choose another incident edge:
      <ul>
       <li> But in general this is wrong. </li>
       <li> A counter example is K_5, r=2, q=3:
        <ol>
         <li> Consider vertices 1,2,3, colour {1,2} with 1 and {2,3} with 2.
         </li>
         <li> Consider vertices 2,3,4, colour {2,4} with 1. </li>
         <li> Consider vertices 2,4,5, colour {4,5} with 2. </li>
         <li> Consider vertices 1,4,5, colour {1,4} with 1. </li>
         <li> There is a monochromatic triangle between vertices 1,2,4 but K_5
         allows a colouring without one. </li>
        </ol>
       </li>
      </ul>
     </li>
     <li> So we can assign for every q-element subset of V(K_n) at least one
     and at most two (incident) edges. </li>
     <li> The number of assignments is 2 * floor(n/q). </li>
     <li> It shouldn't matter much which q-subsets (and which edges in them)
     to choose. </li>
     <li> Searching for an unaffected T is easy: One records which vertices
     are incident to assigned edges and chooses any q free vertices. </li>
     <li> The underlying principle is simply that any solution must contain
     for every q-clique a "multi-coloured" path of length 2. Without case
     distinctions it seems not possible to strengthen that. </li>
    </ol>
   </li>
   <li> The idea of MG was, instead of looking at the q-subsets
   of V(K_n), to consider Ramsey numbers m := NR([p,p],2) < n:
    <ol>
     <li> Again, we are considering some aritrary given phi. </li>
     <li> If we have an unaffected T <= V(K_n), then we can such any
     p different a_1, ..., a_p in T, and set them equal (thus replace them
     by a single new variable a). </li>
     <li> It seems that for an affected T it is harder to do something:
     If one edge get colour c, then we had to make a case distinction,
     expressing that the monochromatic p-clique could have colour c or the
     other colour. </li>
     <li> And, again, "unaffected" likely should mean that for T there is
     also no assigned edge (just) incident with an element of T. </li>
     <li> For example for m=6, p=3, we actually know that there must
     exist two monochromatic triangles: However this again seems hard to
     exploit since we need to make case distinctions about the relative
     position of these two triangles. </li>
     <li> It seems that this process yields less variable reductions than
     the above process? </li>
     <li> The above process exploits a symmetry of the solution space,
     while this process just relies on a symmetry of the K_n (since
     every(!) total assignment of a K_m will have a monochromatic
     p-clique); on the other hand the above process is trivial, while here
     we rely on (non-trivial) knowledge about Ramsey numbers. </li>
     <li> Again the question whether the different choices matter? </li>
     <li> In principle a combination with the above process is possible:
     However case distinctions for the relative positions of the path
     of length 2 and the monochromatic q-clique are needed. </li>
     <li> On the other hand, for unaffected subsets we can always choose
     which process we like. </li>
    </ol>
   </li>
  </ul>

*/

