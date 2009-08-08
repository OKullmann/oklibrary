// Oliver Kullmann, 24.7.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/HindmanProblems/plans/Hindman_2^a(2).hpp
  \brief Investigating a -> hindmani_2^a(2)

  These problems are generated in Maxima by output_hindmani_r2k2_stdname


  \todo Investigate the non-injective versions
  

  \todo hindmani_2^1(2) = 252
  <ul>
   <li> n=252, c=1424, c/n ~ 5.65 </li>
   <li> Trivial for example for OKsolver_2002:
   \verbatim
> OKsolver_2002-O3-DNDEBUG Hindmani_r2k2_1_251.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=4 initial_number_of_variables=251 initial_number_of_clauses=1406 initial_number_of_literal_occurrences=5126 running_time(s)=0.0 number_of_nodes=6 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=17 number_of_pure_literals=67 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=3 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=88 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindmani_r2k2_1_251.cnf
> OKsolver_2002-O3-DNDEBUG Hindmani_r2k2_1_252.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=4 initial_number_of_variables=252 initial_number_of_clauses=1424 initial_number_of_literal_occurrences=5196 running_time(s)=0.0 number_of_nodes=11 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=36 number_of_pure_literals=98 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=3 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=170 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindmani_r2k2_1_252.cnf
   \endverbatim
   </li>
   <li> We should also count all solutions, and classify them. </li>
  </ul>


  \todo hindmani_2^2(2) = 990
  </ul>
   <li> n=914, c=4988, c/n ~ 5.46 </li>
   <li> OKsolver_2002
    \verbatim
kullmann-0:OKplatform> OKsolver_2002-O3-DNDEBUG Hindmani_r2k2_2_989.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=4 initial_number_of_variables=913 initial_number_of_clauses=4966 initial_number_of_literal_occurrences=19864 running_time(s)=0.1 number_of_nodes=130 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=581 number_of_pure_literals=1023 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=15 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=17213 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindmani_r2k2_2_989.cnf
kullmann-0:OKplatform> OKsolver_2002-O3-DNDEBUG Hindmani_r2k2_2_990.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=4 initial_number_of_variables=914 initial_number_of_clauses=4988 initial_number_of_literal_occurrences=19952 running_time(s)=1.6 number_of_nodes=2291 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=10006 number_of_pure_literals=14514 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=17 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=282822 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindmani_r2k2_2_990.cnf
    \endverbatim
   </li>
  </ul>


  \todo hindmani_2^3(2) = 3150
  <ul>
   <li> n=2725, c=16378, c/n ~ 6.01 </li>
   <li> minisat2
> minisat2 Hindmani_r2k2_3_3149.cnf
restarts              : 13
conflicts             : 38361          (2808 /sec)
decisions             : 42749          (2.03 % random) (3129 /sec)
propagations          : 2406216        (176126 /sec)
conflict literals     : 502210         (35.14 % deleted)
Memory used           : 4.34 MB
CPU time              : 13.6619 s
SATISFIABLE
> minisat2 Hindmani_r2k2_3_3150.cnf
restarts              : 16
conflicts             : 91478          (2250 /sec)
decisions             : 100937         (1.98 % random) (2482 /sec)
propagations          : 5733320        (141007 /sec)
conflict literals     : 1104659        (38.96 % deleted)
Memory used           : 4.64 MB
CPU time              : 40.6598 s
UNSATISFIABLE
    \endverbatim
   </li>
  </ul>


  \todo hindmani_2^4(2) = 5600
  <ul>
   <li> n=4598, c=28618, c/n ~ 6.22 </li>
   <li> minisat2
   \verbatim
