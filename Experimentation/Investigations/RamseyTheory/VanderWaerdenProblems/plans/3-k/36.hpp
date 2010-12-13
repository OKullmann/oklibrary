// Oliver Kullmann, 13.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/36.hpp
  \brief On investigations into vdw_2(3,36)


  \todo vanderwaerden_2(3,36) > 1174
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,36) = ???. </li>
   <li> Starting search with the best palindromic solution:
   \verbatim
> cat AltExp/Exp_PdVanderWaerden_2-3-36_gsat-tabu-100-20000000_2010-12-02-143818/VanDerWaerden_pd_2-3-36_1174.cnf_sol | PdExtend-O3-DNDEBUG 1174 > solution

> RunVdW3k 36 1174 rots 100 100000000 solution
   \endverbatim
   </li>
  </ul>


  \todo Palindromic version:
  <ul>
   <li> 1174 < vanderwaerden_g([3,36],"pd") =(conj) 1175 =(conj)
   vanderwaerden([3,36]) - ???. </li>
   <li> vanderwaerden_g([3,36],"pdsat") =(conj) 1158. </li>
   <li> Certificates:
    <ol>
     <li> n=1158:
     \verbatim
27,56,58,62,74,91,101,130,132,148,
149,154,161,165,171,187,189,218,228,232,
257,263,270,276,301,305,321,338,346,364,
393,399,428,432,467,469,473,475,486,498,
520,537,569,572
     \endverbatim
     </li>
     <li> n=1174:
     \verbatim
27,32,40,57,62,79,93,110,116,120,
121,145,151,167,168,196,203,232,234,237,
254,283,289,293,320,337,339,342,356,380,
394,396,409,425,431,448,460,483,512,514,
515,522,549,552,571,577
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 36 gsat-tabu 100 20000000" (older version, without also
   using solution for n-1 in the alternation phase) yields
   \verbatim
Break point 1: 1158
Break point 2: 1175
   \endverbatim
   where all solutions were found within in the first 43 runs (so the second
   break-point is rather weak). </li>
   <li> Rerunning it with doubled cutoff, and using the solution for n-1.
   We also need the ability to start late. </li>
  </ul>

*/
