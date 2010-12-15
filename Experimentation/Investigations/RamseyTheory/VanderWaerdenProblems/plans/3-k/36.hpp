// Oliver Kullmann, 13.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/36.hpp
  \brief On investigations into vdw_2(3,36)


  \todo vanderwaerden_2(3,36) > 1214
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,36) = ???. </li>
   <li> Starting search with the best palindromic solution:
   \verbatim
> cat AltExp/Exp_PdVanderWaerden_2-3-36_gsat-tabu-100-20000000_2010-12-02-143818/VanDerWaerden_pd_2-3-36_1174.cnf_sol | PdExtend-O3-DNDEBUG 1174 > solution

> RunVdW3k 36 1174 rots 100 100000000 solution
   \endverbatim
   </li>
   <li> n=1203 found satisfiable with certificate
   \verbatim
34,46,71,72,79,83,85,88,89,101,
126,145,146,148,153,162,177,183,185,194,
214,233,268,273,293,296,301,307,310,311,
344,348,360,368,379,397,399,421,422,442,
473,479,486,495,508,510,527,545,555,560,
589,597,606,626,627,643,644,658,666,681,
695,700,714,737,740,745,749,751,767,782,
792,823,825,829,849,851,856,878,888,922,
923,934,952,954,960,973,996,999,1028,1034,
1047,1063,1065,1073,1082,1088,1100,1125,1156,1161,
1193
   \endverbatim
   </li>
   <li> n=1214 found satisfiable with certificate
   \verbatim
2,28,33,67,80,114,117,125,142,144,
151,155,187,209,213,215,216,218,244,246,
253,265,266,287,289,303,324,336,339,347,
366,394,401,410,438,440,446,447,458,468,
472,505,524,525,551,562,585,595,606,631,
643,659,668,673,694,696,697,717,746,768,
770,773,779,805,808,810,817,820,838,845,
853,857,879,891,902,921,928,932,949,964,
986,1001,1002,1005,1006,1013,1027,1050,1066,1067,
1079,1104,1117,1143,1154,1186,1214
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
