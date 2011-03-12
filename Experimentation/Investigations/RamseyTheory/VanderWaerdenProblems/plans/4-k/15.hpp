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


  \todo vanderwaerden_2(4,15) > 679
  <ul>
   <li> Start with the palindromic solution for n=679. </li>
  </ul>


  \todo vdw_2^pd(4,15) >= (631,680)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=630:
     \verbatim
3,7,17,22,35,47,48,50,51,52,
55,60,69,71,76,80,81,94,100,105,
106,110,118,123,127,128,138,141,143,145,
146,149,158,163,166,170,172,174,186,189,
192,197,205,210,218,219,232,233,239,240,
245,256,264,265,266,269,270,271,274,280,
284,289,295,302,303,307,312
     \endverbatim
     </li>
     <li> n=679:
     \verbatim
4,7,21,23,30,38,43,48,51,57,
59,61,71,74,76,82,83,91,96,99,
100,101,116,118,119,123,138,145,146,147,
154,166,173,174,176,188,191,192,196,197,
201,209,210,215,216,221,224,231,235,244,
255,261,262,263,265,271,273,276,277,288,
290,294,298,306,307,310,313,318,320,321,
322,328,332,339,340
     \endverbatim
    </ol>
   </li>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
   <li> For now we consider adaptg2wsat as best (it was determined as best
   for vdw_2^pd(5,8) in VanderWaerdenProblems/plans/5-k/general.hpp. </li>
   <li>  "RunPdVdWk1k2 4 15 adaptg2wsat 100 1000000" yields
   \verbatim
Break point 1: 631
Break point 2: 680
   \endverbatim
   finding all solutions in the third round with the old solution.
   </li>
  </ul>

*/
