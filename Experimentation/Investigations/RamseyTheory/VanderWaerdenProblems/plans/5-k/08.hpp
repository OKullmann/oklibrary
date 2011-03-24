// Oliver Kullmann, 11.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/08.hpp
  \brief On investigations into vdw_2(5,8)


  \todo Best complete solver for palindromic problems
  <ul>
   <li> It seems that from the conflict-driven solvers minisat-2.2.0 is best,
   from the look-ahead solvers march_pl (followed by OKsolver_2002, then
   satz215), and that overall minisat-2.2.0 is best. </li>
  </ul>


  \todo Best local-search solver for palindromic problems
  <ul>
   <li> Best ubcsat-algorithm:
   \verbatim
> PdVanderWaerdenCNF-O3-DNDEBUG 5 8 312

> E=run_ubcsat("VanDerWaerden_pd_2-5-8_312.cnf",runs=100,cutoff=1000000)

ag2wsat
 0  1  2
57 36  7
FlipsPerSecond = 208908
paws
 0  1  2  3
52 22 25  1
FlipsPerSecond = 204942
nov
 0  1  2  3
43 20 28  9
FlipsPerSecond = 169408
novp_p
 0  1  2  3  4
39 18 27 15  1
FlipsPerSecond = 129391
novpc
 0  1  2  3  4
36 11 29 23  1
FlipsPerSecond = 206143
anovp
 0  1  2
35 48 17
FlipsPerSecond = 172125
novp
 0  1  2  3
34 16 35 15
FlipsPerSecond = 169951
anovpp
 0  1  2
33 41 26
FlipsPerSecond = 170963
ddfw
 0  1  2
29 59 12
FlipsPerSecond = 89456
dano
 0  1  2
27 44 29
FlipsPerSecond = 169269
g2wsat
 0  1  2  3
27 23 44  6
FlipsPerSecond = 221298
   \endverbatim
   (segmentation faults with saps and sapsnr). So adaptg2wsat seems clearly
   strongest. </li>
  </ul>


  \todo vdw_2^pd(5,8) >= (312,323)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=311:
     \verbatim
1,6,8,9,11,14,16,17,19,22,
26,29,30,32,33,35,40,41,49,50,
55,57,60,61,62,64,68,71,73,74,
75,76,79,80,81,83,84,85,88,89,
90,91,93,96,100,102,104,106,107,109,
114,115,123,124,129,131,132,134,135,136,
138,142,145,147,148,149,150,153,154,155
     \endverbatim
     </li>
     <li> n=322:
     \verbatim
1,2,4,5,6,7,13,14,18,21,
27,32,35,37,39,41,45,46,47,48,
50,51,52,53,55,56,62,64,66,68,
71,76,82,85,89,90,94,96,97,99,
100,101,102,108,109,110,112,120,125,126,
127,129,133,135,136,138,139,140,143,144,
145,150,152,153,154,156,159
     \endverbatim
    </ol>
   </li>
   <li> "RunPdVdWk1k2 5 8 adaptg2wsat 100 1000000" yields
   \verbatim
Break point 1: 306
Break point 2: 323
   \endverbatim
   </li>
   <li> However minisat-2.2.0 determines instances with n <= 312 as
   satisfiable. So this is our first case of a failure of the
   RunPdVdWk1k2-method. Increase the cutoff. </li>
   li> "RunPdVdWk1k2 5 8 adaptg2wsat 100 2000000" yields
   \verbatim
Break point 1: 312
Break point 2: 323
   \endverbatim
   finding all solutions in the first run with the previous solution.
   </li>
  </ul>

*/
