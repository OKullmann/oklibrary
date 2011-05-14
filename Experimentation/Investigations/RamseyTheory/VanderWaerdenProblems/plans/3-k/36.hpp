// Oliver Kullmann, 13.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/36.hpp
  \brief On investigations into vdw_2(3,36)


  \todo vanderwaerden_2(3,36) > 1257
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,36) = 1258. </li>
   <li> Starting search with the best palindromic solution:
   \verbatim
> k=36 n=1174 cutoff=10000000 expdate="2010-12-02-143818"; export k n; cat AltExp/Exp_PdVanderWaerden_2-3-${k}_gsat-tabu-100-${cutoff}_${expdate}/VanDerWaerden_pd_2-3-${k}_${n}.cnf_sol | PdExtend-O3-DNDEBUG ${n} > solution

> RunVdW3k ${k} ${n} rots 100 100000000 solution
   \endverbatim
   </li>
   <li> n=1203 found satisfiable with certificate
   \verbatim
34,46,71,72,79,83,85,88,89,101,
126,145,146,148,153,162,177,183,185,194,
214,233,268,273,293,296,301,307,310,311,
344,348,360,368,379,397,399,421,422,442,
473,479,486,495,508,510,527,545,555,560,
589,597,606,626,627,643,644,658,666,681,
695,700,714,737,740,745,749,751,767,782,
792,823,825,829,849,851,856,878,888,922,
923,934,952,954,960,973,996,999,1028,1034,
1047,1063,1065,1073,1082,1088,1100,1125,1156,1161,
1193
   \endverbatim
   </li>
   <li> n=1214 found satisfiable with certificate
   \verbatim
2,28,33,67,80,114,117,125,142,144,
151,155,187,209,213,215,216,218,244,246,
253,265,266,287,289,303,324,336,339,347,
366,394,401,410,438,440,446,447,458,468,
472,505,524,525,551,562,585,595,606,631,
643,659,668,673,694,696,697,717,746,768,
770,773,779,805,808,810,817,820,838,845,
853,857,879,891,902,921,928,932,949,964,
986,1001,1002,1005,1006,1013,1027,1050,1066,1067,
1079,1104,1117,1143,1154,1186,1214
   \endverbatim
   </li>
   <li> n=1232 found satisfiable with certificate
   \verbatim
31,64,73,74,86,93,96,111,130,131,
147,168,181,197,199,222,234,236,249,278,
281,286,292,295,296,310,316,318,352,353,
360,369,401,406,407,419,429,440,444,480,
493,514,530,551,555,569,582,606,612,619,
623,625,648,678,685,688,693,697,715,717,
725,730,734,759,776,791,796,797,808,834,
851,873,884,910,924,925,945,956,962,981,
984,995,998,1019,1030,1035,1048,1050,1063,1085,
1087,1110,1129,1130,1141,1161,1167,1180,1184,1206
   \endverbatim
   </li>
   <li> n=1244 found satisfiable with certificate
   \verbatim
18,37,52,80,91,99,128,134,166,171,
173,191,195,222,237,240,260,263,265,274,
284,297,313,321,337,343,356,371,374,385,
388,408,417,424,432,459,461,467,499,504,
518,522,528,559,561,565,570,572,573,592,
615,617,633,646,667,684,689,703,721,740,
741,746,750,778,783,792,818,829,839,851,
852,876,894,898,905,925,948,966,968,985,
987,1000,1009,1017,1036,1040,1051,1073,1079,1090,
1091,1116,1127,1133,1151,1162,1170,1172,1188,1209,
1236
   \endverbatim
   </li>
   <li> n=1253 found satisfiable with certificate
   \verbatim
33,51,70,84,101,122,125,127,136,138,
173,183,196,220,242,247,249,250,269,279,
287,292,294,307,312,323,329,343,344,361,
380,384,390,395,398,423,442,458,460,464,
471,477,506,528,529,553,565,569,571,580,
583,606,612,645,656,691,694,699,728,730,
738,767,773,775,788,797,810,824,842,849,
871,898,904,908,915,935,939,941,952,953,
958,973,976,1009,1010,1026,1032,1050,1052,1056,
1071,1084,1093,1098,1121,1126,1157,1167,1172,1175,
1198,1219,1249
   \endverbatim
   </li>
   <li> n=1255 found satisfiable with certificate
   \verbatim
25,36,72,73,80,103,109,142,164,168,
171,173,180,191,201,216,247,253,282,284,
291,294,328,332,339,353,355,358,386,405,
413,423,427,430,464,466,475,480,503,534,
541,549,550,558,586,587,590,612,615,617,
649,654,665,688,702,709,725,735,756,771,
776,783,799,802,812,820,830,836,839,849,
874,894,908,913,923,941,957,960,961,982,
985,1019,1022,1031,1035,1042,1058,1068,1093,1098,
1104,1116,1145,1163,1178,1182,1183,1200,1215,1216,
1241,1244
   \endverbatim
   </li>
   <li> n=1256:
    <ol>
     <li> rots, cutoff=10^8:
     \verbatim
 1  2  3
