// Oliver Kullmann, 3.4.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/07.hpp
  \brief On investigations into vdw_2(5,7)


  \todo Best local search solver
  <ul>
   <li> Small cutoff 10^5:
   \verbatim
> E = run_ubcsat("VanDerWaerden_2-5-7_235.cnf", runs=100)
anovp
 2  3  4  5  6  7
 3  5 21 38 27  6
FlipsPerSecond = 101729
ddfw
 2  3  4  5  6
 2 19 42 28  9
FlipsPerSecond = 13702
rnov
 2  3  4  5  6  7
 1 18 20 41 18  2
FlipsPerSecond = 85056
ag2wsat
 2  3  4  5  6  7
 1 16 33 36 12  2
FlipsPerSecond = 82217
rnovp
 2  3  4  5  6
 1  8 43 33 15
FlipsPerSecond = 61660
anovpp
 2  3  4  5  6  7
 1  7 18 41 29  4
FlipsPerSecond = 59400
novpc
 2  5  6  7  8  9 10 11 12 13 14
 1  2  1  7 12 18 21 19 10  7  2
FlipsPerSecond = 58668
dano
 3  4  5  6  7
 9 25 40 22  4
FlipsPerSecond = 142167
novp
 3  5  6  7  8  9 10 11 12 13 14
 2  1  4  4  9 18 25 12 18  6  1
FlipsPerSecond = 120163
rots
 3  4  5  6  7  8  9
 1  2 12 28 40 16  1
FlipsPerSecond = 81294
paws
 4  5  6  7  8  9 10 11
 4  3 10 19 33 24  6  1
FlipsPerSecond = 69147
irots
 4  5  6  7  8  9 10
 2  4 10 18 44 17  5
FlipsPerSecond = 73190
samd
 4  5  6  7  8  9 10 11 13 16
 2  4  7 21 23 18 18  5  1  1
FlipsPerSecond = 98386
saps
 4  5  6  7  8  9 10
 2  3  8 23 31 27  6
FlipsPerSecond = 54139
g2wsat
 4  5  6  7  8  9 10 11
 2  1  7 28 27 25  8  2
FlipsPerSecond = 26948
   \endverbatim
   Segmentation faults with rsaps, sapsnr (csltok). </li>
   <li> Higher cutoff 10^6:
   \verbatim
> E = run_ubcsat("VanDerWaerden_2-5-7_235.cnf", runs=100,cutoff=1000000, include_algs=list("anovp","ddfw","rnov","ag2wsat","rnovp","anovpp","novpc","dano","novp","rots","paws","irots","samd","saps","g2wsat"))

ddfw
 0  2  3  4
 3 34 54  9
FlipsPerSecond = 16350
ag2wsat
 0  2  3  4
 2 19 61 18
FlipsPerSecond = 58708
paws
 0  2  3  4  5  6  7
 1  1  2 12 29 37 18
FlipsPerSecond = 46145
anovp
 1  2  3  4  5
 3 16 47 32  2
FlipsPerSecond = 28585
rnov
 1  2  3  4  5
 1 16 54 26  3
FlipsPerSecond = 74061
anovpp
 1  2  3  4
 1 11 52 36
FlipsPerSecond = 30302
dano
 2  3  4  5
15 45 39  1
FlipsPerSecond = 41675
rnovp
 2  3  4
 7 53 40
FlipsPerSecond = 55746
rots
 2  3  4  5  6
 3  7 25 45 20
FlipsPerSecond = 40712
novp
 2  3  4  5  6  7  8  9 10
 1  7  3 15 22 29 19  3  1
FlipsPerSecond = 29835
novpc
 2  3  4  5  6  7  8  9
 1  1 13 13 19 28 20  5
FlipsPerSecond = 32568
g2wsat
 3  4  5  6  7  8
 4 17 23 37 18  1
FlipsPerSecond = 78934
samd
 3  4  5  6  7  8  9 10
 2  1 14 22 32 19  7  3
