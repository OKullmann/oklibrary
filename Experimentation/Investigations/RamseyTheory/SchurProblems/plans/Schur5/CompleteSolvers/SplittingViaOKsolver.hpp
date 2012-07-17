// Oliver Kullmann, 17.7.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/RamseyTheory/SchurProblems/plans/Schur5/SplittingViaOKsolver/general.hpp
  \brief On computing lower bounds for schur(5) and variations via Cube-and-Conquer


  \todo SplittingViaOKsolver for direct encoding
  <ul>
   <li> The following data uses the old version of the Schur-generator, with
   a different clause-order and not eliminating the subsumed clauses. This
   should not matter. </li>
   <li> Splitting with D=10,20,30,40,50:
   \verbatim
> SplittingViaOKsolver -D10 Schur_5_160.cnf
> cat Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           800
c initial_number_of_clauses             33760
c initial_number_of_literal_occurrences 99600
c number_of_initial_unit-eliminations   0
c number_of_2-clauses_after_reduction   2000
c running_time(sec)                     3.4
c number_of_nodes                       39
c number_of_2-reductions                0
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        7
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 30857
c splitting_cases                       20

> SplittingViaOKsolver -D20 Schur_5_160.cnf
c running_time(sec)                     65.0
c number_of_nodes                       599
c max_tree_depth                        13
c splitting_cases                       300

> SplittingViaOKsolver -D30 Schur_5_160.cnf
c running_time(sec)                     636.8
c number_of_nodes                       6359
c max_tree_depth                        17
c splitting_cases                       3180

> SplittingViaOKsolver -D40 Schur_5_160.cnf
c running_time(sec)                     4290.0
c number_of_nodes                       46559
c max_tree_depth                        22
c splitting_cases                       23280

> SplittingViaOKsolver -D50 Schur_5_160.cnf
c running_time(sec)                     37030.2
c number_of_nodes                       448199
c max_tree_depth                        27
c splitting_cases                       224100
   \endverbatim
   Remarkable that not a single node was determined, and not even a single
   2-reduction took place: all what happened was unit-propagation. </li>
   <li> To estimate the hardness, for D=50 the first splitting-instance
   (which should be roughly the easiest) is considered on its own:
   \verbatim
> cat D50_1.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    800   26678    743    800   26678    77350     NA     NA   26678    77350   805
 length   count
      2    2974
      3   23553
      4      12
      5     139
   \endverbatim
   Attempts at solving it:
    <ol>
     <li> Running OKsolver-2002 on it XXX
     run with monitoring-depth 20, current status:
     \verbatim
 70127:    566     56.92  5.97E+07    14.50s     1.19s     0y  13d 12h 37m 12s     0     0   75
 92506:    129    101.88  1.07E+08     4.25s     2.14s     0y  23d 17h 34m 28s     0     0   75
     \endverbatim
     </li>
     <li> Running SplittingViaOKsolver with D=50: the first splitting-instance
     was rather easier for minisat-2.2.0, but not the second, so that instance
     is again considered on its own:
      <ol>
       <li> Data on the (full) splitting:
       \verbatim
> SplittingViaOKsolver -D50 D50_1.cnf
> cat SplitViaOKsolver_D50D50_1cnf_2012-07-14-182421/Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           743
c initial_number_of_clauses             26678
c initial_number_of_literal_occurrences 77350
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   2974
c running_time(sec)                     639.7
c number_of_nodes                       11289
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                18
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        18
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 7835508
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             D50_1.cnf
c splitting_directory                   SplitViaOKsolver_D50D50_1cnf_2012-07-14-182421/Instances
c splitting_cases                       5645
       \endverbatim
       Again, not a single node was solved (while now we got a few
       2-reductions). </li>
       <li> Statistics on the second splitting instance:
       \verbatim
> cat D50_1_D50_2.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    800   20271    682    800   20271    57596     NA     NA   20271    57596   807
 length   count
      2    3470
      3   16662
      4      25
      5     114
       \endverbatim
       </li>
       <li> Attempt at solving this second-level splitting instance directly
       with OKsolver-2002, run with monitoring-depth 8:
       \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D8 D50_1_D50_2.cnf
   135:   1602    244.71  6.26E+04    29.72s     4.69s     0y   0d  0h  9m 28s     0     0   36
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           682
c initial_number_of_clauses             20271
c initial_number_of_literal_occurrences 57596
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   3470
c running_time(sec)                     56774.7
c number_of_nodes                       33116810
c number_of_single_nodes                6780563
c number_of_quasi_single_nodes          0
c number_of_2-reductions                221484565
c number_of_pure_literals               0
c number_of_autarkies                   1980831
c number_of_missed_single_nodes         3845325
c max_tree_depth                        69
c proportion_searched                   5.284510e-01
c proportion_single                     6.748504e-07
c total_proportion                      0.5284516993945535
c number_of_table_enlargements          0
c number_of_1-autarkies                 5780791509
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             D50_1_D50_2.cnf
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           682
c initial_number_of_clauses             20271
c initial_number_of_literal_occurrences 57596
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   3470
c running_time(sec)                     140082.8
c number_of_nodes                       80167759
c number_of_single_nodes                17430976
c number_of_quasi_single_nodes          0
c number_of_2-reductions                533138873
c number_of_pure_literals               0
c number_of_autarkies                   5054570
c number_of_missed_single_nodes         9391486
c max_tree_depth                        71
c proportion_searched                   5.284511e-01
c proportion_single                     6.817329e-07
c total_proportion                      0.5284517405403815
c number_of_table_enlargements          0
c number_of_1-autarkies                 14205341478
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             D50_1_D50_2.cnf
> display_seconds(140082.8)
[1] "1.621d"
       \endverbatim
       So at monitor-node 136 the solver fell into a "deep hole", which might
       take a (very) long time.
       </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>

*/
