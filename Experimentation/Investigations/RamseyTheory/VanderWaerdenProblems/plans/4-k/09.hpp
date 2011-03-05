// Oliver Kullmann, 5.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/09.hpp
  \brief On investigations into vdw_2(4,9) >= 309


  \todo vanderwaerden_2(4,9) >= 309
  <ul>
   <li> [Ahmed 2009] states vanderwaerden4k(9) > 254. </li>
   <li> n=254 found satisfiable by adaptnovelty+ (first run with cutoff=10^6;
   seed=719877201, osteps=677160):
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 1000000 -i VanDerWaerden_2-4-9_254.cnf | tee VanDerWaerden_2-4-9_254.cnf_AUS
Clauses = 14532
Variables = 254
TotalLiterals = 77658
FlipsPerSecond = 51723
BestStep_Mean = 480596.590000
Steps_Mean = 983725.910000
Steps_Max = 1000000.000000
PercentSuccess = 5.00
BestSolution_Mean = 19.200000
BestSolution_Median = 21.000000
BestSolution_Min = 0.000000
BestSolution_Max = 26.000000
> E = read_ubcsat("VanDerWaerden_2-4-9_254.cnf_AUS")

 0 13 14 15 16 17 18 19 20 21 22 23 24 25 26
 5  4 11  6  1  2  1  7  3 11 21 10 11  6  1
100
   \endverbatim
   This is an interesting distribution! One should repeat this with, say,
   10000 runs. </li>
   <li> n=255 found satisfiable with cutoff=10^6 (seed=607594063,
   osteps=624845). </l>
   <li> n=256 found satisfiable with cutoff=10^6 (seed=3979253483,
   osteps=609761):
   \verbatim
Clauses = 14763
Variables = 256
TotalLiterals = 78892
FlipsPerSecond = 50406
BestStep_Mean = 463123.530000
Steps_Mean = 982825.650000
Steps_Max = 1000000.000000
PercentSuccess = 4.00
BestSolution_Mean = 20.750000
BestSolution_Median = 23.000000
BestSolution_Min = 0.000000
BestSolution_Max = 27.000000
 0 13 14 15 16 17 19 20 21 22 23 24 25 26 27
 4  2  1  9  5  5  1  3  5 13 16 19  4 10  3
100
   \endverbatim
   </li>
   <l> n=257 found satisfiable with cutoff=10^6 (seed=661764274,
   osteps=909616). </li>
   <li> n=258 found satisfiable with cutoff=10^6 (seed=1516374201,
   osteps=891449):
   \verbatim
Clauses = 14997
Variables = 258
TotalLiterals = 80148
FlipsPerSecond = 50452
BestStep_Mean = 504067.970000
Steps_Mean = 985720.530000
Steps_Max = 1000000.000000
PercentSuccess = 3.00
BestSolution_Mean = 22.500000
BestSolution_Median = 24.000000
BestSolution_Min = 0.000000
BestSolution_Max = 29.000000
 0 15 16 17 19 20 21 22 23 24 25 26 27 28 29
 3  4  4  8  1  3  3  7  6 16 14 20  7  3  1
100
   \endverbatim
   </li>
   <li> n=259 found satisfiable with cutoff=10^6 (seed=81224953,
   osteps=221569):
   \verbatim
Clauses = 15115
Variables = 259
TotalLiterals = 80780
FlipsPerSecond = 50555
BestStep_Mean = 415767.370000
Steps_Mean = 992215.690000
Steps_Max = 1000000.000000
PercentSuccess = 1.00
BestSolution_Mean = 23.450000
BestSolution_Median = 25.000000
BestSolution_Min = 0.000000
BestSolution_Max = 29.000000
 0 13 15 16 17 18 19 21 22 23 24 25 26 27 28 29
 1  1  3  3  7  5  2  1  4  6 12  9 20 18  6  2
