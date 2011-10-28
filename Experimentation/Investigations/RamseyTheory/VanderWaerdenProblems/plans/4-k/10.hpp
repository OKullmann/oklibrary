// Oliver Kullmann, 5.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/10.hpp
  \brief On investigations into vdw_2(4,10) > 328


  \todo vanderwaerden_2(4,10) > 328
  <ul>
   <li> The conjecture is vanderwaerden_2(4,10) = 329. </li>
   <li> Certificate for n=328:
   \verbatim
10,19,23,25,29,30,33,34,36,40,
42,44,45,51,54,58,67,68,77,81,
84,90,91,93,95,99,101,102,105,106,
107,110,112,120,122,125,126,127,130,131,
133,137,139,141,142,148,151,155,164,165,
174,178,181,187,188,190,192,196,198,199,
202,203,204,207,209,213,217,219,222,223,
224,227,228,230,234,236,238,239,245,248,
252,261,262,271,275,278,284,285,287,289,
293,295,296,299,300,304,306,310,319
   \endverbatim
   (obtained by the first successful run of ddfw below). Looks palindromic?
   </li>
   <li> Evaluating
   \verbatim
> E = run_ubcsat("VanDerWaerden_2-4-10_350.cnf", runs=100,cutoff=100000,monitor=TRUE)
   \endverbatim
   by plot(E$alg,E$best): It seems that gsat_tabu, rots and samd are best,
   where samd reached the best min=37, but this might be just an outlier, and
   perhaps rots is best. </li>
   <li> Using cutoff=10^6: the two best min-values are reached by rnovelty
   (twice min=8) and rots (twice min=9), while otherwise the best min=34.
   Considering the success of rnovelty as just luck, rots would again be best
   here. </li>
   <li> Evaluating
   \verbatim
E = run_ubcsat("VanDerWaerden_2-4-10_330.cnf", runs=100,cutoff=100000,monitor=TRUE)
   \endverbatim
   again rots seems best (reaching min=26). </li>
   <li> The same, but with cutoff=10^6: three algorithms, each with one run,
   reached min=3, namely rots, rsaps and sapsnr, while otherwise the best min
   is 26. Again rots seems best. </li>
   <li> The same, but with cutoff=4*10^6: the three best algorithms seem now
   adaptnovelty+, rnovelty+ and saps (the best is min=2; perhaps adapnovelty+
   is best):
   \verbatim
> table(E$best[E$alg=="adaptnoveltyp"])
 3  4  5 28 29 30 31 32 33 34 35
 1  1  1  5  5 11 14 22 20 14  6
> table(E$best[E$alg=="rnoveltyp"])
 2  4 28 30 31 32 33 34 35 36 37 38 39 40 41 42
 1  2  1  2  1  1  7  3 11 15 15 17 13  8  2  1
> table(E$best[E$alg=="saps"])
 2 28 31 33 34 35 36 37 38
 1  1  2  8 14 19 36 18  1
> table(E$best[E$alg=="irots"])
 5 30 31 32 33 34 35 36 37
 1  2  7 10 18 23 25 11  3
   \endverbatim
   </li>
   <li> Evaluating (cutoff=10^5)
   \verbatim
>  E = run_ubcsat("VanDerWaerden_2-4-10_320.cnf", runs=100,cutoff=100000,monitor=TRUE)
   \endverbatim
   gsat-tabu and samd appear best (both reaching min=23).
   </li>
   <li> Now using cutoff=10^6: Three solver reached min=1:
   \verbatim
 table(E$best[E$alg=="novelty"])
 1 42 44 45 46 47 48 49 50 51 52
 1  1  7  6  9 17 21 14 19  3  2
> table(E$best[E$alg=="rnovelty"])
 1 26 29 30 31 32 33 34 35 36 37 38
 1  1  9  7 11 12 17 16 13  7  5  1
> table(E$best[E$alg=="rots"])
 1 23 24 25 26 27 28 29
 1  2  7 24 39 20  6  1
   \endverbatim
   but these could well be outliers. Unclear situation. </li>
   <li> cutoff=10^7: four solvers found solutions:
   \verbatim
