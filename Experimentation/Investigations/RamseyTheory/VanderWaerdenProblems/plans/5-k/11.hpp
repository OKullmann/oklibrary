// Oliver Kullmann, 14.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/11.hpp
  \brief On investigations into vdw_2(5,11)


  \todo vanderwaerden_2(5,11) > 756
  <ul>
   <li> Find best ubcsat-solver. </li>
   <li> Start with the palindromic solution. </li>
  </ul>


  \todo Best complete solver for palindromic problems


  \todo Best local-search solver for palindromic problems
  <ul>
   <li>
   \verbatim
> E=run_ubcsat("VanDerWaerden_pd_2-5-11_756.cnf",runs=100,cutoff=10000000)
# considering the first 25 algorithms, rnovp and anovpp are best:
rnovp
 1  2  3  4
 8 53 37  2
FlipsPerSecond = 74277
anovpp
 1  2  3
 5 58 37
FlipsPerSecond = 73679
# continuing (csoberon):
>  E=run_ubcsat("VanDerWaerden_pd_2-5-11_756.cnf",runs=100,cutoff=10000000,include_algs=names(tail(run_ubcsat_cnf_algs,-25)))
1. dano:
 0  1  2  3
 1 13 48 38
fps: 71931
2. ddfw:
 1  2  3
36 58  6
fps: 28557
3. ag2wsat:
 1  2  3
23 70  7
fps: 130829
4. g2wsat:
 4  5  6  7  8  9
 2  3 12 25 43 15
fps: 121603
   \endverbatim
   Thus altogether the order is: dano, ddfw, ag2wsat, rnovp, anovpp. </li>
  </ul>


  \todo vdw_2^pd(5,11) >= (742,757)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=741:
     \verbatim
4,6,17,25,27,28,29,30,37,40,
45,47,58,61,62,63,65,66,68,74,
76,80,81,88,89,95,96,97,98,101,
103,104,105,106,108,110,115,118,119,121,
122,123,130,135,136,143,144,146,147,150,
151,155,156,158,160,161,162,163,165,169,
174,179,187,191,192,193,195,199,208,218,
221,222,224,232,236,240,241,243,244,246,
252,257,259,261,262,263,264,266,268,271,
274,284,289,291,292,295,296,297,303,305,
307,309,312,316,318,323,325,329,331,332,
334,342,350,351,352,353,362,365,366
     \endverbatim
     </li>
     <li> n=756:
     \verbatim
4,8,11,18,28,31,32,41,43,44,
45,53,57,62,64,65,67,68,69,72,
75,76,78,82,83,88,99,100,102,104,
105,109,111,114,122,123,128,129,132,137,
138,139,140,144,147,148,150,153,160,161,
163,164,167,175,181,185,186,190,199,206,
208,212,215,218,219,223,225,231,232,235,
237,241,242,244,245,246,251,254,256,257,
264,265,266,272,273,282,283,286,287,290,
295,297,299,300,304,305,306,313,316,317,
319,320,323,328,329,330,336,337,346,350,
351,361,366,367,371,372,373,377
     \endverbatim
    </ol>
   </li>
   <li> "RunPdVdWk1k2 5 11 adaptg2wsat 100 20000000" yields
   \verbatim
Break point 1: 740
Break point 2: 757
   \endverbatim
   </li>
   <li> "RunPdVdWk1k2 5 11 adaptg2wsat 100 40000000" yields
   \verbatim
Break point 1: 742
Break point 2: 755
   \endverbatim
   </li>
   <li> Hard regions starts around n=700, and at most 25 runs needed. </li>
   <li> sapsnr yields segmentation fault (cs-oksvr), and so does rsaps.
   On cscarme "RunPdVdWk1k2 5 11 sapsnr 100 40000000" yields
   \verbatim
Break point 1: 692
Break point 2: 725
   \endverbatim
   using at most one run in random-mode. So adaptg2wsat seems superior here.
   </li>
   <li> "RunPdVdWk1k2 5 11 danov+ 100 20000000" yields XXX </li>
  </ul>

*/
