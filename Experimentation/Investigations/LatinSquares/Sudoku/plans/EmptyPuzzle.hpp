// Oliver Kullmann, 27.2.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/LatinSquares/Sudoku/plans/EmptyPuzzle.hpp
  \brief On investigations regarding "empty" Sudoku problems (just finding a solution)


  \todo Box-dimension p=3 with direct encoding and three standard translations
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

real    0m0.171s
user    0m0.063s
sys     0m0.010s

> satz215 Dual_Weak_Sudoku_Box_dim_3.cnf

Aborted after 50m.

> satz215 Strong_Sudoku_Box_dim_3.cnf
**** The instance is satisfiable. *****
NB_MONO= 0, NB_UNIT= 622, NB_BRANCHE= 39, NB_BACK= 0
Program terminated in 0.000 seconds.
satz215 Strong_Sudoku_Box_dim_3.cnf 0.000 39 0 20451 27 1 729 11988 -1458 0 0

real    0m0.266s
user    0m0.112s
sys     0m0.018s
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


  \todo Box-dimension p=4 with direct encoding and three standard translations
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
> satz215 Weak_Sudoku_Box_dim_4.cnf
**** The instance is satisfiable. *****
NB_MONO= 609, NB_UNIT= 4611, NB_BRANCHE= 209, NB_BACK= 27
Program terminated in 0.000 seconds.
satz215 Weak_Sudoku_Box_dim_4.cnf 0.000 209 27 567647 81 1 4096 92416 -12288 0 0

real    0m10.107s
user    0m4.203s
sys     0m0.046s

> satz215 Strong_Sudoku_Box_dim_4.cnf
**** The instance is satisfiable. *****
NB_MONO= 0, NB_UNIT= 3635, NB_BRANCHE= 157, NB_BACK= 0
Program terminated in 0.001 seconds.
satz215 Strong_Sudoku_Box_dim_4.cnf 0.001 157 0 446914 102 1 4096 123904 -12288 0 0

real    0m6.047s
user    0m2.837s
sys     0m0.012s
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


  \todo Box-dimension p=5 with direct encoding and three standard translations
  <ul>
   <li> OKsolver_2002:
   \verbatim
> OKsolver_2002-O3-DNDEBUG Weak_Sudoku_Box_dim_5.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         25
c initial_number_of_variables           15625
c initial_number_of_clauses             563125
c initial_number_of_literal_occurrences 1140625
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   562500
c running_time(sec)                     487.5
c number_of_nodes                       420
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                1013
c number_of_pure_literals               1862
c number_of_autarkies                   2
c number_of_missed_single_nodes         0
c max_tree_depth                        400
c number_of_table_enlargements          0
c number_of_1-autarkies                 2713283
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Weak_Sudoku_Box_dim_5.cnf

> OKsolver_2002-O3-DNDEBUG Strong_Sudoku_Box_dim_5.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         25
c initial_number_of_variables           15625
c initial_number_of_clauses             752500
c initial_number_of_literal_occurrences 1562500
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   750000
c running_time(sec)                     2927.2
c number_of_nodes                       398
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                317
c number_of_pure_literals               0
c number_of_autarkies                   2
c number_of_missed_single_nodes         0
c max_tree_depth                        397
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Strong_Sudoku_Box_dim_5.cnf
   \endverbatim
   So for the strong form now backtracking occurred, however the overhead
   of the rather powerless r_2-reduction is considerable. </li>
   <li> satz215:
   \verbatim
> satz215 Weak_Sudoku_Box_dim_5.cnf

aborted after 40m; needs to be re-run via
"time satz215 Weak_Sudoku_Box_dim_5.cnf"

> satz215 Strong_Sudoku_Box_dim_5.cnf
**** The instance is satisfiable. *****
NB_MONO= 0, NB_UNIT= 14040, NB_BRANCHE= 412, NB_BACK= 0
Program terminated in 0.010 seconds.
satz215 Strong_Sudoku_Box_dim_5.cnf 0.010 412 0 4608214 625 1 15625 752500 -62500 0 0

real    3m38.463s
user    1m44.504s
sys     0m0.094s
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


  \todo Box-dimension p=6 with direct encoding and three standard translations
  <ul>
   <li> satz215 aborts with a memory-error. </li>
   <li> OKsolver_2002:
   \verbatim