> table(E$best[E$alg=="adaptnoveltyp"])
 0  2  3 20 21 22 23 24 25 26 27 28
 2  1  2  2  2  6 16 21 26 17  4  1
> table(E$best[E$alg=="rnoveltyp"])
 0  1 24 25 26 27 28 29 30 31 32 33
 1  2  3  3  4  6 17 25 21 10  7  1
> table(E$best[E$alg=="rsaps"])
 0  3  4 26 27 28 29 30 31 32
 1  1  1  3  5 12 23 32 20  2
> table(E$best[E$alg=="noveltyp"])
 0 39 40 41 42 43 44 45 46 47 48
 1  2  4  4  6 11 18 23 15 15  1
   \endverbatim
   where the above order seems to be appropriate for their ranking.
   So perhaps for now we should assume that adaptnovelty+ is best. </li>
   <li> New evaluation:
   \verbatim
> E=run_ubcsat("VanDerWaerden_2-4-10_328.cnf",runs=100,cutoff=10000000)
> eval_ubcsat_dataframe(E,FALSE)
1. sapsnr:
 1  5 29 30 31 32 33 34 35 36
 1  1  1  1  6 15 27 31 15  2
fps: 185796
2. g2wsat:
 1 31 32 33 34 35 36 37
 1  3  1  9 29 25 28  4
fps: 216183
3. ddfw:
 2  3  4 22 24 25 26 27 28
 7  3  5  1  5 17 29 28  5
fps: 44886
4. ag2wsat:
 2  3  4  5 25 26 27 28 29 30
 3  2  1  1  6 11 20 21 24 11
fps: 196592
5. anovp:
 2  3  4 25 26 27 28 29 30 31 32 33
 2  1  2  3  3 10 13 26 21 15  3  1
fps: 175065
6. anovpp:
 2  3  4  5 21 22 25 26 27 28 29 30 31 32
 1  1  1  2  1  1  2  8 17 13 17 24  9  3
fps: 170560
7. rnov:
 2  4 27 28 29 30 31 32 33 34 35 36 37 38
 1  1  2  2  2  3  5  7 13 15 20 17  9  3
fps: 172362
8. irots:
 3  4  7 24 28 29 30 31 32 33 34
 2  1  1  1  2  3 12 12 31 29  6
fps: 208831
9. rnovp:
 3 28 29 30 31 32 33 34 35 36 37 38
 2  1  1  3  6 11 15 20 18 11  9  3
fps: 172842
10. rsaps:
 3 30 31 32 33 34 35 36
 2  2  6  9 28 36 12  5
fps: 188428
11. paws:
 3  4 28 31 32 33 34 35 36 37
 1  2  1  4  8  5 29 30 19  1
fps: 174601
12. wsattn:
 3  4 50 52 54 55 56 57 58 59 60 61 62
 1  1  1  2  6  8 12 13 22 14 13  4  3
fps: 273443
13. dano:
 3 24 25 26 27 28 29 30 31 32 33
 1  3  1  4  6 13 13 23 19 14  3
fps: 175831
14. saps:
 3 27 30 31 32 33 34 35 36
 1  1  1  7 15 23 35 14  3
fps: 187893
   \endverbatim
   Regarding the "mass" in the good range, ddfw is best. </li>
   <li> n=321 with adaptnovelty+
    <ol>
     <li> cutoff=10^7:
     \verbatim
> nohup ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 10000000 -i VanDerWaerden_2-4-10_321.cnf | tee VanDerWaerden_2-4-10_321.cnf_OUT &
> E=read_ubcsat("VanDerWaerden_2-4-10_321.cnf_OUT")
 0  1  2 21 22 23 24 25 26 27
 1  2  2  1  6 12 24 25 18  9