16 74 10
100
     \endverbatim
     </li>
     <li> Found satisfiable in run 156 (osteps=30357188) with certificate
     \verbatim
11,45,62,75,82,94,120,122,127,133,
136,149,156,186,188,193,211,238,260,262,
275,279,281,284,285,299,305,322,355,358,
378,379,381,386,390,416,427,460,466,469,
484,492,503,521,538,543,564,581,595,601,
614,618,630,632,667,677,688,719,723,729,
756,766,778,785,788,822,825,836,839,860,
867,871,896,913,933,934,936,941,945,963,
971,988,1010,1037,1039,1045,1062,1089,1093,1098,
1099,1113,1118,1121,1130,1132,1155,1172,1193,1222,
1224
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=1257 found satisfiable (adapted from n=1256) with certificate
   \verbatim
11,45,62,75,82,94,120,122,127,133,
136,149,156,186,188,193,211,238,260,262,
275,279,281,284,285,299,305,322,355,358,
378,379,381,386,390,416,427,460,466,469,
484,492,503,521,538,543,564,581,595,601,
614,618,630,632,667,677,688,719,723,729,
756,766,778,785,788,822,825,836,839,860,
867,871,896,913,933,934,936,941,945,963,
971,988,1010,1037,1039,1045,1062,1089,1093,1098,
1099,1113,1118,1121,1130,1132,1155,1172,1193,1222,
1224
   \endverbatim
   (the same as above, although we have osteps=msteps=2 ?).
   </li>
   <li> n=1258:
    <ol>
     <li> rots, cutoff=5*10^7:
     \verbatim
  1   2   3   4
 19 198  87   1
305
     \endverbatim
     </li>
     <li> adaptg2wsat, cutoff=10^7:
     \verbatim
  1   2   3   4
 51 324 566  59
1000
     \endverbatim
     </li>
     <li> ddfw, cutoff=10^7:
     \verbatim
  1   2   3   4   5
 12  60 229 296  34
631
     \endverbatim
     Seems clearly worse than adaptg2wsat.
     </li>
    </ol>
   </li>
  </ul>


  \todo vdw_2^pd(3,36) >= (1186,1213)
  <ul>
   <li> pdvanderwaerden([3,36])[2] =(conj) 1175 =(conj)
   vanderwaerden([3,36]) - 83. </li>
   <li> pdvanderwaerden([3,36])[1] =(conj) 1158. </li>
   <li> Certificates:
    <ol>
     <li> n=1185:
     \verbatim
7,36,47,64,93,129,134,142,166,168,
171,180,183,187,193,209,212,214,234,265,
291,304,308,323,332,344,352,357,376,378,
386,390,393,398,405,427,431,460,485,509,
513,514,516,549,555,567,578
    \endverbatim
     </li>
     <li> n=1212:
     \verbatim
4,30,44,69,75,80,98,112,123,132,
161,163,164,197,208,222,243,245,250,263,
265,279,300,302,311,316,349,351,380,385,
405,438,445,464,467,474,475,479,504,511,
532,546,561,569,593,598
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 36 gsat-tabu 100 20000000" (older version, without also
   using solution for n-1 in the alternation phase) yields
   \verbatim
Break point 1: 1158
Break point 2: 1175
   \endverbatim
   where all solutions were found within in the first 43 runs (so the second
   break-point is rather weak). </li>
    <li> "RunPdVdWk1k2 3 36 gsat-tabu 100 50000000" found solutions up to
   n=1162 (within the first 24 runs), before being aborted. </li>
    <li> "RunPdVdWk1k2 3 36 gsat-tabu 200 60000000" yields
    \verbatim
Break point 1: 1186
Break point 2: 1213
    \endverbatim
    with all solution found in the first 22 runs, and with
    \verbatim
> E=read_ubcsat("Exp_PdVanderWaerden_2-3-36_gsat-tabu-200-60000000_2011-04-03-054450/VanDerWaerden_pd_2-3-36_1187.cnf_OUT",nrows=200)
  1   2   3   4
116  78   4   2
200
> E=read_ubcsat("Exp_PdVanderWaerden_2-3-36_gsat-tabu-200-60000000_2011-04-03-054450/VanDerWaerden_pd_2-3-36_1214.cnf_OUT",nrows=200)
  1   2   3   4
  6 180  12   2
200
   \endverbatim
   </li>
  </ul>

*/
