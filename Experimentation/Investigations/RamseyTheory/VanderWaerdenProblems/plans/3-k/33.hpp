// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/33.hpp
  \brief On investigations into vdw_2(3,33)


  \todo vanderwaerden_2(3,33) > 1063
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,33) = 1064. </li>
   <li> n=997: rots found a solution, with certificate
   \verbatim
16,39,41,68,78,90,96,100,107,127,
150,152,165,174,181,202,218,219,224,242,
256,275,279,281,290,312,313,318,322,353,
364,372,387,390,396,403,429,433,440,441,
446,448,475,478,498,503,507,535,551,552,
557,559,571,575,596,612,614,623,645,646,
655,682,686,697,707,720,723,736,756,774,
779,781,808,810,816,836,845,847,873,892,
904,922,927,929,951,984
   \endverbatim
   </li>
   <li> n=1004: rots found a solution (cutoff=10^7), with certificate
   \verbatim
27,38,68,89,92,94,112,126,131,138,
149,165,175,196,211,237,240,248,270,289,
311,313,314,316,326,351,359,364,397,401,
408,418,422,425,433,437,459,464,475,492,
498,511,512,529,535,556,570,573,581,582,
593,603,609,622,647,649,659,684,692,693,
723,730,741,751,760,766,770,771,794,808,
815,825,841,844,852,866,877,908,915,919,
942,945,977,993
   \endverbatim
   </li>
   <li> n=1018: rots found a solution (cutoff=2*10^7), with certificate
   \verbatim
9,24,46,59,61,78,80,96,117,148,
157,170,178,189,191,195,209,232,246,252,
254,263,288,289,291,306,320,343,363,370,
402,407,413,416,436,439,450,466,473,485,
518,522,524,528,542,547,561,565,577,596,
601,614,622,638,639,659,676,690,703,712,
725,727,732,733,735,740,744,769,772,799,
801,818,843,851,880,894,897,910,918,920,
925,934,947,949,957,986
   \endverbatim
   </li>
   <li> n=1021: rots found a solution (cutoff=2*10^7), adapted from n=1019,
   with certificate
   \verbatim
3,36,45,68,79,95,105,114,116,120,
132,134,151,176,184,190,193,209,213,227,
230,231,243,258,262,280,287,295,305,324,
327,336,362,364,369,393,399,416,430,438,
449,452,453,473,475,504,510,541,546,563,
576,583,597,628,632,634,637,638,650,665,
687,694,706,731,732,734,748,749,769,785,
800,813,817,837,843,850,854,856,880,897,
911,919,924,930,934,946,965,970,983,1008
   \endverbatim
   </li>
   <li> n=1022: rots found a solution (cutoff=2*10^7, run 76, osteps=12590122),
   with certificate
   \verbatim
15,19,40,41,66,73,93,114,115,140,
148,152,161,177,195,198,200,204,226,237,
258,259,271,278,296,299,308,325,336,346,
369,382,383,407,410,417,419,422,447,448,
454,457,481,500,528,533,547,559,565,570,
596,604,621,628,629,648,658,678,679,681,
685,702,707,715,739,740,755,759,787,790,
806,812,813,817,843,863,864,888,891,898,
925,928,929,944,961,972,1002,1014
   \endverbatim
   </li>
   <li> n=1025: rots found a solution (cutoff=2*10^7), adapted from n=1023,
   with certificate
   \verbatim
31,44,48,54,68,87,113,122,127,148,
155,161,165,198,214,216,221,222,224,251,
259,261,270,295,303,312,313,332,346,349,
362,377,383,399,414,424,455,460,480,481,
483,498,517,518,520,529,549,566,591,592,
623,629,631,636,642,658,660,668,695,714,
716,719,720,747,751,753,756,757,769,776,
808,825,827,830,845,851,868,890,905,919,
924,932,964,975,998,1006,1015
   \endverbatim
   </li>
   <li> n=1026:
    <ol>
     <li> rots, cutoff=2*10^7:
     \verbatim
 1  2  3  4  5  6  9 12 13 14 15 16 
 1 11 11  1  1  2  1  1  3  2  4  3 
41 
 1  2  3  4  8 12 13 14 15 16 
 6  8 15  5  1  1  3  4  3  5 
51
     \verbatim
     </li>
     <li> A solution was found (run 145), which with modifications reached
     n=1033. </li>
    </ol>
   </li>
   <li> n=1033:
    <ol>
     <li> rots found a solution (cutoff=2*10^7), adapted from n=1026
     (with considerable modifications), with certificate
     \verbatim
3,34,45,67,68,90,104,119,129,142,
147,151,164,166,182,195,225,230,233,256,
258,269,270,289,290,293,304,327,336,344,
351,363,388,400,406,410,417,441,452,460,
462,475,491,499,517,528,529,534,548,549,
571,591,602,632,637,640,645,663,677,684,
706,711,734,737,739,743,748,770,774,793,
796,813,845,850,862,867,896,898,899,918,
922,933,943,965,972,973,998,1007
     \endverbatim
     </li>
     <li> Another solution was found by rots (cutoff=5*10^7), adapted from
     n=1030 (without modifications), with certificate:
     \verbatim
