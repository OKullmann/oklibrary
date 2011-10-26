// Oliver Kullmann, 11.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/15.hpp
  \brief On investigations into vdw_2(4,15)


  \todo Best ubcsat-solver
  <ul>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
  </ul>


  \todo vanderwaerden_2(4,15) > 726
  <ul>
   <li> Start with the palindromic solution for n=726. </li>
  </ul>


  \todo vdw_2^pd(4,15) >= (724,727)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=723:
     \verbatim
6,13,22,32,36,39,45,48,54,56,
61,62,65,67,82,85,92,93,94,96,
110,111,119,133,136,142,151,153,154,158,
159,162,164,168,172,174,177,178,179,182,
183,185,189,193,194,200,203,207,217,226,
233,240,244,251,254,269,271,274,275,280,
282,288,291,297,300,313,314,323,327,336,
337,339,341,345,347,348,351,352,353,356,
358,362
     \endverbatim
     </li>
     <li> n=726:
     \verbatim
8,11,24,26,29,37,40,55,60,68,
78,80,81,83,84,92,97,99,114,118,
121,123,124,126,128,134,145,150,157,158,
159,161,162,164,165,175,177,178,180,181,
182,194,205,211,213,215,216,218,221,225,
231,234,242,247,249,255,256,258,259,261,
271,279,284,291,299,302,310,313,317,318,
320,322,331,344,346,351,352,353,355,356,
358,359
     \endverbatim
    </ol>
   </li>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite.
   \verbatim
> E=run_ubcsat("VanDerWaerden_pd_2-4-15_726.cnf",runs=100,cutoff=10000000)
1. ag2wsat:
 1  8  9 10 11 12 13
 1  4 14 28 40 12  1
fps: 128240
2. wsattn:
 4 24 26 27 28 29 30 31 32 33 34
 1  4  3  5  4 14 19 20 21  7  2
fps: 110109
3. samd:
 6  8  9 10 11 12 13 15
 2  4 21 24 35  7  6  1
fps: 204175
4. anovpp:
 7  8  9 10 11 12
 4  6 24 40 24  2
fps: 69213
5. anovp:
 7  8  9 10 11 12
 2  7 20 37 33  1
fps: 69307
6. dano:
 7  8  9 10 11 12
 1  5 26 43 20  5
fps: 71201
7. ddfw:
 7  8  9 10 11 12
 1  5 18 44 29  3
fps: 22199
8. gsatt:
 8  9 10 11 12 13 15
 5 14 40 28  9  2  2
   \endverbatim
   So well, we can continue to use adaptg2wsat as best. </li>
   <li> For now we consider adaptg2wsat as best (it was determined as best
   for vdw_2^pd(5,8) in VanderWaerdenProblems/plans/5-k/general.hpp. </li>
   <li>  "RunPdVdWk1k2 4 15 adaptg2wsat 100 1000000" yields
   \verbatim
Break point 1: 631
Break point 2: 680
   \endverbatim
   finding all solutions in the third round with the old solution.
   </li>
   <li>  "RunPdVdWk1k2 4 15 adaptg2wsat 100 10000000" yields
   \verbatim
Break point 1: 724
Break point 2: 727
   \endverbatim
   finding all solutions in the ? round.
   </li>

  </ul>

*/
