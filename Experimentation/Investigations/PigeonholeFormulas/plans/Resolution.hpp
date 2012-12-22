// Oliver Kullmann, 2.3.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/PigeonholeFormulas/plans/Resolution.hpp
  \brief Investigations regarding (full) resolution for pigeonhole clause-sets


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
    <ol>
     <li> An overview on the sizes involved:
     \verbatim
f(n) := (n^2 + 3*n) * 2^(n-2);
for n : 1 thru 5 do block([F:fcs2fcl(weak_php_fcs(n+1,n)), k : f(n)], print(n, k, "[", nvar_shortres_aloamo_fcl(F,k-1), ncl_shortresref_aloamo_fcl(F,k-1), "]", "[", nvar_shortres_aloamo_fcl(F,k), ncl_shortresref_aloamo_fcl(F,k),"]"));

1 2 [ 32 148 ] [ 48 358 ]
2 10 [ 612 65064 ] [ 672 78486 ]
3 36 [ 5202 4751007 ] [ 5364 5023824 ]
4 112 [ 33780 171494303 ] [ 34172 174910216 ]
5 320 [ 198690 4436417912 ] [ 199610 4470118100 ]
     \endverbatim
     </li>
     <li> Easy cases n=1,2:
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
     \endverbatim
     <li> Now n=3 (4 pigeons into 3 holes):
     \verbatim
# running Maxima with "heap_size_ecl_okl=22000000000 oklib --maxima"

output_res_php(3,35);
> n=3 k=35; cat PHP_RES_${n}_${k}.cnf | ExtendedToStrictDimacs-O3-DNDEBUG > Php_res_${n}_${k}.cnf
> cat Php_res_3_35.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
   5202 4751007   5202   5202 4751007 24174282     NA     NA 4751007 24174282     1
 length   count
      1     276
      2   60222
      3   33444
      5 3991680
      6  665280
     12      35
# all clause-lengths 22-56 each with 2 occurrrences
> cat Php_res_3_35.cnf | UnitClausePropagationW-O3-DNDEBUG | ExtendedToStrictDimacs-O3-DNDEBUG > Php_res_3_35_UP.cnf
> cat Php_res_3_35_UP.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG | head -2
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
   5130 2359911   4374   4374 2359911 11278146     NA     NA 2359911 11278146     6
> ManipParam-O3-DNDEBUG Php_res_3_35_UP.cnf 4374 2359911
> cat Php_res_3_35_UP.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
   4374 2359911   4374   4374 2359911 11278146     NA     NA 2359911 11278146     6
 length   count
      2   76182
      3   22488
      4  394128
      5 1723392
      6  143616
     12      35
# all clause-lengths 22-56 each with 2 occurrrences
> minisat-2.2.0 Php_res_3_35_UP.cnf
# output "INDETERMINATE" after 1 day or so.

# r_2-reductions yields 770 assignments, and the conflict-driven solvers seem
to find just them (after a day or so), so just applying them:
> SplittingViaOKsolver -D0 Php_res_3_35_UP.cnf
> cat Php_res_3_35_UP.cnf | ApplyPass-O3-DNDEBUG Instances/1 Temp.cnf
> cat Temp.cnf | ExtendedToStrictDimacs-O3-DNDEBUG > Php_res_3_35_UP_r2.cnf
> cat Php_res_3_35_UP_r2.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG | head -2
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
   4374 1668812   3604   3604 1668812  8166014     NA     NA 1668812  8166014     8
> ManipParam-O3-DNDEBUG Php_res_3_35_UP_r2.cnf 3604 1668812
> cat Php_res_3_35_UP_r2.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
   3604 1668812   3604   3604 1668812  8166014     NA     NA 1668812  8166014     8
 length   count
      2   43247
      3   17940
      4  157897
      5 1306009
      6  143617
# lengths 7-11: 1
     12      36
# lengths 13-17: 1
# lengths 18-38: 2
# lengths 39-52: 1

> minisat-2.2.0 Php_res_3_35_UP_r2.cnf
# output "INDETERMINATE" after 2 days (space-limit), down to 3564 variables


output_res_php(3,36);
> n=3 k=36; cat PHP_RES_${n}_${k}.cnf | ExtendedToStrictDimacs-O3-DNDEBUG > Php_res_${n}_${k}.cnf
> cat Php_res_3_36.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
   5364 5023824   5364   5364 5023824 25564104     NA     NA 5023824 25564104     1
 length   count
      1     276
      2   63528
      3   34824
      5 4221504
      6  703584
     12      36
