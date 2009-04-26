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
   <li> Symmetry breaking:
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

*/

