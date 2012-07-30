// Oliver Kullmann, 19.7.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/SchurProblems/plans/Schur4/general.hpp
  \brief On schur(4) and variations


  \todo Best method for determining schur(4) = 45
  <ul>
   <li> On the unsatisfiable Schur-instances according to
   http://www.di.univaq.it/~formisano/CLPASP/node3.html relsat is fastest,
   while on the satisfiable instances simo is fastest. </li>
   <li> OKsolver_2002 performance looks reasonable (direct encoding, without
   and with symmetry-breaking):
   \verbatim
> OKsolver_2002-O3-DNDEBUG Schur_4_44.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         4
c initial_number_of_variables           176
c initial_number_of_clauses             2188
c initial_number_of_literal_occurrences 6256
c number_of_2-clauses_after_reduction   352
c running_time(sec)                     0.0
c number_of_nodes                       245
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1424
c number_of_pure_literals               0
c number_of_autarkies                   2
c number_of_missed_single_nodes         0
c max_tree_depth                        18
c proportion_searched                   1.550293e-02
c proportion_single                     0.000000e+00
c total_proportion                      0.0155029296875
c number_of_table_enlargements          0
c number_of_1-autarkies                 14851

> OKsolver_2002-O3-DNDEBUG Schur_4_45.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         4
c initial_number_of_variables           180
c initial_number_of_clauses             2279
c initial_number_of_literal_occurrences 6524
c number_of_2-clauses_after_reduction   358
c running_time(sec)                     90.7 (cs-oksvr)
c running_time(sec)                     65.6 (csltok)
c number_of_nodes                       682348
c number_of_single_nodes                5735
c number_of_quasi_single_nodes          0
c number_of_2-reductions                3073787
c number_of_pure_literals               0
c number_of_autarkies                   22464
c number_of_missed_single_nodes         13870
c max_tree_depth                        36
c proportion_searched                   9.997743e-01
c proportion_single                     2.257211e-04
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 33816234

> OKsolver_2002-O3-DNDEBUG Schur_sb_4_45.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         4
c initial_number_of_variables           180
c initial_number_of_clauses             2282
c initial_number_of_literal_occurrences 6527
c number_of_initial_unit-eliminations   5
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           5
c reddiff_number_of_clauses             184
c reddiff_number_of_literal_occurrences 576
c number_of_2-clauses_after_reduction   386
c running_time(sec)                     16.4
c number_of_nodes                       170543
c number_of_single_nodes                1420
c number_of_quasi_single_nodes          0
c number_of_2-reductions                766490
c number_of_pure_literals               0
c number_of_autarkies                   5589
c number_of_missed_single_nodes         3473
c max_tree_depth                        33
c proportion_searched                   9.998047e-01
c proportion_single                     1.953030e-04
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 8386016
   \endverbatim
   </li>
   <li> satz:
   \verbatim
> satz215 Schur_4_44.cnf
**** The instance is satisfiable. *****
NB_MONO= 0, NB_UNIT= 4267, NB_BRANCHE= 204, NB_BACK= 96
Program terminated in 0.010 seconds.
satz215 Schur_4_44.cnf 0.010 204 96 16000 452 1 176 2188 0 267 88

> satz215 Schur_4_45.cnf
**** The instance is unsatisfiable. *****
NB_MONO= 0, NB_UNIT= 10689225, NB_BRANCHE= 432553, NB_BACK= 219567
Program terminated in 36.770 seconds.
satz215 Schur_4_45.cnf 36.770 432553 219567 37340648 1381151 0 180 2279 0 1138582 328260

> satz215 Schur_sb_4_45.cnf
**** The instance is unsatisfiable. *****
NB_MONO= 0, NB_UNIT= 2762779, NB_BRANCHE= 111957, NB_BACK= 56817
Program terminated in 9.290 seconds.
satz215 Schur_sb_4_45.cnf 9.290 111957 56817 9542105 354489 0 180 2282 -204 287189 82420
   \endverbatim
   </li>
   <li> march_pl:
   \verbatim
