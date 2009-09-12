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
y : log(d[-(1:2)])
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
   \endverbatim
   </li>
  </ul>


  \todo Performance of OKsolver-2002
  <ul>
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

   </li>
  </ul>


  \todo Performance of march_pl


  \todo Performance of minisat2


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
   <li> It seems that gsat-tabu is best from the ubcsat-1-0-0 suite.
   \verbatim

   \endverbatim
   </li>
  </ul>


  \todo vanderwaerden_2(4,9) > 262
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

     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=263 </li>
  </ul>

*/

