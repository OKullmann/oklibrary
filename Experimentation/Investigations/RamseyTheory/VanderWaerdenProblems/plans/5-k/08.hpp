// Oliver Kullmann, 11.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/08.hpp
  \brief On investigations into vdw_2(5,8)


  \todo Best local search solver
  <ul>
   <li> For k=7 adaptg2wsat and ddfw are best (see
   VanderWaerdenProblems/plans/5-k/07.hpp). </li>
  </ul>


  \todo vdw_2(5,8) > 330
   <li> Certificate for n=330:
   \verbatim
3,4,5,6,9,12,13,14,21,22,
27,32,35,37,39,41,46,48,50,51,
52,53,55,57,62,64,65,68,71,76,
81,85,89,90,91,96,97,99,100,102,
106,108,110,115,120,125,126,127,133,135,
136,138,139,140,143,144,145,146,150,152,
153,154,156,164,167,170,171,173,177,178,
179,180,182,183,184,185,187,188,190,194,
196,197,198,203,211,213,214,215,217,221,
222,223,224,226,227,229,232,233,234,241,
242,247,252,255,258,261,266,270,271,272,
274,275,277,282,283,285,288,291,295,296,
301,305,309,310,311,316,317,318,320,323,
324,325,328,330
   \endverbatim
   or
   \verbatim
2,6,8,10,11,12,13,15,18,22,
27,32,35,37,39,41,45,46,47,51,
52,57,58,62,64,66,71,79,81,82,
83,85,89,90,91,92,94,95,96,97,
99,100,106,108,109,110,112,115,123,126,
133,134,135,136,138,139,140,143,144,146,
152,153,154,159,164,169,170,171,173,177,
179,180,182,183,185,187,188,189,190,194,
197,200,208,211,213,214,215,217,222,224,
226,228,229,231,234,238,240,241,242,244,
252,257,259,261,265,266,267,271,272,273,
276,277,278,284,286,291,296,301,302,303,
305,311,313,314,316,317,320,321,324,325,
327,328,330
   \endverbatim
   </li>
   <li> Using the palindromic solution for n=322:
    <ol>
     <li> Starting search with the best palindromic solution:
     \verbatim
> k1=5 k2=8 n=322 alg="adaptg2wsat" cutoff=1000000 runs=100 expdate=""; export k1 k2 n; cat AltExp/Exp_PdVanderWaerden_2-${k1}-${k2}_${alg}-${runs}-${cutoff}_${expdate}/VanDerWaerden_pd_2-${k1}-${k2}_${n}.cnf_sol | PdExtend-O3-DNDEBUG ${n} > solution

> RunVdWk1k2 ${k1} ${k2} ${n} adaptg2wsat 100 10000000 solution
UNSAT for n=331
> E=read_ubcsat("Exp_VanderWaerden_2-5-8_322_adaptg2wsat-100-10000000_2011-04-13-080441/VanDerWaerden_2-5-8_331.cnf_OUT",nrows=100)
 3  4  5
 5 46 49
100

> RunVdWk1k2 ${k1} ${k2} ${n} ddfw 100 10000000 solution
UNSAT for n=331
> E=read_ubcsat("Exp_VanderWaerden_2-5-8_322_ddfw-100-10000000_2011-04-13-185202/VanDerWaerden_2-5-8_331.cnf_OUT",nrows=100)
 1  2  3  4  5
 3  1  8 53 35
100

> n=330; RunVdWk1k2 ${k1} ${k2} ${n} adaptg2wsat 100 20000000 AltExp/Exp_VanderWaerden_2-5-8_322_ddfw-100-10000000_2011-04-13-185202/VanDerWaerden_2-${k1}-${k2}_${n}.cnf_sol
UNSAT for n=331
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-8_330_adaptg2wsat-100-20000000_2011-04-13-232550/VanDerWaerden_2-5-8_331.cnf_OUT",nrows=100)
 1  2  3  4  5
 2  3 19 60 16
100

