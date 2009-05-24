// Oliver Kullmann, 23.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_3-3-4-k.hpp
  \brief On investigations into Green-Tao numbers greentao_3(3,4,k)
  

  \todo greentao_3(3,4,4) > 1500
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
   <li> n=1512 </li>
   <li> n=1525: 10 runs with cutoff=100*10^6 yield only min=2. </li>
   <li> n=1550 looks (at first sight) unsatisfiable, 10 runs with
   cutoff=100*10^6 yield min=3 (twice; so cutoff=10^9 would be needed). </li>
  </ul>


  \todo greentao_3(3,4,5) > 1000
  <ul>
   <li> n=1000 trivial for adaptnovely+. </li>
   <li> n=2000 </li>
  </ul>
  
*/

