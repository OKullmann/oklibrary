// Oliver Kullmann, 16.7.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/SchurProblems/plans/Schur5/LocalSearch/general.hpp
  \brief On computing lower bounds for schur(5) and variations via local search

  The main challenge is to find a combination of translation and solver which
  enables to find all known solutions (and possibly more) via SAT.

  Yet we can only find palindromic solutions for n=160, while [Exoo 1994]
  reports to have found 10000 different general solutions for n=160 (but
  only 4 palindromic ones).


  \todo Direct encoding
  <ul>
   <li> Considering Schur_5_159.cnf; the following data is for the old
   version, using the full hypergraph, not, as now, the subsumption-free form;
   this shouldn't make a big difference. The instance-statistics for the new
   form is:
   \verbatim
> cat Schur_5_159.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    795   33084    795    795   33084    97585     NA     NA   33084    97585   798
 length   count
      2    1985
      3   30940
      5     159
   \endverbatim
   </li>
   <li> From the ubcsat-1.0.0 suite saps seems clearly perform best, and
   a local minimum of one falsified clause is easily reached already with
   cutoff = 10000. </li>
   <li> But finding a solution is difficult: 10 runs with cutoff=10*10^6
   all just yield a minimum=1. </li>
   <li> The worst-performing algorithm (w.r.t. mean minimum) seemed to be
   hsat, which now with cutoff=10*10^6 still doesn't get close to 1. </li>
   <li> Using symmetry-breaking and then the minisat2-preprocessor seems
   to make the problem more difficult. </li>
   <li> Finally by
   \verbatim
> ubcsat-okl -alg saps -runs 10000 -cutoff 100000 -i Schur_5_159.cnf
Clauses = 33349
Variables = 795
TotalLiterals = 98380
FlipsPerSecond = 116174
BestStep_Mean = 16171.864100
Steps_Mean = 99993.137900
Steps_Max = 100000.000000
PercentSuccess = 0.01
BestSolution_Mean = 1.445400
BestSolution_Median = 1.000000
BestSolution_Min = 0.000000
BestSolution_Max = 3.000000
   \endverbatim
   we found (exactly) one solution (seed=830151296, msteps=31379). However
   finding a solution is difficult:
   \verbatim
> ubcsat-okl -alg rsaps -runs 1000 -cutoff 10000000 -i Schur_5_159.cnf | tee Schur_5_159.cnf_OUT
Clauses = 33084
Variables = 795
TotalLiterals = 97585
FlipsPerSecond = 205225
BestStep_Mean = 386955.108
Steps_Mean = 10000000
Steps_Max = 10000000
PercentSuccess = 0.00
BestSolution_Mean = 1
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 1

> ubcsat-okl -alg vw1 -runs 10000 -cutoff 1000000 -i Schur_5_159.cnf | tee Schur_5_159.cnf_OUT
Clauses = 33084
Variables = 795
TotalLiterals = 97585
FlipsPerSecond = 472105
BestStep_Mean = 77629.9933
Steps_Mean = 1000000
Steps_Max = 1000000
PercentSuccess = 0.00
BestSolution_Mean = 1.0375
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 3
> E=read_ubcsat("Schur_5_159.cnf_OUT",nrows=10000)
   1    2    3
9627  371    2
10000

> ubcsat-okl -alg vw1 -runs 100000 -cutoff 100000 -i Schur_5_159.cnf | tee Schur_5_159.cnf_OUT
Clauses = 33084
Variables = 795
TotalLiterals = 97585
FlipsPerSecond = 462991
BestStep_Mean = 29011.16233
Steps_Mean = 100000
Steps_Max = 100000
PercentSuccess = 0.00
BestSolution_Mean = 1.24328
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 3
> E=read_ubcsat("Schur_5_159.cnf_OUT",nrows=100000)
    1     2     3
76007 23658   335
100000

> ubcsat-okl -alg saps -runs 100000 -cutoff 100000 -i Schur_5_159.cnf | tee Schur_5_159.cnf_OUT
FlipsPerSecond = 281467
BestStep_Mean = 15666.23514
Steps_Mean = 100000
Steps_Max = 100000
PercentSuccess = 0.00
BestSolution_Mean = 1.42856
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 3
> E=read_ubcsat("Schur_5_159.cnf_OUT",nrows=100000)
    1     2     3
58736 39672  1592
100000

> ubcsat-okl -alg vw2 -v 2005 -runs 100000 -cutoff 100000 -i Schur_5_159.cnf | tee Schur_5_159.cnf_OUT
FlipsPerSecond = 468279
BestStep_Mean = 34740.92829
Steps_Mean = 100000
Steps_Max = 100000
PercentSuccess = 0.00
BestSolution_Mean = 1.17683
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 8
> E=read_ubcsat("Schur_5_159.cnf_OUT",nrows=100000)
    1     2     3     4     5     6     7     8
