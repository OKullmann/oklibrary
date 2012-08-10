// Oliver Kullmann, 17.7.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/RamseyTheory/SchurProblems/plans/Schur5/SplittingViaOKsolver.hpp
  \brief On computing schur(5) and variations via Cube-and-Conquer


  \todo Direct encoding
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
     <li> Running OKsolver-2002 on it, with monitoring-depth 20:
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
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           743
c initial_number_of_clauses             26678
c initial_number_of_literal_occurrences 77350
c number_of_2-clauses_after_reduction   2974
c running_time(sec)                     639.7
c number_of_nodes                       11289
c number_of_quasi_single_nodes          0
c number_of_2-reductions                18
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        18
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 7835508
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
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           682
c initial_number_of_clauses             20271
c initial_number_of_literal_occurrences 57596
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

c sat_status                            2
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


  \todo Direct encoding and full symmetry breaking
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
c number_of_quasi_single_nodes          0
c number_of_2-reductions                0
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        6
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 22193
c splitting_cases                       15

> SplittingViaOKsolver -D20 Schur_fullsb_5_161.cnf
> cat Result
c running_time(sec)                     16.7
c number_of_nodes                       241
c number_of_quasi_single_nodes          0
c number_of_2-reductions                0
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        10
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 182109
c splitting_cases                       121

> SplittingViaOKsolver -D30 Schur_fullsb_5_161.cnf
> cat Result
c number_of_nodes                       1669
c number_of_quasi_single_nodes          0
c number_of_2-reductions                0
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        15
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 1245746
c splitting_cases                       835

> SplittingViaOKsolver -D40 Schur_fullsb_5_161.cnf
> cat Result
c running_time(sec)                     1237.6
c number_of_nodes                       13221
c number_of_quasi_single_nodes          0
c number_of_2-reductions                3
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        20
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 9727935
c splitting_cases                       6611

> SplittingViaOKsolver -D50 Schur_fullsb_5_161.cnf
c running_time(sec)                     4814.9
c number_of_nodes                       74869
c number_of_quasi_single_nodes          0
c number_of_2-reductions                9
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        24
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 54359548
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
c number_of_quasi_single_nodes          0
c number_of_2-reductions                13
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        10
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 192500
c splitting_cases                       141

> SplittingViaOKsolver -D50 D50Schur_fullsb_5_161-1.cnf
c running_time(sec)                     137.2
c number_of_nodes                       2943
c number_of_quasi_single_nodes          0
c number_of_2-reductions                404
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        15
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 1957733
c splitting_cases                       1472

> SplittingViaOKsolver -D70 D50Schur_fullsb_5_161-1.cnf
c running_time(sec)                     1165.4
c number_of_nodes                       27869
c number_of_quasi_single_nodes          0
c number_of_2-reductions                21667
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        22
c proportion_searched                   1.906586e-02
c proportion_single                     0.000000e+00
c total_proportion                      0.01906585693359375
c number_of_table_enlargements          0
c number_of_1-autarkies                 17821105
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


  \todo Palindromic problem (direct encoding)
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
> head -2 SplitViaOKsolver_D50Schur_pd_5_162cnf_2012-07-16-103612/Data
 i n d
1 69 57 11
> cat SplitViaOKsolver_D50Schur_pd_5_162cnf_2012-07-16-103612/Schur_pd_5_162.cnf | ApplyPass-O3-DNDEBUG SplitViaOKsolver_D50Schur_pd_5_162cnf_2012-07-16-103612/Instances/69 pd162D50_1.cnf
> cat pd162D50_1.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    400    8023    348    400    8023    22635     NA     NA    8023    22635   410
 length   count
      2    1566
      3    6386
      4      10
      5      61
     \endverbatim
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


  \todo Palindromic problem with full symmetry-breaking (direct encoding)
  <ul>
   <li> It seems that here, different from monolithic solvers, full symmetry-
   breaking (again) is powerful. </li>
   <li> Current values:
   \verbatim
pdschurfsb(5);
  lambda([n],if n > seconde(schur(5)) then false elseif n <= 157 or n = 159 or n = 160 then true else unknown)
   \endverbatim
   </li>
   <li> First open case n=153 for full symmetry-breaking:
   \verbatim
> SplittingViaOKsolver -D70 Schur_pd_fullsb_5_153.cnf
> cat Result
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           385
c initial_number_of_clauses             10377
c initial_number_of_literal_occurrences 30095
c number_of_initial_unit-eliminations   32
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           32
c reddiff_number_of_clauses             1970
c reddiff_number_of_literal_occurrences 6080
c number_of_2-clauses_after_reduction   1343
c running_time(sec)                     2929.1
c number_of_nodes                       635803
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1089
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        31
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 180654056
c splitting_directory                   SplitViaOKsolver_D70Schur_pd_fullsb_5_153cnf_2012-08-03-025129/Instances
c splitting_cases                       317902

> ProcessSplitViaOKsolver SplitViaOKsolver_D70Schur_pd_fullsb_5_153cnf_2012-08-03-025129
> E=read_processsplit_minisat()
11178: 2.056d, sum-cfs=5.482851e+09, mean-t=15.892s, mean-cfs=490504, sat: 0 1
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.304   7.245  12.590  15.890  20.080 171.000
sd= 13.08489
      95%       96%       97%       98%       99%      100%
 39.46275  41.90728  45.81677  51.56806  63.81160 171.00100
sum= 177642.7
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  12710  242700  402500  490500  618700 4356000
sd= 363736.3
    95%     96%     97%     98%     99%    100%
1153919 1217560 1321229 1472243 1806936 4356345
sum= 5482851182
$t ~ $cfs:
               Estimate  Std. Error  t value  Pr(>|t|)
(Intercept) -1.6659e+00  2.0616e-02  -80.809 < 2.2e-16 ***
E$cfs        3.5796e-05  3.3761e-08 1060.292 < 2.2e-16 ***
R-squared: 0.9902
> E[E$sat==1,]
          i npa  d  rn   rc       t sat    cfs    dec rts      r1 mem ptime
9992 315127 111 21 380 4746 3.53546   1 118795 148855 318 4656006  19     0
     stime     cfl
9992  0.01 2191388

