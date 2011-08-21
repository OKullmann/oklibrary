// Oliver Kullmann, 20.8.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/08.hpp
  \brief On investigations into vdw_2(4,8) = 146


  \todo OKsolver_2002
  <ul>
   <li> vdw_2(4,8) = 146
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D16 -F VanDerWaerden_2-4-8_146.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         8
c initial_number_of_variables           146
c initial_number_of_clauses             4930
c initial_number_of_literal_occurrences 25520
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     417795.2
c number_of_nodes                       260999494
c number_of_single_nodes                22477
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1289661772
c number_of_pure_literals               209803
c number_of_autarkies                   0
c number_of_missed_single_nodes         57858
c max_tree_depth                        47
c number_of_table_enlargements          0
c number_of_1-autarkies                 3107263
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-8_146.cnf
 E = read_oksolver_mon("VanDerWaerden_2-4-8_146.cnf.mo")
65491
> plot_oksolver_mon_nodes(E)
ldstep= 13 step= 8192 left= 128 right= 65536
obs/count= 1.000688 nodes-range= 1 375720 ave-nodes-range= 2282.791 8694.232
> summary_oksolver(E)
Nodes:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
      1     281    1025    3985    3351  375700
2-reductions:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.290   5.100   5.300   5.309   5.510  27.000
Single nodes:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
 0.0000  0.0000  0.0000  0.3432  0.0000 54.0000
Autarkies:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
      0       0       0       0       0       0
Time ~ nodes:
[1] 0.9944431
 (Intercept)      E$nodes
-0.355068103  0.001689846
Single nodes ~ nodes:
[1] 0.6518576
  (Intercept)       E$nodes
-0.2196576270  0.0001412363
Autarkies ~ nodes:
[1] NaN
(Intercept)     E$nodes
          0           0
> hist_oksolver_mon_nodes(E)
Median= 10.00141
Mean= 11.96046
   \endverbatim
   2reds/nds ~ 4.94, n / 4.94 ~ 29.6; in the second half the monitoring nodes
   got much easier, and the distribution of node-counts had one central peak
   around the median and a very thin "heavy tail". </li>
  </ul>


  \todo march_pl
  <ul>
   <li>
   \verbatim
> time march_pl VanDerWaerden_2-4-8_146.cnf
c main():: nodeCount: 29328090
c main():: dead ends in main: 49807
c main():: lookAheadCount: 990103603
c main():: doublelook: #: 141777685, succes #: 118216193
real    1624m7.508s
user    1507m4.853s
sys     1m13.086s
   \endverbatim
   Roughly four times faster than OKsolver_2002, and much fewer nodes. So
   apparently the added resolvents and/or the partial r_3-reduction pays
   off here, and makes march_pl to the best solver on these instances.
   </li>
  </ul>


  \todo Picosat913
  <ul>
   <li> k=8
   \verbatim
400142616 conflicts
29146.6 s (8.1h)
   \endverbatim
   Perhaps the fastest solver here. </li>
  </ul>


  \todo SplittingViaOKsolver
  <ul>
   <li> First with minisat-2.2.0:
   \verbatim
> SplittingViaOKsolver -D20 VanDerWaerden_2-4-8_146.cnf
> cd SplitViaOKsolver_D20VanDerWaerden_248_146cnf_2011-08-20-205807/
> more Md5sum
20e99e1199d616ec681739e350d240c0
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  20.00   20.00   21.00   21.32   22.00   48.00
> table(E$n)
   20    21    22    23    24    25    26    27    28    29    30    31    32
28679 13186 10275  6304  3538  1668   875   387   185    74    51    20    11
   33    34    35    36    37    43    48
    6     1     3     2     1     3     1
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         8
c initial_number_of_variables           146
c initial_number_of_clauses             4930
c initial_number_of_literal_occurrences 25520
c running_time(sec)                     144.9
c number_of_nodes                       130549
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                2837
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        20
c file_name                             VanDerWaerden_2-4-8_146.cnf
c splitting_directory                   SplitViaOKsolver_D20VanDerWaerden_248_146cnf_2011-08-20-205807/Instances
c splitting_cases                       65270

> ProcessSplitViaOKsolver SplitViaOKsolver_D20VanDerWaerden_248_146cnf_2011-08-20-205807
265m01s
> E=read_processsplit_minisat()
65270: 3.747h, sum-cfs=8.831606e+08, mean-t=0.207s, mean-cfs=13531
$t:
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
 0.001999  0.047990  0.097980  0.206700  0.209000 21.760000
sd= 0.4514638
      95%       96%       97%       98%       99%      100%
 0.687895  0.785880  0.943856  1.212434  1.793040 21.758700
sum= 13488.18
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
     49    3264    6714   13530   14140 1151000
sd= 26889.61
       95%        96%        97%        98%        99%       100%
  45313.30   51243.24   61021.88   77150.64  112392.61 1151425.00
sum= 883160594
lm(formula = E$t ~ E$cfs)
              Estimate Std. Error t value Pr(>|t|)
(Intercept) -1.994e-02  1.419e-04  -140.5   <2e-16 ***
E$cfs        1.675e-05  4.714e-09  3552.7   <2e-16 ***
Multiple R-squared: 0.9949,     Adjusted R-squared: 0.9949
   \endverbatim
   </li>
  </ul>


  \todo Local search
  <ul>
   <li> k=8, n=145:
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 1000000 -i VanDerWaerden_2-4-8_145.cnf | tee VanDerWaerden_2-4-8_145.cnf_AUS
Clauses = 4862
Variables = 145
TotalLiterals = 25168
FlipsPerSecond = 107710
BestStep_Mean = 307448.260000
Steps_Mean = 981170.280000
Steps_Max = 1000000.000000
PercentSuccess = 4.00
BestSolution_Mean = 1.150000
BestSolution_Median = 1.000000
BestSolution_Min = 0.000000
BestSolution_Max = 2.000000
> E = read_ubcsat("VanDerWaerden_2-4-8_145.cnf_AUS")
 0  1  2
 4 77 19
100
   \endverbatim
   </li>
   <li> It seems that gsat-tabu is best from the ubcsat-1-0-0 suite?:
   \verbatim
> ubcsat-okl -alg gsat-tabu -runs 100 -cutoff 1000000 -i VanDerWaerden_2-4-8_145.cnf | tee VanDerWaerden_2-4-8_145.cnf_AUS2
Clauses = 4862
Variables = 145
TotalLiterals = 25168
FlipsPerSecond = 150984
BestStep_Mean = 307929.290000
Steps_Mean = 994437.860000
Steps_Max = 1000000.000000
PercentSuccess = 1.00
BestSolution_Mean = 1.320000
BestSolution_Median = 1.000000
BestSolution_Min = 0.000000
BestSolution_Max = 2.000000
 0  1  2
 1 66 33
100
   \endverbatim
   </li>
   So actually it seems that adaptnovelty+ is better, and thus here we have a
   case where better performance for smaller cutoff-values is misleading. </li>
  </ul>


*/
