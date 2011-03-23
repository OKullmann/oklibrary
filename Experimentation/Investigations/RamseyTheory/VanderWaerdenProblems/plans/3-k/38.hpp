// Oliver Kullmann, 14.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/38.hpp
  \brief On investigations into vdw_2(3,38)


  \todo vanderwaerden_2(3,38) > 1378
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
   <li> n=1375 found satisfiable with certificate
   \verbatim
6,30,49,78,80,90,110,123,127,141,
146,149,184,213,225,260,271,274,275,287,
297,312,331,332,339,343,363,380,385,386,
406,408,443,459,474,509,511,517,528,530,
561,570,590,602,604,607,620,635,639,641,
657,664,701,715,718,733,746,756,775,778,
787,813,830,844,852,863,887,889,894,916,
923,941,963,968,974,1005,1009,1014,1029,1035,
1064,1066,1072,1075,1079,1088,1111,1120,1138,1140,
1146,1157,1163,1185,1222,1249,1268,1273,1293,1311,
1330,1342,1347,1374
   \endverbatim
   </li>
   <li> n=1376, with rots:
    <ol>
     <li> cutoff=1*10^6:
     \verbatim
   1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16
   1   13  131  496 1063 1391 1059  558  287  139   80   46   23   11   15   24
  17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32
  31   33   60  101  123  179  305  412  451  594  623  504  362  202   84   34
  33   34 
   6    3 
9444 
     \endverbatim
     </li>
     <li> cutoff=2*10^6:
     \verbatim
 3  4  5  6  7  8 10 15 16 22 24 26 27 28 
 3 16 32 22 10  4  1  1  2  2  2  2  2  1 
100 
  1   2   3   4   5   6   7   8   9  10  11  12  13  14  16  17  18  19  20  21
  1  11  48 155 243 206  91  43  18   9   1   3   1   3   1   2   5   6   7   8
 22  23  24  25  26  27  28  29 
 20  17  16  36  19  20   9   1 
1000
   1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   1 
   4   41  216  728 1133  909  488  207   79   25   15   12    7    4    4    4
  17   18   19   20   21   22   23   24   25   26   27   28   29   30 
  10   12   24   36   47   68   81   97   95  116   75   38    9    1 
4585 
     \endverbatim
     </li>
     <li> cutoff=5*10^6:
     \verbatim
 2  3  4  5  6  7 15 18 26 
10 56 92 88 22  8  1  1  1 
279 
     \endverbatim
     </li>
     <li> cutoff=1*10^7:
     \verbatim
  2   3   4   5   6 
 15  82 114  49   5 
265 
 1  2  3  4  5 
 1  8 16 36 17 
78 
     \endverbatim
     </li>
     <li> cutoff=2*10^7:
     \verbatim
  1   2   3   4   5   6 
  8  77 210 149  16   1 
461
 1  2  3  4  5 
 1 20 69 39  4 
133 
 1  2  3  4  5 
 1 12 23 13  7 
56 
     \endverbatim
     </li>
     <li> cutoff=1*10^8:
     \verbatim
 1  2  3  4 
 6 49 31  1 
87 
     \endverbatim
     </li>
     <li> cutoff=2*10^8:
     \verbatim
 1  2  3 
 5 18  9 
32 
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Trying to determine best ubcsat-1-2-0-algorithm:
    <ol>
     <li>
     \verbatim
E=run_ubcsat("VanDerWaerden_2-3-38_1376.cnf", cutoff=100000,runs=100)
     \endverbatim
     </li>
     <li> Regarding the algorithms mentioned below, most successful are
     \verbatim
ag2wsat
 3  4  5  6  7  8  9 27 28 31 32 33 34 35 36 37 38 42 
 3 14 33 19  5  7  1  1  1  2  2  3  1  1  1  3  1  2 
 3  4  5  6  7  8  9 10 18 21 22 28 29 30 31 32 34 35 36 37 38 39 40 41 
 4 15 28 10  4  4  3  1  1  1  1  1  2  1  2  2  1  2  7  4  1  2  1  2 
FlipsPerSecond = 9909    
ddfw
 3  4  5  6  7  8  9 10 11 12 13 24 25 26 27 28 30 31 32 34 
 1  3 13  9 20 14  9  9  2  3  1  2  2  3  1  2  1  2  2  1 
 3  4  5  6  7  8  9 10 11 12 17 21 24 25 26 27 28 29 32 33 
 1  5  8  6 15 18 10  6  3  3  1  1  4  1  4  5  2  4  1  2 