> OKsolver_2002-O3-DNDEBUG Weak_Sudoku_Box_dim_6.cnf
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         36
c initial_number_of_variables           46656
c initial_number_of_clauses             2450736
c initial_number_of_literal_occurrences 4945536
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   2449440
c running_time(sec)                     8409.0
c number_of_nodes                       411
c number_of_single_nodes                1
c number_of_quasi_single_nodes          0
c number_of_2-reductions                4360
c number_of_pure_literals               1917
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        175
c number_of_table_enlargements          0
c number_of_1-autarkies                 10884262
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Weak_Sudoku_Box_dim_6.cnf

(a previous run was aborted (without result) after 5688 nodes)

> OKsolver_2002-O3-DNDEBUG Strong_Sudoku_Box_dim_6.cnf
XXX
(a previous run showed that the problem was solved in 911 nodes)
   \endverbatim
   The major problem seems the high overhead of the r_2-reduction, which here
   is mostly powerless. </li>
   <li> picosat913
   \verbatim
> picosat913 Weak_Sudoku_Box_dim_6.cnf
c 0 iterations
c 254 restarts
c 0 failed literals
c 91856 conflicts
c 1370001 decisions
c 0 fixed variables
c 20133966 learned literals
c 0.7% deleted literals
c 22630393 propagations
c 61.0% variables used
c 51.3 seconds in library
c 0.4 megaprops/second
c 1 simplifications
c 87 reductions
c 76.6 MB recycled
c 30.3 MB maximally allocated
c 51.3 seconds total run time

> picosat913 Strong_Sudoku_Box_dim_6.cnf
c 0 iterations
c 9 restarts
c 0 failed literals
c 1530 conflicts
c 36274 decisions
c 0 fixed variables
c 263451 learned literals
c 0.3% deleted literals
c 6599783 propagations
c 11.4% variables used
c 3.4 seconds in library
c 1.9 megaprops/second
c 1 simplifications
c 0 reductions
c 0.0 MB recycled
c 50.5 MB maximally allocated
c 3.4 seconds total run time
   \endverbatim
   </li>
   <li> precosat236:
   \verbatim
> precosat236 Weak_Sudoku_Box_dim_6.cnf
c *** CAUGHT SIGNAL 2 ***
c
c 322129 conflicts, 1432262 decisions, 698 random
c 0 iterations, 765 restarts, 0 skipped
c 8 enlarged, 396 shrunken, 3145 rescored, 121 rebiased
c 1 simplifications, 0 reductions
c
c vars: 0 fixed, 0 equiv, 1296 elim, 0 merged
c elim: 729757 resolutions, 1 phases, 0 rounds
c sbst: 0% substituted, 0.0% nots, 0.0% ands, 0.0% xors, 0.0% ites
c arty: 0.00 and 0.00 xor average arity
c prbe: 206414 probed, 68 phases, 2 rounds
c prbe: 0 failed, 0 lifted, 0 merged
c sccs: 0 non trivial, 0 fixed, 0 merged
c hash: 0 units, 0 merged
c mins: 99988984 learned, 1% deleted, 0 strong, 11 depth
c subs: 5544 forward, 670 backward, 0 doms
c strs: 0 forward, 0 backward
c doms: 0 dominators, 0 high, 0 low
c prps: 78248397 propagations, 0.18 megaprops
c
c 434.7 seconds, 402 MB max, 228 MB recycled
c
c *** CAUGHT SIGNAL 2 ***
c

> precosat236 Strong_Sudoku_Box_dim_6.cnf
c 4969 conflicts, 90054 decisions, 62 random
c 0 iterations, 26 restarts, 0 skipped
c 0 enlarged, 192 shrunken, 48 rescored, 3 rebiased
c 1 simplifications, 0 reductions
c
c vars: 0 fixed, 0 equiv, 0 elim, 0 merged
c elim: 600061 resolutions, 1 phases, 0 rounds
c sbst: 0% substituted, 0.0% nots, 0.0% ands, 0.0% xors, 0.0% ites
c arty: 0.00 and 0.00 xor average arity
c prbe: 12124 probed, 2 phases, 0 rounds
c prbe: 0 failed, 0 lifted, 0 merged
c sccs: 0 non trivial, 0 fixed, 0 merged
c hash: 0 units, 0 merged
c mins: 893366 learned, 0% deleted, 0 strong, 4 depth
c subs: 933 forward, 192 backward, 0 doms
c strs: 0 forward, 0 backward
c doms: 0 dominators, 0 high, 0 low
c prps: 2137912 propagations, 0.03 megaprops
c
c 67.6 seconds, 294 MB max, 0 MB recycled
   \endverbatim
   So picosat913 seems stronger. </li>
  </ul>


  \todo Direct encoding, weak-pl translation
  <ul>
   <li> Created by output_weakpl_sdk_stdname(p). </li>
   <li> p=3
    <ol>
     <li> OKsolver_2002
     \verbatim
