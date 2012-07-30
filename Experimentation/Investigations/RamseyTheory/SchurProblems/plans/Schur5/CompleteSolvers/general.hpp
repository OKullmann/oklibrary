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
   <li> See "Palindromic problems, direct encoding" in
   RamseyTheory/SchurProblems/plans/Schur5/LocalSearch/general.hpp for
   finding solutions via local search. </li>
   <li>
   \verbatim
> CRunPdSchur 5 161 "minisat-2.2.0 -cpu-lim=10000"
   \endverbatim
   yields that all n <= 154 and n=156 are SAT, while n=155 and n>=157 are
   unknown. </li>
   <li>
   \verbatim
pdschur(5);
  [{[155],[158]},[161,306]]
   \endverbatim
   means that n=155,158 are unknown, everything else with n<161 is SAT, while
   n>= 161 is unknown. </li>
   <li> [Fredricksen, Sweet, 2000] conjecture that pdschur(5) = [{155,158},161]
   holds. </li>
   <li> With full symmetry-breaking:
   \verbatim
> CRunPdSchurFsb 5 161 "minisat-2.2.0 -cpu-lim=10000"
# aborted after it couldn't solve now n=153
   \endverbatim
   </li>
   <li> At least for the satisfiable instances, with full symmetry breaking it
   takes substantially longer! </li>
   <li> Solver comparisons:
   \verbatim
output_pd_schur_fullsb_stdname(5,146);

> cat Schur_pd_fullsb_5_146.cnf | ExtendedDimacsFullStatisticNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    370    9849    370    370    9849    28550     NA     NA    9849    28550   374
 length   count
      1      20
      2    1105
      3    8650
      5      74
> cat Schur_pd_fullsb_5_146.cnf | UnitClausePropagation-O3-DNDEBUG  > Schur_pd_fullsbUCP_5_146.cnf
> cat Schur_pd_fullsbUCP_5_146.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    370    7843    337    370    7843    22373     NA     NA    7843    22373   379
 length   count
      2    1286
      3    6488
      4       8
      5      61

> satz215 Schur_pd_5_146.cnf
NB_MONO= 0, NB_UNIT= 243993768, NB_BRANCHE= 6366100, NB_BACK= 3258290
Program terminated in 1235.230 seconds.
satz215 Schur_pd_5_146.cnf 1235.230 6366100 3258290 470702379 17146147 1 370 9829 0 6427545 2391110
> satz215 Schur_pd_fullsb_5_146.cnf
**** The instance is satisfiable. *****
NB_MONO= 0, NB_UNIT= 900822276, NB_BRANCHE= 24092834, NB_BACK= 12290366
Program terminated in 4229.920 seconds.
satz215 Schur_pd_fullsb_5_146.cnf 4229.920 24092834 12290366 1627370372 66046550 1 370 9849 -2006 27195660 9524121

> minisat-2.2.0 Schur_pd_5_146.cnf
restarts              : 1596
conflicts             : 759076         (18974 /sec)
decisions             : 996895         (0.00 % random) (24919 /sec)
propagations          : 30077293       (751821 /sec)
conflict literals     : 29233772       (21.53 % deleted)
Memory used           : 31.00 MB
CPU time              : 40.0059 s
> minisat-2.2.0 Schur_pd_fullsb_5_146.cnf
restarts              : 131069
conflicts             : 101638255      (13983 /sec)
decisions             : 125902579      (0.00 % random) (17322 /sec)
propagations          : 3818264189     (525316 /sec)
conflict literals     : 2651552539     (31.67 % deleted)
Memory used           : 95.00 MB
CPU time              : 7268.51 s

> picosat913 Schur_pd_5_146.cnf
c 0 iterations
c 13818 restarts
c 0 failed literals
c 8527198 conflicts
c 10999787 decisions
c 0 fixed variables
c 263254187 learned literals
c 31.9% deleted literals
c 506432115 propagations
c 100.0% variables used
c 468.7 seconds in library
c 1.1 megaprops/second
c 1 simplifications
c 333 reductions
c 1163.4 MB recycled
c 8.0 MB maximally allocated
c 468.7 seconds total run time

> picosat913 Schur_pd_fullsb_5_146.cnf
c 0 iterations
c 72366 restarts
c 0 failed literals
c 56355622 conflicts
c 70326819 decisions
c 33 fixed variables
c 1483459769 learned literals
c 37.2% deleted literals
c 3342268712 propagations
c 100.0% variables used
c 4594.5 seconds in library
c 0.7 megaprops/second
c 1 simplifications
c 1557 reductions
c 2631.7 MB recycled
c 9.6 MB maximally allocated
c 4594.5 seconds total run time
   \endverbatim
   As with vdW-problems, the general impression is that minisat-2.2.0 is best
   on palindromic problems, while symmetry-breaking doesn't help (and
   actually impairs performance, which needs an explanation!). </li>
   <li> Symmetry-breaking seems not counter-productive when used within the
   Cube&Conquer-approach; see Schur5/SplittingViaOKsolver.hpp. </li>
  </ul>


  \todo Forbidden elements for palindromic problems
  <ul>
   <li> Compare "Direct encoding with forbidden elements" in
   Schur5/LocalSearch/general.hpp. </li>
   <li> In [Fredricksen, Sweet, 2000] there is the conjecture that for every
   palindromic solution for n=160 the maximum of the minimal values of the
   blocks over all "equivalent" solutions (obtained by an automorphism of the
   hypergraph via multiplication) is equal to 44. </li>
   <li> From that conjecture follows that pd_schur_rm_nbfclud(5,160,44) should
   be unsatisfiable. </li>
   <li> Using also full symmetry-breaking, i.e., using
   pd_schur_fullsb_rm_nbfcsud(r,n,k), this is very simple:
   \verbatim
