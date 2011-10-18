// Oliver Kullmann, 6.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/12.hpp
  \brief On investigations into vdw_2(4,12)


  \todo Best ubcsat-solver
  <ul>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
   <li> For now we use adaptg2wsat. </li>
   <li> n=400 is a hard satisfiable problem:
   \verbatim
> E=run_ubcsat("VanDerWaerden_2-4-12_400.cnf",runs=100,cutoff=1000000)
WARNING[run_ubcsat]: There have been 3 errors with the following algorithms:
 saps, rsaps, sapsnr .
plot(E$alg,E$min) # useless because one can't identify the algorithms, and
# the bad algorithms spoil the visibility
> eval_ubcsat_dataframe(E)
rnov :
 5  6  7  8  9 10 11 12
 2  4 18 22 24 21  6  3
FlipsPerSecond = 109158
dano :
 5  6  7  8  9 10
 1  2 24 46 23  4
FlipsPerSecond = 107817
ddfw :
 5  6  7  8  9 10
 1  2 13 42 38  4
FlipsPerSecond = 42414
rnovp :
 5  6  7  8  9 10 11
 1  1 13 24 34 22  5
FlipsPerSecond = 108898
anovpp :
 6  7  8  9 10
 8 18 37 36  1
FlipsPerSecond = 108140
anovp :
 6  7  8  9 10
 3 13 45 34  5
FlipsPerSecond = 108566
ag2wsat :
 6  7  8  9 10 11
 1 10 29 34 21  5
FlipsPerSecond = 143691

E2=run_ubcsat("VanDerWaerden_2-4-12_400.cnf",runs=100,cutoff=10000000,include_algs=list("rnov","dano","ddfw","rnovp","anovpp","anovp","ag2wsat"))
anovpp
 1  4  5  6  7  8
 1  2 10 39 41  7
ag2wsat
 2  5  6  7  8  9
 1  7 23 43 25  1
ddfw
 3  5  6  7  8
 1  9 30 47 13
anovp
 4  5  6  7  8
 2 12 28 56  2
dano
 4  5  6  7  8
 1 11 38 45  5
rnovp
 4  5  6  7  8  9
 1  2 30 48 17  2
rnov
 5  6  7  8  9
 3 22 53 21  1

# csnereid:
> E=run_ubcsat("VanDerWaerden_2-4-12_400.cnf",runs=100,cutoff=10000000)
rnov
 4  5  6  7  8
 1  7 34 36 22
FlipsPerSecond = 137565
anovp
 5  6  7  8
 9 35 47  9
FlipsPerSecond = 137183
gsatt
 6  7  8  9 10 11 12 13 14 16 17 18 25 28
 2  4  7 13 28 19 10  8  2  2  2  1  1  1
FlipsPerSecond = 299316
hwsat
 7  8  9 10 11 12 13
 2  6 18 27 32 10  5
FlipsPerSecond = 260877
gwsat
 7  9 10 11 12 13 14 15
 1  1  5 16 29 29 17  2
FlipsPerSecond = 204051
# Aborted after processing 12 algorithms
>  E=run_ubcsat("VanDerWaerden_2-4-12_400.cnf",runs=100,cutoff=10000000,include_algs=names(tail(run_ubcsat_cnf_algs,-12)))
1. rnovp:
 5  6  7  8  9
 4 21 50 24  1
fps: 137936
2. rots:
 5  7  8  9 10 11
 1  6 23 32 36  2
fps: 204549
3. samd:
 5  7  8  9 10 11 12 13 14 15 17 18 20 21
 1  3 14 25 18 18  9  4  2  2  1  1  1  1
fps: 296353
# INCOMPLETE

>  E=run_ubcsat("VanDerWaerden_2-4-12_400.cnf",runs=100,cutoff=10000000)
> eval_ubcsat_dataframe(E,FALSE)
1. ddfw:
 2  5  6  7  8
 1  8 31 55  5
fps: 45363
2. anovp:
 2  5  6  7  8
 1  6 42 46  5
fps: 129942
3. rnov:
 3  5  6  7  8
 1  8 27 48 16
fps: 130624
4. dano:
 4  5  6  7  8
 2  9 36 49  4
fps: 126793
5. ag2wsat:
 4  5  6  7  8
 1  3 29 42 25
fps: 174350
6. samd:
 4  7  8  9 10 11 12 13 14 15 16 17 20
 1  6  5 21 28  9 11  8  3  2  1  2  3
fps: 292844
7. rnovp:
 5  6  7  8  9
 6 30 40 23  1
fps: 130526
8. anovpp:
 5  6  7  8
 4 42 48  6
fps: 129962
9. hwsat:
 6  7  8  9 10 11 12 13
 1  3  8 26 30 18  9  5
