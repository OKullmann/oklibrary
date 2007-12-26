// Oliver Kullmann, 26.12.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/CombinatorialMatrices/plans/general.hpp
  \brief General plans regarding combinatorial matrix theory (in computer algebra system)


  \todo Basic operations
  <ul>
   <li> A "(combinatorial) matrix" is a triple [R,C,f], where R is the set of
   row indices, C the set of column indices, and f is a map with domain
   R x C. </li>
   <li> While a "square matrix" is a pair [I, f], where f is a map with
   domain I x I. </li>
   <li> Square matrices [I,f] can be converted to matrices [I,I,f]. </li>
   <li> Or should f be an array function? </li>
   <li> A matrix "over M" is a matrix whose values are elements of M. </li>
   <li> So combinatorial matrices over {0,1} are exactly the *incidence
   structures*, while combinatorial matrices over {-1,0,+1} are exactly
   the general boolean clause-sets. </li>
   <li> A matrix [R,C,f] can be converted to a Maxima matrix
   <code> genmatrix(lambda([i,j],f(listify(R)[i],listify(C)[j])),
            length(R), length(C)); </code>
   </li>
   <li> A square matrix M=[I,f] is converted into a directed graph dg(M) with
   vertex set I and an directed edge from i to j for i # j iff f(i,j) # 0. </li>
   <li> dgl(M) has a loop at entry [i,i] iff f(i,i) # 0. </li>
   <li> The general hypergraph hyp(M) for matrix M=[I,J,f] has vertex set I,
   hyperedge set J, and hyperedge j contains vertex i iff f(i,j) # 0. </li>
   <li> The general clause-set cls(M) for matrix M=[I,J,f] has variable set I,
   clause-index set J, while clause j contains variable i positively, negatively
   or not iff f(i,j) > 0, < 0, = 0 respectively. </li>
   <li> For a general hypergraph G we can form the vertex-edge incidence matrix
   as well as the edge-vertex incidence matrix. </li>
   <li> For a general clause-set F we can form the clause-variable matrix as
   well as the variable-clause matrix. </li>
   <li> For a directed graph (with loops) G we can form the adjacency
   matrix. </li>
  </ul>

*/