> cd SplitViaOKsolver_D70Schur_pd_fullsb_5_153cnf_2012-08-03-025129/
> cat Instances/315127 > Solution_1
> cat Schur_pd_fullsb_5_153.cnf | ApplyPass-O3-DNDEBUG Solution_1 > Instance_1
> OKsolver_2002-O3-DNDEBUG -O -F Instance_1
c sat_status                            1
c initial_maximal_clause_length         5
c initial_number_of_variables           274
c initial_number_of_clauses             4746
c initial_number_of_literal_occurrences 12980
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   1349
c running_time(sec)                     40.5
c number_of_nodes                       50156
c number_of_single_nodes                34
c number_of_quasi_single_nodes          0
c number_of_2-reductions                543054
c number_of_pure_literals               0
c number_of_autarkies                   1140
c number_of_missed_single_nodes         433
c max_tree_depth                        34
c proportion_searched                   4.945261e-01
c proportion_single                     1.267903e-05
c total_proportion                      0.4945387840270996
c number_of_table_enlargements          0
c number_of_1-autarkies                 4655961

# via vi in Solution_1 the trailing "0" removed
# via vi in Instance_1.pa the initial "v" removed
> cat Instance_1.pa >> Solution_1

# checking:
> cat Schur_pd_fullsb_5_153.cnf | ApplyPass-O3-DNDEBUG Solution_1 result_1
> tail -1 result_1
p cnf 0 0

# via vi transformed Solution_1 into a CNF with a single clause ("p cnf 385 1")
> oklib --maxima
oklib_load_all();
F : read_fcl_f("Solution_1")$
pa : subset(first(F[2]), lambda([x], is(x>0)));
I : invstandardise_pd_schur_aloamo(5,153);
P : extract_partition(map(I,pa));
  [{1,20,23,38,41,44,48,51,54,60,63,70,73},
   {2,7,10,13,28,31,34,39,42,50,53,71,74,75},
   {5,6,18,19,21,22,29,32,49,52,62,65,69,72},
   {8,9,11,12,14,27,30,33,40,43,58,59,61,64},
   {3,4,15,16,17,24,25,26,35,36,37,45,46,47,55,56,57,66,67,68,76,77}]
FP : uncompresss_schurpalindromic_subsets(153,P);
certificate_pdschur_p(5,153,FP);
  true
   \endverbatim
   </li>
   <li> n=154:
   \verbatim
> SplittingViaOKsolver -D70 Schur_pd_fullsb_5_154.cnf
> cat Result
c initial_number_of_variables           385
c initial_number_of_clauses             10497
c initial_number_of_literal_occurrences 30455
c number_of_initial_unit-eliminations   33
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           33
c reddiff_number_of_clauses             2056
c reddiff_number_of_literal_occurrences 6335
c number_of_2-clauses_after_reduction   1339
c running_time(sec)                     4356.3
c number_of_nodes                       996117
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1695
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        32
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 281784243
c splitting_directory                   SplitViaOKsolver_D70Schur_pd_fullsb_5_154cnf_2012-08-05-063312/Instances
c splitting_cases                       498059

> ProcessSplitViaOKsolver SplitViaOKsolver_D70Schur_pd_fullsb_5_154cnf_2012-08-05-063312
> E=read_processsplit_minisat()
266: 32.16m, sum-cfs=7.059976e+07, mean-t=7.255s, mean-cfs=265413, sat: 0 1
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.044   3.075   5.766   7.255  10.030  33.820
sd= 5.503977
     95%      96%      97%      98%      99%     100%
17.64610 18.31150 18.62041 21.21010 26.20921 33.81810
sum= 1929.844
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   1697  125600  222400  265400  355200 1075000
sd= 183855.9
      95%       96%       97%       98%       99%      100%
 605994.2  626466.6  647840.1  712326.4  868537.4 1075040.0
sum= 70599757
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -6.5895e-01  5.2874e-02 -12.463 < 2.2e-16 ***
E$cfs        2.9818e-05  1.6386e-07 181.971 < 2.2e-16 ***
R-squared: 0.9921
> E[E$sat==1,]
         i npa  d  rn   rc        t sat  cfs  dec rts    r1 mem ptime stime
149 335555 116 20 385 4437 0.044002   1 1697 2288  11 67824  18     0  0.01
      cfl
149 29350

> cd SplitViaOKsolver_D70Schur_pd_fullsb_5_154cnf_2012-08-05-063312
> cat Instances/335555 > Solution_1
> cat Schur_pd_fullsb_5_154.cnf | ApplyPass-O3-DNDEBUG Solution_1 > Instance_1
> OKsolver_2002-O3-DNDEBUG -O -F Instance_1
c sat_status                            1
c initial_maximal_clause_length         5
c initial_number_of_variables           269
c initial_number_of_clauses             4437
c initial_number_of_literal_occurrences 11980
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   1420
c running_time(sec)                     5.4
c number_of_nodes                       6024
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                73299
c number_of_pure_literals               0
c number_of_autarkies                   29
c number_of_missed_single_nodes         12
c max_tree_depth                        31
c proportion_searched                   5.283203e-01
c proportion_single                     0.000000e+00
c total_proportion                      0.5283203125
c number_of_table_enlargements          0
c number_of_1-autarkies                 564648

# via vi in Solution_1 the trailing "0" removed
# via vi in Instance_1.pa the initial "v" removed
> cat Instance_1.pa >> Solution_1

# checking:
> cat Schur_pd_fullsb_5_154.cnf | ApplyPass-O3-DNDEBUG Solution_1 result_1
> tail -1 result_1
p cnf 0 0

# via vi transformed Solution_1 into a CNF with a single clause ("p cnf 385 1")
> oklib --maxima
oklib_load_all();
F : read_fcl_f("Solution_1")$
pa : subset(first(F[2]), lambda([x], is(x>0)));
I : invstandardise_pd_schur_aloamo(5,154);
P : extract_partition(map(I,pa));
  [{1,4,10,15,18,24,27,38,41,44,46,52,55,58,60,69,72,74},
   {2,11,12,16,19,25,26,33,39,47,53,56,62,70,71},
   {5,7,8,17,21,23,35,37,48,49,51,63,64,73,76},
   {3,9,14,22,30,32,34,40,42,57,59,61,65,67,77},
   {6,13,20,28,29,31,36,43,45,50,54,66,68,75}]
