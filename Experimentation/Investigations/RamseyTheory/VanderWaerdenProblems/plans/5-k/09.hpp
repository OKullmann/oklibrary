// Oliver Kullmann, 12.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/09.hpp
  \brief On investigations into vdw_2(5,9)


  \todo Best complete solver for palindromic problems
  <ul>
   <li> t seems that from the conflict-driven solvers minisat-2.2.0 is best,
   from the look-ahead solvers march_pl (followed by OKsolver_2002, then
   satz215), and that overall minisat-2.2.0 is best. </li>
  </ul>


  \todo Best local-search solver for palindromic problems


  \todo vdw_2^pd(5,9) >= (446,451)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=445:
     \verbatim
1,2,4,11,12,20,21,26,31,34,
36,38,42,45,48,49,51,53,58,59,
60,62,65,66,67,69,70,72,73,82,
83,85,86,88,89,90,95,97,98,102,
110,113,119,121,123,124,126,128,129,131,
133,139,142,150,155,157,162,163,164,166,
167,169,170,179,180,182,183,185,186,187,
192,194,199,207,210,216,218,220,221,223
     \endverbatim
     </li>
     <li> n=450:
     \verbatim
2,3,6,9,13,14,15,16,22,25,
27,30,36,39,41,45,50,52,53,56,
57,58,60,61,63,64,65,67,71,72,
77,82,88,94,95,96,97,100,101,103,
109,110,111,114,116,122,126,127,131,132,
133,135,136,145,149,150,155,156,158,159,
162,164,165,166,173,179,180,182,183,184,
185,188,190,193,196,203,204,208,209,211,
217,221,225
     \endverbatim
    </ol>
   </li>
   li> "RunPdVdWk1k2 5 9 adaptg2wsat 100 2000000" yields
   \verbatim
Break point 1: 446
Break point 2: 451
   \endverbatim
   finding all solutions in at most 53 runs.
   </li>
  </ul>

*/
