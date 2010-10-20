// Oliver Kullmann, 10.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_2-4-k.hpp
  \brief On investigations into vdW-numbers vdw_2(4,k)

  Instances created by output_vanderwaerden2nd_stdname(4,k,n).
  
  
  \todo Literature overview
  <ul>
   <li> Apparently nothing is known about vdw_2(4,k) ? Could there also be
   a conjecture that it's polynomially bounded? </li>
   <li> The known values:
   \verbatim
create_list(vanderwaerden4k(k),k,1,9);
 [4,7,18,35,55,73,109,146,unknown]
   \endverbatim
   </li>
   <li> Via R we get the prediction f(k) ~ 1.846715 * k^2.09233 when
   excluding the first 2 points:
   \verbatim
d = c(4,7,18,35,55,73,109,146)
plot(d)
x = log((1:length(d))[-(1:2)])
y = log(d[-(1:2)])
plot(x,y)
L = lm(y ~ x)
summary(L)
Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept)  0.61341    0.09491   6.463  0.00295 **
x            2.09233    0.05632  37.153 3.13e-06 ***
lines(x,predict(L))
C = coefficients(L)
f = function(k){exp(C[1]) * k^C[2]}
plot(d)
lines(f(1:length(d)))
f(1:10)
 [1]   1.846715   7.875051  18.394758  33.582025  53.564174  78.441799
 [7] 108.298437 143.205718 183.226470 228.416720
   \endverbatim
   </li>
   <li> Including the conjecture for k=9:
   \verbatim
d = c(4,7,18,35,55,73,109,146,309)
plot(d)
x = log((1:length(d))[-(1:2)])
y = log(d[-(1:2)])
plot(x,y)
L = lm(y ~ x)
summary(L)
Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept)   0.2298     0.3471   0.662    0.537
x             2.3572     0.1964  12.002 7.08e-05 ***
Residual standard error: 0.1886 on 5 degrees of freedom
Multiple R-squared: 0.9665,     Adjusted R-squared: 0.9597
F-statistic:   144 on 1 and 5 DF,  p-value: 7.085e-05
lines(x,predict(L))
C = coefficients(L)
f = function(k){exp(C[1]) * k^C[2]}
plot(d)
lines(f(1:length(d)))
round(f(1:10))
  1   6  17  33  56  86 124 169 223 286
round(f(1:10)) - d
  -3  -1  -1  -2   1  13  15  23 -86
   \endverbatim
   This model is not very good (the outlier for k=9 is hard to handle).
   </li>
  </ul>


  \todo Performance of OKsolver-2002
  <ul>
   <li> The minisat2-preprocessing has no effect here. </li>
   <li> vdw_2(4,4) = 35
   \verbatim
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-4_34.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         4
c initial_number_of_variables           34
c initial_number_of_clauses             352
c initial_number_of_literal_occurrences 1408
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     0.0
c number_of_nodes                       7
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                4
c number_of_pure_literals               3
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        6
c number_of_table_enlargements          0
c number_of_1-autarkies                 8
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-4_34.cnf
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-4_35.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         4
c initial_number_of_variables           35
c initial_number_of_clauses             374
c initial_number_of_literal_occurrences 1496
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     0.0
c number_of_nodes                       71
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                142
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        6
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-4_35.cnf
   \endverbatim
   </li>
   <li> vdw_2(4,5) = 55
   \verbatim
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-5_54.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         5
c initial_number_of_variables           54
c initial_number_of_clauses             797
c initial_number_of_literal_occurrences 3526
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     0.0
c number_of_nodes                       313
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                853
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        12
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-5_54.cnf
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-5_55.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         5
c initial_number_of_variables           55
c initial_number_of_clauses             828
c initial_number_of_literal_occurrences 3663
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     0.1
c number_of_nodes                       683
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1924
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        12
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-5_55.cnf
   \endverbatim
   </li>
   <li> vdw_2(4,6) = 73
   \verbatim
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-6_72.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         6
c initial_number_of_variables           72
c initial_number_of_clauses             1311
c initial_number_of_literal_occurrences 6210
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     0.4
c number_of_nodes                       2842
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                10236
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        21
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-6_72.cnf
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-6_73.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         6
c initial_number_of_variables           73
c initial_number_of_clauses             1349
c initial_number_of_literal_occurrences 6390
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     3.5
c number_of_nodes                       21661
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                78198
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        21
c number_of_table_enlargements          0
c number_of_1-autarkies                 3
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-6_73.cnf
   \endverbatim
   </li>
   <li> vdw_2(4,7) = 109
   \verbatim
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-7_108.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         7
c initial_number_of_variables           108
c initial_number_of_clauses             2808
c initial_number_of_literal_occurrences 13986
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     95.4
c number_of_nodes                       274839
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1210747
c number_of_pure_literals               1
c number_of_autarkies                   0
c number_of_missed_single_nodes         5
c max_tree_depth                        30
c number_of_table_enlargements          0
c number_of_1-autarkies                 51
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-7_108.cnf
> OKsolver_2002-O3-DNDEBUG -M -D10 VanDerWaerden_2-4-7_109.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         7
c initial_number_of_variables           109
c initial_number_of_clauses             2862
c initial_number_of_literal_occurrences 14256
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     462.4
c number_of_nodes                       1173953
c number_of_single_nodes                2
c number_of_quasi_single_nodes          0
c number_of_2-reductions                5248010
c number_of_pure_literals               36
c number_of_autarkies                   0
c number_of_missed_single_nodes         18
c max_tree_depth                        31
c number_of_table_enlargements          0
c number_of_1-autarkies                 1031
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-7_109.cnf
   \endverbatim
   </li>
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
   </li>
   <li> k=9, n=303:
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D30 -F VanDerWaerden_2-4-9_303.cnf
 VanDerWaerden_2-4-9_303.cnf,   30, 1073741824
   26:   1371    830.15  8.91E+11    40.22s    23.54s   801y 156d  0h 43m 57s     0     0   49
   \endverbatim
   so even with 1000 processors one would get in just down to about a month.
   This is "infeasible", and better methods are needed. </li>
  </ul>


  \todo Performance of satz215
  <ul>
   <li>
   \verbatim
