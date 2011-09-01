// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/25.hpp
  \brief On investigations into vdw_2(3,25)


  \todo vanderwaerden_2(3,25) > 655
  <ul>
   <li> The (weak) conjecture is vanderwaerden_2(3,25) = 656. </li>
   <li> Search starting with n=590:
   \verbatim
OKplatform> RunVdW3k 25 590 gsat-tabu 200 50000000
   \endverbatim
   </li>
   <li> Perhaps also here the cutoff-value is too large:
   \verbatim
OKplatform> RunVdW3k 25 609 gsat-tabu 1000 10000000
   \endverbatim
   </li>
   <li> n=609 found satisfiable with certificate
   \verbatim
18,19,31,49,53,55,68,90,93,102,
106,123,129,132,138,143,160,164,175,176,
187,201,206,224,234,243,249,253,254,277,
278,286,287,290,315,317,324,328,335,351,
364,382,386,388,389,401,409,426,435,439,
456,462,465,471,476,493,497,508,509,520,
534,539,557,567,576,586,587
   \endverbatim
   </li>
   <li> n=622 found satisfiable with certificate
   \verbatim
7,12,18,27,37,60,74,82,85,86,
97,101,123,129,132,138,155,159,166,168,
185,196,205,206,208,212,229,230,243,259,
266,270,277,279,304,308,316,317,340,341,
345,351,360,370,393,407,415,418,419,430,
434,456,462,465,471,488,492,499,501,504,
518,539,541,545,562,575,576,599,603,610,
612
   \endverbatim
   </li>
   <li> Restarting the search, now using the solution found for n=622
   as initial assignment:
   \verbatim
OKplatform> cat Exp_VanderWaerden_2-3-25_2010-10-12-161927_609/VanDerWaerden_2-3-25_622.cnf_OUT | extract_solution_ubcsat > Solution_n622
OKplatform> RunVdW3k 25 623 rots 1000 5000000 Solution_n622
   \endverbatim
   </li>
   <li> n=654 found satisfiable with certificate
   \verbatim
1,16,19,39,48,56,76,85,90,92,
98,116,127,149,150,153,155,166,174,187,
192,196,203,211,223,227,241,246,264,266,
270,277,278,285,303,312,320,334,349,352,
357,371,372,381,389,409,418,423,425,433,
449,460,470,482,483,486,488,499,505,520,
525,529,536,544,556,560,574,579,597,599,
603,610,611,636,645
   \endverbatim
   A variation is
   \verbatim
16,19,38,39,48,56,76,85,90,92,
98,116,127,149,150,153,155,166,174,187,
192,196,203,211,223,227,241,246,264,266,
270,277,278,285,303,312,320,334,349,352,
357,371,372,381,389,409,418,423,425,431,
433,449,460,470,482,483,486,488,499,520,
525,529,536,544,556,560,574,579,597,599,
603,610,611,636
   \endverbatim
   </li>
   <li> n=655
    <ol>
     <li> Found satisfiable with certificate
     \verbatim
17,20,40,49,57,77,86,91,93,99,
117,128,150,151,154,156,167,175,188,193,
197,204,212,224,228,242,247,265,267,271,
278,279,286,304,313,321,335,350,353,358,
372,373,382,390,410,419,424,426,434,450,
461,471,483,484,487,489,500,506,521,526,
530,537,545,557,561,575,580,598,600,604,
611,612,637,646
     \endverbatim
     This is essentially different from the above solution. </li>
     <li> 1000 runs with rots and cutoff=5*10^6 don't find a solution:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14
216 309 149 112  17  21  32  43  40  34  18   6   1   2
1000
     \endverbatim
     </li>
     <li> Another 1000 runs with rots and cutoff=5*10^6 don't find a solution:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12
232 292 163 112  31  14  48  39  36  19   6   8
1000
     \endverbatim
     </li>
     <li> 500 runs with rots and cutoff=10^7 don't find a solution:
     \verbatim
  1   2   3   4   5   6   7   8  10  11
191 183  62  35   5   5   6  10   2   1
500
     \endverbatim
     </li>
     <li> So n=655 seems to be quite difficult, and so unsatisfiability for
     n=656 is hard to judge. </li>
     <li> Rots with cutoff=20*10^7 found in run 3 a solution for n=654
     (seed=3127361270, osteps=7949973), and this solution worked also for
     n=655:
     \verbatim
