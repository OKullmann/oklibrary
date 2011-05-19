// Oliver Kullmann, 28.12.2008 (Swansea)
/* Copyright 2008, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/LatinSquares/plans/OrthogonalSquares.hpp
  \brief On investigations regarding mutually orthogonal latin squares


  \todo Experiments with the example from [Knuth, Vol. 4, Fascicle 0]
  <ul>
   <li> Given dk10_rls (see
   ComputerAlgebra/Satisfiability/Lisp/Generators/LatinSquares.mac), the task
   is to find the (unique) orthogonal latin square dk10_o_hrls. </li>
   <li> And this using the direct translation into a (boolean) SAT problem.
   </li>
   <li>
   Easy for the OKsolver-2002:
   \verbatim
LS_b : strong_ls(10)$
LS_o : orthogonality_cond_ls(dk10_rls)$
F : [LS_b[1], union(LS_b[2],LS_o,row_symmetrybreaking_ls(10))]$
SF : standardise_fcs(F)$
output_fcs_v(sconcat("The orthogonal latin square problem (weak form) with dimension ", p, " and matrix according to DK."),SF[1],"KOLS.cnf",SF[2]);
> OKsolver_2002-O3-DNDEBUG KOLS.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=10 initial_number_of_variables=1000 initial_number_of_clauses=18310 initial_number_of_literal_occurrences=39010 running_time(s)=862.1 number_of_nodes=361731 number_of_single_nodes=94 number_of_quasi_single_nodes=0 number_of_2-reductions=3801745 number_of_pure_literals=0 number_of_autarkies=2 number_of_missed_single_nodes=1338 max_tree_depth=63 number_of_table_enlargements=0 reduced_maximal_clause_length=2 reduced_number_of_variables=280 reduced_number_of_clauses=7960 reduced_number_of_literal_occurrences=16690 number_of_1-autarkies=0 number_of_initial_unit-eliminations=280 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=10080 file_name=KOLS.cnf
k
   \endverbatim
   </li>
   <li> But hard for march_pl ("march_pl KOLS.cnf": Aborted after 12 hours
   (where memory usage reached 1GB): progress was made, but unclear how long
   it will take). </li>
   <li> Even easier for OKsolver-2002 with the added dual conditions:
   \verbatim
LS_b : strong_ls(10)$
LS_o : orthogonality_strongcond_ls(dk10_rls)$
Fs : [LS_b[1], union(LS_b[2],LS_o,row_symmetrybreaking_ls(10))]$
SFs : standardise_fcs(Fs)$
output_fcs_v(sconcat("The orthogonal latin square problem (strong form) with dimension ", p, " and matrix according to DK."),SFs[1],"KOLSs.cnf",SFs[2]);
> OKsolver_2002-O3-DNDEBUG KOLSs.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=10 initial_number_of_variables=1000 initial_number_of_clauses=18410 initial_number_of_literal_occurrences=40010 running_time(s)=515.4 number_of_nodes=223649 number_of_single_nodes=1 number_of_quasi_single_nodes=0 number_of_2-reductions=2280396 number_of_pure_literals=0 number_of_autarkies=4 number_of_missed_single_nodes=264 max_tree_depth=48 number_of_table_enlargements=0 reduced_maximal_clause_length=2 reduced_number_of_variables=280 reduced_number_of_clauses=7970 reduced_number_of_literal_occurrences=16970 number_of_1-autarkies=0 number_of_initial_unit-eliminations=280 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=10080 file_name=KOLSs.cnf
   \endverbatim
   (just 8 1/2 minutes)
   </li>
   <li> But still (too) hard for march_pl: stopped after 14 hours (where some
   progress was apparently made, but unclear how much; this time only 150MB
   memory usage). </li>
   <li> Apparently hard for local search algorithms:
   rnovelty+ seems best.
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 10000 -cutoff 1000000 -i KOLSs.cnf
Clauses = 18410
Variables = 1000
TotalLiterals = 40010
FlipsPerSecond = 490886
BestStep_Mean = 407466.394800
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 10.967400
BestSolution_Median = 12.000000
BestSolution_Min = 4.000000
BestSolution_Max = 16.000000
   \endverbatim
   Of course, one should eliminate the unit-clauses in the input.
   </li>
   <li> But it doesn't get much better:
   \verbatim
uSFs : standardise_fcs(cs2fcs(ucp_cs_0(SFs[1][2])))$
output_fcs_v(sconcat("The orthogonal latin square problem (strong form) with dimension ", p, " and matrix according to DK, UCP applied."),uSFs[1],"uKOLSs.cnf",uSFs[2]);
> ubcsat -alg rnovelty+ -runs 10000 -cutoff 1000000 -i uKOLSs.cnf
> summary(E)
 sat            min              omin     
 0:10000   Min.   : 4.000   Min.   :  3544
           1st Qu.: 8.000   1st Qu.:138374
           Median :10.000   Median :318611
           Mean   : 9.736   Mean   :381306
           3rd Qu.:10.000   3rd Qu.:597135
           Max.   :14.000   Max.   :999867
   \endverbatim
   Interessting:
   \verbatim
> table(E$min)
   4    6    8    9   10   11   12   14
  37  291 2305    1 5700    5 1647   14
   \endverbatim
   </li>
   <li> Also with more steps it's hard to break through the barrier
   min=4:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 1000 -cutoff 10000000 -i uKOLSs.cnf
Clauses = 10440
Variables = 720
TotalLiterals = 23040
FlipsPerSecond = 687051
BestStep_Mean = 3393163.045000
Steps_Mean = 10000000.000000
Steps_Max = 10000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 7.476000
BestSolution_Median = 8.000000
BestSolution_Min = 4.000000
BestSolution_Max = 10.000000
   \endverbatim
   </li>
   <li> Finally 1 success out of 100 with 10^8 steps:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 100 -cutoff 100000000 -i uKOLSs.cnf
Clauses = 10440
Variables = 720
TotalLiterals = 23040
FlipsPerSecond = 721981
BestStep_Mean = 32119641.610000
Steps_Mean = 99329630.960000
Steps_Max = 100000000.000000
PercentSuccess = 1.00
BestSolution_Mean = 5.560000
BestSolution_Median = 6.000000
BestSolution_Min = 0.000000
BestSolution_Max = 8.000000
> table(E$min)
 0  4  6  8
 1 26 66  7
   \endverbatim
   (seed = 3644605810). </li>
   <li> Also unsatisfiability, when forbidding the single solution,
   is relatively easy for the OKsolver-2002:
   \verbatim
Fs_s : [LS_b[1], union(LS_b[2],LS_o,row_symmetrybreaking_ls(10),{exluding_solution_ls(dk10_o_hrls)})]$
SFs_s : standardise_fcs(Fs_s)$
output_fcs_v(sconcat("The orthogonal latin square problem (strong form) with dimension ", p, " and matrix according to DK, with excluded solution."),SFs_s[1],"KOLSs_s.cnf",SFs_s[2]);

s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=100 initial_number_of_variables=1000 initial_number_of_clauses=18411 initial_number_of_literal_occurrences=40110 running_time(s)=-538.6 number_of_nodes=1490035 number_of_single_nodes=20 number_of_quasi_single_nodes=0 number_of_2-reductions=15902639 number_of_pure_literals=0 number_of_autarkies=17 number_of_missed_single_nodes=1794 max_tree_depth=68 number_of_table_enlargements=0 reduced_maximal_clause_length=10 reduced_number_of_variables=280 reduced_number_of_clauses=7970 reduced_number_of_literal_occurrences=16980 number_of_1-autarkies=0 number_of_initial_unit-eliminations=280 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=10080 file_name=KOLSs_s.cnf
   \endverbatim
   (roughly one hour). </li>
   <li> Minisat needed 5.3 hours to show unsatisfiability. </li>
   <li> march_pl was stopped after 11 1/2 hours. </li>
  </ul>

*/