100
   \endverbatim
   </li>
   <li> n=260 found satisfiable with cutoff=10^6 (seed=3252333691,
   osteps=745935). </li>
   <li> n=261 found satisfiable by saps with cutoff=10^5 (seed=3081757854,
   osteps=14683). </li>
   <li> n=262
    <ol>
     <li> With gsat-tabu (cutoff=10^6):
     \verbatim
 ubcsat-okl -alg gsat-tabu -runs 100 -cutoff 1000000 -i VanDerWaerden_2-4-9_262.cnf | tee VanDerWaerden_2-4-9_262.cnf_AUS
Clauses = 15470
Variables = 262
TotalLiterals = 82680
FlipsPerSecond = 90269
BestStep_Mean = 356692.350000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 25.490000
BestSolution_Median = 26.000000
BestSolution_Min = 19.000000
BestSolution_Max = 31.000000
19 23 24 25 26 27 28 29 31
 1  9 12 23 32 20  1  1  1
100
     \endverbatim
     </li>
     <li> With rots (cutoff=10^6; solution with seed=427124191 and
     osteps=272999):
     \verbatim
> ubcsat-okl -alg rots -runs 100 -cutoff 1000000 -i VanDerWaerden_2-4-9_262.cnf | tee VanDerWaerden_2-4-9_262.cnf_AUS
Clauses = 15470
Variables = 262
TotalLiterals = 82680
FlipsPerSecond = 72604
BestStep_Mean = 405882.170000
Steps_Mean = 992729.990000
Steps_Max = 1000000.000000
PercentSuccess = 1.00
BestSolution_Mean = 24.760000
BestSolution_Median = 25.000000
BestSolution_Min = 0.000000
BestSolution_Max = 27.000000
 0 16 22 23 24 25 26 27
 1  1  2  4 23 33 25 11
100
     \endverbatim
     </li>
     <li> With adaptnovelty+ (cutoff=10^6):
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 1000000 -i VanDerWaerden_2-4-9_262.cnf | tee VanDerWaerden_2-4-9_262.cnf_AUS
 0 16 17 18 19 20 21 23 24 25 26 27 28 29 30 31
 3  1  2  2  6  2  3  2  1  5 19 17 21 12  3  1
100
     \endverbatim
     Hm --- perhaps adaptnovelty+ is actually better when it comes to larger
     cutoffs (or at least w.r.t. finding solutions)?! </li>
     </li>
    </ol>
   </li>
   <li> n=263 found satisfiable by adaptnovelty+ with cutoff=10^6
   (seed=3699341296, osteps=72347):
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 1000000 -i VanDerWaerden_2-4-9_263.cnf | tee VanDerWaerden_2-4-9_263.cnf_AUS
Clauses = 15589
Variables = 263
TotalLiterals = 83316
FlipsPerSecond = 49375
BestStep_Mean = 464066.000000
Steps_Mean = 976996.130000
Steps_Max = 1000000.000000
PercentSuccess = 3.00
BestSolution_Mean = 26.490000
BestSolution_Median = 28.000000
BestSolution_Min = 0.000000
BestSolution_Max = 32.000000
 0 17 19 20 21 22 23 24 25 26 27 28 29 30 31 32
 3  1  3  2  3  1  2  3  4  3 13 24 17 14  6  1
   \endverbatim
   </li>
   <li> n=264 found satisfiable by adaptnovelty+ with cutoff=10^6
   (seed=1031057194, osteps=813990):
Clauses = 15708
Variables = 264
TotalLiterals = 83952
FlipsPerSecond = 48629
BestStep_Mean = 468605.860000
Steps_Mean = 998139.900000
Steps_Max = 1000000.000000
PercentSuccess = 1.00
BestSolution_Mean = 27.650000
BestSolution_Median = 28.000000
BestSolution_Min = 0.000000
BestSolution_Max = 33.000000
 0 18 19 20 21 23 24 25 26 27 28 29 30 31 32 33
 1  1  1  2  2  2  6  3  6 11 18 16 11 11  8  1
   \endverbatim
   </li>
   <li> n=265 found satisfiable by adaptnovelty+ with cutoff=10^6
   (seed=1091609315, osteps=600425). </li>
   <li> n=266 found satisfiable by adaptnovelty+ with cutoff=10^6
   (seed=4173596225, osteps=318672). </li>
   <li> n=267 found satisfiable by adaptnovelty+ with cutoff=10^6
   (seed=1038073943, osteps=990919):
   \verbatim
