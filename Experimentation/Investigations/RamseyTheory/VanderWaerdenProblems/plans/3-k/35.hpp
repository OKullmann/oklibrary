// Oliver Kullmann, 6.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/35.hpp
  \brief On investigations into vdw_2(3,35)


  \todo vanderwaerden_2(3,35) > 1204
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,35) = 1205. </li>
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
   <li> n=1180
    <ol>
     <li> Found satisfiable (adapted from n=1169, run 118,
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
     <li> ddfw, cutoff=4*10^6:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
  8  69 197 169  32   7   5   1   5  13  29  53  72  34   4
698
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=1181:
    <ol>
     <li> rots, cutoff=10^8:
     \verbatim
  1   2   3   4   6   9  11
 23 105  49   7   1   1   1
187
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=1182 found satisfiable by adaptg2wsat (adapted from n=1180,
   run 49, osteps=7266747, cutoff=10^7) with certificate
   \verbatim
25,37,54,74,89,99,112,117,129,134,
154,163,171,200,203,210,223,226,240,247,
263,265,269,276,282,302,313,333,343,348,
356,358,388,411,422,424,425,459,467,481,
482,485,491,504,518,522,528,561,580,593,
609,615,646,667,670,689,720,728,740,746,
757,765,783,792,795,814,824,839,851,869,
889,892,894,898,906,926,935,966,977,985,
987,1000,1003,1014,1024,1036,1037,1053,1054,1073,
1083,1088,1116,1128,1133,1151,1153,1170
   \endverbatim
   </li>
   <li> n=1183 found satisfiable by adaptg2wsat (run 136, osteps=2581566,
   cutoff=10^7) with certificate
   \verbatim
33,36,71,84,88,100,110,125,147,156,
189,192,193,199,213,218,250,256,263,272,
293,309,322,324,332,361,366,367,369,374,
378,411,415,440,443,457,458,489,494,495,
507,526,528,551,568,588,589,596,602,618,
643,665,670,680,707,711,737,748,754,766,
774,787,788,790,805,813,824,848,855,877,
879,892,898,921,924,935,938,939,951,975,
976,990,995,1027,1032,1033,1049,1062,1070,1099,
1106,1109,1144,1146,1155
   \endverbatim
   </li>
   <li> n=1187 found satisfiable by adaptg2wsat (adapted from n=1184, there
   run 75, osteps=3724245, cutoff=10^7) with certificate
   \verbatim
30,42,66,67,80,101,112,134,149,153,
180,191,214,224,227,235,249,252,261,288,
323,325,328,346,363,376,383,393,399,402,
408,434,445,446,449,471,487,504,511,520,
524,541,550,556,557,568,593,597,605,621,
634,635,658,679,705,708,716,726,730,733,
742,769,800,806,807,815,837,844,878,883,
889,893,926,930,931,938,952,957,985,994,
1001,1004,1005,1022,1029,1063,1065,1066,1068,1075,
1100,1111,1112,1116,1148,1153
   \endverbatim
   </li>
   <li> n=1202 found satisfiable by adaptg2wsat (adapted from n=1190, there
   run 5, osteps=7187038, cutoff=10^7) with certificate
   \verbatim
6,7,9,43,60,65,83,108,118,120,
134,141,161,182,183,215,219,220,235,252,
253,266,272,282,294,305,319,327,331,337,
342,346,377,393,400,404,411,413,437,448,
450,451,478,511,515,522,541,549,568,586,
590,615,635,660,663,664,672,673,696,697,
710,716,746,749,753,775,786,812,818,821,
827,844,855,874,882,885,918,929,934,959,
968,985,996,1005,1006,1008,1012,1030,1045,1049,
1059,1066,1070,1077,1103,1116,1123,1151,1153,1182
   \endverbatim
   </li>
   <li> n=1204 found satisfiable by adaptg2wsat (adapted from n=1203, there
   run 425, osteps=4695882, cutoff=10^7) with certificate
   \verbatim
35,60,69,92,96,97,108,138,146,151,
166,182,190,212,219,225,239,262,264,277,
280,293,301,313,314,319,323,331,350,356,
360,365,368,375,410,430,439,441,447,465,
476,479,498,502,521,523,553,572,576,609,
610,613,619,647,664,689,693,706,708,726,
735,737,758,772,795,804,811,819,841,854,
874,886,909,911,920,922,928,942,957,967,
983,985,996,1004,1020,1034,1039,1053,1078,1091,
1113,1137,1144,1145,1170
   \endverbatim
   </li>
   <li> n=1205:
    <ol>
     <li> adaptg2wsat with cutoff=10^7:
     \verbatim
  1   2   3   4   5   6
  6 101 379 462  51   1
1000
     \endverbatim
     </li>
     <li> adaptg2wsat with cutoff=2*10^7:
     \verbatim
  1   2   3   4
 17  99 407 166
689
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Palindromic version:
  <ul>
   <li> 1142 < pdvanderwaerden([3,35])[2] =(conj) 1143 =(conj)
   vanderwaerden([3,35]) - ???. </li>
   <li> pdvanderwaerden([3,35])[1] =(conj) 1102. </li>
   <li> Certificates:
    <ol>
     <li> n=1101:
     \verbatim
23,27,48,74,75,97,106,122,130,144,
148,161,165,183,206,232,235,253,254,266,
272,285,288,294,319,354,359,377,380,381,
394,397,418,425,450,451,467,499,504,511,
515,533,536,550
     \endverbatim
     </li>
     <li> n=1142:
     \verbatim
10,37,50,80,93,97,129,131,140,163,
174,177,188,209,210,218,220,242,271,285,
312,315,319,336,341,358,369,373,374,376,
384,409,435,443,446,470,478,487,500,514,
525,540,562
     \endverbatim
     </li>
    </ol>
   </li>
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
