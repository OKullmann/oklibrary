// Oliver Kullmann, 6.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/35.hpp
  \brief On investigations into vdw_2(3,35)


  \todo vanderwaerden_2(3,35) > 1165
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,35) = ???. </li>
   <li> Starting search with the best palindromic solution:
   \verbatim
> k=35 n=1142 cutoff=10000000 expdate="2010-11-30-182053"; export k n; cat AltExp/Exp_PdVanderWaerden_2-3-${k}_gsat-tabu-100-${cutoff}_${expdate}/VanDerWaerden_pd_2-3-${k}_${n}.cnf_sol | PdExtend-O3-DNDEBUG ${n} > solution

> RunVdW3k ${k} ${n} rots 100 100000000 solution

   \endverbatim
   </li>
   <li> n=1165 found satisfiable (rots, cutoff=10^8, adopted from n=1148,
   where a solution was found in run 9, osteps=8936284) with certificate
   \verbatim
21,36,65,66,82,103,105,132,134,142,
147,151,157,176,184,206,225,251,264,268,
282,290,301,319,325,341,354,364,393,412,
428,435,443,447,452,484,486,512,517,526,
527,539,558,564,583,595,620,621,634,665,
674,675,689,695,697,712,732,748,761,763,
768,771,776,785,798,808,819,842,854,859,
874,879,880,893,911,924,930,934,954,956,
961,993,1002,1008,1020,1022,1027,1045,1067,1076,
1081,1102,1119,1131
   \endverbatim
   </li>
   <li> n=1166 (rots, cutoff=10^8):
   \verbatim
 1  2  3  4  8  9 11
23 59 12  2  2  1  1
100
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
