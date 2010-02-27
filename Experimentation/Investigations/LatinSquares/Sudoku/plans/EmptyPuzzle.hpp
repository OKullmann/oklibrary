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

Aborted after 50m (1 GB memory usage as reached).

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
   <li> satz215:
   \verbatim
> satz215 Weak_Sudoku_Box_dim_3.cnf
**** The instance is satisfiable. *****
NB_MONO= 89, NB_UNIT= 553, NB_BRANCHE= 41, NB_BACK= 0
Program terminated in 0.000 seconds.
satz215 Weak_Sudoku_Box_dim_3.cnf 0.000 41 0 24973 1 1 729 8829 -1458 0 0

> satz215 Dual_Weak_Sudoku_Box_dim_3.cnf

Aborted after 50m.

> satz215 Strong_Sudoku_Box_dim_4.cnf
**** The instance is satisfiable. *****
NB_MONO= 0, NB_UNIT= 622, NB_BRANCHE= 39, NB_BACK= 0
Program terminated in 0.000 seconds.
satz215 Strong_Sudoku_Box_dim_3.cnf 0.000 39 0 20451 27 1 729 11988 -1458 0 0
   \endverbatim
   </li>
   <li> minisat2:
   \verbatim
> minisat2 Weak_Sudoku_Box_dim_3.cnf
restarts              : 1
conflicts             : 10             (256 /sec)
decisions             : 155            (0.00 % random) (3975 /sec)
propagations          : 852            (21850 /sec)
conflict literals     : 308            (0.00 % deleted)
Memory used           : 2.47 MB
CPU time              : 0.038994 s

> minisat2 Dual_Weak_Sudoku_Box_dim_3.cnf
restarts              : 12
conflicts             : 23417          (36032 /sec)
decisions             : 44398          (1.89 % random) (68315 /sec)
propagations          : 468695         (721179 /sec)
conflict literals     : 972923         (2.84 % deleted)
Memory used           : 2.86 MB
CPU time              : 0.649901 s

> minisat2 Strong_Sudoku_Box_dim_3.cnf
restarts              : 1
conflicts             : 2              (57 /sec)
decisions             : 120            (0.00 % random) (3429 /sec)
propagations          : 780            (22290 /sec)
conflict literals     : 36             (0.00 % deleted)
Memory used           : 2.47 MB
CPU time              : 0.034994 s
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
   <li> OKsolver_2002 with minisat2-preprocessing:
   \verbatim
> OKsolver_2002-m2pp Weak_Sudoku_Box_dim_4.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         30
c initial_number_of_variables           3840
c initial_number_of_clauses             79872
c initial_number_of_literal_occurrences 299520
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   74880
c running_time(sec)                     97.1
c number_of_nodes                       143
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                152
c number_of_pure_literals               423
c number_of_autarkies                   1
c number_of_missed_single_nodes         0
c max_tree_depth                        142
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Weak_Sudoku_Box_dim_4.cnf_m2pp_30039

> OKsolver_2002-m2pp Strong_Sudoku_Box_dim_4.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           4096
c initial_number_of_clauses             111616
c initial_number_of_literal_occurrences 237568
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   110592
c running_time(sec)                     136.5
c number_of_nodes                       151
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                133
c number_of_pure_literals               0
c number_of_autarkies                   1
c number_of_missed_single_nodes         0
c max_tree_depth                        150
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Strong_Sudoku_Box_dim_4.cnf_m2pp_30100
   \endverbatim
   </li>
   It seems the preprocessing made the problems slightly harder. </li>
   <li> march_pl: none of the three problems is solved within an hour. </li>
   <li> satz215:
   \verbatim
**** The instance is satisfiable. *****
NB_MONO= 609, NB_UNIT= 4611, NB_BRANCHE= 209, NB_BACK= 27
Program terminated in 0.000 seconds.
satz215 Weak_Sudoku_Box_dim_4.cnf 0.000 209 27 567647 81 1 4096 92416 -12288 0 0

> satz215 Strong_Sudoku_Box_dim_4.cnf
**** The instance is satisfiable. *****
NB_MONO= 0, NB_UNIT= 3635, NB_BRANCHE= 157, NB_BACK= 0
Program terminated in 0.001 seconds.
satz215 Strong_Sudoku_Box_dim_4.cnf 0.001 157 0 446914 102 1 4096 123904 -12288 0 0
   \endverbatim
   So satz215 seems the strong look-ahead solver. </li>
   <li> minisat2:
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
   <li> Trying picosat913 for the dual weak translation: aborted after 50m.
   </li>
  </ul>


  \todo Box-dimension p=5
  <ul>
   <li> OKsolver_2002:
   \verbatim
> OKsolver_2002-O3-DNDEBUG Weak_Sudoku_Box_dim_5.cnf

> OKsolver_2002-O3-DNDEBUG Strong_Sudoku_Box_dim_5.cnf

   \endverbatim
   </li>
   <li> satz215:
   \verbatim
> satz215 Weak_Sudoku_Box_dim_5.cnf

aborted after 40m; needs to be re-run via
"time satz215 Weak_Sudoku_Box_dim_5.cnf"

> satz215 Strong_Sudoku_Box_dim_5.cnf

   \endverbatim
   </li>
   <li> minisat2:
   \verbatim
> minisat2 Weak_Sudoku_Box_dim_5.cnf
restarts              : 10
conflicts             : 8033           (294 /sec)
decisions             : 51245          (1.44 % random) (1873 /sec)
propagations          : 887380         (32441 /sec)
conflict literals     : 3804297        (0.45 % deleted)
Memory used           : 48.13 MB
CPU time              : 27.3538 s

> minisat2 Strong_Sudoku_Box_dim_5.cnf
restarts              : 4
conflicts             : 793            (136 /sec)
decisions             : 12790          (1.59 % random) (2195 /sec)
propagations          : 100446         (17238 /sec)
conflict literals     : 76103          (0.32 % deleted)
Memory used           : 45.02 MB
CPU time              : 5.82711 s
   \endverbatim
   </li>
   <li> picosat913:
   \verbatim
> picosat913 Weak_Sudoku_Box_dim_5.cnf
c 0 iterations
c 18 restarts
c 0 failed literals
c 3689 conflicts
c 45896 decisions
c 0 fixed variables
c 486772 learned literals
c 0.8% deleted literals
c 1582762 propagations
c 43.0% variables used
c 1.7 seconds in library
c 0.9 megaprops/second
c 1 simplifications
c 12 reductions
c 1.7 MB recycled
c 9.1 MB maximally allocated
c 1.7 seconds total run time

> picosat913 Strong_Sudoku_Box_dim_5.cnf
c 0 iterations
c 2 restarts
c 0 failed literals
c 210 conflicts
c 5488 decisions
c 0 fixed variables
c 16105 learned literals
c 0.1% deleted literals
c 1445517 propagations
c 11.7% variables used
c 0.7 seconds in library
c 2.0 megaprops/second
c 1 simplifications
c 0 reductions
c 0.0 MB recycled
c 9.0 MB maximally allocated
c 0.7 seconds total run time
   \endverbatim
   </li>
  </ul>

*/


