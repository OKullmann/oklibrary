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
XXX
   \endverbatim
   </li>
   <li> k=22, n=463:
   \verbatim
   \endverbatim
   </li>
  </ul>


  \todo Performance of OKsolver
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

*/
