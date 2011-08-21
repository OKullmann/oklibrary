// Oliver Kullmann, 3.4.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/07.hpp
  \brief On investigations into vdw_2(5,7)


  \todo Best local search solver
  <ul>
   <li> Small cutoff 10^5:
   \verbatim
> E = run_ubcsat("VanDerWaerden_2-5-7_235.cnf", runs=100)
anovp
 2  3  4  5  6  7
 3  5 21 38 27  6
FlipsPerSecond = 101729
ddfw
 2  3  4  5  6
 2 19 42 28  9
FlipsPerSecond = 13702
rnov
 2  3  4  5  6  7
 1 18 20 41 18  2
FlipsPerSecond = 85056
ag2wsat
 2  3  4  5  6  7
 1 16 33 36 12  2
FlipsPerSecond = 82217
rnovp
 2  3  4  5  6
 1  8 43 33 15
FlipsPerSecond = 61660
anovpp
 2  3  4  5  6  7
 1  7 18 41 29  4
FlipsPerSecond = 59400
novpc
 2  5  6  7  8  9 10 11 12 13 14
 1  2  1  7 12 18 21 19 10  7  2
FlipsPerSecond = 58668
dano
 3  4  5  6  7
 9 25 40 22  4
FlipsPerSecond = 142167
novp
 3  5  6  7  8  9 10 11 12 13 14
 2  1  4  4  9 18 25 12 18  6  1
FlipsPerSecond = 120163
rots
 3  4  5  6  7  8  9
 1  2 12 28 40 16  1
FlipsPerSecond = 81294
paws
 4  5  6  7  8  9 10 11
 4  3 10 19 33 24  6  1
FlipsPerSecond = 69147
irots
 4  5  6  7  8  9 10
 2  4 10 18 44 17  5
FlipsPerSecond = 73190
samd
 4  5  6  7  8  9 10 11 13 16
 2  4  7 21 23 18 18  5  1  1
FlipsPerSecond = 98386
saps
 4  5  6  7  8  9 10
 2  3  8 23 31 27  6
FlipsPerSecond = 54139
g2wsat
 4  5  6  7  8  9 10 11
 2  1  7 28 27 25  8  2
FlipsPerSecond = 26948
   \endverbatim
   Segmentation faults with rsaps, sapsnr (csltok). </li>
   <li> Higher cutoff 10^6:
   \verbatim
> E = run_ubcsat("VanDerWaerden_2-5-7_235.cnf", runs=100,cutoff=1000000, include_algs=list("anovp","ddfw","rnov","ag2wsat","rnovp","anovpp","novpc","dano","novp","rots","paws","irots","samd","saps","g2wsat"))

ddfw
 0  2  3  4
 3 34 54  9
FlipsPerSecond = 16350
ag2wsat
 0  2  3  4
 2 19 61 18
FlipsPerSecond = 58708
paws
 0  2  3  4  5  6  7
 1  1  2 12 29 37 18
FlipsPerSecond = 46145
anovp
 1  2  3  4  5
 3 16 47 32  2
FlipsPerSecond = 28585
rnov
 1  2  3  4  5
 1 16 54 26  3
FlipsPerSecond = 74061
anovpp
 1  2  3  4
 1 11 52 36
FlipsPerSecond = 30302
dano
 2  3  4  5
15 45 39  1
FlipsPerSecond = 41675
rnovp
 2  3  4
 7 53 40
FlipsPerSecond = 55746
rots
 2  3  4  5  6
 3  7 25 45 20
FlipsPerSecond = 40712
novp
 2  3  4  5  6  7  8  9 10
 1  7  3 15 22 29 19  3  1
FlipsPerSecond = 29835
novpc
 2  3  4  5  6  7  8  9
 1  1 13 13 19 28 20  5
FlipsPerSecond = 32568
g2wsat
 3  4  5  6  7  8
 4 17 23 37 18  1
FlipsPerSecond = 78934
samd
 3  4  5  6  7  8  9 10
 2  1 14 22 32 19  7  3
FlipsPerSecond = 128813
saps
 3  4  5  6  7  8
 1  4 21 35 38  1
FlipsPerSecond = 28160
irots
 4  5  6  7  8
 9 25 46 19  1
FlipsPerSecond = 36925
   \endverbatim
   So ddfw is best (but slow), followed by adaptg2wsat. The FPS numbers at
   least on csltok are unreliable. </li>
  </ul>


  \todo vdw_2(5,7) >= 260
  <ul>
   <li> The conjecture is vdw_2(5,7) = 260. </li>
   <li> Certificate for n=259:
   \verbatim
 7,12,18,19,20,21,26,27,28,29,
