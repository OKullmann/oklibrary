// Oliver Kullmann, 14.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/38.hpp
  \brief On investigations into vdw_2(3,38)


  \todo vanderwaerden_2(3,38) > 1375
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
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Trying to determine best ubcsat-1.2-algorithm:
    <ol>
     <li>
     \verbatim
E=run_ubcsat("VanDerWaerden_2-3-38_1376.cnf", cutoff=100000,runs=100, include_algs=names(new_run_ubcsat_cnf_algs), ubcsat_wrapper="new-ubcsat-okl")
     \endverbatim
     </li>
     <li> Regarding the algorithms mentioned below, most successful are
     \verbatim
ag2wsat
 3  4  5  6  7  8  9 27 28 31 32 33 34 35 36 37 38 42 
 3 14 33 19  5  7  1  1  1  2  2  3  1  1  1  3  1  2 
ddfw
 3  4  5  6  7  8  9 10 11 12 13 24 25 26 27 28 30 31 32 34 
 1  3 13  9 20 14  9  9  2  3  1  2  2  3  1  2  1  2  2  1 
paws
 5  6  7  8  9 10 11 12 13 14 15 29 30 33 34 35 36 37 38 39 40 41 
 1  4  2  9 10  8  6  5  1  3  2  2  1  3  3  7  7  7  7  6  4  2 
anovpp
 7 51 53 54 55 57 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 
 1  1  2  2  1  3  3  3  1  3  1  4  4  2  5  7  3  7 10  7  6  7  6  4  3  2 
79 
 2
rots
 8 15 22 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 
 1  1  1  1  1  1  5 10  9  7  5 14 14  8  3  7  4  4  2  2 
wsatt
11 12 13 14 15 16 17 18 19 20 21 22 23 25 26 30 31 32 33 34 35 36 37 38 39 40 
 1  1  4  2  5  9  9 17  4 10  6  3  3  1  1  1  1  1  2  1  1  3  2  2  1  1 
41 42 44 45 47 53 61 63 
 1  1  1  1  1  1  1  1 
wsattn
12 13 14 15 16 17 18 19 20 21 22 23 24 25 28 29 31 32 33 34 35 37 38 39 40 41 
 1  1  7  3 10 10 11 11  3  4  3  3  2  1  1  1  1  1  1  2  3  2  2  3  3  1 
45 50 51 55 60 64 65 72 
 2  1  1  1  1  1  1  1 
rots
12 24 25 27 28 29 30 31 32 33 34 35 36 37 38 40 41 42 43 
 1  1  3  1  1  5  4  7  8  9  6 19  6 11 10  2  3  1  2 
     \endverbatim
     The first two are very slow. </li>
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

> cat VanDerWaerden_2-3-38_1376.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1376 497557 2364206 0 2364206 4 1
 length count
3 472656
38 24901

 exp_dir="ubcsat_tmp_VanDerWaerden_2-3-38_1376.cnf_2011-01-12-233649"; for F in ${exp_dir}/*.run_ubcsat_stats; do echo -ne "$(basename $F .run_ubcsat_stats)\t"; grep "Flips" $F; done 
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

     \endverbatim
     (there have been segmentation faults; need to rerun it with our newest
     version of ubcsat-1-2-0).
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
