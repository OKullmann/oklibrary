/* Copyright 2010 Oliver Kullmann
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
   <li> 462 < vanderwaerden_g([3,22],"pd") =(conj) 463 =(conj)
   vanderwaerden([3,22])-1.
   </li>
   <li> precosat236:
    <ol>
     <li> n=464: unsat, 217593s. </li>
    </ol>
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
