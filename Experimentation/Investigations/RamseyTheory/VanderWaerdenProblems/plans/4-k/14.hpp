// Oliver Kullmann, 11.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/14.hpp
  \brief On investigations into vdw_2(4,14)


  \todo vanderwaerden_2(4,14) > 681
  <ul>
   <li> Certificate for n=681 (palindromic):
   \verbatim
1,11,24,27,33,40,41,43,44,54,
61,65,71,75,82,88,89,93,98,112,
115,121,122,124,126,132,133,138,143,147,
157,158,161,162,168,170,172,173,179,182,
186,196,205,212,219,223,227,229,230,233,
238,240,250,253,254,261,267,270,279,292,
293,302,306,309,315,316,318,320,324,326,
327,330,331,332,335,337,341,345,347,350,
351,352,355,356,358,362,364,366,367,373,
376,380,389,390,403,412,415,421,428,429,
432,442,444,449,452,453,455,459,463,470,
477,486,496,500,503,509,510,512,514,520,
521,524,525,535,539,544,549,550,556,558,
560,561,567,570,584,589,593,594,600,607,
611,617,621,628,638,639,641,642,649,655,
658,671,681
   \endverbatim
   </li>
   <li> Find best ubcsat-solver:
   \verbatim
> E=run_ubcsat("VanDerWaerden_2-4-14_681.cnf",runs=100,cutoff=10000000)
1. rots:
33 35 36 37 38 39 40 41 42 43
 1  1  8  7 22 22 19 13  6  1
fps: 118951
2. dano:
33 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60
 1  3  4  3  1  4  4  5  6  6 11 10  9 10  6  7  7  3
fps: 68899
3. ddfw:
35 37 38 39 40 41 42 43
 1  4  7 12 23 29 17  7
fps: 18219
4. anovp:
36 37 39 40 41 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59
 1  1  1  1  2  1  2  2  6  5  5  2 13  9 18 10  8  5  5  1  1  1
fps: 64110
5. ag2wsat:
36 39 40 41 42 43 44 45 46 47 48 49
 1  1  3  7  8 13 14 18 20 10  3  2
fps: 117028
6. samd:
38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 59 60 61 65 66
 2  1  1  5  6  8  5  2  6  9  6  6  6  5  9  6  3  2  1  3  3  1  2  1  1
fps: 183665
7. hwsat:
38 40 41 42 43 44 45 46 47 48 49 50 51 52 54 56 58
 2  1  4  1  8  7  7 13  7 12 12 11  3  8  2  1  1
fps: 149982
8. anovpp:
38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57
 1  1  1  1  3  3  4  3  7  2 13  6 16  8  8  5  8  7  1  2
fps: 68787
9. gsatt:
39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 59 60 61 69 71 72 76
 1  6  3  4  3  6  6 10 12  5  4 10  9  2  2  1  2  1  2  3  2  1  1  1  1  1
78
 1
fps: 178954
10. gwsat:
39 40 43 44 45 46 47 48 49 50
 1  1  6  9 18 19 18 19  6  3
fps: 118352
11. irots:
43 44 49 50 51 52 53 54 55 56 57 58 59 60
 1  1  1  2  3  4  7  9 15 16 24  7  6  4
fps: 113590
   \endverbatim
   Doesn't look too promising. </li>
   <li> Start with the palindromic solution for n=681.
   \verbatim
# In Maxima:
C : create_certificate2c_pdvdw([
1,11,24,27,33,40,41,43,44,54,
61,65,71,75,82,88,89,93,98,112,
115,121,122,124,126,132,133,138,143,147,
157,158,161,162,168,170,172,173,179,182,
186,196,205,212,219,223,227,229,230,233,
238,240,250,253,254,261,267,270,279,292,
293,302,306,309,315,316,318,320,324,326,
327,330,331,332,335,337,341], 681);
certificate_pdvdw_p([4,14],681,C);
  true;
output_certificate2_vdw(reverse(C), "VanDerWaerden_2-4-14_681.cnf_sol");

# see below for run with n=682
   </li>
   <li> Starting from scratch: "RunVdWk1k2 4 14 15 rots 200 20000000" yields
   "UNSAT for n=503" with
   \verbatim
  1   2   3   4
 12  75 108   5
200
   \endverbatim
   </li>
   <li> "RunVdWk1k2 4 14 15 rots 200 100000000" yields "UNSAT for n=509" with
   \verbatim
> cat Statistics
    501    52   93053449 1937044990  r
    505   100   13795555 3938887868  r
