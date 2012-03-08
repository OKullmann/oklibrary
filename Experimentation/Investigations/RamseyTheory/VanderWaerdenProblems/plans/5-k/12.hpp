// Oliver Kullmann, 20.3.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/12.hpp
  \brief On investigations into vdw_2(5,12)


  \todo vanderwaerden_2(5,12) > 962
  <ul>
   <li> Certificate for n=962 (palindromic):
   \verbatim
4,8,12,14,18,23,25,31,35,36,
43,51,55,57,58,61,64,65,68,70,
73,75,79,88,93,95,102,113,121,122,
126,127,129,132,134,135,140,142,144,147,
150,152,153,154,157,161,163,164,165,167,
171,172,173,175,184,190,194,195,198,200,
209,212,216,226,227,230,232,234,241,242,
243,246,247,249,250,259,260,262,263,265,
266,269,270,271,275,281,282,285,296,307,
318,319,320,328,333,334,337,343,347,348,
349,351,352,353,356,357,359,360,361,362,
368,369,371,373,374,377,378,381,384,388,
389,400,403,405,407,418,420,431,440,441,
444,445,446,448,450,451,453,454,459,462,
465,469,479,484,494,498,501,504,509,510,
512,513,515,517,518,519,522,523,532,543,
545,556,558,560,563,574,575,579,582,585,
586,589,590,592,594,595,601,602,603,604,
606,607,610,611,612,614,615,616,620,626,
629,630,635,643,644,645,656,667,678,681,
682,688,692,693,694,697,698,700,701,703,
704,713,714,716,717,720,721,722,729,731,
733,736,737,747,751,754,763,765,768,769,
773,779,788,790,791,792,796,798,799,800,
802,806,809,810,811,813,816,819,821,823,
828,829,831,834,836,837,841,842,850,861,
868,870,875,884,888,890,893,895,898,899,
902,905,906,908,912,920,927,928,932,938,
940,945,949,951,955,959
   \endverbatim
   </li>
   <li> Find best ubcsat-solver. </li>
   <li> Start with the palindromic solution for n=949:
   \verbatim
C : create_certificate2c_pdvdw([
1,2,6,8,10,14,17,24,31,36,
45,53,54,57,58,60,67,70,79,80,
84,85,89,90,91,92,96,101,106,107,
110,113,116,120,121,123,124,129,130,133,
136,137,138,139,146,153,158,164,165,171,
174,175,178,183,187,188,189,191,197,198,
200,202,203,213,222,223,224,228,231,233,
234,236,238,244,245,246,248,251,255,256,
259,265,268,269,272,273,276,277,278,280,
291,294,306,307,311,316,319,320,325,327,
331,333,336,337,341,342,345,351,353,356,
357,359,367,370,372,376,377,379,388,389,
391,399,400,404,409,410,418,422,424,428,
430,431,432,435,437,438,441,450,452,453,
454,456,462,463,465,470], 949);
certificate_pdvdw_p([5,12],949,C);
  true
output_certificate2_vdw(reverse(C), "VanDerWaerden_2-5-12_949.cnf_sol");

RunVdWk1k2 5 12 949 adaptg2wsat 200 200000000 VanDerWaerden_2-5-12_949.cnf_sol
Break point: 950
> E=read_ubcsat("VanDerWaerden_2-5-12_950.cnf_OUT",nrows=200)
 2  3  4  5  6
 3 15 75 97 10
200
   \endverbatim
   </li>
   <li> Start with the palindromic solution for n=962:
   \verbatim
C : create_certificate2c_pdvdw([
4,8,12,14,18,23,25,31,35,36,
43,51,55,57,58,61,64,65,68,70,
73,75,79,88,93,95,102,113,121,122,
126,127,129,132,134,135,140,142,144,147,
150,152,153,154,157,161,163,164,165,167,
171,172,173,175,184,190,194,195,198,200,
209,212,216,226,227,230,232,234,241,242,
243,246,247,249,250,259,260,262,263,265,
266,269,270,271,275,281,282,285,296,307,
318,319,320,328,333,334,337,343,347,348,
349,351,352,353,356,357,359,360,361,362,
368,369,371,373,374,377,378,381,384,388,
389,400,403,405,407,418,420,431,440,441,
444,445,446,448,450,451,453,454,459,462,
465,469,479], 962);
certificate_pdvdw_p([5,12],962,C);
  true
output_certificate2_vdw(reverse(C), "VanDerWaerden_2-5-12_962.cnf_sol");

RunVdWk1k2 5 12 962 adaptg2wsat 200 200000000 VanDerWaerden_2-5-12_962.cnf_sol
XXX cs-wsok
   \endverbatim
   </li>
  </ul>


  \todo Best local-search solver for palindromic problems
  <ul>
   <li>
   \verbatim
E=run_ubcsat("VanDerWaerden_pd_2-5-12_949.cnf",runs=100,cutoff=20000000)
# considering only the first 11 algorithm (aborted then):
anovp
 2  3  4  5  6
 2  8 36 49  5
FlipsPerSecond = 54760
# continuing:
E=run_ubcsat("VanDerWaerden_pd_2-5-12_949.cnf",runs=100,cutoff=20000000,include_algs=names(tail(run_ubcsat_cnf_algs,-11)))
rnov
 2  3  4  5
 4 21 58 17
rnovp
 2  3  4  5  6
 4 18 65 12  1