> march_pl Schur_4_45.cnf
c main():: nodeCount: 88863
c main():: dead ends in main: 2888
c main():: lookAheadCount: 19838572
c main():: unitResolveCount: 414098
c main():: time=36.160000
c main():: necessary_assignments: 15926
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 363057, succes #: 287974
c main():: doublelook: overall 1.865 of all possible doublelooks executed
c main():: doublelook: succesrate: 79.319, average DL_trigger: 83.071
s UNSATISFIABLE

> march_pl Schur_sb_4_45.cnf
c main():: nodeCount: 23940
c main():: dead ends in main: 1013
c main():: lookAheadCount: 5146096
c main():: unitResolveCount: 110786
c main():: time=9.080000
c main():: necessary_assignments: 4795
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 91007, succes #: 71949
c main():: doublelook: overall 1.803 of all possible doublelooks executed
c main():: doublelook: succesrate: 79.059, average DL_trigger: 80.526
s UNSATISFIABLE
   \endverbatim
   Satisfiability of Schur_5_119.cnf established with 65945 nodes (in
   ~ 204s). </li>
   <li> minisat-2.2.0: Satisfiability of Schur_4_44.cnf is also trivial, but
   unsatisfiability of Schur_4_45.cnf is more problematic, needing 7091007
   conflicts with minisat2 (26 restarts, 290s):
   \verbatim
> minisat-2.2.0 Schur_4_44.cnf
restarts              : 15
conflicts             : 2722           (61873 /sec)
decisions             : 3559           (0.00 % random) (80899 /sec)
propagations          : 61440          (1396586 /sec)
conflict literals     : 55664          (18.11 % deleted)
Memory used           : 18.00 MB
CPU time              : 0.043993 s
SATISFIABLE
> minisat-2.2.0 Schur_4_45.cnf
restarts              : 40958
conflicts             : 29623831       (43421 /sec)
decisions             : 35334483       (0.00 % random) (51792 /sec)
propagations          : 616192724      (903192 /sec)
conflict literals     : 534085154      (24.50 % deleted)
Memory used           : 23.00 MB
CPU time              : 682.239 s

> minisat-2.2.0 Schur_sb_4_45.cnf
restarts              : 8191
conflicts             : 5128838        (68515 /sec)
decisions             : 6094199        (0.00 % random) (81412 /sec)
propagations          : 109527967      (1463170 /sec)
conflict literals     : 83911520       (25.26 % deleted)
Memory used           : 19.00 MB
CPU time              : 74.8566 s
   \endverbatim
   So apparently minisat2 is not very strong here. </li>
   <li> picosat and precosat:
   \verbatm
> picosat913 Schur_4_45.cnf
s UNSATISFIABLE
c 16 iterations
c 183 restarts
c 2 failed literals
c 3939604 conflicts
c 4599971 decisions
c 134 fixed variables
c 56552941 learned literals
c 34.4% deleted literals
c 140582244 propagations
c 100.0% variables used
c 70.0 seconds in library
c 2.0 megaprops/second
c 7 simplifications
c 1089 reductions
c 290.6 MB recycled
c 1.0 MB maximally allocated
c 70.0 seconds total run time

> picosat913 Schur_sb_4_45.cnf
s UNSATISFIABLE
c 20 iterations
c 41 restarts
c 1 failed literals
c 876447 conflicts
c 1021412 decisions
c 78 fixed variables
c 11788666 learned literals
c 34.3% deleted literals
c 31317156 propagations
c 100.0% variables used
c 13.6 seconds in library
c 2.3 megaprops/second
c 10 simplifications
c 356 reductions
c 61.4 MB recycled
c 0.6 MB maximally allocated
c 13.6 seconds total run time

> precosat-570.1 -v Schur_4_45.cnf
s UNSATISFIABLE
c 5413847 conflicts, 7295562 decisions, 3706 random
c 8 iterations, 52530 restarts, 15262 skipped
c 100 enlarged, 1 shrunken, 52946 rescored, 1130 rebiased
c 2 simplifications, 100 reductions
c 1430.2 seconds, 28 MB max, 525 MB recycled
   \endverbatim
   </li>
   <li> Cryptominisat and other minisat-variations:
   \verbatim
