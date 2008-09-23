// Oliver Kullmann, 20.9.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/plans/VanderWaerdenProblems.hpp
  \brief On investigations into van-der-Waerden-like problems


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/general.hpp. </li>
   <li> See VanderWaerden.cpp (to be updated) for a C++ generator. </li>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Generators/plans/VanderWaerdenProblems.hpp.
   </li>
  </ul>


  \todo Overview on performance of SAT solvers
  <ul>
   <li> We need an overview how SAT solvers perform on the different van der
   Waerden problems. </li>
  </ul>


  \todo Green-Tao numbers
  <ul>
   <li> greentao(2,3)
    <ol>
     <li> greentao(2,3) = 23 (partitioning the first 23 prime numbers into 2
     parts, one part is guaranteed to contain an arithmetic progression of
     size 3, while using a smaller initial segment of prime numbers won't do).
     </li>
     <li> Trivial (for OKsolver-2002, and likely for any solver). </li>
    </ol>
   </li>
   <li> greentao(2,4)
    <ol>
     <li> greentao(2,4) > 400 (trivial for OKsolver-2002). </li>
     <li> greentao(2,4) > 420 with 6683 nodes (OKsolver-2002). </li>
     <li> greentao(2,4) > 430 with 22267 nodes (OKsolver-2002). </li>
     <li> greentao(2,4) > 440 with 77791 nodes (OKsolver-2002). </li>
     <li> greentao(2,4) > 450 with 349914 nodes (OKsolver-2002). </li>
     <li> greentao(2,4) > 460 with 12777 nodes (OKsolver-2002). </li>
     <li> greentao(2,4) > 470 with 59506 nodes (OKsolver-2002). </li>
     <li> greentao(2,4) > 471 with 58889 nodes (OKsolver-2002). </li>
     <li> greentao(2,4) > 472 with 370222 nodes (OKsolver-2002). </li>
     <li> For this n, rnovelty finds a solution quickly (~ 100000 steps).
     </li>
     <li> n = 473: stopped after 884587 nodes (OKsolver). </li>
     <li> But found satisfiable with rnovelty. </li>
     <li> And rnovelty+ is even better. </li>
     <li> n = 480: stopped after 1536394 nodes (OKsolver). </li>
     <li> n = 500: Running it with
     \verbatim
OKsolver_2002-O3-DNDEBUG -M -D18 -F GreenTao_2_4_500.cnf
     \endverbatim
     found a solution after 11 hours (38,937,288 nodes; roughly 2% of the
     search space, looking at depth 18). </li>
     <li> Found satisfiable with rnovelty+. </li>
     <li> n = 510 found satisfiable with rnovelty+. </li>
     <li> n = 511 found satisfiable with rnovelty+. </li>
     <li> n = 512: rnovelty+ yields constantly 1 falsified clause
     (also with "-runs 500 -cutoff 10000000"). </li>
     <li>
     \verbatim
OKplatform> OKsolver_2002-O3-DNDEBUG -M -D16 GreenTao_2_4_512.cnf
     \endverbatim
     seems to need ~ 6 days (csltok). </li>
     <li> n = 515: rnovelty+ yields constantly 1 falsified clause. </li>
     <li> n = 520: rnovelty+ yields constantly 1 falsified clause. </li>
     <li> n = 530: rnovelty+ yields constantly 1 falsified clause. </li>
     <li> n = 540: rnovelty+ yields constantly 1 falsified clause. </li>
     <li> n = 550: rnovelty+ yields constantly 2 falsified clauses. </li>
     <li> n = 600: rnovelty+ yields constantly 6 falsified clauses. </li>
     <li> For n in this magnitude the Maxima computation of the hypergraph is
     already very slow --- a more intelligent algorithm for finding the
     arithmetic progression amongst the prime numbers is needed (likely we
     cannot exploit the speciality of prime numbers, but we do it for arbitrary
     lists of numbers).
      <ol>
       <li> DONE
       One could use memoisation in the form that for every n we store
       the additional hyperedges (k-progressions). </li>
       <li> DONE (doesn't improve a single computation, but several)
       This would it make rather quick, without imposing big memory
       burdens. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> greentao(2,5)
    <ol>
     <li> Trivial for n=5000:
     \verbatim
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2_5_5000.cnf
Clauses = 26470
Variables = 5000
TotalLiterals = 132350
FlipsPerSecond = 254850
BestStep_Mean = 509.700000
Steps_Mean = 509.700000
Steps_Max = 538.000000
PercentSuccess = 100.00
     \endverbatim
     </li>
    </ol>
   </li>
   <li> One should study the density of the clause-sets (and the "threshold")
   here. </li>
   <li> One can also consider non-diagonal Green-Tao numbers. </li>
   <li> One needs to investigate whether bounds are known. </li>
  </ul>

*/

