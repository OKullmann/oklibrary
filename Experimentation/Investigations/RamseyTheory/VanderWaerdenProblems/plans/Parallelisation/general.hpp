// Oliver Kullmann, 22.4.2011 (Guangzhou)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/Parallelisation/general.hpp
  \brief Using parallel SAT solving for van-der-Waerden problems

  Collaboration of OK, YP.


  \todo Connections
  <ul>
   <li> See Solvers/OKsolver/SAT2002/plans/SimpleParallelisation.hpp for the
   tool "SplittingViaOKsolver". </li>
   <li> See "Combination of OKsolver with Minisat" for plans on more
   sophisticated tools for combining OKsolver with minisat. </li>
   <li> Interfaces/DistributedSolving/plans/general.hpp </li>
   <li> ExperimentSystem/plans/DistributedSolving.hpp </li>
  </ul>


  \todo Plingeling
  <ul>
   <li> Running in default-configuration (on the test-machine with 2 threads;
   creation of instance by "VanderWaerdenCNF-O3-DNDEBUG 3 12 135")
   \verbatim
> plingeling276-6264d55-100731 -v VanDerWaerden_2-3-12_135.cnf
c 6442836 decisions, 5457271 conflicts
c 123459397 propagations, 0.1 megaprops/sec
c 4302.8 process time, 100% utilization
c 2160.2 seconds, 58.6 MB
s UNSATISFIABLE
   \endverbatim
   or with 4 threads:
   \verbatim
> plingeling276-6264d55-100731 -v -t 4 VanDerWaerden_2-3-12_135.cnf
c 12955195 decisions, 10977898 conflicts
c 248353207 propagations, 0.1 megaprops/sec
c 7409.4 process time, 93% utilization
c 1999.0 seconds, 117.8 MB
s UNSATISFIABLE
   \endverbatim
   </li>
   <li> Without parallelism much faster:
   \verbatim
> lingeling276-6264d55-100731 -v VanDerWaerden_2-3-12_135.cnf
c 3557174 decisions, 3017745 conflicts
c 68243987 propagations, 0.2 megaprops/sec
c 369.1 seconds, 30.1 MB
s UNSATISFIABLE
   \endverbatim
   </li>
   <li> There seems to be a big communication overhead. </li>
   <li> The running times above are on csltok, and so highly unreliable
   (unstable cpu-frequency), and the experiments need to be repeated.
   According to "megaprops" perhaps lingeling run with double the frequency
   --- still much faster! </li>
  </ul>


  \todo Cryptominisat
  <ul>
   <li> Running on a machine (YP, Dawning 5000A/compute-0-80) with 8 quadcore
   cpu's as follows:
   \verbatim
$ more /proc/cpuinfo
model name	: Quad-Core AMD Opteron(tm) Processor 8350
cpu MHz		: 2008.985
cache size	: 512 KB
cpu cores	: 4
bogomips	: 4019.73
   \endverbatim
   we get
   \verbatim
$ time ./cryptominisat --threads=4 VanDerWaerden_2-3-12_135.cnf
c conflicts                : 2703221
real	25m4.821s
user	82m21.972s
sys	0m9.289s
   \endverbatim
   (user-time is sum over all threads) while with 32 threads after 10h of
   total running time still no result was obtained (this might be due to
   scheduling problems). </li>
   <li> One needs to re-run these experiments when nothing else runs on thes
   machine. </li>
  </ul>
  
  
  \todo Simple parallelisation via OKsolver-2002
  <ul>
   <li> See Solvers/OKsolver/SAT2002/plans/SimpleParallelisation.hpp. </li>
   <li> This tool is ready now, via the helper script
   SAT2002/SplittingViaOKsolver (available through a public link). </li>
   <li> See Interfaces/DistributedSolving/plans/general.hpp for plans on
   tools. </li>
   <li> See "vdw_2^pd(5,8) = (312,323)" in
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/08.hpp for
   another example. </li>
   <li> The above example, first using the "depth-interpretation" of the
   D-parameter:
   \verbatim