2,17,20,40,49,57,77,86,91,93,
99,117,128,150,151,154,156,167,175,188,
193,197,204,212,224,228,242,247,265,267,
271,278,279,286,304,313,321,335,350,353,
358,372,373,382,390,410,419,424,426,434,
450,461,471,483,484,487,489,500,506,521,
526,530,537,545,557,561,575,580,598,600,
604,611,612,637,646
     \endverbatim
     </li>
     <li> A variation was found with rots, cutoff=10^7, as the solution for
     n=654:
     \verbatim
2,17,20,25,39,40,49,57,77,86,
91,93,99,117,128,150,151,154,156,167,
175,188,193,197,204,212,224,228,242,247,
265,267,271,278,279,286,304,313,321,335,
350,353,358,372,373,382,390,410,419,424,
426,434,450,461,471,483,484,487,489,500,
506,521,526,530,537,545,557,561,575,580,
598,600,604,611,612,637
     \endverbatim
     </li>
     <li> Another solution was found with rots, cutoff=2*10^7, again as the
     solution for n=654:
     \verbatim
2,17,20,40,49,57,77,86,91,93,
99,117,128,150,151,154,156,167,175,188,
193,197,204,212,224,228,242,247,265,267,
271,278,279,286,304,313,321,335,350,353,
358,372,373,382,390,410,419,424,426,434,
450,461,471,483,484,487,489,500,506,521,
526,530,537,545,557,561,575,580,598,600,
604,611,612,637,646
     \endverbatim
     This has a larger distance to the above solution (analysis is needed!).
     </li>
     <li> Another variation is
     \verbatim
17,40,49,57,77,86,91,93,99,117,
128,150,151,154,156,167,175,188,193,197,
204,212,224,228,242,247,265,267,271,278,
279,286,304,313,321,335,350,353,358,372,
373,382,390,410,419,424,426,434,450,461,
471,483,484,487,489,500,521,526,530,537,
545,557,561,575,580,598,600,604,611,612,
637
     \endverbatim
     </li>
     <li> Are there essentially different solutions? </li>
    </ol>
   <li>
   <li> n=656
    <ol>
     <li> Cutoff=5*10^6, rots:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12
224 321 162  99  26  24  34  37  30  20  13  10
1000
     \endverbatim
     </li>
     <li> Cutoff=10^7 found in 500 runs no solution. Another 500 runs:
     \verbatim
  1   2   3   4   5   6   7   8   9
187 194  56  39   4   6   7   3   4
500
     \endverbatim
     </li>
     <li> Cutoff=2*10^7, rots:
     \verbatim
  1   2   3   4
333 146  20   1
500
  1   2   3   4
329 147  22   2
500
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo vdw_2^pd(3,25) = (586,607)
  <ul>
   <li> pdvanderwaerden([3,25])[2] = 607 =(conj)
   vanderwaerden([3,25])-49. </li>
   <li> pdvanderwaerden([3,25])[1] = 586. </li>
   <li> Statistics:
   n=587:
   \verbatim
  n        c        l
294    45779   204903
 length   count
      2     390
      3   42281
     13      35
     25    3073
   \endverbatim
   n=608:
   \verbatim
  n        c        l
304    49427   225828
 length   count
      2     101
      3   45790
     13      12
     25    3524
   \endverbatim
   </li>
   <li> Certificates:
    <ol>
     <li> n=585:
     \verbatim
20,30,49,50,72,88,89,91,102,117,
131,137,147,149,150,159,166,175,176,178,
188,194,208,223,234,236,237,253,275,276,
281,291
     \endverbatim
     </li>
     <li> n=586:
     \verbatim
16,20,21,45,66,67,83,96,103,104,
107,128,129,132,134,159,166,167,170,182,
196,207,217,241,245,246,253,259,284
     \endverbatim
     </li>
     <li> n=606:
     \verbatim
25,30,41,59,68,73,97,99,112,118,
131,141,142,146,155,162,184,191,200,204,
205,215,228,234,247,249,273,278,286,287,
291,302
     \endverbatim
     </li>
    </ol>
   </li>
   <li> precosat570:
    <ol>
     <li> n=607: unsat, 1136821s (881484924 conflicts). </li>
     <li> n=587: unsat, 1157892s (900717903 conflicts). </li>
    </ol>
   </li>
   <li> minisat-2.2.0:
    <ol>
     <li> n=608: unsat, 30days (on csltok, with unstable clock frequency;
     6708604472 conflicts, 8254745835 decisions, 256909484007 propagations,
     234914164662 conflict literals). </li>
     <li> SplittingViaOKsolver, n=587:
     \verbatim