> RunVdWk1k2 ${k1} ${k2} ${n} ddfw 100 20000000 AltExp/Exp_VanderWaerden_2-5-8_322_ddfw-100-10000000_2011-04-13-185202/VanDerWaerden_2-${k1}-${k2}_${n}.cnf_sol
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Best complete solver for palindromic problems
  <ul>
   <li> It seems that from the conflict-driven solvers minisat-2.2.0 is best,
   from the look-ahead solvers march_pl (followed by OKsolver_2002, then
   satz215), and that overall minisat-2.2.0 is best. </li>
  </ul>


  \todo Best local-search solver for palindromic problems
  <ul>
   <li> Best ubcsat-algorithm:
   \verbatim
> PdVanderWaerdenCNF-O3-DNDEBUG 5 8 312

> E=run_ubcsat("VanDerWaerden_pd_2-5-8_312.cnf",runs=100,cutoff=1000000)

ag2wsat
 0  1  2
57 36  7
FlipsPerSecond = 208908
paws
 0  1  2  3
52 22 25  1
FlipsPerSecond = 204942
nov
 0  1  2  3
43 20 28  9
FlipsPerSecond = 169408
novp_p
 0  1  2  3  4
39 18 27 15  1
FlipsPerSecond = 129391
novpc
 0  1  2  3  4
36 11 29 23  1
FlipsPerSecond = 206143
anovp
 0  1  2
35 48 17
FlipsPerSecond = 172125
novp
 0  1  2  3
34 16 35 15
FlipsPerSecond = 169951
anovpp
 0  1  2
33 41 26
FlipsPerSecond = 170963
ddfw
 0  1  2
29 59 12
FlipsPerSecond = 89456
dano
 0  1  2
27 44 29
FlipsPerSecond = 169269
g2wsat
 0  1  2  3
27 23 44  6
FlipsPerSecond = 221298
   \endverbatim
   (segmentation faults with saps and sapsnr). So adaptg2wsat seems clearly
   strongest. </li>
  </ul>


  \todo vdw_2^pd(5,8) = (312,323)
  <ul>
   <li> Determined by minisat-2.2.0:
    <ol>
     <li> Hardest problem for n=324:
     \verbatim
restarts              : 16777214
conflicts             : 19006590162    (4124 /sec)
decisions             : 21516030010    (0.00 % random) (4668 /sec)
propagations          : 379970924722   (82439 /sec)
conflict literals     : 508510692844   (22.16 % deleted)
Memory used           : 225.14 MB
CPU time              : 4.60912e+06 s
UNSATISFIABLE
     \endverbatim
     </li>
     <li> While for n=313:
     \verbatim
restarts              : 2097150
conflicts             : 2076757141     (6872 /sec)
decisions             : 2356337872     (0.00 % random) (7798 /sec)
propagations          : 41736971474    (138117 /sec)
conflict literals     : 50113566661    (23.17 % deleted)
Memory used           : 91.68 MB
CPU time              : 302186 s
UNSATISFIABLE
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Certificates:
    <ol>
     <li> n=311:
     \verbatim
1,6,8,9,11,14,16,17,19,22,
26,29,30,32,33,35,40,41,49,50,
55,57,60,61,62,64,68,71,73,74,
75,76,79,80,81,83,84,85,88,89,
90,91,93,96,100,102,104,106,107,109,
114,115,123,124,129,131,132,134,135,136,
138,142,145,147,148,149,150,153,154,155
     \endverbatim
     </li>
     <li> n=322:
     \verbatim
1,2,4,5,6,7,13,14,18,21,
27,32,35,37,39,41,45,46,47,48,
50,51,52,53,55,56,62,64,66,68,
71,76,82,85,89,90,94,96,97,99,
100,101,102,108,109,110,112,120,125,126,
127,129,133,135,136,138,139,140,143,144,
145,150,152,153,154,156,159
     \endverbatim
    </ol>
   </li>
   <li> "RunPdVdWk1k2 5 8 adaptg2wsat 100 1000000" yields
   \verbatim
Break point 1: 306
Break point 2: 323
   \endverbatim
   </li>
   <li> However minisat-2.2.0 determines instances with n <= 312 as
   satisfiable. So this is our first case of a failure of the
   RunPdVdWk1k2-method. Increase the cutoff. </li>
   li> "RunPdVdWk1k2 5 8 adaptg2wsat 100 2000000" yields
   \verbatim
Break point 1: 312
Break point 2: 323
   \endverbatim
   finding all solutions in the first run with the previous solution.
   </li>
  </ul>

*/
