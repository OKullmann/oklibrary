// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/15.hpp
  \brief On investigations into vdw_2(3,15) = 218


  \todo OKsolver
  <ul>
   <li> n=217, 218:
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D26 -F VanDerWaerden_2-3-15_217.cnf
  4597:  12803   1046.97  7.03E+10   225.36s    18.37s    39y  35d 13h  0m 53s     0     0   60
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         15
c initial_number_of_variables           217
c initial_number_of_clauses             13239
c initial_number_of_literal_occurrences 58617
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     84676.4
c number_of_nodes                       4830513
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                47307023
c number_of_pure_literals               2275
c number_of_autarkies                   0
c number_of_missed_single_nodes         3
c max_tree_depth                        60
c number_of_table_enlargements          0
c number_of_1-autarkies                 40863
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-15_217.cnf

> E = read_oksolver_mon("VanDerWaerden_2-3-15_217.cnf.mo")
4012
> plot_oksolver_mon_nodes(E)
ldstep= 9 step= 512 left= 128 right= 4597
obs/count= 1.150283 nodes-range= 1 64636 ave-nodes-range= 727.307 2436.24
> summary_oksolver(E)
Nodes:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
      1       5      22    1200     167   64640
2-reductions:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   0.96    9.53   10.27   11.32   11.65   56.00
Single nodes:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
      0       0       0       0       0       0
Autarkies:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
      0       0       0       0       0       0
Time ~ nodes:
[1] 0.9664525
(Intercept)     E$nodes
 0.33440742  0.01727101
> hist_oksolver_mon_nodes(E,breaks="st")
Median= 4.459432
Mean= 10.22837
   \endverbatim
   </li>
   <li> Node counts show typical quasi-periodic patterns, distribution with
   two peaks, one close to 1, the other at around 12. Here it seems satz215
   is definitely better, which should be either due to the heuristics (by
   chance?!), or the partial r_3-reduction, or both. </li>
  </ul>


  \todo satz215
  <ul>
   <li> n=218:
   \verbatim
****the instance is unsatisfiable *****
NB_MONO= 2544, NB_UNIT= 1879235364, NB_BRANCHE= 66642611, NB_BACK= 33796681
Program terminated in 99407.850 seconds.
satz215 VanDerWaerden_2-3-15_218.cnf 99407.850 66642611 33796681 10812633494 373672422 0 218 13362 0 305553394 82352337
   \endverbatim
   (that is 27 h) </li>
  </ul>


  \todo Splitting via OKsolver
  <ul>
   <li> Starting with n=26:
   \verbatim
# csltok:
> SplittingViaOKsolver -D26 VanDerWaerden_2-3-15_218.cnf
  26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41
1202  505  641  612  621  535  472  388  268  186   76   23   19   14   17   12
  42   43   44
  12   13    3
> SplittingViaOKsolver -D30 VanDerWaerden_2-3-15_218.cnf
> cd more Md5sum
f5c6e03a97f4e218cfa29389a9701569
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  30.00   31.00   34.00   33.99   36.00   52.00
> table(E$n)
  30   31   32   33   34   35   36   37   38   39   40   41   42   43   44   45
2955  926 1023 1100 1131 1344 1276 1134  963  594  249   88   39   30   19   15
  46   47   48   49   50   51   52
  10    5   10    6    2    1    2
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         15
c initial_number_of_variables           218
c initial_number_of_clauses             13362
c initial_number_of_literal_occurrences 59166
c running_time(sec)                     294.1
c number_of_nodes                       25843
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1023
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        26
c file_name                             VanDerWaerden_2-3-15_218.cnf
c splitting_directory                   SplitViaOKsolver_D30VanDerWaerden_2315_218cnf_2011-05-29-010530/Instances
c splitting_cases                       12922

# running times (simple loop)
real    251m42.474s
user    217m52.028s
sys     2m21.823s
# Monitoring in R
12922: 3.40h, sum-cfs=3.806417e+08, mean-t=0.947s, mean-cfs=29457
# Times partially false (reduced clock-frequence), but perhaps not too badly.

# csltok:
> SplittingViaOKsolver -D35 VanDerWaerden_2-3-15_218.cnf
> more Md5sum
e1f1dde53f84a27d52d77e1860507fda
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  35.00   36.00   39.00   39.14   41.00   83.00
> table(E$n)
  35   36   37   38   39   40   41   42   43   44   45   46   47   48   49   50
6468 2883 2981 3209 3278 3136 2764 2221 1763 1318  869  556  236  104   72   69
  51   52   53   54   55   56   57   58   59   60   61   63   66   69   83
 102   97   78   59   27   25    1    5    3    2    1    1    1    1    1
>
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         15
c initial_number_of_variables           218
c initial_number_of_clauses             13362
c initial_number_of_literal_occurrences 59166
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     1109.0
c number_of_nodes                       64663
c number_of_2-reductions                4170
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        29
c file_name                             VanDerWaerden_2-3-15_218.cnf
c splitting_directory                   SplitViaOKsolver_D35VanDerWaerden_2315_218cnf_2011-08-20-014927/
Instances
c splitting_cases                       32331

> E=read_processsplit_minisat()
32331: 2.83h, sum-cfs=3.615115e+08, mean-t=0.315s, mean-cfs=11182
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
0.009998 0.136000 0.237000 0.315200 0.405900 5.435000
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
     41    4755    8441   11180   14400  180100
   \endverbatim
    </li>
   <li> Now we need to test how satz215 as sub-solver does.
   \verbatim
SplitViaOKsolver_D30SNVanDerWaerden_2315_218cnf_2011-05-29-010530> tail Data
12913 12893 30
> cat VanDerWaerden_2-3-15_218.cnf | ApplyPass-O3-DNDEBUG Instances/12893 I12893.cnf
> satz215 I12893.cnf
Aborted after 14m.
   \endverbatim
   </li>
   <li> So it seems that the combination of OKsolver for splitting and
   conflict-driven solver for solving is the winning combination. </li>
  </ul>


  \todo vdw_2^pd(3,15) = (200,205)
  <ul>
   <li> pdvanderwaerden([3,15])[2] = 205 = vanderwaerden([3,15])-13 . </li>
   <li> pdvanderwaerden([3,15])[1] = 200. </li>
   <li> OKsolver_2002 easily determines n=204 as satisfiable and n=201,
   205,...,218 as unsatisfiable. </li>
   <li> So does satz215, march_pl and minisat2. </li>
   <li> "RunPdVdWk1k2 3 15 gsat-tabu 100 200000" yields
   \verbatim
Break point 1: 200
Break point 2: 205
   \endverbatim
   where all solutions were found in the first three runs. </li>
  </ul>

*/
