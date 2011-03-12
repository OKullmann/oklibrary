// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/17.hpp
  \brief On investigations into vdw_2(3,17) = 279


  \todo vanderwaerden_2(3,17) >= 279
  <ul>
   <li> n=270 easily found satisfiable with samd (cutoff=10^6). </li>
   <li> n=275 found satisfiable by adaptnovelty+ (run 14, cutoff=10^6,
   seed=1567198554). </li>
   <li> n=276 found satisfiable by adaptnovelty+ (run 28, cutoff=10^6,
   seed=1440019878). </li>
   <li> n=277 found satisfiable by adaptnovelty+ (run 10, cutoff=2*10^6,
   seed=2732014045). </li>
   <li> n=278
    <ol>
     <li> cutoff=2*10^6:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 2000000 -i VanDerWaerden_2-3-17_278.cnf -solve | tee VanDerWaerden_2-3-17_278.cnf_AUS
Clauses = 21460
Variables = 278
TotalLiterals = 96272
FlipsPerSecond = 69054
BestStep_Mean = 391212.470000
Steps_Mean = 2000000.000000
Steps_Max = 2000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.820000
BestSolution_Median = 2.000000
BestSolution_Min = 1.000000
BestSolution_Max = 2.000000
 1  2
18 82
     \endverbatim
     </li>
     <li> cutoff=4*10^6:
     \verbatim
 1  2
11 33
44
     \endverbatim
     </li>
     <li> cutoff=16*10^6: In run 222 a solution was found by adaptnovelty+
     (seed=2225069948, osteps=12016774). </li>
    </ol>
   </li>
   <li> n=279
    <ol>
     <li> cutoff=16*10^6:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 400 -cutoff 16000000 -i VanDerWaerden_2-3-17_279.cnf -solve | tee VanDerWaerden_2-3-17_279.cnf_AUS
Clauses = 21616
Variables = 279
TotalLiterals = 96978
FlipsPerSecond = 67558
BestStep_Mean = 2654188.382500
Steps_Mean = 16000000.000000
Steps_Max = 16000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.682500
BestSolution_Median = 2.000000
BestSolution_Min = 1.000000
BestSolution_Max = 2.000000
  1   2
127 273
     \endverbatim
     </li>
     <li> cutoff=32*10^6:
     \verbatim
  1   2
202 198
     \endverbatim
     </li>
     <li> cutoff=64*10^6:
     \verbatim
325  75
400
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=280
   \verbatim
> ubcsat-okl -alg samd -runs 100 -cutoff 1000000 -i VanDerWaerden_2-3-17_280.cnf -solve | tee VanDerWaerden_2-3-17_280.cnf_AUS
Clauses = 21772
Variables = 280
TotalLiterals = 97684
FlipsPerSecond = 131553
BestStep_Mean = 335464.530000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 2.090000
BestSolution_Median = 2.000000
BestSolution_Min = 2.000000
BestSolution_Max = 3.000000
 2  3
91  9
> ubcsat-okl -alg samd -runs 100 -cutoff 2000000 -i VanDerWaerden_2-3-17_280.cnf -solve | tee VanDerWaerden_2-3-17_280.cnf_AUS2 
 2  3
97  3
> ubcsat-okl -alg samd -runs 100 -cutoff 4000000 -i VanDerWaerden_2-3-17_280.cnf -solve | tee VanDerWaerden_2-3-17_280.cnf_AUS3
 1  2
 1 99
> ubcsat-okl -alg samd -runs 100 -cutoff 8000000 -i VanDerWaerden_2-3-17_280.cnf -solve | tee VanDerWaerden_2-3-17_280.cnf_AUS3
 1  2
 1 99
> ubcsat-okl -alg samd -runs 100 -cutoff 16000000 -i VanDerWaerden_2-3-17_280.cnf -solve | tee VanDerWaerden_2-3-17_280.cnf_AUS4
 1  2
 4 96
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 16000000 -i VanDerWaerden_2-3-17_280.cnf -solve | tee VanDerWaerden_2-3-17_280.cnf_AUS4
Clauses = 21772
Variables = 280
TotalLiterals = 97684
FlipsPerSecond = 66820
BestStep_Mean = 1808806.410000
Steps_Mean = 16000000.000000
Steps_Max = 16000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.860000
BestSolution_Median = 2.000000
BestSolution_Min = 1.000000
BestSolution_Max = 2.000000
 1  2
14 86
   \endverbatim
   So actually also here adaptnovelty+ seems best. </li>
  </ul>


  \todo Palindromic numbers
  <ul>
   <li> pdvanderwaerden([3,17])[2] = 279 = vanderwaerden([3,17]). </li>
   <li> pdvanderwaerden([3,17])[1] = 256. </li>
   <li> OKsolver_2002 determines n=278 as satisfiable (3m) and
   n=279 as unsatisfiable (30s). </li>
   <li> So does satz215 (1m,20s), march_pl (8s,1m) and minisat2 (27s,16s).
   </li>
   <li> So minisat2 seems best here; determines also n=257, 280 as
   unsatisfiable (15s, 73s). </li>
   <li> "RunPdVdW3k 17 gsat-tabu 100 400000" yields
   \verbatim
Break point 1: 256
Break point 2: 279
   \endverbatim
   where all solutions were found in the first three runs. </li>   
  </ul>

*/