100
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  36040 1817000 3963000 4553000 7101000 9987000
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=322, cutoff=10^7: a solution was found in run 51 (seed=3424017434,
   osteps=2557765). </li>
   <li> n=323, cutoff=10^7: a solution was found in run 22 (seed=420342374,
   osteps=9743243). </li>
   <li> n=324
    <ol>
     <li> cutoff=10^7 found one solution in 1000 runs:
     \verbatim
> nohup ubcsat-okl -alg adaptnovelty+ -runs 1000 -cutoff 10000000 -i VanDerWaerden_2-4-10_324.cnf | tee VanDerWaerden_2-4-10_324.cnf_OUT &
> E=read_ubcsat("VanDerWaerden_2-4-10_324.cnf_OUT")
  0   1   2   3   4   5  18  21  22  23  24  25  26  27  28  29  30  31
  1  11  19   9   4   2   1   2   9  29  69 143 193 254 178  66   9   1
1000
> E[E$min==0,]
    sat min  osteps  msteps       seed
993   1   0 2065594 2065594 2579089101
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   8842 1877000 4370000 4471000 6848000 9996000
     \endverbatim
     </li>
     <li> cutoff=10^8 found three solution:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 100000000 -i VanDerWaerden_2-4-10_324.cnf | tee VanDerWaerden_2-4-10_324.cnf_OUT2
> E = read_ubcsat("VanDerWaerden_2-4-10_324.cnf_OUT2")
 0  1  2  3  4  5 21 22 23 24 25 26
 3  7 13  4  6  2  1  6 12 20 12  4
90
> E[E$min==0,]
   sat min   osteps   msteps       seed
38   1   0 67111454 67111454 3357914690
48   1   0 37435932 37435932 3595047384
62   1   0 68099425 68099425  748374078
     \endverbatim
     </li>
     <li> So it seems one needs to use cutoff=10^8 now. </li>
    </ol>
   </li>
   <li> n = 325, cutoff=10^8: found one solution (seed=1755037834,
   osteps=38900247):
   \verbatim
> E=read_ubcsat("VanDerWaerden_2-4-10_325.cnf_OUT2")
 1  2  3  4  5 21 22 23 24 25 26 27
10 17  7  6  2  2  6 11 15 16  7  1
100
> E = read_ubcsat("VanDerWaerden_2-4-10_325.cnf_OUT")
 0  1  2  3  5 20 21 22 23 24 25 26
 1  6 18  4  4  1  2  4  9 13 16  3
81
   \endverbatim
   </li>
   <li> n = 326, cutoff=10^8: found one solution (seed=64752191,
   osteps=96247637) in 162 runs:
   \verbatim
> E = read_ubcsat("VanDerWaerden_2-4-10_326.cnf_OUT")
 0  1  2  3  4  5 21 22 23 24 25 26 27
 1  6 33 18  6  4  4  4  9 18 36 15  8
162
   \endverbatim
   So we should use cutoff=2*10^8 now. </li>
   <li> n=327, cutoff=2*10^8: In 81 runs one solution was found
   (seed=403939055, osteps=23019617):
   \verbatim
> E = read_ubcsat("VanDerWaerden_2-4-10_327.cnf_OUT")
 0  1  2  3  4  5 22 23 24 25 26 27
 1  1 28 12  5  3  2  5  9  8  6  1
81
   \endverbatim
   And in another 112 runs also one solution was found:
   \verbatim
> nohup ubcsat-okl -alg adaptnovelty+ -runs 200 -cutoff 200000000 -i VanDerWaerden_2-4-10_327.cnf > VanDerWaerden_2-4-10_327.cnf_OUT &
 0  1  2  3  4  5 21 22 23 24 25 26
 1  3 33 11 11  3  1  3  5  9 17 15
112
> summary(E$osteps)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
   525600  32000000  91600000  89860000 138000000 199800000
   \endverbatim
   </li>
   <li> n=328
    <ol>
     <li> cutoff=2*10^8:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 200 -cutoff 200000000 -i VanDerWaerden_2-4-10_328.cnf | tee VanDerWaerden_2-4-10_328.cnf_OUT