84100 14295  1464   118    15     4     2     2
100000
> ubcsat-okl -alg vw2 -v 2005 -runs 200000 -cutoff 200000 -i Schur_5_159.cnf | tee Schur_5_159.cnf_OUT
FlipsPerSecond = 472790
BestStep_Mean = 48188.0324
Steps_Mean = 200000
Steps_Max = 200000
PercentSuccess = 0.00
BestSolution_Mean = 1.05146
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 6
> E=read_ubcsat("Schur_5_159.cnf_OUT",nrows=200000)
     1      2      3      4      5      6
190032   9699    217     50      1      1
200000
   \endverbatim
   </li>
   <li> For n=160 we get
   \verbatim
> ubcsat-okl -alg saps -runs 40000 -cutoff 200000 -i Schur_5_160.cnf | tee Schur_5_160.out
Clauses = 33760
Variables = 800
TotalLiterals = 99600
FlipsPerSecond = 117567
BestStep_Mean = 23914.642400
Steps_Mean = 200000.000000
Steps_Max = 200000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.351275
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 3.000000

# with subsumption-elimination and higher cut-off:
> ubcsat-okl -alg saps -runs 1000 -cutoff 10000000 -i Schur_5_160.cnf | tee Schur_5_160.cnf_OUT
Clauses = 33495
Variables = 800
TotalLiterals = 98805
FlipsPerSecond = 266933
BestStep_Mean = 474910.772
Steps_Mean = 10000000
Steps_Max = 10000000
PercentSuccess = 0.00
BestSolution_Mean = 1
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 1
   \endverbatim
   which is hardly a difference (recall also this instance is satisfiable).
   </li>
   <li> We need to determine the best Ubcsat-solver:
   \verbatim
> E = run_ubcsat("Schur_5_159.cnf", runs=100, cutoff=1000000)
> eval_ubcsat_dataframe(E,FALSE)
1. vw1:
 1  2
97  3
fps: 475873
2. ddfw:
 1  2
92  8
fps: 63800
3. rsaps:
 1  2
88 12
fps: 232423
4. saps:
 1  2
87 13
fps: 295430
5. paws:
 1  2
76 24
fps: 467552
6. wsat:
 1  2
74 26
fps: 449479
7. sapsnr:
 1  2
73 27

# re-run on cs-oksvr for new algorithms:
1. vw25:
 1  2
99  1
fps: 530814
2. vw1:
 1  2
96  4
fps: 522657
3. rsaps:
 1  2
95  5
fps: 239527
4. saps:
 1  2
90 10
fps: 326787
5. ddfw:
 1  2
90 10
fps: 72118
6. wsat:
 1  2
82 18
fps: 499700
7. g2wsat:
 1  2
82 18
fps: 403714
8. sapsnr:
 1  2
76 24
fps: 352249
9. paws:
 1  2
73 27
fps: 513690
   \endverbatim
   So vw25 seems overall best (for the various encodings).
   </li>
  </ul>


  \todo Direct encoding with forbidden elements
  <ul>
   <li> Considering first n=159 and k=40 (forbidding the first 40 elements
   from the first partition); instance-statistics:
   \verbatim
> cat Schur_rm_5_159-40.cnf | UnitClausePropagation-O3-DNDEBUG > Schur_rmUCP_5_159-40.cnf
> cat Schur_rmUCP_5_159-40.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG     pn     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    795   28204    755    795   28204    83105     NA     NA   28204    83105   804
 length   count
      2    1785
      3   26260
      4      40
      5     119
   \endverbatim
   </li>
   <li> Determining the best Ubcsat-solver:
   \verbatim
> E = run_ubcsat("Schur_rm_5_159-40.cnf", runs=100, cutoff=1000000)
> eval_ubcsat_dataframe(E,FALSE)
1. vw25:
  1
100
fps: 473104
2. ddfw:
 1  2
99  1
fps: 59863
3. rsaps:
 1  2
97  3
fps: 222435
4. saps:
 1  2
90 10
fps: 271245
5. vw1:
 1  2
87 13
fps: 450450
6. wsat:
 1  2
85 15
fps: 443518
7. sapsnr:
 1  2
80 20
fps: 278886
8. g2wsat:
 1  2
74 26
fps: 361965
   \endverbatim
   Looks somewhat better than without the restriction. Again vw25 best.
   </li>
  </ul>


  \todo Direct encoding with full symmetry breaking (ordinary and palindromic)
  <ul>
   <li> Considering Schur_fullsb_5_159.cnf; the instance-statistics is:
   \verbatim
> cat Schur_fullsb_5_159.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    795   33104    795    795   33104    97605     NA     NA   33104    97605   799
 length   count
      1      20
      2    1985
      3   30940
      5     159
