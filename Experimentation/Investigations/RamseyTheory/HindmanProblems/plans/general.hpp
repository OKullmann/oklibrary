// Oliver Kullmann, 11.7.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/HindmanProblems/plans/general.hpp
  \brief General plans regarding investigations on Hindman problems

  The hyperedges of "Hindman problems" are of the form
  {x_1, ..., x_k, s_1, ..., s_m, p_1, ..., p_m}, where k >= 1, m = 2^k-1-k,
  and where the s_i resp. p_i correspond to the non-empty sums resp. products
  of the x_i. The standard versions don't require the x_i to be distinct,
  while the injective version do require it. A further parameter s >= 1 allows
  ranges {a, ..., n} of vertices to be considered (default is s = 1).

  The (normal) numbers are called hindman_r(k_1, ..., k_r) resp.
  hindmani_r(k_1, ..., k_r). The generalised versions are
  hindman_r^s(k_1, ..., k_r) and hindmani_r^s(k_1, ..., k_r).


  \todo Connections
  <ul>
   <li> See
   Experimentation/Investigations/RamseyTheory/FolkmanProblems/plans/general.hpp
   for the weaker problems, where only the sums are considered. </li>
  </ul>


  \todo Generation
  <ul>
   <li> hindmanquads_degenerated_ohg allows x=y (as with Schur numbers). </li>
   <li> x <> y is given by hindmanquads_ohg. </li>
   <li> Both above cases concern k=2, while the general cases are treated by
   hindman_ohg. </li>
  </ul>

  \todo Only products
  <ul>
   <li> Also the problems similar to Folkman problems, but using the product
   instead of the sum, should be considered. </li>
   <li> How to call the corresponding hypergraphs and numbers? "allproducts"
   resp. "allproductsi"? </li>
  </ul>


  \todo hindman_2(2) = 39
  <ul>
   <li> OKsolver_2002:
   \verbatim
kullmann-0:OKplatform> OKsolver_2002-O3-DNDEBUG Hindmand_2_38.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=4 initial_number_of_variables=76 initial_number_of_clauses=226 initial_number_of_literal_occurrences=664 running_time(s)=0.0 number_of_nodes=1 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=0 number_of_pure_literals=0 number_of_autarkies=3 number_of_missed_single_nodes=0 max_tree_depth=0 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=0 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=80 file_name=Hindmand_2_38.cnf
kullmann-0:OKplatform> OKsolver_2002-O3-DNDEBUG Hindmand_2_39.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=4 initial_number_of_variables=78 initial_number_of_clauses=232 initial_number_of_literal_occurrences=682 running_time(s)=0.0 number_of_nodes=1 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=1 number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=0 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=0 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=82 file_name=Hindmand_2_39.cnf
   \endverbatim
   </li>
  </ul>


  \todo hindman_3(2) > 80000
  <ul>
   <li> Local search seems ineffective:
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 10 -cutoff 10000000 -i Hindmand_3_7000.cnf
       sat  min     osteps     msteps       seed
      1 0     1     128712   10000000 1974571835
      2 0     1     164697   10000000  141907909
      3 0     1     137570   10000000 2844493766
      4 0     1     156528   10000000 2907523445
      5 0     1     152484   10000000 1281678395
      6 0     1     121241   10000000 1148261914
      7 0     1     110357   10000000  361512568
      8 0     1     116423   10000000 3049483154
      9 0     1     153049   10000000 2648332806
     10 0     1     166842   10000000 1270017279
Clauses = 122728
Variables = 21000
TotalLiterals = 420663
FlipsPerSecond = 10437
BestStep_Mean = 140790.300000
Steps_Mean = 10000000.000000
Steps_Max = 10000000.000000
   \endverbatim
   </ul>
   <li> minisat2 seems to perform best (also on satisfiable instances):