> SplittingViaOKsolver -D4 -SD VanDerWaerden_2-3-12_135.cnf
> cd SplitViaOKsolver_D4VanDerWaerden_2312_135cnf_2011-05-15-073354
> more Md5sum
3115296eddcf269616a3f2cce5355eaf
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  4.000   4.000   6.000   7.438  11.000  18.000
> table(E$n)
 4  6  7 11 18
 5  5  1  4  1
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         12
c initial_number_of_variables           135
c initial_number_of_clauses             5251
c initial_number_of_literal_occurrences 22611
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     0.1
c number_of_nodes                       31
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                0
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        4
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-12_135.cnf
c splitting_directory                   SplitViaOKsolver_D4VanDerWaerden_2312_135cnf_2011-05-15-073354/Instances
c splitting_cases                       16

> cd Instances/
> time for F in $(awk 'NR!=1 {print $2}' ../Data); do cat ../$(cat ../F) | ApplyPass-O3-DNDEBUG $F Temp.cnf; echo $F $(satz215 Temp.cnf | awk '/terminated/ {print $4}'); done; rm Temp.cnf satz215_timetable
13 0.320
7 0.960
11 0.860
14 0.830
15 0.660
3 3.570
6 2.560
8 3.690
10 3.410
12 2.910
16 2.500
1 39.490
2 11.990
4 12.030
5 12.250
9 10.490

real    1m49.306s
user    1m48.887s
sys     0m0.144s
     \endverbatim
   </li>
   <li> Now using the "n-interpretation":
   \verbatim
> SplittingViaOKsolver -D8 VanDerWaerden_2-3-12_135.cnf
> cd SplitViaOKsolver_D8VanDerWaerden_2312_135cnf_2011-05-30-082907/
> more Md5sum
5edbf75d79e5021292a7a6bd25a2363d
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  8.000   8.000   9.000   9.755  11.000  13.000
> table(E$n)
 8  9 10 11 12 13
21  6  8  6  7  5
> more Result
c running_time(sec)                     0.2
c number_of_nodes                       105
c number_of_2-reductions                0
c max_tree_depth                        8
c splitting_cases                       53

> cd Instances
> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done
real    0m39.139s
user    0m38.440s
sys     0m0.293s

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
53: 0.01h, sum-cfs=1.990552e+06, mean-t=0.708s, mean-cfs=37558

> SplittingViaOKsolver -D12 VanDerWaerden_2-3-12_135.cnf
> cd SplitViaOKsolver_D12VanDerWaerden_2312_135cnf_2011-05-30-083254/
> more Md5sum
96d44d832aa27f0abbc531c35baee49d
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  12.00   12.00   14.00   14.05   15.00   20.00
> table(E$n)
12 13 14 15 16 17 18 19 20
65 23 32 27 23 15  3  4  2
> more Result
c running_time(sec)                     0.8
c number_of_nodes                       387
c number_of_2-reductions                2
c max_tree_depth                        12
c splitting_cases                       194

> cd Instances
> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done
real    0m28.275s
user    0m26.884s
sys     0m0.889s

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
194: 0.01h, sum-cfs=1.389903e+06, mean-t=0.121s, mean-cfs=7164
   \endverbatim
   </li>
   <li> One sees that the n-interpretation creates are much more evenly
   splitting, enabling to split harder problems much further (which seems to
   be essential). </li>
   <li> It also seems that the combination of the OKsolver with minisat-2.2.0
   is a good combination: The OKsolver for splitting the hard problem, the
   conflict-driven solver for finishing off. </li>
  </ul>
  
  
  \todo Parallel Satz
  <ul>
   <li> See "Satz" in Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp.
   </li>
  </ul>
  
  
  \todo Using Open MPI
  <ul>
   <li> It seems that to run even the "simple parallelisation" provided e.g.
   by the OKsolver-2002 (see above) on a "supercomputer" (like "Dawning
   5000A"; see above) one needs to utilise the Open-Mpi-interface. </li>
   <li> See http://www.open-mpi.org. </li>
  </ul>


  \todo Update Data
  <ul>
   <li> The Data-files for the following splittings have been changed with
   SplittingViaOKsolver version 0.1.2. </li>
   <li> This involves some changes of the output formatting. </li>
  </ul>
  
  
  \todo Verify vdw_2(4,9) = 309 (conjectured)
  <ul>
   <li> See
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/09.hpp for the
   computations. </li>
  </ul>
  
  
  \todo Verify vdw_2(5,7) = 260 (conjectured)
  <ul>
   <li> See
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/07.hpp. </li>
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
  
*/
