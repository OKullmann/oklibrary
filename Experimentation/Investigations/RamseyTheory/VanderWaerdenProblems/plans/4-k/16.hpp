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


  \todo vanderwaerden_2(4,16) > 871
  <ul>
   <li> Certificate for n=871:
   \verbatim
1,12,19,23,26,36,37,40,43,44,
49,56,64,67,77,78,85,87,92,93,
104,109,110,116,117,121,123,124,136,137,
139,146,148,153,164,170,174,175,177,180,
182,190,201,208,214,215,217,221,231,233,
236,238,243,258,267,269,271,272,276,279,
287,290,298,303,305,310,311,312,314,315,
317,318,327,328,331,333,334,335,342,347,
358,364,366,373,374,378,387,400,402,407,
409,412,414,424,427,428,431,437,439,444,
455,465,466,470,471,473,475,481,484,492,
505,508,512,522,524,527,529,534,536,549,
558,560,562,563,565,570,572,578,589,594,
596,601,602,603,605,606,608,609,618,619,
622,624,625,626,638,649,655,657,664,665,
678,691,693,695,698,700,702,703,705,715,
716,718,719,722,723,728,735,743,754,756,
757,761,766,772,783,787,796,800,803,812,
815,816,818,819,825,832,840,853,861,863,
869
   \endverbatim
   </li>
   <li> Start with the palindromic solution for n=838.
   \verbatim
C : create_certificate2c_pdvdw([
7,10,14,24,26,36,37,40,42,56,
64,67,77,78,80,82,85,93,104,111,
116,118,120,121,124,134,136,137,139,140,
148,153,161,170,174,179,182,184,190,205,
206,208,213,214,215,220,221,233,234,237,
238,245,250,261,267,269,271,272,276,277,
281,287,290,298,303,311,314,315,317,327,
333,335,340,355,358,364,366,369,371,374,
376,387,395,400,402,407,408,409,411,412,
414,415], 838);
certificate_pdvdw_p([4,16],838,C);
  true;
output_certificate2_vdw(reverse(C), "VanDerWaerden_2-4-16_838.cnf_sol");

RunVdWk1k2 4 16 838 adaptg2wsat 200 200000000 VanDerWaerden_2-4-16_838.cnf_sol
UNSAT for n=872
> cat Statistics
# all solutions found in first run with old solution
> E=read_ubcsat("VanDerWaerden_2-4-16_872.cnf_OUT",nrows=200)
26 29 30 31 32 33 34 35 36 37 38 39
 1  2  2  7 13 19 24 23 47 36 25  1
200
   \endverbatim
   </li>
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
     or
     \verbatim
14,21,26,36,37,40,42,49,51,56,
62,64,77,78,80,82,85,93,104,109,
111,116,118,120,124,133,134,136,137,139,
140,146,153,161,170,174,179,182,184,190,
205,208,213,214,215,218,220,221,233,234,
237,238,245,250,261,267,269,271,272,276,
277,281,287,290,298,303,311,314,315,317,
327,333,335,340,355,358,364,366,369,374,
376,387,395,400,402,407,408,409,411,412,
414,415
     \endverbatim
     </li>
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
   <li> "RunPdVdWk1k2 4 16 ddfw 200 40000000"
   \verbatim
> cat Statistics
    697    13    3156363 2759124776  r
    708     2    7505221 1843749526  p
    716     2   27644117 4137870873  p
    718     3   32667413 1398040519  p
    721    31   20500884 3374729475  r
    722     3   10116813 3607758988  r
    724     4   37111035 1039589875  r
    734    37    2257768  153689757  r
    736     8   20703370 3396395856  r
    740    50    6177124  442151357  r
> E=read_ubcsat("VanDerWaerden_pd_2-4-16_812.cnf_OUT")
 5  6  7  8  9
 1  1  2 13  6
23
   \endverbatim
   Was aborted; doesn't look too promising. </li>
   <li> "RunPdVdWk1k2 4 16 adaptg2wsat 200 80000000":
   \verbatim
Break point 1: 824
Break point 2: 839
> cat Statistics
    693     2    7025508  328048345  r
    695     4   38842550 2366232293  r
    699     6    2276284 3975805307  r
    712     2   60655614  885048118  r
    714     6   33137716 4247312130  r
    730    17   35553680 2458077379  r
    736    26   69377613 3911765110  r
    749     2      26573 3969039896  p
    785     3   14318525  146029777  r
    799     2      86447 1834186030  p
    815    82   66224973 2785930033  r
> E=read_ubcsat("VanDerWaerden_pd_2-4-16_825.cnf_OUT",nrows=200)
 1  7  8  9 10 11 12 13 14
12  1  2  6 27 58 57 36  1
200
> E=read_ubcsat("VanDerWaerden_pd_2-4-16_840.cnf_OUT",nrows=200)
 1  2  8  9 10 11 12 13
 5  2  1 18 43 75 48  8
200
   \endverbatim
   </li>
  </ul>

*/
