/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/30.hpp
  \brief On investigations into vdw_2(3,30)


  \todo vanderwaerden_2(3,30) > 902
  <ul>
   <li> The predictions are 954, 948. </li>
   <li> The conjecture is vanderwaerden_2(3,30) = ???. </li>
   <li> Search starting with n=852:
   \verbatim
OKplatform> RunVdW3k 30 852 rots 1000 5000000
   \endverbatim
   </li>
   <li> n=883 found satisfiable with certificate
   \verbatim
12,38,45,66,75,88,90,108,125,127,
140,149,154,157,182,186,193,219,228,230,
250,251,273,287,288,291,304,310,312,342,
349,361,365,367,371,379,398,402,404,408,
423,450,453,460,473,476,482,489,519,524,
526,547,561,583,584,600,606,620,624,635,
637,643,645,663,675,682,694,698,712,731,
732,735,737,754,769,774,783,786,809,823,
828,848,857
   \endverbatim
   </li>
   <li> n=889 found satisfiable with certificate
   \verbatim
30,31,45,71,84,85,92,108,110,127,
156,158,159,178,179,188,193,201,203,216,
240,262,263,270,289,290,293,295,299,304,
332,344,349,351,363,373,380,404,406,417,
423,438,443,460,474,491,511,521,526,528,
534,536,552,566,573,589,595,600,622,632,
647,663,665,677,682,684,706,711,714,734,
739,756,785,799,807,808,825,848,850,861
   \endverbatim
   </li>
   <li> n=891 found satisfiable with certificate
   \verbatim
2,9,27,47,58,78,91,113,120,128,
134,137,152,157,174,195,197,206,212,232,
248,249,261,268,271,280,282,300,313,317,
319,335,350,360,382,391,393,396,419,424,
434,446,453,454,483,485,490,507,528,541,
545,559,565,581,582,596,601,604,613,615,
639,646,650,675,678,683,692,715,726,729,
742,744,761,767,781,787,794,798,816,831,
855,863,874
   \endverbatim
   </li>
   <li> n=894 found satisfiable with certificate
   \verbatim
30,47,60,75,79,105,112,118,121,134,
158,159,173,178,190,210,216,227,245,252,
253,255,266,270,282,284,289,306,321,334,
338,340,344,363,364,371,377,393,403,412,
438,445,451,455,477,482,506,511,514,523,
528,543,549,560,585,586,588,602,603,617,
622,639,654,671,673,677,689,697,704,710,
736,741,763,787,788,800,810,821,824,825,
847,858,874
   \endverbatim
   </li>
   <li> n=897 found satisfiable with certificate
   \verbatim
23,25,33,42,62,75,97,104,112,121,
136,141,158,173,179,181,190,196,216,232,
233,252,255,264,266,290,297,301,303,319,
334,344,366,375,377,380,403,408,418,430,
437,438,467,469,474,491,512,525,529,543,
549,565,566,580,585,588,597,623,630,634,
636,659,662,667,676,677,689,710,713,726,
728,745,751,771,778,782,800,815,821,839,
847,858,884
   \endverbatim
   </li>
   <li> n=898:
    <ol>
     <li> rots with cutoff=5*10^6 didn't find a solution:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17
 37 222 170  58  13   8   4   8  16  40  92 120 121  61  24   5   1
1000
     \endverbatim
     </li>
     <li> cutoff=10^7 found a solution, with certificate
     \verbatim
9,34,54,65,85,91,98,120,127,135,
141,144,159,164,181,202,204,213,219,239,
255,256,268,275,278,287,289,307,320,324,
326,342,349,357,367,389,398,400,403,426,
431,441,453,460,461,490,492,497,514,535,
548,552,566,572,588,589,603,608,611,620,
622,646,653,657,682,685,690,699,722,733,
736,749,751,768,774,788,794,801,805,823,
838,862,870,881
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=901
    <ol>
     <li> rots with cutoff=10^7 found a solution (in run 316, for n=898,
     with small changes then) with certificate
     \verbatim
24,47,61,77,84,102,122,124,133,135,
158,170,175,176,190,195,225,233,240,264,
270,272,277,281,287,299,318,323,324,346,
351,360,381,383,388,412,417,434,447,455,
457,484,486,505,509,523,528,540,558,566,
579,583,597,602,610,614,616,619,620,647,
651,656,679,706,713,714,716,721,745,758,
764,767,768,780,805,819,827,836,838,841,
864,873
     \endverbatim
     </li>
     <li> rots with cutoff=10^8 found another solution (in run 6, for n=900,
     with small changes then) with certificate
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
    </ol>
   </li>
   <li> n=902:
    <ol>
     <li> rots found no solution in 500 runs with cutoff=10^7:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18
 25 175 148  32  11   6   1   1   2   1   2  14  30  27  17   3   4   1
500
     \endverbatim
     </li>
     <li> rots found no solution in 500 runs with cutoff=2*10^7:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  12  13  14  15
 46 285 124  19   3   1   1   1   1   1   6   8   3   1
500
     \endverbatim
     </li>
     <li> rots found no solution in 1000 runs with cutoff=5*10^7:
     \verbatim
  1   2   3   4
210 708  81   1
1000
     \endverbatim
     </li>
     <li> rots, cutoff=10^8:
     \verbatim
 1  2
32 61
93
     \endverbatim
     </li>
     <li> rots found in run 90 a solution with cutoff=10^8 (osteps=87469691),
     with certificate:
     \verbatim
23,40,63,90,98,100,109,111,134,147,
164,172,179,189,201,208,209,225,226,240,
248,257,259,283,290,319,327,331,336,359,
368,370,373,374,386,388,411,423,438,442,
462,467,484,485,497,499,505,534,541,542,
544,555,558,573,581,592,616,623,652,660,
666,669,670,689,692,701,703,707,719,721,
738,764,766,771,795,812,817,818,832,845,
849,877,888
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=903:
    <ol>
     <li> rots with cutoff=10^8:
     \verbatim
 1  2 
29 48 
77 
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Palindromic version
  <ul>
   <li> Best ubcsat-algorithm:
   \verbatim
> PdVanderWaerdenCNF-O3-DNDEBUG 3 30 903 > VanDerWaerden_pd_2-3-30_903.cnf

> E=run_ubcsat("VanDerWaerden_pd_2-3-30_903.cnf",runs=100,cutoff=1000000)

> plot(E$alg,E$min)
> table(E$min[E$alg=="gsat_tabu"])
 3  4  5  6  7
 4  2 35 47 12
> table(E$FlipsPerSecond[E$alg=="gsat_tabu"])
65609 
  100
> table(E$min[E$alg=="samd"])
 3  4  5  6  7
 4  1 34 47 14
> table(E$FlipsPerSecond[E$alg=="samd"])
58474
  100
   \endverbatim
   shows that clearly gsat_tabu and samd are best, where gsat_tabu is
   faster. (Currently eval_ubcsat_dataframe(E) is not usable due to a bug in
   run_ubcsat.) </li>
   <li> n=903
    <ol>
     <li> gsat-tabu with cutoff=10^7:
     \verbatim
  3   4   5   6   7   9 
199  40 178   2   2   1 
422 
     \endverbatim
     doesn't look promising. </li>
     <li> gsat-tabu with cutoff=10^8:
     \verbatim
 3  4  5  6 
47  2  1  1 
51 
     \endverbatim
     seems unsatisfiable. </li>
    </ol>
   </li>
  </ul>

*/
