// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/26.hpp
  \brief On investigations into vdw_2(3,26)


  \todo vanderwaerden_2(3,26) >= 727
  <ul>
   <li> The conjecture is vanderwaerden_2(3,26) = 727. </li>
   <li> Search starting with n=600:
   \verbatim
OKplatform> RunVdW3k 26 600 gsat-tabu 100 100000000
   \endverbatim
   </li>
   <li> Perhaps also here the cutoff-value is too large:
   \verbatim
OKplatform> RunVdW3k 26 655 gsat-tabu 1000 10000000
   \endverbatim
   </li>
   <li> n=659 found satisfiable with certificate
   \verbatim
13,26,27,46,51,64,80,91,113,116,
120,128,138,142,157,159,166,179,188,207,
215,224,237,244,246,265,275,283,290,311,
316,324,340,348,352,353,374,377,381,389,
399,418,420,427,440,449,457,468,476,485,
505,507,522,526,536,544,548,551,572,573,
585,607,614,627,651
   \endverbatim
   </li>
   <li> n=677 found satisfiable with certificate
   \verbatim
5,12,14,38,39,58,68,76,79,91,
105,117,123,125,128,132,150,162,163,179,
191,197,200,202,216,223,243,260,264,273,
274,280,301,302,308,311,334,339,345,347,
350,371,372,391,401,409,412,424,438,450,
456,458,461,465,483,486,495,496,512,520,
524,530,533,535,549,556,576,593,606,607,
613,634,635,641,644,667,672
   \endverbatim
   </li>
   <li> Restarting the experiment, now using the solution found for n=677
   as initial assignment:
   \verbatim
OKplatform> cat Exp_VanderWaerden_2-3-26_2010-10-12-161754_655/VanDerWaerden_2-3-26_677.cnf_OUT | extract_solution_ubcsat > Solution_n677
OKplatform> RunVdW3k 26 678 rots 1000 5000000 Solution_n677
   \endverbatim
   </li>
   <li> n=726:
    <ol>
     <li> Found satisfiable with certificate
     \verbatim
11,35,46,48,69,74,86,93,105,106,
109,115,122,128,152,157,159,167,185,202,
204,216,217,220,222,226,231,234,253,257,
263,278,291,308,313,333,342,344,345,350,
364,379,381,402,407,426,438,439,442,448,
455,461,485,490,492,500,516,518,535,537,
549,550,553,555,559,564,567,586,596,611,
624,641,646,666,675,678,697,701
     \endverbatim
     </li>
     <li> rots with cutoff=10^7 found another solution in run 27:
     \verbatim
26,31,49,51,68,82,86,99,120,125,
144,156,157,160,162,173,174,193,197,203,
208,211,218,231,253,268,277,282,284,285,
290,314,319,321,329,347,364,366,378,382,
384,388,393,401,415,419,432,453,458,470,
477,489,490,493,495,506,507,526,530,536,
541,551,564,581,586,601,610,615,617,618,
623,647,652,654,680,699,711,721,726
     \endverbatim
     </li>
     <li> rots with cutoff=10^7 found another solution, caried over from n=725,
     very close to the above first solution:
     \verbatim
17,35,46,48,69,74,86,93,106,109,
115,122,128,152,157,159,167,185,202,204,
216,217,220,222,226,231,234,253,257,263,
278,291,308,313,333,342,344,345,350,364,
379,381,402,407,419,426,438,439,442,448,
455,461,485,490,492,500,516,518,535,537,
549,550,553,555,559,564,567,586,596,611,
624,641,646,666,675,678,697,701
     \endverbatim
     </li>
     <li> A different solution was found by rots with cutoff=2*10^7, caried
     over from n=725:
     \verbatim
1,6,16,28,47,73,75,80,104,109,
110,112,117,126,141,146,163,176,186,191,
197,201,220,221,234,237,238,250,257,269,
274,295,308,312,326,334,339,343,345,349,
361,363,380,398,406,408,413,437,442,443,
445,450,459,474,496,509,516,519,524,530,
534,553,554,565,567,570,571,583,602,607,
628,641,645,659,676,678,696,701
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=727
    <ol>
     <li> rots, cutoff=5*10^6:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
372 336  83   9   3   2   2  13  30  36  45  39  25   4   1
1000
     \endverbatim
     </li>
     <li> gsat-tabu, cutoff=10*10^6:
     \verbatim
  1   2   3   4   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21
 13  25  21   6   2   5  36  92 208 184 124  67  43  49  32  19  25  15   8   4
 22  23  24  25  26  27  28  29  30  33
  8   3   2   2   1   1   1   1   1   2
1000
     \endverbatim
     (so it seems rots is definitely better). </li>
     <li> rots, cutoff=10^7:
     \verbatim
  1   2   3   4   8   9  10  11
284 171  18   2   4  15   3   3
500
  1   2   3   5   7   8   9  10  11  12
286 183  10   3   2   2   5   3   2   4
500
  1   2   3   7   8   9  10  11  12
281 176  19   1   3   6   5   4   5
500
     \endverbatim
     </li>
     <li> rots, cutoff=2*10^7:
     \verbatim
  1   2   3   9
403  93   2   2
500
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo vdw_2^pd(3,26) : (=634, >=643)
  <ul>
   <li> 642 < pdvanderwaerden([3,26])[2] =(conj) 643 =(conj)
   vanderwaerden([3,26]) - 84. </li>
   <li> pdvanderwaerden([3,26])[1] =(conj) 634. </li>
   <li> Certificates:
    <ol>
     <li> n=633:
     \verbatim
