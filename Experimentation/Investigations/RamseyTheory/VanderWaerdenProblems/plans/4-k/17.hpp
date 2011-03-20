// Oliver Kullmann, 18.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/17.hpp
  \brief On investigations into vdw_2(4,17)


  \todo Best ubcsat-solver
  <ul>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
  </ul>


  \todo vanderwaerden_2(4,17) > ?
  <ul>
   <li> Start with the palindromic solution for n=?. </li>
  </ul>


  \todo vdw_2^pd(4,17) >= (827,1076)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=826:
     \verbatim
7,15,16,27,42,44,49,51,52,54,
55,60,62,65,71,73,74,77,90,91,
95,96,105,110,121,131,137,138,144,154,
156,160,161,162,164,165,173,179,183,194,
196,202,203,204,208,223,224,232,233,245,
246,249,251,253,260,265,276,286,290,297,
305,313,319,324,336,347,358,363,366,367,
368,372,374,375,376,379,381,383,384,394,
396,409
     \endverbatim
     </li>
     <li> n=1075:
     \verbatim
3,8,25,26,31,41,53,54,61,72,
76,83,86,87,99,100,103,105,108,120,
123,128,130,131,132,142,149,151,158,169,
170,172,177,178,180,183,184,192,195,196,
200,201,213,214,219,227,229,239,246,247,
248,255,259,266,267,269,274,275,277,289,
292,294,297,298,310,311,314,322,325,332,
336,343,356,362,364,372,377,378,389,390,
391,395,396,399,407,408,411,413,414,419,
421,422,429,442,449,459,468,474,475,483,
487,488,493,496,508,510,511,516,518,519,
520,526,530,537,538
     \endverbatim
    </ol>
   </li>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
   <li> For now we consider adaptg2wsat as best (it was determined as best
   for vdw_2^pd(5,8) in VanderWaerdenProblems/plans/5-k/general.hpp. </li>
   <li>  "RunPdVdWk1k2 4 17 adaptg2wsat 10 20000000" yields
   \verbatim
Break point 1: 827
Break point 2: 1076
   \endverbatim
   finding all solutions within the first 9 rounds.
   </li>
  </ul>

*/
