// Oliver Kullmann, 8.6.2013 (Swansea)
/* Copyright 2013, 2015, 2016, 2020, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/TawSolver/plans/general.hpp
  \brief Plans regarding updating and improving the TawSolver


  \todo Check instance on which the new ttawSolver is worse by runtime than
  the old one (relative to the number of nodes)
  <ul>
   <li> Easiest to search for instances where the number of nodes is comparable
        while the new ttawSolver is maximally slower than the old ttawSolver.
   </li>
   <li> /data/benchmarks/SAT/SATCompetition/2011/random/medium/unif-k7-r89-v90-c8010-S224440242-006.SATISFIABLE.cnf
       is one possible candidate.
   </li>
   <li> If a combined table of instances from SC11-20 is made and one search in it for
        instances where 2.8.2's runtime is at least 10 % less than 2.11.1, while the number
        of nodes is the same (x stands for 2.8.2, while y stands for 2.11.1):
        file                                              sat.x    t.x    nds.x      r1.x sat.y    t.y    nds.y      r1.y
        Q3inK11.cnf                                           1   0.48       43         2     1   0.60       43         2
        unif-k7-r89-v60-c5340-S1583005701-053.SATISFIABLE.cnf 1 176.03  7740617  31539081     1 204.44  7740617  31539081
        unif-k7-r89-v60-c5340-S1675230100-032.UNKNOWN.cnf     0 805.45 35571081 145097312     0 960.29 35571081 145097312
        unif-k7-r89-v60-c5340-S1720200795-004.UNKNOWN.cnf     0 854.28 37492631 152910485     0 984.28 37492631 152910485
        unif-k7-r89-v90-c8010-S224440242-006.SATISFIABLE.cnf  1 574.14 18300016  99626877     1 831.89 18300016  99626878
   </li>
   <li>
        unif-k7-* are in /data/benchmarks/SAT/SATCompetition/2011/random/medium/
        Q3inK11.cnf is in /data/benchmarks/SAT/SATCompetition/2014/crafted/
   </li>
   <li>
        If the number of nodes is not equal, but comparable (+- 10%), nothing
        interesting can be found additionally.
   </li>
   <li> There are few instances where 2.8.2 coped quite fast, while 2.11.1 didn't
        solve them in 1000 seconds.
   </li>
   <li> /data/benchmarks/SAT/SATCompetition/2014/app/complete-400-0.1-7-9876543214007.cnf
       is one such instance: 2.8.2 solved it in 0.34 seconds.
   </li>
   <li> New results on cs-server (under full load):
\verbatim
^Cs UNKNOWN
c program_name                          tawSolver
c version_number                        2.13.1
c options                               ""
c weights                               4.85,1,0.354,0.11,0.0694;1.46
c file_name                             /data/benchmarks/SAT/SATCompetition/2011/random/medium/unif-k7-r89-v90-c8010-S224440242-006.SATISFIABLE.cnf
c p_param_variables                     90
c p_param_clauses                       8010
c number_tautologies                    0
c max_occurring_variable                90
c number_of_clauses                     8010
c maximal_clause_length                 7
c number_of_literal_occurrences         56070
c running_time(sec)                     17716.32
c number_of_nodes                       609501290
c number_of_binary_nodes                304750634
c number_of_1-reductions                3343136306
c reading-and-set-up_time(sec)          0.009

s SATISFIABLE
c program_name                          ttawSolver
c version_number                        2.13.1
c options                               "PT"
c weights                               7,1,0.31,0.19,0.111765;1.7
c file_name                             /data/benchmarks/SAT/SATCompetition/2011/random/medium/unif-k7-r89-v90-c8010-S224440242-006.SATISFIABLE.cnf
c p_param_variables                     90
c p_param_clauses                       8010
c number_tautologies                    0
c max_occurring_variable                90
c number_of_clauses                     8010
c maximal_clause_length                 7
c number_of_literal_occurrences         56070
c running_time(sec)                     577.02
c number_of_nodes                       18300016
c number_of_binary_nodes                9149993
c number_of_1-reductions                99626878
c number_of_pure_literals               1
c number_wtau_calls                     39294233
c number_tau_iterations                 136861230
c reading-and-set-up_time(sec)          0.006

s SATISFIABLE
c program_name                          tawSolver
c version_number                        2.13.1
c options                               ""
c weights                               7,1,0.31,0.19,0.111765;1.7
c file_name                             /data/benchmarks/SAT/SATCompetition/2011/random/medium/unif-k7-r89-v90-c8010-S224440242-006.SATISFIABLE.cnf
c p_param_variables                     90
c p_param_clauses                       8010
c number_tautologies                    0
c max_occurring_variable                90
c number_of_clauses                     8010
c maximal_clause_length                 7
c number_of_literal_occurrences         56070
c running_time(sec)                     542.93
c number_of_nodes                       18261516
c number_of_binary_nodes                9130743
c number_of_1-reductions                99796719
c reading-and-set-up_time(sec)          0.009

^Cs UNKNOWN
c program_name                          ttawSolver
c version_number                        2.13.1
c options                               "PT"
c weights                               4.85,1,0.354,0.11,0.0694;1.46
c file_name                             /data/benchmarks/SAT/SATCompetition/2011/random/medium/unif-k7-r89-v90-c8010-S224440242-006.SATISFIABLE.cnf
c p_param_variables                     90
c p_param_clauses                       8010
c number_tautologies                    0
c max_occurring_variable                90
c number_of_clauses                     8010
c maximal_clause_length                 7
c number_of_literal_occurrences         56070
c running_time(sec)                     5569.79
c number_of_nodes                       175763794
c number_of_binary_nodes                87881884
c number_of_1-reductions                961252573
c number_of_pure_literals               0
c number_wtau_calls                     376586405
c number_tau_iterations                 1290210008
c reading-and-set-up_time(sec)          0.009
\endverbatim
   We see that the original high runtime (883.89s) was likely due to extreme(?)
   circumstances.
   </li>
   <li> Two "UNKNOWN" instances are solved? Hard to imagine that these
   instances are really "unknown", but one should check. </li>
   <li> According to the SC11 results, the majoriy (but not all) of random instances
   unif-k* with 'UNKNOWN' in thier names were solved by at least one solver at SC11.
   There are both satisfible and unsatisfiable instances among the solved ones, see
   http://www.cril.univ-artois.fr/SAT11/results/globalbybench.php?idev=45&idcat=61
   As for those two 'UNKNOWN' instances that were analysed above, both were solved
   and the results were UNSAT.
   </li>
   <li> Similar load on the server:
   \verbatim
> OKsolver2002 /data/benchmarks/SAT/SATCompetition/2011/random/medium/unif-k7-r89-v60-c5340-S1720200795-004.UNKNOWN.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         7
c initial_number_of_variables           60
c initial_number_of_clauses             5340
c initial_number_of_literal_occurrences 37380
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     798.8
c number_of_nodes                       5676250
c number_of_single_nodes                1
c number_of_quasi_single_nodes          0
c number_of_2-reductions                13983757
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         1
c max_tree_depth                        32
c proportion_searched                   9.999999e-01
c proportion_single                     5.960464e-08
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0

s UNSATISFIABLE
c program_name                          tawSolver
c   version_number                      2.15.0
c   options                             ""
c   weights                             4.85,1,0.354,0.11,0.0694;1.46
c file_name                             /data/benchmarks/SAT/SATCompetition/2011/random/medium/unif-k7-r89-v60-c5340-S1720200795-004.UNKNOWN.cnf
c   p_param_variables                   60
c   p_param_clauses                     5340
c   number_tautologies                  0
c   max_occurring_variable              60
c   number_of_clauses                   5340
c   maximal_clause_length               7
c   number_of_literal_occurrences       37380
c running_time(sec)                     765.29
c number_of_nodes                       34803193
c   number_of_binary_nodes              17401596
c   number_of_single_child_nodes        0
c   number_of_leaves                    17401597
c   number_of_internal_nodes            17401596
c   inodes_per_second                   2.27e+04
c number_of_1-reductions                142529983
c   1-reductions_per_second             1.86e+05
c   1-reductions_per_node               4.10
c reading-and-set-up_time(sec)          0.006

s UNSATISFIABLE
c program_name                          ttawSolver
c   version_number                      2.15.0
c   options                             "PT"
c   weights                             7,1,0.31,0.19,0.111765;1.7
c file_name                             /data/benchmarks/SAT/SATCompetition/2011/random/medium/unif-k7-r89-v60-c5340-S1720200795-004.UNKNOWN.cnf
c   p_param_variables                   60
c   p_param_clauses                     5340
c   number_tautologies                  0
c   max_occurring_variable              60
c   number_of_clauses                   5340
c   maximal_clause_length               7
c   number_of_literal_occurrences       37380
c running_time(sec)                     882.06
c number_of_nodes                       37492631
c   number_of_binary_nodes              18746315
c   number_of_single_child_nodes        0
c   number_of_leaves                    18746316
c   number_of_internal_nodes            18746315
c   inodes_per_second                   2.13e+04
c number_of_1-reductions                152910485
c   1-reductions_per_second             1.73e+05
c   1-reductions_per_node               4.08
c number_of_pure_literals               1
c number_wtau_calls                     74750071
c   number_wtau_calls_per_inode         3.99
c number_tau_iterations                 259436383
c   average_tau_iterations              3.47
c reading-and-set-up_time(sec)          0.006
\endverbatim
   Doesn't look as if something special is going on.
   Likely due to the larger clause-length, the r2-reduction of the OKsolver
   are less effective. </li>
  </ul>


  \todo Switch to new Makefile
  <ul>
   <li> Likely best to introduce some machinery, to avoid duplications.
   </li>
   <li> Regression-testing is needed. </li>
   <li> Application-testing via DirMatching. </li>
   <li> Timing-testing via DirTiming. </li>
   <li> Fuzzing via BRG. </li>
   <li> See "Create tests" below. </li>
  </ul>


  \todo Provide application-tests
  <ul>
   <li> Using the new system. </li>
   <li> Examples:
\verbatim
> for ((i=0; i < 10; ++i)); do BRG "110*110,2" "" $i | ctawSolver -cin -nil | ExtractTawSolver x; done
95 110 220 2 5.04 1 66539577 33269788 33450300 NA 33260448 1062830148387840 0.000 95 110 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
95 110 220 2 0.55 1 7201535 3600767 3623331 NA 3598780 77016421969920 0.000 95 110 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
95 110 220 2 2.16 1 29137663 14568831 14581807 NA 14567038 347071021903872 0.000 95 110 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
99 110 220 2 0.56 1 7263765 3631882 3644195 NA 3630144 60427763182080 0.000 99 110 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
94 110 220 2 0.11 1 1413631 706815 717904 NA 703644 662606438400 0.000 94 110 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
100 110 220 2 0.75 1 9584651 4792325 4848228 NA 4791552 276217344000000 0.000 100 110 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
93 110 220 2 2.46 1 32434175 16217087 16298780 NA 16209000 120142028079360 0.000 93 110 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
100 110 220 2 14.86 1 194252799 97126399 97193259 NA 97109312 1732538406551040 0.000 100 110 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
86 110 220 2 0.05 1 684287 342143 345552 NA 341524 104656717440 0.000 86 110 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
96 110 220 2 0.18 1 2355199 1177599 1181055 NA 1176960 20986092748800 0.000 96 110 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"

> for ((i=0; i < 10; ++i)); do BRG "110*110,2" "" $i | cttawSolver -cin -nil | ExtractTawSolver x; done
95 110 220 2 5.23 1 63162639 31581319 31768388 NA 31571472 1062830148387840 0.000 95 110 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
95 110 220 2 0.62 1 7447679 3723839 3746825 NA 3721984 77016421969920 0.000 95 110 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
95 110 220 2 2.46 1 29487871 14743935 14756273 NA 14742207 347071021903872 0.000 95 110 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
99 110 220 2 0.89 1 10595347 5297673 5305793 NA 5296016 60427763182080 0.000 99 110 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
94 110 220 2 0.12 1 1373311 686655 697226 NA 683304 662606438400 0.000 94 110 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
100 110 220 2 0.84 1 9584651 4792325 4868548 NA 4791552 276217344000000 0.000 100 110 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
93 110 220 2 2.79 1 33251839 16625919 16704128 NA 16618840 120142028079360 0.000 93 110 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
100 110 220 2 16.74 1 194826239 97413119 97475323 NA 97401152 1732538406551040 0.000 100 110 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
86 110 220 2 0.06 1 737535 368767 371930 NA 368088 104656717440 0.000 86 110 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
96 110 220 2 0.19 1 2248703 1124351 1128207 NA 1123712 20986092748800 0.000 96 110 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"

> for ((i=0; i < 10; ++i)); do BRG "800*200,3" "" $i | ctawSolver -cin -nil | ExtractTawSolver x; done
200 800 2400 3 0.36 1 1198089 599044 2495248 NA 518186 4691185288 0.001 200 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
200 800 2400 3 1.30 1 7846883 3923441 5915827 NA 3692318 172641398940 0.000 200 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
200 800 2400 3 0.73 1 2264913 1132456 5212377 NA 962872 7981181539 0.000 200 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
200 800 2400 3 0.09 1 49331 24665 799523 NA 1246 3490912 0.000 200 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
200 800 2400 3 0.39 1 2025111 1012555 2252921 NA 956893 14031588512 0.000 200 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
200 800 2400 3 0.21 1 338579 169289 1746775 NA 117442 490857288 0.000 200 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
200 800 2400 3 0.23 1 771807 385903 1580227 NA 336435 38848104956 0.000 200 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
200 800 2400 3 0.12 1 181663 90831 980937 NA 60227 3197659056 0.000 200 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
200 800 2400 3 0.44 1 1655341 827670 2787005 NA 746749 67920769248 0.000 200 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
200 800 2400 3 0.10 1 90595 45297 903041 NA 17331 1276996896 0.000 200 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"

> for ((i=0; i < 10; ++i)); do BRG "800*200,3" "" $i | cttawSolver -cin -nil | ExtractTawSolver x; done
200 800 2400 3 0.60 1 1180343 590171 2545797 NA 506943 4691185288 0.001 200 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
200 800 2400 3 1.85 1 8067993 4033996 6700052 NA 3780782 172641398940 0.000 200 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
200 800 2400 3 1.26 1 2397191 1198595 5547806 NA 1014750 7981181539 0.000 200 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
200 800 2400 3 0.18 1 49453 24726 809635 NA 940 3490912 0.000 200 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
200 800 2400 3 0.54 1 1803363 901681 2123342 NA 847148 14031588512 0.000 200 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
200 800 2400 3 0.37 1 335843 167921 1681387 NA 118117 490857288 0.000 200 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
200 800 2400 3 0.37 1 778833 389416 1563095 NA 340026 38848104956 0.000 200 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
200 800 2400 3 0.22 1 179553 89776 938984 NA 60733 3197659056 0.000 200 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
200 800 2400 3 0.66 1 1665213 832606 2676421 NA 756863 67920769248 0.000 200 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
200 800 2400 3 0.23 1 96433 48216 1015210 NA 16726 1276996896 0.000 200 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"

> for ((i=0; i < 10; ++i)); do BRG "800*100,4" "" $i | ctawSolver -cin -nil | ExtractTawSolver x; done
100 800 3200 4 3.62 1 2382147 1191073 18353307 NA 123590 5457416 0.000 100 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
100 800 3200 4 5.78 1 4392257 2196128 29459484 NA 404334 16090272 0.000 100 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
100 800 3200 4 3.99 1 3334543 1667271 20488962 NA 411855 18683237 0.000 100 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
100 800 3200 4 3.76 1 3159773 1579886 19023995 NA 402406 19667650 0.000 100 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
100 800 3200 4 3.95 1 3111455 1555727 19778414 NA 342073 16450437 0.000 100 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
100 800 3200 4 2.80 1 1913817 956908 14250864 NA 129347 5494515 0.000 100 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
100 800 3200 4 4.87 1 4167313 2083656 24453622 NA 557551 32967772 0.000 100 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
100 800 3200 4 3.09 1 2053697 1026848 15665702 NA 119523 4786961 0.000 100 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
100 800 3200 4 4.42 1 2989081 1494540 22553900 NA 182806 6789748 0.000 100 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
100 800 3200 4 3.41 1 2060103 1030051 16964511 NA 50468 1795908 0.000 100 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"

r> for ((i=0; i < 10; ++i)); do BRG "800*100,4" "" $i | cttawSolver -cin -nil | ExtractTawSolver x; done
100 800 3200 4 5.88 1 2489343 1244671 19117335 NA 122450 5457416 0.000 100 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
100 800 3200 4 9.23 1 4551619 2275809 30375951 NA 410332 16090272 0.000 100 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
100 800 3200 4 6.51 1 3505411 1752705 21440359 NA 417686 18683237 0.000 100 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
100 800 3200 4 6.05 1 3272125 1636062 19732596 NA 401944 19667650 0.000 100 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
100 800 3200 4 6.41 1 3259233 1629616 20620475 NA 348729 16450437 0.000 100 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
100 800 3200 4 4.48 1 1974773 987386 14492177 NA 131160 5494515 0.000 100 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
100 800 3200 4 7.46 1 4138391 2069195 23915178 NA 557745 32967772 0.000 100 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
100 800 3200 4 5.51 1 2251087 1125543 16966484 NA 123871 4786961 0.000 100 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
100 800 3200 4 7.45 1 3204943 1602471 23961400 NA 184196 6789748 0.000 100 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
100 800 3200 4 5.70 1 2203301 1101650 18012647 NA 49465 1795908 0.000 100 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"

> for ((i=0; i < 10; ++i)); do BRG "800*50,5" "" $i | ctawSolver -cin -nil | ExtractTawSolver x; done
50 800 4000 5 0.23 1 130717 65358 592633 NA 1563 16400 0.001 50 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
50 800 4000 5 0.20 1 110747 55373 511708 NA 467 5516 0.000 50 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
50 800 4000 5 0.16 1 86181 43090 398525 NA 274 3336 0.000 50 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
50 800 4000 5 0.21 1 116013 58006 529296 NA 569 6747 0.000 50 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
50 800 4000 5 0.19 1 109475 54737 498237 NA 1147 12905 0.000 50 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
50 800 4000 5 0.24 1 147753 73876 639044 NA 3217 36402 0.001 50 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
50 800 4000 5 0.21 1 117755 58877 541043 NA 739 8834 0.000 50 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
50 800 4000 5 0.22 1 121163 60581 557258 NA 404 4694 0.000 50 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
50 800 4000 5 0.22 1 130815 65407 564832 NA 2664 30727 0.000 50 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"
50 800 4000 5 0.20 1 113217 56608 520907 NA 837 10125 0.000 50 800 0 "-cin" ""A19"" "ctawSolver" "2.12.1" "4.85,1,0.354,0.11,0.0694;1.46"

> for ((i=0; i < 10; ++i)); do BRG "800*50,5" "" $i | cttawSolver -cin -nil | ExtractTawSolver x; done
50 800 4000 5 0.29 1 131807 65903 590594 NA 1534 16400 0.000 50 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
50 800 4000 5 0.27 1 112017 56008 510879 NA 462 5516 0.000 50 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
50 800 4000 5 0.22 1 91621 45810 419756 NA 267 3336 0.000 50 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
50 800 4000 5 0.29 1 121935 60967 549158 NA 556 6747 0.000 50 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
50 800 4000 5 0.27 1 114715 57357 517313 NA 1105 12905 0.000 50 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
50 800 4000 5 0.32 1 151807 75903 658305 NA 3139 36402 0.001 50 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
50 800 4000 5 0.28 1 122467 61233 556030 NA 765 8834 0.000 50 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
50 800 4000 5 0.29 1 125391 62695 569504 NA 411 4694 0.000 50 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
50 800 4000 5 0.28 1 131291 65645 564650 NA 2665 30727 0.000 50 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
50 800 4000 5 0.27 1 116977 58488 535207 NA 850 10125 0.000 50 800 0 "-cin" ""TA19"" "cttawSolver" "2.12.1" "7,1,0.31,0.19,0.111765;1.7"
\endverbatim
   </li>
  </ul>


  \todo Create tests
  <ul>
   <li> Only applications tests and fuzzing for now. </li>
   <li> As many variations of the tawSolver as possible should be tested;
   for the beginning at least the six "official versions" (SAT-solving,
   counting, marginal probabilities, with the tau-versions each). </li>
   <li> Perhaps app_tests should be renamed "tests". </li>
   <li> The application- and fuzzing tests inside are run by a
   dedicated makefile. </li>
   <li> The application-tests use the new tool "Matching". </li>
   <li> For fuzzing, a reasonably quick form of instances for all 6 solvers
   seems provided by e.g.
     BRG "50*50,2;50*50,3" "u,o" t | mttawSolver -cin -nil
   </li>
   <li> One such instance is produced for all solvers (such that one can
   at least check consistency). </li>
   <li> For the fuzzing-makefile, there are targets for all solvers (so that
   they can run in parallel) -- but that wouldn't work with creating one
   instance for all solvers (for cross-checking)? So parallelism needed
   perhaps to partition the instances instead. <li>
   <li> Say, running p*100 instances for p the number of parallel processes.
   The makefile for that best would be created (with each parallel target
   having its own extension "t,p" of the timestamp-seed). </li>
  </ul>


  \todo Create milestones


  \todo Compute proportion of search completed
  <ul>
   <li> As with the OKsolver, compute how much of the search has been
   completed. </li>
  </ul>


  \todo Positive / negative occurrences
  <ul>
   <li> A compile-time switch POLARITY (default: 0) is to be created, which
   when active extends the clause data-structure by not just determining
   the current total length, but by having, for active clauses, the number
   of remaining positive and negative literals in it. </li>
   <li> And there are then global counters for the total counter of positive
   and negative clauses. If one of them is zero, then the clause-set is
   satisfiable.
    <ol>
     <li> This event should be indicated in the output. </li>
     <li> Perhaps then the assignment is not extended, since this information
     is enough. </li>
     <li> Furthermore information on number of definite Horn (precisely one
     positive literal) and/or definite dual Horn (precisely one negative
     literal) are interesting, with the goal to determine the "distance" from
     a Horn resp. dual Horn clause-set. For this a further compile-time
     switch, call it BIHORN, is needed. </li>
    <ol>
   </li>
   <li> Change of class Clause:
    <ol>
     <li> Instead of data-members length_, old_length we then have length_,
     npos_, nneg_ (old_length would no longer be needed due to
     old_length = npos_ + nneg_). </li>
     <li> Member functions increment, decrement would then be split into two
     versions each. And there would be the additional member functions npos,
     nneg. </li>
    </ol>
   </li>
   <li> Apparently the only other changes to the algorithm would be member
   functions reactivate_0/1 in class ChangeManagement, which for "0" (i.e.,
     re-activating a literal) needed to distinguish the two polarities.
    <ol>
     <li> Where to get this information (to distinguish between the two cases)?
     </li>
    </ol>
   </li>
  </ul>


  \todo Provide information
  <ul>
   <li> On the ExternalSources page the data needs to be entered. </li>
   <li> http://sourceforge.net/projects/tawsolver/ </li>
  </ul>


  \todo Provide Doxygen documentation


  \todo Improving the clause-bitsets
  <ul>
   <li> The following should be transferred to a the general library.
   Otherwise this todo is completed (from the solver these tools were
   removed after version 1.3.7). </li>
   <li> http://graphics.stanford.edu/~seander/bithacks.html
   contains various bit-level operations. </li>
   </li>
   <li> At
   http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious
   we find (slightly adapted):
   \verbatim
// IF YOU KNOW v IS A POWER OF 2:
unsigned int v;  // 32-bit value to find the log2 of
const unsigned int b[] = {0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0, 0xFF00FF00, 0xFFFF0000};
unsigned int r = (v & b[0]) != 0;
for (i = 4; i > 0; --i) r |= ((v & b[i]) != 0) << i;

# The values b[0], ..., b[4]:
1010 1010 1010 1010 1010 1010 1010 1010
1100 1100 1100 1100 1100 1100 1100 1100
1111 0000 1111 0000 1111 0000 1111 0000
1111 1111 0000 0000 1111 1111 0000 0000
1111 1111 1111 1111 0000 0000 0000 0000
   \endverbatim
   </li>
   <li> The result computed is
     (bool(v&b[0]) << 0) | ... | (bool(v&b[4]) << 4)
   Note that the order doesn't matter. </li>
   <li> A proper C++ implementation of "log2s" ("s" for "special"):
   \verbatim
#incluce <limits>
#include <cassert>
#include <cmath>

constexpr int B {std::numeric_limits<unsigned int>::digits};
static_assert(B == 32, "Type \"unsigned int\" has not 32 bits.");
constexpr int N = 5;

constexpr int log2s(const unsigned int v) {
  assert((unsigned int) std::exp2(std::log2(v)) == v);
  constexpr unsigned int b[N] = {0xAAAAAAAA,0xCCCCCCCC,0xF0F0F0F0,0xFF00FF00,0xFFFF0000};
  unsigned int r = (v & b[0]) != 0;
  for (int i = 1; i < N; ++i) r |= ((v & b[i]) != 0) << i;
  return r;
}
   \endverbatim
   </li>
   <li> For B=64 (N=6) we have:
   \verbatim
const unsigned int b[N] = {0xAAAAAAAAAAAAAAAA, 0xCCCCCCCCCCCCCCCC, 0xF0F0F0F0F0F0F0F0, 0xFF00FF00FF00FF00, 0xFFFF00000FFFF0000, 0xFFFFFFFF00000000};
   \endverbatim
   </li>
   <li> A recursive computation of the array b of length N, via the values
   bp(N,0), ..., bp(N,N-1) ("bp" for bit pattern"), is:
   \verbatim
constexpr unsigned int pow2(const unsigned e) { return (e==0)?1:2*pow2(e-1); }
constexpr unsigned int pow22(const unsigned e) {return pow2(pow2(e));}

constexpr unsigned int bp(const unsigned N, const unsigned i) {
  // assert(N >= 0);
  // assert(i >= 0);
  // assert(i < N);
  return (i < N-1) ? bp(N-1,i) * (1 + pow22(N-1)) : pow22(N) - pow22(N-1);
}
   \endverbatim
   </li>
   <li> Then the general computation is:
   \verbatim
#include <limits>
#include <cassert>
#include <cmath>

constexpr int log2(const unsigned int n) { return (n <= 1) ? 0 : 1+log2(n/2); }

constexpr int B {std::numeric_limits<unsigned int>::digits};
constexpr unsigned int N = log2((unsigned int) B);
static_assert(pow2(N) == (unsigned int) B, "Number of bits in \"unsigned int\" not a power of 2.");

static_assert(N==5 or N==6, "Unexpected size of type \"unsigned int\".");
const unsigned int b[6] {bp(N,0),bp(N,1),bp(N,2),bp(N,3),bp(N,4), (N==6) ? bp(N,5) : 0};
// Unfortunately there is no reasonable way in C++ to just define b[N].

inline int log2s(const unsigned int v) {
  assert((unsigned int) std::exp2(std::log2(v)) == v);
  unsigned int r = (v & b[0]) != 0;
  for (unsigned int i = 1; i < N; ++i) r |= ((v & b[i]) != 0) << i;
  return r;
}
   \endverbatim
   </li>
   <li> As constexpr-functions:
   \verbatim
constexpr int log2sr(const unsigned v, const unsigned i) {
  // assert(i < N);
  return (i == 0) ? ((v & bp(N,0)) != 0)  : log2sr(v, i-1) | (((v & bp(N,i)) != 0) << i);
}
constexpr unsigned int log2sc(const unsigned int v) {
  return log2sr(v, N-1);
}
   \endverbatim
   (Remark: In OKs repository under "B149Bitposition.cpp" a more general,
   templatised version can be found.)
   </li>
  </ul>


  \todo Optimising weights
  <ul>
   <li> First considering VanDerWaerden_2-3-12_135.cnf. </li>
   <li> Start with current weights:
   \verbatim
TawSolver> ./RunWeights WEIGHT_2_CLAUSES 2 10 0.1 VanDerWaerden_2-3-12_135.cnf ""
> E1=read_satstat("Result_tawSolver")
> plot(E1$x,E1$nds)
> min(E1$nds)
[1] 975045
> E1[seq(29,37),]
     x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
29 4.8 135 5251  12 10.85   0 977343 488671 11497281   0 0.005
30 4.9 135 5251  12 10.81   0 975045 487522 11472643   0 0.004
31 5.0 135 5251  12 10.85   0 975603 487801 11477414   0 0.005
32 5.1 135 5251  12 10.89   0 975313 487656 11478357   0 0.004
33 5.2 135 5251  12 10.80   0 976191 488095 11490982   0 0.005
34 5.3 135 5251  12 10.76   0 976097 488048 11494725   0 0.004
35 5.4 135 5251  12 10.94   0 977215 488607 11510241   0 0.005
36 5.5 135 5251  12 10.79   0 977449 488724 11512264   0 0.004
37 5.6 135 5251  12 10.86   0 977495 488747 11515979   0 0.005

> plot(E2$x,E2$nds)
> min(E2$nds)
[1] 928059
> E2[seq(29,37),]
     x  rn   rc mcl     t sat    nds   bnds       r1  pls ptime
29 4.8 135 5251  12 18.66   0 929761 464880 11025046 1108 0.001
30 4.9 135 5251  12 18.76   0 929603 464801 11024841 1112 0.003
31 5.0 135 5251  12 18.82   0 928323 464161 11014210 1143 0.002
32 5.1 135 5251  12 18.75   0 928407 464203 11012720 1166 0.004
33 5.2 135 5251  12 18.66   0 929763 464881 11024475 1166 0.002
34 5.3 135 5251  12 18.73   0 929225 464612 11017499 1179 0.002
35 5.4 135 5251  12 18.69   0 928273 464136 11010516 1196 0.004
36 5.5 135 5251  12 18.77   0 928059 464029 11008118 1205 0.003
37 5.6 135 5251  12 18.87   0 929681 464840 11027249 1208 0.003

# x= 5.0 seems a reasonable compromise.

TawSolver> ./RunWeights WEIGHT_BASIS_OPEN 1.1 2.0 0.02 VanDerWaerden_2-3-12_135.cnf "-DWEIGHT_2_CLAUSES=5"

> E1=read_satstat("Result_tawSolver")
> plot(E1$x,E1$nds)
> min(E1$nds)
[1] 966469
> E1[seq(16,26),]
      x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
16 1.40 135 5251  12 10.83   0 982193 491096 11580933   0 0.003
17 1.42 135 5251  12 10.86   0 977437 488718 11521053   0 0.001
18 1.44 135 5251  12 10.82   0 972857 486428 11460593   0 0.004
19 1.46 135 5251  12 10.67   0 967827 483913 11397547   0 0.004
20 1.48 135 5251  12 10.72   0 969641 484820 11417859   0 0.005
21 1.50 135 5251  12 10.75   0 966469 483234 11376615   0 0.001
22 1.52 135 5251  12 10.92   0 979757 489878 11546452   0 0.004
23 1.54 135 5251  12 10.85   0 980043 490021 11555949   0 0.005
24 1.56 135 5251  12 10.84   0 980257 490128 11544503   0 0.005
25 1.58 135 5251  12 10.91   0 976889 488444 11501144   0 0.004
26 1.60 135 5251  12 10.82   0 975603 487801 11477414   0 0.005

> E2=read_satstat("Result_ttawSolver")
> plot(E2$x,E2$nds)
> min(E2$nds)
[1] 928323

> E2[seq(20,30),]
      x  rn   rc mcl     t sat    nds   bnds       r1  pls ptime
20 1.48 135 5251  12 19.04   0 939901 469950 11163329 1165 0.004
21 1.50 135 5251  12 18.99   0 937969 468984 11134761 1163 0.003
22 1.52 135 5251  12 18.98   0 936247 468123 11110328 1163 0.002
23 1.54 135 5251  12 18.92   0 933985 466992 11085816 1147 0.003
24 1.56 135 5251  12 18.79   0 931153 465576 11051506 1139 0.002
25 1.58 135 5251  12 18.71   0 929917 464958 11035018 1150 0.003
26 1.60 135 5251  12 18.76   0 928323 464161 11014210 1143 0.002
27 1.62 135 5251  12 18.69   0 929299 464649 11024378 1149 0.002
28 1.64 135 5251  12 18.78   0 930451 465225 11035708 1154 0.002
29 1.66 135 5251  12 18.64   0 930383 465191 11031771 1170 0.003
30 1.68 135 5251  12 18.84   0 932059 466029 11046036 1167 0.002

# Here we have a clear difference: 1.5 for tawSolver, 1.6 for ttawSolver.
   \endverbatim
   </li>
   <li> With updated WEIGHT_2_CLAUSES, WEIGHT_BASIS_OPEN:
   \verbatim
TawSolver> ./RunWeights WEIGHT_5_CLAUSES 0.01 0.30 0.005 VanDerWaerden_2-3-12_135.cnf ""
> E1[seq(16,24),]
       x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
16 0.085 135 5251  12 10.68   0 967127 483563 11361843   0 0.005
17 0.090 135 5251  12 10.69   0 964125 482062 11333077   0 0.005
18 0.095 135 5251  12 10.60   0 963757 481878 11332793   0 0.004
19 0.100 135 5251  12 10.60   0 963365 481682 11330147   0 0.004
20 0.105 135 5251  12 10.65   0 963307 481653 11329159   0 0.002
21 0.110 135 5251  12 10.68   0 963193 481596 11334130   0 0.005
22 0.115 135 5251  12 10.63   0 963755 481877 11339048   0 0.005
23 0.120 135 5251  12 10.65   0 964259 482129 11348183   0 0.004
24 0.125 135 5251  12 10.67   0 965907 482953 11369838   0 0.004

> E2[seq(20,30),]
       x  rn   rc mcl     t sat    nds   bnds       r1  pls ptime
20 0.105 135 5251  12 18.83   0 932671 466335 11052740 1120 0.005
21 0.110 135 5251  12 18.79   0 931927 465963 11048695 1141 0.004
22 0.115 135 5251  12 18.63   0 930391 465195 11022002 1167 0.003
23 0.120 135 5251  12 18.72   0 929233 464616 11018405 1166 0.001
24 0.125 135 5251  12 18.87   0 928329 464164 11008388 1160 0.001
25 0.130 135 5251  12 18.81   0 928323 464161 11014210 1143 0.002
26 0.135 135 5251  12 18.72   0 929153 464576 11024301 1146 0.003
27 0.140 135 5251  12 18.75   0 929465 464732 11032722 1120 0.003
28 0.145 135 5251  12 18.87   0 930241 465120 11044985 1122 0.002
29 0.150 135 5251  12 18.91   0 931741 465870 11061705 1129 0.002
30 0.155 135 5251  12 18.86   0 931613 465806 11064423 1135 0.002

# tawsolver: x=0.11, ttawsolver: x=0.127
   \endverbatim
   </li>
   <li> With updated WEIGHT_2, WEIGHT_BASIS_OPEN, WEIGHT_5:
   \verbatim
TawSolver> ./RunWeights WEIGHT_4_CLAUSES 0.15 0.45 0.01 VanDerWaerden_2-3-12_135.cnf ""
# Multiple optima; rerunning with higher resolution
TawSolver> ./RunWeights WEIGHT_4_CLAUSES 0.25 0.40 0.001 VanDerWaerden_2-3-12_135.cnf ""
> E1[E1$nds==min(E1$nds),]
       x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
92 0.341 135 5251  12 10.78   0 962757 481378 11325664   0 0.005
> E2[E2$nds==min(E2$nds),]
       x  rn   rc mcl    t sat    nds   bnds       r1  pls ptime
46 0.295 135 5251  12 18.7   0 927705 463852 11007465 1179 0.002
   \endverbatim
   </li>
   <li> With updated WEIGHT_BASIS_OPEN, WEIGHT_5, WEIGHT_4:
   \verbatim
TawSolver> ./RunWeights WEIGHT_2_CLAUSES 4 6 0.05 VanDerWaerden_2-3-12_135.cnf ""
> E1[E1$nds==min(E1$nds),]
      x  rn   rc mcl    t sat    nds   bnds       r1 pls ptime
18 4.85 135 5251  12 10.7   0 962609 481304 11322162   0 0.005
> E2[E2$nds==min(E2$nds),]
   x  rn   rc mcl    t sat    nds   bnds       r1  pls ptime
21 5 135 5251  12 18.7   0 927705 463852 11007465 1179 0.003
   \endverbatim
   </li>
   <li> The minima for the tau-version are always much sharper and much less
   noisy. </li>
   <li> Re-examining WEIGHT_4:
   \verbatim
TawSolver> ./RunWeights WEIGHT_4_CLAUSES 0.27 0.37 0.001 VanDerWaerden_2-3-12_135.cnf ""
> E1[E1$nds==min(E1$nds),]
       x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
85 0.354 135 5251  12 10.64   0 962181 481090 11319517   0 0.003
> E2[E2$nds==min(E2$nds),]
       x  rn   rc mcl     t sat    nds   bnds       r1  pls ptime
26 0.295 135 5251  12 18.79   0 927705 463852 11007465 1179 0.002

# Value for tawSolver very unclear.
   \endverbatim
   </li>
   <li> Re-examining WEIGHT_5:
   \verbatim
TawSolver> ./RunWeights WEIGHT_5_CLAUSES 0.09 0.15 0.001 VanDerWaerden_2-3-12_135.cnf ""
> E1[E1$nds==min(E1$nds),]
      x  rn   rc mcl    t sat    nds   bnds       r1 pls ptime
21 0.11 135 5251  12 10.6   0 962181 481090 11319517   0 0.005
> E2[E2$nds==min(E2$nds),]
       x  rn   rc mcl    t sat    nds   bnds       r1  pls ptime
33 0.122 135 5251  12 18.9   0 927587 463793 11002176 1184 0.004
   \endverbatim
   </li>
   <li> Determining WEIGHT_6:
   \verbatim
TawSolver> ./RunWeights WEIGHT_6 0.02 0.12 0.001 VanDerWaerden_2-3-12_135.cnf ""
> E1[E1$nds==min(E1$nds),]
       x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
50 0.069 135 5251  12 10.69   0 962185 481092 11306038   0 0.004
> E2[E2$nds==min(E2$nds),]
       x  rn   rc mcl     t sat    nds   bnds       r1  pls ptime
58 0.077 135 5251  12 18.75   0 927685 463842 11004258 1183 0.004

TawSolver> ./RunWeights WEIGHT_6 0.067 0.08 0.0001 VanDerWaerden_2-3-12_135.cnf ""
> E1[E1$nds==min(E1$nds),]
        x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
25 0.0694 135 5251  12 10.65   0 962115 481057 11306120   0 0.005
> E2[E2$nds==min(E2$nds),]
        x  rn   rc mcl     t sat    nds   bnds       r1  pls ptime
87 0.0756 135 5251  12 18.84   0 927419 463709 10999237 1174 0.002
   \endverbatim
   <li>
   <li> Determine WEIGHT_BASIS_OPEN:
   \verbatim
TawSolver> ./RunWeights WEIGHT_BASIS_OPEN 1.3 1.8 0.01 VanDerWaerden_2-3-12_135.cnf ""
> E1[E1$nds==min(E1$nds),]
      x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
17 1.46 135 5251  12 10.54   0 961949 480974 11312180   0 0.005
> E2[E2$nds==min(E2$nds),]
     x  rn   rc mcl     t sat    nds   bnds       r1  pls ptime
31 1.6 135 5251  12 18.76   0 927419 463709 10999237 1174 0.002
   \endverbatim
   </li>
   <li> The weights we got are:
   \verbatim
tawSolver:
4.85 0.354 0.11 0.0694; 1.46
ttawSolver:
5 0.295 0.122 0.0756; 1.6
   \endverbatim
   Apparently these new settings do not improve node-counts for k >= 13. </li>
   <li> Now considering VanDerWaerden_2-3-13_160.cnf. Currently we have
   \verbatim
c number_of_variables                   160
c number_of_clauses                     7308
c maximal_clause_length                 13
c number_of_literal_occurrences         31804
c running_time(sec)                     83.33
c number_of_nodes                       5638667
c number_of_binary_nodes                2819333
c number_of_1-reductions                71942645
c reading-and-set-up_time(sec)          0.006

c running_time(sec)                     148.86
c number_of_nodes                       5824621
c number_of_binary_nodes                2912310
c number_of_1-reductions                74713798
c number_of_pure_literals               7371
c reading-and-set-up_time(sec)          0.002
   \endverbatim
   </li>
   <li> Optimising WEIGHT_2 (alone):
   \verbatim
TawSolver> ./RunWeights WEIGHT_2 4 6 0.1 VanDerWaerden_2-3-13_160.cnf ""
> E1[E1$nds==min(E1$nds),]
     x  rn   rc mcl     t sat     nds    bnds       r1 pls ptime
18 5.7 160 7308  13 87.11   0 5633957 2816978 71929506   0 0.002
> E2[E2$nds==min(E2$nds),]
     x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
12 5.1 160 7308  13 144.86   0 5819401 2909700 74659091 7424 0.003
   \endverbatim
   </li>
   <li> Optimising WEIGHT_4 (alone):
   \verbatim
TawSolver> ./RunWeights WEIGHT_4 0.2 0.5 0.01 VanDerWaerden_2-3-13_160.cnf ""
> E1[E1$nds==min(E1$nds),]
      x  rn   rc mcl     t sat     nds    bnds       r1 pls ptime
13 0.32 160 7308  13 90.13   0 5637911 2818955 71955323   0 0.003
> E2[E2$nds==min(E2$nds),]
      x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
14 0.33 160 7308  13 154.98   0 5822929 2911464 74662535 7316 0.003
   \endverbatim
   </li>
   <li> Optimising WEIGHT_5 (alone):
   \verbatim
TawSolver> ./RunWeights WEIGHT_5 0.08 0.15 0.002 VanDerWaerden_2-3-13_160.cnf ""
> E1[E1$nds==min(E1$nds),]
       x  rn   rc mcl     t sat     nds    bnds       r1 pls ptime
34 0.146 160 7308  13 90.03   0 5619049 2809524 71690988   0 0.002
> E2[E2$nds==min(E2$nds),]
       x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
14 0.106 160 7308  13 157.76   0 5821811 2910905 74693913 7438 0.002
TawSolver> ./RunWeights WEIGHT_5 0.08 0.19 0.001 VanDerWaerden_2-3-13_160.cnf ""
XXX
   \endverbatim
   </li>
   <li> Now considering VanDerWaerden_pd_2-3-21_405.cnf Currently we have
   \verbatim
s UNSATISFIABLE
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     101.82
c number_of_nodes                       2239371
c number_of_binary_nodes                1119685
c number_of_1-reductions                32017061
c reading-and-set-up_time(sec)          0.009
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf

c running_time(sec)                     136.52
c number_of_nodes                       2124123
c number_of_binary_nodes                1062061
c number_of_1-reductions                31939538
c number_of_pure_literals               127
c reading-and-set-up_time(sec)          0.009
   \endverbatim
   </li>
   <li> Optimising WEIGHTs for ttawSolver (basically ignoring E1):
   \verbatim
TawSolver> ./RunWeights WEIGHT_2 4 6 0.1 VanDerWaerden_pd_2-3-21_405.cnf ""
> E1[E1$nds==min(E1$nds),]
     x  rn    rc mcl     t sat     nds    bnds       r1 pls ptime
19 5.8 203 21950  21 96.35   0 2204753 1102376 31621828   0 0.009
> E2[E2$nds==min(E2$nds),]
     x  rn    rc mcl      t sat     nds    bnds       r1 pls ptime
16 5.5 203 21950  21 122.86   0 2109955 1054977 31725127 119 0.009

TawSolver> ./RunWeights WEIGHT_4 0.2 0.4 0.01 VanDerWaerden_pd_2-3-21_405.cnf "-DWEIGHT_2=5.5"
> E1[E1$nds==min(E1$nds),]
     x  rn    rc mcl     t sat     nds    bnds       r1 pls ptime
2 0.21 203 21950  21 96.09   0 2204367 1102183 31825416   0 0.011
> E2[E2$nds==min(E2$nds),]
      x  rn    rc mcl      t sat     nds    bnds       r1 pls ptime
12 0.31 203 21950  21 121.91   0 2109419 1054709 31684244 121  0.01

TawSolver> ./RunWeights WEIGHT_5 0.1 0.15 0.002 VanDerWaerden_pd_2-3-21_405.cnf "-DWEIGHT_2=5.5 -DWEIGHT_4=0.31"
> E1[E1$nds==min(E1$nds),]
      x  rn    rc mcl     t sat     nds    bnds       r1 pls ptime
5 0.108 203 21950  21 92.94   0 2206953 1103476 31704707   0 0.009
> E2[E2$nds==min(E2$nds),]
       x  rn    rc mcl     t sat     nds    bnds       r1 pls ptime
12 0.122 203 21950  21 117.3   0 2109419 1054709 31684244 121  0.01

TawSolver> ./RunWeights WEIGHT_6 0.05 0.09 0.001 VanDerWaerden_pd_2-3-21_405.cnf "-DWEIGHT_2=5.5 -DWEIGHT_4=0.31 -DWEIGHT_5=0.122"
> E1[E1$nds==min(E1$nds),]
       x  rn    rc mcl     t sat     nds    bnds       r1 pls ptime
15 0.064 203 21950  21 89.23   0 2200497 1100248 31525253   0 0.015
> E2[E2$nds==min(E2$nds),]
      x  rn    rc mcl      t sat     nds    bnds       r1 pls ptime
7 0.056 203 21950  21 111.81   0 2108175 1054087 31297261 139  0.01
TawSolver> ./RunWeights WEIGHT_6 0.03 0.06 0.0005 VanDerWaerden_pd_2-3-21_405.cnf "-DWEIGHT_2=5.5 -DWEIGHT_4=0.31 -DWEIGHT_5=0.122"
> E1[E1$nds==min(E1$nds),]
       x  rn    rc mcl     t sat     nds    bnds       r1 pls ptime
55 0.057 203 21950  21 89.83   0 2206717 1103358 31532243   0 0.013
> E2[E2$nds==min(E2$nds),]
        x  rn    rc mcl      t sat     nds    bnds       r1 pls ptime
32 0.0455 203 21950  21 111.06   0 2093661 1046830 30867447 154 0.011

TawSolver> ./RunWeights WEIGHT_BASIS_OPEN 1.2 2.0 0.01 VanDerWaerden_pd_2-3-21_405.cnf "-DWEIGHT_2=5.5 -DWEIGHT_4=0.31 -DWEIGHT_5=0.122 -DWEIGHT_6=0.0455"
> E1[E1$nds==min(E1$nds),]
      x  rn    rc mcl     t sat     nds    bnds       r1 pls ptime
23 1.42 203 21950  21 87.84   0 2154305 1077152 30949526   0 0.015
                              file
23 VanDerWaerden_pd_2-3-21_405.cnf
> E2[E2$nds==min(E2$nds),]
     x  rn    rc mcl      t sat     nds    bnds       r1 pls ptime
41 1.6 203 21950  21 110.35   0 2093661 1046830 30867447 154  0.01
   \endverbatim
   So the result of this first run is 5.5, 0.31, 0.122, 0.0455, 1.6.
   </li>
   <li> Let's use that as a first attempt at improved weights for the
   tau-heuristics. </li>
   <li> Second round of optimisations:
   \verbatim
TawSolver> ./RunWeights TWEIGHT_2 5 6 0.01 VanDerWaerden_pd_2-3-21_405.cnf "" ttawSolver
> E[E$nds==min(E$nds),]
     x  rn    rc mcl      t sat     nds    bnds       r1 pls ptime
91 5.9 203 21950  21 117.73   0 2091279 1045639 30791742 134  0.01
   \endverbatim
   No clear minimum.
   </li>
   <li> Perhaps the palindromic instances are not good for optimising the
   general constants, since they are already "too special" ? </li>
   <li> Thus now optimising the tau-weights for VanDerWaerden_2-3-13_160.cnf.
   Currently we have:
   \verbatim
s UNSATISFIABLE
c number_of_variables                   160
c number_of_clauses                     7308
c maximal_clause_length                 13
c number_of_literal_occurrences         31804
c running_time(sec)                     146.12
c number_of_nodes                       5862905
c number_of_binary_nodes                2931452
c number_of_1-reductions                75076427
c number_of_pure_literals               7717
c reading-and-set-up_time(sec)          0.003
c file_name                             VanDerWaerden_2-3-13_160.cnf
   \endverbatim
   </li>
   <li> Complete round of optimisations:
   \verbatim
TawSolver> ./RunWeights TWEIGHT_2 4 7 0.05 VanDerWaerden_2-3-13_160.cnf "" ttawSolver
> E[E$nds==min(E$nds),]
      x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
24 5.15 160 7308  13 144.51   0 5851899 2925949 74933775 7568 0.001

TawSolver> ./RunWeights TWEIGHT_4 0.2 0.4 0.005 VanDerWaerden_2-3-13_160.cnf "-DTWEIGHT_2=5.15" ttawSolver
> E[E$nds==min(E$nds),]
       x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
22 0.305 160 7308  13 144.65   0 5851469 2925734 74929246 7574 0.003

TawSolver> ./RunWeights TWEIGHT_5 0.1 0.15 0.001 VanDerWaerden_2-3-13_160.cnf "-DTWEIGHT_2=5.15 -DTWEIGHT_4=0.305" ttawSolver
> E[E$nds==min(E$nds),]
       x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
18 0.117 160 7308  13 144.74   0 5851209 2925604 74917730 7585 0.003

TawSolver> ./RunWeights TWEIGHT_6 0.03 0.08 0.001 VanDerWaerden_2-3-13_160.cnf "-DTWEIGHT_2=5.15 -DTWEIGHT_4=0.305 -DTWEIGHT_5=0.117" ttawSolver
> E[E$nds==min(E$nds),]
       x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
27 0.056 160 7308  13 139.33   0 5776755 2888377 73878997 7414 0.007

TawSolver> ./RunWeights TWEIGHT_BASIS_OPEN 1.3 1.9 0.01 VanDerWaerden_2-3-13_160.cnf "-DTWEIGHT_2=5.15 -DTWEIGHT_4=0.305 -DTWEIGHT_5=0.117 -DTWEIGHT_6=0.056" ttawSolver
> E[E$nds==min(E$nds),]
     x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
31 1.6 160 7308  13 138.69   0 5776755 2888377 73878997 7414 0.006
XXX
   \endverbatim
   </li>
  </ul>


  \todo Considering XOR-instances
  <ul>
   <li> On the instances TwoXORclauses-20.cnf etc. (see
   Experimentation/Investigations/BooleanFunctions/plans/Hardness/data/TwoXORclauses/Tawsolver )
   the optimisation-options behave differently, and also UCP-strategy 0 is now
   faster. </li>
  </ul>


  \todo Removing superfluous spaces from error-messages
  <ul>
   <li> Lines of errout likely should not start with a space, since they
        are indented (uniformly) anyway; this holds especially for
        continuations, and with
        \verbatim
    errout << "More than " << n_header_clauses << " clauses, contradicting cnf-header.";
        \endverbatim
   </li>
  </ul>

*/