> cat Schur_fullsb_5_159.cnf | UnitClausePropagation-O3-DNDEBUG > Schur_fullsbUCP_5_159.cnf
> cat Schur_fullsbUCP_5_159.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    795   29382    765    795   29382    85858     NA     NA   29382    85858   804
 length   count
      2    2591
      3   26637
      4       5
      5     149
   \endverbatim
   </li>
   <li> Determining the best Ubcsat-solver:
   \verbatim
> E = run_ubcsat("Schur_fullsbUCP_5_159.cnf", runs=100, cutoff=500000)
> eval_ubcsat_dataframe(E,FALSE)
1. vw25:
 1  2  3
78 21  1
fps: 533333
2. ddfw:
 1  2  3
74 22  4
fps: 66606
3. vw1:
 1  2
72 28
fps: 542299
4. wsat:
 1  2  3
64 35  1
fps: 531745
5. paws:
 1  2
56 44
fps: 451916
6. anovpp:
 1  2  3
46 48  6
fps: 434103
7. g2wsat:
 1  2  3
40 59  1
fps: 359971
8. rsaps:
 1  2  3
39 58  3
fps: 238265
   \endverbatim
   </li>
   <li> Searching for solutions:
   \verbatim
> ubcsat-okl -alg vw2 -v 2005 -runs 200000 -cutoff 200000 -i Schur_fullsbUCP_5_159.cnf | tee Schur_fullsbUCP_5_159.cnf_OUT
Clauses = 29382
Variables = 795
TotalLiterals = 85858
FlipsPerSecond = 528071
BestStep_Mean = 62653.66158
Steps_Mean = 200000
Steps_Max = 200000
PercentSuccess = 0.00
BestSolution_Mean = 1.396075
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 5
> E=read_ubcsat("Schur_fullsbUCP_5_159.cnf_OUT",nrows=200000)
     1      2      3      4      5
130503  60371   8572    516     38
200000
   \endverbatim
   The statistics are worse than without symmetry-breaking.
   </li>
   <li> Palindromic problems:
   \verbatim
> ubcsat-okl -alg vw2 -v 2005 -runs 200000 -cutoff 200000 -i Schur_pd_fullsbUCP_5_159.cnf | tee Schur_pd_fullsbUCP_5_159.cnf_OUT
# aborted, since solutions were found:
> E=read_ubcsat("Schur_pd_fullsbUCP_5_159.cnf_OUT")
    0     1     2
    2 65741   127
65870
> E[E$min==0,]
      sat min osteps msteps       seed
30506   1   0 135626 135626 1214594171
62632   1   0 135626 135626 1214594171

> ubcsat-okl -alg vw2 -v 2005 -runs 1 -cutoff 135626 -i Schur_pd_fullsbUCP_5_159.cnf -seed 1214594171 -solve > Solution_1
> OKsolver_2002-O3-DNDEBUG -SF -S="UCP_1" -D0 Schur_pd_fullsb_5_159.cnf

# editing of Solution_1, UCP_1 to transform them into CNF

> oklib --maxima
oklib_load_all();
pa1 : first(read_fcl_f("Solution_1")[2]);
pa2 : first(read_fcl_f("UCP_1")[2]);
pa : compo_pass(pa1,pa2);
pa : subset(pa, lambda([x], is(x>0)));
I : invstandardise_pd_schur_aloamo(5,159);
P : extract_partition(map(I,pa));
    [{1,8,10,17,19,26,31,33,40,42,46,49,51,55},
     {2,3,7,11,12,21,29,30,38,39,47,48,53,57,62,63,71,72,80},
     {4,5,13,27,28,36,44,50,52,59,61,67,68,69,70,76},
     {9,14,16,20,22,24,35,37,41,43,54,56,58,60,64,66,77,79},
     {6,15,18,23,25,32,34,45,65,73,74,75,78}]
FP : uncompresss_schurpalindromic_subsets(159,P);
certificate_pdschurfsb_p(5,159,FP);
  true
certificate_pdwschur_p(5,159,FP);
  true
certificate_pdwschurfsb_p(5,159,FP);
  false
   \endverbatim
   Remarkable that the same seed was used twice, but that happens.
   </li>
   <li> n=158, palindromic case with full symmetry-breaking:
   \verbatim
> ubcsat-okl -alg vw2 -v 2005 -runs 400000 -cutoff 200000 -i Schur_pd_fullsbUCP_5_158.cnf | tee Schur_pd_fullsbUCP_5_158.cnf_OUT
> E=read_ubcsat("Schur_pd_fullsbUCP_5_158.cnf_OUT",nrows=400000)
     1      2
399938     62
400000
   \endverbatim
   Either it's unsatisfiable, or hard to satisfy. </li>
   <li> n=160, palindromic case with full symmetry-breaking:
   \verbatim
