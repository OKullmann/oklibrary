// Oliver Kullmann, 6.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/34.hpp
  \brief On investigations into vdw_2(3,34)


  \todo vanderwaerden_2(3,34) > 1079
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,34) = ???. </li>
   <li> Starting search with the best palindromic solution:
   \verbatim
> cat AltExp/Exp_PdVanderWaerden_2-3-34_gsat-tabu-100-10000000_2010-11-30-182015/VanDerWaerden_pd_2-3-34_1078.cnf_sol | PdExtend-O3-DNDEBUG 1078 > solution

> RunVdW3k 34 1078 rots 100 100000000 solution

   \endverbatim
   </li>
   <li> n=1079: Solution was adopted, with certificate
   \verbatim
23,24,34,57,71,97,98,120,129,145,
157,176,177,182,203,205,208,214,236,245,
250,274,279,288,310,321,342,347,348,351,
379,390,395,398,425,427,453,462,472,490,
495,496,509,531,532,536,543,547,548,570,
583,584,589,607,617,626,652,654,681,684,
689,700,728,731,732,737,758,769,791,800,
805,829,834,843,865,871,874,876,897,902,
903,922,934,950,959,981,982,1008,1022,1045,
1055,1056
   \endverbatim
   </li>
  </ul>


  \todo Palindromic version:
  <ul>
   <li> 1078 < vanderwaerden_g([3,34],"pd") =(conj) 1079 =(conj)
   vanderwaerden([3,34]) - ???. </li>
   <li> vanderwaerden_g([3,34],"pdsat") =(conj) 1046. </li>
   <li> "RunPdVdW3k 34 gsat-tabu 100 10000000" (old version) yields
   \verbatim
Break point 1: 1046
Break point 2: 1079
   \endverbatim
   where all solutions were found within in the first 73 runs (so the second
   break-point is rather weak). </li>
   <li> Rerunning it with doubled cutoff, and using the previous solutions
   properly. We also need the ability to start late. </li>
  </ul>

*/
