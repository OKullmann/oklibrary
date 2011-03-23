// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/24.hpp
  \brief On investigations into vdw_2(3,24)


  \todo vanderwaerden_2(3,24) >= 593
  <ul>
   <li> The conjecture is vanderwaerden_2(3,24) = 593. </li>
   <li> Experience with k=20 is that gsat-tabu is best, however now it seems
   that rots is best (using half of the cutoff of gsat-tabu). </li>
   <li> n=530, gsat-tabu, cutoff=10^8: A solution was easily found. </li>
   <li> n=540, gsat-tabu, cutoff=10^8:
   \verbatim
> ubcsat-okl -alg gsat-tabu -runs 100 -cutoff 100000000 -i VanDerWaerden_2-3-24_540.cnf | tee VanDerWaerden_2-3-24_540.cnf_OUT
# -alg gsat-tabu -runs 100 -cutoff 100000000 -i VanDerWaerden_2-3-24_540.cnf
       sat  min     osteps     msteps       seed
      1 0     1   22497477  100000000 1352939123 
      2 0     2    1782874  100000000 1502390071 
      3 1     0   16984061   16984061 2901963531 
   \endverbatim
   </li>
   <li> n=560, gsat-tabu, cutoff=10^8:
   \verbatim
> ubcsat-okl -alg gsat-tabu -runs 100 -cutoff 100000000 -i VanDerWaerden_2-3-24_560.cnf | tee VanDerWaerden_2-3-24_560.cnf_OUT
# -alg gsat-tabu -runs 100 -cutoff 100000000 -i VanDerWaerden_2-3-24_560.cnf
       sat  min     osteps     msteps       seed
      1 0     1   17750621  100000000 1956429616 
      2 0     5    2138624  100000000  291337957 
      3 0     2     516286  100000000  890207100 
      4 0     2    6874501  100000000 1078823928 
      5 1     0   59397520   59397520 2277207974 
      6 1     0   21657900   21657900 2077294880 
      7 0     2    6943316  100000000 2830567231 
      8 0     1    2073902  100000000 3486000509 
   \endverbatim
   </li>
   <li> Running the search-tool:
   \verbatim
OKplatform> RunVdW3k 24 583 gsat-tabu 1000 10000000
   \endverbatim
   (first "OKplatform> RunVdW3k 24 561 gsat-tabu 100 100000000" was used,
   but the lower cutoff seems more efficient):
    <ol>
     <li> n=578 found satisfiable (seed=3813501165, osteps=1602760) in run 6.
     </li>
     <li> The elements of the k=3-partition of the solution are
     \verbatim
[
 3,22,43,44,61,72,80,89,111,118,
 126,130,140,152,155,177,181,189,196,218,
 227,235,239,246,254,263,264,268,276,283,
 285,292,300,304,305,322,333,341,350,372,
 379,387,391,401,413,416,438,442,450,457,
 479,488,496,500,503,507,515,524,537,546,
 553,561
]
     \endverbatim
     (62 elements). </li>
     <li> Checking:
     \verbatim
default_memory_ecl();
S : [
 3,22,43,44,61,72,80,89,111,118,
 126,130,140,152,155,177,181,189,196,218,
 227,235,239,246,254,263,264,268,276,283,
 285,292,300,304,305,322,333,341,350,372,
 379,387,391,401,413,416,438,442,450,457,
 479,488,496,500,503,507,515,524,537,546,
 553,561
]$
P : create_certificate2_vdw(S,578);
certificate_vdw_p([3,24],578,P);
  true
     \endverbatim
     </li>
     <li> n=579 found satisfiable (seed=1409510574 , osteps=5706287) in run 33.
     The elements of the k=3-partition of the solution are
     \verbatim
