// Oliver Kullmann, 17.4.2009 (Swansea)
/* Copyright 2009, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/SchurProblems/plans/general.hpp
  \brief On computing schur(5)


  \todo Statistics
  <ul>
   <li> Creation:
   \verbatim
output_schur_stdname(5,160);
   \endverbatim
   </li>
   <li> Counts:
   \verbatim
> cat Schur_5_160.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    800   33495    800    800   33495    98805     NA     NA   33495    98805   803
 length   count
      2    2000
      3   31335
      5     160
   \endverbatim
   </li>
  </ul>


  \todo OKsolver_2002
  <ul>
   <li> With a cluster and a few months it looks feasible without any
   special action (the following data is for the old version, using the full
   hypergraph, not, as now, the subsumption-free form; this shouldn't make a
   big difference):
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
  </ul>


  \todo Local search
  <ul>
   <li> Considering Schur_5_159.cnf; the following data is for the old
   version, using the full hypergraph, not, as now, the subsumption-free form;
   this shouldn't make a big difference. </li>
   <li> From the ubcsat-1.0.0 suite saps seems clearly perform best, and
   a local minimum of one falsified clause is easily reached already with
   cutoff = 10000. </li>
   <li> But finding a solution is difficult: 10 runs with cutoff=10*10^6
   all just yield a minimum=1. </li>
   <li> The worst-performing algorithm (w.r.t. mean minimum) seemed to be
   hsat, which now with cutoff=10*10^6 still doesn't get close to 1. </li>
   <li> Using symmetry-breaking and then the minisat2-preprocessor seems
   to make the problem more difficult. </li>
   <li> Finally by
   \verbatim
ubcsat-okl -alg saps -runs 10000 -cutoff 100000 -i Schur_5_159.cnf
Clauses = 33349
Variables = 795
TotalLiterals = 98380
FlipsPerSecond = 116174
BestStep_Mean = 16171.864100
Steps_Mean = 99993.137900
Steps_Max = 100000.000000
PercentSuccess = 0.01
BestSolution_Mean = 1.445400
BestSolution_Median = 1.000000
BestSolution_Min = 0.000000
BestSolution_Max = 3.000000
   \endverbatim
   we found one solution (seed=830151296, msteps=31379). </li>
   <li> On the other hand we get
   \verbatim
ubcsat-okl -alg saps -runs 40000 -cutoff 200000 -i Schur_5_160.cnf | tee Schur_5_160.out
Clauses = 33760
Variables = 800
TotalLiterals = 99600
FlipsPerSecond = 117567
BestStep_Mean = 23914.642400
Steps_Mean = 200000.000000
Steps_Max = 200000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.351275
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 3.000000
   \endverbatim
   so the conjecture shur(5)=160 seems justified. </li>
  </ul>

*/