Clauses = 16071
Variables = 267
TotalLiterals = 85899
FlipsPerSecond = 48162
BestStep_Mean = 413333.000000
Steps_Mean = 999909.190000
Steps_Max = 1000000.000000
PercentSuccess = 1.00
BestSolution_Mean = 29.010000
BestSolution_Median = 30.000000
BestSolution_Min = 0.000000
BestSolution_Max = 34.000000
 0 19 20 21 23 24 25 26 27 28 29 30 31 32 33 34
 1  1  2  2  2  3  3  2  6  6 12 18 19 13  8  2
   \endverbatim
   </li>
   <li> n=268
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 1000000 -i VanDerWaerden_2-4-9_268.cnf | tee VanDerWaerden_2-4-9_268.cnf_AUS
Clauses = 16193
Variables = 268
TotalLiterals = 86552
FlipsPerSecond = 47014
BestStep_Mean = 489412.130000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 29.910000
BestSolution_Median = 31.000000
BestSolution_Min = 20.000000
BestSolution_Max = 35.000000
20 21 22 23 26 27 28 29 30 31 32 33 34 35
 3  1  1  4  2  3  7  8 17 23 18  7  5  1
   \endverbatim
   but found satisfiable with cutoff=2*10^6 (seed=1345857851, osteps=160923):
   \verbatim
 0 19 20 21 22 23 24 26 27 28 29 30 31 32 33 34
 4  1  4  2  1  1  2  7  3 12 17 16 16  9  3  2
100
   \endverbatim
   </li>
   <li> n=269 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=996923753, osteps=1099230). </li>
   <li> n=270 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=3659785745, osteps=807607). </li>
   <li> n=271 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=531082902, osteps=278105). </li>
   <li> n=272 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1970325183, osteps=727890). </li>
   <li> n=273 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1821288488, osteps=474978). </li>
   <li> n=274 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=96564019, osteps=966284). </li>
   <li> n=275 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=83432612, osteps=343301):
   \verbatim
