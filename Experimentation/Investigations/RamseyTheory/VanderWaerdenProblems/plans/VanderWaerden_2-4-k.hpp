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
  </ul>


  \todo vanderwaerden_2(4,9)
  <ul>
   <li> [Ahmed 2009] states vanderwaerden4k(9) > 254. </li>
   <li> n=254 found satisfiable by adaptnovelty+ (first run with cutoff=10^6;
   seed=719877201, osteps=677160). </li>
   <li> n=255 </l>
  </ul>

*/

