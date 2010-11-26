/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/32.hpp
  \brief On investigations into vdw_2(3,32)


  \todo vanderwaerden_2(3,32) > 1006
  <ul>
   <li> n=951: rots found as solution, adapted from n=939 (run 15), with
   certificate:
   \verbatim
22,37,59,60,69,77,106,111,113,114,
119,137,139,158,176,195,208,222,230,244,
245,254,259,261,269,299,304,306,318,319,
328,335,336,359,361,370,372,398,410,415,
433,446,447,470,472,476,491,509,513,540,
544,563,565,592,594,602,615,632,637,651,
669,674,688,689,694,705,726,731,740,748,
750,766,777,799,800,809,840,851,873,877,
890,921,948
   \endverbatim
   </li>
   <li> n=957: rots with cutoff=10^7 found a solution, adapted from n=952
   (run 73, osteps=7574800), with certificate:
   \verbatim
7,15,40,52,71,75,81,94,118,125,
151,168,170,192,199,205,219,236,244,257,
260,279,281,293,294,311,330,334,345,355,
368,382,404,405,422,427,447,451,456,482,
484,493,496,501,503,519,525,538,558,569,
577,590,606,612,614,627,644,663,673,678,
680,688,700,701,706,715,718,741,747,755,
780,789,811,815,826,828,848,852,865,891,
903,914,926
   \endverbatim
   </li>
   <li> n=981: rots with cutoff=2*10^7 found a solution, adapted from n=967
   (run 2, osteps=10924773), with certificate:
   \verbatim
13,26,30,55,67,72,85,95,106,109,
141,143,166,174,200,203,217,220,233,235,
241,243,258,278,285,295,307,314,344,346,
352,354,359,369,389,406,418,420,437,439,
470,476,479,502,507,531,536,548,553,576,
585,590,610,611,618,624,628,647,664,677,
685,687,696,702,714,721,722,724,739,753,
770,790,803,805,812,813,835,864,881,883,
899,901,907,918,943,973
   \endverbatim
   </li>
   <li> n=982: rots with cutoff=2*10^7 found a solution (run 215,
   osteps=11914827), with certificate:
   \verbatim
25,38,59,75,76,83,96,99,105,118,
132,138,147,157,170,192,207,221,224,229,
247,253,260,261,268,286,290,317,321,334,
335,354,358,360,371,397,414,428,443,451,
483,490,503,512,519,525,554,556,557,562,
564,577,586,591,601,614,619,623,650,668,
687,688,693,697,702,730,747,749,761,765,
776,786,798,802,804,815,816,845,876,889,
913,926,927,959
   \endverbatim
   </li>
   <li> n=983:
    <ol>
     <li> rots, cutoff=2*10^7:
     \verbatim
 1  2  3  4  5  6  7  8 10 11 13 14 15 16 
 9 41 41 15  3  1  1  1  3  1  3  4  8 10 
141
 1  2  3  4  5  6 12 13 14 15 16 
 3 17 17  6  1  1  1  1  3  3  1 
54
     \endverbatim
     </li>
     <li> In run 223 with cutoff=2*10^7 a solution was found, which with
     small modifications yielded a solution for n=985 (see below). </li>
    </ol>
   </li>
   <li> n=985: rots with cutoff=2*10^7 found a solution (modifified from
   m=983), with certificate:
   \verbatim
16,36,53,68,71,82,110,114,129,145,
164,179,182,193,195,196,215,230,232,258,
277,289,295,306,307,326,330,336,341,351,
364,381,388,400,410,423,441,454,462,475,
492,497,517,526,528,529,534,548,563,565,
591,608,623,626,628,632,637,639,640,659,
669,674,702,721,733,743,756,758,774,780,
787,808,825,844,845,848,850,867,869,885,
906,919,936,959,965
   \endverbatim
   </li>
   <li> n=993: rots with cutoff=2*10^7 found a solution (modifified from
   m=986), with certificate:
   \verbatim
