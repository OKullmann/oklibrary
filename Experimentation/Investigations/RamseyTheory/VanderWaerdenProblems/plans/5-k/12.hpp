// Oliver Kullmann, 20.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/12.hpp
  \brief On investigations into vdw_2(5,12)


  \todo Best complete solver for palindromic problems


  \todo Best local-search solver for palindromic problems


  \todo vdw_2^pd(5,12) >= (943,950)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=942:
     \verbatim
3,12,19,25,26,34,38,42,49,60,
63,66,67,69,70,71,76,79,83,84,
89,91,92,94,95,96,101,103,108,110,
113,114,116,121,126,129,130,131,132,137,
140,144,155,160,167,168,169,170,173,175,
176,182,186,187,190,191,192,195,197,198,
200,203,215,216,217,224,229,236,243,245,
246,247,249,252,256,258,261,268,270,271,
274,276,278,289,291,292,294,297,300,301,
302,307,308,310,311,314,316,319,325,333,
334,336,338,345,349,351,359,361,362,363,
364,366,368,374,376,383,385,387,389,394,
401,405,415,418,419,421,423,424,425,426,
428,429,430,431,433,441,442,445,446,455,
458,468,469
     \endverbatim
     </li>
     <li> n=949:
     \verbatim
1,2,6,8,10,14,17,24,31,36,
45,53,54,57,58,60,67,70,79,80,
84,85,89,90,91,92,96,101,106,107,
110,113,116,120,121,123,124,129,130,133,
136,137,138,139,146,153,158,164,165,171,
174,175,178,183,187,188,189,191,197,198,
200,202,203,213,222,223,224,228,231,233,
234,236,238,244,245,246,248,251,255,256,
259,265,268,269,272,273,276,277,278,280,
291,294,306,307,311,316,319,320,325,327,
331,333,336,337,341,342,345,351,353,356,
357,359,367,370,372,376,377,379,388,389,
391,399,400,404,409,410,418,422,424,428,
430,431,432,435,437,438,441,450,452,453,
454,456,462,463,465,470
     \endverbatim
    </ol>
   </li>
   <li> "RunPdVdWk1k2 5 12 adaptg2wsat 100 40000000" yields
   \verbatim
Break point 1: 914
Break point 2: 937
   \endverbatim
   using at most 49 runs.
   </li>
   <li> "RunPdVdWk1k2 5 12 adaptg2wsat 100 80000000" yields
   \verbatim
Break point 1: 943
Break point 2: 950
   \endverbatim
   using at most 97 runs.
   </li>
  </ul>

*/
