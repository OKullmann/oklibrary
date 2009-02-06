// Oliver Kullmann, 4.2.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_2-3-6.hpp
  \brief Investigations on greentao_2(3,6)


  \todo Establishing the target
  <ul>
   <li> Analysing the sequence 4,7,23,79,528 by linear regressions seems to
   suggest it is linear in the log-log, and a linear model then is
   exp(exp(-0.05899 + i * 0.38269)), which would yield 11691 as the
   next term. </li>
   <li> I would guess that this is too big (more like 6000). </li>
   <li> n = 5000 looks actually highly unsatisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 100000 -i GreenTao_2_3_6_5000.cnf
       sat  min     osteps     msteps       seed
      1 0   762      95596     100000 1800420164
      2 0   757      87106     100000 1703561105
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 1000000 -i GreenTao_2_3_6_5000.cnf
       sat  min     osteps     msteps       seed
      1 0   728     346940    1000000 1829692653
      2 0   723     924133    1000000 4140965461
      3 0   724     835749    1000000  415080732
      4 0   705     843568    1000000 1307725777
      5 0   727     209350    1000000 2801750750
      6 0   726     652002    1000000 3411586467
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2_3_6_5000.cnf
       sat  min     osteps     msteps       seed
      1 0   714    7083923   10000000  621767107
   \endverbatim
   </li>
   <li> Of course, one needs to investigate again which ubcsat-algorithm
   works best here --- these problems are highly constrained. </li>
   <li> n = 4000 still loks unsatisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 100000 -i GreenTao_2_3_6_4000.cnf
Clauses = 547577
Variables = 4000
TotalLiterals = 1648107
FlipsPerSecond = 8108
BestStep_Mean = 54674.900000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 393.700000
BestSolution_Median = 392.500000
BestSolution_Min = 376.000000
BestSolution_Max = 411.000000
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 1000000 -i GreenTao_2_3_6_4000.cnf
       sat  min     osteps     msteps       seed
      1 0   378     131403    1000000  549915421
   \endverbatim
   </li>
   <li> n = 2000 is satisfiable: ubcsat-rnovelty+ found a solution
   with msteps=419656 and seed=1409958555 (cutoff = 10^6). </li>
   <li> So we assume 2000 < greentao_2(3,6) < 4000. </li>
  </ul>


  \todo Finding the best algorithm from ubcsat
  <ul>
   <li> adaptnovelty+ seems best:
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 10 -i GreenTao_2-3-6_2250.cnf
Clauses = 185490
Variables = 2250
TotalLiterals = 558633
FlipsPerSecond = 18008
BestStep_Mean = 88748.500000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 20.200000
BestSolution_Median = 20.000000
BestSolution_Min = 18.000000
BestSolution_Max = 22.000000
> ubcsat-okl -alg adaptnovelty+ -runs 10 -cutoff 1000000 -i GreenTao_2-3-6_2250.cnf
Clauses = 185490
Variables = 2250
TotalLiterals = 558633
FlipsPerSecond = 33208
BestStep_Mean = 695169.500000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 9.700000
BestSolution_Median = 9.500000
BestSolution_Min = 7.000000
BestSolution_Max = 12.000000
> ubcsat-okl -alg adaptnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2-3-6_2250.cnf
Clauses = 185490
Variables = 2250
TotalLiterals = 558633
FlipsPerSecond = 42551
BestStep_Mean = 3400237.000000
Steps_Mean = 10000000.000000
Steps_Max = 10000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 7.700000
BestSolution_Median = 7.500000
BestSolution_Min = 7.000000
BestSolution_Max = 9.000000
> ubcsat-okl -alg adaptnovelty+ -runs 10 -cutoff 100000000 -i GreenTao_2-3-6_2250.cnf
Clauses = 185490
Variables = 2250
TotalLiterals = 558633
FlipsPerSecond = 49276
BestStep_Mean = 25508521.900000
Steps_Mean = 100000000.000000
Steps_Max = 100000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 6.100000
BestSolution_Median = 6.000000
BestSolution_Min = 5.000000
BestSolution_Max = 7.000000
   \endverbatim
   </li>
   <li> Also "walksat-tabu -v nonull" seems better than rnovelty+ (but not as
   good as adaptnovelty+):
   \verbatim