> SplittingViaOKsolver -D45 VanDerWaerden_pd_2-3-25_587.cnf
> cd SplitViaOKsolver_D45VanDerWaerden_pd_2325_587cnf_2011-08-28-183800
> more Md5sum
84626bb6d8b4681f0c455c4209b82281
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  45.00   47.00   52.00   52.37   57.00  101.00
> table(E$n)
  45   46   47   48   49   50   51   52   53   54   55   56   57   58   59   60
1456  529  452  426  430  482  531  591  582  475  440  422  340  338  325  344
  61   62   63   64   65   66   67   68   69   70   73   86   89   97  101
 305  259  165  109   67   27   13    4    1    1    2    1    1    1    1
> summary(E$d)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   5.00   15.00   17.00   17.22   20.00   29.00
> table(E$d)
   5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
   1    5   17   34   68  136  235  366  505  674  799  886 1024  977  954  813
  21   22   23   24   25   26   27   28   29
 631  447  286  152   63   25   15    5    2
> more Result
c running_time(sec)                     3624.9 (csltok)
c number_of_nodes                       18239
c number_of_quasi_single_nodes          0
c number_of_2-reductions                2202
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        29
c file_name                             VanDerWaerden_pd_2-3-25_587.cnf
c splitting_directory                   SplitViaOKsolver_D45VanDerWaerden_pd_2325_587cnf_2011-08-28-183800/Instances
c splitting_cases                       9120

# with older form of SplittingViaOKsolver:
> ProcessSplitViaOKsolver SplitViaOKsolver_D45VanDerWaerden_pd_2325_587cnf_2011-08-22-004743
323m06s
> E=read_processsplit_minisat()
9120: 4.982h, sum-cfs=2.625604e+08, mean-t=1.966s, mean-cfs=28790
$t:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
 0.09298  0.64790  1.16400  1.96600  2.24700 73.85000
sd= 2.608615
      95%       96%       97%       98%       99%      100%
 6.127070  6.811328  8.254036 10.132820 13.283080 73.854800
sum= 17933.74
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
    231    9408   17680   28790   33540  950400
sd= 37014.56
      95%       96%       97%       98%       99%      100%
 89649.95  99894.72 120153.79 145636.56 189240.46 950354.00
sum= 262560423
$t ~ $cfs:
               Estimate  Std. Error  t value  Pr(>|t|)
(Intercept) -5.8116e-02  2.2823e-03  -25.464 < 2.2e-16 ***
E$cfs        7.0322e-05  4.8672e-08 1444.824 < 2.2e-16 ***
R-squared: 0.9957

# running again, with the newer version, and using the iCNF form now
# (this is only for checking of the newly established tool:
# ProcessSplitViaOKsolver is more powerful, since it records more statistics)
> ExtractiCNF SplitViaOKsolver_D45VanDerWaerden_pd_2325_587cnf_2011-08-28-183800
> ProcessiCNF SplitViaOKsolver_D45VanDerWaerden_pd_2325_587cnf_2011-08-28-183800/VanDerWaerden_pd_2-3-25_587.icnf
353m47s (csltok; partially lower frequency)
> E=read_processsplit_minisat()
9120: 5.466h, sum-cfs=2.625604e+08, mean-t=2.157s, mean-cfs=28790
$t:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
 0.09298  0.63870  1.17300  2.15700  2.37100 70.98000
sd= 3.127154
     95%      96%      97%      98%      99%     100%
 7.01183  7.93559  9.45001 11.85030 15.64135 70.97920
sum= 19676
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
    231    9408   17680   28790   33540  950400
sd= 37014.56
      95%       96%       97%       98%       99%      100%
 89649.95  99894.72 120153.79 145636.56 189240.46 950354.00
sum= 262560423
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -1.7895e-01  1.1533e-02 -15.517 < 2.2e-16 ***
E$cfs        8.1155e-05  2.4594e-07 329.971 < 2.2e-16 ***
R-squared: 0.9227

# now with decisions only:
> ExtractDecisionsiCNF SplitViaOKsolver_D45VanDerWaerden_pd_2325_587cnf_2011-08-28-183800/
> ProcessiCNF SplitViaOKsolver_D45VanDerWaerden_pd_2325_587cnf_2011-08-28-183800/VanDerWaerden_pd_2-3-25_587_decisions.icnf
321m:00s (csltok; likely stable frequency of 2.4GHz throughout)
> E=read_processsplit_minisat()
9120: 4.934h, sum-cfs=2.622482e+08, mean-t=1.948s, mean-cfs=28755
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
 0.2610  0.7049  1.1850  1.9480  2.2110 58.3800
