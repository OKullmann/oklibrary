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
   </li>
   <li> minisat2 seems to perform best (also on satisfiable instances):
   \verbatim
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
   <li> Note that these instances use the translation from non-boolean to
   boolean variables. </li>
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

*/