> satz215 VanDerWaerden_2-4-4_35.cnf
NB_MONO= 0, NB_UNIT= 707, NB_BRANCHE= 111, NB_BACK= 56
> satz215 VanDerWaerden_2-4-5_55.cnf
NB_MONO= 0, NB_UNIT= 19373, NB_BRANCHE= 2528, NB_BACK= 1279
> satz215 VanDerWaerden_2-4-6_73.cnf
NB_MONO= 39, NB_UNIT= 788138, NB_BRANCHE= 78809, NB_BACK= 39800
   \endverbatim
   </li>
   <li> This looks much worse than OKsolver_2002. </li>
  </ul>


  \todo Performance of march_pl
  <ul>
   <li> For VanDerWaerden_2-4-6_73.cnf 5520 nodes and 8.3 s are needed. </li>
   <li> For VanDerWaerden_2-4-7_108.cnf 73308 and 240 s, and for
   VanDerWaerden_2-4-7_109.cnf 250583 nodes and 817 s. </li>
   <li> Comparison with OKsolver_2002 unclear; march_pl takes longer, but needs
   fewer nodes, perhaps due to the partial r_3-reduction and the added
   resolvents. </li>
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
   <li> k=9 likely needs to be run at least a month, while there is no
   (usable) progress meter; and given that march_pl is not really drastically
   faster than OKsolver_2002, it seems futile to run march_pl on instances
   where unsatisfiability is conjectured. </li>
  </ul>


  \todo Performance of minisat2
  <ul>
   <li>
   \verbatim
minisat2 VanDerWaerden_2-4-4_35.cnf
restarts              : 3
conflicts             : 344            (34407 /sec)

> minisat2 VanDerWaerden_2-4-5_55.cnf
restarts              : 8
conflicts             : 3755           (31297 /sec)

> minisat2 VanDerWaerden_2-4-6_73.cnf
restarts              : 17
conflicts             : 194391         (20058 /sec)
   \endverbatim
   </li>
   <li> This looks much worse than OKsolver_2002. </li>
  </ul>


  \todo Performance of picosat236
  <ul>
   <li>
   \verbatim
picosat VanDerWaerden_2-4-4_35.cnf
c 6 iterations
c 2 restarts
c 0 failed literals
c 318 conflicts
c 344 decisions

> picosat VanDerWaerden_2-4-5_55.cnf
c 7 iterations
c 3 restarts
c 0 failed literals
c 3191 conflicts
c 3440 decisions

> picosat VanDerWaerden_2-4-6_73.cnf
c 13 iterations
c 3 restarts
c 0 failed literals
c 107987 conflicts
c 116761 decisions

> picosat VanDerWaerden_2-4-7_108.cnf
c 0 iterations
c 3 restarts
c 0 failed literals
c 2442419 conflicts
c 2679965 decisions
> picosat VanDerWaerden_2-4-7_109.cnf
c 20 iterations
c 3 restarts
c 0 failed literals
c 4433747 conflicts
c 4860329 decisions
   \endverbatim
   </li>
   <li> Seems comparable to OKsolver_2002 (likely better than minisat2). </li>
  </ul>


  \todo Local search for the satisfiable instances
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


  \todo vanderwaerden_2(4,9) >= 309
  <ul>
   <li> [Ahmed 2009] states vanderwaerden4k(9) > 254. </li>
   <li> n=254 found satisfiable by adaptnovelty+ (first run with cutoff=10^6;
   seed=719877201, osteps=677160):
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 1000000 -i VanDerWaerden_2-4-9_254.cnf | tee VanDerWaerden_2-4-9_254.cnf_AUS
Clauses = 14532
Variables = 254
TotalLiterals = 77658
FlipsPerSecond = 51723
BestStep_Mean = 480596.590000
Steps_Mean = 983725.910000
Steps_Max = 1000000.000000
PercentSuccess = 5.00
BestSolution_Mean = 19.200000
BestSolution_Median = 21.000000
BestSolution_Min = 0.000000
BestSolution_Max = 26.000000
> E = read_ubcsat("VanDerWaerden_2-4-9_254.cnf_AUS")

 0 13 14 15 16 17 18 19 20 21 22 23 24 25 26
 5  4 11  6  1  2  1  7  3 11 21 10 11  6  1
