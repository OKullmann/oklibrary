// Oliver Kullmann, 2.3.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/PigeonholeFormulas/plans/SATSolving.hpp
  \brief Investigations regarding SAT solvers for pigeonhole clause-sets


  \todo Statistics on the performance of SAT solvers
  <ul>
   <li> SAT instances created at Maxima level by
   output_weak_php_stdname(m,k) (m pigeons into k holes). </li>
   <li> We should at least consider PHP^{2m}_m, PHP^{m+1}_m amd PHP^m_m. </li>
   <li> Statistics for all our solvers, for m = 0, ... until it takes several
   hours, with curve fittings.
    <ol>
     <li> Cryptominisat 2.9.0:
      <ol>
       <li> PHP^16_15: solved in 75905.41s, with 43,150,728 conflicts (so here
       the "point of competence" seems to have been exceeded). </li>
       <li> PHP^15_14: solved in 3423.40s, with 6,488,768 conflicts. </li>
       <li> PHP^14_13: solved in 1032.63s, with 2,886,433 conflicts. </li>
       <li> PHP^13_12: solved in 286.78s and 1,215,119 conflicts. </li>
       <li> PHP^12_11: solved in 35.15s and 230,963 conflicts. </li>
       <li> PHP^22_11: solved in 1706.90s and 3,338,405 conflicts (quite a
       disaster!). </li>
      </ol>
     </li>
     <li> OKsolver-2002:
      <ol>
       <li> PHP^14_13: solved in 14326.7s, with 4,670,265,599 nodes. </li>
       <li> PHP^13_12: solved in 1145.5s, with 359,251,199 nodes. </li>
       <li> PHP^12_11: solved in 92.1s, with 29,937,599 nodes:
       \verbatim
c initial_maximal_clause_length         11
c initial_number_of_variables           132
c initial_number_of_clauses             738
c initial_number_of_literal_occurrences 1584
c number_of_2-clauses_after_reduction   726
c running_time(sec)                     92.1
c number_of_nodes                       29937599
c number_of_single_nodes                0
c number_of_quasi_single_nodes          9979200
c number_of_2-reductions                38253600
c number_of_pure_literals               12672550
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        55
c proportion_searched                   1.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 171073515
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             PHP_weak_12_11.cnf
       \endverbatim
       (here no missed single-nodes, but many quasi-single nodes).
       </li>
       <li> PHP^22_11: solved in 1887.0s, with 63,201,599 nodes:
       \verbatim
c initial_maximal_clause_length         11
c initial_number_of_variables           242
c initial_number_of_clauses             2563
c initial_number_of_literal_occurrences 5324
c number_of_2-clauses_after_reduction   2541
c running_time(sec)                     1887.0
c number_of_nodes                       63201599
c number_of_single_nodes                0
c number_of_quasi_single_nodes          26611200
c number_of_2-reductions                104781600
c number_of_pure_literals               12672550
c number_of_autarkies                   0
c number_of_missed_single_nodes         14968800
c max_tree_depth                        65
c proportion_searched                   1.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 990012515
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             PHP_weak_22_11.cnf
       \endverbatim
       Interesting the number of (missed!) single nodes, and the number
       of quasi-single nodes. Just trying the opposite branching-order XXX
       </li>
       <li> Finding a satisfying assignment is trivial:
       \verbatim
> OKsolver_2002-O3-DNDEBUG PHP_weak_50_50.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         50
c initial_number_of_variables           2500
c initial_number_of_clauses             61300
c initial_number_of_literal_occurrences 125000
c number_of_2-clauses_after_reduction   61250
c running_time(sec)                     0.7
c number_of_nodes                       49
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                2
c number_of_pure_literals               1222
c number_of_autarkies                   1
c number_of_missed_single_nodes         0
c max_tree_depth                        48
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
       \endverbatim
       </li>
      </ol>
     </li>
     <li> satz215
      <ol>
       <li> PHP^14_13: solved in 24468.120s. </li>
       <li> PHP^13_12: solved in 1814.540s. </li>
       <li> PHP^12_11: solved in 146.810s. </li>
       <li> Finding a satisfying assignment is trivial:
       \verbatim