Clauses = 17057
Variables = 275
TotalLiterals = 91178
FlipsPerSecond = 45803
BestStep_Mean = 931362.360000
Steps_Mean = 1945969.930000
Steps_Max = 2000000.000000
PercentSuccess = 5.00
BestSolution_Mean = 32.160000
BestSolution_Median = 35.000000
BestSolution_Min = 0.000000
BestSolution_Max = 39.000000
 0 24 25 26 28 29 30 31 32 33 34 35 36 37 38 39
 5  2  1  2  4  3  1  2  6  7 16 18 24  4  4  1
   \endverbatim
   </li>
   <li> n=276 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=2699968525, osteps=634428). </li>
   <li> n=277 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1868050270, osteps=757488). </li>
   <li> n=278 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1739242999, osteps=1644942). </li>
   <li> n=279 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1463764243, osteps=318520). </li>
   <li> n=280 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1380488068, osteps=1042265). </li>
   <li> n=281 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=3596813003, osteps=701978). </li>
   <li> n=290 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1761025677, osteps=839031).  </li>
   <li> n=295 found satisfiable by adaptnovelty+ with cutoff=2*10^6
   (seed=1176168555, osteps=1693117).  </li>
   <li> n=296 seemed unsatisfiable:
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 2000000 -i VanDerWaerden_2-4-9_296.cnf -solve | tee VanDerWaerden_2-4-9_296.cnf_AUS
Clauses = 19783
Variables = 296
TotalLiterals = 105772
FlipsPerSecond = 41502
BestStep_Mean = 886547.620000
Steps_Mean = 2000000.000000
Steps_Max = 2000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 49.470000
BestSolution_Median = 51.000000
BestSolution_Min = 1.000000
BestSolution_Max = 55.000000
 1 40 42 43 45 46 47 48 49 50 51 52 53 54 55
 2  1  1  2  2  4  6  8  6 11 16 11 19  7  4
   \endverbatim
   but then a solution was found (seed=1986226015, osteps=154011). </li>
   <li> n=300 seemed unsatisfiable:
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 2000000 -i VanDerWaerden_2-4-9_300.cnf -solve | tee VanDerWaerden_2-4-9_300.cnf_AUS
Clauses = 20326
Variables = 300
TotalLiterals = 108684
FlipsPerSecond = 40762
BestStep_Mean = 887586.680000
Steps_Mean = 2000000.000000
Steps_Max = 2000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 54.620000
BestSolution_Median = 55.000000
BestSolution_Min = 1.000000
BestSolution_Max = 62.000000
 1 46 47 48 50 51 52 53 54 55 56 57 58 59 60 62
 1  1  1  2  4  2  6  8 14 14 10 15 11  7  3  1
   \endverbatim
   but (using cutoff=4*10^6) a solution was found (seed=483608069,
   osteps=1595734). </li>
   <li> n=301:
    <ol>
     <li> 200 runs with cutoff=4*10^6:
     \verbatim
 1  2  3  4 42 43 44 46 47 48 49 50 51 52 53 54 55 56 57 58 59
 3  8  4  1  2  1  1  3  1  2  4  6 16 17 16 23 24 30 19 18  1
     \endverbatim
     </li>
     <li> 62 runs with cutoff=8*10^6 found a solution (seed=1496615047,
     osteps=5004399). </li>
    </ol>
   </li>
   <li> n=302 found satisfiable by adaptnovelty+ with cutoff=8*10^6
   (seed=4209052746, osteps=4948335). </li>
   <li> n=303
    <ol>
     <li>
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 200 -cutoff 8000000 -i VanDerWaerden_2-4-9_303.cnf -solve | tee VanDerWaerden_2-4-9_303.cnf_AUS
Clauses = 20737
Variables = 303
TotalLiterals = 110883
FlipsPerSecond = 41760
BestStep_Mean = 3623527.685000
Steps_Mean = 8000000.000000
Steps_Max = 8000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 48.345000
BestSolution_Median = 54.000000
BestSolution_Min = 1.000000
BestSolution_Max = 59.000000
 1  2  3  4 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59
 3 12  7  1  1  1  1  1  6  6  7  8  9 11 29 26 21 33 12  5
     \endverbatim
     </li>
     <li> 400 runs with cutoff=16*10^6 on cs-wsok: A solution was found in
     round 90 (seed=477566865, osteps=2808546). </li>
    </ol>
   </li>
   <li> n=304 found satisfiable in run 3 with cutoff=16*10^6
   (seed=2362779736, osteps=11583417). </li>
   <li> n=305
    <ol>
     <li> 200 runs with cutoff=4*10^6:
     \verbatim
 2  3  5 51 52 53 54 55 56 57 58 59 60 61 62 63 64
 3 12  1  1  5  7  7 15 17 17 26 21 30 22 12  3  1
     \endverbatim
     </li>
     <li> cutoff=16*10^6: found a solution in run 87 (seed=3154369446,
     osteps=8862697). </li>
    </ol>
   </li>
   <li> n=306 found satisfiable in run 105 (cutoff=16*10^6, seed=4147819434,
   osteps=390227). </li>
   <li> n=307
    <ol>
     <li> cutoff=16*10^6:
     \verbatim
 1  2  3  4  5  6  7 46 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62
 4 18 31 26  7  1  1  1  2  4  6 10 11 12 21 26 39 61 38 46 25  6  4
     \endverbatim
     </li>
     <li> cutoff=32*10^6: Solution found in run 41 (seed=3802003483,
     osteps=106667). </li>
    </ol>
   </li>
   <li> n=308 found satisfiable in run 247 with cutoff=32*10^6
   (seed=2621501336, osteps=4647811), while with cutoff=10^6 and 4000 runs no
   solutions were found:
   \verbatim
  1   2   3   4   5   6   7  46  47  49  50  51  52  53  54  55  56  57  58  59
  1   6  32  12  17   2   2   1   2   1   3   4   9  10  13  23  33  46  94 118
 60  61  62  63  64  65  66  67  68  69  70  71  72