FlipsPerSecond = 1578    
paws
 5  6  7  8  9 10 11 12 13 14 15 29 30 33 34 35 36 37 38 39 40 41 
 1  4  2  9 10  8  6  5  1  3  2  2  1  3  3  7  7  7  7  6  4  2 
 6  7  8  9 10 11 12 13 14 16 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 
 2  2 10 11  7  9  6  1  2  1  1  1  2  2  2  4  6  3  6  5  9  3  2  1  2 
FlipsPerSecond = 8229  
sapsnr
 6 10 12 14 15 16 17 19 21 22 32 34 35 36 37 38 39 40 41 42 43 44 45 46 
 1  2  5  3  4  4  3  2  2  1  1  1  1  5  3  3  6  7 17 10  5  7  6  1 
FlipsPerSecond = 7212
anovpp
 7 51 53 54 55 57 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 
 1  1  2  2  1  3  3  3  1  3  1  4  4  2  5  7  3  7 10  7  6  7  6  4  3  2 
79 
 2
 5 19 26 38 50 53 55 56 57 58 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 
 1  1  1  1  1  2  3  2  4  2  5  2  6  3  4  5  4  4  6  7  5  9  6  2  5  2 
76 77 78 
 2  4  1 
FlipsPerSecond = 2147
dano
 7 54 55 56 59 60 61 62 63 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 81 82 
 1  1  1  2  5  3  4  2  2  2  3  4  4  9  7 10  4  6 13  3  4  2  2  4  1  1 
FlipsPerSecond = 2137
rots
 8 15 22 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 
 1  1  1  1  1  1  5 10  9  7  5 14 14  8  3  7  4  4  2  2 
12 24 25 27 28 29 30 31 32 33 34 35 36 37 38 40 41 42 43 
 1  1  3  1  1  5  4  7  8  9  6 19  6 11 10  2  3  1  2 
19 25 26 28 29 30 31 32 33 34 35 36 37 38 39 40 41 
 1  2  1  2  8  4  6  7 12  8  9 12 10  6  6  4  2 
FlipsPerSecond = 13596
wsatt
11 12 13 14 15 16 17 18 19 20 21 22 23 25 26 30 31 32 33 34 35 36 37 38 39 40 
 1  1  4  2  5  9  9 17  4 10  6  3  3  1  1  1  1  1  2  1  1  3  2  2  1  1 
41 42 44 45 47 53 61 63 
 1  1  1  1  1  1  1  1 
11 12 13 14 15 16 17 18 19 20 21 22 23 24 26 28 29 31 32 34 35 36 37 38 39 42 
 1  1  3  4  3 11 10  9  7  1  6  3  2  2  1  1  2  1  2  3  1  2  1  4  5  2 
43 45 47 52 54 58 59 67 71 76 
 2  1  2  1  1  1  1  1  1  1 
FlipsPerSecond = 2596    
wsattn
12 13 14 15 16 17 18 19 20 21 22 23 24 25 28 29 31 32 33 34 35 37 38 39 40 41 
 1  1  7  3 10 10 11 11  3  4  3  3  2  1  1  1  1  1  1  2  3  2  2  3  3  1 
45 50 51 55 60 64 65 72 
 2  1  1  1  1  1  1  1 
10 13 14 15 16 17 18 19 20 21 22 23 25 27 28 32 33 34 35 36 37 39 41 42 43 46 
 1  1  3  7  7 11 11  9  6  6  5  3  1  2  1  1  2  1  6  1  2  2  1  2  1  1 
47 55 56 58 63 66 
 1  1  1  1  1  1 
FlipsPerSecond = 2541    
     \endverbatim
     So ag2wsat seems the clear winner.
     </li>
     <li> With cutoff=10^6:
     \verbatim
E=run_ubcsat("VanDerWaerden_2-3-38_1376.cnf", cutoff=1000000,runs=100)

ddfw
 1  2  3  4  5  6  7  8  9
 1  7 22 29 20 14  5  1  1
ag2wsat
 2  3  4  5  6
14 22 35 15 14
paws
 2  3  4  5  6  7  8  9
 1  2 11 15 29 22 13  7
rots
 3  4  5  6  7  8 10 11 16 20 21 22 23 24 26 27 28 29 30 31
 2  7 12 16  9 10  1  1  1  3  2  1  4  3  5  7  5  7  3  1
anovpp
 3 26 32 41 44 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67
 2  1  1  2  1  2  3  1  3  1  3  2  3  5  6 11 12  5  9  5  2  8  3  5  3  1
     \endverbatim
     So ddfw and ag2wsat are the winners (where apparently ag2wsat is faster).
     </li>
     <li> It seems very slow? (times on csltok):
     \verbatim