> OKsolver_2002-O3-DNDEBUG WeakPl_Sudoku_Box_dim_3.cnf
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             9072
c initial_number_of_literal_occurrences 20412
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   8748
c running_time(sec)                     40.3
c number_of_nodes                       15054
c number_of_single_nodes                36
c number_of_quasi_single_nodes          0
c number_of_2-reductions                117883
c number_of_pure_literals               0
c number_of_autarkies                   2
c number_of_missed_single_nodes         13
c max_tree_depth                        42
c number_of_table_enlargements          0
c number_of_1-autarkies                 40448
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             WeakPl_Sudoku_Box_dim_3.cnf
     \endverbatim
     </li>
    </ol>
   </li>
   <li> p=4
    <ol>
     <li> For the OKsolver_2002 definitely worse than the weak translation:
     \verbatim
> OKsolver_2002-O3-DNDEBUG WeakPl_Sudoku_Box_dim_4.cnf
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         16
c initial_number_of_variables           4096
c initial_number_of_clauses             93184
c initial_number_of_literal_occurrences 200704
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   92160
c running_time(sec)                     802.6
c number_of_nodes                       3857
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                50571
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        93
c number_of_table_enlargements          0
c number_of_1-autarkies                 30610
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             WeakPl_Sudoku_Box_dim_4.cnf
     \endverbatim
     </li>
     <li> satz215 again worse than the weak translation: aborted after 1m50s.
     </li>
     <li> minisat2:
     \verbatim
> minisat2 WeakPl_Sudoku_Box_dim_4.cnf
restarts              : 1
conflicts             : 0              (0 /sec)
decisions             : 677            (1.03 % random) (1125 /sec)
propagations          : 4096           (6805 /sec)
conflict literals     : 0              ( nan % deleted)
Memory used           : 6.94 MB
CPU time              : 0.601908 s
     \endverbatim
     This looks very similar to the strong form. </li>
     <li> picosat913
     \verbatim
> picosat913 WeakPl_Sudoku_Box_dim_4.cnf
c 0 iterations
c 0 restarts
c 0 failed literals
c 0 conflicts
c 672 decisions
c 0 fixed variables
c 0 learned literals
c 0.0% deleted literals
c 167936 propagations
c 0.0% variables used
c 0.1 seconds in library
c 1.9 megaprops/second
c 1 simplifications
c 0 reductions
c 0.0 MB recycled
c 1.4 MB maximally allocated
c 0.1 seconds total run time
     \endverbatim
     </li>
     <li> From the ubcsat-suite, sapsnr with cutoff=20000 has in 10 runs
     100% success, while adaptnovelty+ with cutoff=1000000 has in 10 runs
     10% success. </li>
    </ol>
   </li>
   <li> p=5
    <ol>
     <li> minisat2:
     \verbatim
restarts              : 8
conflicts             : 4849           (1265 /sec)
decisions             : 50625          (1.35 % random) (13210 /sec)
propagations          : 557895         (145573 /sec)
conflict literals     : 728520         (0.54 % deleted)
Memory used           : 30.72 MB
CPU time              : 3.83242 s
     \endverbatim
     More conflicts than with the strong form, but faster due to the smaller
     clause-set. </li>
     <li> picosat913:
     \verbatim
c 0 iterations
c 16 restarts
c 0 failed literals
c 3339 conflicts
c 33798 decisions
c 0 fixed variables
c 373038 learned literals
c 0.8% deleted literals
c 1500346 propagations
c 43.2% variables used
c 1.2 seconds in library
c 1.3 megaprops/second
c 1 simplifications
c 1 reductions
c 0.4 MB recycled
c 10.0 MB maximally allocated
c 1.2 seconds total run time
     \endverbatim
     This looks a bit weaker than with the strong form. </li>
     <li> From the ubcsat-suite, sapsnr with cutoff=1000000 seems very weak,
     and the same for adaptnovelty+. We need to determine the best algorithm
     from the ubcsat-suite. </li>
     <li> Best Ubcsat-solver:
     \verbatim
