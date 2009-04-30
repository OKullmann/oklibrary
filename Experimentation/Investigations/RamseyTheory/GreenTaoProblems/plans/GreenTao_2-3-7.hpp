// Oliver Kullmann, 25.2.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_2-3-7.hpp
  \brief Investigations on greentao_2(3,7)


  \todo Finding the best algorithm from ubcsat
  <ul>
   <li> Is adaptnovelty+ still best? </li>
  </ul>


  \todo Survey propagation
  <ul>
   <li> For n=5000, 7500, 10000, 12500 survey propagation apparently thinks the
   problem is unsatisfiable, i.e., we always get
   \verbatim
> survey_propagation -l GreenTao_2-3-7_XX000.cnf
<bias>:nan
fixed 1 biased var (+0 ucp)
.:-)
<bias>:nan
contradiction
   \endverbatim
   However these problems are all satisfiable. </li>
   <li> the same behaviour for n=15000 (but here we don't know whether
   it's satisfiable). </li>
  </ul>


  \todo greentao_2(3,7) > 12500
  <ul>
   <li> n=5000: ubcsat::adaptnovelty+ finds it satisfiable with just a
   cutoff=10000. </li>
   </li>
   <li> n=7500: ubcsat::adaptnovelty+ finds it satisfiable with just a
   cutoff=100000. </li>
   </li>
   <li> n=10000: ubcsat::adaptnovelty+ finds it satisfiable with just a
   cutoff=1000000 (seed=3610283050). </li>
   <li> n=12500
    <ol>
     <li>
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 10 -cutoff 1000000 -i GreenTao_2-3-7_12500.cnf
Clauses = 4722593
Variables = 12500
TotalLiterals = 14173471
FlipsPerSecond = 6550
BestStep_Mean = 779318.300000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 14.200000
BestSolution_Median = 14.000000
BestSolution_Min = 8.000000
BestSolution_Max = 19.000000
     \endverbatim
     </li>
     <li> Found satisfiable (first run) with cutoff=10*10^6 (seed=348690330).
     </li>
    </ol>
   </li>
   <li> n=13750
    <ol>
     <li> A cutoff=100*10^6 yields in 10 runs a minimum of 2 and a maximum
     of 4. </li>
     <li> cutoff=10^9 </li>
    </ol>
   </li>
   <li> n=15000
    <ol>
     <li>
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2-3-7_15000.cnf
       sat  min     osteps     msteps       seed
      1 0    36    9340701   10000000 1782442807
      2 0    39    5704494   10000000  470832140
      3 0    37    8891559   10000000 3612813694
      4 0    33    9055929   10000000 1719584600
      5 0    41    9854280   10000000  433897223
     \endverbatim
     </li>
     <li> cutoff multiplied with 10:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 10 -cutoff 100000000 -i GreenTao_2-3-7_15000.cnf
       sat  min     osteps     msteps       seed
      1 0    17   43895806  100000000 1613156495
      2 0    23   98468694  100000000 3089531980
      3 0    25   57245388  100000000  293693136
     \endverbatim
     If this should be satisfiable then it's tough. </li>
    </ol>
   </li>
  </ul>

*/

