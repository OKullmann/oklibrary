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
     <li> But the computations below indicate that 377 is the right
     continutation; once we have figured it out, we should submit it to OEIS.
     </li>
     <li> DONE
     The sequence A002450 is (4^n - 1)/3, which would violate the lower
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
   (pruned) resolution-tree sizes (nodes and leaves), plus the sizes of
   the derived resolution-refutations (counting only different clauses):
   \verbatim
rescompphp(_f,N) := for n : 0 thru N do block([F:weak_php_fcs(n+1,n), T, R],
 T : _f(F),
 R : st2reslrt_cs(T,F[2]),
 printf(true, "~1d | ~5d ~5d | ~5d ~5d | ~5d~&", n, nnds_lrt(T), nlvs_lrt(T), nnds_lrt(R), nlvs_lrt(R), ncl_cs(l_lrt(R)))
)$

rescompphp(dll_simplest_st_trivial1,4);
0 |     1     1 |     1     1 |     1
1 |     5     3 |     5     3 |     5
2 |    29    15 |    21    11 |    19
3 |   197    99 |    97    49 |    65
4 |  1565   783 |   521   261 |   248

rescompphp(dll_simplest_st_trivial2,4);
0 |     1     1 |     1     1 |     1
1 |     5     3 |     5     3 |     5
2 |    29    15 |    21    11 |    19
3 |   197    99 |    97    49 |    65
4 |  1565   783 |   521   261 |   248

rescompphp(dll_simplest_st_first_shortest_clause,6);
0 |     1     1 |     1     1 |     1
1 |     5     3 |     5     3 |     5
2 |    23    12 |    21    11 |    19
3 |   123    62 |    85    43 |    62
4 |   763   382 |   377   189 |   167
5 |  5663  2832 |  1941   971 |   419
6 | 49919 24960 | 11725  5863 |  1025

rescompphp(dll_simplest_st_max_var,4);
0 |     1     1 |     1     1 |     1
1 |     5     3 |     5     3 |     5
2 |    21    11 |    21    11 |    19
3 |    97    49 |    97    49 |    70
4 |   521   261 |   521   261 |   285
   \endverbatim
   </li>
   <li> We see that the heuristics "first_shortest_clause" after tree-pruning
   finds the known best tree-refutations (for 0 <= n <= 3; while for n=3 none
   of the others does so). </li>
   <li> None find for n <= 4 optimal dag-refutation (see
   Investigations/PigeonholeFormulas/plans/Resolution.hpp):
    <ol>
     <li> It seems the best extracted dag-refutations aren't for away from
     the conjectured best dag-refutations. </li>
     <li> Can we achieve optimal tree-refutations, which are at the same time
     optimal dag-refutations? </li>
     <li> Can we find a formula for the size of the derived (best)
     dag-refutations? </li>
    </ol>
   </li>
   <li> The above heuristics have the problem that they also have to deal with
   unit-clauses; using ucp now:
   \verbatim
rescompa_php(_f,N) := for n : 0 thru N do block([F:weak_php_fcs(n+1,n), T0,T, R],
 T0 : _f(F),
 T : rst2st(F[2],T0,1),
 R : st2reslrt_cs(T,F[2]),
 printf(true, "~1d | ~5d ~5d | ~5d ~5d | ~5d~&", n, nnds_lrt(T), nlvs_lrt(T), nnds_lrt(R), nlvs_lrt(R), ncl_cs(l_lrt(R)))
)$

rescompa_php(lambda([FF], dll_rk_st_trivial1(FF,1)),6);
0 |     1     1 |     1     1 |     1
1 |     5     3 |     5     3 |     5
2 |    21    11 |    21    11 |    19
3 |   121    61 |    85    43 |    58
4 |   781   391 |   377   189 |   157
5 |  6001  3001 |  1941   971 |   401
6 | 54653 27327 | 11725  5863 |   997
rescompa_php(lambda([FF], dll_rk_st_trivial2(FF,1)),6);
0 |     1     1 |     1     1 |     1
1 |     5     3 |     5     3 |     5
2 |    21    11 |    21    11 |    19
3 |   121    61 |    85    43 |    58
4 |   781   391 |   377   189 |   157
5 |  6001  3001 |  1941   971 |   401
6 | 54653 27327 | 11725  5863 |   997
rescompa_php(lambda([FF], dll_rk_st_first_shortest_clause(FF,1)),6);
0 |     1     1 |     1     1 |     1
1 |     5     3 |     5     3 |     5
2 |    21    11 |    21    11 |    19
3 |   121    61 |    85    43 |    58
4 |   781   391 |   377   189 |   157
5 |  6001  3001 |  1941   971 |   401
6 | 54653 27327 | 11725  5863 |   997
rescompa_php(lambda([FF], dll_rk_st_max_var(FF,1)),6);
0 |     1     1 |     1     1 |     1
1 |     5     3 |     5     3 |     5
2 |    21    11 |    21    11 |    19
3 |    91    46 |    85    43 |    58
4 |   437   219 |   377   189 |   159
5 |  2441  1221 |  1941   971 |   416
6 | 15955  7978 | 11725  5863 |  1065
   \endverbatim
   </li>
   <li> Now except of the last heuristics all heuristics find the shortest
   (known) tree-refutations as well as the shortest known dag-refutations!
   </li>
  </ul>


  \todo Developing a formula (now for the numbers of leaves)
  <ul>
   <li> Strategy: first pigeon in first hole, then first pigeon in second hole,
   and so on. </li>
   <li> First a recursion:
   \verbatim
trphp(n) := if n=0 then 1 else n*trphp(n-1) + n*n + 1;
for n : 0 thru 6 do print(n,trphp(n));
0 1
1 3
2 11
3 43
4 189
5 971
6 5863
   \endverbatim
   </li>
   <li> Explanation:
    <ol>
     <li> There are n possible holes for the first pigeon. </li>
     <li> After placing the pigeon into one of the holes and performing UCP,
     we obtain an instance isomorphic to PHP^n_(n-1). </li>
     <li> To transform this into binary splitting, in the false branch we
     immediately split regarding the next free hole. </li>
     <li> Once placed, UCP removes the other n pigeons from that hole, which
     creates n leaves (by the binary clauses). </li>
     <li> One further leave is created when the pigeon was denied the last
     hole (by the long clause for that pigeon). </li>
     <li> So the recursion formula is trphp(n) = n*trphp(n-1) + n*n + 1. </li>
    </ol>
   </li>
   <li> Trying to find a closed formula:
   \verbatim
load(solve_rec);
solve_rec(t[n]=n*t[n-1]+n*n+1, t[n], t[0]=1);
  t[n] = ('sum((%j^2+2*%j+2)/(%j+1)!,%j,0,n-1))*n!+n!

t(n) := if n=0 then 1 else sum((j^2+2*j+2)/factorial(j+1), j,0,n-1) * n! + n!;
for n : 0 thru 6 do print(n,t(n));
0 1
1 3
2 11
3 43
4 189
5 971
6 5863
   \endverbatim
   </li>
   <li> What can we do with the sum sum((j^2+2*j+2)/factorial(j+1), j,0,n-1):
    <ol>
     <li> At least we should find a good Theta-expression for it. </li>
    </ol>
   </li>
   <li> The alternative strategy "first pigeon in first hole, then second
   pigeon in first hole, and so on", what does it yield? </li>
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
