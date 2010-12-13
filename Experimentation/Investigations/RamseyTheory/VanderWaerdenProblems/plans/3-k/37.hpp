/ Oliver Kullmann, 13.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/37.hpp
  \brief On investigations into vdw_2(3,37)


  \todo vanderwaerden_2(3,37) > 1294
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,37) = ???. </li>
   <li> Starting search with the best palindromic solution:
   \verbatim
> cat AltExp/Exp_PdVanderWaerden_2-3-37_gsat-tabu-100-20000000_2010-12-02-144008/VanDerWaerden_pd_2-3-37_1294.cnf_sol | PdExtend-O3-DNDEBUG 1294 > solution

> RunVdW3k 37 1294 rots 100 100000000 solution
   \endverbatim
   </li>
  </ul>


  \todo Palindromic version:
  <ul>
   <li> 1294 < vanderwaerden_g([3,37],"pd") =(conj) 1295 =(conj)
   vanderwaerden([3,37]) - ???. </li>
   <li> vanderwaerden_g([3,37],"pdsat") =(conj) 1264. </li>
   <li> Certificates:
    <ol>
     <li> n=1264:
     \verbatim
1,38,69,79,102,106,116,117,134,149,
158,180,187,191,201,217,223,234,256,275,
293,298,302,307,335,345,350,361,375,382,
404,435,449,450,454,460,472,478,486,493,
528,560,567,571,587,589,608,626
     \endverbatim
     </li>
     <li> n=1294:
     \verbatim
8,33,65,72,76,107,109,113,118,141,
176,178,187,220,229,252,255,263,266,270,
294,301,324,335,337,342,352,372,377,381,
389,405,435,442,446,448,451,462,485,514,
527,536,553,562,564,573,596,620,625,638
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 37 gsat-tabu 100 20000000" (older version, without also
   using solution for n-1 in the alternation phase) yields
   \verbatim
Break point 1: 1264
Break point 2: 1295
   \endverbatim
   where all solutions were found within in the first 52 runs (so the second
   break-point is rather weak). </li>
   <li> Rerunning it with doubled cutoff, and using the solution for n-1.
   We also need the ability to start late. </li>
  </ul>

*/
