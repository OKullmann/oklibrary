/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/23.hpp
  \brief On investigations into vdw_2(3,23)


  \todo vanderwaerden_2(3,23) >= 516
  <ul>
   <li> The conjecture is vanderwaerden_2(3,23) = 516. </li>
   <li> Experience with k=20 is that gsat-tabu is best, however we should
   have a look again --- this might change with different k's or with
   different cutoff's. </li>
   <li> n=500, gsat-tabu, cutoff=10^8: easily found a solution. </li>
   <li> n=510, gsat-tabu, cutoff=10^8: a solution was found in the first
   run (seed=1387406527, osteps=43902628). </li>
   <li> n=511, gsat-tabu, cutoff=10^8: in 31 runs two solutions were found
   (seed=2372840954, osteps=16960676). </li>
   <li> n=512:
    <ol>
     <li> gsat-tabu, cutoff=10^8: in 75 runs one solution was found
     (seed=216008612, osteps=49163297). </li>
     <li> cutoff=5*10^7, gsat-tabu finds a solution, taken over from n=511 
     (run 35), with certificate
     \verbatim
8,10,28,33,49,51,59,62,79,83,
95,97,105,106,118,120,142,144,149,152,
171,172,175,197,218,234,240,241,243,258,
266,281,286,303,312,327,346,347,349,355,
356,369,378,379,393,404,416,422,425,441,
443,448,450,468,487,491
     \endverbatim
     </li>
     <li> rots with cutoff=5*10^7 doesn't find a solution in 400 runs:
     \verbatim
  1   2   3
 86 256  58
400
     \endverbatim
     </li>
     <li> cutoff=10^8, gsat-tabu finds the same solution as above in run 36
     (osteps=15901276). </li>
    </ol>
   </li>
   <li> n=513
    <ol>
     <li> gsat-tabu, cutoff=10^8: in 187 runs one solution was found
     (seed=1657670235,osteps=4640848). </li>
     <li> gsat-tabu, cutoff=5*10^7 doesn't find a solution in 200 runs:
     \verbatim
  1   2   3   4   5   6   9
101  78  12   5   2   1   1
200
     \endverbatim
     </li>
     <li> rots, cutoff=5*10^7 doesn't find a solution in 164 runs:
     \verbatim
 1  2  3  4
24 82 57  1
164
     \endverbatim
     (it seems for this k-value gsat-tabu is still better than rots). </li>
    </ol>
   </li>
   <li> n=514, gsat-tabu
    <ol>
     <li> cutoff=10^8: in 116 runs one solution was found (seed=1685669740,
     osteps=9763052). </li>
     <li> cutoff=2*10^8: in 126 runs one solution was found (seed=1343283093,
     osteps=10794090. </li>
    </ol>
   </li>
   <li> n=515:
    <ol>
     <li> gsat-tabu, cutoff=10^8: in 23 runs a solution was found
     (seed=4106190469, osteps=73878847).
     <li> gsat-tabu, cutoff=2*10^7, found a solution, adapted from n=513
     (run 37, osteps=13154898), with certificate:
     \verbatim
5,6,9,27,32,34,50,67,72,78,
99,101,104,113,114,116,121,137,140,145,
162,172,183,185,195,206,214,232,239,241,
261,278,300,301,321,328,331,344,346,347,
352,354,375,389,390,402,412,413,420,425,
439,441,445,454,464,485,491,510,514
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=516, gsat-tabu
    <ol>
     <li> cut=2*10^7:
     \verbatim
  1   2   3   4   5   6
 29 111  77   9   2   3
231
     \endverbatim
     </li>
     <li> cutoff=5*10^7:
     \verbatim
  1   2   3   4   5   6   7
189 351 101  35  10   3   1
690
     \endverbatim
     </li>
     <li> cutoff=10^8: 
     \verbatim
  1   2  3  4  5 6 9 10
188 276 87 28 14 5 1  1
600
     \endverbatim
     </li>
     <li> cutoff=2*10^8: 
     \verbatim
 1  2  3  4  5  8
32 41 17  7  2  1
100
> summary(E$osteps)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
   27750  1668000  6598000 12460000 15130000 77790000
 1  2  3  4  5  7
30 51 14  2  2  1
100
> summary(E$osteps)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
   223800   2602000   7126000  13450000  16280000 107900000
     \endverbatim
     So the higher cutoff here seems completely uneffective. </li>
    </ol>
   </li>
   <li> n=520, gsat-tabu, cutoff=10^8: in 5 runs only min=2 was reached, so
   let's consider this for now as unsatisfiable. </li>
  </ul>


  \todo Palindromic numbers
  <ul>
   <li> 506 < vanderwaerden_g([3,23],"pd") =(conj) 507 =(conj)
   vanderwaerden([3,23])-9. </li>
   <li> vanderwaerden_g([3,23],"pdsat") =(conj) 506. </li>
   <li> precosat236:
    <ol>
     <li> n=516: ?sat, s. </li>
     <li> n=508: ?sat, s. </li>
     <li> n=507: ?sat, s. </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 23 24 516 gsat-tabu 100 4000000" (old version):
   all 1<=n<=506 sat, all 507<=n<=516 unsat. </li>
  </ul>

*/