> minisat2 Hindmani_r2k2_4_5599.cnf
restarts              : 11
conflicts             : 14960          (1504 /sec)
decisions             : 18767          (1.88 % random) (1887 /sec)
propagations          : 1242672        (124961 /sec)
conflict literals     : 248023         (30.22 % deleted)
SATISFIABLE
> minisat2 Hindmani_r2k2_4_5600.cnf
restarts              : 20
conflicts             : 562200         (3346 /sec)
decisions             : 623136         (1.96 % random) (3709 /sec)
propagations          : 43509735       (258975 /sec)
conflict literals     : 7975913        (37.90 % deleted)
UNSATISFIABLE
   \endverbatim
   </li>
   <li> OKsolver_2002-m2pp finds a solution for 5599 in ~ 3000 nodes
   (say, 2 m); unsatisfiability is harder, but doable in, say, 1 h:
   \verbatim
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=19 initial_number_of_variables=2312 initial_number_of_clauses=24676 initial_number_of_literal_occurrences=106840 running_time(s)=1757.9 number_of_nodes=85979 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=643340 number_of_pure_literals=181686 number_of_autarkies=0 number_of_missed_single_nodes=4 max_tree_depth=28 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=8501164 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindmani_r2k2_4_5600.cnf_m2pp_12733
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=16 initial_number_of_variables=2493 initial_number_of_clauses=26858 initial_number_of_literal_occurrences=116064 running_time(s)=-1444.1 number_of_nodes=84627 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=621512 number_of_pure_literals=178792 number_of_autarkies=0 number_of_missed_single_nodes=4 max_tree_depth=27 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=8864762 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindmani_r2k2_4_6000.cnf_m2pp_11324
   \endverbatim
   </li>
   <li> The minisat2-preprocessing seems to help OKsolver_2002 somewhat:
   \verbatim
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=4 initial_number_of_variables=4598 initial_number_of_clauses=28618 initial_number_of_literal_occurrences=114472 running_time(s)=-1470.6 number_of_nodes=110501 number_of_single_nodes=2 number_of_quasi_single_nodes=0 number_of_2-reductions=809722 number_of_pure_literals=2464488 number_of_autarkies=0 number_of_missed_single_nodes=6 max_tree_depth=28 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=78884356 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindmani_r2k2_4_5600.cnf
   \endverbatim
   </li>
  </ul>


  \todo hindmani_2^5(2) = 14364
  <ul>
   <li> n=11534, c=79800, c/n ~ 6.92 </li>
   <li> From the ubcsat-1.0.0-suite it seems rsaps is most effective:
    <ol>
     <li> With cutoff=10^6 in 7 from 10 runs it found a solution. </li>
     <li> With a cutoff=10^5 walksat seemed more efficient, but if failed
     to find a solution in 10 runs with cutoff=10^6. </li>
     <li> Interestingly, walksat-tabu has high (i.e., bad) min-averages,
     but also a large deviation, so that it found one solution with
     cutoff=10^6. </li>
    </ol>
   </li>
   <li> minisat2 seems most efficient, on satisfiable as well as on
   unsatisfiable instances. </li>
   <li>
   \verbatim
> minisat2 Hindmani_r2k2_5_14363.cnf
restarts              : 18
conflicts             : 266053         (1499 /sec)
decisions             : 298233         (1.99 % random) (1680 /sec)
propagations          : 25902069       (145954 /sec)
conflict literals     : 5067946        (28.93 % deleted)
SATISFIABLE
> minisat2 Hindmani_r2k2_5_14364.cnf
restarts              : 24
conflicts             : 3223003        (557 /sec)
decisions             : 3570627        (1.99 % random) (618 /sec)
propagations          : 298912519      (51697 /sec)
conflict literals     : 51860486       (37.08 % deleted)
UNSATISFIABLE
   \endverbatim
   </li>
   <li> OKsolver_2002-m2pp
   \verbatim
