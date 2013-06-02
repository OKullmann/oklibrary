// Oliver Kullmann, 24.5.2008 (Swansea)
/* Copyright 2008, 2011, 2012, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/PigeonholeFormulas/plans/general.hpp
  \brief Investigations regarding pigeonhole clause-sets


  \todo Prime implicates
  <ul>
   <li> The number and the structure of prime implicates for satisfiable php
   is needed. </li>
   <li> Simple computation via resolution closure:
   \verbatim
for n : 0 thru 4 do print(n, length(min_resolution_closure_cs(weak_php_cs(n,n))[1]));
0 0
1 1
2 12
3 87
XXX
   \endverbatim
   </li>
   <li> Alternative method: Expanding to canonical CNF and use Quine/McCluskey:
   \verbatim
# First the size of the canonical CNFs (and the total number of full clauses):
for m : 0 thru 4 do print(m,ncl_fcs(expand_fcs(weak_php_fcs(m,m))),2^(m*m));
0 0 1
1 1 2
2 14 16
3 506 512
4 65512 65536

# Note that the number of full clauses of PHP^m_m is 2^(m*m) - m!.

for m : 0 thru 4 do block([FF:standardise_fcs(expand_fcs(weak_php_fcs(m,m)))],
  output_fcs_v(
    sconcat("Canonical CNF for the pigeonhole principle (weak form) with ", m, " pigeons."),
    FF[1], sconcat("PHP_CNF_",m,".cnf"), FF[2]))$

for ((n=1;n<=4;++n)); do echo ${n}; QuineMcCluskey-n16-O3-DNDEBUG PHP_CNF_${n}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG sd nz; done
0
     n    c     l
     0    0     0
1
     n    c        l
     1    1        1
 length   count
      1       1
2
     n    c        l
     4    12       24
 length   count
      2      12
3
     n     c        l
     9    87      252
 length   count
      2      18
      3      60
      4       9
4
      n      c        l
     16    728     3056
 length   count
      2      48
      4     488
      5     144
      6      48
   \endverbatim
   </li>
   <li> We remark that PHP^m_m has exactly one DNF representation (without
   new variables), since the total satisfying assignments correspond 1-1 to
   the m! permutations of the m pigeons, and we have only these total
   satisfying assignments (a boolean function f has exactly one DNF
   representation without new variables iff f has only total satisfying
   assignments). </li>
  </ul>


  \todo Applying partial assignments
  <ul>
   <li> Minimally unsatisfiable sub-clause-sets of phi * weak_php_cs(m,n) for
   m > n:
    <ol>
     <li> Are these all isomorphic to some weak_php_fcs(k+1,k) ? </li>
     <li> No; for example
     \verbatim
L3 : random_splitting_mus(weak_php_cs(7,5),3,dll_simplest_trivial2)$
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
 [count : 0, F : weak_php_cs(m,n)],
  for seed : 0 do block(
   [L : random_splitting_mus(F,seed,dll_simplest_trivial2), S],
    print("seed:", seed, map(deficiency_cs,L)),
    S : sublist(L,lambda([F],is(deficiency_cs(F) = 3))),
    if not emptyp(S) then (
      S : map(sdp_reduction_cs, S),
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
 [count : 0, F : weak_php_cs(m,n)],
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


  \todo MU structure
  <ul>
   <li> What about the saturation structure of PHP_n (= PHP^{n+1}_n):
    <ol>
     <li> First just considering whether we have saturatedness:
     \verbatim
for n : 0 thru 4 do print(n, saturated_min_unsat_bydef_fcs(weak_php_fcs(n+1,n)));
0 true
1 false
2 false
3 false
4 false
     \endverbatim
     </li>
     <li> The sets of partial assignments not resulting in a MU:
     \verbatim
# do we get all elementary partial assignments?:
for n : 0 thru 4 do print(n, length(non_saturating_pas_bydef_fcs(weak_php_fcs(n+1,n))), 2*nvar_php(n+1,n));
0 0 0
1 2 4
2 12 12
3 12 24
4 20 40
     \endverbatim
     So apparently except for n=2 we get exactly half of the elementary partial
     assignments; these seem to be exactly the positive assignments, except
     for n=1,2:
     \verbatim
for n : 0 thru 4 do block([F:weak_php_fcs(n+1,n)], print(n, is(non_saturating_pas_bydef_fcs(F) = singletons(F[1]))));
0 true
1 false
2 false
3 true
4 true
non_saturating_pas_bydef_fcs(weak_php_fcs(2,1));
  {{-php(1,1)},{-php(2,1)}}
     \endverbatim
     </li>
     <li> Clear that after setting a variable to true we get a non-MU, since
     if we put pigeon i into hole j, then the 2-clauses saying that pigeon
     pigeons k1#k2 can not go into j at the same time for i notin {k1,k2}
     become superfluous. </li>
    </ol>
   </li>
   <li> What about the marginality of PHP_n (= PHP^{n+1}_n):
   \verbatim
for n : 0 thru 4 do print(n, marginal_min_unsat_bydef_fcs(weak_php_fcs(n+1,n)));
0 true
1 true
2 true
3 true
4 true
   \endverbatim
   Rather obviously all PHP_n are marginal. </li>
  </ul>

*/
