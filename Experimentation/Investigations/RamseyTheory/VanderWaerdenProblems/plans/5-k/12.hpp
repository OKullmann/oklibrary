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


  \todo vdw_2^pd(5,12) >= (914,937)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=913:
     \verbatim
3,9,13,24,30,32,35,38,43,53,
56,60,61,68,69,70,73,77,79,86,
87,89,90,92,93,95,102,103,108,109,
115,122,123,124,134,135,139,140,142,144,
145,146,147,150,152,160,167,169,170,171,
174,176,178,181,182,183,187,190,202,204,
209,214,221,224,225,231,243,247,248,250,
251,254,258,260,261,262,263,267,271,273,
275,276,277,285,291,296,301,303,304,307,
309,310,316,321,324,325,326,328,330,334,
336,337,344,345,347,349,350,355,358,359,
361,362,364,365,374,375,376,380,382,390,
392,396,399,400,403,408,411,412,413,417,
419,422,423,434,435,436,443,445,452
     \endverbatim
     </li>
     <li> n=936:
     \verbatim
10,11,12,14,15,17,18,27,30,36,
43,44,45,57,59,60,63,74,76,80,
82,83,86,88,90,91,92,97,99,100,
101,112,119,123,126,127,129,130,131,133,
137,141,147,148,149,150,152,154,160,161,
165,173,179,184,186,187,189,191,193,204,
206,208,211,212,213,215,216,218,224,228,
235,240,249,251,254,257,258,261,262,264,
266,273,276,285,291,295,296,301,304,310,
313,317,319,325,327,328,330,331,332,333,
336,342,345,350,352,353,356,357,358,361,
363,367,369,373,374,382,383,386,390,395,
397,400,405,413,419,421,422,427,428,431,
434,435,436,442,443,444,446,454,455,463,
465
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
  </ul>

*/
