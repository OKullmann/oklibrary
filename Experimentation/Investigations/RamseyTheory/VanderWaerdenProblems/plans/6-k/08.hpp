// Oliver Kullmann, 14.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/6-k/08.hpp
  \brief On investigations into vdw_2(6,8)


  \todo Best local-search solver for palindromic problems
  <ul>
   <li>
   \verbatim
> E=run_ubcsat("VanDerWaerden_pd_2-6-8_1167.cnf",runs=100,cutoff=2000000)
XXX
   \endverbatim
   </li>
  </ul>


  \todo vdw_2^pd(6,8) >= (655,1168)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=654:
     \verbatim
2,3,6,9,12,19,21,25,26,27,
33,36,40,41,42,44,45,48,51,55,
57,62,66,68,71,72,74,75,76,79,
80,82,83,85,90,91,92,100,103,110,
111,112,113,115,116,118,119,120,121,123,
124,125,129,131,134,135,137,138,139,141,
148,152,153,155,163,164,168,170,175,177,
178,179,181,182,185,187,191,192,193,195,
196,197,198,200,201,203,204,205,206,212,
213,216,217,223,224,225,226,228,229,231,
232,233,234,236,237,238,242,244,247,248,
250,251,252,254,259,261,265,266,268,274,
276,277,281,283,288,290,291,292,294,295,
298,300,304,305,306,308,309,310,311,313,
314,316,317,318,319,325,326
     \endverbatim
     </li>
     <li> n=1167:
     \verbatim
4,12,13,20,28,29,30,31,32,39,
43,44,50,53,57,59,60,61,65,67,
68,70,72,73,76,77,80,82,85,88,
89,93,95,96,97,99,100,102,103,104,
105,106,109,110,111,113,115,116,124,129,
130,134,135,136,137,140,146,148,150,151,
153,154,155,158,159,161,162,164,165,167,
168,169,171,175,176,177,179,182,184,187,
188,191,192,194,196,197,199,203,204,205,
207,211,214,220,221,225,230,232,233,234,
235,236,238,239,244,246,251,252,254,255,
256,257,258,260,265,269,270,276,279,283,
285,286,287,291,293,294,296,298,299,302,
303,306,308,311,313,314,315,319,321,322,
323,325,326,328,329,330,331,332,335,336,
337,339,340,341,342,344,350,353,354,355,
356,358,360,361,362,363,366,372,374,375,
376,377,379,380,381,384,385,386,387,388,
390,391,393,394,395,397,401,402,403,405,
408,410,413,414,417,418,420,422,423,425,
429,430,431,433,437,440,446,447,451,456,
458,459,460,461,462,464,465,470,472,477,
478,480,481,482,483,484,486,491,495,496,
502,505,509,511,512,513,517,519,520,522,
524,525,528,529,532,534,537,539,540,541,
545,547,548,549,551,552,554,555,556,557,
558,561,562,563,565,566,567,568,570,576,
579,580,581,582
     \endverbatim
    </ol>
   </li>
   <li> "RunPdVdWk1k2 6 8 adaptg2wsat 100 40000000" yields
   \verbatim
   \endverbatim
Break point 1: 655
Break point 2: 1168
   </li>
   <li> "RunPdVdWk1k2 6 8 adaptg2wsat 100 80000000" yields also
   \verbatim
   \endverbatim
Break point 1: 655
Break point 2: 1168
   using at most 28 runs. </li>
   <li> Hardest problems apparently at around 500 < n < 670. Before *and*
   after always solving it in the first run with previous solution, just a few
   changes needing. </li>
  </ul>

*/
