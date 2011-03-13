// Oliver Kullmann, 12.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/6-k/07.hpp
  \brief On investigations into vdw_2(6,7)


  \todo Best complete solver for palindromic problems


  \todo Best local-search solver for palindromic problems
  <ul>
   <li> Best ubcsat-algorithm:
   \verbatim
> PdVanderWaerdenCNF-O3-DNDEBUG 6 7 1155

> E=run_ubcsat("VanDerWaerden_pd_2-6-7_1155.cnf",runs=100,cutoff=1000000)

   \endverbatim
   </li>
  </ul>


  \todo vdw_2^pd(6,7) >= (587,1156)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=586:
     \verbatim
2,5,8,11,16,21,28,30,31,32,
34,35,38,44,45,46,48,49,50,51,
53,54,56,57,58,59,65,66,69,70,
76,77,78,79,81,82,84,85,86,87,
89,90,91,95,97,100,101,103,104,105,
107,112,114,118,119,121,124,127,129,130,
134,136,141,143,144,145,147,148,151,153,
157,158,159,161,162,163,164,166,167,169,
170,171,172,178,179,182,183,189,190,191,
192,194,195,197,198,199,200,202,203,204,
208,210,213,214,216,217,218,220,225,227,
231,232,234,240,242,243,247,249,254,256,
257,258,260,261,264,266,270,271,272,274,
275,276,277,279,280,282,283,284,285,291,
292
     \endverbatim
     </li>
     <li> n=1155:
     \verbatim
7,12,13,20,22,23,24,25,26,28,
33,37,38,44,47,51,53,54,55,59,
61,62,64,66,67,70,71,74,76,79,
81,82,83,87,89,90,91,93,94,96,
97,98,99,100,103,104,105,107,108,109,
110,112,118,121,122,123,124,126,128,129,
130,131,134,140,142,143,144,145,147,148,
149,152,153,154,155,156,158,159,161,162,
163,165,169,170,171,173,176,178,181,182,
185,186,188,190,191,193,197,198,199,201,
205,208,214,215,219,224,226,227,228,229,
230,232,233,238,239,240,245,246,248,249,
250,251,252,254,259,263,264,270,273,277,
279,280,281,285,287,288,290,292,293,296,
297,300,302,305,307,308,309,313,315,316,
317,319,320,322,323,324,325,326,329,330,
331,333,334,335,336,338,344,347,348,349,
350,352,354,355,356,357,360,366,368,369,
370,371,373,374,375,378,379,380,381,382,
384,385,387,388,389,391,395,396,397,399,
402,404,407,408,411,412,414,416,417,419,
423,424,425,427,431,434,440,441,445,450,
452,453,454,455,456,458,459,464,466,471,
472,474,475,476,477,478,480,485,489,490,
496,499,503,505,506,507,511,513,514,516,
518,519,522,523,526,528,531,533,534,535,
539,541,542,543,545,546,548,549,550,551,
552,555,556,557,559,560,561,562,564,570,
573,574,575,576,578
     \endverbatim
    </ol>
   </li>
   <li> "RunPdVdWk1k2 6 7 adaptg2wsat 100 4000000" yields
   \verbatim
Break point 1: 393
Break point 2: 594
   \endverbatim
   using at most ?? runs.
   </li>
   <li> "RunPdVdWk1k2 6 7 adaptg2wsat 100 20000000" yields
   \verbatim
Break point 1: 587
Break point 2: 1156
   \endverbatim
   using at most ?? runs.
   </li>
  </ul>

*/