100
   \endverbatim
   This is an interesting distribution! One should repeat this with, say,
   10000 runs. </li>
   <li> n=255 found satisfiable with cutoff=10^6 (seed=607594063,
   osteps=624845). </l>
   <li> n=256 found satisfiable with cutoff=10^6 (seed=3979253483,
   osteps=609761):
   \verbatim
Clauses = 14763
Variables = 256
TotalLiterals = 78892
FlipsPerSecond = 50406
BestStep_Mean = 463123.530000
Steps_Mean = 982825.650000
Steps_Max = 1000000.000000
PercentSuccess = 4.00
BestSolution_Mean = 20.750000
BestSolution_Median = 23.000000
BestSolution_Min = 0.000000
BestSolution_Max = 27.000000
 0 13 14 15 16 17 19 20 21 22 23 24 25 26 27
 4  2  1  9  5  5  1  3  5 13 16 19  4 10  3
100
   \endverbatim
   </li>
   <l> n=257 found satisfiable with cutoff=10^6 (seed=661764274,
   osteps=909616). </li>
   <li> n=258 found satisfiable with cutoff=10^6 (seed=1516374201,
   osteps=891449):
   \verbatim
Clauses = 14997
Variables = 258
TotalLiterals = 80148
FlipsPerSecond = 50452
BestStep_Mean = 504067.970000
Steps_Mean = 985720.530000
Steps_Max = 1000000.000000
PercentSuccess = 3.00
BestSolution_Mean = 22.500000
BestSolution_Median = 24.000000
BestSolution_Min = 0.000000
BestSolution_Max = 29.000000
 0 15 16 17 19 20 21 22 23 24 25 26 27 28 29
 3  4  4  8  1  3  3  7  6 16 14 20  7  3  1
100
   \endverbatim
   </li>
   <li> n=259 found satisfiable with cutoff=10^6 (seed=81224953,
   osteps=221569):
   \verbatim
Clauses = 15115
Variables = 259
TotalLiterals = 80780
FlipsPerSecond = 50555
BestStep_Mean = 415767.370000
Steps_Mean = 992215.690000
Steps_Max = 1000000.000000
PercentSuccess = 1.00
BestSolution_Mean = 23.450000
BestSolution_Median = 25.000000
BestSolution_Min = 0.000000
BestSolution_Max = 29.000000
 0 13 15 16 17 18 19 21 22 23 24 25 26 27 28 29
 1  1  3  3  7  5  2  1  4  6 12  9 20 18  6  2
100
   \endverbatim
   </li>
   <li> n=260 found satisfiable with cutoff=10^6 (seed=3252333691,
   osteps=745935). </li>
   <li> n=261 found satisfiable by saps with cutoff=10^5 (seed=3081757854,
   osteps=14683). </li>
   <li> n=262
    <ol>
     <li> With gsat-tabu (cutoff=10^6):
     \verbatim
 ubcsat-okl -alg gsat-tabu -runs 100 -cutoff 1000000 -i VanDerWaerden_2-4-9_262.cnf | tee VanDerWaerden_2-4-9_262.cnf_AUS
Clauses = 15470
Variables = 262
TotalLiterals = 82680
FlipsPerSecond = 90269
BestStep_Mean = 356692.350000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 25.490000
BestSolution_Median = 26.000000
BestSolution_Min = 19.000000
BestSolution_Max = 31.000000
19 23 24 25 26 27 28 29 31
 1  9 12 23 32 20  1  1  1
100
     \endverbatim
     </li>
     <li> With rots (cutoff=10^6; solution with seed=427124191 and
     osteps=272999):
     \verbatim
> ubcsat-okl -alg rots -runs 100 -cutoff 1000000 -i VanDerWaerden_2-4-9_262.cnf | tee VanDerWaerden_2-4-9_262.cnf_AUS
Clauses = 15470
Variables = 262
TotalLiterals = 82680
FlipsPerSecond = 72604
BestStep_Mean = 405882.170000
Steps_Mean = 992729.990000
Steps_Max = 1000000.000000
PercentSuccess = 1.00
BestSolution_Mean = 24.760000
BestSolution_Median = 25.000000
BestSolution_Min = 0.000000
BestSolution_Max = 27.000000
 0 16 22 23 24 25 26 27
 1  1  2  4 23 33 25 11
100
     \endverbatim
     </li>
     <li> With adaptnovelty+ (cutoff=10^6):
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 1000000 -i VanDerWaerden_2-4-9_262.cnf | tee VanDerWaerden_2-4-9_262.cnf_AUS
 0 16 17 18 19 20 21 23 24 25 26 27 28 29 30 31
 3  1  2  2  6  2  3  2  1  5 19 17 21 12  3  1
