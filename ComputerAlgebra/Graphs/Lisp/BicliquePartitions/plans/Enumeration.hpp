// Oliver Kullmann, 21.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/BicliquePartitions/plans/Enumeration.hpp
  \brief Plans regarding enumeration of all biclique partitions in (general) graphs


  \todo Enumerating biclique partitions
  <ul>
   <li> Given a general graph G, enumerate all biclique partitions. </li>
   <li> As discussed in
   ComputerAlgebra/Graphs/Lisp/BicliquePartitions/plans/Bicliques.hpp,
   likely multigraphs are more appropriate, since then we do not need to
   distinguish between different parallel edges. </li>
   <li> The problem is the same as finding all clause-sets without pure
   literals with a given conflict multigraph (namely G). </li>
   <li> The simplest approach:
    <ol>
     <li> Via set_partitions(edge_set) create all possible partitions
     of the edge-set, and then we compute the subset given by bicliquepartp.
     </li>
    </ol>
   </li>
   <li> Alternatively, we translate the biclique partition problem into
   a SAT problem (see
   ComputerAlgebra/Graphs/Lisp/BicliquePartitions/plans/Transformations.hpp)
   and enumerate all solutions.
    <ol>
     <li> However, this is then a harder problem, since we demand certain
     properties of the biclique partition. </li>
    </ol>
   </li>
   <li> Can we write an efficient generator, which creates one biclique
   partition at a time, with polynomial delay, running through all biclique
   partitions without repetition?
    <ol>
     <li> For example, running through the lexicographical ordering?! </li>
     <li> Most natural seems to consider a backtracking approach. </li>
    </ol>
   </li>
   <li> Given all biclique partitions, we can then find all isomorphism
   types:
    <ol>
     <li> Two biclique partitions are isomorphic if the associated clause-sets
     are isomorphic. </li>
     <li> The problem of finding all isomorphism types is then the same as
     determining all isomorphism types of clause-sets without pure literals
     and with conflict multigraph G. </li>
    </ol>
   </li>
   <li> Again the question is whether there is an efficient generator for
   all isomorphism types?
    <ol>
     <li> Definitely we do not need to generate first all biclique partitions.
     </li>
     <li> Instead we use a backtracking approach, where we investigate new
     branches only if they are not isomorphic to branches already considered.
     </li>
    </ol>
   </li>
   <li> Viewing biclique partitions are solutions of a constraint satisfaction
   problem:
    <ol>
     <li> Given a general G = (V,E). </li>
     <li> Find a function b : E -> {1,...,|E|} which yield biclique
     partitions. </li>
     <li> This blows up the search space, but makes the problem more
     accessible. </li>
     <li> Making this translation explicit is discussed below in
     ComputerAlgebra/Graphs/Lisp/BicliquePartitions/plans/Transformations.hpp.
     </li>
     <li> But we can explore such a point of view more implicitely. </li>
    </ol>
   </li>
  </ul>

*/