FlipsPerSecond = 128813
saps
 3  4  5  6  7  8
 1  4 21 35 38  1
FlipsPerSecond = 28160
irots
 4  5  6  7  8
 9 25 46 19  1
FlipsPerSecond = 36925
   \endverbatim
   So ddfw is best (but slow), followed by adaptg2wsat. The FPS numbers at
   least on csltok are unreliable. </li>
  </ul>


  \todo vdw_2(5,7) > 259
  <ul>
   <li> Certificate for n=259:
   \verbatim
 7,12,18,19,20,21,26,27,28,29,
31,32,33,37,42,44,45,51,52,54,
59,63,64,65,67,68,69,70,72,75,
76,78,79,82,84,85,86,87,89,90,
91,95,100,102,103,109,110,112,117,121,
122,123,125,126,127,128,130,133,134,136,
137,140,142,143,144,145,147,148,149,153,
158,160,161,167,168,170,175,179,180,181,
183,184,185,186,188,191,192,194,195,198,
200,201,202,203,205,206,207,211,218,219,
222,225,226,228,233,238,241,242,243,246,
249,251,256
  \endverbatim
   </li>
   <li> Using the palindromic solution for n=235:
    <ol>
     <li> Starting search with the best palindromic solution:
     \verbatim
> k1=5 k2=7 n=235 alg="gsat-tabu" cutoff=200000 runs=100 expdate="2011-04-03-065559"; export k1 k2 n; cat AltExp/Exp_PdVanderWaerden_2-${k1}-${k2}_${alg}-${runs}-${cutoff}_${expdate}/VanDerWaerden_pd_2-${k1}-${k2}_${n}.cnf_sol | PdExtend-O3-DNDEBUG ${n} > solution

> RunVdWk1k2 ${k1} ${k2} ${n} ddfw 100 2000000 solution
UNSAT for n=252
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-7_235_ddfw-100-2000000_2011-04-03-071213/VanDerWaerden_2-5-7_252.cnf_OUT",nrows=100)
 1  2  3  4  5  6  7  8
 1  4  5  1 13 37 36  3
100

> n=251; RunVdWk1k2 ${k1} ${k2} ${n} adaptg2wsat 100 5000000 AltExp/Exp_VanderWaerden_2-5-7_235_ddfw-100-2000000_2011-04-03-071213/VanDerWaerden_2-5-7_${n}.cnf_sol
UNSAT for n=256
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-7_251_adaptg2wsat-100-5000000_2011-04-03-134354/VanDerWaerden_2-5-7_256.cnf_OUT",nrows=100)
 2  3  4  5  6  7  8  9
 3  7  3  2 23 39 22  1
100

> n=255; RunVdWk1k2 ${k1} ${k2} ${n} ddfw 100 5000000 AltExp/Exp_VanderWaerden_2-5-7_251_adaptg2wsat-100-5000000_2011-04-03-134354/VanDerWaerden_2-5-7_${n}.cnf_sol
UNSAT for n=258
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-7_255_ddfw-100-5000000_2011-04-03-191151/VanDerWaerden_2-5-7_258.cnf_OUT",nrows=100)
 2  3  4  5  6  7  8
 9  2  5  8 18 46 12
100

> n=257; RunVdWk1k2 ${k1} ${k2} ${n} adaptg2wsat 100 10000000 AltExp/Exp_VanderWaerden_2-5-7_255_ddfw-100-5000000_2011-04-03-191151/VanDerWaerden_2-5-7_${n}.cnf_sol
UNSAT for n=258
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-7_257_adaptg2wsat-100-10000000_2011-04-04-150849/VanDerWaerden_2-5-7_258.cnf_OUT",nrows=100)
 2  3  4  5  6  7  8
17  9  3 11 11 33 16
100