fps: 253737
   \endverbatim
   Unclear; adaptnovelty+ and "adaptnovelty+ -v params" are good candidates.
   </li>
   <li> However below we find indications that adaptg2wsat is better. </li>
  </ul>


  \todo vanderwaerden_2(4,12) > 401
  <ul>
   <li> Certificate for n=401:
   \verbatim
4,10,13,21,26,27,30,34,43,44,
49,57,66,67,71,75,77,78,81,82,
83,86,92,96,98,101,102,103,107,109,
113,115,117,118,124,131,140,141,150,154,
157,163,164,166,168,172,174,175,179,180,
183,185,193,195,200,203,204,206,210,212,
214,215,224,228,237,238,247,254,260,261,
263,265,269,271,272,275,276,280,286,290,
295,300,303,309,318,321,322,323,326,329,
339,344,346,348,351,360,361,367,372,380,
382,391,393,394,395,399,401
   \endverbatim
   </li>
   <li> Start with the palindromic solution for n=393:
   \verbatim
> k1=4 k2=12 n=393 cutoff=1000000 alg="adaptg2wsat" N=100 expdate="2011-08-29-174820"; export k1 k2 n; cat Exp_PdVanderWaerden_2-${k1}-${k2}_${alg}-${N}-${cutoff}_${expdate}/VanDerWaerden_pd_2-${k1}-${k2}_${n}.cnf_sol | PdExtend-O3-DNDEBUG ${n} > solution-${k1}-${k2}-${n}

> RunVdWk1k2 ${k1} ${k2} ${n} adaptg2wsat 100 100000000 solution-${k1}-${k2}-${n}
   \endverbatim
    n=400 found satisfiable.
   though 100 runs with adaptg2wsat and cutoff=100000000 don't find a solution
   for n=400:
   \verbatim
> E=read_ubcsat("VanDerWaerden_2-4-12_400.cnf_OUT",nrows=100)
 1  2  4  5  6  7
 1  3  4 40 50  2
100
   \endverbatim
   </li>
   <li> Now with "adaptnovelty+ -v params":
   \verbatim
> k1=4 k2=12 n0=400 alg="adaptnovelty+ -v params" runs=100 cutoff=100000000; RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} AltExp/Exp_VanderWaerden_2-4-12_393_adaptg2wsat-100-100000000_2011-08-29-180242/VanDerWaerden_2-4-12_400.cnf_sol
   \endverbatim
   See below (no solution found for n=401). </li>
   </li>
   <li> Starting from scratch (csnereid):
   \verbatim
> k1=4 k2=12 n0=12 alg="adaptnovelty+ -v params" runs=100 cutoff=100000000; nohup RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} &
   \endverbatim
   Depends of course on chance: for n=368 6 runs (random-mode) were needed.
   With another run on n=386 was reached:
   \verbatim
 1  2  3  4
 1 13 65 21
100
   \endverbatim
   </li>
   <li> Starting from scratch (csoberon) and with adaptg2wsat:
   \verbatim
> k1=4 k2=12 n0=12 alg="adaptg2wsat" runs=100 cutoff=100000000; nohup RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} &
   \endverbatim
   n=395 was reached:
   \verbatim
> E=read_ubcsat("VanDerWaerden_2-4-12_395.cnf_OUT",nrows=100)
 1  2  3  4  5  6
 2  2  6 29 60  1
100
   \endverbatim
   So adaptg2wsat seems best. How far it gets depends on chance.
   \verbatim
> k1=4 k2=12 n0=12 alg="adaptg2wsat" runs=200 cutoff=200000000; nohup RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} &
   \endverbatim
   </li>
   <li> Starting from scratch (csnereid) and with adaptnovelty+:
   \verbatim
> k1=4 k2=12 n0=12 alg="adaptnovelty+" runs=100 cutoff=100000000; nohup RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} &
   \endverbatim
   Only n=387 was reached:
   \verbatim
 1  2  3  4
 3  8 62 27
100
   \endverbatim
   </li>
   <li> n=401:
    <ol>
     <li> The above run of "adaptnovelty+ -v params" (starting from n=400)
     yielded:
     \verbatim
> E=read_ubcsat("VanDerWaerden_2-4-12_401.cnf_OUT",nrows=100)
 2  3  4  5  6
 2  1  5 35 57
100
     \endverbatim
     (where using the previous solution min=1 is obtained). </li>
     <li> Trying the above second-best algorithm:
     \verbatim
> k1=4 k2=12 n0=400 alg="adaptg2wsat" runs=100 cutoff=100000000; RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} AltExp/Exp_VanderWaerden_2-4-12_393_adaptg2wsat-100-100000000_2011-08-29-180242/VanDerWaerden_2-4-12_400.cnf_sol
> E=read_ubcsat("VanDerWaerden_2-4-12_401.cnf_OUT",nrows=100)
 1  2  3  4  5  6  7
 2  3  1  5 21 62  6
