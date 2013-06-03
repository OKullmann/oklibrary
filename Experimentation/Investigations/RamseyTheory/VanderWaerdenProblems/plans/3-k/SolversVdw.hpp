// Oliver Kullmann, 3.6.2013 (Swansea)
/* Copyright 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/SolversVdw.hpp
  \brief On SAT solvers computing vdW-numbers vdw_2(3,k) in general


  \todo Performance of OKsolver-2002
  <ul>
   <li> k=4, n=17,18: 7 resp. 9 nodes. </li>
   <li> k=5, n=21,22: 6 resp. 19 nodes. </li>
   <li> k=6, n=31,32: 24 resp. 51 nodes. </li>
   <li> k=7, n=45,46: 11 resp. 139 nodes. </li>
   <li> k=8, n=57,58: 109 resp. 479 nodes. </li>
   <li> k=9, n=76,77: 702 resp. 1591 nodes. </li>
   <li> k=10, n=96,97: 129 resp. 10545 nodes. </li>
   <li> k=11, n=113,114:
    <ol>
     <li> 37374 resp. 41563 nodes. </li>
     <li> With minisat2-preprocessor: identical results (so apparently
     the preprocessor does nothing). </li>
    </ol>
   </li>
   <li> k=12, n=134, 135 (cswsok, 3GHz):
   \verbatim
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-3-12_134.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         12
c initial_number_of_variables           134
c initial_number_of_clauses             5172
c initial_number_of_literal_occurrences 22266
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     222.1
c number_of_nodes                       283568
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                2060055
c number_of_pure_literals               35
c number_of_autarkies                   1
c number_of_missed_single_nodes         0
c max_tree_depth                        36
c proportion_searched                   9.996262e-01
c proportion_single                     0.000000e+00
c total_proportion                      0.9996261596679688
c number_of_table_enlargements          0
c number_of_1-autarkies                 516
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-12_134.cnf

# without tree-pruning: precisely the same.

> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-3-12_135.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         12
c initial_number_of_variables           135
c initial_number_of_clauses             5251
c initial_number_of_literal_occurrences 22611
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     220.8
c number_of_nodes                       281381
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                2049274
c number_of_pure_literals               29
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        36
c proportion_searched                   1.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 490
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-12_135.cnf
   \endverbatim
   </li>
   <li> k=13, n=159, 160 (cswsok, 3GHz):
   \verbatim
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-3-13_159.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         13
c initial_number_of_variables           159
c initial_number_of_clauses             7216
c initial_number_of_literal_occurrences 31398
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     2118.0
c number_of_nodes                       1590139
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                12785699
c number_of_pure_literals               455
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        43
c proportion_searched                   4.645918e-02
c proportion_single                     0.000000e+00
c total_proportion                      0.04645918309688568
c number_of_table_enlargements          0
c number_of_1-autarkies                 12836
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-13_159.cnf

> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-3-13_160.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         13
c initial_number_of_variables           160
c initial_number_of_clauses             7308
c initial_number_of_literal_occurrences 31804
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     3806.0
c number_of_nodes                       2970723
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                24138237
c number_of_pure_literals               706
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        43
c proportion_searched                   1.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 24075
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-13_160.cnf

> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-3-14_186.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         14
c initial_number_of_variables           186
c initial_number_of_clauses             9795
c initial_number_of_literal_occurrences 43014
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     47597.9
c number_of_nodes                       22470241
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                200801343
c number_of_pure_literals               2161
c number_of_autarkies                   0
c number_of_missed_single_nodes         9
c max_tree_depth                        52
c proportion_searched                   1.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 60234
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-14_186.cnf
   \endverbatim
   </li>
  </ul>


  \todo Performance of march_pl
  <ul>
   <li> k=12:
    <ol>
     <li> k=12, n=134: 28s (5567 nodes, cswsok (3GHz)) </li>
     <li> k=12, n=135: 213s (47963 nodes, cswsok (3GHz)) </li>
    </ol>
   </li>
   <li> k=13, n=160:
   \verbatim
c main():: nodeCount: 279061
c main():: dead ends in main: 552
c main():: lookAheadCount: 71404700
c main():: unitResolveCount: 1831368
c main():: time=1944.450073
c main():: necessary_assignments: 37675
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 2653564, succes #: 2089797
c main():: doublelook: overall 3.822 of all possible doublelooks executed
c main():: doublelook: succesrate: 78.754, average DL_trigger: 312.059
   \endverbatim
   </li>
  </ul>


  \todo Performance of satz215
  <ul>
   <li> k=12:
    <ol>
     <li> k=12, n=134: 73s (NB_BRANCHE= 240158, cswsok (3 GHz)) </li>
     <li> k=12, n=135: 79s (NB_BRANCHE= 262304, cswsok (3 GHz)) </li>
    </ol>
   </li>
   <li> k=13, n=160 (cswsok, 3GHz):
   \verbatim
> satz215 VanDerWaerden_2-3-13_160.cnf
**** The instance is unsatisfiable. *****
NB_MONO= 363, NB_UNIT= 39344343, NB_BRANCHE= 1698185, NB_BACK= 863252
Program terminated in 711.470 seconds.
satz215 VanDerWaerden_2-3-13_160.cnf 711.470 1698185 863252 219047856 8567070 0 160 7308 0 4263617 1867228
   \endverbatim
   Compared to OKsolver (see above) we have only 1,698,185 nodes (versus
   2,970,723), and we have 8,567,070 r_2_reductions (versus 24,138,237), and
   1,867,228 r_3-reductions. The OKsolver needs 5-times more time. We need to
   understand this difference! Is the satz-heuristic here better?! Or are the
   r_3-reductions here of key importance?
   </li>
   <li> k=14, n=186 (cswsok, 3GHz):
   \verbatim
> satz215 VanDerWaerden_2-3-14_186.cnf
NB_MONO= 1435, NB_UNIT= 277867725, NB_BRANCHE= 10822316, NB_BACK= 5500793
Program terminated in 6233.370 seconds.
satz215 VanDerWaerden_2-3-14_186.cnf 6233.370 10822316 5500793 1573930613 58462301 0 186 9795 0 29371201 12582731
   \endverbatim
   </li>
   <li> k=15, n=218 (cswsok, 3GHz):
   \verbatim
> satz215 VanDerWaerden_2-3-15_218.cnf
NB_MONO= 3848, NB_UNIT= 1902278418, NB_BRANCHE= 66595028, NB_BACK= 33775013
Program terminated in 54913.090 seconds.
satz215 VanDerWaerden_2-3-15_218.cnf 54913.090 66595028 33775013 11030537203 387523942 0 218 13362 0 314567493 92988051
   \endverbatim
   (that's 15.3 hours). A lot of r2-reductions (387523942) and r3-reductions
   (92988051).
   </li>
   <li> k=16, n=238 (cswsok, 3GHz):
   \verbatim
> satz215 VanDerWaerden_2-3-16_238.cnf
NB_MONO= 87981, NB_UNIT= 17845374254, NB_BRANCHE= 599520428, NB_BACK= 304003189
Program terminated in 562161.320 seconds.
satz215 VanDerWaerden_2-3-16_238.cnf 562161.320 599520428 304003189 101411640715 3643082746 0 238 15812 0 3402007271 917339083
   \endverbatim
   (that's 156 hours).
   </li>
   <li> SplittingViaOKsolver:
   \verbatim
> solver="satz215" ProcessSplitViaOKsolver SplitViaOKsolver_D30VanDerWaerden_2315_218cnf_2013-05-17-015710
# aborted after 2071 from 12922 sub-instances:
> E=read_processsplit_satz()
2071: 21.955h, sum-nds=1.363216e+08, mean-t=38.165s, mean-nds=65824, sat: 0
$t:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
   0.120    0.890    1.420   38.160    2.305 6503.000
sd= 339.4337
     95%      96%      97%      98%      99%     100%
   4.845    5.630    6.979   39.830 1316.095 6503.010
sum= 79038.94
$nds:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
      34     1438     2465    65820     4323 11620000
sd= 586093.9
       95%        96%        97%        98%        99%       100%
    9509.0    11161.4    14417.8    83698.2  2248985.2 11621345.0
sum= 136321627
$t ~ $nds:
              Estimate Std. Error   t value Pr(>|t|)
(Intercept) 6.2299e-02 2.3939e-01    0.2602   0.7947
E$nds       5.7885e-04 4.0600e-07 1425.7567   <2e-16 ***
R-squared: 0.999
   \endverbatim
   That is cleary worse than just plain satz215. </li>
  </ul>


  \todo Tanbir Ahmed's solver
  <ul>
   <li> On cswsok (3 GHz):
   \verbatim
for F in *.cnf; do echo $F; ./ta_solver75 $F | tee -a Output_ta75; done

VanDerWaerden_2-3-12_134.cnf
N_UNITS: 20977611, N_BRANCHES: 1721206, N_BACK: 860593
Running time: 44.09 seconds

VanDerWaerden_2-3-12_135.cnf
N_UNITS: 21978048, N_BRANCHES: 1790733, N_BACK: 895366
Running time: 46.64 seconds

VanDerWaerden_2-3-13_159.cnf
N_UNITS: 821693, N_BRANCHES: 65615, N_BACK: 32796
Running time: 2.01 seconds

VanDerWaerden_2-3-13_160.cnf
N_UNITS: 180658356, N_BRANCHES: 13722975, N_BACK: 6861487
Running time: 462.84 seconds

VanDerWaerden_2-3-14_185.cnf
N_UNITS: 1002639318, N_BRANCHES: 69102886, N_BACK: 34551429
Running time: 3030.05 seconds

VanDerWaerden_2-3-14_186.cnf
N_UNITS: 1498228640, N_BRANCHES: 102268511, N_BACK: 51134255
Running time: 4576.51 seconds

VanDerWaerden_2-3-15_217.cnf
N_UNITS: 4924868287, N_BRANCHES: 309583287, N_BACK: 154791621
Running time: 18378.29 seconds

VanDerWaerden_2-3-15_218.cnf
N_UNITS: 12442138772, N_BRANCHES: 774872707, N_BACK: 387436353
Running time: 47005.83 seconds

VanDerWaerden_2-3-16_238.cnf
N_UNITS: 134192977183, N_BRANCHES: 8120609615, N_BACK: 4060304807
Running time: 532416.20 seconds
   \endverbatim
   </li>
   <li> This solver is the fastest single solver, but nearly 7x slower than
   Cube&Conquer via OKsolver&minisat22. </li>
   <li> We should also try C&C with this solver. </li>
  </ul>


  \todo Performance of conflict-driven solvers
  <ul>
   <li> minisat2:
    <ol>
     <li> k=12, n=134: 1387192 conflicts, 22 restarts (csltok; 598s). </li>
     <li> k=12, n=135: 1454696 conflicts, 22 restarts (csltok; 608s). </li>
     <li> k=13, n=159: 525039 conflicts,  20 restarts (csltok; 263s). </li>
     <li> k=13, n=160: 9298288 conflicts, 27 restarts (csltok; 7019s). </li>
    </ol>
   </li>
   <li> minisat-2.2.0:
    <ol>
     <li> k=12, n=134: 61s (3605914 conflicts; cswsok (3GHz)) </li>
     <li> k=12, n=135: 107s (5963349 conflicts; cswsok (3GHz)) </li>
     <li> k=13, n=159: 13s (701558 conflicts; cswsok (3GHz)) </li>
     <li> k=13, n=160: 1716s (63901998 conflicts; cswsok (3GHz)) </li>
     <li> k=14, n=185: 147s (5619881 conflicts; cswsok (3GHz)) </li>
     <li> k=14, n=186: 16836s (463984635 conflicts; cswsok (3GHz)) </li>
     <li> k=15, n=217: 26319s (500756444 conflicts; cswsok (3GHz)) </li>
     <li> k=15, n=218: 190211s (3205639994 conflicts; cswsok (3GHz)) </li>
     <li> SplittingViaOKsolver (cswsok, 3 GHz):
     \verbatim
> SplittingViaOKsolver -D20 VanDerWaerden_2-3-13_160.cnf
> cd SplitViaOKsolver_D20VanDerWaerden_2313_160cnf_2013-05-17-001649
> more Md5sum
16c1083271af4be87d875298d869a6c7
> more Statistics
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  20.00   20.00   23.00   23.23   25.00   37.00
> table(E$n)
 20  21  22  23  24  25  26  27  28  29  30  31  32  34  37
409 111 125 196 233 201 151  96  52  12   2   2   4   2   3
> summary(E$d)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   5.00   12.00   13.00   13.21   15.00   18.00
> table(E$d)
  5   6   7   8   9  10  11  12  13  14  15  16  17  18
  3  10  28  40  61  86 138 193 264 240 217 195 112  12
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         13
c initial_number_of_variables           160
c initial_number_of_clauses             7308
c initial_number_of_literal_occurrences 31804
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     9.7
c number_of_nodes                       3197
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                110
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        18
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-13_160.cnf
c splitting_directory                   SplitViaOKsolver_D20VanDerWaerden_2313_160cnf_2013-05-17-001649/Insta
nces
c splitting_cases                       1599

> cd ..
> ProcessSplitViaOKsolver SplitViaOKsolver_D20VanDerWaerden_2313_160cnf_2013-05-17-001649
> cat Process_SplitViaOKsolver_D20VanDerWaerden_2313_160cnf_2013-05-17-001649_2013-05-17-002115/Result
  2:35

> SplittingViaOKsolver -D30 VanDerWaerden_2-3-14_186.cnf
c running_time(sec)                     145.7
c splitting_cases                       13527
> ProcessSplitViaOKsolver SplitViaOKsolver_D30VanDerWaerden_2314_186cnf_2013-05-17-002903/
> cat Process_SplitViaOKsolver_D30VanDerWaerden_2314_186cnf_2013-05-17-002903_2013-05-17-003312/Result
  22:08

> SplittingViaOKsolver -D30 VanDerWaerden_2-3-15_218.cnf
c running_time(sec)                     231.8
c splitting_cases                       12922
> ProcessSplitViaOKsolver SplitViaOKsolver_D30VanDerWaerden_2315_218cnf_2013-05-17-015710
> cat Process_SplitViaOKsolver_D30VanDerWaerden_2315_218cnf_2013-05-17-015710_2013-05-17-020436/Result
  141:27
> E=read_processsplit_minisat()
12922: 2.226h, sum-cfs=3.806417e+08, mean-t=0.620s, mean-cfs=29457, sat: 0
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
 0.0150  0.2320  0.3969  0.6203  0.7576 15.5700
sd= 0.6646329
      95%       96%       97%       98%       99%      100%
 1.866720  2.026010  2.252030  2.599404  3.274080 15.569600
sum= 8015.144
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
    408   11500   19900   29460   36550  636800
sd= 29454.45
      95%       96%       97%       98%       99%      100%
 85033.00  92782.64 102557.00 117306.90 145427.27 636785.00
sum= 380641711
$t ~ $cfs:
               Estimate  Std. Error  t value  Pr(>|t|)
(Intercept) -4.3157e-02  5.0876e-04  -84.827 < 2.2e-16 ***
E$cfs        2.2522e-05  1.2214e-08 1844.020 < 2.2e-16 ***
R-squared: 0.9962

> SplittingViaOKsolver -D35 VanDerWaerden_2-3-15_218.cnf
c running_time(sec)                     821.3
c splitting_cases                       32331
> ProcessSplitViaOKsolver SplitViaOKsolver_D35VanDerWaerden_2315_218cnf_2013-05-17-090420
> cat Process_SplitViaOKsolver_D35VanDerWaerden_2315_218cnf_2013-05-17-090420_2013-05-17-092127/Result
  134:31
> E=read_processsplit_minisat()
32331: 1.913h, sum-cfs=3.615115e+08, mean-t=0.213s, mean-cfs=11182, sat: 0
$t:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
0.005999 0.089990 0.158000 0.213100 0.274000 3.906000
sd= 0.1911241
     95%      96%      97%      98%      99%     100%
0.574912 0.619905 0.676897 0.752885 0.910861 3.906410
sum= 6888.165
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
     41    4755    8441   11180   14400  180100
sd= 9784.68
     95%      96%      97%      98%      99%     100%
 29808.0  32137.8  34737.2  38804.8  46902.0 180053.0
sum= 361511501
$t ~ $cfs:
               Estimate  Std. Error  t value  Pr(>|t|)
(Intercept) -4.8847e-03  1.0623e-04  -45.985 < 2.2e-16 ***
E$cfs        1.9491e-05  7.1493e-09 2726.231 < 2.2e-16 ***
R-squared: 0.9957

> SplittingViaOKsolver -D40 VanDerWaerden_2-3-16_238.cnf
c running_time(sec)                     3247.6
c splitting_cases                       104797
> ProcessSplitViaOKsolver SplitViaOKsolver_D40VanDerWaerden_2316_238cnf_2013-05-17-140911
> E=read_processsplit_minisat()
104797: 22.238h, sum-cfs=3.723995e+09, mean-t=0.764s, mean-cfs=35535, sat: 0
$t:
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
 0.009998  0.266000  0.459900  0.763900  0.853900 29.560000
sd= 0.9925851
      95%       96%       97%       98%       99%      100%
 2.414630  2.746580  3.184759  3.804420  4.983320 29.558500
sum= 80056.43
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
     71   12830   22290   35540   40960 1128000
sd= 42953.37
      95%       96%       97%       98%       99%      100%
 110594.8  124136.6  142799.4  167161.1  218275.0 1127634.0
sum= 3723995162
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -5.5472e-02  2.6144e-04 -212.18 < 2.2e-16 ***
E$cfs        2.3059e-05  4.6897e-09 4916.80 < 2.2e-16 ***
R-squared: 0.9957

> SplittingViaOKsolver -D50 VanDerWaerden_2-3-17_279.cnf
c running_time(sec)                     23546.1
c number_of_nodes                       1399505
c number_of_2-reductions                82906
c max_tree_depth                        41
c splitting_cases                       699751
> ProcessSplitViaOKsolver SplitViaOKsolver_D50VanDerWaerden_2317_279cnf_2013-05-18-154809
> cat Process_SplitViaOKsolver_D50VanDerWaerden_2317_279cnf_2013-05-18-154809_2013-05-19-084513/Result
  17439:22
> E=read_processsplit_minisat()
699751: 11.652d, sum-cfs=3.593149e+10, mean-t=1.439s, mean-cfs=51349, sat: 0
$t:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
  0.0100   0.4569   0.8199   1.4390   1.5770 199.0000
sd= 2.289549
      95%       96%       97%       98%       99%      100%
  4.56531   5.16921   5.95509   7.39288  10.27740 199.02700
sum= 1006718
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
     31   17740   30880   51350   58130 4782000
sd= 71854.53
      95%       96%       97%       98%       99%      100%
 158454.0  177082.0  203221.5  249985.0  335532.0 4781946.0
sum= 35931491146
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -1.8913e-01  3.3949e-04  -557.1 < 2.2e-16 ***
E$cfs        3.1701e-05  3.8441e-09  8246.8 < 2.2e-16 ***
R-squared: 0.9898
     \endverbatim
     A big speed-up! </li>
    </ol>
   </li>
   <li> cryptominisat-2.9.0:
    <ol>
     <li> k=12, n=134: 91s (619145 conflicts; csltok) </li>
     <li> k=12, n=135: 763s (2815643 conflicts; csltok) </li>
    </ol>
   </li>
   <li> cryptominisat-2.9.6:
    <ol>
     <li> k=12, n=134: 155s (1693268 conflicts; cswsok (3 GHz)) </li>
     <li> k=12, n=135: 212s (2109106 conflicts; cswsok (3 GHz)) </li>
    </ol>
   </li>
   <li> picosat913:
    <ol>
     <li> k=12, n=134: 7s (368890 conflicts; cswsok (3 GHz)) </li>
     <li> k=12, n=135: 259s (9643671 conflicts; cswsok (3 GHz)) </li>
    </ol>
   </li>
   <li> precosat236:
    <ol>
     <li> k=12, n=134: 52s (1145491 conflicts; cswsok (3 GHz)) </li>
     <li> k=12, n=135: 205s (3583785 conflicts; cswsok (3 GHz)) </li>
    </ol>
   </li>
   <li> precosat-570.1:
    <ol>
     <li> k=12, n=134: 91s (1531799 conflicts; cswsok (3 GHz)) </li>
     <li> k=12, n=135: 211s (2425722 conflicts; cswsok (3 GHz)) </li>
    </ol>
   </li>
   <li> lingelingala-b02aa1a-121013:
    <ol>
     <li> k=12, n=134: 171s (1659607 conflicts; cswsok (3 GHz)) </li>
     <li> k=12, n=135: 476s (3435610 conflicts; cswsok (3 GHz)) </li>
    </ol>
   </li>
   <li> glucose-1.0:
    <ol>
     <li> k=12, n=134: 39s (425399 conflicts; csltok) </li>
     <li> k=12, n=135: 191s (1356325 conflicts; csltok) </li>
     <li> k=13, n=159: 139s (957255 conflicts; csltok) </li>
     <li> k=13, n=160: 3274s (9907932 conflicts; csltok) </li>
    </ol>
   </li>
   <li> glucose-2.0:
    <ol>
     <li> k=12, n=134: 5s (169420 conflicts; cswsok (3 GHz)) </li>
     <li> k=12, n=135: 58s (1263087 conflicts; cswsok (3 GHz)) </li>
     <li> k=13, n=159: 1s (50528 conflicts; cswsok (3 GHz)) </li>
     <li> k=13, n=160: 781s (8377487 conflicts; cswsok (3 GHz)) </li>
     <li> k=14, n=185: 5133s (31516583 conflicts; cswsok (3 GHz)) </li>
     <li> k=14, n=186: 84334s (163500051 conflicts; cswsok (3 GHz)) </li>
     <li> SplittingViaOKsolver (cswsok, 3 GHz):
     \verbatim
> solver="glucose-2.0" ProcessSplitViaOKsolver SplitViaOKsolver_D30VanDerWaerden_2315_218cnf_2013-05-17-015710
> cat Process_SplitViaOKsolver_D30VanDerWaerden_2315_218cnf_2013-05-17-015710_2013-05-17-180553/Result
  187:11
     \endverbatim
     Reasonable fast, but minisat-2.2.0 is faster.
     </li>
    </ol>
   </li>
   <li> glucose-2.2:
    <ol>
     <li> k=12, n=134: 84s (1350204 conflicts; cswsok (3 GHz)) </li>
     <li> k=12, n=135: 94s (1444017 conflicts; cswsok (3 GHz)) </li>
     <li> k=13, n=159: 87s (1332829 conflicts; cswsok (3 GHz)) </li>
     <li> k=13, n=160: 1412s (10447051 conflicts; cswsok (3 GHz)) </li>
     <li> SplittingViaOKsolver (cswsok, 3 GHz):
     \verbatim
> solver="glucose-2.2" ProcessSplitViaOKsolver SplitViaOKsolver_D30VanDerWaerden_2315_218cnf_2013-05-17-015710
> cat Process_SplitViaOKsolver_D30VanDerWaerden_2315_218cnf_2013-05-17-015710_2013-05-19-084751/Result
  132:00

> solver="glucose-2.2" ProcessSplitViaOKsolver SplitViaOKsolver_D40VanDerWaerden_2316_238cnf_2013-05-17-140911
  1288:01
     \endverbatim
     This is now faster than glucose-2.0, but basically the same as
     minisat-2.2.0 (for k=16 a bit faster).
     </li>
    </ol>
    Performance much worse -- apparently due to much more restarts!
   </li>
   <li> minisat-2.2.0 and glucose-2.0 seem best (for the conflict-driven
   solvers, while satz215 seems best overall); however SplittingViaOKsolver
   with minisat-2.2 is much better than satz215. </li>
  </ul>

*/
