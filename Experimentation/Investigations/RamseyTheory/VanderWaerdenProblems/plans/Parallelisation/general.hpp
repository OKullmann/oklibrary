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
   <li> One needs to re-run these experiments when nothing else runs on these
   machine. </li>
  </ul>


  \todo Parallelisation via OKsolver-2002
  <ul>
   <li> See Solvers/OKsolver/SAT2002/plans/SimpleParallelisation.hpp. </li>
   <li> This tool is ready now, via the helper script
   SAT2002/SplittingViaOKsolver (available through a public link). </li>
   <li> See Interfaces/DistributedSolving/plans/general.hpp for plans on
   tools. </li>
   <li> See "vdw_2^pd(5,8) = (312,323)" in
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/08.hpp for
   another example. </li>
   <li> The above example:
   \verbatim
> SplittingViaOKsolver -D8 VanDerWaerden_2-3-12_135.cnf
> cd SplitViaOKsolver_D8VanDerWaerden_2312_135cnf_2011-08-21-101819/
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
c running_time(sec)                     0.2
c number_of_nodes                       105
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                0
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        8
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-12_135.cnf
c splitting_directory                   SplitViaOKsolver_D8VanDerWaerden_2312_135cnf_2011-08-21-101819/Instances
c splitting_cases                       53

> cd ..
> ProcessSplitViaOKsolver SplitViaOKsolver_D8VanDerWaerden_2312_135cnf_2011-08-21-101819/
> cd Process_SplitViaOKsolver_D8VanDerWaerden_2312_135cnf_2011-08-21-101819_2011-08-21-102046
> more Result
UNSATISFIABLE
  PID TTY      STAT   TIME COMMAND
25820 pts/48   S+     0:40 /bin/bash /home/kullmann/OKplatform/bin/ProcessSplitViaOKsolver SplitViaOKsolver_D8VanDerWaerden_2312_135cnf_2011-08-21-101819/
# that is, the whole computation took 0m40s processor time

> oklib --R
> oklib_load_all()
> E=read_processsplit_minisat()
53: 38.38s, sum-cfs=1.990552e+06, mean-t=0.724s, mean-cfs=37558
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
0.05799 0.18600 0.45190 0.72420 0.73690 7.79100
sd= 1.109398
     95%      96%      97%      98%      99%     100%
1.759130 1.759650 1.887312 2.038092 4.805432 7.790820
sum= 38.38314
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   3203   10780   25270   37560   37570  366200
sd= 52672.37
      95%       96%       97%       98%       99%      100%
 90058.80  90488.84  96915.20 104431.80 230403.76 366247.00
sum= 1990552
$t ~ $cfs:
               Estimate  Std. Error  t value  Pr(>|t|)
(Intercept) -6.6133e-02  7.9951e-03  -8.2717 5.476e-11 ***
E$cfs        2.1044e-05  1.2437e-07 169.2024 < 2.2e-16 ***
R-squared: 0.9982
> plot(E$t)
     \endverbatim
   </li>
   <li> One sees that likely a higher splitting-depth would yield a better
   result:
   \verbatim
> SplittingViaOKsolver -D12 VanDerWaerden_2-3-12_135.cnf
> cd SplitViaOKsolver_D12VanDerWaerden_2312_135cnf_2011-08-21-103003/
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
c running_time(sec)                     1.0
c number_of_nodes                       387
c number_of_2-reductions                2
c max_tree_depth                        12
c splitting_cases                       194

> cd ..
> ProcessSplitViaOKsolver SplitViaOKsolver_D12VanDerWaerden_2312_135cnf_2011-08-21-103003/
> cd Process_SplitViaOKsolver_D12VanDerWaerden_2312_135cnf_2011-08-21-103003_2011-08-21-103122/
> more Result
UNSATISFIABLE
  PID TTY      STAT   TIME COMMAND
28375 pts/48   S+     0:30 /bin/bash /home/kullmann/OKplatform/bin/ProcessSplitViaOKsolver SplitViaOKsolver_D12VanDerWaerden_2312_135cnf_2011-08-21-103003/
> oklib --R
> oklib_load_all()
> E=read_processsplit_minisat()
194: 24.3s, sum-cfs=1.389903e+06, mean-t=0.125s, mean-cfs=7164
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
0.02300 0.07399 0.10600 0.12520 0.15550 0.39290
sd= 0.07745311
      95%       96%       97%       98%       99%      100%
0.3080026 0.3152317 0.3344188 0.3510860 0.3685537 0.3929400
sum= 24.29521
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   1112    4242    6070    7164    9059   22640
sd= 4403.824
     95%      96%      97%      98%      99%     100%
17368.30 18059.92 19053.31 19735.08 20662.55 22643.00
sum= 1389903
$t ~ $cfs:
               Estimate  Std. Error  t value Pr(>|t|)
(Intercept) -2.4006e-04  9.8007e-04  -0.2449   0.8068
E$cfs        1.7513e-05  1.1662e-07 150.1702   <2e-16 ***
R-squared: 0.9916
   \endverbatim
   </li>
   <li> It seems that the combination of the OKsolver with minisat-2.2.0
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
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/07.hpp for the
   computations. </li>
  </ul>

*/
