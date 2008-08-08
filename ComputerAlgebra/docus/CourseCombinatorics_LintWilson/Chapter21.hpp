// Oliver Kullmann, 7.8.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/docus/CourseCombinatorics_LintWilson/Chapter21.hpp
  \brief Chapter "Strongly regular graphs and partial geometries" from [A Course in %Combinatorics]


  <h1> Parameters of strongly regular graphs </h1>

  See module ComputerAlgebra/Graphs/Lisp/StrongRegularity.


  <h1> The adjacency matrix and its eigenvalues </h1>


  <h1> Partial geometries </h1>

  See module ComputerAlgebra/IncidenceStructures.


  <h1> Neighbourhood-regular graphs </h1>


  <h1> The "friendship theorem" </h1>

  Exercise 21Q: "If every two persons have exactly one mutual friend,
  then there is a politician (who is the friend of everybody (else))."

  <ul>
   <li> See Exercise 1J in CourseCombinatorics_LintWilson/Chapter01.hpp. </li>
   <li> According to
   ComputerAlgebra/Graphs/Lisp/StrongRegularity/plans/general.hpp, we call
   a graph with loops, such that every two distinct vertices have exactly
   one neighbour, a "weak design graph with loops". </li>
   <li> The isomorphism types of weak design graphs with loops correspond
   exactly to the isomorphism types of projective incidence planes with given
   polarity (see
   ComputerAlgebra/IncidenceStructures/Lisp/plans/ProjectivePlanes.hpp). </li>
   <li> The "friendship theorem" states that every non-empty weak design
   graph has a dominating vertex:
    <ol>
     <li> Weak design graphs with dominating vertices exist exactly for odd
     vertex numbers, and then there is exactly one isomorphism type, namely
     the disjoint union of triangles, all glued together at one common vertex.
     </li>
     <li> Considered as a (polar) incidence structure, these are the
     "near-pencils". Note that near-pencils have other polar representations,
     which correspond to weak design graphs with loops, where the underlying
     graph has a dominating vertex. (There is just one near-pencil with
     n points, but ceil(n/2) polar representations (always w.r.t. the
     appropriate isomorphisms).) </li>
     <li> Proving, that no non-empty weak design graph without dominating
     vertex exists, is done in the book by first employing Exercise 1J, from
     which we get that we must have a non-empty design graph without dominating
     vertex (that is, we have regularity), and then showing using the
     eigenvalue-conditions that this is impossible. </li>
     <li> The (directly) equivalent statement for projective incidence
     planes is, that every projective incidence plane with a polarity without
     absolute points (i.e., every point is mapped to a line not incident with
     that vertex) is empty or a pencil (has a line containing all points) or a
     near-pencil (has a line containing all vertices except of one). </li>
     <li> Important here: Isomorphisms are considered for projective incidence
     planes (not for polar projective planes!). </li>
     <li> Since empty projective planes, pencils and near-pencils are
     degenerated projective planes, this is typically expressed by
     "Polarities for projective incidence planes must have absolute points."
     </li>
    </ol>
   </li>
  </ul>

*/

