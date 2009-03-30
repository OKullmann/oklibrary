// Matthew Gwynne, 30.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AES/plans/OKsolver.hpp
  \brief On investigations into the Advanced Encryption Standard using OKsolver


  \todo Performance of OKsolver on AES instances with 0-16 key bits unknown 
  <ul>
   <li> Run using :
   \verbatim
  for r in `seq 2 2 10`; do
   for k in `seq 0 2 16`; do
       echo "K = " $k "R = " $r;
       ./mergeSameVarCNFs.sh "AES_r${r}.cnf" "AES_UC_r${r}_pn0_kn${k}_cn0_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF.cnf" > "AES_r${r}_kn_${k}_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf" ;
       time /tmp/OKlib/OKlibrary-0.2.1.2_00102/OKplatform/bin/OKsolver_2002-O3-DNDEBUG "AES_r${r}_kn_${k}_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf" > "AES_r${r}_kn_${k}_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.oksolver" 2> "AES_r${r}_kn_${k}_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.oksolver" ;
     done
   done
   \endverbatim
   </li>
   <li> Current results (experiment still running) : 
   \verbatim
cs360678@csnereid:~/Experiments/AES> cat AES_r2*.results.oksolver
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=29000 initial_number_of_literal_occurrences=173136
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=81
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=696 reduced_number_of_clauses=12033
reduced_number_of_literal_occurrences=78281 number_of_1-autarkies=0
number_of_initial_unit-eliminations=696 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1310
file_name=AES_r2_kn_0_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28990 initial_number_of_literal_occurrences=173126
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=81
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=666 reduced_number_of_clauses=10905
reduced_number_of_literal_occurrences=70583 number_of_1-autarkies=0
number_of_initial_unit-eliminations=666 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1319
file_name=AES_r2_kn_10_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28988 initial_number_of_literal_occurrences=173124
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=81
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=660 reduced_number_of_clauses=10833
reduced_number_of_literal_occurrences=70149 number_of_1-autarkies=0
number_of_initial_unit-eliminations=660 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1321
file_name=AES_r2_kn_12_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28986 initial_number_of_literal_occurrences=173122
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=82
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=654 reduced_number_of_clauses=10682
reduced_number_of_literal_occurrences=69159 number_of_1-autarkies=0
number_of_initial_unit-eliminations=654 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1325
file_name=AES_r2_kn_14_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28984 initial_number_of_literal_occurrences=173120
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=84
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=648 reduced_number_of_clauses=10418
reduced_number_of_literal_occurrences=67188 number_of_1-autarkies=0
number_of_initial_unit-eliminations=648 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1329
file_name=AES_r2_kn_16_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28998 initial_number_of_literal_occurrences=173134
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=79
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=690 reduced_number_of_clauses=11918
reduced_number_of_literal_occurrences=77655 number_of_1-autarkies=0
number_of_initial_unit-eliminations=690 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1312
file_name=AES_r2_kn_2_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28996 initial_number_of_literal_occurrences=173132
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=79
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=684 reduced_number_of_clauses=11752
reduced_number_of_literal_occurrences=76662 number_of_1-autarkies=0
number_of_initial_unit-eliminations=684 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1313
file_name=AES_r2_kn_4_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28994 initial_number_of_literal_occurrences=173130
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=81
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=678 reduced_number_of_clauses=11467
reduced_number_of_literal_occurrences=74717 number_of_1-autarkies=0
number_of_initial_unit-eliminations=678 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1317
file_name=AES_r2_kn_6_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28992 initial_number_of_literal_occurrences=173128
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=83
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=672 reduced_number_of_clauses=10968
reduced_number_of_literal_occurrences=70911 number_of_1-autarkies=0
number_of_initial_unit-eliminations=672 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1321
file_name=AES_r2_kn_8_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
   \endverbatim
   </li>
  </ul>

*/