> cryptominisat Schur_4_45.cnf
c conflicts                : 2352049     (5950.63   / sec)
c decisions                : 2667152     (0.21      % random)
c bogo-props               : 61452792834 (155474077.04 / sec)
c conflict literals        : 35048160    (41.54     % deleted)
c Memory used              : 74.22       MB
c CPU time                 : 395.26      s
s UNSATISFIABLE

> glucose-2.0 Schur_4_45.cnf
c conflicts             : 2530673        (10963 /sec)
c decisions             : 2844563        (0.00 % random) (12322 /sec)
c propagations          : 63780315       (276289 /sec)
c conflict literals     : 39237403       (38.63 % deleted)
c nb reduced Clauses    : 11216
c CPU time              : 230.846 s
   \endverbatim
   </li>
   <li> The best conflict-driven solver seems picosat, which still is slower
   than the worst lookahead-solver (OKsolver-2002). </li>
   <li> Fully symmetry-breaking trivialises it, and acts satisfiability-
   equivalent here (all times csltok):
   \verbatim
> OKsolver_2002-O3-DNDEBUG Schur_fullsb_4_44.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         4
c initial_number_of_variables           176
c initial_number_of_clauses             2200
c initial_number_of_literal_occurrences 6268
c number_of_initial_unit-eliminations   20
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           20
c reddiff_number_of_clauses             625
c reddiff_number_of_literal_occurrences 1933
c number_of_2-clauses_after_reduction   426
c running_time(sec)                     0.1
c number_of_nodes                       745
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                4018
c number_of_pure_literals               0
c number_of_autarkies                   64
c number_of_missed_single_nodes         9
c max_tree_depth                        23
c proportion_searched                   2.265625e-01
c proportion_single                     0.000000e+00
c total_proportion                      0.2265625

> OKsolver_2002-O3-DNDEBUG Schur_fullsb_4_45.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         4
c initial_number_of_variables           180
c initial_number_of_clauses             2291
c initial_number_of_literal_occurrences 6536
c number_of_initial_unit-eliminations   20
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           20
c reddiff_number_of_clauses             641
c reddiff_number_of_literal_occurrences 1985
c number_of_2-clauses_after_reduction   436
c running_time(sec)                     0.5
c number_of_nodes                       4136
c number_of_single_nodes                3
c number_of_quasi_single_nodes          0
c number_of_2-reductions                22402
c number_of_pure_literals               0
c number_of_autarkies                   306
c number_of_missed_single_nodes         64
c max_tree_depth                        28
c proportion_searched                   9.999361e-01
c proportion_single                     6.389618e-05
c total_proportion                      1

> satz215 Schur_fullsb_4_44.cnf
Program terminated in 0.030 seconds.

> satz215 Schur_fullsb_4_45.cnf
Program terminated in 0.270 seconds.

> march_pl Schur_fullsb_4_44.cnf
c main():: time=0.040000

> march_pl Schur_fullsb_4_45.cnf
c main():: time=0.380000

> minisat-2.2.0 Schur_fullsb_4_44.cnf
CPU time              : 0.174973 s

> minisat-2.2.0 Schur_fullsb_4_45.cnf
CPU time              : 0.32395 s

> picosat913 Schur_fullsb_4_44.cnf
c 0.1 seconds total run time

> picosat913 Schur_fullsb_4_45.cnf
c 0.3 seconds total run time

> glucose-2.0 Schur_fullsb_4_44.cnf
c CPU time              : 0.215967 s

> glucose-2.0 Schur_fullsb_4_45.cnf
c CPU time              : 1.02984 s
   \endverbatim
   </li>
  </ul>


  \todo Best method for determining wschur(4) = 67
  <ul>
   <li> A good (single) solver is march_pl:
   \verbatim
> march_pl WSchur_4_66.cnf
c main():: nodeCount: 392890
c main():: dead ends in main: 312397
c main():: lookAheadCount: 21379659
c main():: unitResolveCount: 1325480
c main():: time=70.660004
c main():: necessary_assignments: 12995
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 331181, succes #: 273112
c main():: doublelook: overall 1.577 of all possible doublelooks executed
c main():: doublelook: succesrate: 82.466, average DL_trigger: 80.486
c main():: SOLUTION VERIFIED :-)
s SATISFIABLE