31,32,33,37,42,44,45,51,52,54,
59,63,64,65,67,68,69,70,72,75,
76,78,79,82,84,85,86,87,89,90,
91,95,100,102,103,109,110,112,117,121,
122,123,125,126,127,128,130,133,134,136,
137,140,142,143,144,145,147,148,149,153,
158,160,161,167,168,170,175,179,180,181,
183,184,185,186,188,191,192,194,195,198,
200,201,202,203,205,206,207,211,218,219,
222,225,226,228,233,238,241,242,243,246,
249,251,256
  \endverbatim
   </li>
   <li> Using the palindromic solution for n=235:
    <ol>
     <li> Starting search with the best palindromic solution:
     \verbatim
> k1=5 k2=7 n=235 alg="gsat-tabu" cutoff=200000 runs=100 expdate="2011-04-03-065559"; export k1 k2 n; cat AltExp/Exp_PdVanderWaerden_2-${k1}-${k2}_${alg}-${runs}-${cutoff}_${expdate}/VanDerWaerden_pd_2-${k1}-${k2}_${n}.cnf_sol | PdExtend-O3-DNDEBUG ${n} > solution

> RunVdWk1k2 ${k1} ${k2} ${n} ddfw 100 2000000 solution
UNSAT for n=252
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-7_235_ddfw-100-2000000_2011-04-03-071213/VanDerWaerden_2-5-7_252.cnf_OUT",nrows=100)
 1  2  3  4  5  6  7  8
 1  4  5  1 13 37 36  3
100

> n=251; RunVdWk1k2 ${k1} ${k2} ${n} adaptg2wsat 100 5000000 AltExp/Exp_VanderWaerden_2-5-7_235_ddfw-100-2000000_2011-04-03-071213/VanDerWaerden_2-5-7_${n}.cnf_sol
UNSAT for n=256
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-7_251_adaptg2wsat-100-5000000_2011-04-03-134354/VanDerWaerden_2-5-7_256.cnf_OUT",nrows=100)
 2  3  4  5  6  7  8  9
 3  7  3  2 23 39 22  1
100

> n=255; RunVdWk1k2 ${k1} ${k2} ${n} ddfw 100 5000000 AltExp/Exp_VanderWaerden_2-5-7_251_adaptg2wsat-100-5000000_2011-04-03-134354/VanDerWaerden_2-5-7_${n}.cnf_sol
UNSAT for n=258
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-7_255_ddfw-100-5000000_2011-04-03-191151/VanDerWaerden_2-5-7_258.cnf_OUT",nrows=100)
 2  3  4  5  6  7  8
 9  2  5  8 18 46 12
100

> n=257; RunVdWk1k2 ${k1} ${k2} ${n} adaptg2wsat 100 10000000 AltExp/Exp_VanderWaerden_2-5-7_255_ddfw-100-5000000_2011-04-03-191151/VanDerWaerden_2-5-7_${n}.cnf_sol
UNSAT for n=258
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-7_257_adaptg2wsat-100-10000000_2011-04-04-150849/VanDerWaerden_2-5-7_258.cnf_OUT",nrows=100)
 2  3  4  5  6  7  8
17  9  3 11 11 33 16
100

> RunVdWk1k2 ${k1} ${k2} ${n} ddfw 100 10000000 AltExp/Exp_VanderWaerden_2-5-7_255_ddfw-100-5000000_2011-04-03-191151/VanDerWaerden_2-5-7_${n}.cnf_sol
UNSAT for n=258
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-7_257_ddfw-100-10000000_2011-04-04-163842/VanDerWaerden_2-5-7_258.cnf_OUT",nrows=100)
 1  2  3  4  5  6  7  8
 1 19  6  3 10 16 42  3
100

> RunVdWk1k2 ${k1} ${k2} ${n} adaptg2wsat 200 20000000 AltExp/Exp_VanderWaerden_2-5-7_255_ddfw-100-5000000_2011-04-03-191151/VanDerWaerden_2-5-7_${n}.cnf_sol
UNSAT for n=259
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-7_257_adaptg2wsat-200-20000000_2011-04-05-014108/VanDerWaerden_2-5-7_259.cnf_OUT",nrows=100)
 2  3  4  5  6  7  8
19 15  8 15  4 33  6
100