100
     \endverbatim
     Looks better (but could be luck). </li>
     <li> Trying the above third-best algorithm:
     \verbatim
> k1=4 k2=12 n0=400 alg="ddfw" runs=100 cutoff=100000000; RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} AltExp/Exp_VanderWaerden_2-4-12_393_adaptg2wsat-100-100000000_2011-08-29-180242/VanDerWaerden_2-4-12_400.cnf_sol
 4  5  6  7
 7 41 51  1
100
     \endverbatim
     This is clearly worse, and ddfw is also much slower. </li>
     <li> Trying the above fourth-best algorithm:
     \verbatim
> k1=4 k2=12 n0=400 alg="adaptnovelty+" runs=100 cutoff=100000000; RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} AltExp/Exp_VanderWaerden_2-4-12_393_adaptg2wsat-100-100000000_2011-08-29-180242/VanDerWaerden_2-4-12_400.cnf_sol
 2  3  4  5  6  7
 4  1  7 37 40  1
90
     \endverbatim
     Better than ddfw; appears better than "adaptnovelty+ -v params" but that
     could be chance; likely worse than adaptg2wsat. </li>
    </ol>
   </li>
   <li> Starting from scratch with
   "RunVdWk1k2 4 12 12 adaptg2wsat 200 200000000" finds a solution for n=401,
   where for the following n-values a solution was not found using the previous
   solution:
   \verbatim
Exp_VanderWaerden_2-4-12_12_adaptg2wsat-200-200000000_2011-09-07-054150> cat Statistics
    368     2   93604057 1690129001  r
    372    10  173104383  862966825  r
    375    14  156911897 1374830150  r
    390     2  107902833 1147100240  r
    393     2   44450618 1573666775  r
    394   125  155875353  978652971  r
    397   136   64030169 2773919598  r
   \endverbatim
   and where
   \verbatim
> E=read_ubcsat("VanDerWaerden_2-4-12_402.cnf_OUT",nrows=200)
  1   2   3   4   5   6   7
  3  13   1   6  71 104   2
200
   \endverbatim
   </li>
  </ul>


  \todo vdw_2^pd(4,12) = (387,394)
  <ul>
   <li> pdvanderwaerden([4,12])[2] = 394 =(conj)
   vanderwaerden([4,12])-???. </li>
   <li> pdvanderwaerden([4,12])[1] = 387. </li>
   <li> minisat-2.2.0:
    <ol>
     <li> n=388: aborted:
     \verbatim
restarts              : 2490360
conflicts             : 2538607498     (2596 /sec)
decisions             : 2932947182     (0.00 % random) (3000 /sec)
propagations          : 62093142862    (63504 /sec)
conflict literals     : 75126436549    (30.65 % deleted)
Memory used           : 276.00 MB
     \endverbatim
     (cpu-time unreliable). </li>
     <li> This is a hard, but rather small clause-set:
     \verbatim
> cat VanDerWaerden_pd_2-4-12_388.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
194 15544 87698 0 87698 1 1
 length count
2 65
4 12259
6 18
12 3202
     \endverbatim
     </li>
     <li> Now using SplittingViaOKsolver. </li>
     <li> First considering n=388. </li>
     <li> First finding the n-parameter:
     \verbatim
> SplittingViaOKsolver -D10 VanDerWaerden_pd_2-4-12_388.cnf
10 11 12 13
47 27 13  4
> SplittingViaOKsolver -D15 VanDerWaerden_pd_2-4-12_388.cnf
 15  16  17  18  19  20  21
310 159 114  59  18   2   3
> SplittingViaOKsolver -D20 VanDerWaerden_pd_2-4-12_388.cnf
  20   21   22   23   24   25   26   27   28   29   30
2029  879  673  453  234   90   35   18    4    2    2
> SplittingViaOKsolver -D25 VanDerWaerden_pd_2-4-12_388.cnf
   25    26    27    28    29    30    31    32    33    34    35    36    37
10850  4618  4097  3203  2125  1154   503   255   113    43    34    13     9
   38    39    40
    3     4     2
 E=read.table("Data")
> E[E$n==25,][1:10,]
       i  n
16177  1 25
16178  3 25
16179  4 25
16180  6 25
16181  8 25
16182 13 25
16183 15 25
16184 16 25
16185 19 25
16186 21 25
> cat VanDerWaerden_pd_2-4-12_388.cnf | ApplyPass-O3-DNDEBUG Instances/1 > I1.cnf
> minisat-2.2.0 I1.cnf
conflicts             : 857041         (27774 /sec)
CPU time              : 30.8573 s
UNSATISFIABLE
> minisat-2.2.0 I21.cnf
conflicts             : 286648         (29254 /sec)
CPU time              : 9.79851 s
UNSATISFIABLE

