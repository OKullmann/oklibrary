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


  \todo vdw_2^pd(4,16) >= (824,839)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=823:
     \verbatim
5,8,11,17,32,37,40,48,52,53,
58,64,67,77,81,84,85,92,105,114,
116,118,119,128,134,137,145,150,159,162,
164,174,177,178,180,181,182,187,194,205,
211,213,215,216,220,221,223,234,247,249,
254,255,256,258,259,262,272,274,275,277,
284,286,291,299,315,317,318,320,322,328,
339,351,352,353,355,356,358,359,368,369,
375,376,381,383,396,399,405,407,409,410,
412
     \endverbatim
     </li>
     <li> n=838:
     \verbatim
7,10,14,24,26,36,37,40,42,56,
64,67,77,78,80,82,85,93,104,111,
116,118,120,121,124,134,136,137,139,140,
148,153,161,170,174,179,182,184,190,205,
206,208,213,214,215,220,221,233,234,237,
238,245,250,261,267,269,271,272,276,277,
281,287,290,298,303,311,314,315,317,327,
333,335,340,355,358,364,366,369,371,374,
376,387,395,400,402,407,408,409,411,412,
414,415
     \endverbatim
    </ol>
   </li>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite.
   \verbatim
> E=run_ubcsat("VanDerWaerden_pd_2-4-16_812.cnf",runs=100,cutoff=10000000)
> eval_ubcsat_dataframe(E,FALSE)
1. ddfw:
 0  6  7  8  9 10
 1  1  6 14 41 37
fps: 20432
2. dano:
 5  6  7  8  9 10 11
 1  2  7 24 52 13  1
fps: 61096
3. anovpp:
 6  7  8  9 10 11
 3 10 22 48 15  2
fps: 61088
4. rnovp:
 6  7  8  9 10 11 12 13 14
 3  2  7  9 25 32 16  5  1
fps: 54588
5. anovp:
 6  7  8  9 10
 2  9 26 41 22
fps: 54456
6. gsatt:
 7  8  9 10 11 12 13 14 15
 5 15 29 23 15  7  3  1  2
fps: 189268
7. samd:
 7  8  9 10 11 12 13 14 17 19
 4 14 31 24 14  5  5  1  1  1
fps: 190657
8. ag2wsat:
 7  8  9 10 11 12
 4 12 24 40 19  1
fps: 118855
9. rnov:
 7  8  9 10 11 12 13
 2  7 11 26 32 16  6
fps: 54459
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
   <li> "RunPdVdWk1k2 4 16 ddfw 100 20000000"
   \verbatim
Break point 1: 824
Break point 2: 839
> cat Statistics
    719     7   12170461 1505176664  r
    724     8   15041488 2662039661  r
    726    20   14567675  755523896  r
    728     5    2474290 1844965508  r
    745     2     330957 2625203890  p
    769     2     536921  853979877  p
    773     2     873070 1022343822  p
    785     3   17226210 3252382975  p
    812     8    9916128 2796656334  r
> E=read_ubcsat("VanDerWaerden_pd_2-4-16_825.cnf_OUT",nrows=100)
 9 10 11 12 13 14
 1  7 19 35 34  4
100
> E=read_ubcsat("VanDerWaerden_pd_2-4-16_840.cnf_OUT",nrows=100)
 1  2  7  9 10 11 12 13 14
 1  1  1  4  5 21 41 25  1
100
   \endverbatim
   </li>
  </ul>

*/