100
     \endverbatim
     Hm --- perhaps adaptnovelty+ is actually better when it comes to larger
     cutoffs (or at least w.r.t. finding solutions)?! </li>
     </li>
    </ol>
   </li>
   <li> n=263 found satisfiable by adaptnovelty+ with cutoff=10^6
   (seed=3699341296, osteps=72347):
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 1000000 -i VanDerWaerden_2-4-9_263.cnf | tee VanDerWaerden_2-4-9_263.cnf_AUS
Clauses = 15589
Variables = 263
TotalLiterals = 83316
FlipsPerSecond = 49375
BestStep_Mean = 464066.000000
Steps_Mean = 976996.130000
Steps_Max = 1000000.000000
PercentSuccess = 3.00
BestSolution_Mean = 26.490000
BestSolution_Median = 28.000000
BestSolution_Min = 0.000000
BestSolution_Max = 32.000000
 0 17 19 20 21 22 23 24 25 26 27 28 29 30 31 32
 3  1  3  2  3  1  2  3  4  3 13 24 17 14  6  1
   \endverbatim
   </li>
   <li> n=264 found satisfiable by adaptnovelty+ with cutoff=10^6
   (seed=1031057194, osteps=813990):
Clauses = 15708
Variables = 264
TotalLiterals = 83952
FlipsPerSecond = 48629
BestStep_Mean = 468605.860000
Steps_Mean = 998139.900000
Steps_Max = 1000000.000000
PercentSuccess = 1.00
BestSolution_Mean = 27.650000
BestSolution_Median = 28.000000
BestSolution_Min = 0.000000
BestSolution_Max = 33.000000
 0 18 19 20 21 23 24 25 26 27 28 29 30 31 32 33
 1  1  1  2  2  2  6  3  6 11 18 16 11 11  8  1
   \endverbatim
   </li>
   <li> n=265 found satisfiable by adaptnovelty+ with cutoff=10^6
   (seed=1091609315, osteps=600425). </li>
   <li> n=266 found satisfiable by adaptnovelty+ with cutoff=10^6
   (seed=4173596225, osteps=318672). </li>
   <li> n=267 found satisfiable by adaptnovelty+ with cutoff=10^6
   (seed=1038073943, osteps=990919):
   \verbatim
Clauses = 16071
Variables = 267
TotalLiterals = 85899
FlipsPerSecond = 48162
BestStep_Mean = 413333.000000
Steps_Mean = 999909.190000
Steps_Max = 1000000.000000
PercentSuccess = 1.00
BestSolution_Mean = 29.010000
BestSolution_Median = 30.000000
BestSolution_Min = 0.000000
BestSolution_Max = 34.000000
 0 19 20 21 23 24 25 26 27 28 29 30 31 32 33 34
 1  1  2  2  2  3  3  2  6  6 12 18 19 13  8  2
   \endverbatim
   </li>
   <li> n=268
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 1000000 -i VanDerWaerden_2-4-9_268.cnf | tee VanDerWaerden_2-4-9_268.cnf_AUS
Clauses = 16193
Variables = 268
TotalLiterals = 86552
FlipsPerSecond = 47014
BestStep_Mean = 489412.130000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 29.910000
BestSolution_Median = 31.000000
BestSolution_Min = 20.000000
BestSolution_Max = 35.000000
20 21 22 23 26 27 28 29 30 31 32 33 34 35
 3  1  1  4  2  3  7  8 17 23 18  7  5  1
   \endverbatim
   but found satisfiable with cutoff=2*10^6 (seed=1345857851, osteps=160923):
   \verbatim
 0 19 20 21 22 23 24 26 27 28 29 30 31 32 33 34
 4  1  4  2  1  1  2  7  3 12 17 16 16  9  3  2
100
   \endverbatim
   </li>
   <li> n=269 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=996923753, osteps=1099230). </li>
   <li> n=270 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=3659785745, osteps=807607). </li>
   <li> n=271 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=531082902, osteps=278105). </li>
   <li> n=272 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1970325183, osteps=727890). </li>
   <li> n=273 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1821288488, osteps=474978). </li>
   <li> n=274 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=96564019, osteps=966284). </li>
   <li> n=275 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=83432612, osteps=343301):
   \verbatim