FP : uncompresss_schurpalindromic_subsets(154,P);
certificate_pdschur_p(5,154,FP);
  true
   \endverbatim
   </li>
   <li> First open case n=155 for palindromic problems:
   \verbatim
> SplittingViaOKsolver -D30 Schur_pd_fullsb_5_155.cnf
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           395
c initial_number_of_clauses             11034
c initial_number_of_literal_occurrences 32040
c number_of_initial_unit-eliminations   32
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           32
c reddiff_number_of_clauses             2061
c reddiff_number_of_literal_occurrences 6374
c number_of_2-clauses_after_reduction   1394
c running_time(sec)                     10.2
c number_of_nodes                       1245
c number_of_quasi_single_nodes          0
c number_of_2-reductions                0
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        14
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 415640
c splitting_cases                       623

> SplittingViaOKsolver -D50 Schur_pd_fullsb_5_155.cnf
> cat Result
c running_time(sec)                     172.3
c number_of_nodes                       29675
c number_of_quasi_single_nodes          0
c number_of_2-reductions                3
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        23
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 9318076
c splitting_cases                       14838

> SplittingViaOKsolver -D60 Schur_pd_fullsb_5_155.cnf
> cat Result
c running_time(sec)                     772.6
c number_of_nodes                       129221
c number_of_quasi_single_nodes          0
c number_of_2-reductions                138
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        26
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 39307549
c splitting_cases                       64611

> SplittingViaOKsolver -D70 Schur_pd_fullsb_5_155.cnf
> cat Result
c running_time(sec)                     3640.9
c number_of_nodes                       538751
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1690
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        29
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 158468083
c splitting_cases                       269376

> Process SplitViaOKsolver SplitViaOKsolver_D70Schur_pd_fullsb_5_155cnf_2012-07-28-112247
> E=read_processsplit_minisat()
18975: 2.677d, sum-cfs=7.586739e+09, mean-t=12.187s, mean-cfs=399828, sat: 0 1
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.108   5.166   8.913  12.190  15.180 207.100
sd= 11.63747
      95%       96%       97%       98%       99%      100%
 33.22690  35.32268  38.33360  44.18536  57.77011 207.05300
sum= 231255.6
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   2765  184000  308100  399800  505500 5974000
sd= 341112.7
    95%     96%     97%     98%     99%    100%
1038617 1098018 1173054 1340196 1684181 5974231
sum= 7586738938
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -1.3879e+00  1.2720e-02 -109.11 < 2.2e-16 ***
E$cfs        3.3953e-05  2.4203e-08 1402.85 < 2.2e-16 ***
R-squared: 0.9905
> E[E$sat==1,]
           i npa  d  rn   rc         t sat    cfs    dec  rts       r1 mem
993    33374 112 20 395 4904  0.912057   1  33455  42609  124  1275193  18
11026 111460 110 21 395 5017 11.640700   1 430207 539968 1021 16186431  19
      ptime stime     cfl
993       0  0.03  561274
11026     0  0.01 6380636

> cd SplitViaOKsolver_D70Schur_pd_fullsb_5_155cnf_2012-07-28-112247
> cat Instances/33374 > Solution_1
> cat Schur_pd_fullsb_5_155.cnf | ApplyPass-O3-DNDEBUG Solution_1 Instance_1
# minisat-2.2.0 Instance_1 Temp_1 # CAUTION: total assignment!
> OKsolver_2002-O3-DNDEBUG -O -F Instance_1
c sat_status                            1
c initial_maximal_clause_length         5
c initial_number_of_variables           283
c initial_number_of_clauses             4904
c initial_number_of_literal_occurrences 13460
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   1345
c running_time(sec)                     11.9
c number_of_nodes                       12593
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                157476
c number_of_pure_literals               0
c number_of_autarkies                   8
c number_of_missed_single_nodes         3
c max_tree_depth                        29
c proportion_searched                   3.775916e-01
c proportion_single                     0.000000e+00
c total_proportion                      0.377591609954834
c number_of_table_enlargements          0
c number_of_1-autarkies                 1235556
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Instance_1

# via vi in Solution_1 the trailing "0" removed
# via vi in Instance_1.pa the initial "v" removed
> cat Instance_1.pa >> Solution_1
# Solution_1 is now a satisfying assignment
# via vi transformed Solution_1 into a CNF with a single clause ("p cnf 395 1")
> oklib --maxima
oklib_load_all();
F : read_fcl_f("Solution_1")$
pa : subset(first(F[2]), lambda([x], is(x>0)));
I : invstandardise_pd_schur_aloamo(5,155);
P : extract_partition(map(I,pa));
  [{1,15,20,24,28,34,38,42,60,63,71,74,77},
   {2,7,8,12,17,21,22,27,36,46,56,61,62,65,66,75,76,104},
   {5,19,23,25,26,33,37,39,40,41,43,53,54,55,57},
   {6,9,10,11,13,14,29,31,32,47,48,49,51,52,67,68,69,70},
   {3,4,16,18,30,35,44,45,50,58,59,64,72,73,78}]
FP : uncompresss_schurpalindromic_subsets(155,P);
certificate_pdschur_p(5,155,FP);
  true
certificate_pdwschur_p(5,155,FP);
  false
   \endverbatim
   This is a new result (satisfiability was not known before), so the full
   symmetry-breaking seems helpful in combination with C&C for breaking up
   hard problems (where some systematic search is needed). </li>
   <li> n=156:
   \verbatim
> SplittingViaOKsolver -D70 Schur_pd_fullsb_5_156.cnf
> cat Result
c initial_maximal_clause_length         5
c initial_number_of_variables           390
c initial_number_of_clauses             10758
c initial_number_of_literal_occurrences 31220
c number_of_initial_unit-eliminations   33
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           33
c reddiff_number_of_clauses             2082
c reddiff_number_of_literal_occurrences 6418
c number_of_2-clauses_after_reduction   1364
c running_time(sec)                     4047.0
c number_of_nodes                       917655
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1421
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        32
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 264213309
c splitting_directory                   SplitViaOKsolver_D70Schur_pd_fullsb_5_156cnf_2012-08-05-223448/Instances
c splitting_cases                       458828