> exp_dir="ubcsat_tmp_VanDerWaerden_2-3-38_1376.cnf_2011-01-11-104249"; for F in ${exp_dir}/*.run_ubcsat_stats; do echo -ne "$(basename $F .run_ubcsat_stats)\t"; grep "Flips" $F; done
anovp   FlipsPerSecond = 2130
gsat    FlipsPerSecond = 36277
gsats   FlipsPerSecond = 40042
gsatt   FlipsPerSecond = 14703
gwsat   FlipsPerSecond = 9346
hsat    FlipsPerSecond = 34954
hwsat   FlipsPerSecond = 15282
irots   FlipsPerSecond = 10805
novp    FlipsPerSecond = 2160
nov     FlipsPerSecond = 2129
rnovp   FlipsPerSecond = 2188
rnov    FlipsPerSecond = 2193
rots    FlipsPerSecond = 12589
samd    FlipsPerSecond = 14706
wsat    FlipsPerSecond = 2583
wsattn  FlipsPerSecond = 2595
wsatt   FlipsPerSecond = 2537
> exp_dir="ubcsat_tmp_VanDerWaerden_2-3-38_1376.cnf_2011-01-12-233649"; for F in ${exp_dir}/*.run_ubcsat_stats; do echo -ne "$(basename $F .run_ubcsat_stats)\t"; grep "Flips" $F; done 
ag2wsat FlipsPerSecond = 9408   
anovpp  FlipsPerSecond = 2127   
anovp   FlipsPerSecond = 2144   
ddfw    FlipsPerSecond = 1556   
g2wsat  FlipsPerSecond = 9120   
gsat    FlipsPerSecond = 36228  
gsats   FlipsPerSecond = 40717  
gsatt   FlipsPerSecond = 14945  
gwsat   FlipsPerSecond = 9495   
hsat    FlipsPerSecond = 38178  
hwsat   FlipsPerSecond = 15226  
irots   FlipsPerSecond = 10668  
novpc   FlipsPerSecond = 7061   
novp_p  FlipsPerSecond = 4481   
novpp   FlipsPerSecond = 2256   
novp    FlipsPerSecond = 2156   
nov     FlipsPerSecond = 2118   
paws    FlipsPerSecond = 7686   
rnovp   FlipsPerSecond = 2164   
rnov    FlipsPerSecond = 2137   
rots    FlipsPerSecond = 12731  
samd    FlipsPerSecond = 14908  
vw1     FlipsPerSecond = 2488   
wsat    FlipsPerSecond = 2556   
wsattn  FlipsPerSecond = 2597   
wsatt   FlipsPerSecond = 2535   
> exp_dir="ubcsat_tmp_VanDerWaerden_2-3-38_1376.cnf_2011-01-14-003347/"; for F in ${exp_dir}/*.run_ubcsat_stats; do echo -ne "$(basename $F .run_ubcsat_stats)\t"; grep "Flips" $F; done
ag2wsat FlipsPerSecond = 9909   
anovpp  FlipsPerSecond = 2147   
anovp   FlipsPerSecond = 2170   
cwalk   FlipsPerSecond = 26665  
cwalks  FlipsPerSecond = 26579  
dano    FlipsPerSecond = 2137   
ddfw    FlipsPerSecond = 1578   
dwalk   FlipsPerSecond = 6655   
g2wsat  FlipsPerSecond = 9963   
gsat    FlipsPerSecond = 37281  
gsats   FlipsPerSecond = 43914  
gsatt   FlipsPerSecond = 16241  
gwsat   FlipsPerSecond = 10212  
hsat    FlipsPerSecond = 38755  
hwsat   FlipsPerSecond = 16592  
irots   FlipsPerSecond = 11416  
novpc   FlipsPerSecond = 7761   
novp_p  FlipsPerSecond = 4934   
novpp   FlipsPerSecond = 2240   
novp    FlipsPerSecond = 2187   
nov     FlipsPerSecond = 2171   
paws    FlipsPerSecond = 8229   
rgsat   FlipsPerSecond = 7059   
rnovp   FlipsPerSecond = 2154   
rnov    FlipsPerSecond = 2163   
rots    FlipsPerSecond = 13596  
rsaps   FlipsPerSecond = 7461   
samd    FlipsPerSecond = 15867  
sapsnr  FlipsPerSecond = 7212   
saps    FlipsPerSecond = 7246   
uwalk   FlipsPerSecond = 29621  
vw1     FlipsPerSecond = 2519   
vw2     FlipsPerSecond = 2534   
wsat    FlipsPerSecond = 2570   
wsattn  FlipsPerSecond = 2541   
wsatt   FlipsPerSecond = 2596   

These numbers seem correct. They were produced under full load of the
quadcore-processor on csltok (see below for data). Running just a
single process yields much better performance, and thus it seems
a cache-problem:
E=run_ubcsat("AltExp/Exp_VanderWaerden_2-3-38_1376_rots-10000-1000000_2011-01-06-111944/VanDerWaerden_2-3-38_1376.cnf", cutoff=100000,runs=10)
> exp_dir="ubcsat_tmp_VanDerWaerden_2-3-38_1376.cnf_2011-01-15-105402/"; for F in ${exp_dir}/*.run_ubcsat_stats; do echo -ne "$(basename $F .run_ubcsat_stats)\t"; grep "Flips" $F; done
ag2wsat FlipsPerSecond = 26008  
anovpp  FlipsPerSecond = 6741   
anovp   FlipsPerSecond = 6996   
cwalk   FlipsPerSecond = 91575  
cwalks  FlipsPerSecond = 90090  
dano    FlipsPerSecond = 6865   
ddfw    FlipsPerSecond = 3782   
dwalk   FlipsPerSecond = 13428  
g2wsat  FlipsPerSecond = 20429  
gsat    FlipsPerSecond = 77640  
gsats   FlipsPerSecond = 71736  
gsatt   FlipsPerSecond = 49603  
gwsat   FlipsPerSecond = 21580  
hsat    FlipsPerSecond = 65789  
hwsat   FlipsPerSecond = 40258  
irots   FlipsPerSecond = 22826  
novpc   FlipsPerSecond = 16537  
novp_p  FlipsPerSecond = 10361  
novpp   FlipsPerSecond = 7013   
novp    FlipsPerSecond = 6850   
nov     FlipsPerSecond = 6718   
paws    FlipsPerSecond = 17458  
rgsat   FlipsPerSecond = 14296  
rnovp   FlipsPerSecond = 6666   
rnov    FlipsPerSecond = 6871   
rots    FlipsPerSecond = 35765  
rsaps   FlipsPerSecond = 15557  
samd    FlipsPerSecond = 50075  
sapsnr  FlipsPerSecond = 14881  
saps    FlipsPerSecond = 14999  
uwalk   FlipsPerSecond = 94697  
vw1     FlipsPerSecond = 9359   
vw2     FlipsPerSecond = 9698   
wsat    FlipsPerSecond = 9905   
wsattn  FlipsPerSecond = 9731   
wsatt   FlipsPerSecond = 10515  

> cat VanDerWaerden_2-3-38_1376.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1376 497557 2364206 0 2364206 4 1
 length count
3 472656
38 24901
     \endverbatim
     </li>
     <li> Compared with the flips per second in plans/3-k/39.hpp these times
     are completely out of order. </li>
     <li> Run version 1-0-0 on csltok:
     \verbatim
> exp_dir="ubcsat_tmp_VanDerWaerden_2-3-38_1376.cnf_2011-01-13-005548"; for F in ${exp_dir}/*.run_ubcsat_stats; do echo -ne "$(basename $F .run_ubcsat_stats)\t"; grep "Flips" $F; done 
gsat    FlipsPerSecond = 36547  
gsats   FlipsPerSecond = 38438  
gwsat   FlipsPerSecond = 9654
     \endverbatim
     Very similar. </li>
     <li> We have on csltok a quadcore Intel i5 CPU with info
     \verbatim
67: None 00.0: 10103 CPU
  [Created at cpu.301]
  Unique ID: rdCR.j8NaKXDZtZ6
  Hardware Class: cpu
  Arch: X86-64
  Vendor: "GenuineIntel"
  Model: 6.37.5 "Intel(R) Core(TM) i5 CPU       M 520  @ 2.40GHz"
  Features: fpu,vme,de,pse,tsc,msr,pae,mce,cx8,apic,sep,mtrr,pge,mca,cmov,pat,pse36,clflush,dts,acpi,mmx,fxsr,sse,sse2,ss,ht,tm,pbe,syscall,nx,rdtscp,lm,constant_tsc,arch_perfmon,pebs,bts,rep_good,xtopology,nonstop_tsc,aperfmperf,pni,pclmulqdq,dtes64,monitor,ds_cpl,vmx,smx,est,tm2,ssse3,cx16,xtpr,pdcm,sse4_1,sse4_2,popcnt,aes,lahf_lm,ida,arat,tpr_shadow,vnmi,flexpriority,ept,vpid
  Clock: 2400 MHz
  BogoMips: 4787.23
  Cache: 3072 kb
  Units/Processor: 16
  Config Status: cfg=no, avail=yes, need=no, active=unknown

68: None 01.0: 10103 CPU

69: None 02.0: 10103 CPU

70: None 03.0: 10103 CPU
     \endverbatim
     </li>
     <li> cutoff=10^6:
     \verbatim
E=run_ubcsat("VanDerWaerden_2-3-38_1376.cnf", cutoff=1000000,runs=100, include_algs=names(new_run_ubcsat_cnf_algs), ubcsat_wrapper="new-ubcsat-okl")

saps
 5  7  8  9 10 11 12 13 14 15 16 19
 4  4 13 17 19 16 13  7  3  2  1  1
anovp
 5  6  7 23 42 43 44 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65
 3  2  1  1  3  1  1  1  5  2  3  2  3  6  2  1  3  4  6  5  3 10  6  8  6  5
66 67 68 69
 4  1  1  1

interrupted; the FlipsPerSecond are likely corrupted.
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=1377: adaptg2wsat finds a solution (adapted from n=1376,
   osteps=18215068, using "RunVdW3k 38 1376 adaptg2wsat 1000 20000000"):
   \verbatim
3,27,30,38,60,61,64,78,97,106,
124,149,154,175,177,180,198,208,212,217,
223,227,251,265,288,309,326,334,339,365,
371,372,400,437,449,450,467,473,476,482,
487,510,531,541,556,584,585,594,598,619,
634,659,661,668,672,696,705,716,745,763,
782,783,790,804,827,841,852,853,883,890,
893,911,915,920,948,954,975,994,1000,1007,
1012,1029,1031,1037,1065,1078,1086,1096,1100,1111,
1137,1139,1170,1189,1214,1226,1248,1253,1259,1260,
1264,1296,1297,1305,1322,1338,1360,1361,1375,1376
   \endverbatim
   </li>
   <li> n=1378:
    <ol>
     <li> adaptg2wsat, cutoff=2*10^6:
     \verbatim
  1   2   3   4   5   6   7   8   9 
  1  69 306 766 653 318  81  15   2 
2211 
     \endverbatim
     </li>
     <li> adaptg2wsat, cutoff=4*10^6:
     \verbatim
  1   2   3   4   5   6   7 
 10  78 344 502 325 119  28 
1406 
     \endverbatim
     </li>
     <li> adaptg2wsat, cutoff=2*10^7:
     \verbatim
  0   1   2   3   4   5 
  1   5  65 131  67   8 
277 
     \endverbatim
     (see below for certificate). </li>
     <li> adaptg2wsat, cutoff=10^8:
     \verbatim
 1  2  3 
 8 24  8 
40 
     \endverbatim
     </li>
     <li> adaptg2wsat finds a solution in run 277 (cutoff=2*10^7,
     osteps=19945502) with certificate
     \verbatim
35,50,58,72,95,97,110,134,147,161,
163,164,193,198,201,226,237,245,275,280,
300,309,312,331,332,354,369,383,391,409,
410,415,442,446,448,459,480,497,516,526,
539,541,563,570,608,615,631,645,652,665,
674,707,712,718,738,742,753,775,776,781,
800,829,867,874,886,890,901,909,923,924,
929,964,970,975,1001,1007,1008,1012,1040,1051,
1057,1081,1086,1109,1118,1131,1148,1162,1163,1168,
1189,1200,1234,1242,1244,1259,1265,1293,1301,1303,
1310,1311,1331,1348
     \endverbatim
     </li>
     <li> It seems that this cutoff is about right. </li>
    </ol>
   </li>
   <li> n=1379:
    <ul>
     <li> adaptg2wsat, cutoff=2*10^7:
     \verbatim
 1  2  3  4  5 
 1  7 40 53 11 
112 
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo Palindromic version:
  <ul>
   <li> 1368 < pdvanderwaerden([3,38])[2] =(conj) 1369 =(conj)
   vanderwaerden([3,38]) - ???. </li>
   <li> pdvanderwaerden([3,38])[1] =(conj) 1336. </li>
   <li> Certificates:
    <ol>
     <li> n=1335:
     \verbatim
11,47,60,69,82,90,92,126,141,143,
149,163,166,174,197,232,233,247,255,282,
284,289,327,341,343,348,385,386,388,416,
430,432,440,459,469,470,477,507,526,527,
534,563,565,587,588,608,618,655,662

     \endverbatim
     </li>
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
   <li> "RunPdVdWk1k2 3 38 gsat-tabu 100 30000000" yields
   \verbatim
   \endverbatim
Break point 1: 1336
Break point 2: 1369
   where all solutions were found within in the first 17 runs. </li>
  </ul>

*/
