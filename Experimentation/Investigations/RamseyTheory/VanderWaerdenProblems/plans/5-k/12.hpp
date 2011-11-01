// Oliver Kullmann, 20.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/12.hpp
  \brief On investigations into vdw_2(5,12)


  \todo vanderwaerden_2(5,12) > 949
  <ul>
   <li> Find best ubcsat-solver. </li>
   <li> Start with the palindromic solution. </li>
  </ul>


  \todo Best local-search solver for palindromic problems
  <ul>
   <li>
   \verbatim
E=run_ubcsat("VanDerWaerden_pd_2-5-12_949.cnf",runs=100,cutoff=20000000)
# considering only the first 11 algorithm (aborted then):
anovp
 2  3  4  5  6
 2  8 36 49  5
FlipsPerSecond = 54760
XXX
   \endverbatim
   </li>
  </ul>


  \todo vdw_2^pd(5,12) >= (949,950)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=948:
     \verbatim
11,17,18,23,31,32,35,39,41,43,
49,52,54,59,61,65,68,74,77,85,
87,91,93,96,99,102,104,106,107,109,
111,112,119,120,122,124,126,130,133,134,
135,139,142,144,145,156,158,162,165,171,
178,183,184,185,190,192,198,202,203,204,
206,208,209,211,212,213,214,216,227,234,
236,245,247,251,252,253,254,258,259,261,
271,272,273,277,279,286,292,295,297,299,
300,308,309,312,320,324,327,328,334,336,
337,338,347,351,354,359,365,370,371,372,
374,380,383,384,393,402,403,404,405,407,
408,410,411,412,417,419,423,425,426,427,
428,430,431,439,441,446,447,449,451,456,
457,458,459,462,474
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
   <li> "RunPdVdWk1k2 5 12 adaptg2wsat 200 160000000" yields
   \verbatim
Break point 1: 942
Break point 2: 949
   \endverbatim
   using at most 158 runs (for n=929).
   </li>
  </ul>

*/
