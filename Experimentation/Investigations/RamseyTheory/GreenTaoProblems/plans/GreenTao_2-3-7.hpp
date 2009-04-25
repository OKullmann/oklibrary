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
   <li> For n=5000, 7500, 10000 survey propagation apparently thinks the
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
   <li> n=15000 </li>
  </ul>

*/