> E = eval_ubcsat("WeakPl_Sudoku_Box_dim_5.cnf", runs=100, cutoff=100000)
> plot(E$alg,E$best)
> eval_ubcsat_dataframe(E)
rsaps :
 2  4  6  7  8 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
 1  1  1  1  3  1  6  2  4  6  4  4  4  9  9  6  4  1  1  3  2  8  2  4  2  4
31 33 34 35 40 43
 2  1  1  1  1  1
rnoveltyp :
 4  6  7  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
 2  2  1  8  2  5  6 10  5  7  8  7  8 10  6  2  5  3  2  1
rnovelty :
 5  6  7  8  9 10 12 13 14 15 16 17 18 19 20 21 23 24 25
 1  1  3  5  4  2  8  6  7 10 10 16 10  5  2  7  1  1  1
     \endverbatim
     </li>
     <li> rsaps with cutoff=10^6 in 10 runs has 90% success. </li>
    </ol>
   </li>
   <li> p=6
    <ol>
     <li> minisat2:
     \verbatim
> minisat2 WeakPl_Sudoku_Box_dim_6.cnf
*** INTERRUPTED ***
restarts              : 18
conflicts             : 248627         (563 /sec)
decisions             : 537432         (1.70 % random) (1216 /sec)
propagations          : 20995970       (47520 /sec)
conflict literals     : 70230664       (1.51 % deleted)
Memory used           : 396.64 MB
CPU time              : 441.831 s
     \endverbatim
     </li>
     <li> picosat913:
     \verbatim
> picosat913 WeakPl_Sudoku_Box_dim_6.cnf
c 0 iterations
c 181 restarts
c 0 failed literals
c 57449 conflicts
c 740981 decisions
c 0 fixed variables
c 10503622 learned literals
c 1.0% deleted literals
c 17097396 propagations
c 51.9% variables used
c 20.9 seconds in library
c 0.8 megaprops/second
c 1 simplifications
c 13 reductions
c 34.6 MB recycled
c 34.1 MB maximally allocated
c 20.9 seconds total run time
     \endverbatim
     </li>
     <li> rsaps with cutoff=10^6 reached in 10 runs only min=48. </li>
    </ol>
   </li>
   <li> So altogether it seems that this translation is worse than the
   strong translation. </li>
  </ul>


  \todo Direct encoding, weak-pb translation
  <ul>
   <li> Created by output_weakpb_sdk_stdname(p). </li>
   <li> p=3
    <ol>
     <li> OKsolver_2002
     \verbatim
> OKsolver_2002-O3-DNDEBUG WeakPb_Sudoku_Box_dim_3.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             11745
c initial_number_of_literal_occurrences 24057
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   11664
c running_time(sec)                     0.2
c number_of_nodes                       39
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                92
c number_of_pure_literals               0
c number_of_autarkies                   4
c number_of_missed_single_nodes         0
c max_tree_depth                        38
c number_of_table_enlargements          0
c number_of_1-autarkies                 12966
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             WeakPb_Sudoku_Box_dim_3.cnf
     \endverbatim
     </li>
    </ol>
   </li>
   <li> p=4 
    <ol>
     <li> march_pl:
     \verbatim
> march_pl WeakPb_Sudoku_Box_dim_4.cnf
Aborted after 50m.
     \endverbatim
     </li>
     <li> minisat2:
     \verbatim
> minisat2 WeakPb_Sudoku_Box_dim_4.cnf 
restarts              : 1
conflicts             : 0              (0 /sec)
decisions             : 678            (1.33 % random) (543 /sec)
propagations          : 3840           (3077 /sec)
conflict literals     : 0              ( nan % deleted)
Memory used           : 10.71 MB
CPU time              : 1.24808 s

SATISFIABLE
     \endverbatim
     </li>
     <li> OKsolver_2002 with minisat2-preprocessing:
     \verbatim
