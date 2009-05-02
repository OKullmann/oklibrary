// Oliver Kullmann, 26.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_3-3-3-k.hpp
  \brief On investigations into Green-Tao numbers greentao_3(3,3,k)


  \todo greentao_3(3,3,3) = 137
  <ul>
   <li> DONE (available now)
   Symmetry breaking:
    <ol>
     <li> The only available symmetry since the symmetry between the
     partitions, i.e., one vertex can be put into the first partition. </li>
     <li> For vdW-problems it seems that the middle vertices are best used
     here (they have also the highest degrees); but here we should consider
     the vertex degrees. </li>
     <li> The variable of maximal degree is prime number 3. </li>
     <li> So perhaps we should always use this for symmetry breaking. </li>
    </ol>
   </li>
   <li> n=100 easily satisfiable by OKsolver_2002. </li>
   <li> n=125 easily found satisfiable by adaptnovelty+. </li>
   <li> n=132 easily found satisfiable by adaptnovelty+. </li>
   <li> n=135 easily found satisfiable by adaptnovelty+. </li>
   <li> n=136 easily found satisfiable by adaptnovelty+. </li>
   <li> n=137
    <ol>
     <ol> adaptnovelty+ with runs=1000 and cutoff=10^6 yields
     \verbatim
Clauses = 3614
Variables = 411
TotalLiterals = 10431
FlipsPerSecond = 1189726
BestStep_Mean = 162376.034000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
     \endverbatim
     </li>
     <li> OKsolver_2002 seems doable, in, say 1-2 days; one should also
     try it with the minisat2-preprocessor: seems quite a bit improved
     (also finding single nodes and autarkies). Still perhaps a few hours;
     perhaps by using symmetry breaking one gets it really down.
     </li>
     <li> march_pl unclear. </li>
     <li> minisat2 determines unsatisfiability with 2257091 conflicts in less
     than 2 minutes. </li>
     <li> Using symmetry breaking, actually 2686891 conflicts are needed;
     let's assume that this is a random effect. </li>
    </ol>
   </li>
   <li> n=138
    <ol>
     <li> adaptnovelty+ yields (nearly) constantly minimum=1 with
     cutoff=10^6. </li>
    </ol>
   </li>
   <li> n=150
    <ol>
     <li> minisat2 seems not to make progress. </li>
     <li> OKsolver_2002: perhaps doable, but likely not easy. </li>
     <li> From the ubcsat-1.0.0-suite seems adaptnovelty+ to be best. </li>
     <li> cutoff=10*10^6 yields minimum=1; with 10 runs and with 100 runs.
     </li>
    </ol>
   </li>
   <li> n=200
    <ol>
     <li> adaptnovelty+ doesn't seem to achieve better than a minimum=10. </li>
    </ol>
   </li>
  </ul>


  \todo greentao_3(3,3,4) > 430
  <ul>
   <li> n=150 trivially satisfiable by adaptnovelty+. </li>
   <li> n=200 trivially satisfiable by adaptnovelty+. </li>
   <li> n=300 trivially satisfiable by adaptnovelty+. </li>
   <li> n=400 easily satisfiable by adaptnovelty+ (cutoff=100*10^3). </li>
   <li> n=412 found satisfiable by adaptnovelty+ with cutoff=10*10^6
   (e.g., seed=573073979). </li>
   <li> n=419 found satisfiable by adaptnovelty+ with cutoff=10*10^6
   (seed=1429074341). </li>
   <li> n=425 found satisfiable by adaptnovelty+ with cutoff=10*10^6 (40 runs
   needed; seed=610774669). </li>
   <li> n=428: cutoff=50*10^6 found a solution after 17 runs (seed=621103542).
   </li>
   <li> n=429: cutoff=50*10^6 yields 13 solutions in 358 runs (e.g.,
   seed=2019144865). </li>
   <li> n=430: cutoff=50*10^6 yields 1 solution in 276 runs (seed=3076580871).
   </li>
   <li> n=431: cutoff=200*10^6 yields in 4 runs constantly min=1, and so
   does cutoff=50*10^6 in 10 runs; so let's postpone this. </li>
   <li> n=437
    <ol>
     <li> cutoff=10*10^6 with 65 runs only achieved 5 times min=1. </li>
     <li> cutoff=100*10^6 with 10 runs achieved 6 times min=1 and 4 times
     min=2. More runs:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 100000000 -i GreenTao_3-3-3-4_437.cnf
Clauses = 20918
Variables = 1311
TotalLiterals = 63157
FlipsPerSecond = 748371
BestStep_Mean = 29088565.360000
Steps_Mean = 100000000.000000
Steps_Max = 100000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.330000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 2.000000
     \endverbatim
     </li>
     <li> cutoff=10^9 doesn't seem effective:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 10 -cutoff 1000000000 -i GreenTao_3-3-3-4_437.cnf
Clauses = 20918
Variables = 1311
TotalLiterals = 63157
FlipsPerSecond = 724933
BestStep_Mean = 91967835.900000
Steps_Mean = 1000000000.000000
Steps_Max = 1000000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
     \endverbatim
     where the maximum of osteps is 170234040. </li>
     <li> So if this n is to be considered further, then with 1000 runs and
     a cutoff of 200*10^6: Aborted after 440 runs, with most of them yielding
     min=1 and some min=2. </li>
    </ol>
   </li>
   <li> n=450
    <ol>
     <li>
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_3-3-3-4_450.cnf
Clauses = 22012
Variables = 1350
TotalLiterals = 66488
FlipsPerSecond = 799233
BestStep_Mean = 4388254.200000
Steps_Mean = 10000000.000000
Steps_Max = 10000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 3.300000
BestSolution_Median = 3.000000
BestSolution_Min = 3.000000
BestSolution_Max = 4.000000
> ubcsat-okl -alg adaptnovelty+ -runs 10 -cutoff 100000000 -i GreenTao_3-3-3-4_450.cnf
Clauses = 22012
Variables = 1350
TotalLiterals = 66488
FlipsPerSecond = 761899
BestStep_Mean = 23395422.200000
Steps_Mean = 100000000.000000
Steps_Max = 100000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 2.100000
BestSolution_Median = 2.000000
BestSolution_Min = 2.000000
BestSolution_Max = 3.000000
> ubcsat-okl -alg adaptnovelty+ -runs 10 -cutoff 1000000000 -i GreenTao_3-3-3-4_450.cnf
Clauses = 22012
Variables = 1350
TotalLiterals = 66488
FlipsPerSecond = 718685
BestStep_Mean = 436206673.400000
Steps_Mean = 1000000000.000000
Steps_Max = 1000000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.500000
BestSolution_Median = 1.500000
BestSolution_Min = 1.000000
BestSolution_Max = 2.000000
     \endverbatim
     </li>
     <li> This needs to be investigated further, but first we look at smaller
     n. </li>
     <li> Since osteps for cutoff=10^9 comes close to the cutoff value, let's
     try 10 runs with cutoff=4*10^9. </li>
    </ol>
   </li>
   <li> n=500 looks unsatisfiable: cutoff=100*10^3 and 10 runs yields a
   minimum=14, cutoff=10^6 yields minimum=8, cutoff=10*10^6 yields
   minimum=7. </li>
  </ul>

*/

