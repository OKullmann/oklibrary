// Oliver Kullmann, 23.5.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/CombinatorialMatrices/Lisp/plans/general.hpp
  \brief General plans regarding combinatorial matrix theory


  \todo Write tests


  \todo Random invertible matrix
  <ul>
   <li> We need a function which given the matrix dimensions,
   the elements of the field, and functions for the field operations
   yields a random matrix over this field. </li>
   <li> A simple algorithm which picks a random non-zero row and then
   computes the linear hull, and next picks only rows not in the
   linear hull, and so on, should work. </li>
   <li> See 
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp. </li>
  </ul>


  \todo Memoisation
  <ul>
   <li> A "completely lazy" function f in a com [A,B,f] recomputes
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
     has to use global objects.
      <ol>
       <li> On the other hand, this cannot be the case, since otherwise
       we would have *two* hash-maps in f1, which wouldn't work! </li>
       <li> So apparently f1 has a reference to a global object. </li>
       <li> And only when printing out f1, then this global object is
       actually printed. </li>
       <li> One needs to understand CLisp better. </li>
      </ol>
     </li>
     <li> On the other hand,
     \verbatim
f2 : buildq([a : make_array(fixnum,1000)],
       lambda([n], block([v : a[n-1]], 
         if v=0 then a[n-1] : bfloat(log((n-1)!)) else v)))$
     \endverbatim
     doesn't work, apparently due to evaluation problems with the
     assignment. </li>
    </ol>
   </li>
   <li> We should set-up some framework for defining such functions easily.
   </li>
   <li> Should for example the conflict matrix of a clause-set always be
   computed in this way? </li>
   <li> See "Lazy combinatorial matrices" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Hypergraphs.hpp,
   and see "Memoisation for general graphs and multigraphs" in
   ComputerAlgebra/Graphs/Lisp/plans/general.hpp). </li>
  </ul>


  \todo Preprocessing combinatorial matrices
  <ul>
   <li> Given a combinatorial matrix M, we need a facility to get M'
   out of it, which as combinatorial matrix is equal to M, but has all
   values precomputed. </li>
   <li> How to call it? "preprocess(M)" ?? </li>
   <li> For this we can use (similar to above, using buildq to inscribe
   the fixed term into the lambda-term) Maxima-matrices, arrays or
   hash-maps. </li>
   <li> Since arrays and matrices both restrict the indices to integers,
   the only difference is given by access speed:
    <ol>
     <li> One would guess that arrays should be faster. </li>
     <li> Actually, access speed seems to be the same, but the
     zero-initinalised array is created a lost faster than a matrix. </li>
     <li> On the other hand, if we want to convert the combinatorial matrix
     into a Maxima-matrix, then we have it already. </li>
     <li> And the zero-initialisation isn't that useful in most situations,
     where we need to compute the values anyway one by one. </li>
     <li> So we should use Maxima-matrices inside. </li>
     <li> How can the conversion to Maxima-matrices take advantage that already
     the Maxima-matrix is given inside:
      <ol>
       <li> According to our general strategy, that objects "are themselves",
       one could simply provide two versions for the conversion, and leave it
       to the user to decide. </li>
       <li> This seems to be the only possibility, if we don't want to adorne
       the definition of combinatorial matrices with all kinds of attributes
       --- and this we don't want to do. </li>
       <li> Still the problem: How to extract the matrix, which is hidden
       inside? </li>
       <li> Considering f as a term, and relying on a standard form, we can
       just extract the term:
       \verbatim
f1 : buildq([m : matrix([1,2],[3,4])], lambda([i,j],m[i,j]));
f1(1,2);
  2
part(f1,2);
  matrix([1,2],[3,4])[i,j]
       </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Likely we can also produce the other variant, using memoisation
   as discussed above. </li>
   <li> Perhaps then we use better "processed_eager(M)" and "processed_lazy(M)"
   ? </li>
  </ul>


  \todo Organisation
  <ul>
   <li> The topic of "biclique partitions" of combinatorial matrices should
   get its own module. </li>
   <li> What's an appropriate name? Perhaps "SumDecompositions"? </li>
  </ul>


  \todo Biclique partitions for combinatorial matrices
  <ul>
   <li> See ComputerAlgebra/Graphs/Lisp/plans/BicliquePartitions.hpp.
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
     <li> Importantly, for each such submatrix row- and column-sets must
     be disjoint. </li>
     <li> Since rows and columns have names, such a sum has a natural
     meaning. </li>
     <li> We should implement such a "submatrix_sum". </li>
     <li> However, here we do not have submatrices, since the entries
     are different?! </li>
     <li> Perhaps speaking of "embedded matrices" and "canonically embedded
     matrices" ? </li>
     <li> Or something more special, if the entries are just the
     {-1,0,+1}-versions of the original entries? A "sign-submatrix" ?! </li>
     <li> Important also to notice that to capture biclique partitions of
     multi-*graphs* we cannot use a sum of 1-complete submatrices, but
     we need to make these matrices symmetric. </li>
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

