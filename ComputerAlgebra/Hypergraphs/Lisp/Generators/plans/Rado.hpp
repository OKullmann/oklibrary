// Oliver Kullmann, 12.7.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/Generators/plans/Rado.hpp
  \brief Plans regarding hypergraphs generators related to Rado's theorems


  \todo Connections
  <ul>
   <li> See
   Experimentation/Investigations/RamseyTheory/RadoProblems/plans/general.hpp.
   </li>
   <li> The Schur-hypergraphs are special cases, employing linear inequalities
   x_1 + ... + x_{k-1} = x_k. </li>
   <li> Also the Folkman-hypergraphs are special cases. </li>
  </ul>


  \todo Basic functionality
  <ul>
   <li> Implement rado_ohg.
    <ol>
     <li> First a basis of the solution space of Ax=0 has to be computed. </li>
     <li> Then all solutions can be enumerated, and the hyperedges extracted.
     </li>
     <li> Depending on the symmetries of the solution space w.r.t. set
     formation, the same hyperedge might be enumerated many times. Seems hard
     to avoid in general. </li>
    </ol>
   </li>
   <li> Implement radoi_ohg.
    <ol>
     <li> Is there a better solution than first computing rado_ohg, and then
     keeping only the hyperedges of length p (wherer A is an rxp matrix)?
     </li>
    </ol>
   </li>
  </ul>


  \todo Checking regularity
  <ul>
   <li> A Rado parameter tuple [A_1, ..., A_r] is called "regular" for if every
   r-partitioning of NN there is a part P_i containing at least one hyperedge
   from rado_ohg(A_i,n) for suitable n. </li>
   <li> As usual, via compactness this is equivalent to the condition that
   if n is large enough, then the corresponding (non-boolean) clause-set
   becomes unsatisfiable; in other words, rado_m(A_1, ..., A_m) exists. </li>
   <li> Let's use "injectively regular" for the existence of
   radoi_m(A_1, ..., A_m). </li>
   <li> In [Ramsey Theory on the Integers; Landman, Robertson] we find
   the following criterions for (injective) regularity:
    <ol>
     <li> For m=1 and r=1 (just a single equation) with all coefficients
     being non-zero this is Theorem 9.2 (there is a non-empty subset of
     coefficients summing to zero). </li>
     <li> While injective regularity is characterised by Theorem 9.4
     (additionally a solution x of A x = 0 must exist, where x is injective
     (but the entries can be arbitrary integers). </li>
     <li> Allowing more than one single equation is easy by Theorem 9.19 resp.
     9.22: simply each single equation must be regular resp. injectively
     regular. </li>
     <li> Finally having a single matrix is handled by Theorem 9.27. </li>
    </ol>
   </li>
   <li> These conditions need to be implemented. </li>
  </ul>


  \todo Known Rado numbers
  <ul>
   <li> By Theorem 9.11 in [Landman, Robertson] rado_2(matrix([a,b,-b]))
   is known; does this yield some interesting problem instances? </li>
   <li> Section 9.2 in [Landman, Robertson] contains more cases; these
   should be implemented. </li>
  </ul>

*/

