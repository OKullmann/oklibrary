// Oliver Kullmann, 21.8.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/07.hpp
  \brief On investigations into vdw_2(5,6) = 206


  \todo Picosat913
  <ul>
   <li> Aborted picosat913 after 6 days without visible progress. </li>
  </ul>


  \todo OKsolver_2002
  <ul>
   <li>
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
  </ul>


  \todo SplittingViaOKsolver
  <ul>
   <li> First with minisat-2.2.0:
   \verbatim
> SplittingViaOKsolver -D20 VanDerWaerden_2-5-6_206.cnf
> cd SplitViaOKsolver_D20VanDerWaerden_256_206cnf_2011-08-21-175644/
> more Md5sum
917bf02abe81e276a67853fcd57d10a8
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  20.00   20.00   20.00   20.82   21.00   45.00
> table(E$n)
   20    21    22    23    24    25    26    27    28    29    30    31    32
49910 20063 13450  4505  1984   639   266   100    40    19    12     3     4
   34    35    36    40    45
    1     1     2     1     1
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         6
c initial_number_of_variables           206
c initial_number_of_clauses             9343
c initial_number_of_literal_occurrences 50856
c running_time(sec)                     364.9
c number_of_nodes                       182003
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                488
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        20
c file_name                             VanDerWaerden_2-5-6_206.cnf
c splitting_directory                   SplitViaOKsolver_D20VanDerWaerden_256_206cnf_2011-08-21-175644/Instances
c splitting_cases                       91001

   \endverbatim
   </li>
  </ul>

*/
