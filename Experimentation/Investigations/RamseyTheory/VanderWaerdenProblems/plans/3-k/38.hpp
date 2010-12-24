/ Oliver Kullmann, 14.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/38.hpp
  \brief On investigations into vdw_2(3,38)


  \todo vanderwaerden_2(3,38) > 1374
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,38) = ???. </li>
   <li> Starting search with the best palindromic solution:
   \verbatim
> k=38 n=1368 cutoff=30000000 expdate="2010-12-14-225335"; export k n; cat AltExp/Exp_PdVanderWaerden_2-3-${k}_gsat-tabu-100-${cutoff}_${expdate}/VanDerWaerden_pd_2-3-${k}_${n}.cnf_sol | PdExtend-O3-DNDEBUG ${n} > solution

> RunVdW3k ${k} ${n} rots 100 100000000 solution
   \endverbatim
   </li>
   <li> n=1374 found satisfiable with certificate
   \verbatim
8,35,40,41,69,100,115,144,146,150,
153,157,178,183,186,193,218,227,230,238,
254,264,289,291,294,300,312,329,341,349,
359,363,368,374,412,415,433,449,453,470,
476,477,490,526,534,551,563,564,601,622,
624,645,659,670,692,729,735,738,770,781,
786,809,818,819,844,847,877,893,904,920,
921,923,929,930,934,960,970,992,1003,1007,
1015,1032,1034,1068,1071,1081,1089,1103,1106,1114,
1115,1136,1152,1156,1173,1200,1230,1237,1247,1254,
1263,1266,1288,1303,1328,1341,1373
   \endverbatim
   </li>
  </ul>


  \todo Palindromic version:
  <ul>
   <li> 1368 < vanderwaerden_g([3,38],"pd") =(conj) 1369 =(conj)
   vanderwaerden([3,38]) - ???. </li>
   <li> vanderwaerden_g([3,38],"pdsat") =(conj) 1336. </li>
   <li> Certificates:
    <ol>
     <li> n=1336:
     \verbatim
18,29,50,63,92,96,103,131,157,161,
164,198,200,214,237,240,275,283,286,298,
307,309,314,322,344,348,351,383,394,423,
425,431,433,436,455,460,473,492,507,512,
542,547,549,570,607,608,616,619,623,656
     \endverbatim
     </li>
     <li> n=1368:
     \verbatim
12,20,57,90,91,95,96,105,126,128,
140,174,179,196,205,234,242,247,274,285,
308,313,322,327,339,345,350,362,376,401,
419,433,438,461,469,487,496,506,507,512,
540,561,580,603,607,617,618,644,655,683
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 38 gsat-tabu 100 30000000" yields
   \verbatim
   \endverbatim
Break point 1: 1336
Break point 2: 1369
   where all solutions were found within in the first 17 runs. </li>
  </ul>

*/