> RunVdWk1k2 ${k1} ${k2} ${n} ddfw 100 10000000 AltExp/Exp_VanderWaerden_2-5-7_255_ddfw-100-5000000_2011-04-03-191151/VanDerWaerden_2-5-7_${n}.cnf_sol
UNSAT for n=258
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-7_257_ddfw-100-10000000_2011-04-04-163842/VanDerWaerden_2-5-7_258.cnf_OUT",nrows=100)
 1  2  3  4  5  6  7  8
 1 19  6  3 10 16 42  3
100

> RunVdWk1k2 ${k1} ${k2} ${n} adaptg2wsat 200 20000000 AltExp/Exp_VanderWaerden_2-5-7_255_ddfw-100-5000000_2011-04-03-191151/VanDerWaerden_2-5-7_${n}.cnf_sol
UNSAT for n=259
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-7_257_adaptg2wsat-200-20000000_2011-04-05-014108/VanDerWaerden_2-5-7_259.cnf_OUT",nrows=100)
 2  3  4  5  6  7  8
19 15  8 15  4 33  6
100

> n=258; RunVdWk1k2 ${k1} ${k2} ${n} ddfw 200 20000000 AltExp/Exp_VanderWaerden_2-5-7_257_adaptg2wsat-200-20000000_2011-04-05-014108/VanDerWaerden_2-5-7_${n}.cnf_sol
UNSAT for n=260
> E=read_ubcsat("AltExp/Exp_VanderWaerden_2-5-7_258_ddfw-200-20000000_2011-04-05-101330/VanDerWaerden_2-5-7_260.cnf_OUT",nrows=200)
 1  2  3  4  5  6  7  8
 3 31 49 15 31 20 47  4
200

> n=259; RunVdWk1k2 ${k1} ${k2} ${n} adaptg2wsat 200 40000000 AltExp/Exp_VanderWaerden_2-5-7_258_ddfw-200-20000000_2011-04-05-101330/VanDerWaerden_2-5-7_${n}.cnf_sol
UNSAT for n=260
> E=read_ubcsat("Exp_VanderWaerden_2-5-7_259_adaptg2wsat-200-40000000_2011-04-06-104600/VanDerWaerden_2-5-7_260.cnf_OUT",nrows=200)
 1  2  3  4  5  6  7  8
 4 41 71 18 23 17 25  1
200

> RunVdWk1k2 ${k1} ${k2} ${n} ddfw 200 40000000 AltExp/Exp_VanderWaerden_2-5-7_258_ddfw-200-20000000_2011-04-05-101330/VanDerWaerden_2-5-7_${n}.cnf_sol

     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo vdw_2^pd(5,7) = (227,236)
  <ul>
   <li> Determined relatively easily by minisat-2.2.0. </li>
   <li> Interestingly, unsatisfiability for n=237 was very easy (10s,
   267,783 conflicts), but unsatisfiability for n=228 was rather hard (40m,
   48,363,829 conflicts). </li>
   <li> Certificates:
    <ol>
     <li> n=226:
     \verbatim
2,5,8,9,13,16,17,18,19,25,
26,29,30,31,34,40,43,44,46,47,
48,49,51,53,54,60,65,67,68,69,
72,76,78,79,80,81,88,90,91,92,
96,97,98,99,102,103,107,111
     \endverbatim
     </li>
     <li> n=235:
     \verbatim
3,6,8,13,17,18,19,26,30,31,
32,33,38,39,40,41,43,44,45,49,
54,56,57,63,64,66,71,75,76,77,
79,80,81,82,84,87,88,90,91,94,
96,97,98,99,101,102,103,107,112,114,
115
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdWk1k2 5 7 gsat-tabu 10 100000" yields
   \verbatim
Break point 1: 221
Break point 2: 236
   \endverbatim
   </li>
   <li> "RunPdVdWk1k2 5 7 gsat-tabu 10 200000" yields
   \verbatim
Break point 1: 210
Break point 2: 225
   \endverbatim
   </li>
   <li> "RunPdVdWk1k2 5 7 gsat-tabu 100 200000" yields
   \verbatim
Break point 1: 227
Break point 2: 236
   \endverbatim
   </li>
  </ul>

*/