> OKsolver_2002-m2pp -D10 -M Hindmani_r2k2_5_14364.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=12 initial_number_of_variables=5539 initial_number_of_clauses=70134 initial_number_of_literal_occurrences=304920 running_time(s)=-1526.9 number_of_nodes=516451 number_of_single_nodes=4 number_of_quasi_single_nodes=0 number_of_2-reductions=4497536 number_of_pure_literals=1499630 number_of_autarkies=0 number_of_missed_single_nodes=4 max_tree_depth=30 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=82701806 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindmani_r2k2_5_14364.cnf_m2pp_5223
   \endverbatim
   It seems that the advantage of minisat2 is just that expensive (and
   apparently ineffective) look-aheads are avoided --- but nothing in
   the direction of "resolution is more powerful than tree-resolution". </li>
   <li> satz2-m2pp
   \verbatim
**** The instance is unsatisfiable. *****
NB_MONO= 21479226, NB_UNIT= 241108531, NB_BRANCHE= 2944242, NB_BACK= 1488069
Program terminated in 5.762 seconds.
satz215 Hindmani_r2k2_5_14364.cnf_m2pp_8170 5.762 2944242 1488069 1367706445 31952092 0 14364 70134 0 199693920 19963489
   \endverbatim
   Roughly comparable to OKsolver_2002-m2pp.
   </li>
   <li> march_pl
   \verbatim
> march_pl Hindmani_r2k2_5_14364.cnf
c main():: nodeCount: 149162
c main():: dead ends in main: 87
c main():: lookAheadCount: 126655586
c main():: unitResolveCount: 951518
c main():: time=-850.774592
c main():: necessary_assignments: 13848
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 1290425, succes #: 1084725
c main():: doublelook: overall 1.023 of all possible doublelooks executed
c main():: doublelook: succesrate: 84.060, average DL_trigger: 507.780
s UNSATISFIABLE
   \endverbatim
   So march_pl is the most successful look-ahead solver.
   </li>
  </ul>


  \todo hindmani_2^6(2) = 28188
  <ul>
   <li> n=22205, c=164334, c/n ~ 7.40 </li>
   <li> n=28187 found easily satisfiable with rsaps (cutoff=10^5, 10 runs,
   90% success). </li>
   <li> n=28188 unsatisfiable:
    <ol>
     <li> 10 runs with cutoff=10^5 and 10 runs with cutoff=10^6 yield all
     (just) min=1. </li>
     <li> minisat2 determined unsatisfiability in 26 restarts (7534086
     conflicts, 8426651 decisions; say around 10 hours). </li>
     <li> picosat913:
      <ul>
       <li> Without preprocessor:
       \verbatim
c 7 iterations
c 15183 restarts
c 1 failed literals
c 14089448 conflicts
c 15689408 decisions
c 966 fixed variables
c 261431676 learned literals
c 34.8% deleted literals
c 2580396449 propagations
c 38.6% variables used
c 28007.9 seconds in library
c 0.1 megaprops/second
c 4 simplifications
c 16 reductions
c 1240.6 MB recycled
c 119.7 MB maximally allocated
c 28007.9 seconds total run time
       \endverbatim
       roughly 8 hours. </li>
       <li> Using the minisat2 preprocessor:
       \verbatim
c 6 iterations
c 20991 restarts
c 0 failed literals
c 19611716 conflicts
c 21856001 decisions
c 29 fixed variables
c 367241397 learned literals
c 34.1% deleted literals
c 3527076464 propagations
c 32.9% variables used
c 39559.1 seconds in library
c 0.1 megaprops/second
c 3 simplifications
c 24 reductions
c 1754.7 MB recycled
c 110.4 MB maximally allocated
c 39559.1 seconds total run time
       \endverbatim
       Actually, it performs worse than without preprocessing. </li>
      </ul>
     </li>
     <li> march_pl
     \verbatim
> march_pl Hindmani_r2k2_6_28188.cnf
c main():: nodeCount: 555857
c main():: dead ends in main: 240
c main():: lookAheadCount: 774198284
c main():: unitResolveCount: 4046431
c main():: time=-1.785920
c main():: necessary_assignments: 75369
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 5599383, succes #: 4745621
c main():: doublelook: overall 0.725 of all possible doublelooks executed
c main():: doublelook: succesrate: 84.753, average DL_trigger: 832.682
s UNSATISFIABLE
     \endverbatim
     </li>
     <li> OKsolver_2002-m2pp needed 5 days:
     \verbatim
