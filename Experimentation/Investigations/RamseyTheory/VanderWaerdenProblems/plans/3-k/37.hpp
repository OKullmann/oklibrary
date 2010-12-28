/ Oliver Kullmann, 13.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/37.hpp
  \brief On investigations into vdw_2(3,37)


  \todo vanderwaerden_2(3,37) > 1335
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,37) = 1336. </li>
   <li> Starting search with the best palindromic solution:
   \verbatim
> k=37 n=1294 cutoff=20000000 expdate="2010-12-02-144008"; export k n; cat AltExp/Exp_PdVanderWaerden_2-3-${k}_gsat-tabu-100-${cutoff}_${expdate}/VanDerWaerden_pd_2-3-${k}_${n}.cnf_sol | PdExtend-O3-DNDEBUG ${n} > solution

> RunVdW3k ${k} ${n} rots 100 100000000 solution
   \endverbatim
   </li>
   <li> n=1334 found satisfiable with certificate
   \verbatim
10,26,49,60,63,78,83,101,138,157,
160,169,189,194,204,211,212,245,261,263,
280,306,317,350,360,372,374,379,387,409,
411,416,417,419,428,454,467,490,493,502,
507,520,530,535,545,578,594,596,609,646,
655,676,692,693,715,726,752,761,763,767,
787,794,798,804,840,855,860,861,863,889,
890,892,909,914,934,964,966,971,983,1001,
1022,1045,1048,1062,1083,1090,1100,1131,1133,1137,
1149,1151,1159,1164,1170,1193,1194,1211,1225,1231,
1262,1267,1275,1312
   \endverbatim
   </li>
   <li> n=1335 (run 90, osteps=84752609) found satisfiable with certificate
   \verbatim
8,12,14,37,55,86,88,92,111,112,
119,141,148,151,186,193,203,222,225,234,
240,262,273,291,297,308,310,313,341,345,
350,370,384,410,415,419,437,439,445,452,
462,484,498,513,521,526,536,556,567,572,
609,624,630,643,659,661,696,717,740,748,
757,758,770,772,795,807,814,828,851,852,
854,863,865,889,906,928,943,965,974,979,
992,1007,1011,1029,1036,1053,1068,1073,1090,1111,
1140,1147,1150,1165,1184,1192,1202,1216,1229,1233,
1235,1260,1270,1275,1296,1312
   \endverbatim
   </li>
   <li> n=1336:
    <ol>
     <li> rots, cutoff=10^8:
     \verbatim
 1  2  3
31 63  6
100
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Palindromic version:
  <ul>
   <li> 1294 < vanderwaerden_g([3,37],"pd") =(conj) 1295 =(conj)
   vanderwaerden([3,37]) - 41. </li>
   <li> vanderwaerden_g([3,37],"pdsat") =(conj) 1264. </li>
   <li> Certificates:
    <ol>
     <li> n=1264:
     \verbatim
1,38,69,79,102,106,116,117,134,149,
158,180,187,191,201,217,223,234,256,275,
293,298,302,307,335,345,350,361,375,382,
404,435,449,450,454,460,472,478,486,493,
528,560,567,571,587,589,608,626
     \endverbatim
     </li>
     <li> n=1294:
     \verbatim
8,33,65,72,76,107,109,113,118,141,
176,178,187,220,229,252,255,263,266,270,
294,301,324,335,337,342,352,372,377,381,
389,405,435,442,446,448,451,462,485,514,
527,536,553,562,564,573,596,620,625,638
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 37 gsat-tabu 100 20000000" (older version, without also
   using solution for n-1 in the alternation phase) yields
   \verbatim
Break point 1: 1264
Break point 2: 1295
   \endverbatim
   where all solutions were found within in the first 52 runs (so the second
   break-point is rather weak). </li>
   <li> Rerunning it with doubled cutoff, and using the solution for n-1.
   We also need the ability to start late. </li>
  </ul>

*/