9,28,38,45,46,69,71,82,112,117,
127,143,145,156,157,164,180,191,213,214,
231,250,251,260,293,297,302,328,347,349,
362,378,379,386,415,421,435,436,458,460,
478,497,519,524,532,546,547,550,552,561,
564,589,593,624,630,635,637,638,661,675,
694,695,704,731,737,754,774,783,805,809,
822,823,828,846,848,852,860,865,894,904,
917,933,939,941,970,990,991,1015
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=1037: a solution was found by rots (cutoff=2*10^7), adapted from
   n=1034 (run 490), with certificate:
   \verbatim
29,32,39,68,74,94,103,105,106,130,
140,162,163,179,200,209,214,240,246,254,
273,274,277,279,291,311,316,320,335,347,
365,370,372,388,401,427,444,458,462,483,
495,501,505,520,542,547,549,573,584,586,
587,606,607,612,616,644,653,660,680,703,
718,721,735,740,742,755,769,779,791,795,
806,832,834,841,845,853,871,882,888,890,
917,925,949,956,957,982,999,1019
   \endverbatim
   </li>
   <li> n=1038: a solution was found by rots (cutoff=5*10^7), adapted from
   n=1034 (run 50), with certificate:
   \verbatim
14,16,38,57,65,85,88,101,102,107,
125,127,146,158,159,183,195,196,218,242,
247,249,250,255,257,279,286,294,312,343,
353,358,360,361,380,381,384,390,395,421,
423,454,455,477,492,506,509,532,545,569,
571,580,588,590,612,627,639,640,656,662,
691,693,694,701,728,731,738,756,787,791,
793,797,805,810,839,861,862,865,867,878,
884,902,904,915,936,949,973,990,995,1009
   \endverbatim
   while cutoff=2*10^7 yielded
   \verbatim
 1  2  3  4  5  6  7  8 10 11 12 13 14 15 16 17 18 
 3 45 41 18  2  3  1  2  1  1  1  7  6  3  8  6  1 
149 
   \endverbatim
   </li>
   <li> n=1061: a solution was found by rots (cutoff=5*10^7), adapted from
   n=1042 (run 39, osteps=6165331), with certificate:
   \verbatim
15,33,60,73,92,103,106,119,126,140,
156,163,171,180,201,212,230,232,245,251,
255,274,277,286,288,314,323,348,349,356,
365,380,388,393,402,423,440,452,454,460,
476,489,491,513,514,534,547,551,565,578,
600,607,621,637,644,658,689,693,699,718,
721,732,736,739,755,758,767,787,793,804,
810,830,837,861,874,880,883,896,906,933,
941,943,948,952,957,972,977,994,1007,1015,
1017,1046
   \endverbatim
   </li>
   <li> n=1063: a solution was found by rots (cutoff=5*10^7), adapted from
   n=1062 (run 96, osteps=34434161), with certificate:
   \verbatim
30,37,52,64,86,101,104,123,125,141,
154,180,197,199,211,212,217,228,234,248,
265,286,289,303,326,332,337,339,359,369,
374,384,400,402,421,433,434,439,443,448,
451,480,487,495,525,530,550,561,562,581,
596,598,604,619,636,665,672,693,702,707,
715,730,735,754,766,776,781,784,789,807,
809,813,828,858,877,878,881,892,895,915,
918,929,957,976,989,994,1015,1025,1050
   \endverbatim
   </li>
   <li> n=1064:
    <ol>
     <li> rots with cutoff=5*10^7:
     \verbatim
  1   2   3   4   5  18
 12  99 183  28   2   1
325
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Palindromic version:
  <ul>
   <li> 1002 < vanderwaerden_g([3,33],"pd") =(conj) 1003 =(conj)
   vanderwaerden([3,33]) - 61. </li>
   <li> vanderwaerden_g([3,33],"pdsat") =(conj) 992. </li>
   <li> Best ubcsat-algorithm:
   \verbatim
> PdVanderWaerdenCNF-O3-DNDEBUG 3 33 1064

> E=run_ubcsat("VanDerWaerden_pd_2-3-33_1064.cnf",runs=100,cutoff=1000000)
   \endverbatim
   saps and sapsnr yield segmentation faults, and so no evaluation was
   possible (easily), however it seems that again gsat-tabu and samd are
   best. </li>
   <li> n=1064:
    <ol>
     <li> gsat-tabu with cutoff=10^7:
     \verbatim
  2   3   4   5   6   7  10  11  13 
  1  53 272  53   3   1   1   1   1 
386 
     \endverbatim
     doesn't look promising. </li>
     <li> gsat-tabu with cutoff=10^8:
     \verbatim
 3  4  5  6 
27 14  4  1 
46 
     \endverbatim
     seems unsatisfiable. </li>
    <ol>
   </li>
   <li> "RunPdVdW3k 33 34 999 gsat-tabu 100 10000000" finds 1<=n<=996 sat,
   and then
   \verbatim
997 0
998 1
999 0
   \endverbatim
   where the solutions have been found within at most the first 30 runs.
   </li>
   <li> "RunPdVdW3k 33 gsat-tabu 100 10000000" yields
   \verbatim
Break point 1: 992
Break point 2: 1003
   \endverbatim
   where all solutions were found within in the first 7 runs. </li>
  </ul>

*/