sd= 2.432164
      95%       96%       97%       98%       99%      100%
 5.830160  6.625230  7.841906  9.698344 12.513770 58.376100
sum= 17761.73
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
    256    9400   17660   28760   33580  793700
sd= 36453.76
      95%       96%       97%       98%       99%      100%
 89109.15 100048.60 119830.64 145343.84 186240.10 793735.00
sum= 262248222
$t ~ $cfs:
              Estimate Std. Error  t value  Pr(>|t|)
(Intercept) 3.3275e-02 2.1572e-03   15.425 < 2.2e-16 ***
E$cfs       6.6571e-05 4.6462e-08 1432.810 < 2.2e-16 ***
R-squared: 0.9956
     \endverbatim
     So with just the decision-variables given, the problems appear to be
     slightly easier? Or perhaps they are for minisat just the same, but also
     that is somewhat surprising. </li>
     <li> One needs to evaluate this data together with the data for the full
     partial assignments. </li>
     <li> Remark: The application of partial assignments does not update pn,
     pc, and minisat warns about this, but using ExtendedToStrictDimacs
     together with ManipParam shows that on the updated files (where variables
     are numbered consecutively, and the parameter-values are precise) minisat
     behaves identical. </li>
     <li> Perhaps it is just the case that most of the inferred assignments are
     obtained by unit-clause propagation. So a more complete analysis should
     show per splitting three values: number of decision variables, number of
     additional r_1-reductions, number of additional r_2-reductions. </li>
     <li> It's likely easiest to just process the decision-only-assignments by
     UCP, to get the number of additional r_1-reductions. </li>
     <li> For SplittingViaOKsolver, n=608, let's go first for D=30:
     \verbatim
> SplittingViaOKsolver -D30 VanDerWaerden_pd_2-3-25_608.cnf
> cd SplitViaOKsolver_D30SNVanDerWaerden_pd_2325_608cnf_2011-05-19-182858/
> more Md5sum
b69e55a43063ee062364ffc827aa7b08
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  30.00   32.00   36.00   36.53   41.00   47.00
> table(E$n)
 30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47
355 107 116 114 113  99  97  82  97  86 107 115 125 119 104  63  25   4
> more Result
c running_time(sec)                     1341.6
c number_of_nodes                       3855
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                69
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        27
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_pd_2-3-25_608.cnf
c splitting_directory                   SplitViaOKsolver_D30SNVanDerWaerden_pd_2325_608cnf_2011-05-19-182858/Instances
c splitting_cases                       1928
     \endverbatim
     This is very small!
     </li>
     <li> Then let's go for n=35, 40, 45:
     \verbatim
> SplittingViaOKsolver -D35 VanDerWaerden_pd_2-3-25_608.cnf
 35  36  37  38  39  40  41  42  43  44  45  46  47  48  49  50  51  52  54
755 131  97 103  94 138 207 232 302 357 396 446 449 410 265 160  50   8   3
c running_time(sec)                     3075.2
c number_of_nodes                       9205
c number_of_2-reductions                201
c max_tree_depth                        28
c splitting_cases                       4603

> SplittingViaOKsolver -D40 VanDerWaerden_pd_2-3-25_608.cnf
 40  41  42  43  44  45  46  47  48  49  50  51  52  53  54  55  56  57  58  59
713 323 268 336 406 482 569 670 735 779 763 668 605 424 254 176  83  42  19  19
 60  61
 10   3
c running_time(sec)                     6277.1
c number_of_nodes                       16693
c number_of_2-reductions                822
c max_tree_depth                        33
c splitting_cases                       8347

> SplittingViaOKsolver -D45 VanDerWaerden_pd_2-3-25_608.cnf
> cd SplitViaOKsolver_D45VanDerWaerden_pd_2325_608cnf_2011-08-28-185308
> more Md5sum
26c2f81d53b5348005ec7e0d41c6b341
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  45.00   48.00   52.00   52.24   56.00   66.00
> table(E$n)
  45   46   47   48   49   50   51   52   53   54   55   56   57   58   59   60
1915  628  723  773  830  834  794  825  780  700  724  674  635  541  518  441
  61   62   63   64   65   66
 434  318  217  110   41    7
