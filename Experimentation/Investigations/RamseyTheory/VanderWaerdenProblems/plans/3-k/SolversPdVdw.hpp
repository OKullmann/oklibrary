// Oliver Kullmann, 3.6.2013 (Swansea)
/* Copyright 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/SolversPdVdw.hpp
  \brief On SAT solvers computing palindromic vdW-numbers pdvdw_2(3,k)


  \todo Performance of tawSolver
  <ul>
   <li> The following data is on cswsok (3GHz). </li>
   <li> k=17, n=279:
   \verbatim
c running_time(sec)                     6.65
c number_of_nodes                       143319
c number_of_binary_nodes                71659
c number_of_1-reductions                1511306
   \endverbatim
   </li>
   <li> k=18, n=312:
   \verbatim
c running_time(sec)                     59.59
c number_of_nodes                       1063979
c number_of_binary_nodes                531989
c number_of_1-reductions                12050046
   \endverbatim
   </li>
   <li> k=19, n=347:
   \verbatim
c running_time(sec)                     134.32
c number_of_nodes                       2009635
c number_of_binary_nodes                1004817
c number_of_1-reductions                23490743
   \endverbatim
   </li>
   <li> k=20, n=389:
   \verbatim
c running_time(sec)                     738.25
c number_of_nodes                       9076261
c number_of_binary_nodes                4538130
c number_of_1-reductions                110593054
   \endverbatim
   </li>
   <li> k=21, n=405:
   \verbatim
c running_time(sec)                     2541.11
c number_of_nodes                       30470349
c number_of_binary_nodes                15235174
c number_of_1-reductions                374871138
   \endverbatim
   </li>
   <li> k=22, n=463:
   \verbatim
c running_time(sec)                     18305.95
c number_of_nodes                       170414771
c number_of_binary_nodes                85207385
c number_of_1-reductions                2236999086

#final version (dbfe40729515a5643f18e5953abe91d704626edf)
c number_of_variables                   232
c number_of_clauses                     28650
c running_time(sec)                     20753.85
c number_of_nodes                       170414771
c number_of_binary_nodes                85207385
c number_of_1-reductions                2236999086
c max_number_changes                    54879
   \endverbatim
   </li>
   <li> k=23, n=507 (with final version):
   \verbatim
c number_of_variables                   254
c number_of_clauses                     34289
c running_time(sec)                     86869.06
c number_of_nodes                       573190251
c number_of_binary_nodes                286595125
c number_of_1-reductions                7857883219
c max_number_changes                    65646
   \endverbatim
   </li>
  </ul>


  \todo Performance of Satz
  <ul>
   <li> The following data is on cswsok (3GHz). </li>
   <li> k=17, n=279:
   \verbatim
c reddiff_number_of_clauses             0
c running_time(sec)                     12.31
c number_of_nodes                       16466
c number_of_binary_nodes                8634
c number_of_pure_literals               18
c number_of_1-reductions                331064
c number_of_2-look-ahead                1937224
c number_of_2-reductions                74103
c number_of_3-look-ahead                5655
c number_of_3-reductions                3231
   \endverbatim
   </li>
   <li> k=18, n=312:
   \verbatim
c reddiff_number_of_clauses             0
c running_time(sec)                     182.23
c number_of_nodes                       208873
c number_of_binary_nodes                107404
c number_of_pure_literals               8
c number_of_1-reductions                4647841
c number_of_2-look-ahead                23851105
c number_of_2-reductions                774734
c number_of_3-look-ahead                99584
c number_of_3-reductions                63129
   \endverbatim
   </li>
   <li> k=19, n=347:
   \verbatim
c reddiff_number_of_clauses             0
c running_time(sec)                     143.97
c number_of_nodes                       123199
c number_of_binary_nodes                63790
c number_of_pure_literals               22
c number_of_1-reductions                3094368
c number_of_2-look-ahead                17082737
c number_of_2-reductions                627026
c number_of_3-look-ahead                64991
c number_of_3-reductions                37548
   \endverbatim
   </li>
   <li> k=20, n=389:
   \verbatim
c reddiff_number_of_clauses             0
c running_time(sec)                     701.38
c number_of_nodes                       459899
c number_of_binary_nodes                236973
c number_of_pure_literals               41
c number_of_1-reductions                12644383
c number_of_2-look-ahead                68201788
c number_of_2-reductions                2285537
c number_of_3-look-ahead                248693
c number_of_3-reductions                145795
   \endverbatim
   </li>
   <li> k=21, n=405:
   \verbatim
c reddiff_number_of_clauses             0
c running_time(sec)                     2591.63
c number_of_nodes                       1567736
c number_of_binary_nodes                805638
c number_of_pure_literals               161
c number_of_1-reductions                44254990
c number_of_2-look-ahead                229084306
c number_of_2-reductions                7469843
c number_of_3-look-ahead                781064
c number_of_3-reductions                459993
   \endverbatim
   </li>
   <li> k=22, n=463:
   \verbatim
c reddiff_number_of_clauses             0
c running_time(sec)                     9418.25
c number_of_nodes                       4393139
c number_of_binary_nodes                2265287
c number_of_pure_literals               287
c number_of_1-reductions                136151893
c number_of_2-look-ahead                759328415
c number_of_2-reductions                26618563
c number_of_3-look-ahead                3364632
c number_of_3-reductions                1919460
   \endverbatim
   </li>
   <li> k=23, n=507:
   \verbatim
c reddiff_number_of_clauses             0
c running_time(sec)                     35633.40
c number_of_nodes                       12587868
c number_of_binary_nodes                6480362
c number_of_pure_literals               247
c number_of_1-reductions                419514647
c number_of_2-look-ahead                2324947999
c number_of_2-reductions                77105659
c number_of_3-look-ahead                9992839
c number_of_3-reductions                5266746
   \endverbatim
   </li>
  </ul>


  \todo Performance of OKsolver-2002
  <ul>
   <li> The following data is on cswsok (3GHz). </li>
   <li> k=17, n=279:
   \verbatim
c running_time(sec)                     17.6
c number_of_nodes                       5023
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                62235
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        21
   \endverbatim
   </li>
   <li> k=18, n=312:
   \verbatim
c running_time(sec)                     334.7
c number_of_nodes                       100803
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1169797
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        33
   \endverbatim
   </li>
   <li> k=19, n=347:
   \verbatim
c running_time(sec)                     322.2
c number_of_nodes                       62009
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                869867
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        34
   \endverbatim
   </li>
   <li> k=20, n=389:
   \verbatim
c running_time(sec)                     1418.5
c number_of_nodes                       206617
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                2981622
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        34
   \endverbatim
   </li>
   <li> k=21, n=405:
   \verbatim
c running_time(sec)                     3536.1
c number_of_nodes                       490841
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                7257352
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        39
   \endverbatim
   </li>
   <li> k=22, n=463:
   \verbatim
c running_time(sec)                     47592.5
c number_of_nodes                       3197173
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                51285499
c number_of_pure_literals               94
c number_of_autarkies                   0
c number_of_missed_single_nodes         1
c max_tree_depth                        47
   \endverbatim
   </li>
   <li> k=23, n=507:
   \verbatim
XXX
   \endverbatim
   </li>
  </ul>


  \todo Performance of march_pl
  <ul>
   <li> The following data is on cswsok (3GHz). </li>
   <li> k=17, n=279:
   \verbatim
c main():: nodeCount: 1448
c main():: dead ends in main: 13
c main():: lookAheadCount: 368548
c main():: unitResolveCount: 11734
c main():: time=35.259998
c main():: necessary_assignments: 370
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 9144, succes #: 6914
c main():: doublelook: overall 2.568 of all possible doublelooks executed
c main():: doublelook: succesrate: 75.612, average DL_trigger: 1409.377
   \endverbatim
   </li>
   <li> k=18, n=312:
   \verbatim
c main():: nodeCount: 12289
c main():: dead ends in main: 84
c main():: lookAheadCount: 3382245
c main():: unitResolveCount: 101014
c main():: time=269.470001
c main():: necessary_assignments: 3473
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 89638, succes #: 68692
c main():: doublelook: overall 2.748 of all possible doublelooks executed
c main():: doublelook: succesrate: 76.633, average DL_trigger: 1061.048
   \endverbatim
   </li>
   <li> k=19, n=347:
   \verbatim
c main():: nodeCount: 12423
c main():: dead ends in main: 138
c main():: lookAheadCount: 3943140
c main():: unitResolveCount: 127771
c main():: time=499.959991
c main():: necessary_assignments: 4284
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 93667, succes #: 73103
c main():: doublelook: overall 2.453 of all possible doublelooks executed
c main():: doublelook: succesrate: 78.046, average DL_trigger: 1496.145
   \endverbatim
   </li>
   <li> k=20, n=389:
   \verbatim
c main():: nodeCount: 39681
c main():: dead ends in main: 386
c main():: lookAheadCount: 14072852
c main():: unitResolveCount: 440883
c main():: time=1979.619995
c main():: necessary_assignments: 14471
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 329805, succes #: 259413
c main():: doublelook: overall 2.418 of all possible doublelooks executed
c main():: doublelook: succesrate: 78.656, average DL_trigger: 1546.422
   \endverbatim
   </li>
   <li> k=21, n=405:
   \verbatim
c main():: nodeCount: 99493
c main():: dead ends in main: 926
c main():: lookAheadCount: 35709195
c main():: unitResolveCount: 1168836
c main():: time=5052.510254
c main():: necessary_assignments: 38929
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 869332, succes #: 692392
c main():: doublelook: overall 2.512 of all possible doublelooks executed
c main():: doublelook: succesrate: 79.646, average DL_trigger: 1436.372
   \endverbatim
   </li>
   <li> k=22, n=463:
   \verbatim
c main():: nodeCount: 376285
c main():: dead ends in main: 2998
c main():: lookAheadCount: 155528208
c main():: unitResolveCount: 4693870
c main():: time=25841.298828
c main():: necessary_assignments: 153181
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 3725128, succes #: 3005292
c main():: doublelook: overall 2.468 of all possible doublelooks executed
c main():: doublelook: succesrate: 80.676, average DL_trigger: 1555.402
   \endverbatim
   </li>
   <li> k=23, n=507:
   \verbatim
c main():: nodeCount: 876315
c main():: dead ends in main: 6524
c main():: lookAheadCount: 400324456
c main():: unitResolveCount: 11740884
c main():: time=77763.023438
c main():: necessary_assignments: 381321
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 9348272, succes #: 7623499
c main():: doublelook: overall 2.403 of all possible doublelooks executed
c main():: doublelook: succesrate: 81.550, average DL_trigger: 1670.000
   \endverbatim
   </li>
  </ul>


  \todo minisat-2.2.0
  <ul>
   <li> The following data is on cswsok (3GHz). </li>
   <li> k=17, n=279:
   \verbatim
restarts              : 125
conflicts             : 34426          (45725 /sec)
decisions             : 42513          (0.00 % random) (56467 /sec)
propagations          : 860374         (1142769 /sec)
conflict literals     : 481868         (45.43 % deleted)
Memory used           : 20.00 MB
CPU time              : 0.752885 s
   \endverbatim
   </li>
   <li> k=18, n=312:
   \verbatim
restarts              : 1278
conflicts             : 607908         (32821 /sec)
decisions             : 735872         (0.00 % random) (39729 /sec)
propagations          : 15350310       (828753 /sec)
conflict literals     : 11855450       (41.26 % deleted)
Memory used           : 26.00 MB
CPU time              : 18.5222 s
   \endverbatim
   </li>
   <li> k=19, n=347:
   \verbatim
restarts              : 1196
conflicts             : 568924         (29651 /sec)
decisions             : 703059         (0.00 % random) (36642 /sec)
propagations          : 16230849       (845926 /sec)
conflict literals     : 10541271       (45.47 % deleted)
Memory used           : 30.00 MB
CPU time              : 19.1871 s
   \endverbatim
   </li>
   <li> k=20, n=389:
   \verbatim
restarts              : 5042
conflicts             : 2852150        (24178 /sec)
decisions             : 3528665        (0.00 % random) (29913 /sec)
propagations          : 85999586       (729020 /sec)
conflict literals     : 59975511       (44.44 % deleted)
Memory used           : 40.00 MB
CPU time              : 117.966 s
   \endverbatim
   </li>
   <li> k=21, n=405:
   \verbatim
restarts              : 14940
conflicts             : 9179642        (21712 /sec)
decisions             : 11373460       (0.00 % random) (26901 /sec)
propagations          : 284857940      (673768 /sec)
conflict literals     : 206903430      (44.93 % deleted)
Memory used           : 65.00 MB
CPU time              : 422.784 s
   \endverbatim
   </li>
   <li> k=22, n=463:
   \verbatim
restarts              : 65535
conflicts             : 51582064       (16372 /sec)
decisions             : 63997045       (0.00 % random) (20312 /sec)
propagations          : 1717379015     (545082 /sec)
conflict literals     : 1306810558     (44.32 % deleted)
Memory used           : 139.00 MB
CPU time              : 3150.68 s
   \endverbatim
   </li>
   <li> k=23, n=507:
   \verbatim
restarts              : 131071
conflicts             : 108028217      (13188 /sec)
decisions             : 134363355      (0.00 % random) (16403 /sec)
propagations          : 3842638229     (469102 /sec)
conflict literals     : 2874581596     (45.35 % deleted)
Memory used           : 185.00 MB
CPU time              : 8191.48 s
   \endverbatim
   </li>
   <li> k=24, n=593:
   \verbatim
restarts              : 524287
conflicts             : 476716936      (8719 /sec)
decisions             : 591563295      (0.00 % random) (10819 /sec)
propagations          : 18599887471    (340173 /sec)
conflict literals     : 13778598918    (45.50 % deleted)
Memory used           : 431.00 MB
CPU time              : 54677.7 s
   \endverbatim
   </li>
  </ul>


  \todo glucose-2.2
  <ul>
   <li> The following data is on cswsok (3GHz). </li>
   <li> k=17, n=279:
   \verbatim
c conflicts             : 34826          (43758 /sec)
c decisions             : 42243          (0.00 % random) (53077 /sec)
c propagations          : 856404         (1076048 /sec)
c conflict literals     : 472777         (46.21 % deleted)
c nb reduced Clauses    : 1038
c CPU time              : 0.795879 s
   \endverbatim
   </li>
   <li> k=18, n=312:
   \verbatim
c conflicts             : 340568         (23695 /sec)
c decisions             : 409372         (0.00 % random) (28482 /sec)
c propagations          : 8334210        (579859 /sec)
c conflict literals     : 5907358        (46.92 % deleted)
c nb reduced Clauses    : 1832
c CPU time              : 14.3728 s
   \endverbatim
   </li>
   <li> k=19, n=347:
   \verbatim
c conflicts             : 336861         (22607 /sec)
c decisions             : 411242         (0.00 % random) (27599 /sec)
c propagations          : 9358152        (628033 /sec)
c conflict literals     : 5726624        (48.46 % deleted)
c nb reduced Clauses    : 3831
c CPU time              : 14.9007 s
   \endverbatim
   </li>
   <li> k=20, n=389:
   \verbatim
c conflicts             : 1132012        (17077 /sec)
c decisions             : 1382467        (0.00 % random) (20855 /sec)
c propagations          : 32958701       (497198 /sec)
c conflict literals     : 20918084       (49.54 % deleted)
c nb reduced Clauses    : 6862
c CPU time              : 66.2889 s
   \endverbatim
   </li>
   <li> k=21, n=405:
   \verbatim
c conflicts             : 2903573        (12708 /sec)
c decisions             : 3537134        (0.00 % random) (15480 /sec)
c propagations          : 86029511       (376511 /sec)
c conflict literals     : 57645538       (50.26 % deleted)
c nb reduced Clauses    : 10706
c CPU time              : 228.491 s
   \endverbatim
   </li>
   <li> k=22, n=463:
   \verbatim
c conflicts             : 13397451       (8214 /sec)
c decisions             : 16391394       (0.00 % random) (10050 /sec)
c propagations          : 419701238      (257329 /sec)
c conflict literals     : 287632124      (51.16 % deleted)
c nb reduced Clauses    : 21170
c CPU time              : 1630.99 s
   \endverbatim
   </li>
   <li> k=23, n=507:
   \verbatim
c conflicts             : 36314064       (5327 /sec)
c decisions             : 45279709       (0.00 % random) (6642 /sec)
c propagations          : 1207604295     (177137 /sec)
c conflict literals     : 808891457      (52.62 % deleted)
c nb reduced Clauses    : 35546
c CPU time              : 6817.36 s
   \endverbatim
   </li>
   <li> k=24, n=593:
   \verbatim
XXX
   \endverbatim
   </li>
  </ul>


  \todo cryptominisat 296
  <ul>
   <li> The following data is on cswsok (3GHz). </li>
   <li> k=17, n=279:
   \verbatim
c conflicts                : 59443       (16765.94  / sec)
c decisions                : 74385       (1.41      % random)
c bogo-props               : 290602922   (81964777.50 / sec)
c conflict literals        : 636275      (55.16     % deleted)
c Memory used              : 44.63       MB
c CPU time                 : 3.55        s
   \endverbatim
   </li>
   <li> k=18, n=312:
   \verbatim
c conflicts                : 871916      (11133.58  / sec)
c decisions                : 1088441     (0.46      % random)
c bogo-props               : 10047985340 (128303665.75 / sec)
c conflict literals        : 11363718    (56.25     % deleted)
c Memory used              : 78.76       MB
c CPU time                 : 78.31       s
   \endverbatim
   </li>
   <li> k=19, n=347:
   \verbatim
c conflicts                : 765301      (10610.90  / sec)
c decisions                : 972979      (0.47      % random)
c bogo-props               : 8546056772  (118491107.33 / sec)
c conflict literals        : 9898161     (56.73     % deleted)
c Memory used              : 75.16       MB
c CPU time                 : 72.12       s
   \endverbatim
   </li>
   <li> k=20, n=389:
   \verbatim
c conflicts                : 3071462     (7993.99   / sec)
c decisions                : 3912677     (0.24      % random)
c bogo-props               : 55732081469 (145051925.93 / sec)
c conflict literals        : 43823104    (57.27     % deleted)
c Memory used              : 147.80      MB
c CPU time                 : 384.22      s
   \endverbatim
   </li>
   <li> k=21, n=405:
   \verbatim
c conflicts                : 8458496     (5963.80   / sec)
c decisions                : 10801635    (0.18      % random)
c bogo-props               : 224001275243 (157935744.89 / sec)
c conflict literals        : 128081298   (57.71     % deleted)
c Memory used              : 287.72      MB
c CPU time                 : 1418.31     s
   \endverbatim
   </li>
   <li> k=22, n=463:
   \verbatim
c conflicts                : 49716762    (3471.55   / sec)
c decisions                : 63574055    (0.15      % random)
c bogo-props               : 2626670786337 (183411593.68 / sec)
c conflict literals        : 823872626   (58.16     % deleted)
c Memory used              : 1318.82     MB
c CPU time                 : 14321.18    s
   \endverbatim
   </li>
   <li> k=23, n=507:
   \verbatim
   \endverbatim
   </li>
   <li> k=24, n=593:
   \verbatim
   \endverbatim
   </li>
  </ul>


  \todo precosat-570
  <ul>
   <li> The following data is on cswsok (3GHz). </li>
   <li> k=17, n=279:
   \verbatim
c 41961 conflicts, 63292 decisions, 27 random
c 11 iterations, 983 restarts, 0 skipped
c 6 enlarged, 1 shrunken, 409 rescored, 21 rebiased
c 3 simplifications, 6 reductions
c 1.2 seconds, 3 MB max, 3 MB recycled
   \endverbatim
   </li>
   <li> k=18, n=312:
   \verbatim
c 506793 conflicts, 735216 decisions, 363 random
c 9 iterations, 9030 restarts, 0 skipped
c 28 enlarged, 1 shrunken, 4954 rescored, 156 rebiased
c 2 simplifications, 28 reductions
c 24.5 seconds, 9 MB max, 47 MB recycled
   \endverbatim
   </li>
   <li> k=19, n=347:
   \verbatim
c 485357 conflicts, 710776 decisions, 352 random
c 8 iterations, 8830 restarts, 0 skipped
c 27 enlarged, 2 shrunken, 4745 rescored, 146 rebiased
c 3 simplifications, 27 reductions
c 24.1 seconds, 8 MB max, 42 MB recycled
   \endverbatim
   </li>
   <li> k=20, n=389:
   \verbatim
c 1799145 conflicts, 2620097 decisions, 1288 random
c 11 iterations, 29215 restarts, 0 skipped
c 56 enlarged, 1 shrunken, 17594 rescored, 504 rebiased
c 3 simplifications, 56 reductions
c 130.7 seconds, 16 MB max, 178 MB recycled
   \endverbatim
   </li>
   <li> k=21, n=405:
   \verbatim
c 4687589 conflicts, 6717622 decisions, 3434 random
c 13 iterations, 65967 restarts, 0 skipped
c 93 enlarged, 2 shrunken, 45846 rescored, 1022 rebiased
c 4 simplifications, 93 reductions
c 444.8 seconds, 27 MB max, 503 MB recycled
   \endverbatim
   </li>
   <li> k=22, n=463:
   \verbatim
c 22283651 conflicts, 31613291 decisions, 15760 random
c 11 iterations, 265520 restarts, 0 skipped
c 207 enlarged, 2 shrunken, 217970 rescored, 4093 rebiased
c 4 simplifications, 207 reductions
c 2825.6 seconds, 59 MB max, 2579 MB recycled
   \endverbatim
   </li>
   <li> k=23, n=507:
   \verbatim
c 54951563 conflicts, 77506262 decisions, 39144 random
c 20 iterations, 589652 restarts, 0 skipped
c 328 enlarged, 1 shrunken, 537534 rescored, 8650 rebiased
c 3 simplifications, 328 reductions
c 9279.6 seconds, 94 MB max, 2608 MB recycled
   \endverbatim
   </li>
   <li> k=24, n=593:
   \verbatim
c 261084988 conflicts, 366324085 decisions, 184615 random
c 14 iterations, 2543523 restarts, 0 skipped
c 719 enlarged, 1 shrunken, 2554016 rescored, 35799 rebiased
c 3 simplifications, 719 reductions
c 82750.4 seconds, 214 MB max, 951 MB recycled
   \endverbatim
   </li>
  </ul>


  \todo lingelingala-b02aa1a-121013
  <ul>
   <li> The following data is on cswsok (3GHz). </li>
   <li> k=17, n=279:
   \verbatim
c 83802 decisions, 57306 conflicts, 15482.1 conflicts/sec
c 1616255 propagations, 0.4 megaprops/sec
c 3.7 seconds, 5.0 MB
   \endverbatim
   </li>
   <li> k=18, n=312:
   \verbatim
c 919123 decisions, 631242 conflicts, 10726.8 conflicts/sec
c 16709890 propagations, 0.3 megaprops/sec
c 58.8 seconds, 15.6 MB
   \endverbatim
   </li>
   <li> k=19, n=347:
   \verbatim
c 915107 decisions, 621445 conflicts, 10252.5 conflicts/sec
c 18451015 propagations, 0.3 megaprops/sec
c 60.6 seconds, 15.6 MB
   \endverbatim
   </li>
   <li> k=20, n=389:
   \verbatim
c 3633502 decisions, 2446421 conflicts, 6892.3 conflicts/sec
c 75849295 propagations, 0.2 megaprops/sec
c 354.9 seconds, 31.5 MB
   \endverbatim
   </li>
   <li> k=21, n=405:
   \verbatim
c 8672073 decisions, 5878695 conflicts, 5545.8 conflicts/sec
c 185032629 propagations, 0.2 megaprops/sec
c 1060.0 seconds, 47.8 MB
   \endverbatim
   </li>
   <li> k=22, n=463:
   \verbatim
c 41696062 decisions, 28280142 conflicts, 3355.4 conflicts/sec
c 936307021 propagations, 0.1 megaprops/sec
c 8428.2 seconds, 125.1 MB
   \endverbatim
   </li>
   <li> k=23, n=507:
   \verbatim
c 104007799 decisions, 70252631 conflicts, 2461.3 conflicts/sec
c 2452960787 propagations, 0.1 megaprops/sec
c 28542.5 seconds, 173.2 MB
   \endverbatim
   </li>
   <li> k=24, n=593:
   \verbatim
   \endverbatim
   </li>
  </ul>

*/
