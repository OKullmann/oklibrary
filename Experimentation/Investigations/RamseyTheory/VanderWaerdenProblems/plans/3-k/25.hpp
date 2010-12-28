// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/25.hpp
  \brief On investigations into vdw_2(3,25)


  \todo vanderwaerden_2(3,25) > 655
  <ul>
   <li> The (weak) conjecture is vanderwaerden_2(3,25) = 656. </li>
   <li> Search starting with n=590:
   \verbatim
OKplatform> RunVdW3k 25 590 gsat-tabu 200 50000000
   \endverbatim
   </li>
   <li> Perhaps also here the cutoff-value is too large:
   \verbatim
OKplatform> RunVdW3k 25 609 gsat-tabu 1000 10000000
   \endverbatim
   </li>
   <li> n=609 found satisfiable with certificate
   \verbatim
18,19,31,49,53,55,68,90,93,102,
106,123,129,132,138,143,160,164,175,176,
187,201,206,224,234,243,249,253,254,277,
278,286,287,290,315,317,324,328,335,351,
364,382,386,388,389,401,409,426,435,439,
456,462,465,471,476,493,497,508,509,520,
534,539,557,567,576,586,587
   \endverbatim
   </li>
   <li> n=622 found satisfiable with certificate
   \verbatim
7,12,18,27,37,60,74,82,85,86,
97,101,123,129,132,138,155,159,166,168,
185,196,205,206,208,212,229,230,243,259,
266,270,277,279,304,308,316,317,340,341,
345,351,360,370,393,407,415,418,419,430,
434,456,462,465,471,488,492,499,501,504,
518,539,541,545,562,575,576,599,603,610,
612
   \endverbatim
   </li>
   <li> Restarting the search, now using the solution found for n=622
   as initial assignment:
   \verbatim
OKplatform> cat Exp_VanderWaerden_2-3-25_2010-10-12-161927_609/VanDerWaerden_2-3-25_622.cnf_OUT | extract_solution_ubcsat > Solution_n622
OKplatform> RunVdW3k 25 623 rots 1000 5000000 Solution_n622
   \endverbatim
   </li>
   <li> n=654 found satisfiable with certificate
   \verbatim
1,16,19,39,48,56,76,85,90,92,
98,116,127,149,150,153,155,166,174,187,
192,196,203,211,223,227,241,246,264,266,
270,277,278,285,303,312,320,334,349,352,
357,371,372,381,389,409,418,423,425,433,
449,460,470,482,483,486,488,499,505,520,
525,529,536,544,556,560,574,579,597,599,
603,610,611,636,645
   \endverbatim
   A variation is
   \verbatim
16,19,38,39,48,56,76,85,90,92,
98,116,127,149,150,153,155,166,174,187,
192,196,203,211,223,227,241,246,264,266,
270,277,278,285,303,312,320,334,349,352,
357,371,372,381,389,409,418,423,425,431,
433,449,460,470,482,483,486,488,499,520,
525,529,536,544,556,560,574,579,597,599,
603,610,611,636
   \endverbatim
   </li>
   <li> n=655
    <ol>
     <li> Found satisfiable with certificate
     \verbatim
17,20,40,49,57,77,86,91,93,99,
117,128,150,151,154,156,167,175,188,193,
197,204,212,224,228,242,247,265,267,271,
278,279,286,304,313,321,335,350,353,358,
372,373,382,390,410,419,424,426,434,450,
461,471,483,484,487,489,500,506,521,526,
530,537,545,557,561,575,580,598,600,604,
611,612,637,646
     \endverbatim
     This is essentially different from the above solution. </li>
     <li> 1000 runs with rots and cutoff=5*10^6 don't find a solution:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14
216 309 149 112  17  21  32  43  40  34  18   6   1   2
1000
     \endverbatim
     </li>
     <li> Another 1000 runs with rots and cutoff=5*10^6 don't find a solution:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12
232 292 163 112  31  14  48  39  36  19   6   8
1000
     \endverbatim
     </li>
     <li> 500 runs with rots and cutoff=10^7 don't find a solution:
     \verbatim
  1   2   3   4   5   6   7   8  10  11 
