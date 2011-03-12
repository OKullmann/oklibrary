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


  \todo vdw_2^pd(6,7) >= (393,594)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=392:
     \verbatim
4,5,6,9,10,13,15,16,18,20,
21,23,24,27,32,33,34,36,37,44,
45,46,49,50,51,52,53,57,58,60,
61,63,64,67,68,72,75,79,82,83,
85,86,88,89,92,93,94,97,98,102,
107,110,111,114,116,118,123,125,127,128,
130,132,135,137,138,141,143,146,147,148,
149,151,152,153,155,159,164,169,174,179,
181,187,188,190,191,192,193,195,196
     \endverbatim
     </li>
     <li> n=593:
     \verbatim
7,14,18,19,20,21,27,30,35,39,
40,41,43,45,46,49,53,55,56,57,
58,60,62,63,64,67,69,70,72,73,
75,78,79,80,82,84,85,86,87,89,
93,96,97,99,101,102,103,107,112,115,
120,121,122,123,124,127,128,131,132,133,
134,135,140,143,148,152,153,154,156,158,
159,162,166,168,169,170,171,173,175,176,
177,180,182,183,184,185,186,188,191,192,
193,195,197,198,199,200,202,206,209,210,
212,214,215,216,220,225,228,233,234,235,
236,237,240,241,244,245,246,247,248,253,
256,261,265,266,267,269,271,272,275,279,
281,282,283,284,286,288,289,290,293,295,
296,297
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
  </ul>

*/