output_pd_schur_fullsb_rm_stdname(5,160,44);

> OKsolver_2002-O3-DNDEBUG Schur_pd_fullsb_rm_5_160-44.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         5
c initial_number_of_variables           400
c initial_number_of_clauses             11354
c initial_number_of_literal_occurrences 32894
c number_of_initial_unit-eliminations   73
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           73
c reddiff_number_of_clauses             3925
c reddiff_number_of_literal_occurrences 11641
c number_of_2-clauses_after_reduction   1136
c running_time(sec)                     4.1
c number_of_nodes                       4375
c number_of_single_nodes                12
c number_of_quasi_single_nodes          0
c number_of_2-reductions                23858
c number_of_pure_literals               0
c number_of_autarkies                   16
c number_of_missed_single_nodes         101
c max_tree_depth                        31
c proportion_searched                   9.999959e-01
c proportion_single                     4.100613e-06
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 526915
   \endverbatim
   </li>
   <li> Is this still feasible without full symmetry-breaking?:
   \verbatim
> OKsolver_2002-O3-DNDEBUG Schur_pd_rm_5_160-44.cnf
XXX
   \endverbatim
   </li>
   <li> While pd_schur_rm_nbfclud(5,160,43) must be satisfiable:
   \verbatim
output_pd_schur_fullsb_rm_stdname(5,160,43);

> cat Schur_pd_fullsb_rm_5_160-43.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    400   11353    400    400   11353    32893     NA     NA   11353    32893   405
 length   count
      1      63
      2    1200
      3   10010
      5      80

> OKsolver_2002-O3-DNDEBUG Schur_pd_fullsb_rm_5_160-43.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         5
c initial_number_of_variables           400
c initial_number_of_clauses             11353
c initial_number_of_literal_occurrences 32893
c number_of_initial_unit-eliminations   72
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           72
c reddiff_number_of_clauses             3907
c reddiff_number_of_literal_occurrences 11594
c number_of_2-clauses_after_reduction   1142
c running_time(sec)                     4.7
c number_of_nodes                       4479
c number_of_single_nodes                14
c number_of_quasi_single_nodes          0
c number_of_2-reductions                24661
c number_of_pure_literals               0
c number_of_autarkies                   21
c number_of_missed_single_nodes         178
c max_tree_depth                        31
c proportion_searched                   9.999948e-01
c proportion_single                     5.226582e-06
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 563239
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_pd_fullsb_rm_5_160-43.cnf
   \endverbatim
   We see that "full symmetry-breaking" is not satisfiability-preserving! </li>
   <li> Checking without "full symmetry-breaking":
   \verbatim
output_pd_schur_rm_stdname(5,160,43);

> cat Schur_pd_rm_5_160-43.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    400   11333    400    400   11333    32873     NA     NA   11333    32873   404
 length   count
      1      43
      2    1200
      3   10010
      5      80

> OKsolver_2002-O3-DNDEBUG Schur_pd_rm_5_160-43.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         5
c initial_number_of_variables           400
c initial_number_of_clauses             11333
c initial_number_of_literal_occurrences 32873
c number_of_initial_unit-eliminations   43
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           43
c reddiff_number_of_clauses             2212
c reddiff_number_of_literal_occurrences 6356
c number_of_2-clauses_after_reduction   963
c running_time(sec)                     7.9
c number_of_nodes                       9725
c number_of_single_nodes                26
c number_of_quasi_single_nodes          0
c number_of_2-reductions                79088
c number_of_pure_literals               0
c number_of_autarkies                   150
c number_of_missed_single_nodes         247
c max_tree_depth                        38
c proportion_searched                   2.045396e-02
c proportion_single                     4.503236e-07
c total_proportion                      0.02045440673828125
c number_of_table_enlargements          0
c number_of_1-autarkies                 1062904
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_pd_rm_5_160-43.cnf
   \endverbatim
   </li>
  </ul>


  \todo pdwschur(5)
  <ul>
   <li>
   \verbatim
> CRunPdWSchur 5 190 "minisat-2.2.0 -cpu-lim=10000"
   \endverbatim
   yields that all n <= 152 and n=154,155,156 are SAT, while n=153 and n>=157
   are unknown. </li>
   <li>
   \verbatim
pdwschur(5);
  [{[158]},[161,1631]]
   \endverbatim
   means that n=158 is unknown, everything else with n<161 is SAT, while
   n>= 161 is unknown. </li>
   <li> With full symmetry-breaking:
   \verbatim
> CRunPdWSchurFsb 5 190 "minisat-2.2.0 -cpu-lim=10000"
# aborted after n=152 couldn't be determined
   \endverbatim
   Also here we have that full symmetry-breaking makes the problems harder
   for satisfiable cases (while we do not know about the unsatisfiable
   cases). </li>
   <li> n=153 via SplittingViaOKsolver: see Schur5/SplittingViaOKsolver.hpp.
   </li>
  </ul>

*/