191 183  62  35   5   5   6  10   2   1 
500 
     \endverbatim
     </li>
     <li> So n=655 seems to be quite difficult, and so unsatisfiability for
     n=656 is hard to judge. </li>
     <li> Rots with cutoff=20*10^7 found in run 3 a solution for n=654
     (seed=3127361270, osteps=7949973), and this solution worked also for
     n=655:
     \verbatim
2,17,20,40,49,57,77,86,91,93,
99,117,128,150,151,154,156,167,175,188,
193,197,204,212,224,228,242,247,265,267,
271,278,279,286,304,313,321,335,350,353,
358,372,373,382,390,410,419,424,426,434,
450,461,471,483,484,487,489,500,506,521,
526,530,537,545,557,561,575,580,598,600,
604,611,612,637,646
     \endverbatim
     </li>
     <li> A variation was found with rots, cutoff=10^7, as the solution for
     n=654:
     \verbatim
2,17,20,25,39,40,49,57,77,86,
91,93,99,117,128,150,151,154,156,167,
175,188,193,197,204,212,224,228,242,247,
265,267,271,278,279,286,304,313,321,335,
350,353,358,372,373,382,390,410,419,424,
426,434,450,461,471,483,484,487,489,500,
506,521,526,530,537,545,557,561,575,580,
598,600,604,611,612,637
     \endverbatim
     </li>
     <li> Another solution was found with rots, cutoff=2*10^7, again as the
     solution for n=654:
     \verbatim
2,17,20,40,49,57,77,86,91,93,
99,117,128,150,151,154,156,167,175,188,
193,197,204,212,224,228,242,247,265,267,
271,278,279,286,304,313,321,335,350,353,
358,372,373,382,390,410,419,424,426,434,
450,461,471,483,484,487,489,500,506,521,
526,530,537,545,557,561,575,580,598,600,
604,611,612,637,646
     \endverbatim
     This has a larger distance to the above solution (analysis is needed!).
     </li>
     <li> Another variation is
     \verbatim
17,40,49,57,77,86,91,93,99,117,
128,150,151,154,156,167,175,188,193,197,
204,212,224,228,242,247,265,267,271,278,
279,286,304,313,321,335,350,353,358,372,
373,382,390,410,419,424,426,434,450,461,
471,483,484,487,489,500,521,526,530,537,
545,557,561,575,580,598,600,604,611,612,
637
     \endverbatim
     </li>
     <li> Are there essentially different solutions? </li>
    </ol>
   <li>
   <li> n=656
    <ol>
     <li> Cutoff=5*10^6, rots:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12
224 321 162  99  26  24  34  37  30  20  13  10
1000
     \endverbatim
     </li>
     <li> Cutoff=10^7 found in 500 runs no solution. Another 500 runs:
     \verbatim
  1   2   3   4   5   6   7   8   9
187 194  56  39   4   6   7   3   4
500
     \endverbatim
     </li>
     <li> Cutoff=2*10^7, rots:
     \verbatim
  1   2   3   4 
333 146  20   1 
500 
  1   2   3   4
329 147  22   2
500
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Palindromic numbers
  <ul>
   <li> 606 < vanderwaerden_g([3,25],"pd") =(conj) 607 =(conj)
   vanderwaerden([3,25])-49. </li>
   <li> vanderwaerden_g([3,25],"pdsat") =(conj) 586. </li>
   <li> Certificates:
    <ol>
     <li> n=586:
     \verbatim
16,20,21,45,66,67,83,96,103,104,
107,128,129,132,134,159,166,167,170,182,
196,207,217,241,245,246,253,259,284
     \endverbatim
     </li>
     <li> n=606:
     \verbatim
25,30,41,59,68,73,97,99,112,118,
131,141,142,146,155,162,184,191,200,204,
205,215,228,234,247,249,273,278,286,287,
291,302
     \endverbatim
     </li>
    </ol>
   </li>
   <li> precosat570:
    <ol>
     <li> n=607: ?sat, ?s. </li>
     <li> n=587: ?sat, ?s. </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 25 26 618 gsat-tabu 100 8000000" (old version): all
   1<=n<=586 sat, then
   \verbatim
587 0
588 1
589 0
590 1
591 0
592 1
593 0
594 1
595 0
596 1
597 0
598 1
599 0
600 1
601 0
602 1
603 0
604 1
605 0
606 1
   \endverbatim
   and all n>=607 "unsat". All solutions found in the first run.
   </li>
  </ul>

*/
