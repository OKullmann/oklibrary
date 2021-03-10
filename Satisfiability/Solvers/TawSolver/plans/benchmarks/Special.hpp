// Oliver Kullmann, 30.1.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/TawSolver/plans/benchmarks/Special.hpp
  \brief Special benchmarks


  \todo Concrete instances (for SAT and #SAT)
  <ul>
   <li> VanDerWaerden_2-3-12_135.cnf </li>
   <li> VanDerWaerden_pd_2-3-21_405.cnf </li>
   <li> QueensProblem_015.cnf </li>
  </ul>


  \todo Random instances for SAT
  <ul>
   <li> BRG "1350*320,3" "" seed </li>
   <li> BRG "1800*200,4" "" seed </li>
   <li> BRG "1100*70,5" "" seed </li>
  </ul>


  \todo Random instances for #SAT
  <ul>
   <li> BRG "120*120,2" "" seed </li>
   <li> BRG "840*210,3" "" seed </li>
   <li> BRG "880*110,4" "" seed </li>
   <Li> BRG "2000*100,5" "" seed </li>
  </ul>


  \todo Plot alpha and lambda
  <ul>
   <li> For lambda the range
         1,2,...,100,200,...,1000
        looks reasonable at first.
   </li>
   <li> For alpha the range
         0.01, ..., 0.99
        should be feasible.
   </li>
   <li> Runtimes for now are not important (and thus the workload and machine
        isn't important. </li>
   <li> So for the single instances just a single run. </li>
   <li> And for the random instances we use the number R of for each instance-
        type (that is, using seeds=0,...,R-1); say R=100. </li>
   <li> This for the current version with alpha and/or lambda. </li>
   <li> For comparison also running the corresponding standard-versions
        (without alpha and/or lambda) . </li>
   <li> Using the current experiment-system, with all instances created in
        advances. </li>
  </ul>

*/


