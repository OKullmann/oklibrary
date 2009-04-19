// Oliver Kullmann, 17.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/SchurProblems/plans/general.hpp
  \brief On computing schur(5)


  \todo OKsolver_2002
  <ul>
   <li> With a cluster and a few months it looks feasible without any
   special action:
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D20 Schur_5_160.cnf

Name of file and number of monitoring nodes:
 Schur_5_160.cnf, 1048576
  1:   387,   15.4, 16179512.2
  2: 13720,  387.6, 203208398.3
  3:  1568,  441.4, 154290526.5
  4: 17781,  969.0, 254011324.1
  5:  4987, 1102.6, 231232974.1
  6:  6499, 1310.7, 229063611.7
  7: 18361, 1819.8, 272591989.1
  8: 40898, 2820.2, 369652987.8
  9: 10636, 3088.2, 359799455.0
 10: 24184, 3686.7, 386575875.8
 11: 13620, 4012.0, 382437393.0
 12: 35767, 4868.0, 425370957.9
 13: 10667, 5114.3, 412514363.2
 14:  7705, 5335.4, 399605480.3
 15: 18446, 5912.8, 413326668.3

s UNKNOWN
c sat_status=2 initial_maximal_clause_length=5 initial_number_of_variables=800 initial_number_of_clauses=33760 initial_number_of_literal_occurrences=99600 running_time(s)=6116.4 number_of_nodes=231351 number_of_single_nodes=14 number_of_quasi_single_nodes=0 number_of_2-reductions=3696760 number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=282 max_tree_depth=58 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=65040054 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=2000 file_name=Schur_5_160.cnf
   \endverbatim
   </li>
   <li> And with initial symmetry breaking (using
   output_schur_sb_stdname(5,160)) a factor of nearly 5 should be achievable:
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D20 Schur_sb_5_160.cnf
232: 55174, 149176.8, 674088581.0
s UNKNOWN
c sat_status=2 initial_maximal_clause_length=5 initial_number_of_variables=800 initial_number_of_clauses=33764 initial_number_of_literal_occurrences=99604 running_time(s)=150682.3 number_of_nodes=5582755 number_of_single_nodes=261 number_of_quasi_single_nodes=0 number_of_2-reductions=91981749 number_of_pure_literals=0 number_of_autarkies=2 number_of_missed_single_nodes=6184 max_tree_depth=73 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=6 reduced_number_of_clauses=814 reduced_number_of_literal_occurrences=2574 number_of_1-autarkies=1562724854 number_of_initial_unit-eliminations=6 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=2137 file_name=Schur_sb_5_160.cnf
233: 67478, 151039.1, 679574131.9
   \endverbatim
   </li>
   <li> So it seems that on a rather modest machine with 1 processor around
   8000 days are needed. Splitting the problem into 2^13 = 8192 jobs
   this would need a day per job (see "Simple parallelisation" in
   Solvers/OKsolver/SAT2002/plans/general.hpp), and given 100 processor,
   in altogether 3 months it should be done. </li>
  </ul>


  \todo Local search
  <ul>
   <li> Considering Schur_5_159.cnf. </li>
   <li> From the ubcsat-1.0.0 suite saps seems clearly perform best, and
   a local minimum of one falsified clause is easily reached already with
   cutoff = 10000. </li>
   <li> But finding a solution is difficult: 10 runs with cutoff=10*10^6
   all just yield a minimum=1. </li>
   <li> The worst-performing algorithm (w.r.t. mean minimum) seemed to be
   hsat, which now with cutoff=10*10^6 still doesn't get close to 1. </li>
   <li> Using symmetry-breaking and then the minisat2-preprocessor seems
   to make the problem more difficult. </li>
   <li> Finally by
   \verbatim
ubcsat-okl -alg saps -runs 10000 -cutoff 100000 -i Schur_5_159.cnf
Clauses = 33349
Variables = 795
TotalLiterals = 98380
FlipsPerSecond = 116174
BestStep_Mean = 16171.864100
Steps_Mean = 99993.137900
Steps_Max = 100000.000000
PercentSuccess = 0.01
BestSolution_Mean = 1.445400
BestSolution_Median = 1.000000
BestSolution_Min = 0.000000
BestSolution_Max = 3.000000
   \endverbatim
   we found one solution (seed=830151296, msteps=31379). </li>
   <li> On the other hand we get
   \verbatim
ubcsat-okl -alg saps -runs 40000 -cutoff 200000 -i Schur_5_160.cnf | tee Schur_5_160.out
Clauses = 33760
Variables = 800
TotalLiterals = 99600
FlipsPerSecond = 117567
BestStep_Mean = 23914.642400
Steps_Mean = 200000.000000
Steps_Max = 200000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.351275
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 3.000000
   \endverbatim
   so the conjecture shur(5)=160 seems justified. </li>
  </ul>

*/

