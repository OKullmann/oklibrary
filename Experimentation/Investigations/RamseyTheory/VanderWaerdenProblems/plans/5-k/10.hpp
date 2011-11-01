// Oliver Kullmann, 14.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/10.hpp
  \brief On investigations into vdw_2(5,10)


  \todo vanderwaerden_2(5,10) > 611
  <ul>
   <li> Find best ubcsat-solver:
   \verbatim
> E=run_ubcsat("VanDerWaerden_2-5-10_611.cnf",runs=100,cutoff=10000000)
1. rnov:
 7  8  9 10 11 12
 4 22 33 32  8  1
fps: 84624
2. rnovp:
 7  8  9 10 11 12
 3 14 31 38 13  1
fps: 82701
3. ag2wsat:
 7  8  9 10 11
 2 11 26 38 23
fps: 149477
4. dano:
 7  8  9 10 11 12
 2  5 14 45 29  5
fps: 88952
5. ddfw:
 7  8  9 10 11 12
 1  7 16 29 30 17
fps: 27229
6. anovpp:
 7  8  9 10 11 12 13
 1  3 20 32 32  9  3
fps: 86652
7. anovp:
 8  9 10 11 12
 3 19 37 27 14
fps: 84418
8. rots:
11 12 13 14 15 16 17
 1 14 14 34 29  7  1
fps: 145583
   \endverbatim
   Looks like as rnovelty would be the clear winner. </li>
   <li> Start with the palindromic solution. </li>
   <li> Starting from scratch:
   "RunVdWk1k2 5 10 11 rnovelty 100 20000000" yields:
   \verbatim
> cat Statistics
    412     1      15430 1371379553  r
    447     4       2910 2478051207  r
    456     2     292762 3624378650  p
    459     3       2365 3003854103  r
    460     2       2851  502656841  r
    473     2       2923 1207096606  p
    502    10     113202 4106118786  r
    503     6    3723902 3880124301  r
    506     1      31107  727895215  r
    508    22   12821902 3285317607  r
    509     3     133656 2269694929  r
    515    21     408404 2173735794  r
    517    39     129305  356707910  r
    525     1    1386520  399401812  r
    526    10    1189357 4126592357  r
    529    22    3300702  483612490  r
    536    98    4002213 3140412308  r
> E=read_ubcsat("VanDerWaerden_2-5-10_541.cnf_OUT")
1 2 3 4
8 4 1 1
14
   \endverbatim
   "RunVdWk1k2 5 10 11 rnovelty 100 100000000" yields:
   \verbatim
> cat Statistics
    453     3       4217  718970865  p
    464     2      15203  935638139  p
    465     2      20054 3372882574  r
    470     2   81083097 2472797817  r
    476     1      17603  538486244  r
    497     2     412715 1410368130  p
    499     3       7727 1166209006  p
    504    16     844019 3103395799  r
    513    16     253806  545802533  r
    524    14    1661443  963175233  r
    528    30     506205 3819963481  r
    530     2   21969507 3900898346  r
    533     4   11187421  174886118  r
    534    67     963122 1800443265  r
> E=read_ubcsat("VanDerWaerden_2-5-10_538.cnf_OUT")
 1  2
18  9
27
# aborted
XXX
   \endverbatim
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
   <li>
   \verbatim
> E=run_ubcsat("VanDerWaerden_pd_2-5-10_611.cnf",runs=100,cutoff=10000000)
1. ag2wsat:
 0  1  4  5  6  7  8
 2  3  5 17 52 20  1
fps: 162126
2. ddfw:
 0  1  4  5  6  7
 1  1  3 19 59 17
fps: 32896
3. dano:
 0  4  5  6  7
 1  4 13 58 24
fps: 102956
4. rnovp:
 1  4  5  6  7  8
 1  4 22 52 20  1
fps: 104940
5. gwsat:
 1  8  9 10 11 12 13 14
 1  1  6  9 22 37 22  2
fps: 175953
6. irots:
 2  9 10 11 12 13
 2  7  8 33 38 12
fps: 184859
   \endverbatim
   adaptg2wsat seems the clear winner. </li>
  </ul>


  \todo vdw_2^pd(5,10) >= (595,612)
  <ul>
   <li> The conjecture is vdw_2^pd(5,10) = (595,612). </li>
   <li> Certificates:
    <ol>
     <li> n=594:
     \verbatim
3,12,17,22,23,28,29,32,39,40,
43,45,46,50,51,52,56,58,59,62,
63,66,67,71,78,79,81,82,86,87,
88,94,97,100,101,103,105,113,118,120,
122,123,124,125,134,135,145,146,152,154,
162,163,172,174,176,177,179,180,186,187,
189,192,193,196,197,201,207,208,210,212,
215,225,227,229,230,235,241,248,250,251,
252,253,255,256,257,261,263,264,266,272,
273,275,276,277,279,282,288,293
     \endverbatim
     </li>
     <li> n=611:
     \verbatim
10,14,15,18,24,27,28,33,40,41,
45,47,48,49,50,53,54,55,58,65,
69,71,72,80,88,89,94,96,97,99,
100,101,103,107,110,113,114,119,121,123,
124,127,129,130,135,139,141,142,143,145,
148,153,162,163,168,170,171,173,177,181,
186,187,188,189,192,193,194,196,197,198,
201,202,206,209,215,216,217,220,227,228,
236,242,244,245,248,249,251,258,260,261,
266,268,270,271,275,276,278,280,287,289,
290,291,293,296,301
     \endverbatim
     (Another run found exactly the same solution.) </li>
    </ol>
   </li>
   <li> "RunPdVdWk1k2 5 10 adaptg2wsat 100 20000000" yields
   \verbatim
Break point 1: 595
Break point 2: 612
   \endverbatim
   finding all solutions in the 7th run.
   </li>
   <li> "RunPdVdWk1k2 5 10 adaptg2wsat 100 40000000" yields
   \verbatim
> cat Statistics
    551     1    8333889 1210023291  r
    559     2   29769873 3024236198  p
    560     5   36409063 2089433844  r
    566     3      25606 3998212559  p
    567    17   18239786 2571998178  r
    568     2    9425578 1971828075  r
    570    21   15465888 3052336236  r
    582    26   31969997 1426516201  r
> E=read_ubcsat("VanDerWaerden_pd_2-5-10_588.cnf_OUT")
 1  2  3
 5 36 10
51
# interrupted
   \endverbatim
   </li>
   <li> "RunPdVdWk1k2 5 10 adaptg2wsat 100 60000000" yields
   \verbatim
Break point 1: 585
Break point 2: 612
> cat Statistics
    553     1    2543103  178950496  r
    559     2   48329211 3424240417  p
    566     3   19112214 1225851492  p
    580    61   23026442 2879204021  r
    581     2      25607 3107016596  p
    597    12   52843138 2453468376  r
> E=read_ubcsat("VanDerWaerden_pd_2-5-10_586.cnf_OUT",nrows=100)
 1  2  3
20 74  6
100
> E=read_ubcsat("VanDerWaerden_pd_2-5-10_613.cnf_OUT",nrows=100)
 1  4  5  6
16  7 46 31
100
   \endverbatim
   </li>
   <li> One should probe further for n=596 and n=613 XXX </li>
  </ul>

*/
