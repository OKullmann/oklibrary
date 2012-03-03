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
   <li> Using the simple backtracking algorithms we have yet implemented:
   \verbatim
for n : 0 thru 4 do print(n, nnds_lrt(dll_simplest_st_trivial1(weak_php_fcs(n+1,n))));
0 1
1 5
2 29
3 197
4 1565

for n : 0 thru 4 do print(n, nnds_lrt(dll_simplest_st_trivial2(weak_php_fcs(n+1,n))));
0 1
1 5
2 29
3 197
4 1565

for n : 0 thru 4 do print(n, nnds_lrt(dll_simplest_st_first_shortest_clause(weak_php_fcs(n+1,n))));
0 1
1 5
2 23
3 123
4 763

for n : 0 thru 4 do print(n, nnds_lrt(dll_simplest_st_max_var(weak_php_fcs(n+1,n))));
0 1
1 5
2 21
3 97
4 521
   \endverbatim
   </li>
   <li> We see that none of these simple heuristics find always the optimal
   trees. So we have the task of designing an optimal heuristic for the
   this special case (this shouldn't be too hard). </li>
  </ul>

*/
