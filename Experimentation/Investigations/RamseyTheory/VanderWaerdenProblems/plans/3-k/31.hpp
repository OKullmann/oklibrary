// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/31.hpp
  \brief On investigations into vdw_2(3,31)


  \todo vanderwaerden_2(3,31) > 930
  <ul>
   <li> The very weak conjecture is vanderwaerden_2(3,31) = 931. </li>
   <li> DONE (doesn't seem to help)
   The highest n come from palindromic solutions (see below). We should
   restart search for general solutions using these special solutions. </li>
   <li> n=900
    <ol>
     <li> rots, cutoff=10^7:
     \verbatim
 1  2  3  4  5  6
 4 16 46 84 40  1
191
     \endverbatim
     So higher cutoffs are needed.
     </li>
     <li> rots, cutoff=10^7:
     \verbatim
 1  2  3  4  5
 5 16 31 39  1
92
     \endverbatim
     Again, higher cutoffs are needed.
     </li>
    </ol>
   </li>
   <li> n=906: a solution was found, adapted from the solution for k=30,
   n=901, with certificate
   \verbatim
25,48,53,56,80,85,102,103,130,139,
152,167,173,177,196,199,208,210,213,214,
228,245,270,271,284,288,302,310,315,319,
337,356,361,374,384,413,419,426,432,435,
448,450,472,485,493,495,506,529,530,532,
546,561,566,578,584,604,611,615,617,635,
640,643,648,658,670,677,694,707,717,726,
747,752,759,763,765,769,781,806,826,828,
829,833,863,876
   \endverbatim
   </li>
   <li> n=914: a solution was found, adapted from the solution for n=907,
   with certificate
   \verbatim
25,26,41,59,78,88,111,132,137,143,
148,170,173,199,217,222,226,236,243,256,
280,284,285,293,294,318,328,331,337,359,
367,370,392,396,421,429,442,444,458,465,
469,470,476,479,502,503,506,515,522,532,
555,559,569,576,589,613,617,618,626,651,
661,670,691,698,700,701,729,738,744,762,
772,791,802,814,835,848,849,855,886
   \endverbatim
   </li>
   <li> n=916: a solution was found, adapted from the solution for n=915,
   with certificate
   \verbatim
30,61,71,76,99,102,113,121,145,152,
173,178,182,195,210,224,226,227,232,261,
263,283,284,289,301,320,324,330,337,338,
345,363,394,398,412,417,431,435,454,456,
474,483,485,493,506,509,528,552,557,559,
560,565,580,594,596,617,622,634,653,657,
663,670,671,691,700,731,733,745,750,752,
764,779,789,807,811,816,838,842,863,885,
900
   \endverbatim
   </li>
   <li> n=917:
    <ol>
     <li> rots, cutoff=2*10^7:
     \verbatim
 1  2  3   4  5  6 
 4 18 50 124 82  2 
280
  1   2   3   4   5   6 
 21  55 148 391 246  10 
871 
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=918: rots found a solution, adapted from n=917, cutoff=5*10^7,
   with certificate:
   \verbatim
10,40,59,67,84,98,104,115,120,121,
133,148,177,185,191,194,209,214,231,252,
254,283,288,289,305,306,318,325,337,343,
363,370,376,392,394,402,407,411,417,429,
448,476,481,487,510,513,518,522,524,542,
547,564,585,587,616,617,621,638,639,653,
658,688,696,703,709,733,735,740,746,762,
787,809,816,820,844,846,855,860,873,898,
910
   \endverbatim
   </li>
   <li> n=919:
    <ol>
     <li> rots with cutoff=5*10^7 found in 500 rounds no solution:
     \verbatim
  1   2   3   4   5
 28  63 162 215  32
500
     \endverbatim
     </li>
     <li> rots with cutoff=10^8:
     \verbatim
 1  2  3  4
 9 25 50 20
104
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Starting search with the best palindromic solution:
   \verbatim
> cat Exp_PdVanderWaerden_2-3-31_gsat-tabu-100-10000000_2010-11-30-010719/VanDerWaerden_pd_2-3-31_930.cnf_sol | PdExtend-O3-DNDEBUG 930 > solution
> RunVdW3k 31 930 rots 100 100000000 solution

Certificate (the vertices for k=3) for n = 930 :
13,33,50,61,71,75,80,95,108,123,
142,143,154,159,173,182,188,190,201,219,
235,252,261,267,276,278,283,292,312,329,
335,354,359,385,400,417,421,431,438,452,
464,467,479,493,500,510,514,531,546,572,
577,596,602,619,639,648,653,655,664,670,
679,696,712,730,741,743,749,758,772,777,
788,789,808,823,836,851,856,860,870,881,
898,918

> E = read_ubcsat("Exp_VanderWaerden_2-3-31_930_rots-100-100000000_2010-12-03-235537/VanDerWaerden_2-3-31_931.cnf_OUT",nrows=100)
 1  2  3  4  5
 5 23 24 31 17
100
   \endverbatim
   And, as expected, gsat-tabu performs worse:
   \verbatim
> E = read_ubcsat("Exp_VanderWaerden_2-3-31_930_gsat-tabu-100-100000000_2010-12-09-134121/VanDerWaerden_2-3-31_931.cnf_OUT",nrows=100)
 2  3  4  5  6  7  8 10 11 12 13 14 15 16 
 4 20 25 13  7  3  6  2  4  2  6  4  3  1 
100 
   \endverbatim
   So n=931 seems are hard instance. </li>
  </ul>


  \todo Palindromic version
  <ul>
   <li> 930 < pdvanderwaerden([3,31])[2] =(conj) 931 =(conj)
   vanderwaerden([3,31]) - 0. </li>
   <li> pdvanderwaerden([3,31])[1] =(conj) 916. </li>
   <li> Certificates:
    <ol>
     <li> n=915:
     \verbatim
13,17,22,30,42,51,68,79,84,88,
93,113,143,150,177,181,183,208,217,234,
236,237,263,267,276,279,296,299,301,305,
330,338,342,367,370,372,401,422,434,436,
453
     \endverbatim
     </li>
     <li> n=930:
     \verbatim
13,33,50,61,71,75,80,95,108,123,
142,143,154,159,173,182,188,190,201,219,
235,252,261,267,276,278,283,292,312,329,
335,354,359,385,400,417,421,431,438,452,
464
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Best ubcsat-algorithm:
   \verbatim
> PdVanderWaerdenCNF-O3-DNDEBUG 3 31 919

> E=run_ubcsat("VanDerWaerden_pd_2-3-31_919.cnf",runs=100,cutoff=1000000)

> plot(E$alg,E$min)
> table(E$min[E$alg=="gsat_tabu"])
 1  2  3
20 70 10
> table(E$FlipsPerSecond[E$alg=="gsat_tabu"])
67526
  100
> table(E$min[E$alg=="samd"])
 1  2  3
12 65 23
> table(E$FlipsPerSecond[E$alg=="samd"])
60536
  100
   \endverbatim
   shows clearly that gsat-tabu is best, followed by samd (which is also
   slower). (Currently eval_ubcsat_dataframe(E) is not usable due to a bug in
   run_ubcsat.) </li>
   <li> n=919:
    <ol>
     <li> gsat-tabu with cutoff 10^7:
     \verbatim
  1   2 
603 194 
797 
     \endverbatim
     </li>
     <li> gsat-tabu with cutoff=10^8:
     \verbatim
 1  2 
14  1 
15 
    \endverbatim
     seems unsatisfiable. </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 31 32 919 gsat-tabu 100 10000000" old version:
   finds 1<=n<=916 sat, and then
   \verbatim
917 0
918 1
919 0
   \endverbatim
   where the solutions have been found within at most the first 10 runs.
   </li>
   <li> "RunPdVdW3k 31 920 922 gsat-tabu 100 10000000" (old version) found
   solutions for n=920 and 922, where the certificate for the latter is
   \verbatim
20,29,50,57,60,82,103,106,119,129,
153,160,165,174,205,212,220,231,243,258,
262,267,269,282,289,313,331,336,362,375,
377,386,391,398,401,417,424,444,453
   \endverbatim
   </li>
   <li> "RunPdVdW3k 31 gsat-tabu 100 10000000" yields
   \verbatim
Break point 1: 916
Break point 2: 931
   \endverbatim
   where finding the solution needed up to 48 runs. </li>
   <li> Checking the unsatisfiability for n=932:
   \verbatim
> ubcsat-okl -alg gsat-tabu -runs 200 -cutoff 20000000 -i Exp_PdVanderWaerden_2-3-31_gsat-tabu-100-10000000_2010-11-30-010719/VanDerWaerden_pd_2-3-31_932.cnf | tee VanDerWaerden_pd_2-3-31_932.cnf_OUT

> E = read_ubcsat("VanDerWaerden_pd_2-3-31_932.cnf_OUT",nrows=200)
  1   2   3   9
178  20   1   1
200
   \endverbatim
   </li>
  </ul>

*/