> OKsolver_2002-m2pp WeakPb_Sudoku_Box_dim_4.cnf 
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         30
c initial_number_of_variables           3840
c initial_number_of_clauses             106752
c initial_number_of_literal_occurrences 353280
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   101760
c running_time(sec)                     112.1
c number_of_nodes                       146
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                123
c number_of_pure_literals               0
c number_of_autarkies                   6
c number_of_missed_single_nodes         0
c max_tree_depth                        144
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             WeakPb_Sudoku_Box_dim_4.cnf_m2pp_21116
     \endverbatim
     </li>
     <li> OKsolver_2002:
     \verbatim
> OKsolver_2002-O3-DNDEBUG WeakPb_Sudoku_Box_dim_4.cnf 
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           4096
c initial_number_of_clauses             123136
c initial_number_of_literal_occurrences 249856
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   122880
c running_time(sec)                     47.6
c number_of_nodes                       143
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                226
c number_of_pure_literals               0
c number_of_autarkies                   6
c number_of_missed_single_nodes         0
c max_tree_depth                        142
c number_of_table_enlargements          0
c number_of_1-autarkies                 264350
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             WeakPb_Sudoku_Box_dim_4.cnf
     \endverbatim
     </li>
     <li> picosat:
     \verbatim
> picosat913 WeakPb_Sudoku_Box_dim_4.cnf 
s SATISFIABLE

c
c 0 iterations
c 0 restarts
c 0 failed literals
c 0 conflicts
c 672 decisions
c 0 fixed variables
c 0 learned literals
c 0.0% deleted literals
c 229376 propagations
c 0.0% variables used
c 0.1 seconds in library
c 2.3 megaprops/second
c 1 simplifications
c 0 reductions
c 0.0 MB recycled
c 1.3 MB maximally allocated
c 0.1 seconds total run time
     \endverbatim
     </li>
     <li> satz215:
    \verbatim
> satz215 WeakPb_Sudoku_Box_dim_4.cnf 
**** The instance is satisfiable. *****
NB_MONO= 0, NB_UNIT= 4112, NB_BRANCHE= 166, NB_BACK= 6 
Program terminated in 0.000 seconds.
satz215 WeakPb_Sudoku_Box_dim_4.cnf 0.000 166 6 540938 29 1 4096 123136 -12288 0 0

real    0m2.301s
user    0m2.212s
sys     0m0.044s
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Direct encoding, dual-weak-pl translation
  <ul>
   <li> Created by output_dual_weakpl_sdk_stdname(p). </li>
   <li> p=3
    <ol>
     <li> OKsolver_2002:
     \verbatim
> OKsolver_2002-O3-DNDEBUG Dual_WeakPl_Sudoku_Box_dim_3.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             3240
c initial_number_of_literal_occurrences 8748
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   2916
c running_time(sec)                     0.1
c number_of_nodes                       49
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                283
c number_of_pure_literals               0
c number_of_autarkies                   2
c number_of_missed_single_nodes         0
c max_tree_depth                        45
c number_of_table_enlargements          0
c number_of_1-autarkies                 263
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Dual_WeakPl_Sudoku_Box_dim_3.cnf
     \endverbatim
     </li>
    </ol>
   </li>
   <li> p=4 
    <ol>
     <li> OKsolver_2002:
     \verbatim
> OKsolver_2002-O3-DNDEBUG Dual_WeakPl_Sudoku_Box_dim_4.cnf
Aborted after 50m.
     \endverbatim
     </li>
     <li> march_pl:
     \verbatim
> march_pl Dual_WeakPl_Sudoku_Box_dim_4.cnf
Aborted after 50m.
     \endverbatim
     </li>
     <li> OKsolver_2002 with minisat2-preprocessing:
     \verbatim
> OKsolver_2002-m2pp Dual_WeakPl_Sudoku_Box_dim_4.cnf
Aborted after 50m.
     \endverbatim
     </li>
     <li> minisat2:
     \verbatim
> minisat2 Dual_WeakPl_Sudoku_Box_dim_4.cnf
Aborted after 50m.
     \endverbatim
     </li>
     <li> picosat:
     \verbatim
> picosat913 Dual_WeakPl_Sudoku_Box_dim_4.cnf
Aborted after 50m.
     \endverbatim
     </li>
     <li> satz215
     \verbatim
> satz215 Dual_WeakPl_Sudoku_Box_dim_4.cnf
Aborted after 50m.
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>

    
  \todo Direct encoding, dual-weak-pb translation
  <ul>
   <li> Created by output_dual_weakpb_sdk_stdname(p). </li>
   <li> p=3
    <ol>
     <li> OKsolver_2002
     \verbatim
