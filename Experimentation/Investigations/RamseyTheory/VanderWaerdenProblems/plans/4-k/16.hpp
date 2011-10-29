// Oliver Kullmann, 11.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/16.hpp
  \brief On investigations into vdw_2(4,16)


  \todo Best ubcsat-solver
  <ul>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
  </ul>


  \todo vanderwaerden_2(4,16) > 812
  <ul>
   <li> Start with the palindromic solution for n=812. </li>
  </ul>


  \todo vdw_2^pd(4,16) >= (812,813)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=811:
     \verbatim
1,9,12,17,27,28,41,53,57,58,
67,76,80,89,90,92,104,105,106,109,
110,111,115,119,121,130,135,136,138,140,
144,147,160,164,177,186,187,189,191,195,
197,198,201,203,208,218,222,226,227,233,
238,244,251,260,261,270,274,277,288,294,
299,303,313,318,320,323,326,332,334,335,
344,357,367,374,380,381,383,385,389,391,
392,395,396,397,400,402,406
     \endverbatim
     </li>
     <li> n=812:
     \verbatim
13,18,19,24,38,41,44,50,51,67,
71,73,76,77,78,82,88,99,106,115,
116,129,138,139,141,147,149,164,168,170,
173,174,175,178,179,185,189,196,199,203,
213,222,226,229,236,240,246,247,250,251,
252,255,257,261,275,278,282,284,286,287,
296,309,319,326,332,335,337,343,347,348,
352,354,362,364,367,368,369,372,375,381,
383,384,390,393,397,406
     \endverbatim
    </ol>
   </li>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite.
   \verbatim
> E=run_ubcsat("VanDerWaerden_pd_2-4-16_812.cnf",runs=100,cutoff=10000000)
XXX
   \endverbatim
   </li>
   <li> For now we consider adaptg2wsat as best (it was determined as best
   for vdw_2^pd(5,8) in VanderWaerdenProblems/plans/5-k/general.hpp. </li>
   <li>  "RunPdVdWk1k2 4 16 adaptg2wsat 100 1000000" yields
   \verbatim
Break point 1: 688
Break point 2: 719
   \endverbatim
   finding all solutions in the 78th round with the random seed.
   </li>
   <li> Running adaptg2wsat for n=689 with cutoff=10^7 finds a satisfying
   assignment in run 9 (osteps=7766573). </li>
   <li>  "RunPdVdWk1k2 4 16 adaptg2wsat 100 10000000" yields
   \verbatim
Break point 1: 812
Break point 2: 813
   \endverbatim
   finding all solutions in the 72th round with the random seed (for n=695).
   </li>
   <li> Interesting that apparently here the hard problems have n around 700.
   </li>
  </ul>

*/
