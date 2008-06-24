// Oliver Kullmann, 23.5.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/CombinatorialMatrices/Lisp/plans/general.hpp
  \brief General plans regarding combinatorial matrix theory


  \todo Write tests


  \todo Basic notions
  <ul>
   <li> A "(combinatorial) matrix" is a triple [R,C,f], where R is the set of
   row indices, C the set of column indices, and f is a map with domain
   R x C. </li>
   <li> While a "square matrix" is a pair [I, f], where f is a map with
   domain I x I. </li>
   <li> Square matrices [I,f] can be converted to matrices [I,I,f]. </li>
   <li> A matrix "over M" is a matrix whose values are elements of M. </li>
   <li> So combinatorial matrices over {0,1} are exactly the *incidence
   structures*, while combinatorial matrices over {-1,0,+1} are exactly
   the general boolean clause-sets. </li>
   <li> We also have "ordered matrices" and "ordered square matrices",
   where instead of index-sets we have repetition-free lists. </li>
   <li> Abbreviations:
    <ol>
     <li> "cm" for combinatorial matrices </li>
     <li> "scm" for square combinatorial matrices </li>
     <li> "sycm" for symmetric combinatorial matrices </li>
     <li> "ocm" for ordered combinatorial matrices </li>
     <li> "oscm" for ordered square combinatorial matrices </li>
     <li> "osycm" for ordered symmetric combinatorial matrices. </li>
    </ol>
   </li>
  </ul>


  \todo Memoisation
  <ul>
   <li> A "completely lazy" function f in a cm [A,B,f] recomputes
   its values every time again. </li>
   <li> For example for conflict matrices for clause-sets this creates
   a big overhead. </li>
   <li> In the following way we can implement "lazy" functions f, which
   compute the value only once, and then keep the result:
    <ol>
     <li> One possibility is that f(i,j) just calls some f2[i,j]; this
     f2 would then necessarily be a global function. </li>
     <li> Hash-maps can be built into function terms:
     \verbatim
f1 : buildq([hm : sm2hm({[1,77]})],
       lambda([n], block([v : ev_hm(hm,n)], 
         if v=false then set_hm(hm,n,bfloat(log(n!))) else v)));
     \endverbatim
     is a fine memoised function. </li>
     <li> One has to keep in mind that the table is built into f1;
     if this is not wished then (as with using array-functions) one
     has to use global objects. </li>
     <li> On the other hand,
     \verbatim
f2 : buildq([a : make_array(fixnum,1000)],
       lambda([n], block([v : a[n-1]], 
         if v=0 then a[n-1] : bfloat(log((n-1)!)) else v)))$
     \endverbatim
     doesn't work, apparently due to evaluation problems. </li>
    </ol>
   </li>
   <li> We should set-up some framework for defining such functions easily.
   </li>
   <li> Should for example the conflict matrix of a clause-set always be
   computed in this way? </li>
  </ul>


  \todo Conversions
  <ul>
   <li> A matrix [R,C,f] can be converted to a Maxima matrix
   <code> genmatrix(lambda([i,j],f(listify(R)[i],listify(C)[j])),
            length(R), length(C)); </code>
   </li>
   <li> A square matrix M=[I,f] is converted into a directed graph dg(M) with
   vertex set I and an directed edge from i to j for i # j iff f(i,j) # 0.
   </li>
   <li> dgl(M) has a loop at entry [i,i] iff f(i,i) # 0. </li>
   <li> The general hypergraph hyp(M) for matrix M=[I,J,f] has vertex set I,
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


  \todo Biclique partitions for combinatorial matrices
  <ul>
   <li> See also ComputerAlgebra/Graphs/Lisp/plans/BicliquePartitions.hpp.
   </li>
   <li> A combinatorial matrix is "complete" if no entry is zero.
    <ol>
     <li> Write predicate "completecomp". </li>
     <li> Perhaps "1-complete" if all entries are 1 ? </li>
     <li> Perhaps we call a matrix "constant", "1-constant" etc. </li>
     <li> Then 1-complete com's would be 1-constant com's. </li>
    </ol>
   </li>
   <li> A "multi-biclique" corresponds to a complete sub-matrix of a
   combinatorial matrix:
    <ol>
     <li> A "sub-matrix" of a combinatorial matrix is given by a subset
     of the row-indices and a subset of the column-indices. </li>
     <li> We allow overlaps of row and column indices: In the multigraph
     application such an overlap is impossible since loops are absent. </li>
     <li> We allow arbitrary non-zero entries: If all entries are 1, then
     we speak of a "biclique". </li>
     <li> Perhaps we should better just speak of "complete submatrices"
     and "1-complete submatrices". </li>
    </ol>
   </li>
   <li> A biclique-partitioning corresponds to a representation of the
   matrix as a sum of 1-complete submatrices:
    <ol>
     <li> Since rows and columns have names, such a sum has a natural
     meaning. </li>
     <li> We should implement such a "submatrix_sum". </li>
     <li> However, here we do not have submatrices, since the entries
     are different?! </li>
     <li> Perhaps speaking of "embedded matrices" and "canonically embedded
     matrices" ? </li>
     <li> Or something more special, if the entries are just the
     {-1,0,+1}-versions of the original entries? A "sign-submatrix" ?! </li>
    </ol>
   </li>
  </ul>


  \todo Finding a maximal complete submatrix
  <ul>
   <li> Via controlling the order on rows on columns we should be able
   to get "just some" or a "random" such submatrix. </li>
   <li> Likely we do not need this for square matrices. </li>
   <li> The algorithm is as follows:
    <ol>
     <li> Maintain row-set A and column-set B (initially empty). </li>
     <li> And maintain extA, extB for the sets of rows and columns (i.e.,
     indices) which can be added (initially all rows resp. columns). </li>
     <li> For the next extension (either extending A or B) we need to
     randomly choose between "(A,B)" and "(B,A)", that is, first try
     to extend A, if not possible, then try B, or the other way around. </li>
     <li> According to the given order we then choose an element of extA
     resp. extB, add it to A resp. B, and remove this element from extA/B
     as well as removing all now disabled extensions. </li>
     <li> Perhaps for the choice between "(A,B)" and "(B,A)" we use
     a function supplied as parameter. </li>
    </ol>
   </li>
  </ul>


  \todo Finding a "biclique partition"
  <ul>
   <li> I.e., representing a combinatorial matrix as a sum of
   1-complete sign-submatrices. </li>
   <li> Again, the choice between "just some" and a "random" partition
   should be governed by the order-decisions. </li>
   <li> The basic algorithm is:
    <ol>
     <li> Choose some maximal complete sign-submatrix (see above). </li>
     <li> Pass this to some parameter-function which chooses a sub-matrix.
     </li>
     <li> Substract this sub-matrix, and repeat the loop. </li>
    </ol>
   </li>
   <li> Interesting graphs to partition are:
    <ol>
     <li> The complete graphs (corresponding to 1-regular
     hitting clause-sets). </li>
     <li> Complete bipartite graphs (corresponding to "exact"
     bihitting clause-sets). </li>
     <li> The unsatisfiable cases basically yield all minimally unsatisfiable
     clause-sets which are 1-regular hitting (i.e., SMUSAT(1)) resp.
     which have hermitian rank 1. So the latter is of special interest,
     due to the conjecture of [Galesi, Kullmann] about their structure.  </li>
     <li> See "The conjecture of [Galesi, Kullmann]" in
     ComputerAlgebra/Graphs/Lisp/plans/BicliquePartitions.hpp. </li>
    </ol>
   </li>
  </ul>

*/

