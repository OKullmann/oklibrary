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


  \todo vdw_2^pd(5,11) >= (742,763)
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
     <li> n=762:
     \verbatim
4,12,15,17,20,22,24,27,30,37,
39,42,46,48,53,59,60,64,65,66,
70,72,80,82,83,85,87,90,95,97,
99,101,102,105,108,115,120,122,123,126,
127,131,134,136,138,139,145,148,149,152,
157,162,172,177,178,179,181,182,189,197,
198,200,201,203,207,208,210,211,212,213,
215,220,221,226,228,229,233,244,247,255,
258,265,266,268,271,273,276,286,288,291,
292,293,294,298,299,300,301,304,305,306,
308,309,311,313,315,321,323,328,329,333,
338,341,346,347,350,351,358,359,361,366,
372,380
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
   <li> "RunPdVdWk1k2 5 11 danov+ 100 20000000" yields
   \verbatim
Break point 1: 730
Break point 2: 763
> cat Statistics
    693     2    5188820   23771175  r
    701     7    2699820 3989538528  r
    719     3   12634923 3256881742  p
    720     2   14804766 3952877916  r
    722     3   17496508 1718934804  r
    729    26    9447735 2018278808  r
    734     1   13341462 1131130439  r
    744     2   16157401  832487773  p
    758    72   16698529 3230957694  r
    760    42    2316105 2527185156  r
> E=read_ubcsat("VanDerWaerden_pd_2-5-11_731.cnf_OUT",nrows=100)
 1  2  3
10 59 31
100
> E=read_ubcsat("VanDerWaerden_pd_2-5-11_764.cnf_OUT",nrows=100)
 1  2  3  4
 5 50 44  1
100
   \endverbatim
   Run "RunPdVdWk1k2 5 11 danov+ 100 60000000":
   \verbatim
Break point 1: 734
# interrupted
> cat Statistics
    728     2   30171162 2645228588  p
    734     2   30326304 1137542626  p
    748    13   34426618 3159649799  r
    754     3   14762804 3149427122  r
> E=read_ubcsat("VanDerWaerden_pd_2-5-11_735.cnf_OUT",nrows=100)
 1  2  3
12 71 17
100
> E=read_ubcsat("VanDerWaerden_pd_2-5-11_756.cnf_OUT",nrows=100)
 1  2
52 47
99
   \endverbatim
   Run "RunPdVdWk1k2 5 11 danov+ 200 100000000": XXX
   </li>
  </ul>

*/
