// Oliver Kullmann, 14.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/39.hpp
  \brief On investigations into vdw_2(3,39)


  \todo vanderwaerden_2(3,39) > 1416
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
     <li> cutoff=2*10^6:
     \verbatim
   1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16
   4   31  211  734 1403 1757 1517  945  644  389  254  167  117   90   95  106
  17   18   19   20   21   22   23   24   25   26   27   28   29   30
 144  200  407  635  990 1510 1826 2029 1869 1204  553  150   17    2
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
     <li> cutoff=2*10^7:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  14  16  17  19  20  21
 10  47 240 621 594 246  57  17   8   2   2   1   2   1   3   4
1855
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
     <li> Found satisfiable (rots, osteps=12345027, run 175, cutoff=2*10^8):
     \verbatim
3,26,53,56,76,79,85,90,98,129,
164,172,174,187,191,211,240,260,266,267,
270,277,278,301,315,334,340,375,377,388,
389,423,426,433,451,460,482,492,519,520,
542,573,581,593,597,608,611,634,645,648,
673,705,710,721,727,742,745,764,795,803,
815,816,838,858,859,862,877,904,914,930,
932,941,963,1000,1010,1015,1038,1041,1060,1062,
1074,1091,1092,1097,1128,1129,1149,1186,1193,1208,
1222,1239,1245,1259,1269,1276,1284,1300,1302,1311,
1314,1351,1358,1373,1374,1380
     \endverbatim
     </li>
     <li> gsat-tabu, cutoff=2*10^6:
     \verbatim
   3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18
   3    1    3    3    3    6    6    6   19   35   83  162  275  517  808 1125
  19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34
1357 1648 1802 1754 1699 1559 1301 1081  927  729  581  476  369  291  219  193
  35   36   37   38   39   40   41   42   43   44   45   46   47   48   49   50
 138  139  100   72   79   67   49   50   38   30   22   20   21   10   11    8
  51   52   53   54   55   56   57   58   59   60   61   62   65   66   67   69
   7   13    8    6    5    7    4    4    3    5    6    4    2    1    1    3
  70   71   72   74   75   76   77   78   79   81   83   84   88   90   95   97
   2    2    1    2    1    1    1    1    4    1    1    1    1    1    1    1
  98  101  110  113
   1    1    1    1
20000
     \endverbatim
     Clearly worse than rots. </li>
    </ol>
   </li>
   <li> Trying to determine the best ubcsat-1-2-0-algorithm:
   \verbatim
E=run_ubcsat("VanDerWaerden_2-3-39_1415.cnf", cutoff=100000,runs=100)

ag2wsat
 3  4  5  6  7  8  9 11 24 26 28 29 30 31 32 33 34 35 36 37 38 39 40
 1  9 11 12  2  3  1  1  2  1  1  7  5  4 11  9  3  6  3  4  2  1  1
ddfw
 3  4  5  6  7  8  9 10 11 13 16 17 20 21 22 23 25 26 27 28 29 30 31 32
 1  1  6 13  8  9  2  1  3  1  1  1  1  3  1  2  6 11  9  8  7  3  1  1
anovp
 4 46 48 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74
 1  1  1  1  2  1  1  4  3  7  4  3  2  7  4  5  7  6  4 12  6  4  3  4  1  4
75 76
 1  1
paws
 6  7  8  9 10 11 12 13 14 15 18 23 28 29 30 31 32 33 34 35 36 37 38 40
 2  3  3  7  2  2  3  1  2  1  1  1  1  2  4  4  5 11  9  8 15  8  4  1
saps
 9 12 13 15 16 21 26 32 33 34 35 36 37 38 39 40 41 43 44
 2  2  1  1  1  1  1  4  3  3  4  8 18 10 14 17  8  1  1
sapsnr
11 12 13 14 15 16 17 18 19 21 29 31 33 34 35 36 37 38 39 40 41 42 43
 1  1  2  1  2  1  5  2  2  1  2  3  3  7  2  9 10 12 14 12  6  1  1