> satz215 PHP_weak_50_50.cnf
satz215 PHP_weak_50_50.cnf 0.350 49 0 85848 0 1 2500 61300 0 0 0
       \endverbatim
       </li>
      </ol>
     </li>
     <li> march_pl
      <ol>
       <li> PHP^14_13: solved in time=38014.687500s with 1,076,150,280 nodes.
       </li>
       <li> PHP^13_12: solved in time=2845.439941s and 83,735,568 nodes. </li>
       <li> PHP^12_11: solved in time=219.839996s and 6,874,774 nodes. </li>
       <li> Finding a satisfying assignment is rather easy:
       \verbatim
> march_pl PHP_weak_50_50.cnf
c main():: nodeCount: 73
c main():: dead ends in main: 0
c main():: lookAheadCount: 85731
c main():: unitResolveCount: 2500
c main():: time=28.200001
c main():: necessary_assignments: 128
       \endverbatim
       </li>
      </ol>
     </li>
     <li> Precosat570:
      <ol>
       <li> PHP^14_13: solved in 70872.4s, with 43,974,359 conflicts. </li>
       <li> PHP^13_12: solved in 4174.1s, with 7,068,716 conflicts. </li>
       <li> PHP^12_11: solved in 251.2s, with 1,213,305 conflicts. </li>
       <li> PHP^22_11: solved in 3086.2s, with 4,079,747 conflicts. </li>
      </ol>
     </li>
     <li> Glucose 2.0:
      <ol>
       <li> PHP^12_11: solved in time=769.392s and 2480875 conflicts. </li>
      </ol>
     </li>
     <li> Glucose 1.0:
      <ol>
       <li> PHP^12_11: solved in time=1529.39s and 4,938,313 conflicts. </li>
      </ol>
     </li>
     <li> minisat-2.2.0:
      <ol>
       <li> PHP^16_15: After 2650m (nearly 2 days) and more than 3,317,199,477
       conflicts no visible progress. </li>
       <li> PHP^15_14: After 137328s (38h) and 3,348,376,683 conflicts no
       visible progress. </li>
       <li> PHP^14_13: After 112273s and 3,349,660,843 conflicts no
       visible progress. </li>
       <li> PHP^13_12: After 156595s and 5,596,327,305 conflicts no
       visible progress. </li>
       <li> PHP^12_11: solved in 7091.04s and 214,027,827 conflicts. </li>
       <li> Finding a satisfying assignment is easy:
       \verbatim
> minisat-2.2.0 PHP_weak_50_50.cnf
conflicts             : 0              (0 /sec)
decisions             : 1227           (0.00 % random) (17531 /sec)
CPU time              : 0.069989 s
       \endverbatim
       </li>
      </ol>
     </li>
     <li> Picosat913:
      <ol>
       <li> PHP^12_11: solved in 9365.7s, with 457,292,071 conflicts. </li>
      </ol>
     </li>
     <li> Grasp:
      <ol>
       <li> PHP^12_11: self-abortion after 4.282s and 1595 conflicts. It says
       "Terminating NSAT" ? Likely "RESOURCES EXCEEDED" in the output is
       to be taken seriously. We need to ask Joao Marques-Silva. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Also SplittingViaOKsolver+minisat is to be considered.
    <ol>
     <li> For PHP^16_15 apparenty D >= 100 for the splitting should be
     considered. </li>
     <li> However also for D=100 it doesn't seem successful; so perhaps
     higher D needs to be used. </li>
     <li> Perhaps these are examples where this approach is not efficient.
     Perhaps since even for D=100 no r_2-reductions were found? </li>
     <li> For D=120 360360 sub-instances were created (again 0 2-reductions).
     And regarding the full assignments, all assignments achieved *exactly*
     130 variables. While the number of decision-variables in them range
     from 5 to 60 (without gaps). This seems all to be very systematic. </li>
     <li> Still minisat-2.2.0 takes a long time just on the first instance.
     This perhaps is much worse than minisat-2.2.0 alone on the instance. </li>
     <li> Perhaps here the application of the partial assignments doesn't make
     the instances much easier. </li>
     <li> On the other hand, the problem seems very hard for minisat-2.2.0,
     as seen above. </li>
    </ol>
   </li>
   <li> We also consider the performance of E_1(m) and E_2(m), where E_1 is
   the original Cook-extension of PHP^{m+1}_m, and E_2 is the extension of E_1
   obtained by using the resolution-refutation. </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/OKsolver2002.hpp
   for some investigations into the trees computed by the OKsolver-2002. </li>
  </ul>

*/
