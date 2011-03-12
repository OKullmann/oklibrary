// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/18.hpp
  \brief On investigations into vdw_2(3,18) = 312


  \todo vanderwaerden_2(3,18) >= 312
  <ul>
   <li> n=300 found satisfiable with adaptnovelty+ with cutoff=10^6. </li>
   <li> n=301 found satisfiable with adaptnovelty+ with cutoff=10^6
   (seed=3824107188, osteps=627657). </li>
   <li> n=302 : cutoff=10^6
   \verbatim
 1  2
71 29
100
   \endverbatim
   but found satisfiable with cutoff=2*10^6 (seed=1205573131, osteps=988309,
   adaptnovelty+). </li>
   <li> n=303 : cutoff=10^6
   \verbatim
 1  2
61 39
100
   \endverbatim
    but found satisfiable with cutoff=2*10^6 (seed=3612981472, osteps=1991145,
    adaptnovelty+). </li>
   <li> n=305:
    <ol>
     <li> cutoff=10^6
     \verbatim
 1  2
61 39
100
     \endverbatim
     </li>
     <li> cutoff=2*10^6 found a solution in run 99 (seed=2434783444,
     osteps=532233, adaptnovelty+). </li>
    </ol>
   </li>
   <li> n=310: cutoff=10^6
   \verbatim
 1  2  3
60 39  1
100
   \endverbatim
   but with cutoff=8*10^6 in run 112 a solution was found (seed=1883662893,
   osteps=3087470, adaptnovelty+). </li>
   <li> n=311:
    <ol>
     <li> cutoff=8*10^6 yields in 400 runs always min=1. </li>
     <li> Using cutoff=4*10^6, in round 944 a solution was found
     (seed=1836688823, osteps=1914277, adaptnovelty+). </li>
    </ol>
   </li>
   <li> n=312
    <ol>
     <li> cutoff=4*10^6 with adaptnovelty+:
     \verbatim
   1    2
1942   58
2000
     \endverbatim
     (the histogram of osteps is nearly strictly decreasing, only at about
     3*10^6 we seem to have a local minimum, followed by a slight increase,
     peaking at 3.5*10^6). </li>
     <li> cutoff=8*10^6 with adaptnovelty+:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 2000 -cutoff 8000000 -i VanDerWaerden_2-3-18_312.cnf -solve | tee VanDerWaerden_2-3-18_312.cnf_AUS2
Clauses = 26889
Variables = 312
TotalLiterals = 121302
FlipsPerSecond = 56414
BestStep_Mean = 1106252.638500
Steps_Mean = 8000000.000000
Steps_Max = 8000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000500
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 2.000000
   1    2
1999    1
2000
     \endverbatim
     (now the histogram is strictly decreasing).
     </li>
     <li> cutoff=10^6 to compare samd and adaptnovelty+:
     \verbatim
> ubcsat-okl -alg samd -runs 8000 -cutoff 1000000 -i VanDerWaerden_2-3-18_312.cnf -solve | tee VanDerWaerden_2-3-18_312.cnf_AUS3
Clauses = 26889
Variables = 312
TotalLiterals = 121302
FlipsPerSecond = 108486
BestStep_Mean = 296836.281000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.140750
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 4.000000
   1    2    3    4
6885 1105    9    1
8000

> ubcsat-okl -alg adaptnovelty+ -runs 8000 -cutoff 1000000 -i VanDerWaerden_2-3-18_312.cnf -solve | tee VanDerWaerden_2-3-18_312.cnf_AUS4
Clauses = 26889
Variables = 312
TotalLiterals = 121302
FlipsPerSecond = 56657
BestStep_Mean = 364589.452625
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.430375
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 3.000000
   1    2    3
4708 3141  151
8000
     \endverbatim
     So actually samd seems to clearly outperform adaptnovelty+. </li>
     <li> Now cutoff=8*10^6:
     \verbatim
> ubcsat-okl -alg samd -runs 1000 -cutoff 8000000 -i VanDerWaerden_2-3-18_312.cnf -solve | tee VanDerWaerden_2-3-18_312.cnf_AUS5
Clauses = 26889
Variables = 312
TotalLiterals = 121302
FlipsPerSecond = 122375
BestStep_Mean = 486193.876000
Steps_Mean = 8000000.000000
Steps_Max = 8000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.013000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 10.000000
  1   2  10
995   4   1
1000
     \endverbatim
     So perhaps with higher cutoff adaptnovelty+ gets better? Though samd
     appears to be faster. </li>
    </ol>
   </li>
  </ul>


  \todo Palindromic numbers
  <ul>
   <li> pdvanderwaerden([3,18])[2] = 312 = vanderwaerden([3,18]). </li>
   <li> pdvanderwaerden([3,18])[1] = 299. </li>
   <li> minisat2 determines n=311 as satisfiable (42s) and
   n=312 as unsatisfiable (228s). </li>
   <li> Other running times:
    <ol>
     <li> picosat913 (22s, 74s) </li>
     <li> precosat236 (24s, 72s) </li>
     <li> march_pl (427s,768s) </li>
     <li> satz215 (5s, 434s,) </i>
     <li> OKsolver_2002 (110s, 2064s) </li>
    </ol>
   </li>
   <li> So actually picosat913 seems best. Determines also n=300, 313 as
   unsat (81s, 23s). </li>
   <li> "RunPdVdW3k 18 gsat-tabu 100 500000" yields
   \verbatim
Break point 1: 299
Break point 2: 312
   \endverbatim
   where all solutions were found in the first 14 runs. </li>
  </ul>

*/
