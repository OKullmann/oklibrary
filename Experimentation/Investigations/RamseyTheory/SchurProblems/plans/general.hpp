// Oliver Kullmann, 17.4.2009 (Swansea)
/* Copyright 2009, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/SchurProblems/plans/general.hpp
  \brief General plans regarding investigations on Schur problems (and generalisations)

  The hyperedges of "generalised Schur problems" are of the form
  {x_1, ..., x_{k-1}, x_k}, where x_k is the sum of x_1, ..., x_{k-1}.
  The (original) Schur problems have k=3. For the (standard) versions
  the x_1, ..., x_{k-1} don't need to be distinct, while this is required
  for the "weak" versions.

  The numbers are called genschur_m(k_1, ..., k_m) resp.
  wgenschur_m(k_1, ..., k_m). For the standard Schur-numbers we have
  schur(r) = genschur_r(3,...,3) and
  wschur(r) = wgenschur_r(3,...,3).

  The standard Schur-numbers are available at Maxima-level via schur(r):
  create_list(schur(r),r,1,8);
    [2,5,14,45,[161,306],[537,1837],[1681,12860],[3916,109601]].
  The weak Schur-numbers are available at Maxima-level via wschur(r):
  create_list(wschur(r),r,1,8);
    [3,9,24,67,[190,1631],[573,11743],[1681,95901],[3916,876809]].

  SAT-translations are
  output_schur_stdname(r,n) for the SAT-problem "schur(r) > n ?", using
  the (strong) direct translation for the Schur-hypergraph, and
  output_schur_sb_stdname(r,n), additionally using symmetry-breaking
  (putting vertex 1 into the first partition (colour-class).

  The underlying hypergraph is schurtriples_hg(n), e.g.
  schurtriples_hg(5) =
    [{1,2,3,4,5},{{1,2},{1,2,3},{1,3,4},{1,4,5},{2,3,5},{2,4}}],
  while
  schurtriples_ohg(5) =
    [[1,2,3,4,5],[{1,2},{1,2,3},{1,3,4},{2,4},{1,4,5},{2,3,5}]].
  Subsumption-eliminiation removes the hyperedge {1,2,3} here:
  schurtriples_me_hg(5) =
    [{1,2,3,4,5},{{1,2},{1,3,4},{1,4,5},{2,3,5},{2,4}}],
  schurtriples_me_ohg(5) =
    [[1,2,3,4,5],[{1,2},{1,3,4},{2,4},{1,4,5},{2,3,5}]].
  The SAT-translation uses the subsumption-eliminated hypergraph.

  Good basic information is available in Chapter 8 of [Ramsey Theory on the
  Integers; Landman, Robertson].

  The palindromic problems are created by pd_output_schur_stdname(r,n).


  \todo Overview
  <ul>
   <li> See RamseyTheory/SchurProblems/plans/Schur4/general.hpp for
   investigations on schur(4). </li>
   <li> See RamseyTheory/SchurProblems/plans/Schur5/general.hpp for
   investigations on schur(5). </li>
   <li> See RamseyTheory/SchurProblems/plans/Schur6/general.hpp for
   investigations on schur(6). </li>
   <li> See "What is known" below for a quick overview. </li>
   <li> See below for a naive quick overview on the performance of complete
   SAT solvers. </li>
  </ul>


  \todo What is known
  <ul>
   <li> For a natural number r >= 1 the Schur-number schur(r) is the smallest
   n such that for every partition {1,...,n} into r parts at least one part
   contains a triple (x,y,z) in {1,...,n}^3 with x+y=z. </li>
   <li> So schur(r) = genschur_r(3, ..., 3). </li>
   <li> And wschur(r) = wgenschur_r(3, ..., 3). </li>
   <li> Only schur(1)=2, schur(2)=5, schur(3)=14 and schur(4)=45
   are known; this is A030126 in the OEIS. </li>
   <li> And only wschur(1)=3, wschur(2)=9, wschur(3)=24 and wschur(4)=67
   are known; this is A118771 in the OEIS. </li>
   <li> The determination of (w)schur(r) for 1 <= r <= 3 is trivial for
   any SAT solver. </li>
   <li> See http://mathworld.wolfram.com/SchurNumber.html . </li>
   <li> While schur(5)=160 is conjectured as research problem 8.1 in
   [Landman, Robertson, 2003]), where 160 <= schur(5) <= 316 is known.
   However the conjecture must be schur(5) = 161, since since 1994 a
   certificate showing schur(5)>160 is known. </li>
   <li> Furthermore schur(6) >= 537 and schur(7) >= 1681. </li>
   <li> At http://www.di.univaq.it/~formisano/CLPASP/node3.html one finds
   some results (mchaff, simo, relsat, zchaff) obtained by answer-set
   translations (likely irrelevant here). </li>
   <li> There is also a little talk
   http://www.rumathphysics.org/brandt/schur.pdf ; we could contact the
   author (once we can enumerate all solutions). </li>
   <li> Also the strict versions, where x,y are required to be different,
   are true. Let's call the numbers wschur(r) ("w" for "weak"). </li>
   <li> The generalised Schur-numbers genschur_m(k_1, ..., k_m) for m >= 0
   and k_i >= 2 asks for the existence of 1 <= i <= m such that part i
   contains a tuple (x_1, ..., x_{k_i}) with x_{k_i} = x_1 + ... + x_{k_i-1}.
   </li>
  </ul>


  \todo OKsolver_2002
  <ul>
   <li> Performance looks reasonable:
   \verbatim
schur(4) = 45:
> OKsolver_2002-O3-DNDEBUG Schur_4_44.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         4
c initial_number_of_variables           176
c initial_number_of_clauses             2188
c initial_number_of_literal_occurrences 6256
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   352
c running_time(sec)                     0.0
c number_of_nodes                       245
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1424
c number_of_pure_literals               0
c number_of_autarkies                   2
c number_of_missed_single_nodes         0
c max_tree_depth                        18
c proportion_searched                   1.550293e-02
c proportion_single                     0.000000e+00
c total_proportion                      0.0155029296875
c number_of_table_enlargements          0
c number_of_1-autarkies                 14851
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_4_44.cnf
> OKsolver_2002-O3-DNDEBUG Schur_4_45.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         4
c initial_number_of_variables           180
c initial_number_of_clauses             2279
c initial_number_of_literal_occurrences 6524
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   358
c running_time(sec)                     65.6
c number_of_nodes                       682348
c number_of_single_nodes                5735
c number_of_quasi_single_nodes          0
c number_of_2-reductions                3073787
c number_of_pure_literals               0
c number_of_autarkies                   22464
c number_of_missed_single_nodes         13870
c max_tree_depth                        36
c proportion_searched                   9.997743e-01
c proportion_single                     2.257211e-04
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 33816234
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_4_45.cnf

> OKsolver_2002-O3-DNDEBUG Schur_sb_4_45.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         4
c initial_number_of_variables           180
c initial_number_of_clauses             2282
c initial_number_of_literal_occurrences 6527
c number_of_initial_unit-eliminations   5
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           5
c reddiff_number_of_clauses             184
c reddiff_number_of_literal_occurrences 576
c number_of_2-clauses_after_reduction   386
c running_time(sec)                     16.4
c number_of_nodes                       170543
c number_of_single_nodes                1420
c number_of_quasi_single_nodes          0
c number_of_2-reductions                766490
c number_of_pure_literals               0
c number_of_autarkies                   5589
c number_of_missed_single_nodes         3473
c max_tree_depth                        33
c proportion_searched                   9.998047e-01
c proportion_single                     1.953030e-04
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 8386016
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_sb_4_45.cnf


schur(5) >= 160 (with subsumption-elimination; see
Investigations/RamseyTheory/SchurProblems/plans/general.hpp)

> OKsolver_2002-O3-DNDEBUG -M -D20 Schur_5_159.cnf
 Schur_5_159.cnf,   20,    1048576
level: nodes processed, average nodes, predicted total nodes, time for monitoring node, average time, predicted remaining time, single nodes, autarkies, depth

...
    83:  55910  37832.75  3.97E+10  1319.32s   902.30s    29y 364d 16h 27m 14s     0     0   74
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         5
c initial_number_of_variables           795
c initial_number_of_clauses             33084
c initial_number_of_literal_occurrences 97585
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   1985
c running_time(sec)                     76401.9
c number_of_nodes                       3208666
c number_of_single_nodes                203
c number_of_quasi_single_nodes          0
c number_of_2-reductions                44073479
c number_of_pure_literals               0
c number_of_autarkies                   2
c number_of_missed_single_nodes         4728
c max_tree_depth                        74
c proportion_searched                   7.934820e-05
c proportion_single                     4.956919e-11
c total_proportion                      7.934824679978192e-05
c number_of_table_enlargements          0
c number_of_1-autarkies                 890963760
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Schur_5_159.cnf

OKplatform> OKsolver_2002-O3-DNDEBUG Schur_5_119.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=595 initial_number_of_clauses=19009 initial_number_of_literal_occurrences=55780 running_time(s)=74.7 number_of_nodes=16042 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=282401 number_of_pure_literals=0 number_of_autarkies=13 number_of_missed_single_nodes=1 max_tree_depth=47 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=2874235 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1485 file_name=Schur_5_119.cnf
   \endverbatim
   </li>
   <li> On the unsatisfiable instances according to
   http://www.di.univaq.it/~formisano/CLPASP/node3.html
   only relsat is faster, while on the satisfiable instances
   only simo is faster. </li>
   <li> The minisat2-preprocessor doesn't seem to help:
   \verbatim
OKplatform> OKsolver_2002-m2pp Schur_5_119.cnf

s UNKNOWN
c sat_status=2 initial_maximal_clause_length=12 initial_number_of_variables=476 initial_number_of_clauses=18219 initial_number_of_literal_occurrences=84980 running_time(s)=560.1 number_of_nodes=132039 number_of_single_nodes=269 number_of_quasi_single_nodes=0 number_of_2-reductions=1694425 number_of_pure_literals=0 number_of_autarkies=1 number_of_missed_single_nodes=449 max_tree_depth=59 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=47 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=950 file_name=Schur_5_119.cnf_m2pp_7169
   \endverbatim
   </li>
  </ul>


  \todo march_pl
  <ul>
   <li> Satisfiability of Schur_4_44.cnf is also trivial. </li>
   <li> And unsatisfiability of Schur_4_45.cnf is established quickly (similar
   to satz):
   \verbatim
> march_pl Schur_4_45.cnf
c main():: nodeCount: 88863
c main():: dead ends in main: 2888
c main():: lookAheadCount: 19838572
c main():: unitResolveCount: 414098
c main():: time=36.160000
c main():: necessary_assignments: 15926
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 363057, succes #: 287974
c main():: doublelook: overall 1.865 of all possible doublelooks executed
c main():: doublelook: succesrate: 79.319, average DL_trigger: 83.071
s UNSATISFIABLE

> march_pl Schur_sb_4_45.cnf
c main():: nodeCount: 23940
c main():: dead ends in main: 1013
c main():: lookAheadCount: 5146096
c main():: unitResolveCount: 110786
c main():: time=9.080000
c main():: necessary_assignments: 4795
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 91007, succes #: 71949
c main():: doublelook: overall 1.803 of all possible doublelooks executed
c main():: doublelook: succesrate: 79.059, average DL_trigger: 80.526
s UNSATISFIABLE
   \endverbatim
   </li>
   <li> Satisfiability of Schur_5_119.cnf established with 65945 nodes (in
   ~ 204s). </li>
  </ul>


  \todo satz
  <ul>
   <li> schur(4):
   \verbatim
> satz215 Schur_4_44.cnf
**** The instance is satisfiable. *****
NB_MONO= 0, NB_UNIT= 4267, NB_BRANCHE= 204, NB_BACK= 96
Program terminated in 0.010 seconds.
satz215 Schur_4_44.cnf 0.010 204 96 16000 452 1 176 2188 0 267 88

> satz215 Schur_4_45.cnf
**** The instance is unsatisfiable. *****
NB_MONO= 0, NB_UNIT= 10689225, NB_BRANCHE= 432553, NB_BACK= 219567
Program terminated in 36.770 seconds.
satz215 Schur_4_45.cnf 36.770 432553 219567 37340648 1381151 0 180 2279 0 1138582 328260

> satz215 Schur_sb_4_45.cnf
**** The instance is unsatisfiable. *****
NB_MONO= 0, NB_UNIT= 2762779, NB_BRANCHE= 111957, NB_BACK= 56817
Program terminated in 9.290 seconds.
satz215 Schur_sb_4_45.cnf 9.290 111957 56817 9542105 354489 0 180 2282 -204 287189 82420
   \endverbatim
   </li>
  </ul>


  \todo minisat
  <ul>
   <li> Satisfiability of Schur_4_44.cnf is also trivial. </li>
   <li> But unsatisfiability of Schur_4_45.cnf is more problematic,
   needs 7091007 conflicts with minisat2 (26 restarts, 290s):
   \verbatim
> minisat-2.2.0 Schur_4_45.cnf
interrupted after 192.148 s

> minisat-2.2.0 Schur_sb_4_45.cnf
restarts              : 8191
conflicts             : 5128838        (68515 /sec)
decisions             : 6094199        (0.00 % random) (81412 /sec)
propagations          : 109527967      (1463170 /sec)
conflict literals     : 83911520       (25.26 % deleted)
Memory used           : 19.00 MB
CPU time              : 74.8566 s
   \endverbatim
   </li>
   <li> So apparently minisat2 is not very strong here. </li>
   <li> The best conflict-driven solver seems picosat, which still is slower
   than the worst lookahead-solver (OKsolver-2002). </li>
  </ul>


  \todo picosat and precosat
  <ul>
   <li> schur(4):
   \verbatim
> picosat913 Schur_4_45.cnf
s UNSATISFIABLE
c 16 iterations
c 183 restarts
c 2 failed literals
c 3939604 conflicts
c 4599971 decisions
c 134 fixed variables
c 56552941 learned literals
c 34.4% deleted literals
c 140582244 propagations
c 100.0% variables used
c 70.0 seconds in library
c 2.0 megaprops/second
c 7 simplifications
c 1089 reductions
c 290.6 MB recycled
c 1.0 MB maximally allocated
c 70.0 seconds total run time

> picosat913 Schur_sb_4_45.cnf
s UNSATISFIABLE
c 20 iterations
c 41 restarts
c 1 failed literals
c 876447 conflicts
c 1021412 decisions
c 78 fixed variables
c 11788666 learned literals
c 34.3% deleted literals
c 31317156 propagations
c 100.0% variables used
c 13.6 seconds in library
c 2.3 megaprops/second
c 10 simplifications
c 356 reductions
c 61.4 MB recycled
c 0.6 MB maximally allocated
c 13.6 seconds total run time


> precosat-570.1 -v Schur_4_45.cnf
interrupted after 148.5 seconds
   \endverbatim
   </li>
  </ul>


  \todo Minisat-variants
  <ul>
   <li> Glucose
   \verbatim
> glucose-1.0 Schur_4_45.cnf
interrupted after 222.335 s

> glucose-2.0 Schur_4_45.cnf
c restarts              : 26
c nb ReduceDB           : 42
c nb removed Clauses    : 1920927
c nb learnts DL2        : 415
c nb learnts size 2     : 185
c nb learnts size 1     : 19
c conflicts             : 2530673        (16706 /sec)
c decisions             : 2844563        (0.00 % random) (18778 /sec)
c propagations          : 63780315       (421048 /sec)
c conflict literals     : 39237403       (38.63 % deleted)
c nb reduced Clauses    : 11216
c CPU time              : 151.48 s
s UNSATISFIABLE
   \endverbatim
   </li>
   <li> Cryptominisat
   \verbatim
> cryptominisat Schur_4_45.cnf
interrupted after 138.47 s
   \endverbatim
   </li>
  </ul>

*/


