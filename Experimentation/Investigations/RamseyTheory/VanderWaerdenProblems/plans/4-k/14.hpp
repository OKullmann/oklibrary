// Oliver Kullmann, 11.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/14.hpp
  \brief On investigations into vdw_2(4,14)


  \todo Best ubcsat-solver
  <ul>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
  </ul>


  \todo vanderwaerden_2(4,14) > 623
  <ul>
   <li> Start with the palindromic solution for n=623. </li>
  </ul>


  \todo vdw_2^pd(4,14) >= (611,624)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=610:
     \verbatim
1,15,24,31,38,40,42,46,48,49,
52,53,54,57,63,67,69,72,84,86,
88,89,95,98,102,107,121,125,128,134,
135,137,139,150,151,154,156,166,169,170,
171,174,175,177,181,183,185,192,199,208,
209,222,231,232,234,236,240,242,243,246,
248,253,257,261,263,267,268,271,272,274,
283,289,292,296,305
     \endverbatim
     </li>
     <li> n=623:
     \verbatim
7,14,18,19,24,34,37,45,50,52,
57,58,59,61,64,65,75,77,78,82,
87,94,105,113,115,116,118,120,123,125,
131,142,149,154,158,159,161,171,172,175,
177,178,179,184,186,191,199,202,208,210,
217,218,231,239,244,246,252,253,256,258,
259,268,271,272,274,275,276,283,296,299,
305,307,309,310,312
     \endverbatim
    </ol>
   </li>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
   <li> For now we consider adaptg2wsat as best (it was determined as best
   for vdw_2^pd(5,8) in VanderWaerdenProblems/plans/5-k/general.hpp. </li>
   <li>  "RunPdVdWk1k2 4 14 adaptg2wsat 100 1000000" yields
   \verbatim
Break point 1: 611
Break point 2: 624
   \endverbatim
   finding all solutions in the first round with the old solution.
   </li>
  </ul>

*/
