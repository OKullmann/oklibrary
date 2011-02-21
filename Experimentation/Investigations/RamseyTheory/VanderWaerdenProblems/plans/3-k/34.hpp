// Oliver Kullmann, 6.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/34.hpp
  \brief On investigations into vdw_2(3,34)


  \todo vanderwaerden_2(3,34) > 1143
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,34) = ???. </li>
   <li> Starting search with the best palindromic solution:
   \verbatim
> k=34 n=1078 cutoff=10000000 expdate="2010-11-30-182015"; export k n; cat AltExp/Exp_PdVanderWaerden_2-3-${k}_gsat-tabu-100-${cutoff}_${expdate}/VanDerWaerden_pd_2-3-${k}_${n}.cnf_sol | PdExtend-O3-DNDEBUG ${n} > solution

> RunVdW3k ${k} ${n} rots 100 100000000 solution

   \endverbatim
   </li>
   <li> n=1079: Solution was adopted, with certificate
   \verbatim
23,24,34,57,71,97,98,120,129,145,
157,176,177,182,203,205,208,214,236,245,
250,274,279,288,310,321,342,347,348,351,
379,390,395,398,425,427,453,462,472,490,
495,496,509,531,532,536,543,547,548,570,
583,584,589,607,617,626,652,654,681,684,
689,700,728,731,732,737,758,769,791,800,
805,829,834,843,865,871,874,876,897,902,
903,922,934,950,959,981,982,1008,1022,1045,
1055,1056
   \endverbatim
   </li>
   <li> n=1128 found satisfiable (rots, cutoff=10^8) with certificate
   \verbatim
31,56,73,89,93,106,137,138,145,158,
163,174,180,200,209,231,235,241,246,249,
256,272,291,322,323,330,348,357,365,380,
394,416,433,454,457,459,463,494,527,545,
552,565,568,581,611,616,626,639,644,648,
655,661,663,675,681,690,693,698,722,727,
749,766,772,787,790,792,803,804,809,811,
827,860,866,885,897,907,912,938,944,949,
975,977,994,1018,1033,1036,1046,1062,1083,1100
   \endverbatim
   </li>
   <li> n=1129
    <ol>
     <li> rots, cutoff=10^8:
     \verbatim
 1  2  3
 6 72 22
100
     \endverbatim
     </li>
     <li> A solution was found in 164 runs (osteps=172017857, cutoff=2*10^8):
     \verbatim
29,36,52,77,86,87,121,123,130,163,
177,184,188,198,204,222,241,258,259,263,
271,295,300,311,333,336,345,369,373,380,
382,389,417,422,426,447,448,457,485,491,
494,496,500,510,528,530,555,559,584,592,
621,628,642,665,666,679,685,695,703,718,
743,744,752,753,755,780,781,787,813,824,
826,833,850,863,866,891,892,903,907,917,
924,928,938,966,972,975,1009,1011,1028,1036,
1055,1085,1092,1102
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=1133
    <ol>
     <li> solution was found by rots, adapted from n=1129:
     \verbatim
3,36,37,52,77,86,87,103,121,124,
130,163,177,184,188,198,204,222,241,258,
259,263,288,295,300,311,333,336,345,369,
370,380,382,389,417,422,426,447,448,457,
485,491,493,494,496,500,528,530,555,559,
584,592,621,628,632,642,665,666,679,685,
695,718,743,744,752,753,755,780,781,787,
813,824,826,833,843,850,863,866,892,907,
917,924,938,966,972,975,1002,1009,1028,1036,
1048,1051,1055,1085,1092,1109
     \endverbatim
     </li>
     <li> adaptg2wsat, cutoff=10^7:
     \verbatim
  1   2   3   4
  7  36 103  24
170
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=1134:
    <ol>
     <li> rots, cutoff=2*10^8:
     \verbatim
 1  2  3
 7 32  5
44
     \endverbatim
     </li>
     <li> adaptg2wsat finds in run 645 a solution (cutoff=2*10^7,
     osteps=12340158). </li>
    </ol>
   </li>
   <li> n=1143: a solution was found (adapted from n=1134), with certificate:
   \verbatim
33,41,43,52,82,106,110,111,122,126,
144,147,157,163,180,196,217,218,221,230,
231,233,265,284,291,304,311,329,333,341,
342,369,376,379,381,406,407,415,418,422,
444,469,480,492,513,524,543,550,551,580,
588,592,607,635,637,638,659,661,666,674,
677,698,699,728,736,755,765,776,785,814,
820,822,823,825,829,857,860,866,899,924,
931,957,968,971,995,998,1000,1010,1024,1035,
1047,1068,1081,1098,1106,1110
   \endverbatim
   </li>
  </ul>


  \todo Palindromic version:
  <ul>
   <li> 1078 < vanderwaerden_g([3,34],"pd") =(conj) 1079 =(conj)
   vanderwaerden([3,34]) - ???. </li>
   <li> vanderwaerden_g([3,34],"pdsat") =(conj) 1046. </li>
   <li> Certificates:
    <ol>
     <li> n=1045:
     \verbatim
     \endverbatim
     </li>
     <li> n=1078:
     \verbatim
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 34 gsat-tabu 100 10000000" (old version) yields
   \verbatim
Break point 1: 1046
Break point 2: 1079
   \endverbatim
   where all solutions were found within in the first 73 runs (so the second
   break-point is rather weak). </li>
   <li> Rerunning it with doubled cutoff, and using the previous solutions
   properly. We also need the ability to start late. </li>
   <li> n=1045:
    <ol>
     <li> From scratch, adaptg2wsat with cutoff=2*10^6:
     \verbatim
 1  2  3  4  5  6 
 2  2 15 31 41  9 
100 
     \endverbatim
     </li>
     <li> From scratch, adaptg2wsat with cutoff=4*10^6:
     \verbatim
 2  3  4  5
 2 27 56 15
100
     \endverbatim
     </li>
     <li> From scratch, ddfw with cutoff=4*10^6:
     \verbatim
 1  2  3  4  5 
 2 17 39 40  2 
100 
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=1078:
    <ol>
     <li> From scratch, adaptg2wsat with cutoff=2*10^6:
     \verbatim
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>

*/
