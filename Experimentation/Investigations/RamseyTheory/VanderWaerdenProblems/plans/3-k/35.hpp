// Oliver Kullmann, 6.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/35.hpp
  \brief On investigations into vdw_2(3,35)


  \todo vanderwaerden_2(3,35) > 1142
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,35) = ???. </li>
   <li> Starting search with the best palindromic solution:
   \verbatim
> cat AltExp/Exp_PdVanderWaerden_2-3-35_gsat-tabu-100-10000000_2010-11-30-182053/VanDerWaerden_pd_2-3-35_1142.cnf_sol | PdExtend-O3-DNDEBUG 1142 > solution

> RunVdW3k 35 1142 rots 100 100000000 solution

   \endverbatim
   </li>
  </ul>


  \todo Palindromic version:
  <ul>
   <li> 1142 < vanderwaerden_g([3,35],"pd") =(conj) 1143 =(conj)
   vanderwaerden([3,35]) - ???. </li>
   <li> vanderwaerden_g([3,35],"pdsat") =(conj) 1102. </li>
   <li> "RunPdVdW3k 35 gsat-tabu 100 10000000" (old version) yields
   \verbatim
Break point 1: 1102
Break point 2: 1143
   \endverbatim
   where all solutions were found within in the first 71 runs (so the second
   break-point is rather weak). </li>
   <li> Rerunning it with doubled cutoff, and using the previous solutions
   properly. We also need the ability to start late. </li>
  </ul>

*/
