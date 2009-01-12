// Oliver Kullmann, 8.8.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/CombinatorialMatrices/Lisp/plans/Basics.hpp
  \brief General plans regarding basic notions and functions for (combinatorial) matrices


  \todo Complete the tests


  \todo Organisation
  <ul>
   <li> Split-off several files. </li>
   <li> RandomMatrices.mac </li>
   <li> AlgebraicOperations.mac </li>
   <li> DONE Isomorphisms.mac </li>
  </ul>


  \todo Basic notions
  <ul>
   <li> A "(combinatorial) matrix" is a triple [R,C,f], where R is the set of
   row indices, C the set of column indices, and f is a map with domain
   R x C. </li>
   <li> While a "square matrix" is a pair [I, f], where f is a map with
   domain I x I. </li>
   <li> Square matrices [I,f] can be converted to matrices [I,I,f]. </li>
   <li> A matrix "over M" is a matrix whose values are elements of M. </li>
   <li> So combinatorial matrices over {0,1} correspond to *incidence
   structures*, while combinatorial matrices over {-1,0,+1} correspond
   to labelled boolean clause-sets. </li>
   <li> We also have "ordered matrices" and "ordered square matrices",
   where instead of index-sets we have repetition-free lists. </li>
   <li> Abbreviations:
    <ol>
     <li> "com" for combinatorial matrices </li>
     <li> "scom" for square combinatorial matrices </li>
     <li> "sycom" for symmetric combinatorial matrices: perhaps better not,
     since we only express type information? </li>
     <li> "ocom" for ordered combinatorial matrices </li>
     <li> "oscom" for ordered square combinatorial matrices </li>
     <li> ("osycom" for ordered symmetric combinatorial matrices). </li>
    </ol>
   </li>
   <li> A "standardised combinatorial matrix" (stcom) has index sets
   {1,...,n} for n in NN. </li>
  </ul>


  \todo Conversions
  <ul>
   <li> Reflect on the naming of "lmcom2m, lmscom2m" (compare with
   "Improving the okl-arrays" in
   ComputerAlgebra/DataStructures/Lisp/plans/HashMaps.hpp). </li>
   <li> DONE (it seems using "matrix" is preferable)
   A matrix [R,C,f] can be converted to a Maxima matrix
   <code> genmatrix(lambda([i,j],f(listify(R)[i],listify(C)[j])),
            length(R), length(C)); </code>
   </li>
   <li> A square matrix M=[I,f] is converted into a directed graph dg(M) with
   vertex set I and an directed edge from i to j for i # j iff f(i,j) # 0.
   </li>
   <li> dgl(M) has a loop at entry [i,i] iff f(i,i) # 0. </li>
   <li> DONE
   The general hypergraph hyp(M) for matrix M=[I,J,f] has vertex set I,
   hyperedge set J, and hyperedge j contains vertex i iff f(i,j) # 0. </li>
   <li> The labelled clause-set cls(M) for matrix M=[I,J,f] has variable set I,
   clause-index set J, while clause j contains variable i positively,
   negatively or not iff f(i,j) > 0, < 0, = 0 respectively. </li>
   <li> For a general hypergraph G we can form the vertex-edge incidence matrix
   as well as the edge-vertex incidence matrix. </li>
   <li> For a labelled clause-set F we can form the clause-variable matrix as
   well as the variable-clause matrix. </li>
   <li> For a directed graph (with loops) G we can form the adjacency
   matrix. </li>
  </ul>


  \todo Symmetric matrices
  <ul>
   <li> Maxima has "symmetricp" (in package "ctensor"), but this does not work.
    <ol>
     <li> It seems to belong to the base, but is not defined. </li>
     <li> Then, after load(ctensor), it comes up with an error, showing that
     apparently it is defined differently. </li>
     <li> Make a bug-report. </li>
    </ol>
   </li>
  </ul>


  \todo Duality and polarity
  <ul>
   <li> See ComputerAlgebra/IncidenceStructures/Lisp/plans/ProjectivePlanes.hpp
   for examples/applications. </li>
   <li> See ComputerAlgebra/IncidenceStructures/Lisp/plans/general.hpp. </li>
  </ul>

*/