> ubcsat-okl -alg vw2 -v 2005 -runs 200000 -cutoff 200000 -i Schur_pd_fullsbUCP_5_160.cnf | tee Schur_pd_fullsbUCP_5_160.cnf_OUT
# aborted after finding a solution
> E=read_ubcsat("Schur_pd_fullsbUCP_5_160.cnf_OUT")
   0    1    2
   1 4901    3
4905
> E[E$min==0,]
     sat min osteps msteps      seed
3220   1   0  81502  81502 146562509

> ubcsat-okl -alg vw2 -v 2005 -runs 1 -cutoff 81502 -i Schur_pd_fullsbUCP_5_160.cnf -seed 146562509 -solve > Solution_1
> OKsolver_2002-O3-DNDEBUG -SF -S="UCP_1" -D0 Schur_pd_fullsb_5_160.cnf

# editing of Solution_1, UCP_1 to transform them into CNF
> oklib --maxima
oklib_load_all();
pa1 : first(read_fcl_f("Solution_1")[2]);
pa2 : first(read_fcl_f("UCP_1")[2]);
pa : compo_pass(pa1,pa2);
pa : subset(pa, lambda([x], is(x>0)));
I : invstandardise_pd_schur_aloamo(5,160);
P : extract_partition(map(I,pa));
  [{1,6,15,19,26,40,49,54,62,70,74,79},
   {2,7,8,12,22,27,32,36,42,46,52,55,56,61,65,66,71,75,76,80},
   {4,5,20,21,28,34,35,47,50,53,59,60,72},
   {13,14,16,18,33,38,39,41,48,58,67,68,69,73},
   {3,9,10,11,17,23,24,25,29,30,31,37,43,44,45,51,57,63,64,77,78}]
FP : uncompresss_schurpalindromic_subsets(160,P);
certificate_pdschurfsb_p(5,160,FP);
  true
certificate_pdwschur_p(5,160,FP);
  true
certificate_pdwschurfsb_p(5,160,FP);
  false
   \endverbatim
   </li
  </ul>


  \todo Logarithmic translation
  <ul>
   <li> Creation by output_schur_logarithmic_stdname(5,n). </li>
   <li> Instance-statistics:
   \verbatim
> cat Schur_L_5_159.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    477   31812    477    477   31812   282261     NA     NA   31812   282261   639
 length   count
      3     477
      6     395
      9   30940
   \endverbatim
   </li>
   <li> Determining the best Ubcsat-solver:
   \verbatim
> E = run_ubcsat("Schur_L_5_159.cnf", runs=100, cutoff=1000000)
> eval_ubcsat_dataframe(E,FALSE)
1. rsaps:
  1
100
fps: 102642
2. vw25:
 1  2  3
79 20  1
fps: 147039
3. saps:
 1  2
71 29
fps: 61302
4. sapsnr:
 1  2
66 34
fps: 118170
   \endverbatim
   </li>
   <li> Searching for solutions:
   \verbatim
> ubcsat-okl -alg rsaps -runs 100000 -cutoff 100000 -i Schur_L_5_159.cnf | tee Schur_L_5_159.cnf_OUT
Clauses = 31812
Variables = 477
TotalLiterals = 282261
FlipsPerSecond = 102405
BestStep_Mean = 21045.48729
Steps_Mean = 99999.47998
Steps_Max = 100000
PercentSuccess = 0.00
BestSolution_Mean = 1.00875
BestSolution_Median = 1
BestSolution_Min = 0
BestSolution_Max = 2
> E=read_ubcsat("Schur_L_5_159.cnf_OUT",nrows=100000)
    0     1     2
    2 99121   877
100000

> ubcsat-okl -alg rsaps -runs 100000 -cutoff 100000 -i Schur_L_5_160.cnf | tee Schur_L_5_160.cnf_OUT
Clauses = 32215
Variables = 480
TotalLiterals = 285855
FlipsPerSecond = 105153
BestStep_Mean = 21549.21393
Steps_Mean = 100000
Steps_Max = 100000
PercentSuccess = 0.00
BestSolution_Mean = 1.01042
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 2
> E=read_ubcsat("Schur_L_5_160.cnf_OUT",nrows=100000)
    1     2
98958  1042
100000
   \endverbatim
   </li>
  </ul>


  \todo Weak nested standard translation
  <ul>
   <li> Creation by output_schur_standnest_stdname(5,n). </li>
   <li> Instance-statistics:
   \verbatim
> cat Schur_N_5_159.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    636   31335    636    636   31335   262108     NA     NA   31335   262108   639
 length   count
      2      79
      3    6188
      4      79
      6    6267
      8     158
      9    6188
     12   12376
   \endverbatim
   </li>
   <li> Determining the best Ubcsat-solver:
   \verbatim
> E = run_ubcsat("Schur_N_5_159.cnf", runs=100, cutoff=1000000)
> eval_ubcsat_dataframe(E,FALSE)
1. gwsat:
 1  2  3
93  4  3
fps: 126576
2. rsaps:
 1  2
