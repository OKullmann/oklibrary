// Oliver Kullmann, 3.2.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_2-5-5_OKsolver.hpp
  \brief Investigations on OKsolver for greentao_2(5)


  \todo greentao_2(5) : threshold behaviour of OKsolver-2002
  <ul>
   <li> Investigating the threshold of solvability for the OKsolver.
   One main conjecture is that Green-Tao problems behave like random problems.
   </li>
   <li> And, similarly to random problems, there seems
   to be a threshold, where below it OKsolver solves the problem
   without backtracking, while as soon as backtracking starts, the
   problems get very hard. </li>
   <li> This threshold seems to lie between n=20000 and n = 25000.
   <li> n = 20625, density = 14.29895757575758 below the threshold:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=20623 initial_number_of_clauses=294916 initial_number_of_literal_occurrences=1474580 running_time(s)=1961.7 number_of_nodes=6058 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=31532 number_of_pure_literals=4170 number_of_autarkies=17 number_of_missed_single_nodes=0 max_tree_depth=4548 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=876707 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_20625.cnf
   \endverbatim
   number_of_nodes=6058, max_tree_depth=4548, number_of_autarkies=17. </li>
   <li> n = 20938, density = 14.45181010602732 below the threshold:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=20936 initial_number_of_clauses=302592 initial_number_of_literal_occurrences=1512960 running_time(s)=-134.9 number_of_nodes=13317 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=166810 number_of_pure_literals=10313 number_of_autarkies=10 number_of_missed_single_nodes=1 max_tree_depth=4642 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=915632 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_20938.cnf
   \endverbatim
   number_of_nodes=13317, max_tree_depth=4642, number_of_autarkies=10. </li>
   <li> n = 21094, density = 14.52175974210676 below the threshold:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=21092 initial_number_of_clauses=306322 initial_number_of_literal_occurrences=1531610 running_time(s)=-1124.3 number_of_nodes=11418 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=141272 number_of_pure_literals=6025 number_of_autarkies=10 number_of_missed_single_nodes=0 max_tree_depth=4599 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=901676 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21094.cnf
   \endverbatim
   number_of_nodes=11418, max_tree_depth=4599, number_of_autarkies=10. </li>
   <li> n = 21133, density = 14.53802110443382 below the threshold:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=21131 initial_number_of_clauses=307232 initial_number_of_literal_occurrences=1536160 running_time(s)=-1440.9 number_of_nodes=42649 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=768325 number_of_pure_literals=37735 number_of_autarkies=29 number_of_missed_single_nodes=0 max_tree_depth=4607 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=1173442 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21133.cnf
   \endverbatim
   number_of_nodes=42649, max_tree_depth=4607, number_of_autarkies=29. </li>
   <li> n = 21153, density = 14.5477237271309 below the threshold:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=21151 initial_number_of_clauses=307728 initial_number_of_literal_occurrences=1538640 running_time(s)=-1644.1 number_of_nodes=37709 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=692225 number_of_pure_literals=23458 number_of_autarkies=7 number_of_missed_single_nodes=1 max_tree_depth=4601 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=1952904 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21153.cnf
   \endverbatim
   number_of_nodes=37709, max_tree_depth=4601, number_of_autarkies=7. </li>
   <li> n = 21163, density = 14.55086707933658 below (or about) the threshold:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=21161 initial_number_of_clauses=307940 initial_number_of_literal_occurrences=1539700 running_time(s)=-1325.5 number_of_nodes=103433 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=1964466 number_of_pure_literals=53123 number_of_autarkies=12 number_of_missed_single_nodes=5 max_tree_depth=4593 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=2295888 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21163.cnf
   \endverbatim
   number_of_nodes=103433, max_tree_depth=4593, number_of_autarkies=12. </li>
   <li> n = 21167, density = 14.55246374072849 below (or about) the threshold:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=21165 initial_number_of_clauses=308032 initial_number_of_literal_occurrences=1540160 running_time(s)=728.8 number_of_nodes=114686 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=2174254 number_of_pure_literals=58330 number_of_autarkies=12 number_of_missed_single_nodes=4 max_tree_depth=4620 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=2659174 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21167.cnf
   \endverbatim
   number_of_nodes=114686, max_tree_depth=4620, number_of_autarkies=12. </li>
   <li> n = 21170, density = 14.55370807746812 above the threshold:
   \verbatim
s UNKNOWN
c sat_status=2 initial_maximal_clause_length=5 initial_number_of_variables=21168 initial_number_of_clauses=308102 initial_number_of_literal_occurrences=1540510 running_time(s)=522.1 number_of_nodes=498048 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=9099379 number_of_pure_literals=200245 number_of_autarkies=8 number_of_missed_single_nodes=19 max_tree_depth=2378 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=23567938 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21170.cnf
   \endverbatim
   number_of_nodes=498048, max_tree_depth=2378, number_of_autarkies=8. </li>
   <li> Regarding the 13 missed single nodes: It would be interesting to run
   the OKsolver-2002 with the order of branches inverted. </li>
   <li> n = 21172, density = 14.55403362932175 above the threshold:
   \verbatim
s UNKNOWN
c sat_status=2 initial_maximal_clause_length=5 initial_number_of_variables=21170 initial_number_of_clauses=308138 initial_number_of_literal_occurrences=1540690 running_time(s)=-1760.2 number_of_nodes=410857 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=7531575 number_of_pure_literals=166713 number_of_autarkies=8 number_of_missed_single_nodes=15 max_tree_depth=2366 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=18406255 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21172.cnf
   \endverbatim
   number_of_nodes=410857, max_tree_depth=2366, number_of_autarkies=8 </li>
   <li> n = 21250, density = 14.59143529411765 above the threshold:
   \verbatim
s UNKNOWN
c sat_status=2 initial_maximal_clause_length=5 initial_number_of_variables=21248 initial_number_of_clauses=310068 initial_number_of_literal_occurrences=1550340 running_time(s)=138.1 number_of_nodes=421583 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=8373297 number_of_pure_literals=187646 number_of_autarkies=14 number_of_missed_single_nodes=39 max_tree_depth=1187 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=15088691 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21250.cnf
   \endverbatim
   number_of_nodes=421583, max_tree_depth=1187, number_of_autarkies=14. </li>
   <li> This should be repeated, until around 400000 nodes:
   n = 22500, density = 15.2064 above the threshold:
   \verbatim
s UNKNOWN
c sat_status=2 initial_maximal_clause_length=5 initial_number_of_variables=22498 initial_number_of_clauses=342144 initial_number_of_literal_occurrences=1710720 running_time(s)=-1872.7 number_of_nodes=28974 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=516260 number_of_pure_literals=2207 number_of_autarkies=0 number_of_missed_single_nodes=2 max_tree_depth=613 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=63014 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_22500.cnf
   \endverbatim
   number_of_nodes=28974, max_tree_depth=613. </li>
   <li> Again, zero autarkies. </li>
  </ul>

*/

