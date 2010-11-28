/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/20.hpp
  \brief On investigations into vdw_2(3,20)


  \todo vanderwaerden_2(3,20) >= 389
  <ul>
   <li> Experience with k=19 is that gsat-tabu is best, and that seems to be
   true here too. </li>
   <li> Evaluating
   \verbatim
> E = run_ubcsat("VanDerWaerden_2-3-20_385.cnf", runs=100,cutoff=100000,monitor=TRUE)
   \endverbatim
   gsat-tabu, adaptnovelty+ and rnoveltyp seem best (all reaching min=2). </li>
   </li>
   <li> Now using cutoff=10^6: gsat-tabu seems best, and also finds as only
   solver a solution (this could be luck, and rnovelty as the second solver
   could be better). </li>
   <li> n=386:
    <ol>
     <li> cutoff=2*10^6 with gsat-tabu: found a solution in run 2 
     (seed=3063820134). </li>
    </ol>
   </li>
   <li> n=386: gsat-tabu finds a solution (modified stepwise from n=379), with
   certificate:
   \verbatim
18,30,35,43,45,50,64,74,79,81,
96,97,101,102,104,114,133,143,148,150,
156,160,171,188,197,214,225,229,235,237,
242,252,271,281,283,284,288,289,304,306,
311,321,335,340,342,350,355,367
   \endverbatim
   Another run with gsat-tabu found also this solution, this time modified
   stepwise from n=383. </li>
   <li> n=387:
    <ol>
     <li> cutoff=2*10^6 with gsat-tabu:
     \verbatim
 2  3  4
14 50 36
100
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  26010  189900  596200  702300 1104000 1967000
     \endverbatim
     </li>
     <li> cutoff=4*10^6 with gsat-tabu:
     \verbatim
 2  3  4
30 56 14
100
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  41100  614600 1393000 1479000 2188000 3910000
     \endverbatim
     </li>
     <li> cutoff=4*10^6 with rnovelty: found a solution in run 62
    (seed=1299962486, osteps=1466958). </li>
     <li> rnovelty might be better:
     \verbatim
 0  2  3  4
 1 21 29 11
62
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  14410  614000 1303000 1467000 2288000 3844000
     \endverbatim
     </li>
     <li> For comparison: adaptnovelty+:
     \verbatim
>  ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 8000000 -i VanDerWaerden_2-3-20_387.cnf -solve | tee VanDerWaerden_2-3-20_387.cnf_AUS4
 1  2  3  4
 1 51 44  4
100
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
 193900 1162000 2486000 2976000 4207000 7925000
     \endverbatim
     Unclear; but it seems the cutoff has to be increased to 8*10^6. </li>
     <li> Using RunVdW3k with gsat-tabu, runs=100 and cutoff=8*10^6 finds a
     solution for n=386, but not for n=387:
     \verbatim
 1  2  3  4 
 1 41 55  3 
100 
     \endverbatim
     rnovelty with the same parameters has bigger problems, and finds only
     solutions up to n=381. </li>
    </ol>
   </li>
   <li> n=388
    <ol>
     <li> rnovelty with cutoff=8*10^6: found a solution in run 33
     (seed=2441787444, osteps=4412722). Reproduction does not succeed (new
     order of clauses etc.), but we just see whether we can find just new
     solutions. </li>
     <li> gsat-tabu with cutoff=8*10^6:
     \verbatim
 1  2  3  4
 3 32 62  3
100
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  25570  924300 2283000 2818000 4242000 7901000
     \endverbatim
     </li>
     <li> RunVdW3k with gsat-tabu, cut=2*10^7 finds a solution in run 1, with
     certificate
     \verbatim
20,32,37,45,47,52,66,76,81,83,
98,99,103,104,106,116,135,145,150,152,
158,162,173,190,199,216,227,231,237,239,
244,254,273,283,285,286,290,291,306,308,
313,323,337,342,344,352,357,369
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=389: 
    <ol>
     <li> cutoff=8*10^6:
     \verbatim
> ubcsat-okl -alg rnovelty -runs 100 -cutoff 8000000 -i VanDerWaerden_2-3-20_389.cnf -solve | tee VanDerWaerden_2-3-20_389.cnf_AUS
 2  3  4
