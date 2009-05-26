// Oliver Kullmann, 23.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_3-3-4-k.hpp
  \brief On investigations into Green-Tao numbers greentao_3(3,4,k)
  

  \todo greentao_3(3,4,4) > 1525
  <ul>
   <li> Creating problems via output_greentao_stdname([3,4,4],n). </li>
   <li> n=500 trivially satisfiable (adaptnovelty+). </li>
   <li> n=600 trivially satisfiable. </li>
   <li> n=700 trivially satisfiable. </li>
   <li> n=900 trivially satisfiable. </li>
   <li> n=1000 trivially satisfiable. </li>
   <li> n=1100 still easy to solve (cutoff=100*10^3). </li>
   <li> n=1200 still easy to solve (cutoff=10^6). </li>
   <li> n=1300 rather easy to solve (cutoff=10^6). </li>
   <li> n=1400: one solution in 10 runs with cutoff=10^6. </li>
   <li> n=1500: only min=3 for 10 runs with cutoff=10*10^6; cutoff=100*10^6
   found a solution in the third run (osteps=39412430, seed=1928236138). </li>
   <li> n=1512: one run in 10 with cutoff=100*10^6 found a solution
   (osteps=37551993, seed=4046775428). While another 100 runs with this
   cutoff actually found no solution. </li>
   <li> n=1518
    <ol>
     <li> 9 runs with cutoff=100*10^6 yield only a minimum of 2. </li>
     <li> cutoff=500*10^6: run 18 found a solution (osteps=309459026,
     seed=3122222825). </li>
    </ol>
   </li>
   <li> n=1525
    <ol>
     <li> 10 runs with cutoff=100*10^6 yield only min=2. </li>
     <li> cutoff=500*10^6: run 16 yields a solution (msteps=375313903,
     seed=2274305602). </li>
    </ol>
   </li>
   <li> n=1531 </li>
   <li> n=1537: 16 runs only yield min=1 twice, so a cutoff of 2.5*10^9
   would be needed. </li>
   <li> n=1550
    <ol>
     <li> Looks (at first sight) unsatisfiable: 10 runs with
     cutoff=100*10^6 yield min=3 (twice; so cutoff=10^9 would be needed). </li>
     <li> cutoff=500*10^6: 13 runs only yield min=2, so a cutoff of 2.5*10^9
     would be needed. </li>
    </ol>
   </li>
  </ul>


  \todo greentao_3(3,4,5) > 4000
  <ul>
   <li> n=1000 trivial for adaptnovelty+. </li>
   <li> n=2000 trivial for adaptnovelty+ (70% success with cutoff=10^4). </li>
   <li> n=2200 trivial for adaptnovelty+ (70% success with cutoff=10^4). </li>
   <li> n=3000 trivial for adaptnovelty+ (90% success with cutoff=10^5). </li>
   <li> n=4000 simple for adaptnovelty+ (100% success with cutoff=10^6). </li>
  </ul>
  
*/