> exp_dir="ubcsat_tmp_VanDerWaerden_2-3-39_1415.cnf_2011-01-11-173950"; for F in ${exp_dir}/*.run_ubcsat_stats; do echo -ne "$(basename $F .run_ubcsat_stats)\t"; grep "Flips" $F; done
ag2wsat FlipsPerSecond = 158103
anovpp  FlipsPerSecond = 168805
anovp   FlipsPerSecond = 167842
cwalk   FlipsPerSecond = 154083
cwalks  FlipsPerSecond = 153728
dano    FlipsPerSecond = 171116
ddfw    FlipsPerSecond = 231374
dwalk   FlipsPerSecond = 164745
g2wsat  FlipsPerSecond = 160026
gsat    FlipsPerSecond = 156079
gsats   FlipsPerSecond = 154250
gsatt   FlipsPerSecond = 154178
gwsat   FlipsPerSecond = 158781
hsat    FlipsPerSecond = 157085
hwsat   FlipsPerSecond = 158328
irots   FlipsPerSecond = 154967
novpc   FlipsPerSecond = 161681
novp_p  FlipsPerSecond = 158303
novpp   FlipsPerSecond = 153988
novp    FlipsPerSecond = 168606
nov     FlipsPerSecond = 167757
paws    FlipsPerSecond = 161577
rgsat   FlipsPerSecond = 162575
rnovp   FlipsPerSecond = 170358
rnov    FlipsPerSecond = 171145
rots    FlipsPerSecond = 154679
rsaps   FlipsPerSecond = 156838
samd    FlipsPerSecond = 154488
sapsnr  FlipsPerSecond = 162522
saps    FlipsPerSecond = 163479
uwalk   FlipsPerSecond = 154226
vw1     FlipsPerSecond = 163693
vw2     FlipsPerSecond = 163079
wsat    FlipsPerSecond = 163586
wsattn  FlipsPerSecond = 154703
wsatt   FlipsPerSecond = 154823

> cat VanDerWaerden_2-3-39_1415.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n 
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1415 525490 2499546 0 2499546 4 1
 length count
3 499849
39 25641
  \endverbatim
   We need to consider cutoff=10^6.
   </li>
   <li>
   \verbatim
On csoberon:

E=run_ubcsat("~/AltExp/Exp_VanderWaerden_2-3-39_1415_rots-100-100000000_2011-01-06-203740/VanDerWaerden_2-3-39_1415.cnf", cutoff=1000000,runs=100)

ddfw
 1  2  3  4  5  6  7
 3  7 29 29 24  6  2
ag2wsat
 1  2  3  4  5  6 21 23
 2 11 21 37 18  9  1  1
paws
 1  2  3  4  5  6  7  8  9 10 12 13 15 28 30
 1  1  3  7 22 16 23  9  9  2  2  2  1  1  1
rots
 3  4  5  6  7  9 10 11 19 21 22 23 24 25 26 27 28 29 30 31
 1  1  5  7  4  3  1  1  1  2  5 10 14 12 13 10  3  4  2  1
anovpp
 4  5 22 33 34 36 39 41 43 44 45 46 48 49 50 51 52 53 54 55 56 57 58 59 60 61
 1  1  1  1  1  1  1  1  1  2  1  4  4  4  5  3  5  6  6  5  6  5 12  6  6  1
62 63 65
 5  4  1

The FlipsPerSecond are likely corrupted.
   \endverbatim
   From the saps-family sapsnr seems best. Altogether ddfw and ag2wsat seem
   clearly best. </li>
   <li> n=1416:
    <ol>
     <li> rots with cutoff=2*10^8
     \verbatim
 2  3  4  5
 7 28 36  3
74
     \endverbatim
     </li>
     <li> ddfw found a solution in run 108 (osteps=1366102,
     cutoff=2*10^6):
     \verbatim
34,47,63,71,93,100,101,112,138,140,
167,169,204,211,214,228,251,262,266,285,
293,297,302,317,339,340,373,396,399,404,
408,410,428,451,463,473,488,502,510,545,
556,561,599,626,630,632,635,650,667,672,
673,685,706,741,743,746,767,784,796,804,
840,843,877,878,885,889,915,917,922,928,
946,954,981,996,1028,1033,1055,1062,1079,1092,
1094,1099,1137,1154,1173,1174,1191,1210,1240,1259,
1261,1296,1304,1316,1322,1329,1335,1337,1338,1375,
1377,1408,1410
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
   <li> Trying to determine best ubcsat-1.2-algorithm:
   \verbatim
E=run_ubcsat("VanDerWaerden_pd_2-3-39_1410.cnf", cutoff=1000000,runs=100)

samd
 1  2  3  4  5  6  7
 9 16 30 26 12  5  2
gsatt
 1  2  3  4  5  6
 8 25 34 19 10  4
ddfw
 2  3  4  5  6  7
 1  4 15 39 28 13
anovpp
 3  4  5  6  7  8  9 10 11 12 13 14 17
 1  3  2  9  9 11 17 19 10  9  6  3  1
anovp
 3  4  5  6  7  8  9 10 11 12 13 14 15
 1  2  5  7  6 12 13 15 17 12  8  1  1
rots
 4  5  6  7  8  9
 5  6 15 46 18 10
ag2wsat
 4  5  6  7  8  9 10
 4 11 17 41 12 10  5
hwsat
 4  5  6  7  8  9 10 11 12
 4  4 11 15 20 19 17  6  4
dano
 4  6  7  8  9 10 11 12 13 14 15
 2  5  7 17 17 11 13 11 12  2  3
wsattn
 4 10 11 12 13 14 15 16 17 18 19 20 21 22
 1  4  6  5  6  6 22 14  7 12  8  5  3  1
paws
 5  6  7  8  9 10 11
 2  6 24 26 26 14  2

> exp_dir="ubcsat_tmp_VanDerWaerden_pd_2-3-39_1410.cnf_2011-01-11-191236"; for F in ${exp_dir}/*.run_ubcsat_stats; do echo -ne "$(basename $F .run_ubcsat_stats)\t"; grep "Flips" $F; done
ag2wsat FlipsPerSecond = 1600512
anovpp  FlipsPerSecond = 1602821
anovp   FlipsPerSecond = 7961783
cwalk   FlipsPerSecond = 1685204
cwalks  FlipsPerSecond = 1680108
dano    FlipsPerSecond = 5015045
ddfw    FlipsPerSecond = 12484395
dwalk   FlipsPerSecond = 1895016
g2wsat  FlipsPerSecond = 1603849
gsat    FlipsPerSecond = 1637197
gsats   FlipsPerSecond = 1552072
gsatt   FlipsPerSecond = 1753463
gwsat   FlipsPerSecond = 1898614
hsat    FlipsPerSecond = 1561280
hwsat   FlipsPerSecond = 1819505
irots   FlipsPerSecond = 1614466
novpc   FlipsPerSecond = 2929974
novp_p  FlipsPerSecond = 1679825
novpp   FlipsPerSecond = 8216927
novp    FlipsPerSecond = 5260389
nov     FlipsPerSecond = 5321980
paws    FlipsPerSecond = 2770851
rgsat   FlipsPerSecond = 2708559
rnovp   FlipsPerSecond = 6365372
rnov    FlipsPerSecond = 8000000
rots    FlipsPerSecond = 2084636
rsaps   FlipsPerSecond = 2283105
samd    FlipsPerSecond = 1785077
sapsnr  FlipsPerSecond = 1804077
saps    FlipsPerSecond = 1829157
uwalk   FlipsPerSecond = 1666111
vw1     FlipsPerSecond = 2314279
vw2     FlipsPerSecond = 2251745
wsat    FlipsPerSecond = 2296211
wsattn  FlipsPerSecond = 2321802
wsatt   FlipsPerSecond = 2322880
   \endverbatim
   Are these FPS'values correct?
   </li>
  </ul>

*/
