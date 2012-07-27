// Oliver Kullmann, 17.7.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/RamseyTheory/SchurProblems/plans/Schur5/SplittingViaOKsolver.hpp
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
> OKsolver_2002-O3-DNDEBUG -M -D20 D50_1.cnf
 70127:    566     56.92  5.97E+07    14.50s     1.19s     0y  13d 12h 37m 12s     0     0   75
 92506:    129    101.88  1.07E+08     4.25s     2.14s     0y  23d 17h 34m 28s     0     0   75
133128:     55    100.51  1.05E+08     1.53s     2.23s     0y  23d 14h 25m 54s     0     0   78
^C
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
c running_time(sec)                     311213.4
c number_of_nodes                       19225796
c number_of_single_nodes                1096574
c number_of_quasi_single_nodes          0
c number_of_2-reductions                207941293
c number_of_pure_literals               0
c number_of_autarkies                   324663
c number_of_missed_single_nodes         520692
c max_tree_depth                        78
c proportion_searched                   1.269612e-01
c proportion_single                     3.111316e-07
c total_proportion                      0.1269614993137118
c number_of_table_enlargements          0
c number_of_1-autarkies                 4859177916
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             D50_1.cnf
     \endverbatim
     With node 133129 it felt finally into a "deep hole".
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


  \todo SplittingViaOKsolver for direct encoding and full symmetry breaking
  <ul>
   <li> n=161 is the interesting problem. </li>
   <li> Splitting with D=10,20,30,40,50:
   \verbatim
> SplittingViaOKsolver -D10 Schur_fullsb_5_161.cnf
> cat Result
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           805
c initial_number_of_clauses             33926
c initial_number_of_literal_occurrences 100050
c number_of_initial_unit-eliminations   30
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           30
c reddiff_number_of_clauses             3772
c reddiff_number_of_literal_occurrences 11907
c number_of_2-clauses_after_reduction   2626
c running_time(sec)                     2.2
c number_of_nodes                       29
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                0
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        6
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 22193
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_fullsb_5_161.cnf
c splitting_directory                   SplitViaOKsolver_D10Schur_fullsb_5_161cnf_2012-07-26-204134/Instances
c splitting_cases                       15

> SplittingViaOKsolver -D20 Schur_fullsb_5_161.cnf
> cat Result
c running_time(sec)                     16.7
c number_of_nodes                       241
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                0
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        10
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 182109
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_fullsb_5_161.cnf
c splitting_directory                   SplitViaOKsolver_D20Schur_fullsb_5_161cnf_2012-07-26-204212/Instances
c splitting_cases                       121

> SplittingViaOKsolver -D30 Schur_fullsb_5_161.cnf
> cat Result
c number_of_nodes                       1669
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                0
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        15
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 1245746
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_fullsb_5_161.cnf
c splitting_directory                   SplitViaOKsolver_D30Schur_fullsb_5_161cnf_2012-07-26-204311/Instances
c splitting_cases                       835

> SplittingViaOKsolver -D40 Schur_fullsb_5_161.cnf
> cat Result
c running_time(sec)                     1237.6
c number_of_nodes                       13221
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                3
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        20
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 9727935
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_fullsb_5_161.cnf
c splitting_directory                   SplitViaOKsolver_D40Schur_fullsb_5_161cnf_2012-07-26-204723/Instances
c splitting_cases                       6611

> SplittingViaOKsolver -D50 Schur_fullsb_5_161.cnf
c running_time(sec)                     4814.9
c number_of_nodes                       74869
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                9
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        24
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 54359548
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_fullsb_5_161.cnf
c splitting_directory                   SplitViaOKsolver_D50Schur_fullsb_5_161cnf_2012-07-26-223856/Instances
c splitting_cases                       37435
   \endverbatim
   </li>
   <li> Considering the first splitting case for D=50:
   \verbatim
> cat D50Schur_fullsb_5_161-1.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    805   23162    715    805   23162    66272     NA     NA   23162    66272   811
 length   count
      2    3488
      3   19530
      4      14
      5     130

> SplittingViaOKsolver -D30 D50Schur_fullsb_5_161-1.cnf
> cat Result
c running_time(sec)                     14.7
c number_of_nodes                       281
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                13
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        10
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 192500
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             D50Schur_fullsb_5_161-1.cnf
c splitting_directory                   SplitViaOKsolver_D30D50Schur_fullsb_5_1611cnf_2012-07-27-084636/Instances
c splitting_cases                       141

> SplittingViaOKsolver -D50 D50Schur_fullsb_5_161-1.cnf
c running_time(sec)                     137.2
c number_of_nodes                       2943
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                404
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        15
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 1957733
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             D50Schur_fullsb_5_161-1.cnf
c splitting_directory                   SplitViaOKsolver_D50D50Schur_fullsb_5_1611cnf_2012-07-27-084758/Instances
c splitting_cases                       1472

> SplittingViaOKsolver -D70 D50Schur_fullsb_5_161-1.cnf
c running_time(sec)                     1165.4
c number_of_nodes                       27869
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                21667
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        22
c proportion_searched                   1.906586e-02
c proportion_single                     0.000000e+00
c total_proportion                      0.01906585693359375
c number_of_table_enlargements          0
c number_of_1-autarkies                 17821105
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             D50Schur_fullsb_5_161-1.cnf
c splitting_directory                   SplitViaOKsolver_D70D50Schur_fullsb_5_1611cnf_2012-07-27-085719/Instances
c splitting_cases                       13731

> ProcessSplitViaOKsolver SplitViaOKsolver_D70D50Schur_fullsb_5_1611cnf_2012-07-27-085719
> E=read_processsplit_minisat()
13731: 1.647h, sum-cfs=7.221649e+07, mean-t=0.432s, mean-cfs=5259, sat: 0
$t:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
  0.0360   0.1000   0.1400   0.4319   0.2480 110.0000
