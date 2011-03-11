// Oliver Kullmann, 11.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/13.hpp
  \brief On investigations into vdw_2(4,13)


  \todo Best ubcsat-solver
  <ul>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
  </ul>


  \todo vanderwaerden_2(4,13) > 537
  <ul>
   <li> Start with the palindromic solution for n=537. </li>
  </ul>


  \todo vdw_2^pd(4,13) >= (519,538)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=518:
     \verbatim
11,13,26,27,28,31,34,38,43,49,
52,65,66,79,82,88,89,93,100,103,
104,105,111,118,120,123,124,128,129,131,
135,137,140,146,153,163,172,176,179,186,
188,190,196,197,200,205,207,211,215,217,
220,221,222,226,228,234,236,237,246,250,
259
     \endverbatim
     </li>
     <li> n=537:
     \verbatim
2,15,19,21,22,26,34,35,38,39,
44,46,59,68,70,72,73,75,77,78,
80,82,91,99,104,106,111,112,115,116,
128,129,131,134,135,136,148,156,159,165,
167,170,175,185,196,201,203,209,210,212,
213,215,216,225,226,229,231,233,238,240,
253,256,262,264,266,267,269
     \endverbatim
    </ol>
   </li>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
   <li> For now we consider adaptg2wsat as best (it was determined as best
   for vdw_2^pd(5,8) in VanderWaerdenProblems/plans/5-k/general.hpp. </li>
   <li>  "RunPdVdWk1k2 4 13 adaptg2wsat 100 1000000" yields
   \verbatim
Break point 1: 519
Break point 2: 538
   \endverbatim
   finding all solutions at the latest in the 6th run with random seed.
   </li>
  </ul>

*/