> ubcsat-okl -alg  walksat-tabu -v nonull -runs 10 -i GreenTao_2-3-6_2250.cnf
Clauses = 185490
Variables = 2250
TotalLiterals = 558633
FlipsPerSecond = 24728
BestStep_Mean = 66136.100000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 21.300000
BestSolution_Median = 22.000000
BestSolution_Min = 13.000000
BestSolution_Max = 25.000000
> ubcsat-okl -alg  walksat-tabu -v nonull -runs 10 -cutoff 1000000 -i GreenTao_2-3-6_2250.cnf
Clauses = 185490
Variables = 2250
TotalLiterals = 558633
FlipsPerSecond = 32283
BestStep_Mean = 642215.000000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 14.500000
BestSolution_Median = 14.500000
BestSolution_Min = 11.000000
BestSolution_Max = 16.000000
   \endverbatim
   </li>
   <li> rnovelty+ seems third from the ubcsat-range (version 1.0.0):
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 100000 -i GreenTao_2-3-6_2250.cnf
Clauses = 185490
Variables = 2250
TotalLiterals = 558633
FlipsPerSecond = 22267
BestStep_Mean = 51646.200000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 24.600000
BestSolution_Median = 24.000000
BestSolution_Min = 21.000000
BestSolution_Max = 28.000000
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 1000000 -i GreenTao_2-3-6_2250.cnf
Clauses = 185490
Variables = 2250
TotalLiterals = 558633
FlipsPerSecond = 43269
BestStep_Mean = 495302.000000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 18.200000
BestSolution_Median = 19.500000
BestSolution_Min = 11.000000
BestSolution_Max = 23.000000
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2-3-6_2250.cnf
Clauses = 185490
Variables = 2250
TotalLiterals = 558633
FlipsPerSecond = 42383
BestStep_Mean = 4779885.400000
Steps_Mean = 10000000.000000
Steps_Max = 10000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 11.500000
BestSolution_Median = 12.000000
BestSolution_Min = 10.000000
BestSolution_Max = 13.000000
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 100000000 -i GreenTao_2-3-6_2250.cnf
Clauses = 185490
Variables = 2250
TotalLiterals = 558633
FlipsPerSecond = 39750
BestStep_Mean = 51162593.300000
Steps_Mean = 100000000.000000
Steps_Max = 100000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 7.300000
BestSolution_Median = 7.000000
BestSolution_Min = 6.000000
BestSolution_Max = 9.000000
   \endverbatim
   </li>
  </ul>


  \todo greentao_2(3,6) > 2000 (finding lower bounds)
  <ul>
   <li> n = 2250 looks unsatisfiable: see above. </li>
   <li> n = 2500 looks unsatisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 100000 -i GreenTao_2_3_6_2500.cnf
Clauses = 226059
Variables = 2500
TotalLiterals = 680712
FlipsPerSecond = 13038
BestStep_Mean = 68629.200000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 61.900000
BestSolution_Median = 62.500000
BestSolution_Min = 57.000000
BestSolution_Max = 64.000000
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 1000000 -i GreenTao_2_3_6_2500.cnf
Clauses = 226059
Variables = 2500
TotalLiterals = 680712
FlipsPerSecond = 13062
BestStep_Mean = 495738.100000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 54.000000
BestSolution_Median = 55.000000
BestSolution_Min = 47.000000
BestSolution_Max = 57.000000
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2_3_6_2500.cnf
Clauses = 226059
Variables = 2500
TotalLiterals = 680712
FlipsPerSecond = 12799
BestStep_Mean = 4374264.300000
Steps_Mean = 10000000.000000
Steps_Max = 10000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 50.300000
BestSolution_Median = 50.500000
BestSolution_Min = 46.000000
BestSolution_Max = 53.000000
   \endverbatim
   <li> n = 3000 looks unsatisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 100000 -i GreenTao_2_3_6_3000.cnf
Clauses = 318899
Variables = 3000
TotalLiterals = 960054
FlipsPerSecond = 7081
BestStep_Mean = 52997.600000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 137.300000
BestSolution_Median = 137.500000
BestSolution_Min = 126.000000
BestSolution_Max = 146.000000
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 1000000 -i GreenTao_2_3_6_3000.cnf
       sat  min     osteps     msteps       seed
      1 0   132     899782    1000000  257061986
      2 0   133     533282    1000000 3225188000
   \endverbatim
   </li>
  </ul>


  \todo Upper bounds
  <ul>
   <li> It appears that OKsolver would need, say, 50 years for n=5000 on the
   laptop. </li>
   <li> With a cluster, this would be feasible; and perhaps optimising the
   heuristics yields something. </li>
   <li> We need to investigate march_pl and minisat (problematic that progress
   is hard to judge). </li>
   <li> Also higher n could be tried. </li>
  </ul>

*/

