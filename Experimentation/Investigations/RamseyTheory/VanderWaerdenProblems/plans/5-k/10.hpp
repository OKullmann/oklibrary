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
  <ul>
   <li> It seems that from the conflict-driven solvers minisat-2.2.0 is best,
   from the look-ahead solvers march_pl (followed by OKsolver_2002, then
   satz215), and that overall minisat-2.2.0 is best. </li>
  </ul>


  \todo Best local-search solver for palindromic problems


  \todo vdw_2^pd(5,10) >= (595,612)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=594:
     \verbatim
3,12,17,22,23,28,29,32,39,40,
43,45,46,50,51,52,56,58,59,62,
63,66,67,71,78,79,81,82,86,87,
88,94,97,100,101,103,105,113,118,120,
122,123,124,125,134,135,145,146,152,154,
162,163,172,174,176,177,179,180,186,187,
189,192,193,196,197,201,207,208,210,212,
215,225,227,229,230,235,241,248,250,251,
252,253,255,256,257,261,263,264,266,272,
273,275,276,277,279,282,288,293
     \endverbatim
     </li>
     <li> n=611:
     \verbatim
10,14,15,18,24,27,28,33,40,41,
45,47,48,49,50,53,54,55,58,65,
69,71,72,80,88,89,94,96,97,99,
100,101,103,107,110,113,114,119,121,123,
124,127,129,130,135,139,141,142,143,145,
148,153,162,163,168,170,171,173,177,181,
186,187,188,189,192,193,194,196,197,198,
201,202,206,209,215,216,217,220,227,228,
236,242,244,245,248,249,251,258,260,261,
266,268,270,271,275,276,278,280,287,289,
290,291,293,296,301
     \endverbatim
    </ol>
   </li>
   <li> "RunPdVdWk1k2 5 10 adaptg2wsat 100 20000000" yields
   \verbatim
Break point 1: 595
Break point 2: 612
   \endverbatim
   finding all solutions in the 7th run.
   </li>
  </ul>

*/