> ProcessSplitViaOKsolver SplitViaOKsolver_D70Schur_pd_fullsb_5_156cnf_2012-08-05-223448
> E=read_processsplit_minisat()
4806: 1.074d, sum-cfs=2.991115e+09, mean-t=19.308s, mean-cfs=622371, sat: 0 1
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.248   9.026  14.820  19.310  24.640 199.300
sd= 15.69064
     95%      96%      97%      98%      99%     100%
 47.6690  52.4193  57.6322  67.5742  79.9350 199.3200
sum= 92794.07
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  10960  316600  503400  622400  804100 5071000
sd= 449787.7
    95%     96%     97%     98%     99%    100%
1444224 1543456 1734281 1949537 2299523 5070607
sum= 2991114807
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -2.3160e+00  3.4585e-02 -66.966 < 2.2e-16 ***
E$cfs        3.4745e-05  4.5041e-08 771.399 < 2.2e-16 ***
R-squared: 0.992
> E[E$sat==1,]
          i npa  d  rn   rc        t sat   cfs   dec rts     r1 mem ptime stime
4409 136459 112 19 385 4776 0.248015   1 10962 13881  46 422689  18     0  0.01
        cfl
4409 182234

> cd SplitViaOKsolver_D70Schur_pd_fullsb_5_156cnf_2012-08-05-223448/
> cat Instances/136459 > Solution_1
> cat Schur_pd_fullsb_5_156.cnf | ApplyPass-O3-DNDEBUG Solution_1 Instance_1
> OKsolver_2002-O3-DNDEBUG -O -F Instance_1
c sat_status                            1
c initial_maximal_clause_length         5
c initial_number_of_variables           278
c initial_number_of_clauses             4776
c initial_number_of_literal_occurrences 13002
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   1418
c running_time(sec)                     11.1
c number_of_nodes                       11098
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                135413
c number_of_pure_literals               0
c number_of_autarkies                   3
c number_of_missed_single_nodes         4
c max_tree_depth                        24
c proportion_searched                   9.091187e-01
c proportion_single                     0.000000e+00
c total_proportion                      0.90911865234375
c number_of_table_enlargements          0
c number_of_1-autarkies                 1088749

# via vi in Solution_1 the trailing "0" removed
# via vi in Instance_1.pa the initial "v" removed
> cat Instance_1.pa >> Solution_1

# checking:
> cat Schur_pd_fullsb_5_156.cnf | ApplyPass-O3-DNDEBUG Solution_1 result_1
> tail -1 result_1
p cnf 0 0

# via vi transformed Solution_1 into a CNF with a single clause ("p cnf 390 1")
oklib_load_all();
F : read_fcl_f("Solution_1")$
pa : subset(first(F[2]), lambda([x], is(x>0)));
I : invstandardise_pd_schur_aloamo(5,156);
P : extract_partition(map(I,pa));
  [{1,3,15,22,26,39,46,50,60,62,64,70,74},
   {2,7,8,23,24,27,37,41,52,63,69,72,73},
   {4,5,18,19,20,28,29,30,42,43,51,65,66,67,68,75},
   {9,10,11,12,13,14,31,32,33,34,36,38,53,54,55,56,57,59,61,77,78},
   {6,16,17,21,25,35,40,44,45,47,48,49,58,71,76}]
FP : uncompresss_schurpalindromic_subsets(156,P);
certificate_pdschurfsb_p(5,156,FP);
  true
certificate_pdwschur_p(5,156,FP);
  true
certificate_pdwschurfsb_p(5,156,FP);
  false
   \endverbatim
   </li>
   <li> n=157:
   \verbatim
> SplittingViaOKsolver -D70 Schur_pd_fullsb_5_157.cnf
> cat Result
c initial_maximal_clause_length         5
c initial_number_of_variables           395
c initial_number_of_clauses             10899
c initial_number_of_literal_occurrences 31635
c number_of_initial_unit-eliminations   32
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           32
c reddiff_number_of_clauses             2022
c reddiff_number_of_literal_occurrences 6246
c number_of_2-clauses_after_reduction   1383
c running_time(sec)                     3100.0
c number_of_nodes                       631007
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1132
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        31
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 185594837
c splitting_directory                   SplitViaOKsolver_D70Schur_pd_fullsb_5_157cnf_2012-08-07-135006/Instances
c splitting_cases                       315504

> ProcessSplitViaOKsolver SplitViaOKsolver_D70Schur_pd_fullsb_5_157cnf_2012-08-07-135006
> E=read_processsplit_minisat()
2867: 15.243h, sum-cfs=1.758937e+09, mean-t=19.140s, mean-cfs=613511, sat: 0 1
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.100   7.954  14.400  19.140  25.470 137.900
sd= 16.40265
      95%       96%       97%       98%       99%      100%
 52.32010  55.12036  63.32984  71.24335  79.81263 137.91300
sum= 54874.46
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   4951  278600  483100  613500  809200 3858000
sd= 479648.8
    95%     96%     97%     98%     99%    100%
1569219 1694472 1855567 2089583 2301739 3857938
sum= 1758936963
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -1.7485e+00  4.6513e-02  -37.59 < 2.2e-16 ***
E$cfs        3.4047e-05  5.9732e-08  570.00 < 2.2e-16 ***
R-squared: 0.9913
> E[E$sat==1,]
        i npa  d  rn   rc        t sat  cfs   dec rts     r1 mem ptime stime
345 19118 113 16 395 4817 0.188011   1 7911 10245  31 301903  18     0  0.01
       cfl
345 136542

> cd SplitViaOKsolver_D70Schur_pd_fullsb_5_157cnf_2012-08-07-135006
> cat Instances/19118 > Solution_1
> cat Schur_pd_fullsb_5_157.cnf | ApplyPass-O3-DNDEBUG Solution_1 Instance_1
> OKsolver_2002-O3-DNDEBUG -O -F Instance_1
c sat_status                            1
c initial_maximal_clause_length         5
c initial_number_of_variables           282
c initial_number_of_clauses             4817
c initial_number_of_literal_occurrences 13134
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   1410
c running_time(sec)                     18.4
c number_of_nodes                       20252
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                243181
c number_of_pure_literals               0
c number_of_autarkies                   27
c number_of_missed_single_nodes         5
c max_tree_depth                        29
c proportion_searched                   7.331543e-01
c proportion_single                     0.000000e+00
c total_proportion                      0.733154296875
c number_of_table_enlargements          0
c number_of_1-autarkies                 1971878

