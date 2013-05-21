// Oliver Kullmann, 1.11.2008 (Swansea)
/* Copyright 2008, 2009, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/docus/Sudoku.hpp
  \brief How to use the Sudoku generator functions


  <h1> Sudoku via Maxima in the OKlibrary </h1>


  <h2> Generating problem instances </h2>

  \verbatim
(%i1) oklib_load_all();
(%i2) P100 : [[1,3,5],[1,7,1],[1,9,8],[2,4,5],[2,8,3],
 [3,5,8],[3,6,9],[3,7,4],[4,3,9],[4,5,1],[4,6,4],[4,9,2],
 [6,1,2],[6,4,3],[6,5,6],[6,7,7],[7,3,7],[7,4,2],[7,5,9],
 [8,2,8],[8,6,5],[9,1,3],[9,3,4],[9,7,6]]$
(%i3) output_sdk_puzzle(3,P100,"Problem 100 in [extreme Sudoku, Gould].","SP100.cnf");

(%i4) P120 : [[1,1,8],[1,5,5],[1,9,2],[2,3,3],[2,5,4],
 [2,7,8],[3,3,4],[3,4,8],[3,6,7],[3,7,3],[4,1,5],[4,5,9],
 [4,9,8],[5,2,6],[5,8,9],[6,1,1],[6,5,2],[6,9,5],[7,3,1],
 [7,4,6],[7,6,9],[7,7,5],[8,3,5],[8,5,8],[8,7,9],[9,1,3],
[9,5,1],[9,9,4]]$
(%i5) output_sdk_puzzle(3,P120,"Problem 120 in [extreme Sudoku, Gould].","SP100.cnf");

(%i6) Sudogen0_707 : [
 [1,1,4],[1,4,3],[1,7,6],[1,9,8],
 [2,3,5],[2,6,1],[2,8,3],
 [3,2,1],[3,3,6],[3,5,4],[3,9,7],
 [4,2,7],[4,8,8],
 [5,6,2],
 [6,2,5],[6,3,1],[6,6,6],[6,7,3],[6,8,7],
 [7,3,2],[7,5,9],[7,7,8],
 [8,4,1],
 [9,6,4],[9,9,9]]$
(%i7) output_sdk_puzzle(3,Sudogen0_707,"Problem Sudogen0-707 in [The Hidden Logic of Sudoku, Berthier].","S0_707.cnf");

(%i8) EasterMonster;
  [[1,1,1],[1,9,2],[2,2,9],[2,4,4],[2,8,5],[3,3,6],[3,7,7],
   [4,2,5],[4,4,9],[4,6,3],[5,5,7],[6,4,8],[6,5,5],[6,8,4],
   [7,1,7],[7,7,6],[8,2,3],[8,6,9],[8,8,8],[9,3,2],[9,9,1]]
(%i9) output_sdk_EasterMonster();
   \endverbatim


   <h2> Solving problem instances </h2>

   Via the OKsolver-2002:
   \verbatim
> OKsolver_2002-O3-DNDEBUG SP100.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             10554
c initial_number_of_literal_occurrences 23352
c number_of_initial_unit-eliminations   577
c reddiff_maximal_clause_length         1
c reddiff_number_of_variables           577
c reddiff_number_of_clauses             9699
c reddiff_number_of_literal_occurrences 21410
c number_of_2-clauses_after_reduction   720
c running_time(sec)                     0.0
c number_of_nodes                       1
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        0
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             SP100.cnf
s SATISFIABLE

> OKsolver_2002-O3-DNDEBUG SP120.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             10558
c initial_number_of_literal_occurrences 23356
c number_of_initial_unit-eliminations   547
c reddiff_maximal_clause_length         2
c reddiff_number_of_variables           547
c reddiff_number_of_clauses             9432
c reddiff_number_of_literal_occurrences 20792
c number_of_2-clauses_after_reduction   976
c running_time(sec)                     0.0
c number_of_nodes                       1
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                5
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        0
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             SP120.cnf

> OKsolver_2002-O3-DNDEBUG S0_707.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             10555
c initial_number_of_literal_occurrences 23353
c number_of_initial_unit-eliminations   537
c reddiff_maximal_clause_length         2
c reddiff_number_of_variables           537
c reddiff_number_of_clauses             9368
c reddiff_number_of_literal_occurrences 20635
c number_of_2-clauses_after_reduction   1013
c running_time(sec)                     0.0
c number_of_nodes                       1
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                22
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        0
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             S0_707.cnf

> OKsolver_2002-O3-DNDEBUG S_EasterMonster.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             10551
c initial_number_of_literal_occurrences 23349
c number_of_initial_unit-eliminations   490
c reddiff_maximal_clause_length         3
c reddiff_number_of_variables           490
c reddiff_number_of_clauses             8984
c reddiff_number_of_literal_occurrences 19739
c number_of_2-clauses_after_reduction   1343
c running_time(sec)                     0.0
c number_of_nodes                       9
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                100
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        3
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             S_EasterMonster.cnf

> OKsolver_2002-O3-DNDEBUG -O -F S_EasterMonster.cnf

A : rest(rest(read_nested_list("S_EasterMonster.cnf.pa")[1]),-1)$
B : sublist(A,lambda([x],is(x>0)));
length(B);
  81
output_fcs("", cs2fcs({setify(-B)}), "Sol_EasterMonster");

> vi Sol_EasterMonster # removal of non-clausal lines
> cp S_EasterMonster.cnf S_EasterMonster_Neg.cnf
> cat Sol_EasterMonster >> S_EasterMonster_Neg.cnf

>  OKsolver_2002-O3-DNDEBUG S_EasterMonster_Neg.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         81
c initial_number_of_variables           729
c initial_number_of_clauses             10552
c initial_number_of_literal_occurrences 23430
c number_of_initial_unit-eliminations   490
c reddiff_maximal_clause_length         21
c reddiff_number_of_variables           490
c reddiff_number_of_clauses             8984
c reddiff_number_of_literal_occurrences 19760
c number_of_2-clauses_after_reduction   1343
c running_time(sec)                     0.0
c number_of_nodes                       9
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                100
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        3
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             S_EasterMonster_Neg.cnf
   \endverbatim
 
  Remark: Using the number of 2-reductions as a measure of hardness is
  fragile, since it depends on the branchings used; however perhaps it can
  serve as some hardness-indication (especially if only one node has been
  used).

*/

