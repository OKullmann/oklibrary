// Oliver Kullmann, 20.11.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/Embeddings/Isomorphisms/plans/GraphIsomorphisms.hpp
  \brief %Algorithms to determine graph isomorphisms (directly or by reducing
  it to (generalised) SAT problems)


  Given two graphs G_1, G_2 together with maps v in V(G_1) -> D(v) <= V(G_2)
  specifying to which vertices v may me mapped, find a graph isomorphism from
  G_1 to G_2 fulfilling these specifications.

  For direct graph isomorphism algorithms see
  Combinatorics/Graphs/Isomorphisms/plans/general.hpp.


  \todo Vertex invariants
  <ul>
   <li> The first step consists in applying algorithms which further reduce
   the sets D(v), for example by using that isomorphisms maintain vertex
   degrees. </li>
   <li> More generally, each vertex is labelled by some list of multisets of
   natural numbers, such that isomorphisms must maintain these lists; the
   first step is to use just the vertex degrees, the second step is to use for
   each vertex the multiset, which maps each possible vertex degree of a
   neighbour to the count of such neighbours; and so on (is this the idea of
   "nauty" ?). </li>
   <li> Another possibility is to use adjacency relations: If w is
   adjacent to v, then for every value w' in D(w) there must exist a value v'
   in D(v) such that w' is adjacent to v'. </li>
   <li> Either such algorithms are processed up to a certain level, or "until
   they get stuck", and then the remaining problem is formulated as a
   satisfiability problem. </li>
  </ul>


  \todo Alliances
  <ul>
   <li> Still just given G_1, G_2 and D, but D may have been further refined
   by the previous step. </li>
   <li> We obtain a alliance of active clause-sets with variable set V(G_1)
   and variable domains given by D, with members
   <ol>
    <li> BIJ(V(G_1), D, V(G_2)) (see module "Generators"). </li>
    <li> A virtual clause-set which in principal for every vertex v and set V
    of possible values can yield for every vertex w adjacent to v in G the set
    of possible values W given by the set of w' such that v' in V with {v',w'}
    in E(G_2) exists. </li>
    <li> Of main importance are the cases with |V| = 1 (these conditions ensure
    completeness of the translation). </li>
    <li> This active clause-set may also offer stronger inferences. </li>
    <li> In principal it is again an algorithm from the first step, processing
    a refined domain-assignment as input and computing further refinements.
    </li>
   </ol>
   </li>
   <li> This alliance then is passed to a SAT algorithm (or output, using the canonical
   clause-representation). </li>
  </ul>


  \todo What about the graph embedding problem ? This should likely go to
  another module.

*/