83 17
fps: 102845
3. g2wsat:
 1  2  3  4  5  6  7
65 14  9  7  2  2  1
fps: 117436
4. hwsat:
 1  2  3  4  5
62 10 19  7  2
fps: 119872
5. dano:
 1  2  3  4  5  7  8
52 23 11  6  3  4  1
fps: 78569
6. saps:
 1  2  3  4  5  6
52 14 13 12  7  2
fps: 105617
7. wsattn:
 1  2  3  4  5  6  7  8
51 21 10  7  5  4  1  1
fps: 135650
8. paws:
 1  2  3  4  5  6  7  8  9 11 12
51 15  5  5  6  4  7  3  2  1  1
fps: 162303

# re-run on cs-oksvr for new algorithms:
1. vw25:
  1
100
fps: 142013
2. gwsat:
 1  2  3  5
88  7  4  1
fps: 127645
3. rsaps:
 1  2
87 13
fps: 108494
4. jack:
 1  2  3  4  5
77  3 13  1  6
fps: 66194
5. g2wsat:
 1  2  3  4  5  7
69 12 10  4  3  2
fps: 115440
6. hwsat:
 1  2  3  4  5  7
67 15 12  2  3  1
fps: 125439
7. wsattn:
 1  2  3  4  5  6  7  8  9
57 16  8  6  5  1  4  2  1
fps: 137802
8. ag2wsat:
 1  2  3  4  5  6
55 15 14  8  6  2
fps: 121353
9. wsatt:
 1  2  3  4  5  6  7  9 10
53 12 11 10  7  3  2  1  1
fps: 138839
10. dano:
 1  2  3  4  5  6  7
52 24 11  3  5  4  1
fps: 77251
11. sapsnr:
 1  2  3  4  5  6  7  8  9
51 13 11  7  8  2  2  1  5
fps: 114519
   \endverbatim
   Doesn't look better than the direct encoding, and is slower by, say, a
   factor of 3. Again vw25 best.
   </li>
   <li> Searching for a solution:
   \verbatim
> ubcsat-okl -alg gwsat -runs 100000 -cutoff 100000 -i Schur_N_5_159.cnf | tee Schur_N_5_159.cnf_OUT
Clauses = 31335
Variables = 636
TotalLiterals = 262108
FlipsPerSecond = 125743
BestStep_Mean = 44998.58349
Steps_Mean = 100000
Steps_Max = 100000
PercentSuccess = 0.00
BestSolution_Mean = 2.2196
BestSolution_Median = 2
BestSolution_Min = 1
BestSolution_Max = 10
> E=read_ubcsat("Schur_N_5_159.cnf_OUT",nrows=100000)
    1     2     3     4     5     6     7     8     9    10
33427 26502 29397  6989  3108   278   223    72     3     1
100000
   \endverbatim
   This is weaker than what we get with the direct encoding. </li>
  </ul>


  \todo Palindromic problems, direct encoding
  <ul>
   <li> Determining the best Ubcsat-solver:
   \verbatim
> E = run_ubcsat("Schur_pd_5_160.cnf", runs=100, cutoff=1000000)
> eval_ubcsat_dataframe(E,FALSE)
1. saps:
 0  1
 4 96
fps: 574542
2. gwsat:
  1
100
fps: 536682
3. gsatt:
  1
100
fps: 314762
4. wsat:
  1
100
fps: 1081315
5. rsaps:
  1
100
fps: 572017
6. sapsnr:
  1
100
fps: 583601
7. rots:
  1
100
fps: 315876
8. irots:
  1
100
fps: 292894
9. samd:
  1
100
fps: 316126
10. wsattn:
  1
100
fps: 1107174
11. novpp:
  1
100
fps: 937647
12. paws:
  1
100
fps: 1040150
13. ddfw:
  1
100
fps: 203728
14. g2wsat:
  1
100
fps: 687521
15. ag2wsat:
  1
100
fps: 677048
16. vw1:
  1
100
fps: 1060895

# re-run on cs-oksvr:
1. saps:
 0  1
 2 98
fps: 576779
2. sapsnr:
 0  1
 1 99
fps: 579835
3. gwsat:
  1
100
fps: 535992
4. gsatt:
  1
100
fps: 315288
5. wsat:
  1
100
fps: 1083541
6. rsaps:
  1
100
fps: 569898
7. rots:
  1
100
fps: 316196
8. irots:
  1
100
fps: 292586
9. samd:
  1
100
fps: 316536
10. wsattn:
  1
100
fps: 1106317
11. novpp:
  1
100
fps: 928333
12. paws:
  1
100
fps: 1033271
13. ddfw:
  1
100
fps: 205259
14. g2wsat:
  1
100
fps: 687521
15. ag2wsat:
  1
100
fps: 674673
16. vw1:
  1
100
fps: 1063603
17. vw25:
  1
