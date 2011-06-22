// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/27.hpp
  \brief On investigations into vdw_2(3,27)


  \todo vanderwaerden_2(3,27) >= 770
  <ul>
   <li> The conjecture is vanderwaerden_2(3,27) = 770. </li>
   <li> Search starting with n=610:
   \verbatim
OKplatform> RunVdW3k 27 610 gsat-tabu 100 100000000
   \endverbatim
   </li>
   <li> Perhaps also here the cutoff-value is too large:
   \verbatim
OKplatform> RunVdW3k 27 678 gsat-tabu 1000 10000000
   \endverbatim
   </li>
   <li> n=682 found satisfiable with certificate
    \verbatim
10,25,42,53,58,68,71,86,102,115,
116,129,139,145,158,182,184,197,198,202,
213,227,231,240,247,269,276,284,290,313,
314,319,327,334,361,363,372,377,387,405,
406,419,445,448,458,459,463,474,488,492,
501,506,508,530,537,545,551,574,588,595,
617,619,622,633,648,666
    \endverbatim
   </li>
   <li> n=725 found satisfiable with certificate
    \verbatim
18,23,36,55,57,75,88,98,110,117,
125,129,134,149,155,166,168,171,172,186,
191,192,194,199,205,231,236,258,282,283,
305,316,320,332,334,339,342,351,369,377,
388,408,413,414,421,431,443,450,458,462,
467,488,499,501,504,505,525,527,532,538,
556,564,569,591,598,612,615,616,638,649,
653,665,672,675,689,710
    \endverbatim
   </li>
   <li> Restarting the search, now using the solution found for n=725
   as initial assignment:
   \verbatim
OKplatform> cat Exp_VanderWaerden_2-3-27_2010-10-12-161406_678/VanDerWaerden_2-3-27_725.cnf_OUT | extract_solution_ubcsat > Solution_n725
OKplatform> RunVdW3k 27 726 rots 1000 5000000 Solution_n725
   \endverbatim
   </li>
   <li> n=769:
    <ol>
     <li> Found satisfiable with certificate
      \verbatim
25,29,35,54,72,74,77,99,103,111,
114,135,136,149,165,176,188,192,202,209,
223,246,250,251,260,266,287,294,296,313,
321,325,331,333,334,336,358,361,362,377,
387,405,407,410,414,432,436,442,444,447,
468,469,482,498,521,525,535,542,556,579,
583,584,593,599,620,629,646,654,658,664,
666,667,669,694,701,710,731,747,764
      \endverbatim
     </li>
     <li> 1000 runs with rots and cutoff=5000000 found 4 solutions, from which
     (only) one is reproducible (due to a bug in Ubcsat, now resolved):
     \verbatim
  0   1   2   3   4   5   6   7   9  11  12  13  14  15  16  17  18  19
  4 473 315  71   7   4   2   1   2   2   8  13  15  32  26  14   9   2
1000
> E[E$sat==1,]
    sat min  osteps  msteps       seed
107   1   0 2782015 2782015 2990676734
435   1   0 1430527 1430527 1209355894
467   1   0 3552707 3552707 3160209839
656   1   0 3273460 3273460  813836620

> ubcsat-okl -alg rots -cutoff 1430527 -seed 1209355894 -i Exp_VanderWaerden_2-3-27_2010-10-13-184057_726_rots-1000-5000000/VanDerWaerden_2-3-27_769.cnf -solve | tee VanDerWaerden_2-3-27_769.cnf_OUT3
> ExtractCertificate_vdW2 VanDerWaerden_2-3-27_769.cnf_OUT3
6,23,39,60,69,76,101,103,104,106,
112,116,124,141,150,171,177,186,187,191,
214,228,235,245,249,272,288,301,302,323,
326,328,334,338,339,356,360,363,365,383,
393,408,409,412,434,436,437,439,445,449,
457,474,476,483,504,510,519,520,524,547,
561,568,578,582,594,605,621,634,635,656,
659,667,671,693,696,698,716,741,745
     \endverbatim
     </li>
     <li> Cutoff=10^6, rots:
     \verbatim
 1  2  3  4  5  6  9 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 
26 58 51 12  6  6  1  2  3  9  9 21 27 38 50 57 30 35 32 18  6  3 
500 
     \endverbatim
     </li>
     <li> Cutoff=10^7, rots, finds a solution in run 209, the same as last one
     above, but without vertex 339. </li>
     <li> Cutoff=2*10^7, rots, finds a solution in run 211 (osteps=7295133),
     the same as last one above. </li>
    </ol>
   </li>
   <li> n=770:
    <ol>
     <li> Cutoff=5*10^6, rots:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20
