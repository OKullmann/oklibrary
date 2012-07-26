// Oliver Kullmann, 16.7.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/SchurProblems/plans/Schur5/CompleteSolvers/general.hpp
  \brief On computing lower bounds for schur(5) and variations via complete DPLL-solvers


  \todo Overview
  <ul>
   <li> See
   RamseyTheory/SchurProblems/plans/Schur5/SplittingViaOKsolver/general.hpp
   for the Cube-and-Conquer method. </li>
  </ul>


  \todo Estimation of run times for direct encoding
  <ul>
   <li> For n=160 and OKsolver_2002:
    <ol>
     <li> n=160 and n=161 shouldn't be very different (when run to completion).
     </li>
     <li> Also OKsolver_2002 should be only by a factor of two slower than
     satz and march_pl. </li>
     <li> Looking at "SplittingViaOKsolver for direct encoding" (in
     Schur5/SplittingViaOKsolver/general.hpp), if we optimistically
     combine the two phases considered into, say, 200000 * 1000 = 200*10^6
     nodes, and assume that each node can be solved in 2 days (it might
     be much worse), then we needed 400*10^6 days. If we had 10^6 nodes
     at our disposal, then this would take 400 days. </li>
     <li> So if one could mobilise large Internet resources, then the job
     might be feasible, but not with a (large) cluster. </li>
    </ol>
   </li>
   <li> For n=162 and palindromic problem:
    <ol>
     <li> Looking at "SplittingViaOKsolver for palindromic problem (direct
     encoding)" in Schur5/CompleteSolvers/SplittingViaOKsolver.hpp), with the
     splitting depth of 70 and 132720 splitting cases, one can hope that
     on average a sub-instance needs 1 day, and so 130*10^3 days would be
     needed. With 10^3 computer-nodes at our disposal, this should be
     feasible (given that we further improve the process). </li>
    </ol>
   </li>
  </ul>


  \todo OKsolver_2002 for direct encoding
  <ul>
   <li> the following data is for the old version, using the full hypergraph,
   not, as now, the subsumption-free form; this shouldn't make a big
   difference:
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D20 Schur_5_160.cnf
Name of file, monitoring depth and number of monitoring nodes:
 Schur_5_160.cnf,   20,    1048576
level: nodes processed, average nodes, predicted total nodes, time for monitoring node, average time, predicted remaining time, single nodes, autarkies, depth

...
14:   7705  14770.00  1.55E+10   250.47s   429.83s    14y 106d 12h 50m 47s     0     0   58
15:  18446  15015.07  1.57E+10   570.63s   439.22s    14y 220d 10h 39m 21s     0     0   58
...
39:   3382  16130.87  1.69E+10    93.10s   478.83s    15y 336d  1h 12m 16s     0     0   58
40:   5527  15865.77  1.66E+10   144.01s   470.46s    15y 234d 11h  3m 12s     0     0   58
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           800
c initial_number_of_clauses             33760
c initial_number_of_literal_occurrences 99600
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   2000
c running_time(sec)                     19635.3
c number_of_nodes                       667953
c number_of_single_nodes                18
c number_of_quasi_single_nodes          0
c number_of_2-reductions                10676278
c number_of_pure_literals               0
c number_of_autarkies                   1
c number_of_missed_single_nodes         359
c max_tree_depth                        58
c proportion_searched                   3.908341e-05
c proportion_single                     6.698131e-11
c total_proportion                      3.908347571268678e-05
c number_of_table_enlargements          0
c number_of_1-autarkies                 187851110
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_5_160.cnf

19635.3 / 3.908347571268678e-05 ~ 15.9 years
   \endverbatim
   </li>
   <li> And with initial symmetry breaking (using
   output_schur_sb_stdname(5,160)) a factor of nearly 5 should be achievable:
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D20 Schur_sb_5_160.cnf
Name of file, monitoring depth and number of monitoring nodes:
 Schur_sb_5_160.cnf,   20,    1048576
level: nodes processed, average nodes, predicted total nodes, time for monitoring node, average time, predicted remaining time, single nodes, autarkies, depth

...
   548:  22149  41402.39  4.34E+10   648.51s  1099.41s    36y 195d 19h  1m 23s     0     0   85
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           800
c initial_number_of_clauses             33764
c initial_number_of_literal_occurrences 99604
c number_of_initial_unit-eliminations   6
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           6
c reddiff_number_of_clauses             814
c reddiff_number_of_literal_occurrences 2574
c number_of_2-clauses_after_reduction   2137
c running_time(sec)                     602715.8
c number_of_nodes                       22697398
c number_of_single_nodes                1279
c number_of_quasi_single_nodes          0
c number_of_2-reductions                367966941
c number_of_pure_literals               0
c number_of_autarkies                   320
c number_of_missed_single_nodes         23646
c max_tree_depth                        85
c proportion_searched                   5.231135e-04
c proportion_single                     4.278459e-10
c total_proportion                      0.0005231138784438372
c number_of_table_enlargements          0
c number_of_1-autarkies                 6357718175
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_sb_5_160.cnf

602715.8 / 0.0005231138784438372 ~ 36.5 years
   \endverbatim
   </li>
   <li> These estimations are likely too optimistic, as the analysis of
   SplittingViaOKsolver above shows. </li>
  </ul>


  \todo pdschur(5)
  <ul>
   <li>
   \verbatim
> CRunPdSchur 5 161 "minisat-2.2.0 -cpu-lim=10000"
   \endverbatim
   yields that all n <= 154 and n=156 are SAT, while n=155 and n>=157 are
   unknown. </li>
   <li>
   \verbatim
pdschur(5);
  [{[155]},[157,306]]
   \endverbatim
   means that n=155 is unknown, n<155 and n=156 is SAT, while n>= 157 is
   unknown.
   </li>
  </ul>


*/