> OKsolver_2002-m2pp -D20 -M Hindmani_r2k2_6_28188.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=14 initial_number_of_variables=10708 initial_number_of_clauses=145336 initial_number_of_literal_occurrences=624542 running_time(s)=437409.1 number_of_nodes=1754977 number_of_single_nodes=30 number_of_quasi_single_nodes=0 number_of_2-reductions=18313260 number_of_pure_literals=8277966 number_of_autarkies=0 number_of_missed_single_nodes=2 max_tree_depth=33 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=496260936 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindmani_r2k2_6_28188.cnf_m2pp_1000
     \endverbatim
     Why does march_pl need fewer nodes?
     </li>
    </ol>
   </li>
   <li> n=30000
    <ol>
     <li> rsaps in 10 runs with cutoff=10^5,10^6 always found min=1, while
     minisat2 made apparently no progress after 23 restarts. So let's assume
     this problem is unsatisfiable. </li>
    </ol>
   </li>
  </ul>


  \todo hindmani_2^7(2) = 53312
  <ul>
   <li> n=41497, c=327078, c/n ~ 7.88 </li>
   <li> Using rsaps for satisfiability detection. </li>
   <li> n=53311 found easily satisfiable (10 runs with cutoff=10^4 yield 100%
   success). </li>
   <li> It seems that the satisfiable problems become easier with increasing
   start-value! </li>
   <li> n=53312 looks unsatisfiable (10 runs with cutoffs 10^4, 10^5 and 10^6
   each yield always min=1). </li>
   <li> minisat2 determined unsatisfiability in 30 restarts (30602289
   conflicts, 33994511 decisions; 226393s (~ 2 1/2 days). </li>
   <li> picosat913 </li>
   <li> march_pl </li>
   <li> OKsolver_2002-m2pp
    <ol>
     <li> After observing 5808 nodes at depth 20, the
     prediction is 24*10^6 nodes, and about 6 months (at cs-oksvr). </li>
     <li> Different from a=8 (see below), here patterns are hardly visible.
     </li>
    </ol>
   </li>
  </ul>


  \todo hindmani_2^8(2) = 97280
  <ul>
   <li> n=75151, c=627618, c/n ~ 8.35 </li>
   <li> Using rsaps for satisfiability detection. </li>
   <li> n=97279 found easily satisfiable (10 runs with cutoff=2*10^4 yield
   100% success). </li>
   <li> n=97280 appears to be unsatisfiable: constant min=1 for cutoff=2*10^4
   as for 10^5 and 10^6). </li>
   <li> minisat2 determined unsatisfiability in 30 restarts (27807987
   conflicts, 31114037 decisions, 351550s (4 days). </li>
   <li> picosat913 </li>
   <li> OKsolver_2002-m2pp:
    <ol>
     <li> After observing 15900 nodes at depth 24, the prediction is
     90*10^6 nodes (an average of 5.37 nodes per observation node), and about
     4 years. </li>
     <li> After 90000 nodes, the prediction is 62*10^6 nodes (average 3.72
     nodes per observation node), and about 2 1/2 years. </li>
     <li> The distribution of node-counts over the observation-nodes (ordered
     as they were created) shows a periodic patterns, easy segments
     interspersed with hard segments.
      <ol>
       <li> Of course, this all has to do with the switch between different
       subtrees, and some are apparently hard, and some easy --- however there
       seems to be a non-trivial pattern. </li>
       <li> A major period seems to be 1024 observation nodes, while the
       minimal period seems to be 256 (forcing a "homogenous" behaviour per
       cell). </li>
       <li> Inside a cell the hard problems seem often accumulated a either
       the left or the right edge (where 1024 sometimes needs to be replaced
       by a smaller power of 2). </li>
      </ol>
     </li>
     <li> The final output was
     \verbatim
c sat_status=2 initial_maximal_clause_length=15 initial_number_of_variables=36512 initial_number_of_clauses=564400 initial_number_of_literal_occurrences=2403960 running_time(s)=855236.4 number_of_nodes=333456 number_of_single_nodes=3 number_of_quasi_single_nodes=0 number_of_2-reductions=5709657 number_of_pure_literals=6423281 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=38 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=365124254 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindmani_r2k2_8_97280.cnf_m2pp_1658
     \endverbatim
     </li>
     <li> Much closer inspections are needed in order to see whether here
     really something non-random is going on. </li>
    </ol>
   </li>
  </ul>


  \todo 153900 <= hindmani_2^9(2)
  <ul>
   <li> n=117964, c=1025114, c/n ~ 8.69 </li>
   <li> Using rsaps for satisfiability detection; cutoff = 4*10^4 seems
   sufficient. </li>
   <li> n=153899 found easily satisfiable  (10 runs with cutoff=4*10^4 yield
   100% success). </li>
   <li> n=153900 seems unsatisfiable: constant min=1 for cutoff=4*10^4,
   as for 10^5 and 10^6. </li>
   <li> minisat2 </li>
   <li> OKsolver_2002-m2pp: After 550 monitoring nodes at level 24 the
   prediction is 498*10^6 nodes and around 50 years. So actually
   this problem (a=9) appears to be at least as hard as for a=10
   (w.r.t the search-tree size; processing per node takes less time). </li>
  </ul>


  \todo 302400 <= hindmini_2^10(2)
  <ul>
   <li> n=232001, c=2151496, c/n ~ 9.27 </li>
   <li> Using rsaps for satisfiability detection; cutoff = 5*10^4 seems
   sufficient. </li>
   <li> As usual (here!), the osteps-value are very close together. They rise
   in the course of increasing n. At around n=300000, cutoff=4*10^4 starts
   becoming not always sufficient:
   \verbatim
> ubcsat-okl -i Hindmani_r2k2_10_300000.cnf -runs 10 -cutoff 40000 -alg rsaps
       sat  min     osteps     msteps       seed
      1 1     0      38677      38677  512636378
      2 1     0      38529      38529  364640058
      3 1     0      38391      38391  860276095
      4 1     0      39678      39678 3401998204
      5 0     1      39607      40000 4006022246
      6 1     0      39848      39848  858709410
      7 0     1      39722      40000 1271966077
      8 1     0      38953      38953 1617802422
      9 1     0      39952      39952 1789433686
     10 0    24      39965      40000 2311572258
Clauses = 2131996
Variables = 300000
TotalLiterals = 8527984
FlipsPerSecond = 341
BestStep_Mean = 39332.200000
Steps_Mean = 39402.800000
Steps_Max = 40000.000000
PercentSuccess = 70.00
BestSolution_Mean = 2.600000
BestSolution_Median = 0.000000
BestSolution_Min = 0.000000
BestSolution_Max = 24.000000
   \endverbatim
   </li>
   <li> n=302399 found easily satisfiable (10 runs with cutoff=5*10^4 yield
   100% success). </li>
   <li> n=302400 seems unsatisfiable:
    <ol>
     <li> 10 runs with cutoff=5*10^4 yield 9 times min=1, once min=2. </li>
     <li> 10 runs with cutoff=10^5 yield 9 times min=1, once min=2. </li>
     <li> 10 runs with cutoff=10^6 yield </li>
    </ol>
   </li>
   <li> minisat2 </li>
   <li> OKsolver_2002-m2pp: running it with options "-M -D24", from processing
   the first 182 observation-nodes at depth 24 one obtains a prediction of
   about 330*10^6 nodes, which makes about 50 years. </li>
  </ul>


 \todo 353925 <= hindmini_2^11(2)
  <ul>
   <li> n=268425, c=2502996, c/n ~ 9.32 </li>
   <li> Using rsaps for satisfiability detection; cutoff = 6*10^4 seems
   sufficient. </li>
   <li> n=353924 easily satisfiable: 10 runs with cutoff=6*10^4 yield 100%
   success. </li>
   <li> n=353925 seems unsatisfiable: 10 runs with cutoff=10^5 yield
   constant min=1. </li>
   <li> minisat2 </li>
  </ul>


  \todo hindmini_2^12(2)
  <ul>
   <li> We need to write a little C++ generator. </li>
   <li> And we should create now a simple program which performs the
   computations automatically. See "Simpler strategies" in
    ExperimentSystem/ControllingLocalSearch/plans/PointOfUnsatisfiability.hpp
   </li>
   <li> Let's start with cutoff = 8*10^4 and with n = 500000. </li>
  </ul>


  \todo Analysing the map a -> hindmani_2^a(2)
  <ul>
   <li> For 1 <= a <= 11 the known values are
   252, 990, 3150, 5600, 14364, 28188, 53312, 97280, 153900, 302400, 353925
   (where the last three values are conjectured). </li>
   <li> [log,log^2]-transformation looks linear for 1 <= a <= 10:
    <ol>
     <li> The linear model is
     log(log(hindmani_2^a(2))) = 1.690803 + 0.357812 * log(a), which
     yields hindmani_2^a(2) ~ 226.7474^(a^0.3578117). </li>
     <li> The predicted
     values are
     227, 1044, 3089, 7383, 15480, 29663, 53188, 90593, 148071, 233945. </li>
     <li> Fitting a non-linear model for h(a) ~ B^(a^e) (via nls) yields
     h(a) ~ 99.04179^(a^0.43811). </li>
     <li> Now the predicted values are
     99, 506, 1697, 4607, 10957, 23746, 48008, 91906, 168340, 297222. </li>
     <li> This predicts h(11) ~ 508680. </li>
     <li> However this is far off --- a better model is needed. </li>
     <li> Removing the outliers at a = 4, 10 (the value for a=4 seems too low,
     while the value for a=10 seems too high) yields a better model:
     \verbatim
A = 1:11
H = c(252,990,3150,5600,14364,28188,53312,97280,153900,302400,353925)
plot(log(A),log(log(H)))
Ex = c(-4,-10)
LH = lm(log(log(H[Ex])) ~ log(A[Ex]))

summary(LH)
Residuals:
       Min         1Q     Median         3Q        Max
-0.0123530 -0.0049474  0.0008959  0.0050046  0.0115686
Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept) 1.698516   0.006254   271.6 2.42e-15 ***
log(A[Ex])  0.353497   0.003667    96.4 3.41e-12 ***
Residual standard error: 0.00818 on 7 degrees of freedom
Multiple R-squared: 0.9992,     Adjusted R-squared: 0.9991
F-statistic:  9292 on 1 and 7 DF,  p-value: 3.407e-12