147 242 291 412 527 555 505 424 272 130  49  16   1
   \endverbatim
   </li>
   <li> n=309
    <ol>
     <li> cutoff=2*10^6
     \verbatim
  1   2   3   4   5   6   7   8  46  47  48  49  50  51  52  53  54  55  56  57
  5  11  44  25  27   5   3   1   2   1   1   5   7   6  16  13  22  29  42  49
 58  59  60  61  62  63  64  65  66  67  68  69  70
 94 134 213 272 408 463 535 571 470 304 157  60   5
4000
     \endverbatim
     </li>
     <li> cutoff=4*10^6:
     \verbatim
  1   2   3   4   5   6   7   8  45  46  47  48  49  50  51  52  53  54  55  56
 10  25  81  55  46  18   4   4   1   1   1   5   7   6   7  16  26  29  49  72
 57  58  59  60  61  62  63  64  65  66  67  68
125 176 249 346 446 517 514 547 369 181  59   8
4000
     \endverbatim
     </li>
     <li> cutoff=8*10^6:
     \verbatim
  1   2   3   4   5   6   7   8  45  47  48  49  50  51  52  53  54  55  56  57
 19  36 165 112  82  26  13   5   2   4   4   7  16  22  28  47  65  98 133 187
 58  59  60  61  62  63  64  65  66  67
294 381 478 561 508 403 217  70  16   1
4000
     \endverbatim
     </li>
     <li> cutoff=16*10^6
     \verbatim
  1   2   3   4   5   6   7   8  44  45  46  47  48  49  50  51  52  53  54  55
 33  88 295 221 162  56  21   9   1   3   2   7  18  27  37  39  54  73 117 159
 56  57  58  59  60  61  62  63  64
182 271 415 465 475 417 222 100  31
4000
     \endverbatim
     </li>
     <li> cutoff=32*10^6
     \verbatim
 1  2  3  4  5  6  7  8 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62
 6 16 52 31 26  6  4  3  1  1  1  4  1  5 11  8 11 17 28 27 34 39 32 23  9  4
400
     \endverbatim
     </li>
     <li> cutoff=64*10^6
     \verbatim
 1  2  3  4  5  6  7 44 45 48 49 50 51 52 53 54 55 56 57 58 59 60
13 22 87 66 32 11  8  1  1  1  2  9  9 10 15 13 15 25 25 21  7  7
400
     \endverbatim
     </li>
     <li> cutoff=128*10^6
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 400 -cutoff 128000000 -i VanDerWaerden_2-4-9_309.cnf -solve | tee VanDerWaerden_2-4-9_309.cnf_AUS3
  1   2   3   4   5   6   7   8  46  49  50  51  52  53  54  55  56  57  58
 22  63 156  60  39  11   1   1   1   1   1   5   6   7  11   4   3   5   3
400
> ubcsat-okl -alg samd -runs 400 -cutoff 128000000 -i VanDerWaerden_2-4-9_309.cnf -solve | tee VanDerWaerden_2-4-9_309.cnf_AUS6
  2   3   4   5   7  45  46  47  48  49  50  51  52  53  54  55
  1   6   1   3   1   2   9  28  64 121 106  40  15   1   1   1
400
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=310
    <ol>
     <li> 200 runs with cutoff=4*10^6:
     \verbatim
 2  3  4  5  7 52 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69
 2  3  4  4  1  1  3  2  3  2  8  4 14 15 22 35 27 23 10 12  4  1
     \endverbatim
     </li>
     <li> 200 runs with cutoff=6*10^6:
     \verbatim
 2  3  4  5  6 51 52 55 56 57 58 59 60 61 62 63 64 65 66 67 68
 1  9  2  2  2  1  3  1  8  6  8 12 22 21 24 27 22 15  9  4  1
     \endverbatim
     </li>
     <li> cutoff=16*10^6, 400 runs:
     \verbatim
 2  3  4  5  6  7  8 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65
 6 29 20 22  8  3  3  2  1  9  3  4  8 10 13 20 28 40 37 55 44 21 12  2
     \endverbatim
     </li>
     <li> cutoff=32*10^6
     \verbatim
 2  3  4  5  6  7  8 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
14 45 33 30 17  5  2  3  1  1  2  9  9 10 17 27 23 29 54 36 16 10  7
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>

*/
