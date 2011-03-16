// Oliver Kullmann, 14.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/10.hpp
  \brief On investigations into vdw_2(5,10)


  \todo Best complete solver for palindromic problems


  \todo Best local-search solver for palindromic problems


  \todo vdw_2^pd(5,11) >= (740,757)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=739:
     \verbatim
2,8,10,19,22,23,26,27,37,41,
44,45,54,56,62,66,67,72,73,75,
78,79,81,82,87,94,95,96,99,100,
102,103,105,106,109,115,118,120,122,129,
138,145,146,147,149,152,153,154,155,160,
161,162,169,172,173,175,178,182,183,190,
191,193,201,204,206,210,211,212,215,216,
224,225,228,230,232,233,236,237,239,243,
248,249,251,255,259,264,270,272,275,276,
278,282,284,287,289,292,293,299,300,302,
303,304,307,310,313,322,330,331,332,334,
343,349,352,355,359,361,363,365,366,369
     \endverbatim
     </li>
     <li> n=756:
     \verbatim
4,8,11,18,28,31,32,41,43,44,
45,53,57,62,64,65,67,68,69,72,
75,76,78,82,83,88,99,100,102,104,
105,109,111,114,122,123,128,129,132,137,
138,139,140,144,147,148,150,153,160,161,
163,164,167,175,181,185,186,190,199,206,
208,212,215,218,219,223,225,231,232,235,
237,241,242,244,245,246,251,254,256,257,
264,265,266,272,273,282,283,286,287,290,
295,297,299,300,304,305,306,313,316,317,
319,320,323,328,329,330,336,337,346,350,
351,361,366,367,371,372,373,377
     \endverbatim
    </ol>
   </li>
   <li> "RunPdVdWk1k2 5 11 adaptg2wsat 100 20000000" yields
   \verbatim
Break point 1: 740
Break point 2: 757
   \endverbatim
   </li>
  </ul>

*/
