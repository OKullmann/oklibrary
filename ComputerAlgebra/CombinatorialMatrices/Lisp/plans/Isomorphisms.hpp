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
   <li> Via isomorphism of combinatorial {-1,0,1}-matrices we can decide
   var-isomorphism of labelled clause-sets. </li>
   <li> And isomorphism of general hypergraphs is the special case where only
   {0,1}-matrices are involved. </li>
   <li> While isomorphism of graphs with loops is the special case where only
   square {0,1}-matrices are involved. </li>
  </ul>


  \todo Invariants for matrix isomorphism
  <ul>
   <li> Additionally to the distribution of row- and column-sums we should
   involve other invariants for isomorphic matrices. </li>
   <li> We have the rank. </li>
   <li> If matrices A, B are isomorphic, then the square matrices A^t A, B^t B
   are isomorphic, and, since A^t, B^t are isomorphic, also A A^t, B B^t are
   isomorphic (again, as square matrices). So we can use invariants for square
   matrix isomorphism (see below). </li>
   <li> This covers the rank-criterion, since we have rank(A^t A) = rank(A A^t)
   = rank(A). </li>
   <li> The conditions for equal row- and column-sum-distributions are also
   covered by considering the associated square matrices, since their diagonals
   contain the column- and row-sums. </li>
  </ul>


  \todo Invariants for square matrix isomorphism
  <ul>
   <li> DONE
   The characteristic polynomial (compare "hermitian_rank_charpoly" in
   ComputerAlgebra/LinearAlgebra/Lisp/QuadraticForms.mac) should be a good
   invariant test. </li>
   <li> Easier to compute are trace, rank and determinant, which are included
   when computing the characteristic polynomial for square matrices. </li>
   <li> Another invariant is the diagonal as a multiset. Is this covered by the
   characteristic polynomial? In other words, can square matrices with
   non-isomorphic diagonals have the same characteristic polynomial? </li>
  </ul>

*/

