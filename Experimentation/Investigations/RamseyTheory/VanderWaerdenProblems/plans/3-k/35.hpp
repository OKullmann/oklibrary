// Oliver Kullmann, 6.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/35.hpp
  \brief On investigations into vdw_2(3,35)


  \todo vanderwaerden_2(3,35) > 1180
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,35) = ???. </li>
   <li> Starting search with the best palindromic solution:
   \verbatim
> k=35 n=1142 cutoff=10000000 expdate="2010-11-30-182053"; export k n; cat AltExp/Exp_PdVanderWaerden_2-3-${k}_gsat-tabu-100-${cutoff}_${expdate}/VanDerWaerden_pd_2-3-${k}_${n}.cnf_sol | PdExtend-O3-DNDEBUG ${n} > solution

> RunVdW3k ${k} ${n} rots 100 100000000 solution

   \endverbatim
   </li>
   <li> n=1165 found satisfiable (rots, cutoff=10^8, adopted from n=1148,
   where a solution was found in run 9, osteps=8936284) with certificate
   \verbatim
21,36,65,66,82,103,105,132,134,142,
147,151,157,176,184,206,225,251,264,268,
282,290,301,319,325,341,354,364,393,412,
428,435,443,447,452,484,486,512,517,526,
527,539,558,564,583,595,620,621,634,665,
674,675,689,695,697,712,732,748,761,763,
768,771,776,785,798,808,819,842,854,859,
874,879,880,893,911,924,930,934,954,956,
961,993,1002,1008,1020,1022,1027,1045,1067,1076,
1081,1102,1119,1131
   \endverbatim
   </li>
   <li> n=1166 (rots, cutoff=10^8):
   \verbatim
 1  2  3  4  8  9 11
23 59 12  2  2  1  1
100
   \endverbatim
   but another 200 runs found in run 172 (osteps=6567054) a solution:
   \verbatim
17,19,44,63,93,96,100,110,118,131,
154,172,174,195,204,229,233,239,241,265,
266,269,279,307,318,332,344,352,369,380,
396,413,417,433,443,450,466,501,524,525,
535,544,562,572,574,575,581,603,612,646,
648,651,673,683,686,688,702,709,713,739,
747,750,759,762,788,799,820,824,825,840,
850,857,861,877,887,908,936,942,951,979,
981,1010,1018,1021,1042,1043,1056,1079,1089,1090,
1095,1121,1154,1164
   \endverbatim
   </li>
   <li> n=1168 found satisfiable (adapted from n=1166) with certificate
   \verbatim
17,19,44,63,93,96,100,110,118,131,
154,172,174,195,204,229,233,239,241,265,
266,269,279,307,318,332,344,352,369,380,
396,413,417,433,443,450,466,501,524,525,
535,544,562,572,574,575,581,603,612,646,
648,651,673,683,686,688,702,709,713,739,
747,750,759,762,788,799,820,824,825,840,
850,857,861,877,887,908,936,942,951,979,
981,1010,1018,1021,1042,1043,1056,1079,1089,1090,
1095,1121,1154,1164,1167
   \endverbatim
   </li>
   <li> n=1180 found satisfiable (adapted from n=1169, run 118,
   osteps=11416401) with certificate
   \verbatim
19,32,63,71,100,106,108,124,125,143,
157,163,175,194,198,204,209,217,246,248,
249,269,278,311,315,320,322,342,343,357,
380,383,385,416,420,422,439,453,454,463,
494,496,508,531,545,552,587,601,602,618,
624,626,653,655,676,690,693,698,713,729,
730,737,759,764,766,774,787,792,804,823,
833,866,870,883,901,912,920,934,951,959,
971,975,994,1012,1014,1018,1025,1026,1051,1070,
1082,1092,1107,1125,1129,1160,1174
   \endverbatim
   </li>
  </ul>


  \todo Palindromic version:
  <ul>
   <li> 1142 < vanderwaerden_g([3,35],"pd") =(conj) 1143 =(conj)
   vanderwaerden([3,35]) - ???. </li>
   <li> vanderwaerden_g([3,35],"pdsat") =(conj) 1102. </li>
   <li> "RunPdVdW3k 35 gsat-tabu 100 10000000" (old version) yields
   \verbatim
Break point 1: 1102
Break point 2: 1143
   \endverbatim
   where all solutions were found within in the first 71 runs (so the second
   break-point is rather weak). </li>
   <li> Rerunning it with doubled cutoff, and using the previous solutions
   properly. We also need the ability to start late. </li>
  </ul>

*/