451 349  80   9   2   1   1   1   1   1   4   4  11  13  27  17  13   9   3   1
 21  22
  1   1
1000
  1   2   3   4   5   6   8   9  12  13  14  15  16  17  18  19
452 361  72   8   2   1   1   1   7   4  18  31  11  16  13   2
1000
     \endverbatim
     </li>
     <li> Cutoff=10^7, rots:
     \verbatim
  1   2   3  14  15  16
354 138   5   1   1   1
500
  1   2   3   4  10  13  14  15  17 
369 117   5   1   1   2   3   1   1 
500
  1   2   3   5  13  14  15  16  18
711 253  16   3   4   4   6   2   1
1000
     \endverbatim
     </li>
     <li> Cutoff=2*10^7, rots:
     \verbatim
  1   2   3                                 
131  11   1
143                            
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo vdw_2^pd(3,27) =(conj) (664, 699)
  <ul>
   <li> 698 < pdvanderwaerden([3,27])[2] =(conj) 699 =(conj)
   vanderwaerden([3,27]) - 71. </li>
   <li> pdvanderwaerden([3,27])[1] = 664. </li>
   <li> Certificates:
    <ol>
     <li> n=663:
     \verbatim
19,38,61,67,81,82,104,109,119,120,
123,132,135,139,148,149,162,181,191,206,
212,222,241,254,255,268,283,284,294,297,
323,328
     \endverbatim
     </li>
     <li> n=698:
     \verbatim
23,30,56,67,93,105,106,116,118,124,
128,136,137,147,162,186,190,191,211,229,
230,234,240,253,265,273,283,284,292,314,
327,341,348
     \endverbatim
     </li>
    </ol>
   </li>
   <li> SplittingViaOKsolver, using minisat-2.2.0:
    <ol>
     <li> n=700:
     \verbatim
> SplittingViaOKsolver -D70 VanDerWaerden_pd_2-3-27_700.cnf

     \endverbatim
     </li>
     <li> n=665:
     \verbatim
> SplittingViaOKsolver -D65 VanDerWaerden_pd_2-3-27_665.cnf
> more Md5sum
19228edf731c9135b8bba2c5fe4b0e64
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  65.00   68.00   72.00   73.67   79.00  181.00
> table(E$n)
   65    66    67    68    69    70    71    72    73    74    75    76    77
14246  6658  6706  7086  7363  7151  6515  5770  4944  4288  4117  3953  3962
   78    79    80    81    82    83    84    85    86    87    88    89    90
 4273  4381  4390  4416  4308  3852  3349  2469  1702  1186   775   545   428
   91    92    93    94    95    96    97    98    99   100   101   102   103
  290   195   114    62    48    22    12     4     4     6    11    14    12
  104   105   106   107   108   109   110   111   112   113   114   115   116
   16    11    17    17    14    20    11     9     5     5     3     3     3
  118   119   120   121   122   125   126   127   128   129   131   133   134
    1     1     2     2     1     1     3     2     1     1     1     1     1
  136   137   139   141   152   157   180   181
    1     2     1     1     1     1     1     1
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         27
c initial_number_of_variables           333
c initial_number_of_clauses             58526
c initial_number_of_literal_occurrences 263527
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   442
c running_time(sec)                     134471.6
c number_of_nodes                       239579
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                42914
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        39
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_pd_2-3-27_665.cnf
c splitting_directory                   SplitViaOKsolver_D65VanDerWaerden_pd_2327_665cnf_2011-06-13-115318/Instances
c splitting_cases                       119788

> cd Instances
> nohup OKP=~/SAT-Algorithmen/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C "; fi; done &

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
119788: 121.49h, sum-cfs=3.624973e+09, mean-t=3.651s, mean-cfs=30262
> summary(E$t)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
  0.0520   0.9841   1.7880   3.6510   3.6200 388.2000
> summary(E$cfs)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
     40    8176   15840   30260   30970 2572000
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdWk1k2 3 27 gsat-tabu 100 6000000" yields
   \verbatim
Break point 1: 664
Break point 2: 699
   \endverbatim
   All solutions found within the first two runs. </li>
  </ul>

*/