> OKsolver_2002-O3-DNDEBUG Dual_WeakPb_Sudoku_Box_dim_3.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             11907
c initial_number_of_literal_occurrences 25515
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   11664
c running_time(sec)                     0.4
c number_of_nodes                       34
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                23
c number_of_pure_literals               0
c number_of_autarkies                   2
c number_of_missed_single_nodes         0
c max_tree_depth                        33
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Dual_WeakPb_Sudoku_Box_dim_3.cnf
     \endverbatim
     </li>
    </ol>
   </li>
   <li> p=4 
    <ol>
     <li> OKsolver_2002:
     \verbatim
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           4096
c initial_number_of_clauses             123648
c initial_number_of_literal_occurrences 258048
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   122880
c running_time(sec)                     47.8
c number_of_nodes                       142
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                96
c number_of_pure_literals               0
c number_of_autarkies                   1
c number_of_missed_single_nodes         0
c max_tree_depth                        141
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Dual_WeakPb_Sudoku_Box_dim_4.cnf
     \endverbatim
     </li>
     <li> march_pl:
     \verbatim
> march_pl Dual_WeakPb_Sudoku_Box_dim_4.cnf
Aborted after 50m.
     \endverbatim
     </li>
     <li> OKsolver_2002 with minisat2-preprocessing:
     \verbatim
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           4096
c initial_number_of_clauses             111360
c initial_number_of_literal_occurrences 233472
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   110592
c running_time(sec)                     42.6
c number_of_nodes                       144
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                123
c number_of_pure_literals               0
c number_of_autarkies                   4
c number_of_missed_single_nodes         0
c max_tree_depth                        143
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Dual_WeakPb_Sudoku_Box_dim_4.cnf_m2pp_25744
\endverbatim
     </li>
     <li> minisat2:
     \verbatim
> minisat2 Dual_WeakPb_Sudoku_Box_dim_4.cnf
restarts              : 8
conflicts             : 3257           (4112 /sec)
decisions             : 10540          (1.39 % random) (13307 /sec)
propagations          : 194504         (245571 /sec)
conflict literals     : 385829         (0.64 % deleted)
Memory used           : 8.35 MB
CPU time              : 0.792049 s

SATISFIABLE
     \endverbatim
     </li>
     <li> picosat:
     \verbatim
> picosat913 Dual_WeakPb_Sudoku_Box_dim_4.cnf
s SATISFIABLE

c
c 0 iterations
c 12 restarts
c 0 failed literals
c 1885 conflicts
c 8924 decisions
c 0 fixed variables
c 153703 learned literals
c 0.5% deleted literals
c 399484 propagations
c 82.8% variables used
c 0.2 seconds in library
c 1.8 megaprops/second
c 1 simplifications
c 5 reductions
c 0.5 MB recycled
c 1.6 MB maximally allocated
c 0.2 seconds total run time
     \endverbatim
     </li>
     <li> satz215:
     \verbatim
> satz215 Dual_WeakPb_Sudoku_Box_dim_4.cnf 
**** The instance is satisfiable. *****
NB_MONO= 0, NB_UNIT= 3599, NB_BRANCHE= 155, NB_BACK= 0 
Program terminated in 0.000 seconds.
satz215 Dual_WeakPb_Sudoku_Box_dim_4.cnf 0.000 155 0 445398 121 1 4096 123648 -12288 0 0

real    0m2.794s
user    0m2.668s
sys     0m0.032s
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Summary
  <ul>
   <li> First reporting on the performance of the 7 forms (weak, dualweak,
   strong, weakpl, weakpb, dualweakpl, dualweakpb) of the direct translation.
   </li>
   <li> OKsolver:
    <ol>
     <li> p = 3: weakpl is worst (not solved yet), the rest is roughly
     comparable (though dualweak needs 122 nodes, more than double the
     others). </li>
     <li> p = 4: weak best, slightly worse weakpb, dualweakpb, then strong;
     dualweak very weak; unsolved weakpl, dualweakpl. </li>
     <li> p = 5: weak best, then strong; unsolved dualweak, weakpl,
     dualweakpl. Not tried yet: weakpb, dualweakpb. </li>
     <li> p = 6: weak best, then strong; unsolved dualweak, weakpl,
     dualweakpl. Not tried yet: weakpb, dualweakpb. </li>
    </ol>
   </li>
  </ul>

*/