> n=258; RunVdWk1k2 ${k1} ${k2} ${n} ddfw 200 20000000 AltExp/Exp_VanderWaerden_2-5-7_257_adaptg2wsat-200-20000000_2011-04-05-014108/VanDerWaerden_2-5-7_${n}.cnf_sol
UNSAT for n=260
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-7_258_ddfw-200-20000000_2011-04-05-101330/VanDerWaerden_2-5-7_260.cnf_OUT",nrows=200)
 1  2  3  4  5  6  7  8
 3 31 49 15 31 20 47  4
200

> n=259; RunVdWk1k2 ${k1} ${k2} ${n} adaptg2wsat 200 40000000 AltExp/Exp_VanderWaerden_2-5-7_258_ddfw-200-20000000_2011-04-05-101330/VanDerWaerden_2-5-7_${n}.cnf_sol
UNSAT for n=260
> E=read_ubcsat("Exp_VanderWaerden_2-5-7_259_adaptg2wsat-200-40000000_2011-04-06-104600/VanDerWaerden_2-5-7_260.cnf_OUT",nrows=200)
 1  2  3  4  5  6  7  8
 4 41 71 18 23 17 25  1
200

> RunVdWk1k2 ${k1} ${k2} ${n} ddfw 200 40000000 AltExp/Exp_VanderWaerden_2-5-7_258_ddfw-200-20000000_2011-04-05-101330/VanDerWaerden_2-5-7_${n}.cnf_sol
> E=read_ubcsat("Exp_VanderWaerden_2-5-7_259_ddfw-200-40000000_2011-04-07-015657/VanDerWaerden_2-5-7_260.cnf_OUT",nrows=200)
 1  2  3  4  5  6  7
12 52 78 16 25  9  8
200

> E=read_ubcsat("Exp_VanderWaerden_2-5-7_259_adaptg2wsat-200-80000000_2011-04-08-063442/VanDerWaerden_2-5-7_260.cnf_OUT",nrows=200)
 1  2  3  4  5  6  7
 8 78 72 14 18  7  3
200
> E=read_ubcsat("Exp_VanderWaerden_2-5-7_260_ddfw-200-80000000_2011-04-09-121350/VanDerWaerden_2-5-7_260.cnf_OUT",nrows=200)
 1  2  3  4  5  6
 5 49 40  2  5  1
102
> E=read_ubcsat("Exp_VanderWaerden_2-5-7_260_ddfw-200-80000000_2011-04-11-011901/VanDerWaerden_2-5-7_260.cnf_OUT",nrows=200)
 1  2  3  4  5  6  7
 5 84 71  9  6  3  1
179
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Using SplittingViaOKsolver
  <ul>
   <li> The following used older versions, and needs update. </li>
   <li> First using the depth-interpretation:
   \verbatim
> SplittingViaOKsolver -D12 -SD VanDerWaerden_2-5-7_260.cnf
> cd SplitViaOKsolver_D12VanDerWaerden_257_260cnf_2011-05-04-192928
> more Md5sum
7f04792a3aaa3a9d4395c9f85342e66b
> more Statistics
> E=read.table("Data")
> summary(E)
       n
 Min.   :12.00
 1st Qu.:12.00
 Median :13.00
 Mean   :13.38
 3rd Qu.:14.00
 Max.   :22.00
> table(E)
E
  12   13   14   15   16   17   18   19   20   21   22
1428 1118  740  452  203   84   54    8    6    2    1
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         7
c initial_number_of_variables           260
c initial_number_of_clauses             13824
c initial_number_of_literal_occurrences 80128
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     71.0
c number_of_nodes                       8191
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        12
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-5-7_260.cnf
c splitting_directory                   SplitViaOKsolver_D12VanDerWaerden_257_260cnf_2011-05-04-192928/Instances
c splitting_cases                       4096

 SplittingViaOKsolver -D16 -SD VanDerWaerden_2-5-7_260.cnf
> cd SplitViaOKsolver_D16VanDerWaerden_257_260cnf_2011-05-04-193348
> more Md5sum
12804168d8212efd37f5cbcb8550888a
> more Statistics
> E=read.table("Data")
> summary(E)
       n
 Min.   :16.00
 1st Qu.:17.00
 Median :18.00
 Mean   :18.68
 3rd Qu.:20.00
 Max.   :34.00
> table(E)
E
   16    17    18    19    20    21    22    23    24    25    26    27    28
 9177 12561 13302 10882  7966  4878  3145  1769   931   486   223   121    53
   29    30    32    33    34
   25    13     2     1     1
OKplatform> more Result
c running_time(sec)                     884.0
c number_of_nodes                       131071
c number_of_2-reductions                134
c max_tree_depth                        16
c splitting_cases                       65536