# interrupted (next ddfw)
E=run_ubcsat("VanDerWaerden_pd_2-5-12_949.cnf",runs=100,cutoff=20000000,include_algs=names(tail(run_ubcsat_cnf_algs,-25)))
1. ddfw:
 2  3  4  5  6
 3 16 54 26  1
fps: 18963
2. ag2wsat:
 2  3  4  5  6
 2 10 46 41  1
fps: 104883
3. dano:
 3  4  5  6
11 39 46  4
fps: 60472
   \endverbatim
   </li>
  </ul>


  \todo vdw_2^pd(5,12) >= (950,963)
  <ul>
   <li> Certificates:
    <ol>
    <li> n=962:
    \verbatim
4,8,12,14,18,23,25,31,35,36,
43,51,55,57,58,61,64,65,68,70,
73,75,79,88,93,95,102,113,121,122,
126,127,129,132,134,135,140,142,144,147,
150,152,153,154,157,161,163,164,165,167,
171,172,173,175,184,190,194,195,198,200,
209,212,216,226,227,230,232,234,241,242,
243,246,247,249,250,259,260,262,263,265,
266,269,270,271,275,281,282,285,296,307,
318,319,320,328,333,334,337,343,347,348,
349,351,352,353,356,357,359,360,361,362,
368,369,371,373,374,377,378,381,384,388,
389,400,403,405,407,418,420,431,440,441,
444,445,446,448,450,451,453,454,459,462,
465,469,479
     \endverbatim
     </li>
     <li> n=949:
     \verbatim
1,2,6,8,10,14,17,24,31,36,
45,53,54,57,58,60,67,70,79,80,
84,85,89,90,91,92,96,101,106,107,
110,113,116,120,121,123,124,129,130,133,
136,137,138,139,146,153,158,164,165,171,
174,175,178,183,187,188,189,191,197,198,
200,202,203,213,222,223,224,228,231,233,
234,236,238,244,245,246,248,251,255,256,
259,265,268,269,272,273,276,277,278,280,
291,294,306,307,311,316,319,320,325,327,
331,333,336,337,341,342,345,351,353,356,
357,359,367,370,372,376,377,379,388,389,
391,399,400,404,409,410,418,422,424,428,
430,431,432,435,437,438,441,450,452,453,
454,456,462,463,465,470
     \endverbatim
    </ol>
   </li>
   <li> "RunPdVdWk1k2 5 12 adaptg2wsat 100 40000000" yields
   \verbatim
Break point 1: 914
Break point 2: 937
   \endverbatim
   using at most 49 runs.
   </li>
   <li> "RunPdVdWk1k2 5 12 adaptg2wsat 100 80000000" yields
   \verbatim
Break point 1: 943
Break point 2: 950
   \endverbatim
   using at most 97 runs.
   </li>
   <li> "RunPdVdWk1k2 5 12 adaptg2wsat 200 160000000" yields
   \verbatim
Break point 1: 942
Break point 2: 949
   \endverbatim
   using at most 158 runs (for n=929).
   </li>
   <li> "RunPdVdWk1k2 5 12 rnovelty+ 100 80000000" yields
   \verbatim
Break point 1: 916
Break point 2: 963
> cat Statistics
    885     1   12995971 1872038589  r
    887     2    1094362 2893614176  p
    899     2   19018352  140272131  r
    910    17   35644771 1173659953  r
    911    83   14660463 2410513510  r
    912     7   54026989 1417832874  r
    916     5   35454921  424575310  r
    918     9    4188048   60126414  r
    930    25   31626758 1608268637  r
> E=read_ubcsat("VanDerWaerden_pd_2-5-12_917.cnf_OUT",nrows=100)
 1  2  3  4
14 76  9  1
100
> E=read_ubcsat("VanDerWaerden_pd_2-5-12_964.cnf_OUT",nrows=100)
 2  3  4  5
15 55 29  1
100
   \endverbatim
   "RunPdVdWk1k2 5 12 rnovelty+ 200 160000000" yields
   \verbatim
Break point 1: 926
> E=read_ubcsat("VanDerWaerden_pd_2-5-12_927.cnf_OUT",nrows=200)
  1   2   3   4
 42 137  20   1
200
# aborted; new:
Break point 1: 934
Break point 2: 955
    865     3   96622038 3010090764  p
    874     1    2975785 1617541791  r
    876     3   38196227 3352983321  p
    887     8   17532962 2384873228  r
    889     2  102731575 2752221167  p
    891    10   59181254   58624121  r
    908     2   90433396 2063352169  r
    910    16  134828899 1290390872  r
    918     2    8364877 1299041736  p
    919   118   51912123 4071970533  r
    922     2   48697129 1468213001  p
    926     2   49446662  973022998  p
    929     2   48879315  949234963  p
    940    38   17287133 2869615146  r
> E=read_ubcsat("VanDerWaerden_pd_2-5-12_935.cnf_OUT",nrows=200)
  1   2   3   4
  8 105  84   3
200
> E=read_ubcsat("VanDerWaerden_pd_2-5-12_956.cnf_OUT",nrows=200)
 1  2  3  4
16 89 90  5
200
   \endverbatim
   Success seems to strongly rely on the choice of a solution for easier
   instances --- with luck they can be extended over a longer distance, while
   otherwise the problems are too hard to find a solution. </li>
  </ul>

*/
