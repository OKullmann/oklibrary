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
   <li> Results mentioned in the paper:
    <ol>
     <li> SOLS(n) as well as SODLS(n) is non-empty iff n notin {2,3,6}. </li>
     <li> SSSODLS(n) is non-empty iff n cong 0,1,3 mod 4 and n#3. </li>
     <li> For n cong 1,5 mod 6, n >= 5, PSSSODLS(n) is not empty. </li>
    </ol>
    All these constructions should be implemented at the Maxima-level (see
    below). </li>
   </li>
   <li> Regarding the non-emptiness of PSSSODLS(n) the following is known:
    <ol>
     <li> n=1 is non-empty. </li>
     <li> There are no SOLS for n in {2,3} and thus PSSSODLS(n) is empty here.
     </li>
     <li> PSSSODLS(4) is empty ("easy to see"). </li>
     <li> PSSSODLS(5) non-empty by above general result. </li>
     <li> PSSSODLS(6) empty, since SOLS(6) empty. </li>
     <li> PSSSODLS(7) non-empty by above general result. </li>
     <li> PSSSODLS(8) non-empty; the example from the paper for an element
     is available as psssodls_8_ls (in
     ComputerAlgebra/CombinatorialMatrices/Lisp/LatinSquares/BasicNotions.mac).
     </li>
     <li> PSSSODLS(n) non-empty for n cong 0 mod 4, n >= 8, n notin {12,24},
     as shown in Theorem 1.5 the paper. </li>
    </ol>
   </li>
   <li> Constructions in the paper (apparently m,n > 1):
    <ol>
     <li> Given A in PSSSODLS(m) and B in PSSSODLS(n), there is C in
     PSSSODLS(m*n) (Construction 2.2). </li>
     <li> Given A in SSSODLS(n), there is B in PSSSODLS(8*n)
     (Construction 2.3). </li>
     <li> Given A in SSSODLS(n), there is B in PSSSODLS(4*n)
     (Construction 2.5). </li>
    </ol>
   </li>
   <li> Since our indices are 1-based, we use the standard matrix-indices,
   starting with 1, and also the values of latin squares are 1-based. </li>
   <li> Via psssodls_p(A) we can check whether matrix A is a PSSSODLS. </li>
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


  \todo SAT translations
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Generators/LatinSquares.mac
   for the framework. </li>
   <li> And see "Different encodings" and "Further conditions" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/LatinSquares.hpp
   for the implementation of SAT translations. </li>
   <li> We want to construct formulas Fpsssodls_n, which are satisfiable iff
   PSSSODLS(n) is not empty, and where from a solution an element is
   constructible. </li>
   <li> Handling the non-boolean variables:
    <ol>
     <li> Given the current availability of tools and methods, it seems easiest
     for now to jump directly to the boolean level, specifying some reasonable
     encodings. </li>
     <li> That is, we consider some t_i(n) <= VA, which are sets of boolean
     variables, together with a map ip_i ("interpretation"), which maps
     total assignments over V_n to elements of PSSSODLS(n). </li>
     <li> The "formulas" (clause-sets with possible extensions, like
     pseudo-boolean conditions) F_i(n) have var(F_i(n)) <= t_i(n), where
     F_i(n) is satisfiable iff PSSSODLS(n) is non-empty, and where for a total
     satisfying assignment via ip_i we get a solution. </li>
     <li> I (OK) see three possible t_i (t_1,t_2,t_3):
      <ol>
       <li> t_1 just uses the so-called direct encoding, i.e., the variables
       ls(i,j,k), already used in
       ComputerAlgebra/Satisfiability/Lisp/Generators/LatinSquares.mac. </li>
       <li> t_2 uses the unary encoding, that is, variables uls(i,j,k) for
       field (i,j) and 1 <= k <= n-1, where value 1 <= p <= n is encoded by
       uls(i,j,k) being true for exactly k < p. This is unary when considering
       the actual value as p-1, i.e., 0-based. And we have the property
       that if uls(i,j,k) is true, then also for all k' < k, and if it's
       false, then also for all k' > k. </li>
       <li> t_3 uses both variable-types at the same time, with the linking
         ls(i,j,k) <-> (not uls(i,j,k) and uls(i,j,k-1))
       (where for k=1 the undefined term "uls(i,j,k-1)" is not there). </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> According to our general approach for "good translations", the target
   is to achieve a hardness as low as possible (not using "too many" clauses).
   </li>
   <li> This is applied (currently) to the components of the translation, where
   the components should be as "big as possible". </li>
   <li> Bijectivity-constraints:
    <ol>
     <li> The latin-square conditions as well as the diagonality-condition
     are bijectivity-conditions (in the affected lines all values are used
     precisely once). </li>
     <li> We need special tools for handling such conditions. </li>
     <li> How strong is the dependency on the encoding (direct versus unary)?
     </li>
     <li> "Orthogonal" bijectivity-conditions interfere -- what can be said
     w.r.t. the prime implicates? </li>
    </ol>
   </li>
   <li> Strong symmetry:
    <ol>
     <li> For appropriate pair of values x, y we have x+y=n-1 (using 0-based
     values). </li>
     <li> For both encodings this can be expressed directly (without using some
     form of addition). </li>
    </ol>
   </li>
   <li> Self-orthogonality:
    <ol>
     <li> This can be expressed in general as the conditions that for
     different cells (i,j), (i',j') with equal values the transposed cells
     (j,i), (j',i') must have different values. </li>
     <li> For arbitrary square matrices we have here just (i,j)#(i',j'). </li>
     <li> However for latin squares this can be strengthened to i#i' *and*
     j#j'. </li>
    </ol>
   </li>
   <li> Pan-diagonality:
    <ol>
     <li> This can be expressed by cardinality-conditions using the unary
     encoding. </li>
    </ol>
   </li>
   <li> We have thus four blocks of conditions (bijectivity, strong symmetry,
   self-orthogonality, pan-diagonality): Are there interesting combinations
   of these components (where we have good representations)? </li>
  </ul>


  \todo Further properties
  <ul>
   <li> The example psssodls_9_ls is also a Sudoku (as realised by MH). </li>
   <li> Are there other psssodls of this dimension which are also Sudoku?
    <ol>
     <li> What are the symmetries of the problem (being a psssodls and a
     Sudoku)? </li>
     <li> We can replace numbers i by 10-i. </li>
     <li> And we can transpose the matrix. </li>
     <li> Is that it? </li>
     <li> By these operations we get altogether 4 variations of psssodls_9_ls.
     </li>
    </ol>
   </li>
   <li> Are there also psssodls of dimension 9 which are not Sudoku? </li>
   <li> And what about different dimensions? </li>
  </ul>

*/
