// Oliver Kullmann, 11.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/13.hpp
  \brief On investigations into vdw_2(4,13)


  \todo Best ubcsat-solver
  <ul>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
  </ul>


  \todo vanderwaerden_2(4,13) > 537
  <ul>
   <li> Start with the palindromic solution for n=537. </li>
  </ul>


  \todo vdw_2^pd(4,13) >= (519,538)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=518:
     \verbatim
11,13,26,27,28,31,34,38,43,49,
52,65,66,79,82,88,89,93,100,103,
104,105,111,118,120,123,124,128,129,131,
135,137,140,146,153,163,172,176,179,186,
188,190,196,197,200,205,207,211,215,217,
220,221,222,226,228,234,236,237,246,250,
259
     \endverbatim
     </li>
     <li> n=537:
     \verbatim
2,15,19,21,22,26,34,35,38,39,
44,46,59,68,70,72,73,75,77,78,
80,82,91,99,104,106,111,112,115,116,
128,129,131,134,135,136,148,156,159,165,
167,170,175,185,196,201,203,209,210,212,
213,215,216,225,226,229,231,233,238,240,
253,256,262,264,266,267,269
     \endverbatim
    </ol>
   </li>
   <li> Splitting the problem for n=520:
   \verbatim
> SplittingViaOKsolver -D36 VanDerWaerden_pd_2-4-13_520.cnf
> cd SplitViaOKsolver_D36VanDerWaerden_pd_2413_520cnf_2011-08-16-014738
kullmann-0:SplitViaOKsolver_D36VanDerWaerden_pd_2413_520cnf_2011-08-16-014738> cat Md5sum
d7d5e27d64ecbc33f704eb5ea4dcdd17
> cat Statistics
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   36.0    36.0    38.0    38.1    39.0    74.0
> table(E$n)
    36     37     38     39     40     41     42     43     44     45     46
456354 206435 200482 177376 133722  86738  49517  25284  12916   6934   3842
    47     48     49     50     51     52     53     54     55     56     57
  2255   1379    777    480    288    187    133     90     68     32     33
    58     59     60     61     62     63     64     65     67     70     71
    19     19     10     14     12      2      6      5      5      1      1
    72     74
     1      1
> cat Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         13
c initial_number_of_variables           260
c initial_number_of_clauses             27605
c initial_number_of_literal_occurrences 158420
c number_of_2-clauses_after_reduction   87
c running_time(sec)                     96449.7
c number_of_nodes                       2730839
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                70732
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        32
c file_name                             VanDerWaerden_pd_2-4-13_520.cnf
c splitting_directory                   SplitViaOKsolver_D36VanDerWaerden_pd_2413_520cnf_2011-08-16-014738/Instances
c splitting_cases                       1365418

# on cs-oksvr:
> nohup ProcessSplitViaOKsolver SplitViaOKsolver_D36VanDerWaerden_pd_2413_520cnf_2011-08-16-014738 &
   \endverbatim
   </li>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
   <li> For now we consider adaptg2wsat as best (it was determined as best
   for vdw_2^pd(5,8) in VanderWaerdenProblems/plans/5-k/general.hpp. </li>
   <li> "RunPdVdWk1k2 4 13 adaptg2wsat 100 1000000" yields
   \verbatim
Break point 1: 519
Break point 2: 538
   \endverbatim
   </li>
   <li> "RunPdVdWk1k2 4 13 adaptg2wsat 100 10000000" yields
   \verbatim
Break point 1: 519
Break point 2: 538
   \endverbatim
   All solutions found in the first round using the previous solution (using
   at most 2609468 steps).
   </li>
  </ul>

*/