4,18,23,36,57,79,86,105,113,120,
128,129,142,157,158,163,166,181,207,210,
215,216,231,244,245,260,265,268,282,287,
297,316
     \endverbatim
     </li>
     <li> n=642:
     \verbatim
16,40,47,61,66,81,84,98,110,126,
127,134,147,153,161,176,177,189,190,192,
206,211,229,234,248,255,263,272,277,284,
298,316
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Instance statistics:
   \verbatim
> cat VanDerWaerden_pd_2-3-26_635.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
318 53476 240198 0 240198 1 1
 length count
1 1
2 422
3 49547
13 12
14 24
26 3470
> cat VanDerWaerden_pd_2-3-26_643.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
322 54844 246504 0 246504 1 1
 length count
1 1
2 428
3 50813
13 12
14 24
26 3566
   \endverbatim
   </li>
   <li> SplittingViaOKsolver, using minisat-2.2.0:
    <ol>
     <li> n=644:
     \verbatim
> SplittingViaOKsolver -D60 VanDerWaerden_pd_2-3-26_644.cnf
> cd SplitViaOKsolver_D60VanDerWaerden_pd_2326_644cnf_2011-06-04-174059
> more Md5sum
f82f9777d8005a9c1aba79d73c6f03b1
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  60.00   65.00   69.00   69.85   74.00  139.00
> table(E$n)
   60    61    62    63    64    65    66    67    68    69    70    71    72
10066  3308  3366  3819  4322  4864  5439  5846  6347  6184  6393  6232  5765
   73    74    75    76    77    78    79    80    81    82    83    84    85
 5290  4733  4014  3412  2982  2679  2462  2327  2036  1694  1186   758   530
   86    87    88    89    90    91    92    93    94    96    97    99   100
  321   242   147   132    89    62    30    16     6     3     1     1     1
  108   139
    1     1
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         26
c initial_number_of_variables           322
c initial_number_of_clauses             55319
c initial_number_of_literal_occurrences 253357
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   107
c running_time(sec)                     100950.6
c number_of_nodes                       214221
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                14756
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        46
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_pd_2-3-26_644.cnf
c splitting_directory                   SplitViaOKsolver_D60VanDerWaerden_pd_2326_644cnf_2011-06-04-174059/Instances
c splitting_cases                       107107

> cd Instances
> OKP=~/SAT-Algorithmen/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
     \endverbatim
     </li>
     <li> n=635:
     \verbatim
> SplittingViaOKsolver -D30 VanDerWaerden_pd_2-3-26_635.cnf
 30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47  48
283 105  82  84  87  72  72  72  78  71  84  79  70  51  43  29  20   7   1
> SplittingViaOKsolver -D40 VanDerWaerden_pd_2-3-26_635.cnf
 40  41  42  43  44  45  46  47  48  49  50  51  52  53  54  55  56  57  58  59
603 322 345 369 414 481 480 466 435 400 367 304 226 171 113  93  52  56  50  30
 60  61  62  65  68  70  71
 13   4   4   1   1   1   1
> SplittingViaOKsolver -D50 VanDerWaerden_pd_2-3-26_635.cnf
  50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65
2700 1078  956 1115 1236 1465 1535 1474 1319 1246 1170 1146 1104 1191 1038 1029
  66   67   68   69   70   71   72   73   74   75   76   78   79   80   82   83
 810  578  424  224  111   56   40   16   11    7    2    1    1    1    1    4
  84   85   86   87   88   90
   4    2    1    4    3    1
c running_time(sec)                     21294.6
c number_of_nodes                       46207
c number_of_2-reductions                4882
c splitting_cases                       23104

> SplittingViaOKsolver -D60 VanDerWaerden_pd_2-3-26_635.cnf
> cd SplitViaOKsolver_D60SNVanDerWaerden_pd_2326_635cnf_2011-05-27-192636/
> more Md5sum
04555cc13f284e6025de4b76016121c8
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  60.00   63.00   67.00   67.95   71.00  137.00
> table(E$n)
  60   61   62   63   64   65   66   67   68   69   70   71   72   73   74   75
8921 3586 3287 3699 4337 5182 5776 5920 5578 4519 3551 2676 2106 1684 1488 1414
  76   77   78   79   80   81   82   83   84   85   86   87   88   89   90   91
1428 1582 1635 1558 1369 1133  877  526  328  182   74   48   37   25   14   13
  92   93   94   95   96   98   99  100  101  102  103  104  105  106  107  108
  10    8    2    3    5    3    4    6    2    6    3    4    4    6    3    5
 109  110  118  123  124  125  126  131  137
   4    2    1    1    1    1    2    1    1
> more Result
c running_time(sec)                     64585.8
c number_of_nodes                       149285
c number_of_2-reductions                24779
c max_tree_depth                        37
c splitting_cases                       74641

> cd Instances
> OKP=~/SAT-Algorithmen/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
74641: 40.58h, sum-cfs=1.292356e+09, mean-t=1.957s, mean-cfs=17314
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdWk1k2 3 26 gsat-tabu 100 4000000":
   \verbatim
Break point 1: 634
Break point 2: 643
   \endverbatim
   All solutions found within the first 30 runs. </li>
   <li> Further checking unsatisfiability for n=644:
   \verbatim
ubcsat-okl -alg gsat-tabu -runs 100 -cutoff 10000000 -i Exp_PdVanderWaerden_2-3-26_gsat-tabu-100-4000000_2010-12-01-003724/VanDerWaerden_pd_2-3-26_644.cnf
   \endverbatim
   reaches constantly min=1. </li>
  </ul>

*/
