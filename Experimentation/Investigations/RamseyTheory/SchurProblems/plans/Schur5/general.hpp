// Oliver Kullmann, 17.4.2009 (Swansea)
/* Copyright 2009, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/SchurProblems/plans/Schur5/general.hpp
  \brief On computing schur(5) and variations

  The conjecture is schur(5) = 161.

  (In [Ramsey Theory on the Integers; Landman, Robertson, 2003] we have the
  conjecture "schur(5) = 160", but already 1994 it was shown schur(5) >= 161
  (see below).)


  \todo Overview
  <ul>
   <li> See RamseyTheory/SchurProblems/plans/Schur5/LocalSearch/general.hpp
   for using local search. </li>
  </ul>


  \todo Statistics
  <ul>
   <li> Creation:
   \verbatim
output_schur_stdname(5,160);
   \endverbatim
   </li>
   <li> Counts:
   \verbatim
> cat Schur_5_160.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    800   33495    800    800   33495    98805     NA     NA   33495    98805   803
 length   count
      2    2000
      3   31335
      5     160
   \endverbatim
   </li>
  </ul>


  \todo Certificates
  <ul>
   <li> From [A Lower Bound for Schur Numbers and Multicolor Ramsey Numbers of
   K_3; Exoo, 1994]:
   \verbatim
C : uncompresss_palindromic_subsets(160,[
{4,5,15,16,22,28,29,39,40,41,42,48,49,59},
{2,3,8,14,19,20,24,25,36,46,47,51,62,73},
{7,9,11,12,13,17,27,31,32,33,35,37,53,56,57,61,79},
{1,6,10,18,21,23,26,30,34,38,43,45,50,54,65,74},
{44,52,55,58,60,63,64,66,67,68,69,70,71,72,75,76,77,78,80}
]);
certificate_pdschur_p(5,160,C);
  true
   \endverbatim
   and thus schur(5) >= 161 (it seems that was overlooked in the literature!).
   </li>
  </ul>


  \todo OKsolver_2002
  <ul>
   <li> With a cluster and a few months it looks feasible without any
   special action (the following data is for the old version, using the full
   hypergraph, not, as now, the subsumption-free form; this shouldn't make a
   big difference):
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D20 Schur_5_160.cnf
Name of file, monitoring depth and number of monitoring nodes:
 Schur_5_160.cnf,   20,    1048576
level: nodes processed, average nodes, predicted total nodes, time for monitoring node, average time, predicted remaining time, single nodes, autarkies, depth

...
14:   7705  14770.00  1.55E+10   250.47s   429.83s    14y 106d 12h 50m 47s     0     0   58
15:  18446  15015.07  1.57E+10   570.63s   439.22s    14y 220d 10h 39m 21s     0     0   58
...
39:   3382  16130.87  1.69E+10    93.10s   478.83s    15y 336d  1h 12m 16s     0     0   58
40:   5527  15865.77  1.66E+10   144.01s   470.46s    15y 234d 11h  3m 12s     0     0   58
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           800
c initial_number_of_clauses             33760
c initial_number_of_literal_occurrences 99600
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   2000
c running_time(sec)                     19635.3
c number_of_nodes                       667953
c number_of_single_nodes                18
c number_of_quasi_single_nodes          0
c number_of_2-reductions                10676278
c number_of_pure_literals               0
c number_of_autarkies                   1
c number_of_missed_single_nodes         359
c max_tree_depth                        58
c proportion_searched                   3.908341e-05
c proportion_single                     6.698131e-11
c total_proportion                      3.908347571268678e-05
c number_of_table_enlargements          0
c number_of_1-autarkies                 187851110
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_5_160.cnf

19635.3 / 3.908347571268678e-05 ~ 15.9 years
   \endverbatim
   </li>
   <li> And with initial symmetry breaking (using
   output_schur_sb_stdname(5,160)) a factor of nearly 5 should be achievable:
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D20 Schur_sb_5_160.cnf
Name of file, monitoring depth and number of monitoring nodes:
 Schur_sb_5_160.cnf,   20,    1048576
level: nodes processed, average nodes, predicted total nodes, time for monitoring node, average time, predicted remaining time, single nodes, autarkies, depth

...
   548:  22149  41402.39  4.34E+10   648.51s  1099.41s    36y 195d 19h  1m 23s     0     0   85
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           800
c initial_number_of_clauses             33764
c initial_number_of_literal_occurrences 99604
c number_of_initial_unit-eliminations   6
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           6
c reddiff_number_of_clauses             814
c reddiff_number_of_literal_occurrences 2574
c number_of_2-clauses_after_reduction   2137
c running_time(sec)                     602715.8
c number_of_nodes                       22697398
c number_of_single_nodes                1279
c number_of_quasi_single_nodes          0
c number_of_2-reductions                367966941
c number_of_pure_literals               0
c number_of_autarkies                   320
c number_of_missed_single_nodes         23646
c max_tree_depth                        85
c proportion_searched                   5.231135e-04
c proportion_single                     4.278459e-10
c total_proportion                      0.0005231138784438372
c number_of_table_enlargements          0
c number_of_1-autarkies                 6357718175
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_sb_5_160.cnf

602715.8 / 0.0005231138784438372 ~ 36.5 years
   \endverbatim
   </li>
  </ul>


  \todo SplittingViaOKsolver
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
     \endverbatim
     </li>
     <li> Running SplittingViaOKsolver with D=50: the first splitting-instance
     was rather easier for minisat-2.2.0, but not the second, so that instance
     is again considered on its own:
      <ol>
       <li> Data on the splitting:
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