> march_pl WSchur_4_67.cnf
c main():: nodeCount: 412143413
c main():: dead ends in main: 331744670
c main():: lookAheadCount: -1561532857
c main():: unitResolveCount: 1429444613
c main():: time=52775.781250
c main():: necessary_assignments: 4381778
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 170379894, succes #: 114466474
c main():: doublelook: overall 0.873 of all possible doublelooks executed
c main():: doublelook: succesrate: 67.183, average DL_trigger: 30.507
s UNSATISFIABLE
   \endverbatim
   What makes it better than the OKsolver-2002: the partial use of r_3 (hard
   to believe), the preprocessing, or the heuristics (or a combination)?
   </li>
   <li> satz:
   \verbatim
> satz215 WSchur_4_66.cnf
**** The instance is satisfiable. *****
NB_MONO= 0, NB_UNIT= 209956681, NB_BRANCHE= 6682105, NB_BACK= 3386332
Program terminated in 1283.070 seconds.
satz215 WSchur_4_66.cnf 1283.070 6682105 3386332 882747460 29052556 1 264 4686 0 27717730 8154543
> satz215 WSchur_4_67.cnf
**** The instance is unsatisfiable. *****
NB_MONO= 0, NB_UNIT= 1621052683, NB_BRANCHE= 51091114, NB_BACK= 25888442
Program terminated in 10105.670 seconds.
satz215 WSchur_4_67.cnf 10105.670 51091114 25888442 6862605102 229452858 0 268 4825 0 255142733 66695431
   \endverbatim
   Even better than march_pl: so preprocessing seems to play no role. </li>
   <li> picosat:
   \verbatim
> picosat913 WSchur_4_66.cnf
s SATISFIABLE
c 0 iterations
c 15993 restarts
c 0 failed literals
c 10099961 conflicts
c 12216171 decisions
c 0 fixed variables
c 221450542 learned literals
c 25.3% deleted literals
c 485356254 propagations
c 100.0% variables used
c 489.9 seconds in library
c 1.0 megaprops/second
c 1 simplifications
c 1008 reductions
c 1036.0 MB recycled
c 3.3 MB maximally allocated
c 489.9 seconds total run time

> picosat913 WSchur_4_67.cnf
# aborted after more than a day
   \endverbatim
   </li>
   <li> OKsolver-2002 with direct encoding can't solve the problems in
   reasonable time:
   \verbatim
> OKsolver_2002-O3-DNDEBUG WSchur_4_66.cnf
UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         4
c initial_number_of_variables           264
c initial_number_of_clauses             4686
c initial_number_of_literal_occurrences 13728
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   396
c running_time(sec)                     155582.9
c number_of_nodes                       726931883
c number_of_single_nodes                81741642
c number_of_quasi_single_nodes          0
c number_of_2-reductions                4495336218
c number_of_pure_literals               0
c number_of_autarkies                   27680966
c number_of_missed_single_nodes         28194741
c max_tree_depth                        69
c proportion_searched                   1.321801e-01
c proportion_single                     2.146691e-05
c total_proportion                      0.1322015809900776
c number_of_table_enlargements          0
c number_of_1-autarkies                 42722583942
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             WSchur_4_66.cnf

> OKsolver_2002-O3-DNDEBUG WSchur_4_67.cnf
UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         4
c initial_number_of_variables           268
c initial_number_of_clauses             4825
c initial_number_of_literal_occurrences 14140
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   402
c running_time(sec)                     156905.2
c number_of_nodes                       703978392
c number_of_single_nodes                74796092
c number_of_quasi_single_nodes          0
c number_of_2-reductions                4395754877
c number_of_pure_literals               0
c number_of_autarkies                   20622889
c number_of_missed_single_nodes         23730621
c max_tree_depth                        68
c proportion_searched                   1.279120e-01
c proportion_single                     1.759440e-05
c total_proportion                      0.1279295635676844
c number_of_table_enlargements          0
c number_of_1-autarkies                 42263717955
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             WSchur_4_67.cnf
   \endverbatim
   Around that proportation of solved assignments the solver got stuck, and
   made only progress in the fourth digit (so it could take a long time).
   </li>
   <li> minisat-2.2.0 can't solve the unsatisfiable problem in reasonable time:
   \verbatim