# via vi in Solution_1 the trailing "0" removed
# via vi in Instance_1.pa the initial "v" removed
> cat Instance_1.pa >> Solution_1

# checking:
> cat Schur_pd_fullsb_5_157.cnf | ApplyPass-O3-DNDEBUG Solution_1 result_1
> tail -1 result_1
p cnf 0 0

# via vi transformed Solution_1 into a CNF with a single clause ("p cnf 395 1")
oklib_load_all();
F : read_fcl_f("Solution_1")$
pa : subset(first(F[2]), lambda([x], is(x>0)));
I : invstandardise_pd_schur_aloamo(5,157);
P : extract_partition(map(I,pa));
  [{1,3,7,9,22,24,34,49,51,53,64,66,74,78},
   {2,12,17,25,26,41,44,48,54,59,68,72,75},
   {4,5,13,19,20,21,29,31,37,46,47,61,63,70,71,73},
   {8,10,14,15,27,32,36,39,56,58,60,65,76,77},
   {6,11,16,18,23,28,30,33,35,38,40,42,43,45,50,52,55,57,62,67,69,79}]
FP : uncompresss_schurpalindromic_subsets(157,P);
certificate_pdschurfsb_p(5,157,FP);
  true
certificate_pdwschur_p(5,157,FP);
  true
certificate_pdwschurfsb_p(5,157,FP);
  false
   \endverbatim
   </li>
   <li> n=158:
   \verbatim
> SplittingViaOKsolver -D70 Schur_pd_fullsb_5_158.cnf
> cat Result
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           400
c initial_number_of_clauses             11435
c initial_number_of_literal_occurrences 33230
c number_of_initial_unit-eliminations   33
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           33
c reddiff_number_of_clauses             2176
c reddiff_number_of_literal_occurrences 6717
c number_of_2-clauses_after_reduction   1406
c running_time(sec)                     3439.7
c number_of_nodes                       676537
c number_of_quasi_single_nodes          0
c number_of_2-reductions                872
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        30
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 201682920
c splitting_directory                   SplitViaOKsolver_D70Schur_pd_fullsb_5_158cnf_2012-07-31-200509/Instances
c splitting_cases                       338269

> ProcessSplitViaOKsolver SplitViaOKsolver_D70Schur_pd_fullsb_5_158cnf_2012-07-31-200509
XXX cs-oksvr
   \endverbatim
   </li>
   <li> n=159:
   \verbatim
> SplittingViaOKsolver -D70 Schur_pd_fullsb_5_159.cnf
> cat Result
c initial_maximal_clause_length         5
c initial_number_of_variables           400
c initial_number_of_clauses             11185
c initial_number_of_literal_occurrences 32485
c number_of_initial_unit-eliminations   32
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           32
c reddiff_number_of_clauses             2052
c reddiff_number_of_literal_occurrences 6345
c number_of_2-clauses_after_reduction   1402
c running_time(sec)                     2949.8
c number_of_nodes                       621713
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1511
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        30
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 185942714
c splitting_directory                   SplitViaOKsolver_D70Schur_pd_fullsb_5_159cnf_2012-08-08-150802/Instances
c splitting_cases                       310857
# not processed, since solutions were found via local search
   \endverbatim
   </li>
  </ul>


  \todo Weak palindromic problem with full symmetry-breaking (direct encoding)
  <ul>
   <li> It seems that here, different from monolithic solvers, full symmetry-
   breaking (again) is powerful. </li>
   <li> Current values:
   \verbatim
pdwschurfsb(5);
  lambda([n],if n > 1631 then false elseif n <= 158 or n = 160 then true else unknown)
   \endverbatim
   </li>
   <li> Open case n=152:
   \verbatim
> SplittingViaOKsolver -D70 WSchur_pd_fullsb_5_152.cnf
> cat Result
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           380
c initial_number_of_clauses             10231
c initial_number_of_literal_occurrences 29670
c number_of_initial_unit-eliminations   35
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           35
c reddiff_number_of_clauses             2151
c reddiff_number_of_literal_occurrences 6600
c number_of_2-clauses_after_reduction   1302
c running_time(sec)                     2116.5
c number_of_nodes                       576241
c number_of_quasi_single_nodes          0
c number_of_2-reductions                682
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        29
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 159048440
c splitting_directory                   SplitViaOKsolver_D70WSchur_pd_fullsb_5_152cnf_2012-08-03-234001/Instances
c splitting_cases                       288121

> ProcessSplitViaOKsolver SplitViaOKsolver_D70WSchur_pd_fullsb_5_152cnf_2012-08-03-234001
> E=read_processsplit_minisat()
1645: 4.215h, sum-cfs=5.560082e+08, mean-t=9.225s, mean-cfs=337999, sat: 0 1
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.224   4.072   6.936   9.225  12.500  55.040
sd= 7.372861
     95%      96%      97%      98%      99%     100%
24.53430 26.70026 28.53764 30.07758 33.23634 55.03940
sum= 15174.69
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  10120  161300  264900  338000  456700 1756000
sd= 246921.3
      95%       96%       97%       98%       99%      100%
 833579.6  915753.7  987086.6 1045019.0 1126276.0 1756498.0
sum= 556008154
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -8.4564e-01  2.0337e-02 -41.581 < 2.2e-16 ***
E$cfs        2.9794e-05  4.8591e-08 613.159 < 2.2e-16 ***
R-squared: 0.9956
> E[E$sat==1,]
          i npa  d  rn   rc       t sat   cfs    dec rts      r1 mem ptime
1158 247285 115 19 375 4315 2.43215   1 98545 122123 255 3741012  18     0
1381   5231 114 14 375 4334 0.49203   1 22598  28202  80  869316  18     0
     stime     cfl
1158  0.01 1720694
1381  0.01  350170

