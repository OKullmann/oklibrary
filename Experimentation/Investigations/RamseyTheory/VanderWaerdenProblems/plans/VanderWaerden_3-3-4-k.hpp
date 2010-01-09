// Oliver Kullmann, 8.1.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_3-3-4-k.hpp
  \brief On investigations into vdW-numbers vdw_3(3,4,k)

  Generated as non-boolean formal clause-set with uniform domain via
  vanderwaerden_nbfcsud([3,4,k],n) at Maxima-level.

  Using the (strong) standard translation, a standardised boolean clause-set
  is obtained by vanderwaerden_aloamo_stdfcs([3,4,k],n).

  Using the standard nested translation, a standardised boolean clause-set
  is obtained by vanderwaerden_standnest_stdfcs([3,4,k],n).

  File-output is obtained by output_vanderwaerden_stdname([3,4,k],n) resp.
  output_vanderwaerden_standnest_stdname([3,4,k],n).


  \todo Overview
  <ul>
   <li> The known values:
   \verbatim
create_list(vanderwaerden34k(k),k,1,5);
 [18,21,51,89,unknown]
   \endverbatim
   </li>
   <li> In [Landman, Robertson; Ramsey Theory on the Integers] we find
   "vanderwaerden_3(3,4,4) = 84", however below we see that
   vanderwaerden_3(3,4,4) > 88. </li>
   <li> The next unknown value to compute is vanderwaerden_3(3,4,5). </li>
  </ul>


  \todo vanderwaerden_3(3,4,4) = 89
  <ul>
   <li> First just using aloamo-translation without symmetry-breaking. </li>
   <li> Finding a satisfying assignment for n = 83 easy for OKsolver_2002 (in
   5958 nodes). </li>
   <li> Finding a satisfying assignment for n = 84 still relatively easy for
   OKsolver_2002 (in 95712 nodes; ~ 40s); however using monitor-depth 16, no
   node at level 16 was completed. </li>
   <li> Finding a satisfying assignment for n = 85 still relatively easy for
   OKsolver_2002 (in 90435 nodes). </li>
   <li> Finding a satisfying assignment for n = 86 still relatively easy for
   OKsolver_2002 (in 106506 nodes). </li>
   <li> Finding a satisfying assignment for n = 87 still relatively easy for
   OKsolver_2002 (in 111055 nodes). </li>
   <li> Finding a satisfying assignment for n = 88 quite hard:
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D20 VanDerWaerden_3-3-4-4_88.cnf
 66818:   1741    625.66  6.56E+08     0.99s     0.41s     0y   4d 16h 17m  4s     0     0   52

s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         4
c initial_number_of_variables           264
c initial_number_of_clauses             4738
c initial_number_of_literal_occurrences 16444
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   264
c running_time(sec)                     27511.5
c number_of_nodes                       41805610
c number_of_single_nodes                12
c number_of_quasi_single_nodes          0
c number_of_2-reductions                341702797
c number_of_pure_literals               0
c number_of_autarkies                   10
c number_of_missed_single_nodes         261
c max_tree_depth                        52
c number_of_table_enlargements          0
c number_of_1-autarkies                 3187088053
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_3-3-4-4_88.cnf
   \endverbatim
   </li>
   <li> n = 89 with OKsolver_2002: </li>
   <li> minisat2: Aborted for n=84 after 27 restarts (13m); it seems minisat2
   is weak on vdW-instances (though it could be here the case that for deciding
   unsatisfiability it's better). </li>
   <li> picosat:
    <ol>
     <li> Found a solution for n=84 in 3m17s. </li>
     <li> Found a solution for n=85 in 4m30s. </li>
     <li> Found a solution for n=86 in 1m18s. </li>
     <li> Found a solution for n=87 in 9m37s. </li>
     <li> n=88 </li>
    </ol>
   </li>
   </li>
   <li> march_pl: found a solution for n=84 in 5 1/2 m (117976 nodes). </li>
   <li> satz215: Aborted for n=84 after 9m; again it could be that it performs
   better on unsatisfiable instances. </li>
  </ul>
  
*/