> minisat-2.2.0 WSchur_4_66.cnf
restarts              : 13820
conflicts             : 8510770        (28710 /sec)
decisions             : 10389224       (0.00 % random) (35046 /sec)
propagations          : 236234619      (796899 /sec)
conflict literals     : 221361213      (16.45 % deleted)
Memory used           : 32.00 MB
CPU time              : 296.443 s

> minisat-2.2.0 WSchur_4_67.cnf
restarts              : 1515514
conflicts             : 1454227363     (13776 /sec)
decisions             : 1749514948     (0.00 % random) (16574 /sec)
propagations          : 38257521750    (362428 /sec)
conflict literals     : 37443253041    (18.99 % deleted)
Memory used           : 108.00 MB
CPU time              : 105559 s
   \endverbatim
   </li>
   <li> SplittingViaOKsolver (using minisat-2.2.0):
   \verbatim
> SplittingViaOKsolver -D40 WSchur_4_66.cnf
> cat SplitViaOKsolver_D40WSchur_4_66cnf_2012-07-19-192843/Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         4
c initial_number_of_variables           264
c initial_number_of_clauses             4686
c initial_number_of_literal_occurrences 13728
c number_of_2-clauses_after_reduction   396
c running_time(sec)                     271.6
c number_of_nodes                       201407
c number_of_quasi_single_nodes          0
c number_of_2-reductions                696
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        24
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 45197627
c splitting_cases                       100704
> cat Md5sum
080dbcd2783899d98b45e00974ed18cf
> cat Statistics
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  40.00   40.00   42.00   42.31   44.00   49.00
> table(E$n)
   40    41    42    43    44    45    46    47    48    49
28272 11064 13416 18096 16776  6264  5520  1248    24    24
> summary(E$d)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  10.00   17.00   18.00   18.13   20.00   24.00
> table(E$d)
   10    11    12    13    14    15    16    17    18    19    20    21    22
   11    60   339   982  2637  5922 10812 16169 19305 18720 14018  7943  3026
   23    24
  680    80


> ProcessSplitViaOKsolver SplitViaOKsolver_D40WSchur_4_66cnf_2012-07-19-192843
> cat Result
SATISFIABLE
Found 24 satisfiable sub-instances.
  PID TTY      STAT   TIME COMMAND
27401 pts/2    R+   308:05 /bin/bash /home/csoliver/OKplatform/bin/ProcessSplitViaOKsolver SplitViaOKsolver_D40WSchur_4_66cnf_2012-07-19-192843
> cat SatisfyingAssignments
48875 7425
48876 7429
49205 9447
49206 9451
49535 11469
49536 11473
52229 32601
52230 32605
52559 34623
52560 34627
52889 36645
52890 36649
55583 57777
55584 57781
55913 59799
55914 59803
56243 61821
56244 61825
58937 82953
58938 82957
59267 84975
59268 84979
59597 86997
59598 87001
> E=read_processsplit_minisat()
100704: 3.813h, sum-cfs=8.768916e+08, mean-t=0.136s, mean-cfs=8708, sat: 0 1
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
 0.0080  0.0640  0.1000  0.1363  0.1520  8.3490
sd= 0.1911814
     95%      96%      97%      98%      99%     100%
0.320020 0.352022 0.404025 0.496031 0.736046 8.348520
sum= 13727.21
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
    123    4165    6437    8708    9912  509000
sd= 12001.76
      95%       96%       97%       98%       99%      100%
 20132.00  22292.00  25611.46  31218.14  45977.25 508953.00
sum= 876891551
$t ~ $cfs:
               Estimate  Std. Error  t value  Pr(>|t|)
(Intercept) -1.8749e-03  6.4397e-05  -29.115 < 2.2e-16 ***
E$cfs        1.5870e-05  4.3430e-09 3654.097 < 2.2e-16 ***
R-squared: 0.9925



