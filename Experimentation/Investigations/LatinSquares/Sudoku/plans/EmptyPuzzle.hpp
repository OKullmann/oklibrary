// Oliver Kullmann, 27.2.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/LatinSquares/Sudoku/plans/EmptyPuzzle.hpp
  \brief On investigations regarding "empty" Sudoku problems (just finding a solution)


  \todo Box-dimension p=3
  <ul>
   <li> OKsolver_2002:
   \verbatim
> OKsolver_2002-O3-DNDEBUG Weak_Sudoku_Box_dim_3.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             8829
c initial_number_of_literal_occurrences 18225
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   8748
c running_time(sec)                     0.1
c number_of_nodes                       39
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                119
c number_of_pure_literals               68
c number_of_autarkies                   3
c number_of_missed_single_nodes         0
c max_tree_depth                        38
c number_of_table_enlargements          0
c number_of_1-autarkies                 12965
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Weak_Sudoku_Box_dim_3.cnf

> OKsolver_2002-O3-DNDEBUG Dual_Weak_Sudoku_Box_dim_3.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             3159
c initial_number_of_literal_occurrences 8019
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   2916
c running_time(sec)                     0.1
c number_of_nodes                       122
c number_of_single_nodes                13
c number_of_quasi_single_nodes          0
c number_of_2-reductions                671
c number_of_pure_literals               39
c number_of_autarkies                   10
c number_of_missed_single_nodes         6
c max_tree_depth                        50
c number_of_table_enlargements          0
c number_of_1-autarkies                 1974
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Dual_Weak_Sudoku_Box_dim_3.cnf

> OKsolver_2002-O3-DNDEBUG Strong_Sudoku_Box_dim_3.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             11988
c initial_number_of_literal_occurrences 26244
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   11664
c running_time(sec)                     0.3
c number_of_nodes                       36
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                25
c number_of_pure_literals               0
c number_of_autarkies                   2
c number_of_missed_single_nodes         0
c max_tree_depth                        35
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Strong_Sudoku_Box_dim_3.cnf
   \endverbatim
   So with weak and strong form no backtracking is needed, but it is needed
   with the dual weak form.
   </li>
   <li> march_pl:
   \verbatim
> march_pl Weak_Sudoku_Box_dim_3.cnf
c main():: nodeCount: 4104
c main():: dead ends in main: 1370
c main():: lookAheadCount: 96244
c main():: unitResolveCount: 68904
c main():: time=0.830000
c main():: necessary_assignments: 4418
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 2839, succes #: 676
c main():: doublelook: overall 3.037 of all possible doublelooks executed
c main():: doublelook: succesrate: 23.811, average DL_trigger: 4.051
c main():: SOLUTION VERIFIED :-)
s SATISFIABLE

> march_pl Dual_Weak_Sudoku_Box_dim_3.cnf

> march_pl Strong_Sudoku_Box_dim_3.cnf
c main():: nodeCount: 143
c main():: dead ends in main: 0
c main():: lookAheadCount: 25395
c main():: unitResolveCount: 729
c main():: time=1.200000
c main():: necessary_assignments: 24
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 1539, succes #: 0
c main():: doublelook: overall 6.062 of all possible doublelooks executed
c main():: doublelook: succesrate: 0.000, average DL_trigger: 6.053
c main():: SOLUTION VERIFIED :-)
s SATISFIABLE
   \endverbatim
   <li> p=3, satz215:
   \verbatim
> satz215 Weak_Sudoku_Box_dim_3.cnf
**** The instance is satisfiable. *****
NB_MONO= 89, NB_UNIT= 553, NB_BRANCHE= 41, NB_BACK= 0
Program terminated in 0.000 seconds.
satz215 Weak_Sudoku_Box_dim_3.cnf 0.000 41 0 24973 1 1 729 8829 -1458 0 0

> satz215 Dual_Weak_Sudoku_Box_dim_3.cnf

   \endverbatim
   </li>
  </ul>


  \todo Box-dimension p=4
  <ul>
   <li> OKsolver_2002:
   \verbatim
> OKsolver_2002-O3-DNDEBUG Weak_Sudoku_Box_dim_4.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           4096
c initial_number_of_clauses             92416
c initial_number_of_literal_occurrences 188416
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   92160
c running_time(sec)                     32.7
c number_of_nodes                       139
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                319
c number_of_pure_literals               469
c number_of_autarkies                   3
c number_of_missed_single_nodes         0
c max_tree_depth                        137
c number_of_table_enlargements          0
c number_of_1-autarkies                 262599
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Weak_Sudoku_Box_dim_4.cnf

> OKsolver_2002-O3-DNDEBUG Dual_Weak_Sudoku_Box_dim_4.cnf
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         16
c initial_number_of_variables           4096
c initial_number_of_clauses             31488
c initial_number_of_literal_occurrences 73728
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   30720
c running_time(sec)                     9121.6
c number_of_nodes                       2170917
c number_of_single_nodes                9990
c number_of_quasi_single_nodes          0
c number_of_2-reductions                15034724
c number_of_pure_literals               2225993
c number_of_autarkies                   11547
c number_of_missed_single_nodes         5343
c max_tree_depth                        189
c number_of_table_enlargements          0
c number_of_1-autarkies                 208610462
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Dual_Weak_Sudoku_Box_dim_4.cnf

> OKsolver_2002-O3-DNDEBUG Strong_Sudoku_Box_dim_4.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           4096
c initial_number_of_clauses             123904
c initial_number_of_literal_occurrences 262144
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   122880
c running_time(sec)                     108.9
c number_of_nodes                       144
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                117
c number_of_pure_literals               0
c number_of_autarkies                   2
c number_of_missed_single_nodes         0
c max_tree_depth                        143
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Strong_Sudoku_Box_dim_4.cnf
   \endverbatim
   So for the weak form one backtrack was needed, none for the strong
   form, while the dual weak form gets very hard. </li>
   <li> p=4, march_pl: none of the problems is solved within an hour. </li>
   <li> p=4, minisat2:
   \verbatim
> minisat2 Weak_Sudoku_Box_dim_4.cnf
restarts              : 1
conflicts             : 0              (0 /sec)
decisions             : 678            (1.33 % random) (478 /sec)
propagations          : 3840           (2707 /sec)
conflict literals     : 0              ( nan % deleted)
Memory used           : 9.52 MB
CPU time              : 1.41878 s

> minisat2 Dual_Weak_Sudoku_Box_dim_4.cnf
*** INTERRUPTED ***
restarts              : 25
conflicts             : 4412395        (483 /sec)
decisions             : 5597098        (1.91 % random) (613 /sec)
propagations          : 101006508      (11062 /sec)
conflict literals     : 786048341      (2.91 % deleted)
Memory used           : 239.70 MB
CPU time              : 9131.2 s

> minisat2 Strong_Sudoku_Box_dim_4.cnf
restarts              : 1
conflicts             : 0              (0 /sec)
decisions             : 677            (1.03 % random) (799 /sec)
propagations          : 4096           (4837 /sec)
conflict literals     : 0              ( nan % deleted)
Memory used           : 7.06 MB
CPU time              : 0.846871 s
   \endverbatim
   </li>
   <li> So at least for complete solvers and the empty board the dual weak
   translation makes the problem very hard. </li>
  </ul>

*/