# all clause-lengths 22-57 each with 2 occurrrences
> cat Php_res_3_36.cnf | UnitClausePropagationW-O3-DNDEBUG | ExtendedToStrictDimacs-O3-DNDEBUG > Php_res_3_36_UP.cnf
> cat Php_res_3_36_UP.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG | head -2
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
   5292 2527824   4536   4536 2527824 12101256     NA     NA 2527824 12101256     6
> ManipParam-O3-DNDEBUG Php_res_3_36_UP.cnf 4536 2527824
> cat Php_res_3_36_UP.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
   4536 2527824   4536   4536 2527824 12101256     NA     NA 2527824 12101256     6
 length   count
      2   79968
      3   23508
      4  410760
      5 1856400
      6  157080
     12      36
# all clause-lengths 22-57 each with 2 occurrrences
> minisat-2.2.0 Php_res_3_36_UP.cnf
# aborted after 106543 s, with 148789 restarts and down to 3775 variables
# according to table

> glucose
# aborted after 1752 m, down to 3797 variables

> precosat
# aborted after 48027.0 s, down to 3795 variables (seems most efficient from
# the tree conflict-driven solvers)

# Using local search for Php_res_3_36_UP.cnf:
E = run_ubcsat("Php_res_3_36_UP.cnf", runs=100, cutoff=100000)
> eval_ubcsat_dataframe(E)
1. rsaps:
 1  2  3  4  5  6  8
10 23 26 26 12  2  1
fps: 40298
2. saps:
 2  3  4  5  6  7  8  9 10 11
 4  7  9 14 14 24 15  7  3  3
fps: 25816
3. sapsnr:
 3  4  5  6  7  8  9 10 11
 7  4 17 16 26 20  5  3  2
fps: 25216
4. gwsat:
 3  4  5  6  7  8 10
 1  9 18 45 21  5  1
fps: 45884
5. nov:
 4  5  6  7  8  9 10 11 12
 4  8  9 24 13 12 19 10  1
fps: 24953

> ubcsat-okl -alg rsaps -cutoff 100000 -runs 10000 -i Php_res_3_36_UP.cnf | tee Php_res_3_36_UP.cnf_OUT
   1    2    3    4    5    6    7    8    9   10
 756 2650 3005 1957 1060  393  137   34    6    2
10000
> ubcsat-okl -alg rsaps -cutoff 1000000 -runs 10000 -i Php_res_3_36_UP.cnf | tee Php_res_3_36_UP.cnf_OUT
XXX cs-wsok

# r_2-reductions yields 792 assignments, and the conflict-driven solvers seem
to find just them (after a day or so), so just applying them:
> SplittingViaOKsolver -D0 Php_res_3_36_UP.cnf
> cat Php_res_3_36_UP.cnf | ApplyPass-O3-DNDEBUG Instances/1 Temp.cnf
> cat Temp.cnf | ExtendedToStrictDimacs-O3-DNDEBUG > Php_res_3_36_UP_r2.cnf
> cat Php_res_3_36_UP_r2.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG | head -2
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
   4536 1801380   3744   3744 1801380  8825184     NA     NA 1801380  8825184     8
> ManipParam-O3-DNDEBUG Php_res_3_36_UP_r2.cnf 3744 1801380
> cat Php_res_3_36_UP_r2.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
   3744 1801380   3744   3744 1801380  8825184     NA     NA 1801380  8825184     8
 length   count
      2   45696
      3   18876
      4  165901
      5 1413721
      6  157081
# lengthss 7-11: 1
     12      37
# lengths 13-17: 1
# lengths 18-39: 2
# lengths 40-53: 1

> minisat-2.2.0 Php_res_3_36_UP_r2.cnf
# output "INDETERMINATE" after 2 days (space-limit), down to 3707 variables
> glucose-2.0 Php_res_3_36_UP_r2.cnf
# aborted after 2 days, down to 3711 variables
> precosat-570.1 -v Php_res_3_36_UP_r2.cnf
# aborted after 2days, down to 3636 variables

> ubcsat-okl -alg rsaps -cutoff 1000000 -runs 10000 -i Php_res_3_36_UP_r2.cnf | tee Php_res_3_36_UP_r2.cnf_OUT
   1    2    3
9419  575    6
10000
     \endverbatim
     </li>
     precosat-570.1 seems best. But symmetry-breaking is needed.
    </ol>
   </li>
   <li> Established values for rc(n):
    <ol>
     <li> rc(0) = 1 (= 1+0) </li>
     <li> rc(1) = 5 (= 3+2) </li>
     <li> rc(2) = 19 (= 9+10) </li>
    </ol>
   </li>
   <li> For upper bounds extracted from tree-refutations see
   "Tree-resolution refutations found by simple algorithms" in
   Investigations/PigeonholeFormulas/plans/TreeResolution.hpp. </li>
  </ul>

*/
