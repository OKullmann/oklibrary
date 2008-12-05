// Oliver Kullmann, 1.11.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
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
(%i2) S3 : strong_sdk(3)$
(%i3) P100 : map(set,{sdk(1,3,5),sdk(1,7,1),sdk(1,9,8),sdk(2,4,5),sdk(2,8,3),
 sdk(3,5,8),sdk(3,6,9),sdk(3,7,4),sdk(4,3,9),sdk(4,5,1),sdk(4,6,4),sdk(4,9,2),
 sdk(6,1,2),sdk(6,4,3),sdk(6,5,6),sdk(6,7,7),sdk(7,3,7),sdk(7,4,2),sdk(7,5,9),
 sdk(8,2,8),sdk(8,6,5),sdk(9,1,3),sdk(9,3,4),sdk(9,7,6)});
(%i4) SP100 : [S3[1],union(S3[2],P100)]$
(%i5) sSP100 : standardise_fcs(SP100)$
(%i6) output_fcs_v(
 sconcat("The Sudoku problem (strong form) with box-dimension ", 3, ", with initial constraints from Problem 100 in [extreme Sudoku, Gould]."),
 sSP100[1],"SP100.cnf",sSP100[2]);

(%i7) P120 : map(set,{sdk(1,1,8),sdk(1,5,5),sdk(1,9,2),sdk(2,3,3),sdk(2,5,4),
 sdk(2,7,8),sdk(3,3,4),sdk(3,4,8),sdk(3,6,7),sdk(3,7,3),sdk(4,1,5),sdk(4,5,9),
 sdk(4,9,8),sdk(5,2,6),sdk(5,8,9),sdk(6,1,1),sdk(6,5,2),sdk(6,9,5),sdk(7,3,1),
 sdk(7,4,6),sdk(7,6,9),sdk(7,7,5),sdk(8,3,5),sdk(8,5,8),sdk(8,7,9),sdk(9,1,3),
sdk(9,5,1),sdk(9,9,4)});
(%i8) SP120 : [S3[1],union(S3[2],P120)]$
(%i9) fcs_p(SP120);
Evaluation took 165.0739 seconds (189.7842 elapsed) using 438.141 MB.
(%o9) true
(%i10) sSP120 : standardise_fcs(SP120)$
(%i11) output_fcs_v(
 sconcat("The Sudoku problem (strong form) with box-dimension ", 3, ", with initial constraints from Problem 120 in [extreme Sudoku, Gould]."),
 sSP120[1],"SP120.cnf",sSP120[2]);
   \endverbatim


   <h2> Solving problem instances </h2>

   Via the C-OKsolver-2002:
   \verbatim
> OKsolver_2002-O3-DNDEBUG SP100.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=729 initial_number_of_clauses=10554 initial_number_of_literal_occurrences=23352 running_time(s)=0.0 number_of_nodes=1
  number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=1 number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=0
   number_of_table_enlargements=0 reduced_maximal_clause_length=1 reduced_number_of_variables=577 reduced_number_of_clauses=9699 reduced_number_of_literal_occurrences=21410 
  number_of_1-autarkies=0 number_of_initial_unit-eliminations=577 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=720 file_name=SP100.cnf

> OKsolver_2002-O3-DNDEBUG SP120.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=729 initial_number_of_clauses=10558 initial_number_of_literal_occurrences=23356 running_time(s)=0.0 number_of_nodes=1 
  number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=5 number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=0
  number_of_table_enlargements=0 reduced_maximal_clause_length=2 reduced_number_of_variables=547 reduced_number_of_clauses=9432 reduced_number_of_literal_occurrences=20792 
  number_of_1-autarkies=0 number_of_initial_unit-eliminations=547 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=976 file_name=SP120.cnf
   \endverbatim

*/