> summary(E$d)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   5.00   16.00   19.00   19.44   23.00   36.00
> table(E$d)
   5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
   1    5   14   37   75  131  239  375  495  662  756  906 1025 1147 1173 1104
  21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36
 989  865  794  705  567  412  305  244  150   95   83   57   35   13    1    2
> more Result
c running_time(sec)                     11495.7 (cs-wsok)
c number_of_nodes                       26923
c number_of_2-reductions                1271
c max_tree_depth                        36
c splitting_cases                       13462

# old run:
> cd Instances
> OKP=~/SAT-Algorithmen/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done
real    2987m55.420s
user    2972m38.595s
sys     11m37.584s
# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
13462: 48.77h, sum-cfs=1.428662e+09, mean-t=13.043s, mean-cfs=106126

# running it again (on cs-wsok):
# (this is only for checking of the newly established tool:
# ProcessSplitViaOKsolver is more powerful, since it records more statistics)
> ExtractiCNF SplitViaOKsolver_D45VanDerWaerden_pd_2325_608cnf_2011-08-28-185308/
> ProcessiCNF SplitViaOKsolver_D45VanDerWaerden_pd_2325_608cnf_2011-08-28-185308/VanDerWaerden_pd_2-3-25_608.icnf
2652m:59s
> E=read_processsplit_minisat()
13462: 1.8d, sum-cfs=1.428662e+09, mean-t=11.553s, mean-cfs=106126
$t:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
   0.325    2.573    6.025   11.550   13.300 1673.000
sd= 29.15414
       95%        96%        97%        98%        99%       100%
  36.58810   41.32830   47.37997   57.32012   83.40890 1672.72000
sum= 155526.7
$cfs:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
     251    26330    59210   106100   125300 11470000
sd= 220711.3
       95%        96%        97%        98%        99%       100%
  329836.3   369242.1   415118.7   514569.9   721279.6 11467455.0
sum= 1428662285
$t ~ $cfs:
               Estimate  Std. Error  t value  Pr(>|t|)
(Intercept) -2.3875e+00  2.9328e-02  -81.408 < 2.2e-16 ***
E$cfs        1.3136e-04  1.1976e-07 1096.872 < 2.2e-16 ***
R-squared: 0.9889

# now decisions only:
> ExtractDecisionsiCNF SplitViaOKsolver_D45VanDerWaerden_pd_2325_608cnf_2011-08-28-185308/
> ProcessICNF SplitViaOKsolver_D45VanDerWaerden_pd_2325_608cnf_2011-08-28-185308/VanDerWaerden_pd_2-3-25_608_decisions.icnf
2646m28s
13462: 1.794d, sum-cfs=1.419040e+09, mean-t=11.511s, mean-cfs=105411
$t:
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
   0.5219    2.6970    6.0990   11.5100   13.2000 1400.0000
sd= 24.74269
       95%        96%        97%        98%        99%       100%
  36.89415   40.88368   46.61349   57.31364   82.35891 1399.73000
sum= 154960.7
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
    251   26300   59200  105400  124800 9858000
sd= 196146.8
      95%       96%       97%       98%       99%      100%
 333137.6  364875.6  413895.5  511745.3  715237.6 9858492.0
sum= 1419040180
$t ~ $cfs:
               Estimate  Std. Error  t value  Pr(>|t|)
(Intercept) -1.7235e+00  2.3437e-02  -73.538 < 2.2e-16 ***
E$cfs        1.2555e-04  1.0525e-07 1192.853 < 2.2e-16 ***
R-squared: 0.9906
     \endverbatim
     Likely one should go higher (say, n=50 or n=55); but already here a big
     saving. </li>
     <li> Again we see that with decisions-only it performs a tiny bit better
     (and again apparently on worst-case sub-instances); one needs to see the
     structure of the partial assignments for the sub-instances. </li>
     <li> Now using n=55:
     \verbatim
> SplittingViaOKsolver -D55 VanDerWaerden_pd_2-3-25_608.cnf

     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 25 26 618 gsat-tabu 100 8000000" (old version): all
   1<=n<=586 sat, then
   \verbatim
587 0
588 1
589 0
590 1
591 0
592 1
593 0
594 1
595 0
596 1
597 0
598 1
599 0
600 1
601 0
602 1
603 0
604 1
605 0
606 1
   \endverbatim
   and all n>=607 "unsat". All solutions found in the first run.
   </li>
  </ul>

*/
