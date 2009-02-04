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


  \todo greentao_2(3,6) > 2000
  <ul>
   <li> n = 3000 </li>
  </li>

*/

