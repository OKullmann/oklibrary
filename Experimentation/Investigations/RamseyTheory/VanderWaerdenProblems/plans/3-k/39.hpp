// Oliver Kullmann, 14.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
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
   <li> n=1415: "RunVdW3k 39 1415 rots N cutoff"
    <ol>
     <li> cutoff=10^6:
     \verbatim
   2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17
   3   34  106  273  410  428  310  205  122   82   67   34   45   20   30   43
  18   19   20   21   22   23   24   25   26   27   28   29   30   31
  81  123  245  368  543  773 1083 1283 1321  980  613  265   92   18
10000
   1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16
   2   11   64  235  559  802  858  626  411  271  187  104   82   74   63   64
  17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32
  88  173  247  427  721 1056 1630 2143 2464 2473 2073 1287  589  184   29    2
  33
   1
20000
     \endverbatim
     </li>
     <li> cutoff=10^7:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  15  16  17  18  19  20  21
  1  12  76 219 271 177  95  46  23  15   4   2   1   2   4   5   4   6   9  12
 22  23  24
  9   5   2
1000
     \endverbatim
     </li>
     <li> cutoff=10^8:
     \verbatim
 1  2  3  4  5
 1 11 49 36  3
100
     \endverbatim
     </li>
     <li> cutoff=2*10^8:
     \verbatim
 1  2  3  4
 1 27 58 14
100
     \endverbatim
     </li>
    </ol>
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