kullmann-0:OKplatform> minisat2 Hindmand_3_40000.cnf
This is MiniSat 2.0 beta
WARNING: for repeatability, setting FPU to use double precision
============================[ Problem Statistics ]=============================
|                                                                             |
|  Number of variables:  120000                                               |
|  Number of clauses:    805390                                               |
|  Parsing time:         3.06         s                                       |
============================[ Search Statistics ]==============================
| Conflicts |          ORIGINAL         |          LEARNT          | Progress |
|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |
===============================================================================
|         0 |   62629   613294  3095858 |   204431        0    nan |  0.000 % |
|       100 |   62629   613294  3095858 |   224874      100     75 |  0.000 % |
|       250 |   62629   613294  3095858 |   247361      250     92 |  0.000 % |
|       475 |   62629   613294  3095858 |   272098      475    103 |  0.000 % |
===============================================================================
restarts              : 4
conflicts             : 545            (2 /sec)
decisions             : 334862         (1.70 % random) (1256 /sec)
propagations          : 502545         (1885 /sec)
conflict literals     : 49960          (2.41 % deleted)
Memory used           : 153.79 MB
CPU time              : 266.641 s
SATISFIABLE
   \endverbatim
   </li>
   <li> n=80000 still found easily satisfiable (4 restarts) by minisat2. </li>
   <li> For larger problems we need a C++ program. </li>
   <li> Neil Hindman remarked that for the infinite versions
   (partitioning the set of natural numbers, and asking for arbitrarily long
   sequences), which are implied by the finite versions, the degenerated
   form (which just asks for some sequence) and the non-degenerated form
   (asking for an injective sequence) are equivalent. </li>
  </ul>


  \todo hindmani_2(2) = 252
  <ul>
   <li> n=502, c=1908, c/n ~ 3.80 </li>
   <li> OKsolver_2002
    \verbatim
kullmann-0:OKplatform> OKsolver_2002-O3-DNDEBUG Hindman_2_251.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=4 initial_number_of_variables=502 initial_number_of_clauses=1908 initial_number_of_literal_occurrences=6130 running_time(s)=0.0 number_of_nodes=6 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=27 number_of_pure_literals=0 number_of_autarkies=64 number_of_missed_single_nodes=0 max_tree_depth=3 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=2328 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=502 file_name=Hindman_2_251.cnf
kullmann-0:OKplatform> OKsolver_2002-O3-DNDEBUG Hindman_2_252.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=4 initial_number_of_variables=504 initial_number_of_clauses=1928 initial_number_of_literal_occurrences=6204 running_time(s)=0.1 number_of_nodes=11 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=36 number_of_pure_literals=0 number_of_autarkies=98 number_of_missed_single_nodes=0 max_tree_depth=3 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=4624 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=504 file_name=Hindman_2_252.cnf
    \endverbatim
    Interesting that the problems are so easy. </li>
   </li>
  </ul>


  \todo hindmani_2^2(2) = 990
  </ul>
   <li> n=914, c=4988, c/n ~ 5.46 </li>
   <li> OKsolver_2002
    \verbatim
kullmann-0:OKplatform> OKsolver_2002-O3-DNDEBUG Hindman2gen_2_989.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=4 initial_number_of_variables=913 initial_number_of_clauses=4966 initial_number_of_literal_occurrences=19864 running_time(s)=0.1 number_of_nodes=130 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=581 number_of_pure_literals=1023 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=15 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=17213 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindman2gen_2_989.cnf
kullmann-0:OKplatform> OKsolver_2002-O3-DNDEBUG Hindman2gen_2_990.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=4 initial_number_of_variables=914 initial_number_of_clauses=4988 initial_number_of_literal_occurrences=19952 running_time(s)=1.6 number_of_nodes=2291 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=10006 number_of_pure_literals=14514 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=17 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=282822 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindman2gen_2_990.cnf
    \endverbatim
   </li>
  </ul>


  \todo hindmani_2^3(2) = 3150
  <ul>
   <li> n=2725, c=16378, c/n ~ 6.01 </li>
   <li> minisat2
