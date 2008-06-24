// Oliver Kullmann, 23.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/Isomorphisms/plans/general.hpp
  \brief Plans regarding graph isomorphisms


  \todo Create milestones


  \todo Connections to other modules
  <ul>
   <li> See Combinatorics/Graphs/Isomorphisms/plans/general.hpp for the
   main page. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp
   for isomorphisms of clause-sets. </li>
  </ul>


  \todo Overview on implementations
  <ul>
   <li> Maxima
    <ol>
     <li> isomorphism(g1,g2) </li>
     <li> is_isomorphic(g1,g2) </li>
    </ol>
    are the two available functions from the graphs-package, using a very
    simple backtracking algorithm. Regarding groups, apparently only
    todd_coxeter is available, which computes the order of a group
    presentation through relations.
   </li>
   <li> Sage
    <ol>
     <li> N.I.C.E. implements nauty, which apparently computes the
     lexicographical canonical numbering (?; see below), and also generators
     for the automorphism group (of one or of both graphs?). </li>
     <li> The current documentation (2.7.2) doesn't say much, and one
     needs to read the literature. </li>
     <li> There is also substantial support for combinatorial group theory.
     </li>
    </ol>
   </li>
   <li> Boost
    <ol>
     <li> See Graphs/Isomorphisms/SimpleBacktracking.cpp for an application.
     </li>
    </ol>
   </li>
   <li> Gap </li>
  </ul>


  \todo The lexicographical canonical numbering
  <ul>
   <li> For {0,1}-vectors of the same length we have the linear order given
   by the lexicographical order (with the 0-vector as smallest and 1-vector
   as largest element). </li>
   <li> An ordered graph yields an ordered adjacency matrix; the rows of this
   matrix can be concatenated, yielding a {0,1}-vector, for which then the
   lexicographical order applies. </li>
   <li> So on ordered graphs with the same number of vertices we have a
   linear order. </li>
   <li> Now for a graph one considers some vertex order which yields the
   smallest associated ordered graph. </li>
   <li> The associated {0,1}-vector is a complete invariant for graph
   isomorphism, and choosing some associated vertex order (by some rule)
   yields a canonical numbering (two graphs are isomorphic iff the canonical
   numbering yields an isomorphism). </li>
   <li> This all follows from the fact that two graphs are isomorphic iff
   their adjacency matrices are isomorphic (as combinatorial square
   matrices), where combinatorial square matrices are isomorphic iff they
   can be ordered to be equal. </li>
   <li> Does this canonical numbering establish a functor? </li>
   <li> For combinatorial square matrices over linearly ordered sets we can
   consider a canonical ordering, which yields the lexicographically
   smallest ordered representation. </li>
   <li> The first approach is to find such canonical numberings and their
   the associated invariants by brute force. </li>
   <li> Is the problem of finding such a canonical numbering in NP? </li>
   <li> Obviously instead of graphs we can allow multigraphs with loops
   (i.e., symmetric matrices over NN_0), and also multi digraphs  with loops
   (i.e., square matrices over NN_0). </li>
   <li> Since two general graphs are isomorphic iff the underlying multigraphs
   with loops are isomorphic, also general graphs can be handled here. (Note
   though that general graphs allow for more morphisms, since the edges
   have an identity.) </li>
  </ul>

*/

