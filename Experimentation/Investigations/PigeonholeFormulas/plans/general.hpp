// Oliver Kullmann, 24.5.2008 (Swansea)
/* Copyright 2008, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/PigeonholeFormulas/plans/general.hpp
  \brief Investigations regarding pigeonhole clause-sets


  \todo Statistics on the performance of SAT solvers
  <ul>
   <li> SAT instances created at Maxima level by
   output_weak_php_stdname(m,k) (m pigeons into k holes). </li>
   <li> We should at least consider PHP^{2m}_m, PHP^{m+1}_m amd PHP^m_m. </li>
   <li> Statistics for all our solvers, for m = 0, ... until it takes several
   hours, with curve fittings.
    <ol>
     <li> Cryptominisat 2.9.0:
      <ol>
       <li> PHP^16_15: solved in 75905.41s, with 43,150,728 conflicts (so here
       the "point of competence" seems to have been exceeded). </li>
       <li> PHP^15_14: solved in 3423.40s, with 6,488,768 conflicts. </li>
       <li> PHP^14_13: solved in 1032.63s, with 2,886,433 conflicts. </li>
       <li> PHP^13_12: solved in 286.78s and 1,215,119 conflicts. </li>
       <li> PHP^12_11: solved in 35.15s and 230,963 conflicts. </li>
       <li> PHP^22_11: solved in 1706.90s and 3,338,405 conflicts (quite a
       disaster!). </li>
      </ol>
     </li>
     <li> OKsolver-2002:
      <ol>
       <li> PHP^14_13: solved in 14326.7s, with 4,670,265,599 nodes. </li>
       <li> PHP^13_12: solved in 1145.5s, with 359,251,199 nodes. </li>
       <li> PHP^12_11: solved in 92.1s, with 29,937,599 nodes:
       \verbatim
c initial_maximal_clause_length         11
c initial_number_of_variables           132
c initial_number_of_clauses             738
c initial_number_of_literal_occurrences 1584
c number_of_2-clauses_after_reduction   726
c running_time(sec)                     92.1
c number_of_nodes                       29937599
c number_of_single_nodes                0
c number_of_quasi_single_nodes          9979200
c number_of_2-reductions                38253600
c number_of_pure_literals               12672550
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        55
c proportion_searched                   1.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 171073515
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             PHP_weak_12_11.cnf
       \endverbatim
       (here no missed single-nodes, but many quasi-single nodes).
       </li>
       <li> PHP^22_11: solved in 1887.0s, with 63,201,599 nodes:
       \verbatim
c initial_maximal_clause_length         11
c initial_number_of_variables           242
c initial_number_of_clauses             2563
c initial_number_of_literal_occurrences 5324
c number_of_2-clauses_after_reduction   2541
c running_time(sec)                     1887.0
c number_of_nodes                       63201599
c number_of_single_nodes                0
c number_of_quasi_single_nodes          26611200
c number_of_2-reductions                104781600
c number_of_pure_literals               12672550
c number_of_autarkies                   0
c number_of_missed_single_nodes         14968800
c max_tree_depth                        65
c proportion_searched                   1.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 990012515
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             PHP_weak_22_11.cnf
       \endverbatim
       Interesting the number of (missed!) single nodes, and the number
       of quasi-single nodes. Just trying the opposite branching-order XXX
       </li>
       <li> Finding a satisfying assignment is trivial:
       \verbatim
> OKsolver_2002-O3-DNDEBUG PHP_weak_50_50.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         50
c initial_number_of_variables           2500
c initial_number_of_clauses             61300
c initial_number_of_literal_occurrences 125000
c number_of_2-clauses_after_reduction   61250
c running_time(sec)                     0.7
c number_of_nodes                       49
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                2
c number_of_pure_literals               1222
c number_of_autarkies                   1
c number_of_missed_single_nodes         0
c max_tree_depth                        48
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
       \endverbatim
       </li>
      </ol>
     </li>
     <li> satz215
      <ol>
       <li> PHP^14_13: solved in 24468.120s. </li>
       <li> PHP^13_12: solved in 1814.540s. </li>
       <li> PHP^12_11: solved in 146.810s. </li>
       <li> Finding a satisfying assignment is trivial:
       \verbatim
> satz215 PHP_weak_50_50.cnf
satz215 PHP_weak_50_50.cnf 0.350 49 0 85848 0 1 2500 61300 0 0 0
       \endverbatim
       </li>
      </ol>
     </li>
     <li> march_pl
      <ol>
       <li> PHP^14_13: solved in time=38014.687500s with 1,076,150,280 nodes.
       </li>
       <li> PHP^13_12: solved in time=2845.439941s and 83,735,568 nodes. </li>
       <li> PHP^12_11: solved in time=219.839996s and 6,874,774 nodes. </li>
       <li> Finding a satisfying assignment is rather easy:
       \verbatim
> march_pl PHP_weak_50_50.cnf
c main():: nodeCount: 73
c main():: dead ends in main: 0
c main():: lookAheadCount: 85731
c main():: unitResolveCount: 2500
c main():: time=28.200001
c main():: necessary_assignments: 128
       \endverbatim
       </li>
      </ol>
     </li>
     <li> Precosat570:
      <ol>
       <li> PHP^14_13: solved in 70872.4s, with 43,974,359 conflicts. </li>
       <li> PHP^13_12: solved in 4174.1s, with 7,068,716 conflicts. </li>
       <li> PHP^12_11: solved in 251.2s, with 1,213,305 conflicts. </li>
       <li> PHP^22_11: solved in 3086.2s, with 4,079,747 conflicts. </li>
      </ol>
     </li>
     <li> Glucose 2.0:
      <ol>
       <li> PHP^12_11: solved in time=769.392s and 2480875 conflicts. </li>
      </ol>
     </li>
     <li> Glucose 1.0:
      <ol>
       <li> PHP^12_11: solved in time=1529.39s and 4,938,313 conflicts. </li>
      </ol>
     </li>
     <li> minisat-2.2.0:
      <ol>
       <li> PHP^16_15: After 2650m (nearly 2 days) and more than 3,317,199,477
       conflicts no visible progress. </li>
       <li> PHP^15_14: After 137328s (38h) and 3,348,376,683 conflicts no
       visible progress. </li>
       <li> PHP^14_13: After 112273s and 3,349,660,843 conflicts no
       visible progress. </li>
       <li> PHP^13_12: After 156595s and 5,596,327,305 conflicts no
       visible progress. </li>
       <li> PHP^12_11: solved in 7091.04s and 214,027,827 conflicts. </li>
       <li> Finding a satisfying assignment is easy:
       \verbatim
> minisat-2.2.0 PHP_weak_50_50.cnf
conflicts             : 0              (0 /sec)
decisions             : 1227           (0.00 % random) (17531 /sec)
CPU time              : 0.069989 s
       \endverbatim
       </li>
      </ol>
     </li>
     <li> Picosat913:
      <ol>
       <li> PHP^12_11: solved in 9365.7s, with 457,292,071 conflicts. </li>
      </ol>
     </li>
     <li> Grasp:
      <ol>
       <li> PHP^12_11: self-abortion after 4.282s and 1595 conflicts. It says
       "Terminating NSAT" ? Likely "RESOURCES EXCEEDED" in the output is
       to be taken seriously. We need to ask Joao Marques-Silva. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Also SplittingViaOKsolver+minisat is to be considered.
    <ol>
     <li> For PHP^16_15 apparenty D >= 100 for the splitting should be
     considered. </li>
     <li> However also for D=100 it doesn't seem successful; so perhaps
     higher D needs to be used. </li>
     <li> Perhaps these are examples where this approach is not efficient.
     Perhaps since even for D=100 no r_2-reductions were found? </li>
     <li> For D=120 360360 sub-instances were created (again 0 2-reductions).
     And regarding the full assignments, all assignments achieved *exactly*
     130 variables. While the number of decision-variables in them range
     from 5 to 60 (without gaps). This seems all to be very systematic. </li>
     <li> Still minisat-2.2.0 takes a long time just on the first instance.
     This perhaps is much worse than minisat-2.2.0 alone on the instance. </li>
     <li> Perhaps here the application of the partial assignments doesn't make
     the instances much easier. </li>
     <li> On the other hand, the problem seems very hard for minisat-2.2.0,
     as seen above. </li>
    </ol>
   </li>
   <li> We also consider the performance of E_1(m) and E_2(m), where E_1 is
   the original Cook-extension of PHP^{m+1}_m, and E_2 is the extension of E_1
   obtained by using the resolution-refutation. </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/OKsolver2002.hpp
   for some investigations into the trees computed by the OKsolver-2002. </li>
  </ul>


  \todo Basic statistics
  <ul>
   <li> Implemented formulas:
   \verbatim
for n : 0 thru 10 do block([m:n+1], print(n,nvar_php(m,n),ncl_list_weak_php(m,n),ncl_weak_php(m,n),deficiency_weak_php(m,n)));
0 0 [[0,1]] 1 1
1 2 [[1,2],[2,1]] 3 1
2 6 [[2,9]] 9 3
3 12 [[2,18],[3,4]] 22 10
4 20 [[2,40],[4,5]] 45 25
5 30 [[2,75],[5,6]] 81 51
6 42 [[2,126],[6,7]] 133 91
7 56 [[2,196],[7,8]] 204 148
8 72 [[2,288],[8,9]] 297 225
9 90 [[2,405],[9,10]] 415 325
10 110 [[2,550],[10,11]] 561 451
   \endverbatim
   </li>
   <li> The numbers of satisfying assignments for satisfiable instances:
   \verbatim
for m : 0 thru 4 do print(m,satprob_weak_php(m,m)*2^(m*m));
0 1
1 1
2 2
3 6
4 24
   \endverbatim
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


  \todo Tree-resolution complexity
  <ul>
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


  \todo Resolution complexity
  <ul>
   <li> Now we consider the minimum complexity rc(n) for PHP^{n+1}_n (counting
   the number of different clauses). </li>
   <li> It is known rc(n) >= 2^(Omega(n)). </li>
   <li> In [Iwama, Miyazaki, 1999] it is shown rc(n) <= O(n^2 * 2^n). </li>
   <li> In [Cook, 1976] one finds the claim
   rc(n) <= 1/2 n^3 + 1/2 n^2 + n + 1 + (n^2 + 3 n) * 2^(n-2).
   (In the paper one has (n^2 + 3 n) * 2^(n-2), which stands for the number of
   steps, and thus the number of clauses of PHP has to be added.) </li>
   <li> Alaisdair Urquhart conjectures this to be exact. </li>
   <li> Numerically:
   \verbatim
f(n) := 1/2*n^3 + 1/2*n^2 + n + 1 + (n^2 + 3*n) * 2^(n-2);
for n : 0 thru 20 do print(n,f(n),(n^2 + 3*n) * 2^(n-2));

0 1 0
1 5 2
2 19 10
3 58 36
4 157 112
5 401 320
6 997 864
7 2444 2240
8 5929 5632
9 14239 13824
10 33841 33280
11 79586 78848
12 185269 184320
13 427181 425984
14 976333 974848
15 2213656 2211840
16 4982929 4980736
17 11143739 11141120
18 24775705 24772608
19 54791726 54788096
20 120590461 120586240
   \endverbatim
   </li>
   <li> Experimentally:
   \verbatim
output_res_php(n,k) := outputext_fcl(sconcat("PHP with n=",n," and k=",k,"."), shortresref_aloamo_fcl(fcs2fcl(weak_php_fcs(n+1,n)),k),sconcat("PHP_RES_",n,"_",k,".cnf"));

output_res_php(1,1);
> n=1 k=1; cat PHP_RES_${n}_${k}.cnf | ExtendedToStrictDimacs-O3-DNDEBUG > Php_res_${n}_${k}.cnf
UNSATISFIABLE

output_res_php(1,2);
> n=1 k=2; cat PHP_RES_${n}_${k}.cnf | ExtendedToStrictDimacs-O3-DNDEBUG > Php_res_${n}_${k}.cnf
SATISFIABLE

output_res_php(2,9);
> n=2 k=9; cat PHP_RES_${n}_${k}.cnf | ExtendedToStrictDimacs-O3-DNDEBUG > Php_res_${n}_${k}.cnf
UNSATISFIABLE (minisat-2.2.0: 22714981 conflicts, 1303.62 s)
output_res_php(2,10);
n=2 k=10; cat PHP_RES_${n}_${k}.cnf | ExtendedToStrictDimacs-O3-DNDEBUG > Php_res_${n}_${k}.cnf
SATISFIABLE

output_res_php(3,35);
# (likely) needs too much memory for a 4GB machine
   \endverbatim
   </li>
   <li> Established values for rc(n):
    <ol>
     <li> rc(0) = 1 (= 1+0) </li>
     <li> rc(1) = 5 (= 3+2) </li>
     <li> rc(2) = 19 (= 9+10) </li>
    </ol>
   </li>
  </ul>

*/
