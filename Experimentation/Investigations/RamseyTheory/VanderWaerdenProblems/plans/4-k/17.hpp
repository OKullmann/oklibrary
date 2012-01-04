// Oliver Kullmann, 18.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/17.hpp
  \brief On investigations into vdw_2(4,17)


  \todo Best ubcsat-solver
  <ul>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
  </ul>


  \todo vanderwaerden_2(4,17) > 1112
  <ul>
   <li> Certificate for n=1112:
   \verbatim
11,20,31,34,45,54,61,65,71,72,
73,80,81,94,95,98,103,104,116,117,
120,131,132,142,149,158,162,170,172,177,
178,183,184,195,196,200,201,202,205,213,
214,217,219,220,227,228,235,239,248,265,
267,269,274,275,281,292,293,294,297,298,
299,302,314,316,317,322,324,326,332,343,
345,352,363,374,386,390,391,396,399,407,
408,413,414,419,421,423,429,433,441,442,
449,453,459,460,461,463,468,469,471,474,
483,486,488,491,492,504,505,516,519,530,
537,550,556,557,558,560,565,566,568,571,
572,580,583,584,588,589,590,601,605,607,
608,615,616,617,623,634,635,643,647,657,
668,669,681,682,687,690,699,702,704,705,
710,712,713,714,720,724,732,733,740,750,
751,752,759,760,762,766,774,777,779,782,
783,787,796,799,807,811,821,828,830,837,
841,847,848,849,856,857,859,862,871,875,
879,880,881,892,893,898,899,906,914,918,
925,926,938,944,946,948,953,954,959,960,
971,972,973,976,977,978,981,995,996,1001,
1011,1022,1023,1024,1037,1042,1045,1057,1065,1069,
1070,1075,1086,1090,1092,1098,1100,1101,1108
   \endverbatim
   </li>
   <li> Start with the palindromic solution for n=1075.
   \verbatim
C : create_certificate2c_pdvdw([
3,8,25,26,31,41,53,54,61,72,
76,83,86,87,99,100,103,105,108,120,
123,128,130,131,132,142,149,151,158,169,
170,172,177,178,180,183,184,192,195,196,
200,201,213,214,219,227,229,239,246,247,
248,255,259,266,267,269,274,275,277,289,
292,294,297,298,310,311,314,322,325,332,
336,343,356,362,364,372,377,378,389,390,
391,395,396,399,407,408,411,413,414,419,
421,422,429,442,449,459,468,474,475,483,
487,488,493,496,508,510,511,516,518,519,
520,526,530,537,538], 1075);
certificate_pdvdw_p([4,17],1075,C);
  true;
output_certificate2_vdw(reverse(C), "VanDerWaerden_2-4-17_1075.cnf_sol");
   \endverbatim
   "RunVdWk1k2 4 17 1075 adaptg2wsat 200 400000000 VanDerWaerden_2-4-17_1075.cnf_sol"
   \verbatim
# all solutions found in first round with old solution
> E=read_ubcsat("VanDerWaerden_2-4-17_1113.cnf_OUT")
69 71 72 73 74 75 76 77 78 79 80
 2  1  1  1  3  9  8  9 11  6  2
53
   \endverbatim
   Perhaps for these hard problems the only chances are relatively small
   modifications of previous solutions. </li>
   <li> n=1113:
    <ol>
     <li>
     RunVdWk1k2 4 17 1112 adaptg2wsat 200 800000000 VanDerWaerden_2-4-17_1112.cnf_sol
     \verbatim
> E=read_ubcsat("VanDerWaerden_2-4-17_1113.cnf_OUT",nrows=200)
64 69 70 71 72 73 74 75 76 77 78 79 80 81
 1  2  3  6 15 13 13 31 27 42 37  7  2  1
200
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo vdw_2^pd(4,17) >= (855,1076)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=854:
     \verbatim
11,17,28,39,41,42,52,56,61,62,
64,69,75,79,82,98,105,110,112,113,
115,128,130,141,144,157,158,174,180,182,
183,185,187,200,207,215,221,225,226,228,
234,239,245,249,256,260,263,268,274,278,
279,280,282,283,288,289,291,294,304,306,
323,324,334,347,349,352,355,356,357,359,
367,371,374,389,405,408,409,413,419
     \endverbatim
     </li>
     <li> n=1075:
     \verbatim