> minisat2 Hindman2gen_3_3149.cnf
restarts              : 13
conflicts             : 38361          (2808 /sec)
decisions             : 42749          (2.03 % random) (3129 /sec)
propagations          : 2406216        (176126 /sec)
conflict literals     : 502210         (35.14 % deleted)
Memory used           : 4.34 MB
CPU time              : 13.6619 s
SATISFIABLE
> minisat2 Hindman2gen_3_3150.cnf
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
> minisat2 Hindman2gen_4_5599.cnf
restarts              : 11
conflicts             : 14960          (1504 /sec)
decisions             : 18767          (1.88 % random) (1887 /sec)
propagations          : 1242672        (124961 /sec)
conflict literals     : 248023         (30.22 % deleted)
SATISFIABLE
> minisat2 Hindman2gen_4_5600.cnf
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
c sat_status=0 initial_maximal_clause_length=19 initial_number_of_variables=2312 initial_number_of_clauses=24676 initial_number_of_literal_occurrences=106840 running_time(s)=1757.9 number_of_nodes=85979 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=643340 number_of_pure_literals=181686 number_of_autarkies=0 number_of_missed_single_nodes=4 max_tree_depth=28 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=8501164 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindman2gen_4_5600.cnf_m2pp_12733
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=16 initial_number_of_variables=2493 initial_number_of_clauses=26858 initial_number_of_literal_occurrences=116064 running_time(s)=-1444.1 number_of_nodes=84627 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=621512 number_of_pure_literals=178792 number_of_autarkies=0 number_of_missed_single_nodes=4 max_tree_depth=27 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=8864762 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindman2gen_4_6000.cnf_m2pp_11324
   \endverbatim
   </li>
   <li> The minisat2-preprocessing seems to help OKsolver_2002 somewhat:
   \verbatim
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=4 initial_number_of_variables=4598 initial_number_of_clauses=28618 initial_number_of_literal_occurrences=114472 running_time(s)=-1470.6 number_of_nodes=110501 number_of_single_nodes=2 number_of_quasi_single_nodes=0 number_of_2-reductions=809722 number_of_pure_literals=2464488 number_of_autarkies=0 number_of_missed_single_nodes=6 max_tree_depth=28 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=78884356 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindman2gen_4_5600.cnf
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
> minisat2 Hindman2gen_5_14363.cnf
restarts              : 18
conflicts             : 266053         (1499 /sec)
decisions             : 298233         (1.99 % random) (1680 /sec)
propagations          : 25902069       (145954 /sec)
conflict literals     : 5067946        (28.93 % deleted)
SATISFIABLE
> minisat2 Hindman2gen_5_14364.cnf
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
> OKsolver_2002-m2pp -D10 -M Hindman2gen_5_14364.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=12 initial_number_of_variables=5539 initial_number_of_clauses=70134 initial_number_of_literal_occurrences=304920 running_time(s)=-1526.9 number_of_nodes=516451 number_of_single_nodes=4 number_of_quasi_single_nodes=0 number_of_2-reductions=4497536 number_of_pure_literals=1499630 number_of_autarkies=0 number_of_missed_single_nodes=4 max_tree_depth=30 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=82701806 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=Hindman2gen_5_14364.cnf_m2pp_5223
   \endverbatim
   It seems that the advantage of minisat2 is just that expensive (and
   apparently ineffective) look-aheads are avoided --- but nothing in
   the direction of "resolution is more powerful than tree-resolution". </li>
   <li> satz2-m2pp
   \verbatim
**** The instance is unsatisfiable. *****
NB_MONO= 21479226, NB_UNIT= 241108531, NB_BRANCHE= 2944242, NB_BACK= 1488069
Program terminated in 5.762 seconds.
satz215 Hindman2gen_5_14364.cnf_m2pp_8170 5.762 2944242 1488069 1367706445 31952092 0 14364 70134 0 199693920 19963489
   \endverbatim
   Roughly comparable to OKsolver_2002-m2pp.
   </li>
   <li> march_pl
   \verbatim
> march_pl Hindman2gen_5_14364.cnf
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
     <li> march_pl
     \verbatim
