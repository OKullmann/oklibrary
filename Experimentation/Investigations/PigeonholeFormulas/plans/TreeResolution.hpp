// Oliver Kullmann, 2.3.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/PigeonholeFormulas/plans/TreeResolution.hpp
  \brief Investigations regarding tree resolution for pigeonhole clause-sets


  \todo Tree-resolution complexity
  <ul>
   <li> Here counting all nodes. </li>
   <li> The standard cases (n+1,n):
   \verbatim
> ssh -t csoliver@cs-wsok.swansea.ac.uk /usr/bin/screen -xRR
> oklib --maxima
for n : 0 thru 3 do print(n,treecomp_refutation_cs(weak_php_cs(n+1,n)));
0 1
1 5
2 21
# aborted after 2 weeks

> screen
> oklib --maxima
oklib_load_all();
oklib_monitor:true;
oklib_monitor_level:1;

T2 : optimal_splitting_tree(weak_php_cs(3,2))$
T2[2];
  21

T3 : optimal_splitting_tree(weak_php_cs(4,3))$
M[optimal_splitting_tree]: depth: 0 , new best variable: php(1,1) , new min size: 85
# This is the optimal tree-resolution complexity, since all variables behave
# equally.
   \endverbatim
   </li>
   <li> The cases (n+2,n):
   \verbatim
for k : 0 thru 2 do print(k, optimal_splitting_tree(weak_php_cs(k+2,k))[2]);
0 1
1 5
2 21
T53 : optimal_splitting_tree_rec(weak_php_cs(5,3),0,85)$
M[optimal_splitting_tree]: depth: 0 , new best variable: php(1,1) , new min size: 85
# This is the optimal tree-resolution complexity, since all variables behave
# equally.
# Computation took about 40 days (using Ecl)
   \endverbatim
   Perhaps (n+2,n) is always the same as (n+1,n) ?
   </li>
   <li> Known bounds for the tree-resolution complexity trc(n) for
   PHP^{n+1}_n (counting the number of nodes in the tree):
    <ol>
     <li> From [Kullmann 1999] one gets hardness = n, and thus
     2^(n+1)-1 <= trc(n) <= 2*(n^2+n+1)^n-1. </li>
     <li> For PHP^m_n (m > n) we get
     2^(n+1)-1 <= trc(n,m) <= 2*(m*n+1)^n-1. </li>
     <li> [Iwama, Miyazaki, 1999] show the stronger bounds
     (n/4)^(n/4) <= trc(n) <= O(n^2 * n!). </li>
     <li> [Dantrchev, Riis, 2001] show more generally
     n^(Omega(n)) <= trc(n,m) <= m^(O(n)), where the upper bounds holds for
     *every* regular tree-resolution refutation. </li>
    </ol>
   </li>
   <li> A formula for comp_R^*(PHP^{n+1}_n):
    <ol>
     <li> Yet we have the sequence 1,5,21,85 (for 0<=n<=3). </li>
     <li> There are four sequences in OEIS, extending it with 341,341,342,421.
     </li>
     <li> The sequence A002450 is (4^n - 1)/3, which would violate the lower
     bound (n/4)^(n/4). </li>
     <li> We should be able to compute comp_R^*(PHP^{4+1}_4) by a C++ version
     of optimal_splitting_tree_rec. </li>
     <li> But perhaps by improving the Maxima-version and using sharper bounds
     we can still do it at Maxima level. </li>
    </ol>
   </li>
  </ul>


  \todo Tree-resolution refutations found by simple algorithms
  <ul>
   <li> Using the simple backtracking algorithms we have yet implemented,
   printing the splitting-tree sizes (nodes and leaves) and the derived
   (pruned) resolution-tree sizes (nodes and leaves):
   \verbatim
rescompphp(_f,N) := for n : 0 thru N do block([F:weak_php_fcs(n+1,n), T, R],
 T : _f(F),
 R : st2reslrt_cs(T,F[2]),
 print(n, nnds_lrt(T), nlvs_lrt(T), nnds_lrt(R), nlvs_lrt(R))
)$

rescompphp(dll_simplest_st_trivial1,4);
0 1 1 1 1
1 5 3 5 3
2 29 15 21 11
3 197 99 97 49
4 1565 783 521 261

rescompphp(dll_simplest_st_trivial2,4);
0 1 1 1 1
1 5 3 5 3
2 29 15 21 11
3 197 99 97 49
4 1565 783 521 261

rescompphp(dll_simplest_st_first_shortest_clause,4);
0 1 1 1 1
1 5 3 5 3
2 23 12 21 11
3 123 62 85 43
4 763 382 377 189

rescompphp(dll_simplest_st_max_var,4);
0 1 1 1 1
1 5 3 5 3
2 21 11 21 11
3 97 49 97 49
4 521 261 521 261
   \endverbatim
   </li>
   <li> We see that the heuristics "first_shortest_clause" after tree-pruning
   finds the known best refutations (for 0 <= n <= 3; while for n=3 none of
   the others does so). </li>
  </ul>


  \todo Short proofs with the hitting proof-system
  <ul>
   <li> See Satisfiability/Lisp/ConflictCombinatorics/HittingProofSystem.mac
   for the basic notions. </li>
   <li> Extracting hitting refutations by stcs2hitref from splitting trees:
   \verbatim
hitcompphp(_f,N) := for n : 0 thru N do
 block([F:weak_php_fcs(n+1,n), T1, T2, H1, H2],
  T1 : _f(F),
  F : F[2],
  T2 : treepruning_st(T1,F),
  H1 : stcs2hitref0(F,T1),
  H2 : stcs2hitref0(F,T2),
  print(n, nlvs_lrt(T2), ncl_cs(H1), ncl_cs(H2))
)$

hitcompphp(dll_simplest_st_trivial1,4);
0 1 1 1
1 3 3 3
2 11 12 11
3 49 77 49
4 261 638 261

hitcompphp(dll_simplest_st_trivial2,4);
0 1 1 1
1 3 3 3
2 11 12 11
3 49 77 49
4 261 638 261

hitcompphp(dll_simplest_st_first_shortest_clause,4);
0 1 1 1
1 3 3 3
2 11 12 11
3 43 59 43
4 189 366 189

hitcompphp(dll_simplest_st_max_var,4);
0 1 1 1
1 3 3 3
2 11 11 11
3 49 49 49
4 261 261 261
   \endverbatim
   </li>
   <li> Only the tree-hitting refutations were found. </li>
   <li> The above used the obvious heuristics of choosing smaller clauses if
   there is a choose (for constructing the hitting set); now the oppositive,
   choosing always larger clauses:
   \verbatim
hitcompphp2(_f,N) := for n : 0 thru N do
 block([F:weak_php_fcs(n+1,n), T1, T2, H1, H2],
  T1 : _f(F),
  F : F[2],
  T2 : treepruning_st(T1,F),
  H1 : stcs2hitref1(F,T1),
  H2 : stcs2hitref1(F,T2),
  print(n, nlvs_lrt(T2), ncl_cs(H1), ncl_cs(H2))
)$

hitcompphp2(dll_simplest_st_trivial1,4);
hitcompphp2(dll_simplest_st_trivial2,4);
hitcompphp2(dll_simplest_st_first_shortest_clause,4);
hitcompphp2(dll_simplest_st_max_var,4);
   \endverbatim
   No difference to above. </li>
  </ul>

*/