3,8,25,26,31,41,53,54,61,72,
76,83,86,87,99,100,103,105,108,120,
123,128,130,131,132,142,149,151,158,169,
170,172,177,178,180,183,184,192,195,196,
200,201,213,214,219,227,229,239,246,247,
248,255,259,266,267,269,274,275,277,289,
292,294,297,298,310,311,314,322,325,332,
336,343,356,362,364,372,377,378,389,390,
391,395,396,399,407,408,411,413,414,419,
421,422,429,442,449,459,468,474,475,483,
487,488,493,496,508,510,511,516,518,519,
520,526,530,537,538
     \endverbatim
     or (very similar)
     \verbatim
3,8,25,26,31,33,34,41,54,61,
71,72,83,86,87,99,100,103,105,108,
120,122,123,130,131,132,149,151,158,169,
170,172,177,178,180,183,184,192,195,196,
200,201,213,214,219,227,229,239,246,248,
255,259,266,267,269,274,275,277,289,292,
294,297,298,310,311,314,322,325,332,336,
343,344,356,362,364,372,377,378,389,390,
391,395,396,399,407,408,411,413,414,419,
421,422,429,442,449,459,468,474,475,483,
487,488,493,496,508,510,511,516,518,519,
520,526,530,537,538
     \endverbatim
     </li>
    </ol>
   </li>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite.
   \verbatim
> E=run_ubcsat("VanDerWaerden_pd_2-4-17_1075.cnf",runs=100,cutoff=20000000)
# Considering only the first 10 algorithms (aborted then): (csnereid)
gsatt
23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 41 42 43
 1  3  2  2  6  9 17 24  8  9  5  5  1  2  3  1  1  1
FlipsPerSecond = 138195
# continuing:
> E=run_ubcsat("VanDerWaerden_pd_2-4-17_1075.cnf",runs=100,cutoff=20000000,include_algs=names(tail(run_ubcsat_cnf_algs,-10)))
# anovp, rnovp, rnov, rsaps, sapns, saps are worse than gsatt
# continuing (next rots)
> E=run_ubcsat("VanDerWaerden_pd_2-4-17_1075.cnf",runs=100,cutoff=20000000,include_algs=names(tail(run_ubcsat_cnf_algs,-16)))
ddfw
 0 27 28 29 30 31 32 33 34
 1  2  6  9 15 18 27 14  3
wsattn
 2 63 64 66 67 68 69 70 71 72 73 74 75 76
 1  1  1  2  5  7  9 10  8 13 13 14 13  3
samd
24 25 26 27 28 29 30 31 32 33 34 35 36
 1  1  4  9 15 12 15 19 11  6  4  1  2
# continuing with g2wsat
> E=run_ubcsat("VanDerWaerden_pd_2-4-17_1075.cnf",runs=100,cutoff=20000000,include_algs=names(tail(run_ubcsat_cnf_algs,-26)))
1. dano:
23 28 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49
 1  1  1  1  2  2  2  5  6  2  7  8 12  8 12 15  6  3  4  2
fps: 39890
2. ag2wsat:
26 28 29 30 31 32 33 34 35 36
 1  2  2  1  5 17 23 22 15 12
fps: 68957
   \endverbatim
   So the ranking is: ddfw, wsattn, gsatt, dano, samd, ag2wsat. </li>
   <li> For now we consider adaptg2wsat as best (it was determined as best
   for vdw_2^pd(5,8) in VanderWaerdenProblems/plans/5-k/general.hpp. </li>
   <li>  "RunPdVdWk1k2 4 17 adaptg2wsat 10 20000000" yields
   \verbatim
Break point 1: 827
Break point 2: 1076
   \endverbatim
   finding all solutions within the first 9 rounds.
   </li>
   <li>  "RunPdVdWk1k2 4 17 adaptg2wsat 100 20000000" yields
   \verbatim
Break point 1: 845
Break point 2: 1076
   \endverbatim
   finding all solutions within the first 37 rounds.
   </li>
   <li> "RunPdVdWk1k2 4 17 ddfw 100 20000000" yields
   \verbatim
Break point 1: 855
Break point 2: 1064
> cat Statistics
    828     2   13392861 2997944289  p
# otherwise all solutions found in the first run with the old solution
> E=read_ubcsat("VanDerWaerden_pd_2-4-17_856.cnf_OUT",nrows=100)
 1  2  3
10 59 31
100
> E=read_ubcsat("VanDerWaerden_pd_2-4-17_1065.cnf_OUT",nrows=100)
 1 25 26 27 28 29 30 31 32
 1  1  3  7  7 22 29 27  3
100
   \endverbatim
   </li>
  </ul>

*/