> march_pl Hindman2gen_6_28188.cnf
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
     <li> OKsolver_2002-m2pp seems to take, say, 10 days. </li>
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
   <li> march_pl </li>
   <li> OKsolver_2002-m2pp: it seems it would take a few months. </li>
  </ul>


  \todo 97280 <= hindmani_2^8(2)
  <ul>
   <li> n=75151, c=627618, c/n ~ 8.35 </li>
   <li> Using rsaps for satisfiability detection. </li>
   <li> n=97279 found easily satisfiable (10 runs with cutoff=2*10^4 yield
   100% success). </li>
   <li> n=97280 appears to be unsatisfiable: constant min=1 for cutoff=2*10^4
   as for 10^5 and 10^6). </li>
   <li> minisat2 </li>
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
   <li> OKsolver_2002-m2pp: After 146 monitoring nodes at level 24 the
   prediction is 413*10^6 nodes and around 50 years. So actually
   this problem (a=9) appears to be at least as hard as for a=10
   (w.r.t the search-tree size; processing per node takes less time). </li>
  </ul>


  \todo 302400 <= hindmini_2^10(2)
  <ul>
   <li> n=232001, c=2151496, c/n ~ 9.27 </li>
   <li> Using rsaps for satisfiability detection; cutoff = 5*10^4 seems
   sufficient. </li>
   <li> As usual (here!), the osteps-value are very close together. They rise
   in the course of increasing n. At around n=300000  cutoff=4*10^4 starts
   becoming not always sufficient:
   \verbatim
> ubcsat-okl -i Hindman2gen_10_300000.cnf -runs 10 -cutoff 40000 -alg rsaps
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
   <li> OKsolver_2002-m2pp: running it with options "-M -D24", from processing
   the first 182 observation-nodes at depth 24 one obtains a prediction of
   about 330*10^6 nodes, which makes about 50 years. </li>
  </ul>


 \todo 353916 < hindmini_2^11(2)
  <ul>
   <li> n=?, c=?, c/n ~ ? </li>
   <li> Reminder: instances are created by
   output_hindman2gen_stdname(11,n). </li>
   <li> Using rsaps for satisfiability detection; cutoff = 6*10^4 seems
   sufficient. </li>
   <li> n=353916 easily satisfiable. </li>
   <li> n=353925 seems unsatisfiable. </li>
  </ul>


  \todo Analysing the map a -> hindmani_2^a(2)
  <ul>
   <li> For 1 <= a <= 10 the known values are
   252, 990, 3150, 5600, 14364, 28188, 53312, 97280, 153900, 302400
   (where the last three values are conjectured). </li>
   <li> [log,log^2]-transformation looks linear: The linear model is
   log(log(hindmani_2^a(2))) = 1.690803 + 0.357812 * log(a), which
   yields hindmani_2^a(2) ~ 226.7474^(a^0.3578117). The predicted
   values are
   227, 1044, 3089, 7383, 15480, 29663, 53188, 90593, 148071, 233945. </li>
   <li> Fitting a non-linear model for h(a) ~ B^(a^e) (via nls) yields
   h(a) ~ 99.04179^(a^0.43811), where the predicted values are
   99, 506, 1697, 4607, 10957, 23746, 48008, 91906, 168340, 297222. </li>
   <li> This predicts h(11) ~ 508680. </li>
   <li> The successive quotients are (rounded to two decimal places)
   3.93, 3.19, 1.78, 2.565, 1.96, 1.89, 1.82, 1.58, 1.96. </li>
   <li> The peaks are for a= 2, 5, 10. </li>
   <li> The values are of a somewhat limited diversity:
   \verbatim
for i : 1 thru length(L) do print(i, L[i], ifactors(L[i]));
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
   \endverbatim
   </li>
   <li> The sequence of densities is (excluding a=1; real densities)
   5.46, 6.01, 6.22, 6.92, 7.40, 7.88, 8.35, 8.69, 9.27. </li>
   <li> This is rather linear, with outliers for a=4, 9. The linear
   model is rho(i) = 4.50556 + 0.47500 * i (excluding n=1), which yields
   5.46 5.93 6.41 6.88 7.36 7.83 8.31 8.78 9.26. </li>
   <li> The densities w.r.t. the formal number of variables are (excluding a=1)
   5.04, 5.20, 5.11, 5.56, 5.83, 6.14, 6.45, 6.66, 7.11.
   This looks less suitable. </li>
   <li> One needs to determine the n- and/or c-values (numbers of variables
   and numbers of clauses). </li>
   <li> The n-sequence is (excluding a=1; these are the real occurring
   variables)
   914, 2725, 4598, 11534, 22205, 41497, 75151, 117964, 232001. </li>
   <li> Again, [log, log^2] transformations looks linear. </li>
   <li> The c-sequence is (excluding a=1)
   4988, 16378, 28618, 79800, 164334, 327078, 627618, 1025114, 2151496. </li>
  </ul>

*/