100
fps: 971440
   \endverbatim
   saps clearly best.
   </li>
   <li> Finding solutions for n=157 (apparently [Fredricksen, Sweet, 2000]
   claim it to be satisfiable):
   \verbatim
> ubcsat-okl -alg saps -cutoff 10000000 -runs 100 -i Schur_pd_5_157.cnf | tee Schur_pd_5_157.cnf_OUT
Clauses = 10879
Variables = 395
TotalLiterals = 31615
FlipsPerSecond = 628792
BestStep_Mean = 3963.34
Steps_Mean = 10000000
Steps_Max = 10000000
PercentSuccess = 0.00
BestSolution_Mean = 1
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 1

> ubcsat-okl -alg saps -cutoff 1000000 -runs 10000 -i Schur_pd_5_157.cnf | tee Schur_pd_5_157.cnf_OUT
> E=read_ubcsat("Schur_pd_5_157.cnf_OUT",nrows=10000)
   0    1
   3 9997
10000
> E$seed[E$min==0]
[1] "302998036"  "3887686207" "15797683"
   \endverbatim
   </li>
   <li> Searching solutions for n=158:
   \verbatim
> ubcsat-okl -alg saps -cutoff 200000 -runs 100000 -i Schur_pd_5_158.cnf | tee Schur_pd_5_158.cnf_OUT
Clauses = 11415
Variables = 400
TotalLiterals = 33210
FlipsPerSecond = 561815
BestStep_Mean = 1457.60476
Steps_Mean = 200000
Steps_Max = 200000
PercentSuccess = 0.00
BestSolution_Mean = 1
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 1
   \endverbatim
   Could be that this instance is harder to satisfy, or unsatisfiable.
   </li>
   <li> Finding solutions for n=159 (apparently [Fredricksen, Sweet, 2000]
   claim it to be satisfiable):
   \verbatim
> ubcsat-okl -alg saps -cutoff 1000000 -runs 10000 -i Schur_pd_5_159.cnf | tee Schur_pd_5_159.cnf_OUT
Clauses = 11165
Variables = 400
TotalLiterals = 32465
FlipsPerSecond = 585812
BestStep_Mean = 9027.5888
Steps_Mean = 1000000
Steps_Max = 1000000
PercentSuccess = 0.00
BestSolution_Mean = 1
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 1
> E=read_ubcsat("Schur_pd_5_159.cnf_OUT",nrows=10000)
> summary(E$osteps)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
   154.0    631.8   1982.0   9028.0  10490.0 161100.0

> ubcsat-okl -alg saps -cutoff 200000 -runs 100000 -i Schur_pd_5_159.cnf | tee Schur_pd_5_159.cnf_OUT
FlipsPerSecond = 588907
BestStep_Mean = 9077.0986
Steps_Mean = 199993.23584
Steps_Max = 200000
PercentSuccess = 0.00
BestSolution_Mean = 0.99997
BestSolution_Median = 1
BestSolution_Min = 0
BestSolution_Max = 2
> E=read_ubcsat("Schur_pd_5_159.cnf_OUT",nrows=100000)
    0     1     2
    4 99995     1
100000
> E$seed[E$min==0]
[1] "4159171111" "114785590"  "2258818468" "4102133400"
   \endverbatim
   </li>
   <li> Finding solutions for n=160:
   \verbatim
> ubcsat-okl -alg saps -cutoff 10000000 -runs 100 -i Schur_pd_5_160.cnf | tee Schur_pd_5_160.cnf_OUT
Clauses = 11290
Variables = 400
TotalLiterals = 32830
FlipsPerSecond = 574221
BestStep_Mean = 757499.69
Steps_Mean = 9256157.25
Steps_Max = 10000000
PercentSuccess = 15.00
BestSolution_Mean = 0.85
BestSolution_Median = 1
BestSolution_Min = 0
BestSolution_Max = 1
> E=read_ubcsat("Schur_pd_5_160.cnf_OUT",nrows=100)
 0  1
15 85
100
   \endverbatim
   </li>
   <li> Searching solutions for n=161:
   \verbatim
> ubcsat-okl -alg saps -cutoff 1000000 -runs 20000 -i Schur_pd_5_161.cnf | tee Schur_pd_5_161.cnf_OUT
Clauses = 11822
Variables = 410
TotalLiterals = 34405
FlipsPerSecond = 533877
BestStep_Mean = 7418.49065
Steps_Mean = 1000000
Steps_Max = 1000000
PercentSuccess = 0.00
BestSolution_Mean = 1
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 1
   \endverbatim
   Looks unsatisfiable.
   </li>
   <li> Searching solutions for n=162:
   \verbatim