Clauses = 17057
Variables = 275
TotalLiterals = 91178
FlipsPerSecond = 45803
BestStep_Mean = 931362.360000
Steps_Mean = 1945969.930000
Steps_Max = 2000000.000000
PercentSuccess = 5.00
BestSolution_Mean = 32.160000
BestSolution_Median = 35.000000
BestSolution_Min = 0.000000
BestSolution_Max = 39.000000
 0 24 25 26 28 29 30 31 32 33 34 35 36 37 38 39
 5  2  1  2  4  3  1  2  6  7 16 18 24  4  4  1
   \endverbatim
   </li>
   <li> n=276 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=2699968525, osteps=634428). </li>
   <li> n=277 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1868050270, osteps=757488). </li>
   <li> n=278 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1739242999, osteps=1644942). </li>
   <li> n=279 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1463764243, osteps=318520). </li>
   <li> n=280 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1380488068, osteps=1042265). </li>
   <li> n=281 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=3596813003, osteps=701978). </li>
   <li> n=290 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1761025677, osteps=839031).  </li>
   <li> n=295 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1176168555, osteps=1693117).  </li>
   <li> n=296 seemed unsatisfiable:
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 2000000 -i VanDerWaerden_2-4-9_296.cnf -solve | tee VanDerWaerden_2-4-9_296.cnf_AUS
Clauses = 19783
Variables = 296
TotalLiterals = 105772
FlipsPerSecond = 41502
BestStep_Mean = 886547.620000
Steps_Mean = 2000000.000000
Steps_Max = 2000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 49.470000
BestSolution_Median = 51.000000
BestSolution_Min = 1.000000
BestSolution_Max = 55.000000
 1 40 42 43 45 46 47 48 49 50 51 52 53 54 55
 2  1  1  2  2  4  6  8  6 11 16 11 19  7  4
   \endverbatim
   but then a solution was found (seed=1986226015, osteps=154011). </li>
   <li> n=300 seemed unsatisfiable:
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 2000000 -i VanDerWaerden_2-4-9_300.cnf -solve | tee VanDerWaerden_2-4-9_300.cnf_AUS
Clauses = 20326
Variables = 300
TotalLiterals = 108684
FlipsPerSecond = 40762
BestStep_Mean = 887586.680000
Steps_Mean = 2000000.000000
Steps_Max = 2000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 54.620000
BestSolution_Median = 55.000000
BestSolution_Min = 1.000000
BestSolution_Max = 62.000000
 1 46 47 48 50 51 52 53 54 55 56 57 58 59 60 62
 1  1  1  2  4  2  6  8 14 14 10 15 11  7  3  1
   \endverbatim
   but (using cutoff=4*10^6) a solution was found (seed=483608069,
   osteps=1595734). </li>
   <li> n=301:
    <ol>
     <li> 200 runs with cutoff=4*10^6:
     \verbatim
 1  2  3  4 42 43 44 46 47 48 49 50 51 52 53 54 55 56 57 58 59
 3  8  4  1  2  1  1  3  1  2  4  6 16 17 16 23 24 30 19 18  1
     \endverbatim
     </li>
     <li> 62 runs with cutoff=8*10^6 found a solution (seed=1496615047,
     osteps=5004399). </li>
    </ol>
   </li>
   <li> n=302 found satisfiable by adaptnovelty+ with cutoff=8*10^6
   (seed=4209052746, osteps=4948335). </li>
   <li> n=303
    <ol>
     <li>
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 200 -cutoff 8000000 -i VanDerWaerden_2-4-9_303.cnf -solve | tee VanDerWaerden_2-4-9_303.cnf_AUS
Clauses = 20737
Variables = 303
TotalLiterals = 110883
FlipsPerSecond = 41760
BestStep_Mean = 3623527.685000
Steps_Mean = 8000000.000000
Steps_Max = 8000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 48.345000
BestSolution_Median = 54.000000
BestSolution_Min = 1.000000
BestSolution_Max = 59.000000
 1  2  3  4 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59
 3 12  7  1  1  1  1  1  6  6  7  8  9 11 29 26 21 33 12  5
     \endverbatim
     </li>
     <li> 400 runs with cutoff=16*10^6 on cs-wsok: A solution was found in
     round 90 (seed=477566865, osteps=2808546). </li>
    </ol>
   </li>
   <li> n=304 found satisfiable in run 3 with cutoff=16*10^6
   (seed=2362779736, osteps=11583417). </li>
   <li> n=305
    <ol>
     <li> 200 runs with cutoff=4*10^6:
     \verbatim
 2  3  5 51 52 53 54 55 56 57 58 59 60 61 62 63 64
 3 12  1  1  5  7  7 15 17 17 26 21 30 22 12  3  1
     \endverbatim
     </li>
     <li> cutoff=16*10^6: found a solution in run 87 (seed=3154369446,
     osteps=8862697). </li>
    </ol>
   </li>
   <li> n=306 found satisfiable in run 105 (cutoff=16*10^6, seed=4147819434,
   osteps=390227). </li>
   <li> n=307
    <ol>
     <li> cutoff=16*10^6:
     \verbatim
 1  2  3  4  5  6  7 46 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62
 4 18 31 26  7  1  1  1  2  4  6 10 11 12 21 26 39 61 38 46 25  6  4
     \endverbatim
     </li>
     <li> cutoff=32*10^6: Solution found in run 41 (seed=3802003483,
     osteps=106667). </li>
    </ol>
   </li>
   <li> n=308 found satisfiable in run 247 with cutoff=32*10^6
   (seed=2621501336, osteps=4647811), while with cutoff=10^6 and 4000 runs no
   solutions were found:
   \verbatim
  1   2   3   4   5   6   7  46  47  49  50  51  52  53  54  55  56  57  58  59
  1   6  32  12  17   2   2   1   2   1   3   4   9  10  13  23  33  46  94 118
 60  61  62  63  64  65  66  67  68  69  70  71  72
147 242 291 412 527 555 505 424 272 130  49  16   1
   \endverbatim
   </li>
   <li> n=309
    <ol>
     <li> cutoff=2*10^6
     \verbatim
  1   2   3   4   5   6   7   8  46  47  48  49  50  51  52  53  54  55  56  57
  5  11  44  25  27   5   3   1   2   1   1   5   7   6  16  13  22  29  42  49
 58  59  60  61  62  63  64  65  66  67  68  69  70
 94 134 213 272 408 463 535 571 470 304 157  60   5