[
 15,34,51,56,64,73,88,92,93,102,
 110,114,117,138,139,143,151,167,175,179,
 180,201,204,208,216,226,245,247,254,276,
 284,295,303,325,332,334,353,363,371,375,
 378,399,400,404,412,428,436,440,441,462,
 469,477,486,487,491,506,508,515,523,528,
 545,564
]
     \endverbatim
     (62 elements). </li>
     <li> n=580 found satisfiable (seed=3723870221 , osteps=366990) in run 11.
     The elements of the k=3-partition of the solution are
     \verbatim
default_memory_ecl();
S : [
 10,29,46,51,59,66,68,83,87,88,
 97,105,112,133,134,138,146,162,170,174,
 175,196,199,203,211,221,240,242,249,271,
 279,290,298,320,327,329,348,358,366,370,
 373,394,395,399,407,423,431,435,436,457,
 460,464,472,481,482,486,501,503,510,518,
 523,540,559
]$
P : create_certificate2_vdw(S,580);
certificate_vdw_p([3,24],580,P);
  true
     \endverbatim
     (63 elements). </li>
     <li> n=586 found satisfiable (seed=3343037665, osteps=1598927, run=12),
     where the elements of the k=3-partition of the solution are
     \verbatim
18,37,54,59,67,74,76,91,95,96,
105,113,120,141,142,146,154,170,178,182,
183,204,207,211,219,229,248,250,257,279,
287,298,306,328,335,337,356,366,374,378,
381,402,403,407,415,431,439,443,444,465,
472,480,489,490,494,509,511,518,526,531,
548,567
     \endverbatim
     </li>
     <li> n=587 found satisfiable (seed=410357823, osteps=7862929, run=39),
     where the elements of the k=3-partition of the solution are
     \verbatim
18,37,54,59,67,74,76,91,95,96,
105,113,120,141,142,146,154,170,178,182,
183,204,207,211,219,229,248,250,257,279,
287,298,306,328,335,337,356,366,374,378,
381,402,403,407,415,431,439,443,444,465,
472,480,489,490,494,509,511,518,526,531,
548,567
     \endverbatim
     This is the same as for n=586 ! </li>
     <li> n=590 found satisfiable (seed=1242463204, osteps=2333499, run=48),
     with solution
     \verbatim
21,40,57,62,70,77,79,94,98,99,
108,116,120,123,144,145,149,157,173,181,
185,186,207,210,214,222,232,251,253,260,
282,290,301,309,331,338,340,359,369,377,
381,384,405,406,410,418,434,442,446,447,
468,471,475,483,492,493,497,512,514,521,
529,534,551,570
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Evaluating
   \verbatim
E = run_ubcsat("VanDerWaerden_2-3-24_561.cnf", runs=100,cutoff=1000000)
   \endverbatim
   by plot(E$alg,E$best) and eval_ubcsat_dataframe(E):
   \verbatim
gsat_tabu : 
 0  1  2  3  4  5  6  7  8  9 10 
 4  4 13  6 18 20 17 12  3  1  2 
samd : 
 0  1  2  3  4  5  6  7  8  9 10 
 3  4  8 10 17 12 22 12  7  4  1 
rots : 
 0  1  2  3  4  5  6  7  8  9 10 
 1  5  2  8  4 10 15 15 21 17  2 
   \endverbatim
   Now only these best three algorithms:
   \verbatim
E = run_ubcsat("VanDerWaerden_2-3-24_575.cnf", include_algs=list("gsatt","samd","rots"), runs=500,cutoff=10000000)
plot(E$alg,E$best)
eval_ubcsat_dataframe(E)
rots : 
  0   1   2   3   4   5   6   7   8 
 35  90  94 110  94  45  19   9   4
FlipsPerSecond = 67637
samd : 
  0   1   2   3   4   5   6   7   8  10  11  12  14  16  17 
 27  49  78 137  87  64  41   7   2   1   3   1   1   1   1
FlipsPerSecond = 118920
gsat_tabu : 
  0   1   2   3   4   5   6   7   8   9  15 
 23  62  98 131  98  41  34   7   4   1   1 
