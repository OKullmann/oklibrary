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


  \todo Local search for the direct encoding
  <ul>
   <li> Considering Schur_5_159.cnf; the following data is for the old
   version, using the full hypergraph, not, as now, the subsumption-free form;
   this shouldn't make a big difference. </li>
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
   we found one solution (seed=830151296, msteps=31379). However finding a
   solution seems difficult:
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
XXX cs-wsok
   \endverbatim
   </li>
   <li> On the other hand we get
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
   so the conjecture schur(5)=160 seems justified --- however this is false
   (see above)! </li>
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

*/