> SplittingViaOKsolver -D40 WSchur_4_67.cnf
> cat SplitViaOKsolver_D40WSchur_4_67cnf_2012-07-19-121844/Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         4
c initial_number_of_variables           268
c initial_number_of_clauses             4825
c initial_number_of_literal_occurrences 14140
c number_of_2-clauses_after_reduction   402
c running_time(sec)                     276.9
c number_of_nodes                       201407
c number_of_quasi_single_nodes          0
c number_of_2-reductions                696
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        24
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 46003255
c splitting_cases                       100704
> cat Statistics
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  40.00   40.00   42.00   42.31   44.00   49.00
> table(E$n)
   40    41    42    43    44    45    46    47    48    49
28272 11064 13416 18096 16776  6264  5520  1248    24    24
> summary(E$d)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  10.00   17.00   18.00   18.13   20.00   24.00
> table(E$d)
   10    11    12    13    14    15    16    17    18    19    20    21    22
   11    60   339   982  2637  5922 10812 16169 19305 18720 14018  7943  3026
   23    24
  680    80

> ProcessSplitViaOKsolver SplitViaOKsolver_D40WSchur_4_67cnf_2012-07-19-121844
> cat Result
UNSATISFIABLE
  PID TTY      STAT   TIME COMMAND
13122 pts/2    R+   339:12 /bin/bash /home/csoliver/OKplatform/bin/ProcessSplitViaOKsolver SplitViaOKsolver_D40WSchur_4_67cnf_2012-07-19-121844
> E=read_processsplit_minisat()
100704: 4.279h, sum-cfs=9.087163e+08, mean-t=0.153s, mean-cfs=9024, sat: 0
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.008   0.072   0.112   0.153   0.176   9.177
sd= 0.2058379
     95%      96%      97%      98%      99%     100%
0.372023 0.411545 0.468029 0.560035 0.780048 9.176570
sum= 15405.8
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
    136    4333    6710    9024   10380  540200
sd= 12170.76
      95%       96%       97%       98%       99%      100%
 21118.70  23298.76  26364.00  31717.58  46117.16 540202.00
sum= 908716288
$t ~ $cfs:
              Estimate Std. Error  t value  Pr(>|t|)
(Intercept) 3.0345e-03 1.5026e-04   20.195 < 2.2e-16 ***
E$cfs       1.6617e-05 9.9176e-09 1675.510 < 2.2e-16 ***
R-squared: 0.9654
   \endverbatim
   </li>
   <li> All the satisfying assignments should be determined (if possible).
    <ol>
     <li> See "Determining all solutions" below. </li>
     <li> 24 = 4!, and so one might assume that we have one "essential"
     satisfiable node together with its 24 symmetric variations. </li>
     <li> And then it seems that the underlying hypergraph doesn't have
     non-trivial automorphisms. </li>
    </ol>
   </li>
   <li> Fully symmetry-breaking makes it very simple for satz215 (all
   times csltok):
   \verbatim
> OKsolver_2002-O3-DNDEBUG WSchur_fullsb_4_66.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         4
c initial_number_of_variables           264
c initial_number_of_clauses             4698
c initial_number_of_literal_occurrences 13740
c number_of_initial_unit-eliminations   16
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           16
c reddiff_number_of_clauses             733
c reddiff_number_of_literal_occurrences 2370
c number_of_2-clauses_after_reduction   587
c running_time(sec)                     31.3
c number_of_nodes                       125444
c number_of_single_nodes                146
c number_of_quasi_single_nodes          1
c number_of_2-reductions                811741
c number_of_pure_literals               0
c number_of_autarkies                   13270
c number_of_missed_single_nodes         5178
c max_tree_depth                        45
c proportion_searched                   1.142583e-01
c proportion_single                     1.692871e-06
c total_proportion                      0.1142600122839212
c number_of_table_enlargements          0
c number_of_1-autarkies                 11404899
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             WSchur_fullsb_4_66.cnf

> satz215 WSchur_fullsb_4_66.cnf
**** The instance is satisfiable. *****
NB_MONO= 0, NB_UNIT= 662863, NB_BRANCHE= 21391, NB_BACK= 10885
Program terminated in 2.420 seconds.
satz215 WSchur_fullsb_4_66.cnf 2.420 21391 10885 1881405 85328 1 264 4698 -831 57363 16855