lines(log(A[Ex]), predict(LH))

H2 = H[Ex]
A2 = A[Ex]
NLH = nls(H2 ~ B^(A2^E),  start = c(B=99, E=0.44))

summary(NLH)
Parameters:
   Estimate Std. Error t value Pr(>|t|)
B 2.707e+02  1.650e+01   16.41 7.62e-07 ***
E 3.440e-01  4.631e-03   74.29 2.11e-11 ***
Residual standard error: 2625 on 7 degrees of freedom
Number of iterations to convergence: 6
Achieved convergence tolerance: 1.158e-06

H2
252    990   3150  14364  28188  53312  97280 153900 353925
round(predict(NLH))
271   1223   3545  17044  32024  56344  94228 151311 354887

plot(A,H)
lines(A2,predict(NLH))
     \endverbatim
     So the model here is h(a) ~ 270.6736628^(a^0.3440171). For a=12 this
     predicts h(12) ~ 523282. </li>
    </ol>
   </li>
   <li> The successive quotients are (rounded to two decimal places)
   3.93, 3.19, 1.78, 2.565, 1.96, 1.89, 1.82, 1.58, 1.96, 1.17. </li>
   <li> The values are of a somewhat limited (number-theoretical) diversity:
   \verbatim
H : [252, 990, 3150, 5600, 14364, 28188, 53312, 97280, 153900, 302400, 353925];
for i : 1 thru length(H) do print(i, H[i], ifactors(H[i]));
1 252     [[2,2],[3,2],[7,1]]
2 990     [[2,1],[3,2],[5,1],[11,1]]
3 3150    [[2,1],[3,2],[5,2],[7,1]]
4 5600    [[2,5],[5,2],[7,1]]
5 14364   [[2,2],[3,3],[7,1],[19,1]]
6 28188   [[2,2],[3,5],[29,1]]
7 53312   [[2,6],[7,2],[17,1]]
8 97280   [[2,10],[5,1],[19,1]]
9 153900  [[2,2],[3,4],[5,2],[19,1]]
10 302400 [[2,6],[3,3],[5,2],[7,1]]
11 353925 [[3,2],[5,2],[11,2],[13,1]]
   \endverbatim
   </li>
   <li> The sequence of densities:
    <ol>
     <li> We get (real densities):
     5.65, 5.46, 6.01, 6.22, 6.92, 7.40, 7.88, 8.35, 8.69, 9.27, 9.32. </li>
     <li> The densities w.r.t. the formal number of variables are
     5.65, 5.04, 5.20, 5.11, 5.56, 5.83, 6.14, 6.45, 6.66, 7.11, 7.07.
     This looks less suitable. </li>
     <li> The (real) densities are rather linear, with outliers for 
     a= 2, 4, 9, 11, where likely a=1 is to be excluded. </li>
     <li> So the "regular values" are a= 3, 5, 6, 7, 8, 10. </li>
     The linear model is
     \verbatim
