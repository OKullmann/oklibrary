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
ullmann-0:OKplatform> minisat2 Hindmand_3_40000.cnf
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
   \endverbatim
   </li>
  </ul>

*/

