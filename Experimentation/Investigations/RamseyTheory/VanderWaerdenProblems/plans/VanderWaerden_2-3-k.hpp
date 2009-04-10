// Oliver Kullmann, 10.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_2-3-k.hpp
  \brief On investigations into vdW-numbers vdw_2(3,k)
  
  
  \todo Literature overview
  <ul>
   <li> A conjecture seems to be that vdw_2(3,k) <= k^2 for k >= 3. </li>
   <li> The known values with k=1, ..., 16:
   \verbatim
k vdw_2(3,k)
1 3
2 6
3 9

16 238
   \endverbatim
   </li>
  </ul>


  \todo Performance of OKsolver-2002
  <ul>
   <li> k=16, n=238: </li>
  </ul>


  \todo Performance of march_pl


  \todo Performance of minisat2


  \todo Local search for the satisfiable instances
  <ul>
   <li> The first impression is that ubcsat::samd is best. </li>
   <li> For k=16, n=237 a local minimum of 1 is easily found (even with just
   100000 steps), while to find a solution one should use, say, 10^7 steps
   (and then solutions seem to be found often, e.g., seed=370833091). </li>
  </ul>

*/

