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
   isomorphic (again, as square matrices).
    <ol>
     <li> So we can use invariants for square matrix isomorphism (see below).
     </li>
     <li> This covers the rank-criterion, since we have rank(A^t A) =
     rank(A A^t) = rank(A). </li>
     <li> The conditions for equal row- and column-sum-distributions are also
     covered by considering the associated square matrices, since their
     diagonals contain the column- and row-sums. </li>
     <li> The matrices A^t A and A A^t (as well as B^t B and B B^t) have
     identical characteristic polynomials, so they don't need to be computed
     twice. </li>
    </ol>
   </li>
   <li> But the value-distributions for A, B are not covered. </li>
   <li> A polytime computable invariant in case of externally square matrices
   is whether the matrices are fully indecomposable. </li>
  </ul>


  \todo Invariants for square matrix isomorphism
  <ul>
   <li> DONE
   The characteristic polynomial (compare "hermitian_rank_charpoly" in
   ComputerAlgebra/LinearAlgebra/Lisp/QuadraticForms.mac) should be a good
   invariant test. </li>
   <li> Easier to compute are trace, rank and determinant, which are included
   when computing the characteristic polynomial for square matrices. </li>
   <li> Another invariant is the main diagonal as a multiset.
    <ol>
     <li> This is not covered by the characteristic polynomial, as the
     matrices [[2,0],[0,0]] and [[1,1],[1,1]] show which have both the
     characteristic polynomial x^2 - 2x. </li>
    </ol>
   </li>
   <li> Then we have the invariants for (general) matrices:
    <ol>
     <li> row-sum distribution </li>
     <li> column-sum distribution </li>
     <li> value-distribution. </li>
    </ol>
    As the above example shows, none of these invariants is covered by the
    characteristic polynomial.
   </li>
   <li> Using scom2dgl, we can compute invariants for the associated
   directed graphs (with loops):
    <ol>
     <li> The number of loops. </li>
     <li> Strong connectedness (corresponding to "irreducibility"). </li>
    </ol>
   </li>
  </ul>


  \todo Self-duality
  <ul>
   <li> Compared to the standard checks for isomorphic matrices (see
   "Invariants for square matrix isomorphism" above), many tests can
   be saved:
    <ol>
     <li> As mentioned above, the characteristic polynomials of A^t A
     and A A^t are necessarily identical, so this test can be saved at all.
     </li>
     <li> Other tests only need to be performed "half". </li>
    </ol>
   </li>
  </ul>

*/