> E = read_ubcsat("VanDerWaerden_2-4-10_328.cnf_OUT")
 1  2  3  4  5 21 22 23 24 25 26 27 28
 2 51 45 28  9  1  7  3 12 17 19  4  2
200
> summary(E$osteps)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
   599500  40370000  84970000  92870000 141000000 198300000
     \endverbatim
     </li>
     <li> cutoff=4*10^8:
     \verbatim
> nohup ubcsat-okl -alg adaptnovelty+ -runs 200 -cutoff 400000000 -i VanDerWaerden_2-4-10_328.cnf > VanDerWaerden_2-4-10_328.cnf_OUT &
> E=read_ubcsat("VanDerWaerden_2-4-10_328.cnf_OUT")
 0  1  2  3  4  5 23 24 25 26
 1  1 29 17  4  2  1  1  1  1
58
> summary(E$osteps)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
  1000000  36620000 124200000 146200000 228000000 397700000
     \endverbatim
     So cutoff=4*10^8 seems needed now.
     </li>
     <li> ddfw: cutoff=10^7
     \verbatim
 1  2  3  4  5  6 23 24 25 26 27 28
 1  7  2  4  8  1  1  4  8 25 31  8
100
     \endverbatim
     cutoff=2*10^7:
     \verbatim
 2  3  4  5  6 22 24 25 26 27
13  7 15  5  2  2  4 14 28 10
100
     \endverbatim
     cutoff=4*10^7:
     \verbatim
 2  3  4  5  6 21 23 24 25 26 27
24 16 11  5  1  2  2  7 11 17  4
100
     \endverbatim
     cutoff=8*10^7:
     \verbatim
      4 1     0             63918863             63918863  826885685
     98 1     0             69022414             69022414  292766181
 0  1  2  3  4  5  6 21 22 23 24 25 26
 2  2 28 32 13  4  1  1  1  6  4  4  2
100
     </li>
    </ol>
   </li>
   <li> n=329
    <ol>
     <li>cutoff=4*10^8:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 200 -cutoff 400000000 -i VanDerWaerden_2-4-10_329.cnf | tee VanDerWaerden_2-4-10_329.cnf_OUT
> E = read_ubcsat("VanDerWaerden_2-4-10_329.cnf_OUT")
 1  2  3  4  5  6 21 23 24 25 26 27
 7 75 53 26 12  2  2  2  9  6  5  1
200
> summary(E$osteps)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
  4042000  78170000 166600000 178900000 260800000 397600000

> E=read_ubcsat("VanDerWaerden_2-4-10_329.cnf_OUT2")
 1  2  3  4  5  6 22 23 24 25 26 27
 3 47 31 17  4  3  1  2  5  5  6  2
126
> summary(E$osteps)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
  1242000  67280000 164600000 174800000 284900000 394100000
     \endverbatim
     </li>
     <li> cutoff=5*10^8:
     \verbatim
  1   2   3   4   5   6  23  24  25  26
  5 103  59  31  10   3   2   7  12   8
240
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=330 with adaptnovelty+
    <ol>
     <li> cutoff=10^7
     \verbatim
 2  4 23 26 28 29 30 31 32 33 34
 5  3  2  3 11 17 24 19  9  5  2
100
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  11110 1841000 4807000 4775000 7835000 9856000
     \endverbatim
     </li>
     <li> cutoff=10^8:
     \verbatim
 2  3  4  5  6 22 23 24 25 26 27 28 29 30
33 50 39 22  3  1  1  9 23 40 43 53 11  4
332
  2   3   4   5   6  21  22  23  24  25  26  27  28  29  30
 84 113 112  48  18   2   3  13  19  62 117 189 162  51   7
1000
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Using the palindromic solution for n=328:
    <ol>
     <li> Starting search with the best palindromic solution:
     \verbatim
> k1=4 k2=10 n=328 alg="adaptg2wsat" cutoff=1000000 runs=10 expdate="2011-03-26-101957"; export k1 k2 n; cat AltExp/Exp_PdVanderWaerden_2-${k1}-${k2}_${alg}-${runs}-${cutoff}_${expdate}/VanDerWaerden_pd_2-${k1}-${k2}_${n}.cnf_sol | PdExtend-O3-DNDEBUG ${n} > solution