> ubcsat-okl -alg saps -cutoff 20000000 -runs 1000 -i Schur_pd_5_162.cnf | tee Schur_pd_5_162.cnf_OUT
Clauses = 11556
Variables = 405
TotalLiterals = 33615
FlipsPerSecond = 551812
BestStep_Mean = 1530.921
Steps_Mean = 20000000
Steps_Max = 20000000
PercentSuccess = 0.00
BestSolution_Mean = 1
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 1
   \endverbatim
   Looks unsatisfiable.
   </li>
  </ul>


  \todo Palindromic weak problems, direct encoding
  <ul>
   <li> n=189:
   \verbatim
> ubcsat-okl -alg saps -runs 100 -cutoff 10000000 -i WSchur_pd_5_189.cnf | tee WSchur_pd_5_189.cnf_OUT
Clauses = 15620
Variables = 475
TotalLiterals = 45635
FlipsPerSecond = 514308
BestStep_Mean = 130070.4
Steps_Mean = 10000000
Steps_Max = 10000000
PercentSuccess = 0.00
BestSolution_Mean = 1.51
BestSolution_Median = 2
BestSolution_Min = 1
BestSolution_Max = 2
> E=read_ubcsat("WSchur_pd_5_189.cnf_OUT",nrows=100)
 1  2
49 51
100
> ubcsat-okl -alg rsaps -runs 100 -cutoff 10000000 -i WSchur_pd_5_189.cnf | tee WSchur_pd_5_189.cnf_OUT
Clauses = 15620
Variables = 475
TotalLiterals = 45635
FlipsPerSecond = 470283
BestStep_Mean = 50748.41
Steps_Mean = 10000000
Steps_Max = 10000000
PercentSuccess = 0.00
BestSolution_Mean = 1
BestSolution_Median = 1
BestSolution_Min = 1
BestSolution_Max = 1
   \endverbatim
   </li>
   <li> Determining the best Ubcsat-solver (done below again for satisfiable
   case):
   \verbatim
> E = run_ubcsat("WSchur_pd_5_189.cnf", runs=100, cutoff=1000000)
> eval_ubcsat_dataframe(E,FALSE)
1. rsaps:
  1
100
fps: 464922
2. g2wsat:
  1
100
fps: 600276
3. hwsat:
 1  2
99  1
fps: 307267
4. vw1:
 1  2
99  1
fps: 891424
5. rots:
 1  2
97  3
fps: 261766
6. wsat:
 1  2
91  9
fps: 889759
7. ddfw:
 1  2
89 11
fps: 120941
8. ag2wsat:
 1  2
89 11
fps: 594919

# re-run on cs-oksvr:
1. rsaps:
  1
100
fps: 440180
2. g2wsat:
  1
100
fps: 598695
3. vw25:
  1
100
fps: 834168
4. rots:
 1  2
99  1
fps: 262426
5. vw1:
 1  2
99  1
fps: 885504
6. hwsat:
 1  2
98  2
fps: 307409
7. wsat:
 1  2
92  8
fps: 892140
8. ag2wsat:
 1  2
88 12
fps: 595806
9. ddfw:
 1  2
86 14
fps: 120998
   \endverbatim
   Also here vw25 best.
   </li>
   <li> The first open cases are n=158,161. </li>
   <li> Determining the best Ubcsat-solver for a satisfiable case:
   \verbatim
> E = run_ubcsat("WSchur_pd_5_153.cnf", runs=200, cutoff=100000)
> eval_ubcsat_dataframe(E,FALSE)
1. wsat:
  1
200
fps: 1154068
2. saps:
  1
200
fps: 628536
3. rsaps:
  1
200
fps: 612557
4. sapsnr:
  1
200
fps: 639182
5. rots:
  1
200
fps: 319438
6. paws:
  1
200
fps: 1005025
7. g2wsat:
  1
200
fps: 726216
8. ag2wsat:
  1
200
fps: 712251
9. vw1:
  1
200
fps: 1146789
10. vw25:
  1
200
fps: 1028278
   \endverbatim
   Hard to differentiate. Let's use vw25. </li>
   <li> To determine performance, investigating satisfiable case n=153:
   \verbatim
> ubcsat-okl -alg vw2 -v 2005 -cutoff 100000 -runs 200000 -i WSchur_pd_5_153.cnf | tee WSchur_pd_5_153.cnf_OUT
FlipsPerSecond = 1022957
BestStep_Mean = 7004.23837
Steps_Mean = 99924.38374
Steps_Max = 100000
PercentSuccess = 0.16
BestSolution_Mean = 0.998445
BestSolution_Median = 1
BestSolution_Min = 0
BestSolution_Max = 2
> E=read_ubcsat("WSchur_pd_5_153.cnf_OUT",nrows=200000)
     0      1      2
   321 199669     10
200000

