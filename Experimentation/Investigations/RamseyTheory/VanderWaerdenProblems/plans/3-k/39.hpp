/ Oliver Kullmann, 14.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/39.hpp
  \brief On investigations into vdw_2(3,39)


  \todo vanderwaerden_2(3,39) > 1414
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,39) = ???. </li>
   <li> Starting search with the best palindromic solution:
   \verbatim
>  k=39 n=1410 cutoff=35000000 expdate="2010-12-14-225714"; export k n; cat AltExp/Exp_PdVanderWaerden_2-3-${k}_gsat-tabu-100-${cutoff}_${expdate}/VanDerWaerden_pd_2-3-${k}_${n}.cnf_sol | PdExtend-O3-DNDEBUG ${n} > solution

> RunVdW3k ${k} ${n} rots 100 100000000 solution
   \endverbatim
   </li>
   <li> n=1414 found satisfiable (run 39, osteps=2711240) with certificate
   \verbatim
4,40,61,99,102,104,112,114,151,176,
178,181,213,215,216,255,287,289,292,299,
302,303,340,341,364,372,373,376,413,414,
431,438,440,451,458,477,509,532,546,548,
557,569,579,585,586,598,636,643,657,668,
672,694,709,736,743,770,771,780,784,816,
838,845,857,875,882,884,891,895,921,928,
953,986,1023,1029,1030,1038,1050,1064,1067,1069,
1079,1087,1103,1112,1140,1141,1143,1149,1150,1153,
1190,1191,1214,1226,1235,1264,1272,1297,1298,1307,
1323,1356,1371,1381
   \endverbatim
   </li>
  </ul>


  \todo Palindromic version:
  <ul>
   <li> 1410 < vanderwaerden_g([3,39],"pd") =(conj) 1411 =(conj)
   vanderwaerden([3,39]) - ???. </li>
   <li> vanderwaerden_g([3,39],"pdsat") =(conj) 1406. </li>
   <li> Certificates:
    <ol>
     <li> n=1406:
     \verbatim
4,37,55,80,111,115,117,124,130,152,
158,163,191,198,204,222,231,237,259,269,
274,277,305,339,343,346,348,352,370,374,
413,416,420,444,448,450,453,457,485,491,
524,531,564,565,570,592,602,607,638,654,
658,672,679,681,689
     \endverbatim
     </li>
     <li> n=1410:
     \verbatim
4,39,64,68,101,106,125,126,162,175,
183,200,212,219,229,250,257,267,287,293,
304,306,330,347,348,380,384,397,405,415,
422,451,458,476,478,479,489,508,515,516,
525,528,546,550,583,589,602,637,639,664,
680,694,698
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 39 gsat-tabu 100 35000000" yields
   \verbatim
Break point 1: 1406
Break point 2: 1411
   \endverbatim
   where all solutions were found within in the first 10 runs for phase 1, and
   within the first 37 runs for phase 2. </li>
  </ul>

*/