42 53  5
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  31700 1449000 2589000 2981000 4027000 7955000
>  ubcsat-okl -alg gsat-tabu -runs 100 -cutoff 8000000 -i VanDerWaerden_2-3-20_389.cnf -solve | tee VanDerWaerden_2-3-20_389.cnf_AUS2
 1  2  3  4
 1 29 60 10
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  18300  631700 1531000 2300000 3661000 7883000
     \endverbatim
     It seems rnovelty might be a bit better, however this doesn't seem to
     be true (see below). </li>
     <li> gsat-tabu, cutoff=2*10^7:
     \verbatim
  1   2   3   4   5 
 13 315 164   6   2 
500 
     \endverbatim
     </li>
     <li> cutoff = 10^8:
     \verbatim
> nohup ubcsat-okl -alg rnovelty -runs 1000 -cutoff 100000000 -i VanDerWaerden_2-3-20_389.cnf -solve > VanDerWaerden_2-3-20_389.cnf_AUS
> E=read_ubcsat("VanDerWaerden_2-3-20_389.cnf_AUS")
  1   2
 28 371
399
> summary(E$osteps)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
   67020  3267000  9072000 15800000 21300000 92970000
> summary(E$osteps[E$min==1])
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
  551000 24360000 57580000 49740000 72950000 92970000

> ubcsat-okl -alg rnovelty -runs 1000 -cutoff 100000000 -i VanDerWaerden_2-3-20_389.cnf -solve | tee VanDerWaerden_2-3-20_389.cnf_AUS3
> E = read_ubcsat("VanDerWaerden_2-3-20_389.cnf_AUS3")
  1   2
 67 933
1000
> summary(E$osteps)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
   17140  3765000  9559000 15030000 18850000 98290000

> nohup ubcsat-okl -alg gsat-tabu -runs 1000 -cutoff 100000000 -i VanDerWaerden_2-3-20_389.cnf -solve > VanDerWaerden_2-3-20_389.cnf_AUS4 &
> E=read_ubcsat("VanDerWaerden_2-3-20_389.cnf_AUS4")
  1   2   3   4   5   6
165 750  71  10   3   1
1000
> summary(E$osteps)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
    7145  4473000 13520000 21760000 31660000 99550000

> nohup ubcsat-okl -alg adaptnovelty+ -runs 1000 -cutoff 100000000 -i VanDerWaerden_2-3-20_389.cnf -solve > VanDerWaerden_2-3-20_389.cnf_AUS5 &
> E = read_ubcsat("VanDerWaerden_2-3-20_389.cnf_AUS5");
  1   2   3
 64 520   6
590
> summary(E$osteps)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
   51760  5938000 15350000 22130000 34500000 99970000

     \endverbatim
     So actually gsat-tabu is better than rnovelty for higher cutoff, and
     adaptnovelty+ gets better, but still worse than gsat-tabu. </li>
    </ol>
   </li>
  </ul>


  \todo Palindromic numbers
  <ul>
   <li> vanderwaerden_g([3,20],"pd") = 389 = vanderwaerden([3,20])
   (assuming that all n>388 are unsolvable). </li>
   <li> precosat236 (precosat570):
    <ol>
     <li> n=389: unsat, 332s (337s). </li>
     <li> n=388: sat, 149s (781s). </li>
     <li> n=387: unsat, 361s (307s). </li>
     <li> n=386: sat, 1112s (1215s). </li>
     <li> n=385: unsat, 332s (319s). </li>
     <li> n=384: sat, 320s (659s). </li>
     <li> n=383: unsat, 325s (310s). </li>
     <li> n=382: sat, 237s (989s). </li>
     <li> n=381: unsat, 333s (322s). </li>
    </ol>
    It doesn't look as if there would be an incentive to use version 570
    instead of 236.
   </li>
   <li> Again, for all odd n very soon one variable is fixed --- which?
   However this only happens with version 236, not with version 570 (there it
   mostly takes about 23s to find this forced assignment (sometimes less,
   sometimes considerably more)! </li>
   <li> Other running times (for n=388):
    <ol>
     <li> minisat2 (2370s) </li>
     <li> picosat913 (455s) </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 20 20 389 gsat-tabu 100 1000000": all 1<=n<=380 sat,
   and then
   \verbatim
381 0
382 1
383 0
384 1
385 0
386 1
387 0
388 1
389 0
   \endverbatim
   </li>
  </ul>

*/