FlipsPerSecond = 121030
   \endverbatim
   It seems that rots with half of the cutoff of gsat-tabu is better than
   gsat-tabu. </li>
   <li> Running an search:
   \verbatim
OKplatform> RunVdW3k 24 591 rots 1000 5000000
   \endverbatim
   </li>
   <li> n=592
    <ol>
     <li> Found satisfiable (rots, seed=1337569052, osteps=2845761,
     run=398), with solution
     \verbatim
22,41,58,63,71,78,80,95,99,100,
109,117,121,124,145,146,150,158,174,182,
186,187,208,211,215,223,233,252,254,261,
283,291,302,310,332,339,341,360,370,378,
382,385,406,407,411,419,435,443,447,448,
469,472,476,484,493,494,498,513,515,522,
530,535,552,571
     \endverbatim
     </li>
     <li> 1000 runs with rots, cutoff=5*10^6 found only one solution:
     \verbatim
> E=read_ubcsat("VanDerWaerden_2-3-24_592.cnf_OUT",nrows=1000)
  0   1   2   3   4   5   6   7   8   9  10  11 
  1  25 167 265 267 132  60  35  20  17   8   3 
1000
> E[E$sat==1,]
    sat min  osteps  msteps       seed
168   1   0 2119635 2119635 1621341392
     \endverbatim
     where the solution is
     \verbatim
> ubcsat-okl -alg rots -seed 1621341392 -cutoff 2119635 -i Exp_VanderWaerden_2-3-24_2010-10-12-214502_591/VanDerWaerden_2-3-24_592.cnf -solve | tee VanDerWaerden_2-3-24_592.cnf_OUT2
>
> ExtractCertificate_vdW2 VanDerWaerden_2-3-24_592.cnf_OUT2
22,41,58,63,71,78,80,95,99,100,
109,117,124,145,146,150,158,174,182,186,
187,208,211,215,223,233,252,254,261,283,
291,302,310,332,339,341,360,370,378,382,
385,406,407,411,419,435,443,447,448,469,
472,476,484,493,494,498,513,515,522,530,
535,552,571
     \endverbatim
     This is the same solution as above, only vertex 121 here was left
     out. </li>
     <li> 500 runs with rots, cutoff=10^7 found only one solution. </li>
     <li> rots, cutoff=10^7, finds a solution in run 23 (osteps=1273935,
     seed=2153150186):
     \verbatim
22,41,58,63,71,78,80,95,99,100,
109,117,121,124,145,146,150,158,174,182,
186,187,208,211,215,223,233,252,254,261,
283,291,302,310,332,339,341,360,370,378,
382,385,406,407,411,419,435,443,447,448,
469,476,484,493,494,498,513,515,522,530,
535,552,571
     \endverbatim
     The same solution as above (last version), but with vertex 121 replacing
    vertex 472. </li>
     <li> rots, cutoff=10^7, finds a solution in run 215 (osteps=1898408,
     seed=4066830458), the same as the first one found. </li>
     <li> rots, cutoff=5*10^7, finds a solution in run 14 (osteps=19500171,
     seed=1877893773), which was already found. </li>
     <li> So one could conjecture that this solution with small variations is
     the only solution for n=592 (which would show that vdw_2(3,24)=593). </li>
    </ol>
   </li>
   <li> Restarting the search, now using the solution found for n=592
   as initial assignment:
   \verbatim
