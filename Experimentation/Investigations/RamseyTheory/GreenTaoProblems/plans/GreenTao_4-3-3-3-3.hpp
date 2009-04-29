// Oliver Kullmann, 22.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_4-3-3-3-3.hpp
  \brief Investigations on greentao_4(3,3,3,3)


  \todo Lower bounds: greentao_4(3,3,3,3) > 375
  <ul>
   <li> Best we see first where adaptnovelty+ finds easily solutions. </li>
   <li> n=300 very easily satisfiable. </li>
   <li> n=338 easily satisfiable. </li>
   <li> n=348 found satisfiable (cutoff=10^6, seed=1463613527). </li>
   <li> n=352 found satisfiable (cutoff=10^6, seed=3476207011). </li>
   <li> n=355 found satisfiable (cutoff=10^6, seed=3865650519). </li>
   <li> n=356 found satisfiable (cutoff=10^6, seed=1266825813). </li>
   <li> n=357
    <ol>
     <li>
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 1000000 -i GreenTao_4-3-3-3-3_357.cnf
clauses = 26471
Variables = 1428
TotalLiterals = 77628
FlipsPerSecond = 659761
BestStep_Mean = 447147.770000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 2.330000
BestSolution_Median = 2.000000
BestSolution_Min = 1.000000
BestSolution_Max = 4.000000
     \endverbatim
     seems unsatisfiable.
     </li>
     <li>
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 1000 -cutoff 10000000 -i GreenTao_4-3-3-3-3_357.cnf
Clauses = 26471
Variables = 1428
TotalLiterals = 77628
FlipsPerSecond = 628361
BestStep_Mean = 3492205.863000
Steps_Mean = 9675267.971000
Steps_Max = 10000000.000000
PercentSuccess = 5.90
BestSolution_Mean = 1.236000
BestSolution_Median = 1.000000
BestSolution_Min = 0.000000
BestSolution_Max = 2.000000
     \endverbatim
     A solution was found e.g. with seed=876327096. </li>
    </ol>
   </li>
   <li> n=358 found satisfiable with cutoff=10*10^6 and seed=1641133745. </li>
   <li> n=359 found satisfiable with cutoff=10*10^6 and seed=2108885839. </li>
   <li> n=360 found satisfiable with cutoff=10*10^6 and seed=1802549929. </li>
   <li> n=361 found satisfiable with cutoff=10*10^6 and seed=1427782088
   (140 runs needed). </li>
   <li> n=362 found satisfiable with cutoff=10*10^6 and seed=2256326590. </li>
   <li> n=363 found satisfiable with cutoff=10*10^6 and seed=342405381. </li>
   <li> n=364 found satisfiable (seed=373433483)
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 1000 -cutoff 10000000 -i GreenTao_4-3-3-3-3_364.cnf
Clauses = 27420
Variables = 1456
TotalLiterals = 80440
FlipsPerSecond = 602655
BestStep_Mean = 3488853.231000
Steps_Mean = 9993570.374000
Steps_Max = 10000000.000000
PercentSuccess = 0.10
BestSolution_Mean = 2.195000
BestSolution_Median = 2.000000
BestSolution_Min = 0.000000
BestSolution_Max = 4.000000
   \endverbatim
   </li>
   <li> n=365 found satisfiable (seed=2430690635)
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 1000 -cutoff 10000000 -i GreenTao_4-3-3-3-3_365.cnf
Clauses = 27543
Variables = 1460
TotalLiterals = 80804
FlipsPerSecond = 598431
BestStep_Mean = 3608383.766000
Steps_Mean = 9990625.610000
Steps_Max = 10000000.000000
PercentSuccess = 0.10
BestSolution_Mean = 2.248000
BestSolution_Median = 2.000000
BestSolution_Min = 0.000000
BestSolution_Max = 4.000000
   \endverbatim
     </li>
    </ol>
   </li>
   <li> n=366 found satisfiable (seed=3191925315). </li>
   <li> n=367 found satisfiable (seed=1210488827). </li>
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 1000 -cutoff 10000000 -i GreenTao_4-3-3-3-3_367.cnf
Clauses = 27805
Variables = 1468
TotalLiterals = 81580
FlipsPerSecond = 597684
BestStep_Mean = 3695380.296000
Steps_Mean = 9999170.437000
Steps_Max = 10000000.000000
PercentSuccess = 0.10
BestSolution_Mean = 2.468000
BestSolution_Median = 2.000000
BestSolution_Min = 0.000000
BestSolution_Max = 4.000000
   \endverbatim
   </li>
   <li> n=368 found satisfiable:
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 1000 -cutoff 10000000 -i GreenTao_4-3-3-3-3_368.cnf
Clauses = 27936
Variables = 1472
TotalLiterals = 81968
FlipsPerSecond = 601632
BestStep_Mean = 3627492.165000
Steps_Mean = 9984052.649000
Steps_Max = 10000000.000000
PercentSuccess = 0.30
BestSolution_Mean = 2.580000
BestSolution_Median = 3.000000
BestSolution_Min = 0.000000
BestSolution_Max = 4.000000
   \endverbatim
   for example seed=1454356055. </li>
   <li> n=369 found satisfiable (2 solutions in 807 runs with
   cutoff=10000000=10*10^6, for example seed=191448713). </li>
   <li> n=370
    <ol>
     <li>
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 1000 -cutoff 10000000 -i GreenTao_4-3-3-3-3_370.cnf
Clauses = 28202
Variables = 1480
TotalLiterals = 82756
FlipsPerSecond = 612643
BestStep_Mean = 3801288.200000
Steps_Mean = 10000000.000000
Steps_Max = 10000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 2.821000
BestSolution_Median = 3.000000
BestSolution_Min = 1.000000
BestSolution_Max = 4.000000
     \endverbatim
     </li>
     <li>
     \verbatim With cutoff=20000000 in 1295 runs one solution was found
     (seed=406385002). </li>
    </ol>
   </li>
   <li> n=375:
    <ol>
     <li>
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 3000 -cutoff 10000000 -i GreenTao_4-3-3-3-3_375.cnf
Clauses = 28901
Variables = 1500
TotalLiterals = 84828
FlipsPerSecond = 587133
BestStep_Mean = 3945694.062333
Steps_Mean = 10000000.000000
Steps_Max = 10000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 3.491667
BestSolution_Median = 4.000000
BestSolution_Min = 1.000000
BestSolution_Max = 5.000000
ubcsat-okl -alg adaptnovelty+ -runs 1000 -cutoff 20000000 -i GreenTao_4-3-3-3-3_375.cnf
Clauses = 28901
Variables = 1500
TotalLiterals = 84828
FlipsPerSecond = 577930
BestStep_Mean = 7458479.743000
Steps_Mean = 20000000.000000
Steps_Max = 20000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 3.156000
BestSolution_Median = 3.000000
BestSolution_Min = 1.000000
BestSolution_Max = 5.000000
> ubcsat-okl -alg adaptnovelty+ -runs 1000 -cutoff 100000000 -i GreenTao_4-3-3-3-3_375.cnf
Clauses = 28901
Variables = 1500
TotalLiterals = 84828
FlipsPerSecond = 601664
BestStep_Mean = 31289071.326000
Steps_Mean = 100000000.000000
Steps_Max = 100000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 2.463000
BestSolution_Median = 2.000000
BestSolution_Min = 1.000000
BestSolution_Max = 4.000000
     \endverbatim
     </li>
     <li> Finally using cutoff=10^9 in run 31 a solution (seed=1958151049,
     osteps=765212681) was found. </li>
    </ol>
   </li>
   <li> n=400, cutoff=10^9 </li>
   <li> n=450: using cutoff=10^9, one run reaches a minimum of 14, which
   seems hopeless. </li>
   <li> n=600
    <ol>
     <li> The instance-generation by Maxima takes far too long.
      <ul>
       <li> nbfcsfd2fcs_aloamo is rather slow, but a worse bottleneck
       is standardise_fcs. </li>
       <li> And worst is standard_statistics_fcs! </li>
       <li> Where actually these statistics are not so informative anymore,
       since they mix three different types of clauses. So we better
       should have separate statistics on the hypergraphs, on the alo-
       and alo-clauses, and for everything together. </li>
      </ul>
     </li>
     <li> adaptnovelty+ reaches only values in the sixties. </li>
    </ol>
   </li>
  </ul>


  \todo Upper bounds
  <ul>
   <li> From the experience with "greentao_3(3,3,3) = 137" (see
   Investigations/RamseyTheory/GreenTaoProblems/plans/general.hpp)
   it seems that minisat2 performs best here. </li>
   <li> One one should employ symmetry breaking here. </li>
  </ul>

*/