D = c(5.65,5.46,6.01,6.22,6.92,7.40,7.88,8.35,8.69,9.27,9.32)
Res = c(3, 5, 6, 7, 8, 10)
L = lm(D[Res] ~ A[Res])

summary(L)
Residuals:
        1         2         3         4         5         6
 0.008362 -0.016893 -0.004520  0.007853  0.010226 -0.005028
Coefficients:
              Estimate Std. Error t value Pr(>|t|)
(Intercept)   4.598757   0.015058   305.4  6.9e-10 ***
A[Res]        0.467627   0.002192   213.3  2.9e-09 ***
Residual standard error: 0.01191 on 4 degrees of freedom
Multiple R-squared: 0.9999,     Adjusted R-squared: 0.9999
F-statistic: 4.549e+04 on 1 and 4 DF,  p-value: 2.899e-09

round(predict(L),2)
6.00 6.94 7.40 7.87 8.34 9.28
D[Res]
6.01 6.92 7.40 7.88 8.35 9.27
plot(A,D)
lines(A[Res],predict(L))
     \endverbatim
     </li>
    </ol>
   </li>
   <li> One needs to determine the n- and/or c-values (numbers of variables
   and numbers of clauses). </li>
   <li> The n-sequence is (these are the real occurring variables)
   252, 914, 2725, 4598, 11534, 22205, 41497, 75151, 117964, 232001, 268425.
    <ol>
     <li> Again, [log, log^2] transformations looks linear.
     \verbatim
N = c(252,914,2725,4598,11534,22205,41497,75151,117964,232001,268425)
LN = lm(log(log(N)) ~ log(A))
plot(log(A),log(log(N)))
lines(log(A),predict(LN))
     \endverbatim
     </li>
     <li> Now the outliers appear to be a= 1, 4, 10. </li>
    </ol>
   </li>
   <li> The c-sequence is
   1424, 4988, 16378, 28618, 79800, 164334, 327078, 627618, 1025114,
   2151496, 2502996.
    <ol>
     <li> Again, [log, log^2] transformations looks most appropriate, but
     rather bumpy. </li>
    </ol>
   </li>
  </ul>

*/