OKplatform> cat Exp_VanderWaerden_2-3-24_2010-10-12-214502_591/VanDerWaerden_2-3-24_592.cnf_OUT | extract_solution_ubcsat > Solution_n592
OKplatform> RunVdW3k 24 593 rots 1000 5000000 Solution_n592
   \endverbatim
   (this solution didn't help here). </li>
   <li> n=593:
    <ol>
     <li> Cutoff=5*10^6, rots:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11 
 19 165 260 275 142  64  35  23  11   5   1 
1000 
 1  2  3  4  5  6  7  8  9 10 
 4 45 73 84 35 16 11 12  6  4 
290 
    \endverbatim
     </li>
     <li> Cutoff=10^7, gsat-tabu:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17 
  9  65  73 202 232 216 126  42   7   7   4   3   3   8   1   1   1 
1000 
     \endverbatim
     (it seems clear that rots is better). </li>
     <li> Cutoff=10^7, rots:
     \verbatim
  1   2   3   4   5   6   7   8 
 13 148 181 124  29   3   1   1 
500
  1   2   3   4   5   6   7 
 14 155 163 129  29   7   3 
500 
     \endverbatim
     </li>
     <li> Cutoff=5*10^7, rots:
     \verbatim
 1  2  3 
16 67 17 
100 
 1  2  3 
14 73 13 
100 
     \endverbatim
     </li>
     <li> Let's consider this as unsatisfiable. </li>
    </ol>
   </li>
  </ul>


  \todo Palindromic numbers
  <ul>
   <li> pdvanderwaerden([3,24])[2] = 593 =(conj) vanderwaerden([3,24]). </li>
   <li> pdvanderwaerden([3,24])[1] = 568. </li>
   <li> Certificates:
    <ol>
      <li> n=567:
     \verbatim
1,20,29,30,46,55,77,78,87,104,
106,107,114,117,132,143,164,175,190,193,
200,201,203,220,229,230,252,261,262,277,
278,281
     \endverbatim
     </li>
    <li> n=568:
     \verbatim
10,29,46,51,59,66,68,83,87,88,
97,105,112,133,134,138,146,162,170,174,
175,196,199,203,211,221,240,242,249,271,
279
     \endverbatim
     </li>
     <li> n=592:
     \verbatim
22,41,58,63,71,78,80,95,99,100,
109,117,121,124,145,146,150,158,174,182,
186,187,208,211,215,223,233,252,254,261,
283,291
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Instance statistics:
   \verbatim
> cat VanDerWaerden_pd_2-3-24_569.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
285 43133 192672 0 192672 1 1
 length count
1 1
2 378
3 39704
12 12
13 23
24 3015
> cat VanDerWaerden_pd_2-3-24_593.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
297 46882 209793 0 209793 1 1
 length count
1 1
2 394
3 43156
12 12
13 24
24 3295
   \endverbatim
   </li>
   <li> precosat:
    <ol>
     <li> n=593: unsat, 824141s with precosat236. </li>
     <li> n=569: unsat, 275640s with precosat570, while precosat236
     after 1125645s made only some progress (2 variables fixed). </li>
    </ol>
    So perhaps for the larger instances precosat570 is better.
   </li>
   <li> minisat-2.2.0:
    <ol>
     <li> n=594: unsat, 750979s (csltok, partially with lower clock frequency,
     thus unreliable); 1889572833 conflicts. </li>
     <li> n=593: unsat, 89540s (csltok (with lower load); 468878167 conflicts,
     with added unit-clause). </li>
     <li> n=569: unsat, 87399s (csltok (with lower load); 487418128 conflicts,
     with added unit-clause). </li>
    </ol>
    The run-times seem a bit better than with precosat570 (though there is the
    load-issue).
   </li>
   <li> "RunPdVdW3k 24 25 593 gsat-tabu 100 6000000": all 1<=n<=568 sat,
   and then
   \verbatim
569 0
570 1
571 0
572 1
573 0
574 1
575 0
576 1
577 0
578 1
579 0
580 1
581 0
582 1
583 0
584 1
585 0
586 1
587 0
588 1
589 0
590 1
591 0
592 1
593 0
   \endverbatim
   All solutions found in the first run.
   </li>
   <li> "RunPdVdWk1k2 3 24 gsat-tabu 100 2000000" confirms the above numbers.
   </li>
  </ul>

*/