sd= 1.713718
      95%       96%       97%       98%       99%      100%
  1.38609   1.79531   2.44495   3.89784   6.24239 109.99100
sum= 5930.945
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
     31     908    1874    5259    4052  618600
sd= 14547.11
     95%      96%      97%      98%      99%     100%
 20445.5  24558.8  29958.7  45797.8  66841.2 618577.0
sum= 72216489
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -1.6082e-01  4.5264e-03 -35.529 < 2.2e-16 ***
E$cfs        1.1270e-04  2.9263e-07 385.150 < 2.2e-16 ***
R-squared: 0.9153
   \endverbatim
   </li>
   <li> This looks promising! </li>
   <li> Directly processing the instance:
   \verbatim
> satz215 D50Schur_fullsb_5_161-1.cnf
NB_MONO= 90, NB_UNIT= 146466324, NB_BRANCHE= 2561833, NB_BACK= 1301223
Program terminated in 2006.810 seconds.
satz215 D50Schur_fullsb_5_161-1.cnf 2006.810 2561833 1301223 413322450 13247541 0 805 23162 -726 16946878 5138133
   \endverbatim
   Even faster.
   </li>
  </ul>


  \todo SplittingViaOKsolver for palindromic problem (direct encoding)
  <ul>
   <li> Creation via e.g. output_pd_schur_stdname(5,162). </li>
   <li> Consider n=162; instance-statistics:
   \verbatim
> cat Schur_pd_5_162.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    405   11556    405    405   11556    33615     NA     NA   11556    33615   408
 length   count
      2    1215
      3   10260
      5      81
   \endverbatim
   </li>
   <li> Splitting with D=20,30,40,50,70:
   \verbatim
> cat SplitViaOKsolver_D20Schur_pd_5_162cnf_2012-07-16-101722/Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           405
c initial_number_of_clauses             11556
c initial_number_of_literal_occurrences 33615
c number_of_2-clauses_after_reduction   1215
c running_time(sec)                     9.6
c number_of_nodes                       559
c number_of_quasi_single_nodes          0
c number_of_2-reductions                0
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        13
c proportion_searched                   0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 215277
c splitting_cases                       280

> cat SplitViaOKsolver_D30Schur_pd_5_162cnf_2012-07-16-101810/Result
c running_time(sec)                     64.9
c number_of_nodes                       4199
c max_tree_depth                        16
c total_proportion                      0
c splitting_cases                       2100

> cat SplitViaOKsolver_D40Schur_pd_5_162cnf_2012-07-16-102029/Result
c running_time(sec)                     504.2
c number_of_nodes                       36119
c max_tree_depth                        21
c total_proportion                      0
c splitting_cases                       18060

> cat SplitViaOKsolver_D50Schur_pd_5_162cnf_2012-07-16-103612/Result
c running_time(sec)                     4625.1
c number_of_nodes                       265439
c max_tree_depth                        26
c splitting_cases                       132720
   \endverbatim
   Also here not a single node was solved, and not even a single 2-reduction
   was found. </li>
   <li> Solving the first sub-instance (for the above D=50):
    <ol>
     <li> Instance-statistics:
     \verbatim
> cat pd162D50_1.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    400   80236    348    400    8023    22635     NA     NA    8023    22635   410
 length   count
      2    1566
      3    6386
      4      10
      5      61
     \endverbatim
     (the false pc-value is a bug: the old 6 has been kept).
     </li>
     <li> Splitting with D=50,70:
     \verbatim
> cat SplitViaOKsolver_D50pd162D50_1cnf_2012-07-16-124512/Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           348
c initial_number_of_clauses             8023
c initial_number_of_literal_occurrences 22635
c number_of_2-clauses_after_reduction   1566
c running_time(sec)                     35.8
c number_of_nodes                       7813
c number_of_quasi_single_nodes          0
c number_of_2-reductions                18
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        18
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 2336215
c splitting_cases                       3907

> cat SplitViaOKsolver_D70pd162D50_1cnf_2012-07-16-125115/Result
c running_time(sec)                     330.6
c number_of_nodes                       86315
c number_of_2-reductions                1650
c max_tree_depth                        23
c total_proportion                      0
c splitting_cases                       43158
     \endverbatim
     Still not a single node was solved (but now we have some 2-reductions).
     </li>
     <li> Solving it with ProcessSplitViaOKsolver (employing minisat-2.2.0),
     in 701:25 m:
     \verbatim
> E=read_processsplit_minisat()
43158: 10.815h, sum-cfs=1.431975e+09, mean-t=0.902s, mean-cfs=33180, sat: 0
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
 0.0160  0.4320  0.7240  0.9021  1.1400 10.7500
sd= 0.7124658
     95%      96%      97%      98%      99%     100%
 2.27214  2.42015  2.61732  2.90818  3.53622 10.75270
sum= 38933.41
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
    280   15560   25810   33180   42460  366000
sd= 26718.4
      95%       96%       97%       98%       99%      100%
 85493.75  91356.80  98682.00 109313.20 132201.58 366031.00
sum= 1431974993
$t ~ $cfs:
              Estimate Std. Error t value  Pr(>|t|)
(Intercept) 3.7573e-02 1.1625e-03  32.322 < 2.2e-16 ***
E$cfs       2.6056e-05 2.7288e-08 954.851 < 2.2e-16 ***
R-squared: 0.9548
     \endverbatim
     </li>
     <li> So solving Schur_pd_5_162.cnf could be in reach. </li>
    </ol>
   </li>
  </ul>

*/
