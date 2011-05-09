// Oliver Kullmann, 8.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/LatinSquares/plans/SelfOrthogonal.hpp
  \brief On investigations regarding self-orthogonal latin squares


  \todo Searching pandiagonal latin squares
  <ul>
   <li> According to [Constructions for pandiagonal strongly symmetric
   self-orthogonal diagonal Latin squares, Chen, Li, Zhang]. </li>
   <li> There PSSSODLS(n) is defined, which we treat as the set of all
   "pandiagonal strongly symmetric self-orthogonal diagonal Latin squares of
   order n":
    <ol>
     <li> "Diagonal" means that diagonal and "back-diagonal" (from right-top
     to left-bottom) contains all numbers. </li>
     <li> "Self-orthogonal": the matrix is orthogonal (see above) to its
     transposed. </li>
     <li> The set of all self-orthogonal diagonal Latin squares of order n is
     SODLS(n). </li>
     <li> "Strongly symmetric" means we have l_{i,j} + l_{n-1-i,n-1-j} = n-1,
     using indices 0,...,n-1 for the square l. </li>
     <li> This additional condition yields SSSODLS(n). </li>
     <li> "Pandiagonal" means if for each w in {0,1...,n-1}, using arithmetic
     modulo n in the indices, we have sum_{i=0}^{n-1} l_{i,i+w} =
     sum{i=0}^{n-1} l_{i,w-i} = n*(n-1)/2. </li>
    </ol>
   </li>
   <li> Problem 4.2 in the paper asks whether PSSSODLS(n) is non-empty
   for n=12,24 or n congruent 3 (mod 6) (where n >= 9). </li>
   <li> The example from the paper for an element of PSSSODLS(8) is available
   as psssodls_8_ls (in
   ComputerAlgebra/CombinatorialMatrices/Lisp/LatinSquares/BasicNotions.mac).
   </li>
   <li> Since our indices are 1-based, we use the standard matrix-indices,
   starting with 1, and also the values of latin squares are 1-based. </li>
   <li> See okltest_psssodls_8_ls in
   ComputerAlgebra/CombinatorialMatrices/Lisp/LatinSquares/tests/BasicNotions.mac
   for how to check the defining properties. </li>
   <li> In [Discrete Mathematics using Latin Squares, Laywine, Mullen] the
   term "pandiagonal" is a strengthening of "diagonal", that is, for all
   pandiagonals all values must be different. This is stronger than the
   above notion of "pandiagonal", which only requires the sum of the entries
   in the pandiagonals to behave as if all values would be different. </li>
   <li> In
   ComputerAlgebra/CombinatorialMatrices/Lisp/LatinSquares/BasicNotions.mac
   we use "strictly pandiagonal" and "pandiagonal" (this in the above sense).
   </li>
  </ul>

*/