> cd SplitViaOKsolver_D70WSchur_pd_fullsb_5_152cnf_2012-08-03-234001/
> cat Instances/247285 > Solution_1
> cat WSchur_pd_fullsb_5_152.cnf | ApplyPass-O3-DNDEBUG Solution_1 > Instance_1
> OKsolver_2002-O3-DNDEBUG -O -F Instance_1
c sat_status                            1
c initial_maximal_clause_length         5
c initial_number_of_variables           265
c initial_number_of_clauses             4315
c initial_number_of_literal_occurrences 11738
c number_of_2-clauses_after_reduction   1292
c running_time(sec)                     8.9
c number_of_nodes                       10112
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                121641
c number_of_pure_literals               0
c number_of_autarkies                   5
c number_of_missed_single_nodes         1
c max_tree_depth                        25
c proportion_searched                   9.804688e-01
c proportion_single                     0.000000e+00
c total_proportion                      0.98046875
c number_of_table_enlargements          0
c number_of_1-autarkies                 948439

# via vi in Solution_1 the trailing "0" removed
# via vi in Instance_1.pa the initial "v" removed
> cat Instance_1.pa >> Solution_1

# checking:
> cat WSchur_pd_fullsb_5_152.cnf | ApplyPass-O3-DNDEBUG Solution_1 result_1
> tail -1 result_1
p cnf 0 0

# via vi transformed Solution_1 into a CNF with a single clause ("p cnf 380 1")
> oklib --maxima
oklib_load_all();
F : read_fcl_f("Solution_1")$
pa : subset(first(F[2]), lambda([x], is(x>0)));
I : invstandardise_pd_wschur_aloamo(5,152);
P : extract_partition(map(I,pa));
  [{1,5,15,23,25,33,35,42,44,52,54,61,71,73},
   {3,7,12,28,29,30,38,43,47,49,64,69,70},
   {4,9,14,16,19,21,26,50,55,56,57,58,63,68},
   {2,10,11,17,24,31,36,37,40,45,59,66,75},
   {6,8,13,18,20,22,27,32,34,39,41,46,48,51,53,60,62,65,67,72,74,76}]
FP : uncompresss_wschurpalindromic_subsets(152,P);
certificate_pdwschur_p(5,152,FP);
  true
certificate_pdschur_p(5,152,FP);
  false
   \endverbatim
   </li>
   <li> Open case n=153:
   \verbatim
> SplittingViaOKsolver -D30 WSchur_pd_fullsb_5_153.cnf
> cat Result
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           385
c initial_number_of_clauses             10377
c initial_number_of_literal_occurrences 30095
c number_of_initial_unit-eliminations   32
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           32
c reddiff_number_of_clauses             1978
c reddiff_number_of_literal_occurrences 6112
c number_of_2-clauses_after_reduction   1351
c running_time(sec)                     8.4
c number_of_nodes                       1089
c number_of_quasi_single_nodes          0
c number_of_2-reductions                0
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        14
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 352260
c splitting_cases                       545

> SplittingViaOKsolver -D50 WSchur_pd_fullsb_5_153.cnf
> cat Result
c running_time(sec)                     165.1
c number_of_nodes                       27145
c number_of_quasi_single_nodes          0
c number_of_2-reductions                4
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        22
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 8248546
c splitting_cases                       13573

> SplittingViaOKsolver -D60 WSchur_pd_fullsb_5_153.cnf
c running_time(sec)                     687.5
c number_of_nodes                       117087
c number_of_quasi_single_nodes          0
c number_of_2-reductions                38
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        26
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 34435065
c splitting_cases                       58544

> SplittingViaOKsolver -D70 WSchur_pd_fullsb_5_153.cnf
> cat Result
c running_time(sec)                     2080.8
c number_of_nodes                       454251
c number_of_quasi_single_nodes          0
c number_of_2-reductions                553
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        29
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 129181374
c splitting_directory                   SplitViaOKsolver_D70WSchur_pd_fullsb_5_153cnf_2012-07-28-094727/Instances
c splitting_cases                       227126

> ProcessSplitViaOKsolver SplitViaOKsolver_D70WSchur_pd_fullsb_5_153cnf_2012-07-28-094727
> E=read_processsplit_minisat()
11649: 2.6d, sum-cfs=6.750158e+09, mean-t=19.283s, mean-cfs=579462, sat: 0 1
$t:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
  0.3299   8.2910  14.6100  19.2800  24.1500 332.9000
sd= 17.83307
      95%       96%       97%       98%       99%      100%
 49.48850  55.23624  62.68350  74.20714  87.35518 332.91700
sum= 224631.1
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  13190  274900  461800  579500  731200 7735000
sd= 474178.7
    95%     96%     97%     98%     99%    100%
1411206 1554759 1736299 2025211 2371964 7734584
sum= 6750157549
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -2.4275e+00  2.2583e-02 -107.49 < 2.2e-16 ***
E$cfs        3.7467e-05  3.0162e-08 1242.19 < 2.2e-16 ***
R-squared: 0.9925
> E[E$sat==1,]
         i npa  d  rn   rc        t sat   cfs   dec rts     r1 mem ptime stime
9929 89255 110 21 380 4733 0.478927   1 18732 23241  63 738392  19     0  0.01
        cfl
9929 344789
   \endverbatim
   It seems that the full symmetry-breaking helps the C&C approach.
   </li>
   <li> Open case n=155:
   \verbatim
> SplittingViaOKsolver -D70 WSchur_pd_fullsb_5_155.cnf
> cat Result
c initial_maximal_clause_length         5
c initial_number_of_variables           390
c initial_number_of_clauses             10638
c initial_number_of_literal_occurrences 30870
c number_of_initial_unit-eliminations   32
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           32
c reddiff_number_of_clauses             1982
c reddiff_number_of_literal_occurrences 6123
c number_of_2-clauses_after_reduction   1360
c running_time(sec)                     1794.6
c number_of_nodes                       407975
c number_of_quasi_single_nodes          0
c number_of_2-reductions                738
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        28
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 117992076
c splitting_directory                   SplitViaOKsolver_D70WSchur_pd_fullsb_5_155cnf_2012-08-05-142247/Instances
c splitting_cases                       203988

