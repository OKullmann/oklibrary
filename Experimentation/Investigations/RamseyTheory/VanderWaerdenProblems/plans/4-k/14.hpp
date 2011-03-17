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


  \todo vanderwaerden_2(4,14) > 681
  <ul>
   <li> Start with the palindromic solution for n=681. </li>
  </ul>


  \todo vdw_2^pd(4,14) >= (617,682)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=616:
     \verbatim
6,7,12,22,35,37,42,48,50,59,
68,69,71,73,74,79,82,95,102,103,
104,106,109,119,120,122,123,126,127,134,
147,150,156,158,160,161,163,165,170,176,
179,187,192,194,207,217,219,220,222,223,
224,229,236,244,253,255,257,262,263,265,
273,284,289,291,296,297,298,300,301,303,
304
     \endverbatim
     </li>
     <li> n=681:
     \verbatim
1,11,24,27,33,40,41,43,44,54,
61,65,71,75,82,88,89,93,98,112,
115,121,122,124,126,132,133,138,143,147,
157,158,161,162,168,170,172,173,179,182,
186,196,205,212,219,223,227,229,230,233,
238,240,250,253,254,261,267,270,279,292,
293,302,306,309,315,316,318,320,324,326,
327,330,331,332,335,337,341
     \endverbatim
    </ol>
   </li>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
   <li> For now we consider adaptg2wsat as best (it was determined as best
   for vdw_2^pd(5,8) in VanderWaerdenProblems/plans/5-k/general.hpp. </li>
   <li> "RunPdVdWk1k2 4 14 adaptg2wsat 100 1000000" yields
   \verbatim
Break point 1: 611
Break point 2: 624
   \endverbatim
   </li>
   <li> "RunPdVdWk1k2 4 14 adaptg2wsat 100 10000000" yields
   \verbatim
Break point 1: 617
Break point 2: 682
   \endverbatim
   finding all solutions within the first 31 runs. </li>
  </ul>

*/