> satz215 WSchur_fullsb_4_67.cnf
**** The instance is unsatisfiable. *****
NB_MONO= 0, NB_UNIT= 11183277, NB_BRANCHE= 349813, NB_BACK= 177672
Program terminated in 44.800 seconds.
satz215 WSchur_fullsb_4_67.cnf 44.800 349813 177672 33717030 1394086 0 268 4837 -846 1416796 419705

> march_pl WSchur_fullsb_4_66.cnf
c main():: nodeCount: 72382
c main():: dead ends in main: 34830
c main():: lookAheadCount: 11081391
c main():: unitResolveCount: 301097
c main():: time=25.230000
c main():: necessary_assignments: 4917
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 172240, succes #: 142398
c main():: doublelook: overall 1.579 of all possible doublelooks executed
c main():: doublelook: succesrate: 82.674, average DL_trigger: 111.172
# aborted after two minutes

> picosat913 WSchur_fullsb_4_66.cnf
# aborted after a minute
  </ul>


  \todo Using the nested translation
  <ul>
   <li> OKsolver_2002:
   \verbatim
> OKsolver_2002-O3-DNDEBUG Schur_N_4_45.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         9
c initial_number_of_variables           135
c initial_number_of_clauses             1964
c initial_number_of_literal_occurrences 13059
c number_of_2-clauses_after_reduction   22
c running_time(sec)                     10949.7
c number_of_nodes                       100421458
c number_of_single_nodes                166201
c number_of_quasi_single_nodes          8
c number_of_2-reductions                236083026
c number_of_pure_literals               23424712
c number_of_autarkies                   2
c number_of_missed_single_nodes         80448
c max_tree_depth                        45
c proportion_searched                   9.827227e-01
c proportion_single                     1.727727e-02
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 6542457

> OKsolver_2002-O3-DNDEBUG Schur_SN_4_45.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         9
c initial_number_of_variables           135
c initial_number_of_clauses             2099
c initial_number_of_literal_occurrences 13329
c number_of_2-clauses_after_reduction   157
c running_time(sec)                     13872.3
c number_of_nodes                       83531451
c number_of_single_nodes                79562
c number_of_quasi_single_nodes          113
c number_of_2-reductions                266957969
c number_of_pure_literals               22308
c number_of_autarkies                   1499341
c number_of_missed_single_nodes         106303
c max_tree_depth                        45
c proportion_searched                   9.946118e-01
c proportion_single                     5.388183e-03
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 4967775
   \endverbatim
   Very strange: node-count etc. worse for the weak form, but faster!
   </li>
   <li> satz:
   \verbatim
> satz215 Schur_N_4_45.cnf
**** The instance is unsatisfiable. *****
NB_MONO= 504297029, NB_UNIT= 5513076769, NB_BRANCHE= 827175462, NB_BACK= 414714292
Program terminated in 80406.780 seconds. (cs-wsok)
satz215 Schur_N_4_45.cnf 80406.780 827175462 414714292 49958325674 1801444883 0 135 1964 0 2542611148 744460464
> satz215 Schur_SN_4_45.cnf
# aborted after 7394:55 m
   \endverbatim
   Interesting that the nested encoding is so bad for lookahead-solvers! And
   the strong nested is even worse!
   </li>
   <li> march_pl:
   \verbatim
> march_pl Schur_SN_4_45.cnf
c main():: nodeCount: 39922909
c main():: dead ends in main: 4195734
c main():: lookAheadCount: -875717856
c main():: unitResolveCount: 144883950
c main():: time=10033.229492
c main():: necessary_assignments: 7087717
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 109520846, succes #: 69927172
c main():: doublelook: overall 3.245 of all possible doublelooks executed
c main():: doublelook: succesrate: 63.848, average DL_trigger: 4.958
s UNSATISFIABLE
   \endverbatim
   </li>
   <li> minisat:
   \verbatim