> E=read_ubcsat("VanDerWaerden_2-4-14_509.cnf_OUT",nrows=200)
 1  2  3
17 91 92
200
   \endverbatim
   </li>
   <li> n=682, rots, cutoff=10^8:
   \verbatim
RunVdWk1k2 4 14 681 rots 200 100000000 VanDerWaerden_2-4-14_681.cnf_sol
> E=read_ubcsat("VanDerWaerden_2-4-14_682.cnf_OUT",nrows=200)
32 33 34 35 36 37 38 39
 7  7 16 38 42 53 27 10
200
   \endverbatim
   </li>
  </ul>


  \todo vdw_2^pd(4,14) >= (617,682)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=616:
     \verbatim
6,7,12,22,35,37,42,48,50,59,
68,69,71,73,74,79,82,95,102,103,
104,106,109,119,120,122,123,126,127,134,
147,150,156,158,160,161,163,165,170,176,
179,187,192,194,207,217,219,220,222,223,
224,229,236,244,253,255,257,262,263,265,
273,284,289,291,296,297,298,300,301,303,
304
     \endverbatim
     </li>
     <li> n=681:
     \verbatim
1,11,24,27,33,40,41,43,44,54,
61,65,71,75,82,88,89,93,98,112,
115,121,122,124,126,132,133,138,143,147,
157,158,161,162,168,170,172,173,179,182,
186,196,205,212,219,223,227,229,230,233,
238,240,250,253,254,261,267,270,279,292,
293,302,306,309,315,316,318,320,324,326,
327,330,331,332,335,337,341
     \endverbatim
    </ol>
   </li>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite.
   \verbatim
> E=run_ubcsat("VanDerWaerden_pd_2-4-14_681.cnf",runs=100,cutoff=1000000)
1. ddfw:
 0  2 18 19 20 21 22 23 24 25
 2  1  1  2 12  7 26 28 19  2
fps: 22116
2. ag2wsat:
 0 20 21 22 23 24 25 26 27 28
 1  1  1  6 15 17 22 27  8  2
fps: 129109
3. anovp:
 0 20 23 24 25 26 27 28 29 30 31 32 33
 1  1  1  5 10 10 17 15 16 11  6  5  2
fps: 78761
4. wsatt:
 0 44 45 48 49 50 51 52 53 54 55 56 57 58 59
 1  1  1  4  5  7 11 13 14 21 10  5  4  2  1
fps: 124022
5. anovpp:
 2 18 20 21 22 23 24 25 26 27 28 29 30 31 32 33
 1  1  1  1  1  4  8 10  5 12 20 17  9  5  4  1
fps: 78678
6. saps:
 4 25 26 27 28 29 30 31
 1  1  7  7 21 34 19 10
fps: 121402
   \endverbatim
   As often, ddfw yields best results, however it is very slow. </li>
   <li> For now we consider adaptg2wsat as best (it was determined as best
   for vdw_2^pd(5,8) in VanderWaerdenProblems/plans/5-k/general.hpp. </li>
   <li> "RunPdVdWk1k2 4 14 adaptg2wsat 100 1000000" yields
   \verbatim
Break point 1: 611
Break point 2: 624
   \endverbatim
   </li>
   <li> "RunPdVdWk1k2 4 14 adaptg2wsat 100 10000000" yields
   \verbatim
Break point 1: 617
Break point 2: 682
   \endverbatim
   finding all solutions within the first 31 runs. </li>
   <li> "RunPdVdWk1k2 4 14 ddfw 100 10000000" yields the same breakpoints,
   with
   \verbatim
> cat Statistics
    520     1    9308596  985190826  r
    612    15    6123479 2910917089  r
    625    18    2066647 1008711277  r
> E=read_ubcsat("VanDerWaerden_pd_2-4-14_683.cnf_OUT",nrows=100)
 1  2 18 19 20 21 22 23
 7 24  2  4  8 31 20  4
100
   \endverbatim
   </li>
   <li> n=682:
    <ol>
     <li> ddfw, cutoff=4*10^7:
     \verbatim
 2  3  4 15 16 17 18 19
13 44  9  1  4  6 10  1
88
 2  3  4 14 15 16 17 18 19
24 47 12  1  1  6 14  9  5
119
     \endverbatim
     </li>
     <li> adaptg2wsat, cutoff=10^8:
     \verbatim
> E=read_ubcsat("VanDerWaerden_pd_2-4-14_682.cnf_OUT",nrows=200)
  2   3   4  14  15  16  17  18
 64 103  13   1   1   1  10   7
200
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>

*/