> RunVdWk1k2 ${k1} ${k2} ${n} irots 100 10000000 solution
UNSAT for n=329
 2  4 27 28 29 30 31 32 33 34 35
 1  1  1  1  3  7 22 14 26 20  4
100
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Starting from scratch, with adaptg2wsat:
   \verbatim
> k1=4 k2=10 n0=11 alg="adaptg2wsat" runs=1000 cutoff=400000000; nohup RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} &
# Finds all solutions in the first round with the old solution, except for
# n=326, which needed 52 round, and n=327, which needed 3 rounds with the old
# solution.
> E=read_ubcsat("VanDerWaerden_2-4-10_329.cnf_OUT")
  1   2   3   4   5  22  23  24  25  26
 22 330 189  73  13   1   1   3   4   1
637
   \endverbatim
   </li>
   <li> Starting with the solution of n=328, with ddfw:
   \verbatim
> k1=4 k2=10 n0=328 alg="ddfw" runs=1000 cutoff=100000000; RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} VanDerWaerden_2-${k1}-${k2}_${n0}.sol
> E=read_ubcsat("VanDerWaerden_2-4-10_329.cnf_OUT")
 2  3  4  5  6 25
19 14 11  2  1  1
48
   \endverbatim
   </li>
  </ul>


  \todo vdw_2^pd(4,10) = (320,329)
  <ul>
   <li> Established by "CRunPdVdWk1k2 4 10 minisat-2.2.0". </li>
   <li> Evaluation:
   \verbatim
> E=read_crunpdvdw_minisat()
$t:
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
0.000e+00 1.999e-03 6.998e-03 4.578e+00 9.289e-01 3.405e+02
sd= 23.05140
      95%       96%       97%       98%       99%      100%
 15.73858  22.72751  35.82062  47.20643  77.42341 340.51000
sum= 1451.081
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
      0       0      33  129100   41710 7534000
sd= 541492.6
      95%       96%       97%       98%       99%      100%
 536310.8  779823.2 1081312.6 1446754.1 2001610.9 7533850.0
sum= 40918332
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -8.8299e-01  1.4898e-01  -5.927 8.092e-09 ***
E$cfs        4.2303e-05  2.6802e-07 157.835 < 2.2e-16 ***
R-squared: 0.9875
   \endverbatim
   The plot shows clearly that even problems (i.e., even v) are harder. </li>
   <li> Certificates:
    <ol>
     <li> n=319:
     \verbatim
7,10,20,23,25,26,27,34,39,47,
50,56,58,60,61,65,66,68,70,76,
79,87,92,94,99,100,101,103,104,106,
107,116,117,119,120,122,123,124,129,131,
136,144,147,153,155,157,158,160
     \endverbatim
     </li>
     <li> n=328:
     \verbatim
10,19,23,25,29,30,33,34,36,40,
42,44,45,51,58,67,68,77,81,84,
90,91,93,95,99,101,102,105,106,107,
110,112,120,122,125,126,127,130,131,133,
137,139,141,142,148,151,155,164
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdWk1k2 4 10 gsat-tabu 10 200000" yields
   \verbatim
Break point 1: 315
Break point 2: 316
   \endverbatim
   while "RunPdVdWk1k2 4 10 gsat-tabu 10 1000000" yields
   \verbatim
Break point 1: 316
Break point 2: 329
   \endverbatim
   By "RunPdVdWk1k2 4 10 adaptg2wsat 10 200000" we get
   \verbatim
Break point 1: 315
Break point 2: 320
   \endverbatim
   while by "RunPdVdWk1k2 4 10 adaptg2wsat 10 1000000" we get
   \verbatim
Break point 1: 320
Break point 2: 329
   \endverbatim
   It seems that adaptg2wsat is better than gsat-tabu. </li>
  </ul>

*/