8,24,41,46,66,75,103,114,115,122,
144,149,157,175,196,225,233,245,246,251,
255,260,283,288,292,299,300,305,334,337,
360,362,393,394,397,403,410,411,418,430,
447,448,455,471,477,490,504,508,527,529,
556,566,579,584,601,630,632,638,653,667,
689,690,693,695,726,730,732,736,743,744,
751,773,804,815,818,837,838,845,860,884,
889,891,911,928,929,949,966,991
   \endverbatim
   </li>
   <li> n=994: rots with cutoff=2*10^7 yielded
   \verbatim
  1   2   3   4   5   6   7   8   9  10  13  14  15  16  17  18  19 
  7 110 184  70  12   6   3   1   1   1   2   5   8   9  17   9   4 
449 
   \endverbatim
   </li>
   <li> n=999: rots with cutoff=5*10^7 found a solution (modifified from
   n=991, run 79, osteps=8284852), with certificate:
   \verbatim
7,20,49,57,66,93,103,104,116,131,
155,161,172,174,178,209,214,226,237,264,
266,283,285,288,309,311,320,322,326,338,
345,353,375,390,400,420,422,433,436,449,
451,459,488,493,494,501,507,511,538,542,
567,570,599,604,616,621,653,655,659,673,
678,695,716,727,733,745,753,755,766,770,
784,792,797,819,834,843,856,866,892,900,
901,917,930,951,958,974,975
   \endverbatim
   </li>
   <li> n=1006: rots with cutoff=5*10^7 found a solution (modifified from
   n=1000, run 9, osteps=10231663), with certificate:
   \verbatim
16,43,49,61,66,81,86,103,132,155,
158,164,172,192,195,197,214,228,234,238,
246,251,265,275,290,319,327,343,354,356,
357,376,377,380,394,401,423,451,454,456,
465,473,488,502,505,530,541,547,549,567,
578,579,584,613,623,635,636,645,652,660,
672,676,687,689,690,719,734,752,756,769,
783,789,806,820,824,835,858,861,874,882,
911,932,935,941,946,969,978
   \endverbatim
   </li>
  </ul>


  \todo Palindromic version:
  <ul>
   <li> Best ubcsat-algorithm:
   \verbatim
> PdVanderWaerdenCNF-O3-DNDEBUG 3 32 1007 > VanDerWaerden_pd_2-3-32_1007.cnf

> E=run_ubcsat("VanDerWaerden_pd_2-3-32_1007.cnf",runs=100,cutoff=1000000)

> plot(E$alg,E$min)
> table(E$min[E$alg=="gsat_tabu"])
 4  5  6  7  8 10 
 1 10 38 35 15  1 
> table(E$FlipsPerSecond[E$alg=="gsat_tabu"])
55618 
  100 
> table(E$min[E$alg=="samd"])
 5  6  7  8  9 11 
 6 37 39 14  3  1 
> table(E$FlipsPerSecond[E$alg=="samd"])
55488 
  100 
> table(E$min[E$alg=="rots"])
 5  6  7  8  9 10 11 
 1  1  7 22 31 27 11 
> table(E$FlipsPerSecond[E$alg=="rots"])
46627 
  100 
   \endverbatim
   shows clearly that gsat-tabu is best, followed by samd. (Currently
   eval_ubcsat_dataframe(E) is not usable due to a bug in run_ubcsat.) </li>
   <li> n=1007:
    <ol>
     <li> gsat-tabu with cutoff=10^7:
     \verbatim
  4   5   6   7   8  10 
110 180  71   3   3   1 
368 
     \endverbatim
     doesn't look promising. </li>
     <li> gsat-tabu with cutoff=10^8:
     \verbatim
 4  5  6  7  8 
34  9  2  2  1 
48 
     \endverbatim
     seems unsatisfiable. </li>
    </ol>
   </li>
  </ul>

*/