> ProcessSplitViaOKsolver SplitViaOKsolver_D70WSchur_pd_fullsb_5_155cnf_2012-08-05-142247
> E=read_processsplit_minisat()
822: 3.978h, sum-cfs=4.669884e+08, mean-t=17.421s, mean-cfs=568112, sat: 0 1
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.656   6.393  12.510  17.420  21.220 192.700
sd= 18.51401
      95%       96%       97%       98%       99%      100%
 48.92190  53.51658  61.06488  79.10456  85.19918 192.68800
sum= 14320.22
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  27080  233600  432300  568100  702800 5216000
sd= 533209.8
    95%     96%     97%     98%     99%    100%
1496219 1648431 1809731 2319176 2561892 5215656
sum= 466988358
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -2.2406e+00  7.6046e-02 -29.464 < 2.2e-16 ***
E$cfs        3.4609e-05  9.7630e-08 354.493 < 2.2e-16 ***
R-squared: 0.9935
> E[E$sat==1,]
        i npa  d  rn   rc      t sat    cfs    dec rts       r1 mem ptime stime
414 37094 112 20 390 4730 7.9405   1 283607 356829 669 10682080  18     0  0.01
        cfl
414 4821596

> cd SplitViaOKsolver_D70WSchur_pd_fullsb_5_155cnf_2012-08-05-142247
> cat Instances/37094 > Solution_1
> cat WSchur_pd_fullsb_5_155.cnf | ApplyPass-O3-DNDEBUG Solution_1 > Instance_1
> OKsolver_2002-O3-DNDEBUG -O -F Instance_1
c sat_status                            1
c initial_maximal_clause_length         5
c initial_number_of_variables           278
c initial_number_of_clauses             4730
c initial_number_of_literal_occurrences 12877
c number_of_2-clauses_after_reduction   1407
c running_time(sec)                     18.7
c number_of_nodes                       21441
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                268032
c number_of_pure_literals               0
c number_of_autarkies                   14
c number_of_missed_single_nodes         18
c max_tree_depth                        37
c proportion_searched                   9.267578e-01
c proportion_single                     0.000000e+00
c total_proportion                      0.9267578125
c number_of_table_enlargements          0
c number_of_1-autarkies                 2026595

# via vi in Solution_1 the trailing "0" removed
# via vi in Instance_1.pa the initial "v" removed
> cat Instance_1.pa >> Solution_1

# checking:
> cat WSchur_pd_fullsb_5_155.cnf | ApplyPass-O3-DNDEBUG Solution_1 result_1
> tail -1 result_1
p cnf 0 0

# via vi transformed Solution_1 into a CNF with a single clause ("p cnf 390 1")
> oklib --maxima
oklib_load_all();
F : read_fcl_f("Solution_1")$
pa : subset(first(F[2]), lambda([x], is(x>0)));
I : invstandardise_pd_wschur_aloamo(5,155);
P : extract_partition(map(I,pa));
  [{1,4,11,14,17,20,27,30,32,35,45,48,53,61,69,74},
   {2,3,7,13,18,23,33,34,38,50,54,58,62,70,78},
   {5,9,12,15,16,19,22,29,36,39,42,46,49,66,73,77},
   {6,8,10,21,24,25,26,28,37,40,41,44,57,60},
   {31,43,47,51,52,55,56,59,63,64,65,67,68,71,72,75,76}]
FP : uncompresss_wschurpalindromic_subsets(155,P);
certificate_pdwschur_p(5,155,FP);
  true
certificate_pdschur_p(5,155,FP);
  false
   \endverbatim
   </li>
   <li> n=156:
   \verbatim
> SplittingViaOKsolver -D70 WSchur_pd_fullsb_5_156.cnf
> cat Result
c initial_maximal_clause_length         5
c initial_number_of_variables           390
c initial_number_of_clauses             10758
c initial_number_of_literal_occurrences 31220
c number_of_initial_unit-eliminations   35
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           35
c reddiff_number_of_clauses             2227
c reddiff_number_of_literal_occurrences 6840
c number_of_2-clauses_after_reduction   1349
c running_time(sec)                     2880.1
c number_of_nodes                       581319
c number_of_quasi_single_nodes          0
c number_of_2-reductions                483
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        28
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 166443741
c splitting_directory                   SplitViaOKsolver_D70WSchur_pd_fullsb_5_156cnf_2012-08-07-135407/Instances
c splitting_cases                       290660

> ProcessSplitViaOKsolver SplitViaOKsolver_D70WSchur_pd_fullsb_5_156cnf_2012-08-07-135407
> E=read_processsplit_minisat()
3369: 17.204h, sum-cfs=1.861865e+09, mean-t=18.384s, mean-cfs=552646, sat: 0 1
$t:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
  0.5089   8.3440  14.6100  18.3800  23.1500 214.4000
sd= 16.38686
      95%       96%       97%       98%       99%      100%
 44.45220  47.25181  54.08120  62.09676  86.40503 214.40000
sum= 61934.77
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  19880  275500  459400  552600  694100 5541000
sd= 433863.7
    95%     96%     97%     98%     99%    100%
1254468 1351570 1509760 1709463 2295812 5541232
sum= 1861865139
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -2.4197e+00  3.7360e-02 -64.768 < 2.2e-16 ***
E$cfs        3.7643e-05  5.3176e-08 707.895 < 2.2e-16 ***
R-squared: 0.9933
> E[E$sat==1,]
          i npa  d  rn   rc        t sat   cfs   dec rts      r1 mem ptime
3163 103733 114 20 390 4668 0.671897   1 26242 32763  94 1026392  19     0
     stime    cfl
3163  0.01 426401

> cd SplitViaOKsolver_D70WSchur_pd_fullsb_5_156cnf_2012-08-07-135407
> cat Instances/103733 > Solution_1
> cat WSchur_pd_fullsb_5_156.cnf | ApplyPass-O3-DNDEBUG Solution_1 > Instance_1
> OKsolver_2002-O3-DNDEBUG -O -F Instance_1
c sat_status                            1
c initial_maximal_clause_length         5
c initial_number_of_variables           276
c initial_number_of_clauses             4668
c initial_number_of_literal_occurrences 12734
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   1360
c running_time(sec)                     0.3
c number_of_nodes                       228
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                2982
c number_of_pure_literals               0
c number_of_autarkies                   4
c number_of_missed_single_nodes         0
c max_tree_depth                        13
c proportion_searched                   1.162109e-01
c proportion_single                     0.000000e+00
c total_proportion                      0.1162109375
c number_of_table_enlargements          0
c number_of_1-autarkies                 23017