> minisat-2.2.0 Schur_N_4_45.cnf
restarts              : 98302
conflicts             : 76437869       (47708 /sec)
decisions             : 120716625      (0.00 % random) (75344 /sec)
propagations          : 955791712      (596549 /sec)
conflict literals     : 1610804911     (20.98 % deleted)
Memory used           : 26.00 MB
CPU time              : 1602.2 s

> minisat-2.2.0 Schur_SN_4_45.cnf
restarts              : 32767
conflicts             : 23215445       (42132 /sec)
decisions             : 27643669       (0.00 % random) (50168 /sec)
propagations          : 494976252      (898291 /sec)
conflict literals     : 430578339      (23.48 % deleted)
Memory used           : 22.00 MB
CPU time              : 551.02 s
UNSATISFIABLE
   \endverbatim
   strong form actually somewhat better than the direct translation.
   </li>
   <li> picosat and relatives:
   \verbatim
> precosat-570.1 -v Schur_N_4_45.cnf
s UNSATISFIABLE
c 7516175 conflicts, 9520399 decisions, 4825 random
c 9 iterations, 2318 restarts, 97037 skipped
c 119 enlarged, 1 shrunken, 73484 rescored, 1571 rebiased
c 2 simplifications, 119 reductions
c 1880.5 seconds, 36 MB max, 805 MB recycled

> precosat-570.1 -v Schur_SN_4_45.cnf
s UNSATISFIABLE
c 6986638 conflicts, 8804135 decisions, 4470 random
c 7 iterations, 3966 restarts, 86335 skipped
c 115 enlarged, 1 shrunken, 68306 rescored, 1531 rebiased
c 2 simplifications, 115 reductions
c 1876.3 seconds, 36 MB max, 750 MB recycled


> picosat913 Schur_N_4_45.cnf
c 12 iterations
c 9 restarts
c 0 failed literals
c 7784571 conflicts
c 11412324 decisions
c 42 fixed variables
c 150683214 learned literals
c 27.0% deleted literals
c 114381472 propagations
c 100.0% variables used
c 143.1 seconds in library
c 0.8 megaprops/second
c 7 simplifications
c 1828 reductions
c 722.8 MB recycled
c 1.3 MB maximally allocated
c 143.1 seconds total run time

> picosat913 Schur_SN_4_45.cnf
c 17 iterations
c 10 restarts
c 0 failed literals
c 3392279 conflicts
c 4125225 decisions
c 63 fixed variables
c 62119291 learned literals
c 30.4% deleted literals
c 63730946 propagations
c 100.0% variables used
c 66.6 seconds in library
c 1.0 megaprops/second
c 8 simplifications
c 964 reductions
c 301.6 MB recycled
c 1.1 MB maximally allocated
c 66.6 seconds total run time
   \endverbatim
   </li>
   <li> cryptominisat:
   \verbatim
> cryptominisat Schur_N_4_45.cnf
c conflicts                : 8287335     (2269.69   / sec)
c decisions                : 10252270    (0.18      % random)
c bogo-props               : 268187490755 (73449860.78 / sec)
c conflict literals        : 164922311   (31.96     % deleted)
c Memory used              : 193.54      MB
c CPU time                 : 3651.30     s
s UNSATISFIABLE

> cryptominisat Schur_SN_4_45.cnf
c conflicts                : 2437178     (6247.63   / sec)
c decisions                : 2763321     (0.20      % random)
c bogo-props               : 61875726162 (158616509.90 / sec)
c conflict literals        : 35842002    (42.13     % deleted)
c Memory used              : 94.43       MB
c CPU time                 : 390.10      s
s UNSATISFIABLE
   \endverbatim
   </li>
  </ul>


  \todo Determining all solutions
  <ul>
   <li> We need to determine all solutions of the 4 main combinations we
   consider (weak, palindromic), and below the respectivey Ramsey-number,
   and we need to represent them. </li>
   <li> For wschur-problems see above. </li>
   <li> For schur-problems [Fredricksen, Sweet, 2000] asserts that there are
   273 sum-free partitions (apparently the colour-symmetry has been already
   broken?), and 24 of them are palindromic, and they are all equivalent
   (here the automorphism group has size totient(44+1)=24), where the depth
   of 6 of them is 13 (the maximum). </li>
  </ul>

/*
