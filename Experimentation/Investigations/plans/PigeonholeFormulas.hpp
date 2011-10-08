// Oliver Kullmann, 24.5.2008 (Swansea)
/* Copyright 2008, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/plans/PigeonholeFormulas.hpp
  \brief Data regarding pigeonhole clause-sets


  \todo Collect the known data.
  <ul>
   <li> ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/OKsolver2002.hpp
   </li>
  </ul>


  \todo Conflict-independence number
  <ul>
   <li> Computed via
   \verbatim
for n : 0 thru 6 do print(n,independence_number_m_cs(weak_php_cs(n+1,n)));
0 1
1 2
2 6
3 18
4 40
5 75
6 126
   \endverbatim
   </li>
   <li> We should be able to figure this out; see below. </li>
  </ul>



  \todo Conflict-partition_number
  <ul>
   <li> The conflict-partition_number as upper-bounded by the length of
   hitting_decomposition_m_cs(weak_php(n+1,n)[2]) seems to be the
   same as the conflict-independence number:
   \verbatim
for n : 0 thru 6 do print(n,partition_number_m_cs(weak_php_cs(n+1,n)));
0 1
1 2
2 6
3 18
4 40
5 75
6 126
   \endverbatim
   </li>
   <li> Perhaps the conflict-graph is even perfect?! </li>
   <li> In general: Is the hermitian deficiency actually an upper bound
   on the conflict-partition-number ?!? </li>
  </ul>


  \todo Hermitian rank
  <ul>
   <li> Data on hermitian rank and deficiency:
   \verbatim
for n : 0 thru 6 do block([F:weak_php_cs(n+1,n),h],h:hermitian_rank_cs(F),print(n,h,ncl_cs(F)-h));
0 0 1
1 1 2
2 3 6
3 4 18
4 5 40
5 6 75
6 7 126
   \endverbatim
   </li>
   <li> We should be able to figure this out:
    <ol>
     <li> hermitian_rank(weak_php(m+1,m)) = m+1 </li>
     <li> hermitian_deficiency and conflict_independence_number follow
     suit. </li>
    </ol>
   </li>
   <li> Analogously to "eigensharp", we can create a notion for clause-sets
   where the conflict-indepence-number equals the hermitian defect, and show
   that weak_php is an instance (while not being eigensharp). </li>
  </ul>


  \todo Characteristic polynomial
  <ul>
   <li> Data:
   \verbatim
for n : 0 thru 6 do print(n, ":",charpoly_cs(weak_php_cs(n+1,n)));
0 : -x
1 : -x^3 + 2*x
2 : -x^9 + 12*x^7 - 36*x^5 + 32*x^3
3 : x^22 - 36*x^20 + 432*x^18 - 2160*x^16 + 3888*x^14
4 : -x^45 + 80*x^43 - 2400*x^41 + 34560*x^39 - 241920*x^37 + 663552*x^35
5 : -x^81 + 150*x^79 - 9000*x^77 + 280000*x^75 - 4800000*x^73 + 43200000*x^71 - 160000000*x^69
6 : -x^133 + 252*x^131 - 26460*x^129 + 1512000*x^127 - 51030000*x^125 + 1020600000*x^123 - 11226600000*x^121 + 52488000000*x^119
   \endverbatim
   </li>
   <li> For n >= 2 there seem to be n+2 monomials, with the powers forming
   an arithmetic progression with distance 2, and coefficients alternating
   in sign. </li>
   <li> These clause-sets seem to have non-zero eigenvalues of the form
   +- a, +- b, so that we altogether get 4 different values here? </li>
  </ul>


  \todo Prime implicates
  <ul>
   <li> The number of prime implicates for satisfiable php would be
   interesting. </li>
  </ul>


  \todo Applying partial assignments
  <ul>
   <li> Minimally unsatisfiable sub-clause-sets of phi * weak_php(m,n) for
   m > n:
    <ol>
     <li> Are these all isomorphic to some weak_php(k+1,k) ? </li>
     <li> No; for example
     \verbatim
L3 : random_splitting_mus(weak_php(7,5)[2],3,dll_simplest_trivial2)$
map(deficiency_cs,L3);
  [51, 47, 43, 39, 36, 32, 29, 26, 24, 20, 8, 7, 5, 3, 1, 1, 1, 1]
     \endverbatim
     but
     \verbatim
for n : 1 thru 5 do print(deficiency_weak_php(n+1,n));
1 3 10 25 51
     \endverbatim
     so there are new deficiencies! </li>
     <li> Which deficiencies can be realised? </li>
     <li> What can be said about the clause-sets obtained? </li>
     <li> Perhaps deficiency 2 cannot be realised? </li>
     <li> What are the types obtained for deficiency 3 ? Experiment:
     \verbatim
def3cls : [];
experiment(m,n) := block(
 [count : 0, F : weak_php(m,n)[2]],
  for seed : 0 do block(
   [L : random_splitting_mus(F,seed,dll_simplest_trivial2), S],
    print("seed:", seed, map(deficiency_cs,L)),
    S : sublist(L,lambda([F],is(deficiency_cs(F) = 3))),
    if not emptyp(S) then (
      S : map(singular_dp_reduction, S),
      def3cls : append(def3cls,S),
      for i : 1 thru length(S) do print(count + 1, ":", S[i]),
      count : count + length(S)
    )))$
experiment(7,5);
     \endverbatim
     Only php(3,2) (up to isomorphism) found for seed <= 8.
     </li>
     <li> To speed it out, and remove trivial cases, now with trivial
     DP-reduction:
def3cls : [];
experiment(m,n) := block(
 [count : 0, F : weak_php(m,n)[2]],
  for seed : 9 do block(
   [L : random_splitting_nsing_mus(F,seed,dll_simplest_trivial2), S],
    print("seed:", seed, map(deficiency_cs,L)),
    S : sublist(L,lambda([F],is(deficiency_cs(F) = 3))),
    if not emptyp(S) then (
      def3cls : append(def3cls,S),
      for i : 1 thru length(S) do print(count + 1, ":", S[i]),
      count : count + length(S)
    )))$
experiment(7,5);
     \endverbatim
     Again, only php(3,2) (up to isomorphism) found for seed <= 20.
     </li>
    </ol>
   </li>
  </ul>

*/

