// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/22.hpp
  \brief On investigations into vdw_2(3,22)


  \todo vanderwaerden_2(3,22) >= 464
  <ul>
   <li> The conjecture is vanderwaerden_2(3,22) = 464. </li>
   <li> Experience with k=20 is that gsat-tabu is best, however we should
   have a look again --- this might change with different k's or with
   different cutoff's. </li>
   <li> n=430, gsat-tabu, cutoff=10^8: 12 runs each found a solution. </li>
   <li> n=435, gsat-tabu, cutoff=10^8: 5 runs each found a solution. </li>
   <li> n=438, gsat-tabu, cutoff=10^8: 6 runs each found a solution. </li>
   <li> n=439, gsat-tabu, cutoff=10^8: 5 runs each found a solution. </li>
   <li> n=440, gsat-tabu, cutoff=10^8: 3 runs each found a solution (while
   one run apparently didn't find a solution). </li>
   <li> n=445, gsat-tabu, cutoff=10^8: 5 runs each found a solution. </li>
   <li> n=450, gsat-tabu, cutoff=10^8: 3 runs each found a solution (while
   one run possibly didn't find a solution). </li>
   <li> n=455, gsat-tabu, cutoff=10^8: in 3 runs one solution was found
   (seed=638321166, osteps=8804984). </li>
   <li> n=458:
    <ol>
     <li> ddfw, cutoff=8*10^6:
     \verbatim
 1  2 
80 20 
100 
     \endverbatim
     Looks good, but doesn't find a solution?
     </li>
    </ol>
   </li>
   <li> n=460, gsat-tabu, cutoff=10^8: in 300 runs one solution was found
   (seed=3944359483, osteps=7057558):
   \verbatim
 0   1  2 3 4 5
 1 258 32 6 2 1
300
   \endverbatim
   </li>
   <li> n=461, gsat-tabu, cutoff=2*10^8: In 39 runs a solution was found
   (seed=696047484, osteps=20698025). It seems one should less steps. </li>
   <li> n=462
    <ol>
     <li> gsat-tabu, cutoff=5*10^7:
     \verbatim
 1  2  3
81 17  2
100
     \endverbatim
     In further 313 runs one solution was found (seed=270943421,
     osteps=11236258). </li>
     </li>
     <li> gsat-tabu, cutoff=5*10^7, finds a solution, adapted from n=458
     (run 31), with certificate:
     \verbatim
6,19,25,38,48,54,63,67,69,79,
92,98,106,111,122,125,141,154,156,169,
179,180,198,199,209,222,224,237,253,256,
267,272,280,286,299,309,311,315,324,328,
330,340,353,359,367,372,383,386,402,415,
417,430,440,459,460
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=463
    <ol>
     <li> gsat-tabu, cutoff=5*10^7: in 140 runs one solution was found
    (seed=552382418, osteps=1894212) (while 74 runs with cutoff=10^8 didn't
    find a solution). </li>
     <li> gsat-tabu, cutoff=5*10^7 doesn't find a solution in 200 runs:
     \verbatim
  1   2   3   4
150  45   4   1
200
     \endverbatim
     </li>
     <li> gsat-tabu, cutoff=5*10^7, finds a solution, adapted from n=462:
     \verbatim
3,22,23,33,46,61,77,80,91,96,
104,110,123,133,135,139,148,152,154,164,
177,183,191,196,207,210,226,239,241,254,
264,265,283,284,294,307,309,322,338,341,
352,357,365,371,384,394,396,400,409,415,
425,438,442,452
     \endverbatim
     </li>
     <li> gsat-tabu finds a solution, adapted from n=458 (run=20,
     osteps=8086906, cutoff=10^8):
     \verbatim
22,23,33,46,61,77,80,91,96,104,
110,123,133,135,139,148,152,154,164,177,
183,191,196,207,210,226,239,241,254,264,
265,283,284,294,307,309,322,338,341,352,
357,365,371,384,394,396,400,409,413,415,
425,444,452,457
     \endverbatim
     (close to the above solution). </li>
     <li> adaptg2wsat with cutoff=10^6:
     \verbatim
 1  2  3  4  5
 3 28 39 26  4
100
     \endverbatim
     With cutoff=2*10^6:
     \verbatim
 1  2  3  4 
 3 52 40  5 
100 
     \endverbatim
     </li>
     <li> ddfw with cutoff=10^6:
     \verbatim
 1  2  3  4  5 
 2 30 39 28  1 
100 
     \endverbatim
     With cutoff=2*10^6:
     \verbatim
 1  2  3  4
 6 50 41  3
100
     \endverbatim
     With cutoff=4*10^6:
     \verbatim
 1  2  3
10 80 10
100
     \endverbatim
     With cutoff=8*10^6:
     \verbatim
 1  2  3 
25 72  3 
100 
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Determining the best algorithm from the ubcsat-1-2-0-suite:
    <ol>
     <li>
     \verbatim
E=run_ubcsat("AltExp/Exp_VanderWaerden_2-3-22_2010-10-27-111534_458_gsat-tabu-500-100000000/VanDerWaerden_2-3-22_463.cnf", cutoff=100000,runs=100)

ddfw
 1  2  3  4  5  6  7  8 
 2  1 10 18 32 25 11  1 
FlipsPerSecond = 16988
ag2wsat
 1  2  3  4  5  6  7  8  9 
 1  4 13 21 26 26  7  1  1 
FlipsPerSecond = 40228
gsatt
 2  3  4  5  6  7  8 
 6  9 23 24 25 11  2 
FlipsPerSecond = 75165
dano
 2  3  4  5  6  7  8 
 4 19 21 29 16 10  1 
FlipsPerSecond = 27754   
rnov
 2  3  4  5  6  7  8 
 4 16 27 19 22 10  2 
FlipsPerSecond = 28062
anovp
 2  3  4  5  6  7 
 4 13 31 24 20  8 
FlipsPerSecond = 27749
rnovp
 2  3  4  5  6  7  8  9 
 3 23 24 22 17  7  3  1 
FlipsPerSecond = 27927
anovpp
 2  3  4  5  6  7  8 
 2 13 18 34 26  5  2 
FlipsPerSecond = 27640   

samd
 2  3  4  5  6  7  8  9 
 2  6 24 23 26 13  4  2 
FlipsPerSecond = 75364   

rots
 3  4  5  6  7  8  9 10 
 1  8 14 22 33 19  1  2 
FlipsPerSecond = 60753   

segmentation faults for saps, rsaps, sapsnr
     \endverbatim
     Also here ag2wsat seems best. </li>
    </ol>
   </li>
   <li> n=464, gsat-tabu:
    <ol>
     <li> cutoff=5*10^7:
     \verbatim
   1   2  3  4 5 6 7
2019 615 63 12 4 2 1
2716
  1   2   3   4   5   7
357 133   7   1   1   1
500
     \endverbatim
     </li>
     <li> cutoff=10^8:
     \verbatim
 1  2  3  6
71 25  3  1
100
 1  2  3  5
79 18  2  1
100
 1  2  3 
77 23  1 
101 
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Palindromic numbers
  <ul>
   <li> vanderwaerden_g([3,22],"pd") = 463 =(conj)
   vanderwaerden([3,22])-1. </li>
   <li> vanderwaerden_g([3,22],"pdsat") = 444. </li>
   <li> precosat236:
    <ol>
     <li> n=464: unsat, 217593s. </li>
     <li> n=463: unsat, 15421s. </li>
     <li> n=445: unsat, 23965s. </li>
    </ol>
   </li>
   <li> cryptominisat version 2.9.0:
    <ol>
     <li> n=463: unsat, 47212s (csltok, high load, so possibly slow). </li>
     <li> n=445: unsat, 47673s (same conditions). </li>
    </ol>
    One gets the impression, that cryptominisat is not better than for
    example precosat236. Perhaps one could try turning off all xor-reasoning,
    and also compiling with more optimisation.
   </li>
   <li> minisat-2.2.0, with unit-clause for middle-point added:
    <ol>
     <li> n=463: unsat, 8554s (csltok). </li>
     <li> n=445: unsat, 8124s (same conditions). </li>
    </ol>
    So minisat-2.2.0 might be somewhat faster.
   </li>
   <li>  "RunPdVdW3k 22 23 464 gsat-tabu 100 2000000": alle 1<=n<=444 sat,
   and then
     \verbatim
445 0
446 1
447 0
448 1
449 0
450 1
451 0
452 1
453 0
454 1
455 0
456 1
457 0
458 1
459 0
460 1
461 0
462 1
463 0
464 0
   \endverbatim
   </li>
  </ul>

*/
