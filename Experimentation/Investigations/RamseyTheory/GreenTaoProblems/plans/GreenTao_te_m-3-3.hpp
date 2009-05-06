// Oliver Kullmann, 3.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_te_m-3-3.hpp
  \brief Investigations regarding the "transversal extension" numbers greentao_{m+2}([2,...,2,3,3])


  \todo greentao_3(2,3,3) = 31
  <ul>
   <li> n=30 trivially satisfiable by adaptnovelty+. </li>
   <li> Also trivial for the OKsolver_2002, which also determines that n=31
   is unsatisfiable with 171 nodes. </li>
  </ul>


  \todo greentao_4(2,2,3,3) = 39
  <ul>
   <li> Very easy for OKsolver_2002. </li>
  </ul>


  \todo greentao_5(2,2,2,3,3) = 41
  <ul>
   <li> n=40 trivially satisfiable by adaptnovelty+. </li>
   <li> Also still easy for OKsolver_2002. </li>
   <li> And n=41 needs 163846 nodes (less than a minute) with the OKsolver.
   </li>
   <li> minisat2 needs only less than a second. </li>
   <li> While satz215 seems to have some bigger problems. </li>
   <li> And also march_pl seems to have some problems. </li>
  </ul>

  
  \todo greentao_6(2,2,2,2,3,3) = 47
  <ul>
   <li> n=46 trivially satisfiable by adaptnovelty+. </li>
   <li> n=47 found unsatisfiable by minisat2 in 30 seconds. </li>
  </ul>


  \todo greentao_7(2,...,2,3,3) = 53
  <ul>
   <li> n=52 trivially satisfiable by adaptnovelty+. </li>
   <li> n=53 poses some difficulties for minisat2 (learning
   very long clauses), but in ~ 70m and 11222376 conflicts (27 restarts)
   unsatisfiability was confirmed. </li>
   <li> Using symmetry breaking
   ("output_greentao_sb_stdname([2,2,2,2,2,3,3],53)"): It's actually harder
   for minisat2, 15589496 conflicts and 28 restarts. It is understandable
   that minisat2 is hard to steer, and thus can't exploit the additional
   symmetry breaking rule, but again it gets a bit harder? </li>
  </ul>


  \todo greentao_8(2,...,2,3,3) >= 55
  <ul>
   <li> n=53 trivially satisfiable with cutoff=10000. </li>
   <li> For n=54 however a cutoff=10^6 is needed (still easy). </li>
   <li> n=55 yields constantly min=1 in 10 runs with cutoff=10^6 and
   with cutoff=10*10^6. </li>
   <li> The jump in minisat2's run time experienced above (from m=4 to
   m=5) seems to indicate a factor of 100; so roughly 5 days. We should
   give minisat2 10 days. </li>
   <li> We should also try the alternative encoding as discussed in
   "Alternative handling of parameter-values 2" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/VanderWaerdenProblems.hpp.
   </li>
  </ul>


  \todo greentao_9(2,...,2,3,3) >= 60
  <ul>
   <li> n=55, cutoff=10^6: trivially satisfiable. </li>
   <li> n=56, cutoff=10^6: trivially satisfiable. </li>
   <li> n=57, cutoff=10^6: still 100% success (always with 10 runs). </li>
   <li> n=58, cutoff=10^6: now 30% success. </li>
   <li> n=59, cutoff=10^6: 70% success. </li>
   <li> n=60, cutoff=10^6: 0% success, also with cutoff=10*10^6. </li>
  </ul>


  \todo greentao_10(2,...,2,3,3) >= 62
  <ul>
   <li> n=60, cutoff=10^6: 100% success. </li>
   <li> n=61, cutoff=10^6: 100% success. </li>
   <li> n=62, cutoff=10^6: 0% success, also with cutoff=10*10^6. </li>
  </ul>


  \todo greentao_11(2,...,2,3,3) >= 67
  <ul>
   <li> n=62, cutoff=10^6: 100% success. </li>
   <li> n=63, cutoff=10^6: 100% success. </li>
   <li> n=64, cutoff=10^6: 100% success. </li>
   <li> n=65, cutoff=10^6: 80% success. </li>
   <li> n=66, cutoff=10^6: 0% success, but 80% success with cutoff=10*10^6.
   </li>
   <li> n=67, cutoff=10*10^6: 0% success, also with cutoff=100*10^6. </li>
  </ul>


  \todo greentao_12(2,...,2,3,3) >= 71
  <ul>
   <li> n=67, cutoff=10*10^6: 100% success. </li>
   <li> n=68, cutoff=10*10^6: 100% success. </li>
   <li> n=69, cutoff=10*10^6: 100% success. </li>
   <li> n=70, cutoff=10*10^6: 90% success. </li>
   <li> n=71, cutoff=10*10^6: 0% success; 100 runs, cutoff=50*10^6:
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 50000000 -i GreenTao_12-2-2-2-2-2-2-2-2-2-2-3-3_71.cnf
Clauses = 30221
Variables = 852
TotalLiterals = 61766
FlipsPerSecond = 347739
BestStep_Mean = 387364.340000
Steps_Mean = 50000000.000000
Steps_Max = 50000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
   \endverbatim
   </li>
  </ul>

*/