4000
     \endverbatim
     </li>
     <li> cutoff=4*10^6:
     \verbatim
  1   2   3   4   5   6   7   8  45  46  47  48  49  50  51  52  53  54  55  56
 10  25  81  55  46  18   4   4   1   1   1   5   7   6   7  16  26  29  49  72
 57  58  59  60  61  62  63  64  65  66  67  68
125 176 249 346 446 517 514 547 369 181  59   8
4000
     \endverbatim
     </li>
     <li> cutoff=8*10^6:
     \verbatim
  1   2   3   4   5   6   7   8  45  47  48  49  50  51  52  53  54  55  56  57
 19  36 165 112  82  26  13   5   2   4   4   7  16  22  28  47  65  98 133 187
 58  59  60  61  62  63  64  65  66  67
294 381 478 561 508 403 217  70  16   1
4000
     \endverbatim
     </li>
     <li> cutoff=16*10^6
     \verbatim
  1   2   3   4   5   6   7   8  44  45  46  47  48  49  50  51  52  53  54  55
 33  88 295 221 162  56  21   9   1   3   2   7  18  27  37  39  54  73 117 159
 56  57  58  59  60  61  62  63  64
182 271 415 465 475 417 222 100  31
4000
     \endverbatim
     </li>
     <li> cutoff=32*10^6
     \verbatim
 1  2  3  4  5  6  7  8 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62
 6 16 52 31 26  6  4  3  1  1  1  4  1  5 11  8 11 17 28 27 34 39 32 23  9  4
400
     \endverbatim
     </li>
     <li> cutoff=64*10^6
     \verbatim
 1  2  3  4  5  6  7 44 45 48 49 50 51 52 53 54 55 56 57 58 59 60
13 22 87 66 32 11  8  1  1  1  2  9  9 10 15 13 15 25 25 21  7  7
400
     \endverbatim
     </li>
     <li> cutoff=128*10^6
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 400 -cutoff 128000000 -i VanDerWaerden_2-4-9_309.cnf -solve | tee VanDerWaerden_2-4-9_309.cnf_AUS3
  1   2   3   4   5   6   7   8  46  49  50  51  52  53  54  55  56  57  58
 22  63 156  60  39  11   1   1   1   1   1   5   6   7  11   4   3   5   3
400
> ubcsat-okl -alg samd -runs 400 -cutoff 128000000 -i VanDerWaerden_2-4-9_309.cnf -solve | tee VanDerWaerden_2-4-9_309.cnf_AUS6
  2   3   4   5   7  45  46  47  48  49  50  51  52  53  54  55
  1   6   1   3   1   2   9  28  64 121 106  40  15   1   1   1
400
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=310
    <ol>
     <li> 200 runs with cutoff=4*10^6:
     \verbatim
 2  3  4  5  7 52 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69
 2  3  4  4  1  1  3  2  3  2  8  4 14 15 22 35 27 23 10 12  4  1
     \endverbatim
     </li>
     <li> 200 runs with cutoff=6*10^6:
     \verbatim
 2  3  4  5  6 51 52 55 56 57 58 59 60 61 62 63 64 65 66 67 68
 1  9  2  2  2  1  3  1  8  6  8 12 22 21 24 27 22 15  9  4  1
     \endverbatim
     </li>
     <li> cutoff=16*10^6, 400 runs:
     \verbatim
 2  3  4  5  6  7  8 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65
 6 29 20 22  8  3  3  2  1  9  3  4  8 10 13 20 28 40 37 55 44 21 12  2
     \endverbatim
     </li>
     <li> cutoff=32*10^6
     \verbatim
 2  3  4  5  6  7  8 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
14 45 33 30 17  5  2  3  1  1  2  9  9 10 17 27 23 29 54 36 16 10  7
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo vanderwaerden_2(4,10) > 328
  <ul>
   <li> We don't have a nice prediction, except of that it's greater (or 
   equal) than 309. </li>
   <li> Evaluating
   \verbatim
> E = eval_ubcsat("VanDerWaerden_2-4-10_350.cnf", runs=100,cutoff=100000,monitor=TRUE)
   \endverbatim
   by plot(E$alg,E$best): It seems that gsat_tabu, rots and samd are best,
   where samd reached the best min=37, but this might be just an outlier, and
   perhaps rots is best. </li>
   <li> Using cutoff=10^6: the two best min-values are reached by rnovelty
   (twice min=8) and rots (twice min=9), while otherwise the best min=34.
   Considering the success of rnovelty as just luck, rots would again be best 
   here. </li>
   <li> Evaluating
   \verbatim
E = eval_ubcsat("VanDerWaerden_2-4-10_330.cnf", runs=100,cutoff=100000,monitor=TRUE)
   \endverbatim
   again rots seems best (reaching min=26). </li>
   <li> The same, but with cutoff=10^6: three algorithms, each with one run,
   reached min=3, namely rots, rsaps and sapsnr, while otherwise the best min 
   is 26. Again rots seems best. </li>
   <li> The same, but with cutoff=4*10^6: the three best algorithms seem now
   adaptnovelty+, rnovelty+ and saps (the best is min=2; perhaps adapnovelty+ 
   is best):
   \verbatim