> ubcsat-okl -alg saps -cutoff 100000 -runs 200000 -i WSchur_pd_5_153.cnf | tee WSchur_pd_5_153.cnf_OUT
FlipsPerSecond = 641957
BestStep_Mean = 2079.5598
Steps_Mean = 99959.77667
Steps_Max = 100000
PercentSuccess = 0.05
BestSolution_Mean = 0.99953
BestSolution_Median = 1
BestSolution_Min = 0
BestSolution_Max = 1
> E=read_ubcsat("WSchur_pd_5_153.cnf_OUT",nrows=200000)
     0      1
    94 199906
200000
   \endverbatim
   </li>
   <li> n=157 with full symmetry-breaking (not clear whether saps is also best
   here, but let's try):
   \verbatim
> ubcsat-okl -alg saps -cutoff 200000 -runs 200000 -i WSchur_pd_fullsbUCP_5_157.cnf | tee WSchur_pd_fullsbUCP_5_157.cnf_OUT
# aborted due to solution found:
> E=read_ubcsat("WSchur_pd_fullsbUCP_5_157.cnf_OUT")
    0     1
    1 86892
86893
> E[E$min==0,]
      sat min osteps msteps       seed
43604   1   0    702    702 4120959699

> ubcsat-okl -alg saps -runs 1 -cutoff 702 -i WSchur_pd_fullsbUCP_5_157.cnf -seed 4120959699 -solve > Solution_1
> OKsolver_2002-O3-DNDEBUG -SF -S="UCP_1" -D0 WSchur_pd_fullsb_5_157.cnf

# editing of Solution_1, UCP_1 to transform them into CNF
> oklib --maxima
oklib_load_all();
pa1 : first(read_fcl_f("Solution_1")[2]);
pa2 : first(read_fcl_f("UCP_1")[2]);
pa : compo_pass(pa1,pa2);
pa : subset(pa, lambda([x], is(x>0)));
I : invstandardise_pd_wschur_aloamo(5,157);
P : extract_partition(map(I,pa));
  [{1,8,31,37,46,48,50,59,61,63,65,70,72,76},
   {3,4,5,11,12,13,19,34,35,41,42,43,49,51,57,71,79},
   {2,6,9,10,14,17,36,39,40,44,47,52,55,60,68},
   {18,21,22,23,24,25,26,27,28,29,30,32,33,38,69,73,74,75,77,78},
   {7,15,16,20,45,53,54,56,58,62,64,66,67}]
FP : uncompresss_wschurpalindromic_subsets(157,P);
certificate_pdwschurfsb_p(5,157,FP);
  true
certificate_pdschur_p(5,157,FP);
  true
certificate_pdschurfsb_p(5,157,FP);
  false
   \endverbatim
   </li>
   <li> n=159 with full symmetry-breaking:
   \verbatim
> ubcsat-okl -alg saps -cutoff 200000 -runs 200000 -i WSchur_pd_fullsbUCP_5_159.cnf | tee WSchur_pd_fullsbUCP_5_159.cnf_OUT
XXX cs-wsok
   \endverbatim
   </li>
   <li> n=160 with full symmetry-breaking:
   \verbatim
> ubcsat-okl -alg saps -cutoff 200000 -runs 200000 -i WSchur_pd_fullsbUCP_5_160.cnf | tee WSchur_pd_fullsbUCP_5_160.cnf_OUT
# aborted since solution was found:
> E=read_ubcsat("WSchur_pd_fullsbUCP_5_160.cnf_OUT")
  0   1
  1 134
135
> E[E$min==0,]
   sat min osteps msteps       seed
65   1   0  21468  21468 2896961143

> ubcsat-okl -alg saps -runs 1 -cutoff 21468 -i WSchur_pd_fullsbUCP_5_160.cnf -seed 2896961143 -solve > Solution_1
> OKsolver_2002-O3-DNDEBUG -SF -S="UCP_1" -D0 WSchur_pd_fullsb_5_160.cnf

# editing of Solution_1, UCP_1 to transform them into CNF
> oklib --maxima
oklib_load_all();
pa1 : first(read_fcl_f("Solution_1")[2]);
pa2 : first(read_fcl_f("UCP_1")[2]);
pa : compo_pass(pa1,pa2);
pa : subset(pa, lambda([x], is(x>0)));
I : invstandardise_pd_wschur_aloamo(5,160);
P : extract_partition(map(I,pa));
    [{1,6,8,10,23,25,30,37,39,42,54,57,66,71,75},
     {2,3,7,11,12,17,21,26,35,36,40,41,45,49,50,59,64,65,69,73,74,78},
     {9,13,27,32,33,34,38,48,56,58,62,63,77},
     {14,15,16,18,22,24,47,51,53,60,70,72,80},
     {4,5,19,20,28,29,31,43,44,46,52,55,61,67,68,76,79}]
FP : uncompresss_wschurpalindromic_subsets(160,P);
certificate_pdwschurfsb_p(5,160,FP);
  true
certificate_pdschur_p(5,160,FP);
  true
certificate_pdschurfsb_p(5,160,FP);
  false
   \endverbatim
   </li>
  </ul>

*/
