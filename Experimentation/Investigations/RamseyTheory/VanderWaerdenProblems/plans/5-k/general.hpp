// Oliver Kullmann, 9.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/general.hpp
  \brief On investigations into vdW-numbers vdw_2(5,k)

  Instances created by output_vanderwaerden2nd_stdname(5,k,n) at Maxima-level,
  or by "VanderWaerden-O3-DNDEBUG 5 k n" at C++ level (instances differ by
  the order of negated literals in clauses).
  An ubcsat-experiment is run by "RunVdWk1k2 5 k n0 alg runs cutoff".

  The palindromic versions are created by
  output_pd_vanderwaerden2nd_stdname(5,k,n) at Maxima-level, or by
  "PdVanderWaerdenCNF-O3-DNDEBUG 5 k n" at C++ level (instances differ by
  the order of negated literals in clauses).
  An ubcsat-experiment is run by "RunPdVdWk1k2 5 k n0 alg runs cutoff".
  And running a complete solver by "CRunPdVdWk1k2 5 k solver".


  \todo Literature overview
  <ul>
   <li> Apparently nothing is known about vdw_2(5,k) ? </li>
   <li> Our conjecture is that it's polynomially bounded, </li>
   <li> The known values:
   \verbatim
create_list(vanderwaerden5k(k),k,1,1);
 [unknown]
   \endverbatim
   </li>
  </ul>


  \todo Best complete solver for vdw_2(5,k)
  <ul>
   <li> For vdw_2(4,k) in
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/general.hpp
   picosat913 seems strongest. </li>
   <li> k=5, n=178:
    <ol>
     <li> picosat913 needs 15.5 s and 491198 conflicts. </li>
     <li> precosat236 needs 55.5 seconds and 541873 conflicts. </li>
     <li> precosat-570.1 needs 65.1 seconds and 630130 conflicts. </li>
     <li> march_pl needs 97.129997 s and 51527 nodes. </li>
     <li> minisat-2.2.0 needs 119.879 s and 3698746 conflict. </li>
     <li> OKsolver_2002 needs 179.0 s and 109721 nodes. </li>
     <li> glucose needs 183.496 s and 1163241 conflicts. </li>
     <li> minisat2 needs 279.645 s and 881363 conflicts. </li>
     <li> cryptominisat needs 334.18 s and 1485531 conflicts. </li>
     <li> satz215 aborted after 800 s. </li>
    </ol>
   </li>
   <li> k=6, n=206:
    <ol>
     <li> Aborted picosat913 after 6 days without visible progress. </li>
     <li> OKsolver_2002:
     \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D18 -F VanDerWaerden_2-5-6_206.cnf
 20887:   1920   2029.39  5.32E+08    29.89s     9.17s     0y  25d 14h 49m 14s     0     0   50
 20888:   6265   2029.60  5.32E+08    68.87s     9.18s     0y  25d 15h  0m 34s     0     0   50
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         6
c initial_number_of_variables           206
c initial_number_of_clauses             9343
c initial_number_of_literal_occurrences 50856
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     192505.5
c number_of_nodes                       42482986
c number_of_single_nodes                83
c number_of_quasi_single_nodes          0
c number_of_2-reductions                177935418
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         1349
c max_tree_depth                        50
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-5-6_206.cnf
     \endverbatim
     The prediction of the number of nodes = 5.32E+08 seems reliable (stable
     behaviour, with on average around 4 r_2-reductions per node), but the
     time is much too high (speed-problems on csltok, roughly 10 times slower
     than normal). </li>
    </ol>
   </li>
  </ul>


  \todo Best complete solver for palindromic problems
  <ul>
   <li> Considering k=8, it seems that from the conflict-driven solvers
   minisat-2.2.0 is best, from the look-ahead solvers march_pl (followed
   by OKsolver_2002, then satz215), and that overall minisat-2.2.0 is best.
   See Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/08.hpp.
   </li>
   <li> k=6: minisat-2.2.0 determines easily (< 1s) vdw_2^pd(5,6) = (195,206).
   </li>
   <li> k=7: minisat-2.2.0 determines vdw_2^pd(5,7) = (227,236) (48,363,829
   conflicts for n=228, 267,783 conflicts for n=237). </li>
  </ul>


  \todo Best local-search solver for palindromic problems
  <ul>
   <li> adaptg2wsat seems clearly strongest for k=8 (see
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/08.hpp). </li>
  </ul>

*/
