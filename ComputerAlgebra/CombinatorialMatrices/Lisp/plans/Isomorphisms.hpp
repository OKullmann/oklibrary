// Oliver Kullmann, 9.8.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/CombinatorialMatrices/Lisp/plans/Isomorphisms.hpp
  \brief Plans for isomorphisms of (combinatorial) matrices


  \todo Isomorphism testing by backtracking
  <ul>
   <li> We need a backtracking isomorphism solver, as a generalised SAT
   solver. </li>
   <li> Compare
   ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp. </li>
  </ul>


  \todo Connections to other modules
  <ul>
   <li> Via isomorphism of combinatorial matrices we can decide var-isomorphism
   of labelled clause-sets. </li>
   <li> And isomorphism of general hypergraphs is the special case where only
   {0,1}-matrices are involved. </li>
  </ul>


  \todo Necessary conditions
  <ul>
   <li> Computing the characteristic polynomial for a square matrix (compare
   "hermitian_rank_charpoly" in
   ComputerAlgebra/LinearAlgebra/Lisp/QuadraticForms.mac) seems to be a
   reasonable additional test. </li>
   <li> For non-square matrices M one can consider trans(M)*M and M*trans(M),
   obtaining square matrices for which the characteristic polynomials can be
   computed. </li>
   <li> One can also compute rank and determinant, which are included when
   computing the characteristic polynomial for square matrices; does this still
   hold for arbitrary matrices? </li>
   <li> Likely the conditions for row- and column-sums are not covered by
   characteristic polynomials? </li>
  </ul>

*/