> table(E$best[E$alg=="adaptnoveltyp"])

 3  4  5 28 29 30 31 32 33 34 35
 1  1  1  5  5 11 14 22 20 14  6
> table(E$best[E$alg=="rnoveltyp"])

 2  4 28 30 31 32 33 34 35 36 37 38 39 40 41 42
 1  2  1  2  1  1  7  3 11 15 15 17 13  8  2  1
> table(E$best[E$alg=="saps"])

 2 28 31 33 34 35 36 37 38
 1  1  2  8 14 19 36 18  1
> table(E$best[E$alg=="irots"])

 5 30 31 32 33 34 35 36 37
 1  2  7 10 18 23 25 11  3
   \endverbatim
   </li>
   <li> Evaluating (cutoff=10^5)
   \verbatim
>  E = eval_ubcsat("VanDerWaerden_2-4-10_320.cnf", runs=100,cutoff=100000,monitor=TRUE)
   \endverbatim
   gsat-tabu and samd appear best (both reaching min=23).
   </li>
   <li> Now using cutoff=10^6: Three solver reached min=1:
   \verbatim
 table(E$best[E$alg=="novelty"])
 1 42 44 45 46 47 48 49 50 51 52
 1  1  7  6  9 17 21 14 19  3  2
> table(E$best[E$alg=="rnovelty"])
 1 26 29 30 31 32 33 34 35 36 37 38
 1  1  9  7 11 12 17 16 13  7  5  1
> table(E$best[E$alg=="rots"])
 1 23 24 25 26 27 28 29
 1  2  7 24 39 20  6  1
   \endverbatim
   but these could well be outliers. Unclear situation. </li>
   <li> cutoff=10^7: four solvers found solutions:
   \verbatim
> table(E$best[E$alg=="adaptnoveltyp"])
 0  2  3 20 21 22 23 24 25 26 27 28
 2  1  2  2  2  6 16 21 26 17  4  1
> table(E$best[E$alg=="rnoveltyp"])
 0  1 24 25 26 27 28 29 30 31 32 33
 1  2  3  3  4  6 17 25 21 10  7  1
> table(E$best[E$alg=="rsaps"])
 0  3  4 26 27 28 29 30 31 32
 1  1  1  3  5 12 23 32 20  2
> table(E$best[E$alg=="noveltyp"])
 0 39 40 41 42 43 44 45 46 47 48
 1  2  4  4  6 11 18 23 15 15  1
   \endverbatim
   where the above order seems to be appropriate for their ranking.
   So perhaps for now we should assume that adaptnovelty+ is best. </li>
   <li> n=321 with adaptnovelty+
    <ol>
     <li> cutoff=10^7:
     \verbatim
> nohup ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 10000000 -i VanDerWaerden_2-4-10_321.cnf | tee VanDerWaerden_2-4-10_321.cnf_OUT &
> E=read_ubcsat("VanDerWaerden_2-4-10_321.cnf_OUT")
 0  1  2 21 22 23 24 25 26 27
 1  2  2  1  6 12 24 25 18  9
100
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  36040 1817000 3963000 4553000 7101000 9987000
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=322, cutoff=10^7: a solution was found in run 51 (seed=3424017434,
   osteps=2557765). </li>
   <li> n=323, cutoff=10^7: a solution was found in run 22 (seed=420342374,
   osteps=9743243). </li>
   <li> n=324
    <ol>
     <li> cutoff=10^7 found one solution in 1000 runs:
     \verbatim
> nohup ubcsat-okl -alg adaptnovelty+ -runs 1000 -cutoff 10000000 -i VanDerWaerden_2-4-10_324.cnf | tee VanDerWaerden_2-4-10_324.cnf_OUT &
> E=read_ubcsat("VanDerWaerden_2-4-10_324.cnf_OUT")
  0   1   2   3   4   5  18  21  22  23  24  25  26  27  28  29  30  31
  1  11  19   9   4   2   1   2   9  29  69 143 193 254 178  66   9   1
1000
> E[E$min==0,]
    sat min  osteps  msteps       seed
993   1   0 2065594 2065594 2579089101
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   8842 1877000 4370000 4471000 6848000 9996000
     \endverbatim
     </li>
     <li> cutoff=10^8 found three solution:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 100000000 -i VanDerWaerden_2-4-10_324.cnf | tee VanDerWaerden_2-4-10_324.cnf_OUT2
> E = read_ubcsat("VanDerWaerden_2-4-10_324.cnf_OUT2")
 0  1  2  3  4  5 21 22 23 24 25 26
 3  7 13  4  6  2  1  6 12 20 12  4
90
> E[E$min==0,]
   sat min   osteps   msteps       seed
