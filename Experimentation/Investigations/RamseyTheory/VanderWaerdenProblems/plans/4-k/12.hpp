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

E2=run_ubcsat("VanDerWaerden_2-4-12_400.cnf",runs=100,cutoff=10000000,include_algs=list("rnov","dano","ddfw","rnopv","anovpp","anopv","ag2wsat"))
   \endverbatim
   </li>
  </ul>


  \todo vanderwaerden_2(4,12) > 400
  <ul>
   <li> Start with the palindromic solution for n=393:
   \verbatim
> k1=4 k2=12 n=393 cutoff=1000000 alg="adaptg2wsat" N=100 expdate="2011-08-29-174820"; export k1 k2 n; cat Exp_PdVanderWaerden_2-${k1}-${k2}_${alg}-${N}-${cutoff}_${expdate}/VanDerWaerden_pd_2-${k1}-${k2}_${n}.cnf_sol | PdExtend-O3-DNDEBUG ${n} > solution-${k1}-${k2}-${n}

> RunVdWk1k2 ${k1} ${k2} ${n} adaptg2wsat 100 100000000 solution-${k1}-${k2}-${n}
   \endverbatim
   </li>
   <li> n=400 found satisfiable (above run found a solution for n=394 in run
   14) with certificate
   \verbatim
6,7,8,10,19,21,29,34,40,41,
50,53,55,57,62,72,75,78,79,80,
83,92,98,101,106,111,115,121,125,126,
129,130,132,136,138,140,141,147,154,163,
164,173,177,186,187,189,191,195,197,198,
201,206,208,216,218,221,222,226,227,229,
233,235,237,238,244,247,251,260,261,270,
277,283,284,286,288,292,294,298,299,300,
303,305,309,318,319,323,324,326,330,334,
335,344,352,357,358,367,371,374,375,380,
388,391,392,397
   \endverbatim
   though 100 runs with adaptg2wsat and cutoff=100000000 don't find a solution
   for n=400:
   \verbatim
> E=read_ubcsat("VanDerWaerden_2-4-12_400.cnf_OUT",nrows=100)
 1  2  4  5  6  7
 1  3  4 40 50  2
100
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