# via vi in Solution_1 the trailing "0" removed
# via vi in Instance_1.pa the initial "v" removed
> cat Instance_1.pa >> Solution_1

# checking:
> cat WSchur_pd_fullsb_5_156.cnf | ApplyPass-O3-DNDEBUG Solution_1 result_1
> tail -1 result_1
p cnf 0 0

# via vi transformed Solution_1 into a CNF with a single clause ("p cnf 390 1")
> oklib --maxima
oklib_load_all();
F : read_fcl_f("Solution_1")$
pa : subset(first(F[2]), lambda([x], is(x>0)));
I : invstandardise_pd_wschur_aloamo(5,156);
P : extract_partition(map(I,pa));
  [{1,5,14,25,32,36,42,49,52,55,58,62,65,71},
   {3,4,12,13,18,23,29,37,38,48,57,59,68,73,78},
   {2,8,9,15,21,27,28,33,34,39,40,45,46,50,51,63,64,69,70,75},
   {6,10,11,19,24,26,31,44,47,56,60,61,74,76},
   {7,16,17,20,22,30,35,41,43,53,54,66,67,72,77}]
FP : uncompresss_wschurpalindromic_subsets(156,P);
certificate_pdwschurfsb_p(5,156,FP);
  true
certificate_pdschur_p(5,156,FP);
  true
certificate_pdschurfsb_p(5,156,FP);
  false
   \endverbatim
   </li>
   <li> Open case n=158:
   \verbatim
> SplittingViaOKsolver -D70 WSchur_pd_fullsb_5_158.cnf
> cat Result
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           395
c initial_number_of_clauses             11029
c initial_number_of_literal_occurrences 32025
c number_of_initial_unit-eliminations   35
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           35
c reddiff_number_of_clauses             2233
c reddiff_number_of_literal_occurrences 6859
c number_of_2-clauses_after_reduction   1360
c running_time(sec)                     3236.8
c number_of_nodes                       620355
c number_of_quasi_single_nodes          0
c number_of_2-reductions                875
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        28
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 180581403
c splitting_directory                   SplitViaOKsolver_D70WSchur_pd_fullsb_5_158cnf_2012-08-01-175931/Instances
c splitting_cases                       310178

> ProcessSplitViaOKsolver SplitViaOKsolver_D70WSchur_pd_fullsb_5_158cnf_2012-08-01-175931
> E=read_processsplit_minisat()
15273: 4.735d, sum-cfs=1.138473e+10, mean-t=26.789s, mean-cfs=745416, sat: 0 1
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   0.15   12.26   20.12   26.79   35.53  337.60
sd= 22.5382
      95%       96%       97%       98%       99%      100%
 70.55790  76.85130  85.61704  92.01112 105.94828 337.60200
sum= 409142
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   5671  374500  591200  745400  990200 7559000
sd= 555896.2
    95%     96%     97%     98%     99%    100%
1828674 1980499 2182581 2337051 2605462 7559338
sum= 11384734269
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -3.3095e+00  2.7606e-02 -119.88 < 2.2e-16 ***
E$cfs        4.0378e-05  2.9689e-08 1360.03 < 2.2e-16 ***
R-squared: 0.9918
> E[E$sat==1,]
           i npa  d  rn   rc        t sat  cfs  dec rts     r1 mem ptime stime
14911 310168 113 23 395 4912 0.149977   1 5671 7545  30 226521  19     0  0.01
         cfl
14911 100768

> cd SplitViaOKsolver_D70WSchur_pd_fullsb_5_158cnf_2012-08-01-175931
> cat Instances/310168 > Solution_1
> cat WSchur_pd_fullsb_5_158.cnf | ApplyPass-O3-DNDEBUG Solution_1 > Instance_1
> OKsolver_2002-O3-DNDEBUG -O -F Instance_1
c sat_status                            1
c initial_maximal_clause_length         5
c initial_number_of_variables           282
c initial_number_of_clauses             4912
c initial_number_of_literal_occurrences 13413
c number_of_2-clauses_after_reduction   1416
c running_time(sec)                     7.9
c number_of_nodes                       10394
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                114162
c number_of_pure_literals               0
c number_of_autarkies                   106
c number_of_missed_single_nodes         70
c max_tree_depth                        32
c proportion_searched                   1.538773e-01
c proportion_single                     0.000000e+00
c total_proportion                      0.1538772583007812
c number_of_table_enlargements          0
c number_of_1-autarkies                 934088

# via vi in Solution_1 the trailing "0" removed
# via vi in Instance_1.pa the initial "v" removed
> cat Instance_1.pa >> Solution_1

# checking:
> cat WSchur_pd_fullsb_5_158.cnf | ApplyPass-O3-DNDEBUG Solution_1 result_1
> tail -1 result_1
p cnf 0 0

# via vi transformed Solution_1 into a CNF with a single clause ("p cnf 395 1")
> oklib --maxima
oklib_load_all();
F : read_fcl_f("Solution_1")$
pa : subset(first(F[2]), lambda([x], is(x>0)));
I : invstandardise_pd_wschur_aloamo(5,158);
P : extract_partition(map(I,pa));
  [{1,6,15,20,28,33,36,41,46,50,55,57,60,62},
   {3,8,12,13,18,35,40,44,49,54,63,64,68,69,74},
   {9,14,19,22,26,27,37,42,43,47,71,76,77,78},
   {4,5,11,17,23,24,25,31,32,38,39,45,51,58,59,65,66,72,73,79},
   {2,7,10,16,21,29,30,34,48,52,53,56,61,67,70,75}]
FP : uncompresss_wschurpalindromic_subsets(158,P);
certificate_pdwschur_p(5,158,FP);
  true
certificate_pdschur_p(5,158,FP);
  false
   \endverbatim
   </li>
  </ul>

*/