38   1   0 67111454 67111454 3357914690
48   1   0 37435932 37435932 3595047384
62   1   0 68099425 68099425  748374078
     \endverbatim
     </li>
     <li> So it seems one needs to use cutoff=10^8 now. </li>
    </ol>
   </li>
   <li> n = 325, cutoff=10^8: found one solution (seed=1755037834,
   osteps=38900247):
   \verbatim
> E=read_ubcsat("VanDerWaerden_2-4-10_325.cnf_OUT2")
 1  2  3  4  5 21 22 23 24 25 26 27
10 17  7  6  2  2  6 11 15 16  7  1
100
> E = read_ubcsat("VanDerWaerden_2-4-10_325.cnf_OUT")
 0  1  2  3  5 20 21 22 23 24 25 26
 1  6 18  4  4  1  2  4  9 13 16  3
81
   \endverbatim
   </li>
   <li> n = 326, cutoff=10^8: found one solution (seed=64752191,
   osteps=96247637) in 162 runs:
   \verbatim
> E = read_ubcsat("VanDerWaerden_2-4-10_326.cnf_OUT")
 0  1  2  3  4  5 21 22 23 24 25 26 27
 1  6 33 18  6  4  4  4  9 18 36 15  8
162
   \endverbatim
   So we should use cutoff=2*10^8 now. </li>
   <li> n=327, cutoff=2*10^8: In 81 runs one solution was found 
   (seed=403939055, osteps=23019617):
   \verbatim
> E = read_ubcsat("VanDerWaerden_2-4-10_327.cnf_OUT")
 0  1  2  3  4  5 22 23 24 25 26 27
 1  1 28 12  5  3  2  5  9  8  6  1
81
   \endverbatim
   And in another 112 runs also one solution was found:
   \verbatim
> nohup ubcsat-okl -alg adaptnovelty+ -runs 200 -cutoff 200000000 -i VanDerWaerden_2-4-10_327.cnf > VanDerWaerden_2-4-10_327.cnf_OUT &
 0  1  2  3  4  5 21 22 23 24 25 26
 1  3 33 11 11  3  1  3  5  9 17 15
112
> summary(E$osteps)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
   525600  32000000  91600000  89860000 138000000 199800000
   \endverbatim
   </li>
   <li> n=328
    <ol>
     <li> cutoff=2*10^8:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 200 -cutoff 200000000 -i VanDerWaerden_2-4-10_328.cnf | tee VanDerWaerden_2-4-10_328.cnf_OUT
> E = read_ubcsat("VanDerWaerden_2-4-10_328.cnf_OUT")
 1  2  3  4  5 21 22 23 24 25 26 27 28
 2 51 45 28  9  1  7  3 12 17 19  4  2
200
> summary(E$osteps)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
   599500  40370000  84970000  92870000 141000000 198300000
     \endverbatim
     </li> 
     <li> cutoff=4*10^8:
     \verbatim
> nohup ubcsat-okl -alg adaptnovelty+ -runs 200 -cutoff 400000000 -i VanDerWaerden_2-4-10_328.cnf > VanDerWaerden_2-4-10_328.cnf_OUT &
> E=read_ubcsat("VanDerWaerden_2-4-10_328.cnf_OUT")
 0  1  2  3  4  5 23 24 25 26
 1  1 29 17  4  2  1  1  1  1
58
> summary(E$osteps)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
  1000000  36620000 124200000 146200000 228000000 397700000
     \endverbatim
     So cutoff=4*10^8 seems needed now.
     </li>
    </ol>
   </li>
   <li> n=329
    <ol>
     <li>cutoff=4*10^8:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 200 -cutoff 400000000 -i VanDerWaerden_2-4-10_329.cnf | tee VanDerWaerden_2-4-10_329.cnf_OUT
> E = read_ubcsat("VanDerWaerden_2-4-10_329.cnf_OUT")
 1  2  3  4  5  6 21 23 24 25 26 27
 7 75 53 26 12  2  2  2  9  6  5  1
200
> summary(E$osteps)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
  4042000  78170000 166600000 178900000 260800000 397600000

> E=read_ubcsat("VanDerWaerden_2-4-10_329.cnf_OUT2")
 1  2  3  4  5  6 22 23 24 25 26 27
 3 47 31 17  4  3  1  2  5  5  6  2
126
> summary(E$osteps)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
  1242000  67280000 164600000 174800000 284900000 394100000
     \endverbatim
     </li>
     <li> cutoff=5*10^8:
     \verbatim
  1   2   3   4   5   6  23  24  25  26
  5 103  59  31  10   3   2   7  12   8
240
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=330 with adaptnovelty+
    <ol>
     <li> cutoff=10^7 
     \verbatim
 2  4 23 26 28 29 30 31 32 33 34
 5  3  2  3 11 17 24 19  9  5  2
100
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  11110 1841000 4807000 4775000 7835000 9856000
     \endverbatim
     </li>
     <li> cutoff=10^8:
     \verbatim
 2  3  4  5  6 22 23 24 25 26 27 28 29 30
33 50 39 22  3  1  1  9 23 40 43 53 11  4
332
  2   3   4   5   6  21  22  23  24  25  26  27  28  29  30
 84 113 112  48  18   2   3  13  19  62 117 189 162  51   7
1000
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>

*/