> SplittingViaOKsolver -D20 -SD VanDerWaerden_2-5-7_260.cnf
> cd SplitViaOKsolver_D20VanDerWaerden_257_260cnf_2011-05-05-050221
> more Md5sum
de6910bd80af2b5961613741068664af
OKplatform> more Statistics
> E=read.table("Data")
> summary(E)
       n
 Min.   :20.00
 1st Qu.:22.00
 Median :24.00
 Mean   :24.65
 3rd Qu.:26.00
 Max.   :70.00
> table(E)
E
    20     21     22     23     24     25     26     27     28     29     30
 40841  88833 133680 153876 151467 131506 104803  79336  56249  38242  25576
    31     32     33     34     35     36     37     38     39     40     41
 16393  10392   6558   4070   2492   1564    947    638    394    266    162
    42     43     44     45     46     47     48     49     50     51     53
   101     63     45     31     10     12      5      3      2      1      5
    54     55     56     59     61     62     66     67     70
     1      1      1      3      2      1      1      1      1
> more Result
c running_time(sec)                     12610.3
c number_of_nodes                       2097151
c number_of_2-reductions                5912
c max_tree_depth                        20
c splitting_cases                       1048575
   \endverbatim
   </li>
   <li> Now using the "n-interpretation":
   \verbatim
> SplittingViaOKsolver -D20 VanDerWaerden_2-5-7_260.cnf
    20     21     22     23     24     25     26     27     28     29     30
175461  43196  22448   6666   2676    729    250     97     35     21      4
    31     32
     5      2
c running_time(sec)                     8320.0
c number_of_nodes                       503179
c number_of_2-reductions                829
c max_tree_depth                        20
c splitting_cases                       251590

> SplittingViaOKsolver -D22 VanDerWaerden_2-5-7_260.cnf
> more Md5sum
26ac75768382fd0367a80d8a399b0a52
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  22.00   22.00   22.00   22.56   23.00   36.00
> table(E$n)
    22     23     24     25     26     27     28     29     30     31     32
478980 131515  70627  22974   9576   2898   1043    396    140     65     29
    33     34     35     36
    10      8      3      2
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         7
c initial_number_of_variables           260
c initial_number_of_clauses             13824
c initial_number_of_literal_occurrences 80128
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     38803.0
c number_of_nodes                       1436531
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                3165
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        22
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-5-7_260.cnf
c splitting_directory                   SplitViaOKsolver_D22VanDerWaerden_257_260cnf_2011-06-04-174027/Instances
c splitting_cases                       718266

> cd Instances
> OKP=~/SAT-Algorithmen/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done
# Aborted
real    6164m6.490s
user    6061m51.875s
sys     16m52.403s

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
248: 100.96h, sum-cfs=3.898054e+09, mean-t=1465.519s, mean-cfs=15717959
> summary(E$t)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
    1.724   167.900   593.200  1466.000  1428.000 25040.000
> summary(E$cfs)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
    38260   2534000   7662000  15720000  16650000 216600000
   \endverbatim
   So the problem is very hard, and further splitting is needed, at least
   D=26. </li>
  </ul>


  \todo vdw_2^pd(5,7) = (227,236)
  <ul>
   <li> Determined relatively easily by minisat-2.2.0. </li>
   <li> Interestingly, unsatisfiability for n=237 was very easy (10s,
   267,783 conflicts), but unsatisfiability for n=228 was rather hard (40m,
   48,363,829 conflicts). </li>
   <li> Certificates:
    <ol>
     <li> n=226:
     \verbatim
2,5,8,9,13,16,17,18,19,25,
26,29,30,31,34,40,43,44,46,47,
48,49,51,53,54,60,65,67,68,69,
72,76,78,79,80,81,88,90,91,92,
96,97,98,99,102,103,107,111
     \endverbatim
     </li>
     <li> n=235:
     \verbatim
3,6,8,13,17,18,19,26,30,31,
32,33,38,39,40,41,43,44,45,49,
54,56,57,63,64,66,71,75,76,77,
79,80,81,82,84,87,88,90,91,94,
96,97,98,99,101,102,103,107,112,114,
115
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdWk1k2 5 7 gsat-tabu 10 100000" yields
   \verbatim
Break point 1: 221
Break point 2: 236
   \endverbatim
   </li>
   <li> "RunPdVdWk1k2 5 7 gsat-tabu 10 200000" yields
   \verbatim
Break point 1: 210
Break point 2: 225
   \endverbatim
   </li>
   <li> "RunPdVdWk1k2 5 7 gsat-tabu 100 200000" yields
   \verbatim
Break point 1: 227
Break point 2: 236
   \endverbatim
   </li>
  </ul>

*/
