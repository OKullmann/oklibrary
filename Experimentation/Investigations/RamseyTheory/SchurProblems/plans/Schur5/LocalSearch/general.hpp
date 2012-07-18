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
  enables to find all known solutions (and possibly) via SAT.


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
   \endverbatim
   Doesn't look better than the direct encoding, and is slower by, say, a
   factor of 3.
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
      <li> Searching solutions for n=162:
   \verbatim
> ubcsat-okl -alg saps -cutoff 20000000 -runs 1000 -i Schur_pd_5_162.cnf | tee Schur_pd_5_162.cnf_OUT
> E=read_ubcsat("Schur_pd_5_162.cnf_OUT",nrows=1000)
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

*/
