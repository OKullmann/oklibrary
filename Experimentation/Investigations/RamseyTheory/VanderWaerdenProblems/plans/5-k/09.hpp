// Oliver Kullmann, 12.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/09.hpp
  \brief On investigations into vdw_2(5,9)


  \todo Best complete solver for palindromic problems
  <ul>
   <li> t seems that from the conflict-driven solvers minisat-2.2.0 is best,
   from the look-ahead solvers march_pl (followed by OKsolver_2002, then
   satz215), and that overall minisat-2.2.0 is best. </li>
  </ul>


  \todo Best local-search solver for palindromic problems


  \todo vdw_2^pd(5,9) >= (446,473)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=445:
     \verbatim
1,2,4,11,12,20,21,26,31,34,
36,38,42,45,48,49,51,53,58,59,
60,62,65,66,67,69,70,72,73,82,
83,85,86,88,89,90,95,97,98,102,
110,113,119,121,123,124,126,128,129,131,
133,139,142,150,155,157,162,163,164,166,
167,169,170,179,180,182,183,185,186,187,
192,194,199,207,210,216,218,220,221,223
     \endverbatim
     Another solution (closely related):
     \verbatim
1,2,11,12,20,21,26,28,31,34,
36,42,45,48,49,51,53,58,59,60,
62,65,66,67,69,70,72,73,82,83,
85,86,88,89,90,95,97,98,102,110,
113,119,121,123,124,126,128,129,131,133,
139,142,150,155,157,162,163,164,166,167,
169,170,179,180,182,183,185,186,187,192,
194,199,207,210,216,218,220,221,223
     \endverbatim
     </li>
     <li> n=472:
     \verbatim
4,6,8,9,13,17,19,24,27,28,
34,39,41,43,44,45,48,52,53,56,
57,59,63,65,70,72,77,78,80,82,
85,94,95,97,98,100,101,102,106,108,
112,114,116,117,123,126,130,139,140,144,
149,153,156,162,163,165,167,171,173,174,
177,178,179,182,184,188,192,194,197,198,
199,202,203,205,209,211,213,214,220,223,
227,236
     \endverbatim
    </ol>
   </li>
   <li> Using SplittingViaOKsolver together with minisat-2.2.0:
    <ol>
     <li> n=447:
     \verbatim
> SplittingViaOKsolver -D17 VanDerWaerden_pd_2-5-9_447.cnf
> cd SplitViaOKsolver_D17VanDerWaerden_pd_259_447cnf_2011-05-18-182137/
> more Md5sum
f11f67fd06ffd653818137f130c07a7c
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  17.00   24.00   29.00   30.12   35.00   86.00
> table(E$n)
  17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32
  84  621 1875 3835 6179 8093 8762 8359 7418 6268 5585 4964 5027 5166 5479 5613
  33   34   35   36   37   38   39   40   41   42   43   44   45   46   47   48
5610 5581 5324 4833 4192 3606 2998 2614 2184 1799 1487 1153  971  794  629  488
  49   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64
 427  361  285  219  219  170  127  105   72   87   54   64   40   39   25   22
  65   66   67   68   69   70   71   72   73   74   75   76   77   78   80   81
  23   24   18   18    9   11    5   10    8    6    3    2    1    3    3    1
  85   86
   1    1
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         9
c initial_number_of_variables           224
c initial_number_of_clauses             18025
c initial_number_of_literal_occurrences 113027
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     6306.2
c number_of_nodes                       261725
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                25028
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        17
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_pd_2-5-9_447.cnf
c splitting_directory                   SplitViaOKsolver_D17VanDerWaerden_pd_259_447cnf_2011-05-18-182137/Instances
c splitting_cases                       130054

> E=read.table("Data")
> E[E$n==20,][1:10,]
          i  n
123640 1255 20
123641 1646 20
123642 1647 20
123643 1663 20
123644 1664 20
123645 1711 20
123646 1712 20
123647 1883 20
123648 1884 20
123649 1885 20

> cat VanDerWaerden_pd_2-5-9_447.cnf | ApplyPass-O3-DNDEBUG Instances/1255 > Test1255.cnf
> minisat-2.2.0 Test1255.cnf
restarts              : 13820
conflicts             : 8521124        (12794 /sec)
decisions             : 9898141        (0.00 % random) (14861 /sec)
propagations          : 212201322      (318597 /sec)
conflict literals     : 195070169      (27.77 % deleted)
Memory used           : 44.00 MB
CPU time              : 666.05 s
INDETERMINATE
     \endverbatim
     This looks rather bleak.
     </li>
     <li> Okay, let's set n=23:
     \verbatim
> SplittingViaOKsolver -D23 -SN VanDerWaerden_pd_2-5-9_447.cnf

     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdWk1k2 5 9 adaptg2wsat 100 2000000" yields
   \verbatim
Break point 1: 446
Break point 2: 451
   \endverbatim
   finding all solutions in at most 53 runs.
   </li>
   <li> "RunPdVdWk1k2 5 9 adaptg2wsat 100 10000000" yields
   \verbatim
Break point 1: 446
Break point 2: 473
   \endverbatim
   finding all solutions in at most 21 runs.
   </li>
  </ul>

*/
