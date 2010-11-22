/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/26.hpp
  \brief On investigations into vdw_2(3,26)


  \todo vanderwaerden_2(3,26) >= 727
  <ul>
   <li> The conjecture is vanderwaerden_2(3,26) = 727. </li>
   <li> Search starting with n=600:
   \verbatim
OKplatform> RunVdW3k 26 600 gsat-tabu 100 100000000
   \endverbatim
   </li>
   <li> Perhaps also here the cutoff-value is too large:
   \verbatim
OKplatform> RunVdW3k 26 655 gsat-tabu 1000 10000000
   \endverbatim
   </li>
   <li> n=659 found satisfiable with certificate
   \verbatim
13,26,27,46,51,64,80,91,113,116,
120,128,138,142,157,159,166,179,188,207,
215,224,237,244,246,265,275,283,290,311,
316,324,340,348,352,353,374,377,381,389,
399,418,420,427,440,449,457,468,476,485,
505,507,522,526,536,544,548,551,572,573,
585,607,614,627,651
   \endverbatim
   </li>
   <li> n=677 found satisfiable with certificate
   \verbatim
5,12,14,38,39,58,68,76,79,91,
105,117,123,125,128,132,150,162,163,179,
191,197,200,202,216,223,243,260,264,273,
274,280,301,302,308,311,334,339,345,347,
350,371,372,391,401,409,412,424,438,450,
456,458,461,465,483,486,495,496,512,520,
524,530,533,535,549,556,576,593,606,607,
613,634,635,641,644,667,672
   \endverbatim
   </li>
   <li> Restarting the experiment, now using the solution found for n=677
   as initial assignment:
   \verbatim
OKplatform> cat Exp_VanderWaerden_2-3-26_2010-10-12-161754_655/VanDerWaerden_2-3-26_677.cnf_OUT | extract_solution_ubcsat > Solution_n677
OKplatform> RunVdW3k 26 678 rots 1000 5000000 Solution_n677
   \endverbatim
   </li>
   <li> n=726:
    <ol>
     <li> Found satisfiable with certificate
     \verbatim
11,35,46,48,69,74,86,93,105,106,
109,115,122,128,152,157,159,167,185,202,
204,216,217,220,222,226,231,234,253,257,
263,278,291,308,313,333,342,344,345,350,
364,379,381,402,407,426,438,439,442,448,
455,461,485,490,492,500,516,518,535,537,
549,550,553,555,559,564,567,586,596,611,
624,641,646,666,675,678,697,701
     \endverbatim
     </li>
     <li> rots with cutoff=10^7 found another solution in run 27:
     \verbatim
26,31,49,51,68,82,86,99,120,125,
144,156,157,160,162,173,174,193,197,203,
208,211,218,231,253,268,277,282,284,285,
290,314,319,321,329,347,364,366,378,382,
384,388,393,401,415,419,432,453,458,470,
477,489,490,493,495,506,507,526,530,536,
541,551,564,581,586,601,610,615,617,618,
623,647,652,654,680,699,711,721,726
     \endverbatim
     </li>
     <li> rots with cutoff=10^7 found another solution, caried over from n=725,
     very close to the above first solution:
     \verbatim
17,35,46,48,69,74,86,93,106,109,
115,122,128,152,157,159,167,185,202,204,
216,217,220,222,226,231,234,253,257,263,
278,291,308,313,333,342,344,345,350,364,
379,381,402,407,419,426,438,439,442,448,
455,461,485,490,492,500,516,518,535,537,
549,550,553,555,559,564,567,586,596,611,
624,641,646,666,675,678,697,701
     \endverbatim
     </li>
     <li> A different solution was found by rots with cutoff=2*10^7, caried
     over from n=725:
     \verbatim
1,6,16,28,47,73,75,80,104,109,
110,112,117,126,141,146,163,176,186,191,
197,201,220,221,234,237,238,250,257,269,
274,295,308,312,326,334,339,343,345,349,
361,363,380,398,406,408,413,437,442,443,
445,450,459,474,496,509,516,519,524,530,
534,553,554,565,567,570,571,583,602,607,
628,641,645,659,676,678,696,701
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=727
    <ol>
     <li> rots, cutoff=5*10^6:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
372 336  83   9   3   2   2  13  30  36  45  39  25   4   1
1000
     \endverbatim
     </li>
     <li> gsat-tabu, cutoff=10*10^6:
     \verbatim
  1   2   3   4   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21
 13  25  21   6   2   5  36  92 208 184 124  67  43  49  32  19  25  15   8   4
 22  23  24  25  26  27  28  29  30  33
  8   3   2   2   1   1   1   1   1   2
1000
     \endverbatim
     (so it seems rots is definitely better). </li>
     <li> rots, cutoff=10^7:
     \verbatim
  1   2   3   4   8   9  10  11
284 171  18   2   4  15   3   3
500
  1   2   3   5   7   8   9  10  11  12
286 183  10   3   2   2   5   3   2   4
500
  1   2   3   7   8   9  10  11  12
281 176  19   1   3   6   5   4   5
500
     \endverbatim
     </li>
     <li> rots, cutoff=2*10^7:
     \verbatim
  1   2   3   9
403  93   2   2
500
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>

*/
