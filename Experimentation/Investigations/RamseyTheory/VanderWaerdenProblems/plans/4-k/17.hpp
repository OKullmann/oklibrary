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


  \todo vanderwaerden_2(4,17) > 1075
  <ul>
   <li> Start with the palindromic solution for n=1075. </li>
  </ul>


  \todo vdw_2^pd(4,17) >= (845,1076)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=844:
     \verbatim
8,19,26,30,34,44,46,49,50,51,
53,54,57,66,75,77,83,88,102,111,
114,120,126,128,131,133,137,141,148,151,
152,158,162,169,172,185,195,202,209,211,
217,219,220,234,238,240,243,244,245,249,
255,257,259,260,266,269,273,283,292,296,
299,306,310,316,320,321,325,327,340,345,
348,354,356,357,363,379,380,389,393,402,
405,407,411,413,414,417,418,419,422,424,
428
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
     or (very similar)
     \verbatim
3,8,25,26,31,33,34,41,54,61,
71,72,83,86,87,99,100,103,105,108,
120,122,123,130,131,132,149,151,158,169,
170,172,177,178,180,183,184,192,195,196,
200,201,213,214,219,227,229,239,246,248,
255,259,266,267,269,274,275,277,289,292,
294,297,298,310,311,314,322,325,332,336,
343,344,356,362,364,372,377,378,389,390,
391,395,396,399,407,408,411,413,414,419,
421,422,429,442,449,459,468,474,475,483,
487,488,493,496,508,510,511,516,518,519,
520,526,530,537,538
     \endverbatim
     </li>
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
   <li>  "RunPdVdWk1k2 4 17 adaptg2wsat 10 20000000" yields
   \verbatim
Break point 1: 845
Break point 2: 1076
   \endverbatim
   finding all solutions within the first 37 rounds.
   </li>
  </ul>

*/