> SplittingViaOKsolver -D30 VanDerWaerden_pd_2-4-12_388.cnf
> cd SplitViaOKsolver_D30SNVanDerWaerden_pd_2412_388cnf_2011-05-20-091001
> more Md5sum
316fb102dce8874b4512acaecfde9db3
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  30.00   30.00   31.00   31.99   33.00   70.00
> table(E$n)
   30    31    32    33    34    35    36    37    38    39    40    41    42
44995 21408 19887 17229 12297  7661  4122  2074  1042   554   332   178   120
   43    44    45    46    47    48    49    50    51    52    53    54    55
   88    39    32    17     9     7    10    11     4     2     3     2     2
   58    62    64    65    68    70
    1     1     1     1     1     1
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         12
c initial_number_of_variables           194
c initial_number_of_clauses             15544
c initial_number_of_literal_occurrences 87698
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   65
c running_time(sec)                     2063.5
c number_of_nodes                       264275
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                9332
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        27
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_pd_2-4-12_388.cnf
c splitting_directory                   SplitViaOKsolver_D30SNVanDerWaerden_pd_2412_388cnf_2011-05-20-091001/Instances
c splitting_cases                       132131

> cd Instances
> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done
real    2897m28.451s
user    2894m17.633s
sys     50m59.587s

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
132131: 41.18h, sum-cfs=3.998369e+09, mean-t=1.122s, mean-cfs=30261
     \endverbatim
     </li>
     <li> Now considering n=395. Trying D=31,34:
     \verbatim
> SplittingViaOKsolver -D31 VanDerWaerden_pd_2-4-12_395.cnf
   31    32    33    34    35    36    37    38    39    40    41    42    43
20597 11607 10643  8971  6600  4031  2209  1122   559   270   147    84    58
   44    45    46    47    48    49    50    52    53    54    55    57    61
   38    23    13     9     6     9     4     2     5     3     1     3     1
   62    64    73
    1     1     1
c running_time(sec)                     3029.3
c number_of_nodes                       134037
c number_of_2-reductions                5191
c max_tree_depth                        25

> SplittingViaOKsolver -D34 VanDerWaerden_pd_2-4-12_395.cnf
> cd SplitViaOKsolver_D34SNVanDerWaerden_pd_2412_395cnf_2011-05-27-131557
> more Md5sum
0de78936d281545108d9ef3d6e3f2c8c
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  34.00   34.00   36.00   36.31   38.00   86.00
> table(E$n)
   34    35    36    37    38    39    40    41    42    43    44    45    46
42061 24202 22756 19772 14990 10051  5909  3277  1649   945   501   340   238
   47    48    49    50    51    52    53    54    55    56    57    58    59
  142    99    78    52    39    26    23    18    12    10    12     8     3
   60    61    62    63    64    65    68    71    73    75    79    86
    4     3     5     2     1     2     1     1     2     1     1     1
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         12
c initial_number_of_variables           198
c initial_number_of_clauses             15889
c initial_number_of_literal_occurrences 88834
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   65
c running_time(sec)                     5747.0
c number_of_nodes                       294541
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                18043
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        27
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_pd_2-4-12_395.cnf
c splitting_directory                   SplitViaOKsolver_D34SNVanDerWaerden_pd_2412_395cnf_2011-05-27-131557/Instances
c splitting_cases                       147237

> cd Instances
> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done
real    1031m44.125s
user    977m35.662s
sys     53m4.491s

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
147237: 8.22h, sum-cfs=7.677296e+08, mean-t=0.201s, mean-cfs=5214
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Do we have an easy-hard pattern based on parity? </li>
   <li> Certificates:
    <ol>
     <li> n=386:
     \verbatim
12,17,19,22,28,32,41,43,48,50,
51,53,56,61,70,72,77,79,86,91,
92,101,102,104,105,107,108,109,121,129,
132,138,142,143,147,148,150,152,158,161,
169,174,176,181,182,183,185,186,188,189
     \endverbatim
     </li>
     <li> n=393:
     \verbatim
5,7,12,16,17,26,28,34,35,42,
52,57,65,71,74,79,83,85,89,90,
91,94,100,104,106,111,114,115,117,121,
123,125,126,132,139,148,149,158,162,171,
172,174,176,180,182,183,186,187,188,191,
193,197
     \endverbatim
    </ol>
   </li>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
   <li> For now we consider adaptg2wsat as best (it was determined as best
   for vdw_2^pd(5,8) in VanderWaerdenProblems/plans/5-k/general.hpp. </li>
   <li>  "RunPdVdWk1k2 4 12 adaptg2wsat 100 1000000" yields
   \verbatim
Break point 1: 387
Break point 2: 394
   \endverbatim
   finding all solutions at the latest in the first run with random seed.
   </li>
  </ul>

*/
