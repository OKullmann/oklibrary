// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/28.hpp
  \brief On investigations into vdw_2(3,28)


  \todo vanderwaerden_2(3,28) >= 827
  <ul>
   <li> The conjecture is vanderwaerden_2(3,28) = 827. </li>
   <li> Search starting with n=620:
   \verbatim
OKplatform> RunVdW3k 28 620 gsat-tabu 1000 10000000
   \endverbatim
   </li>
   <li> n=733 found satisfiable (seed=1020588263, osteps=2293839, run=1),
   with solution
   \verbatim
22,40,49,59,72,75,77,81,88,89,
94,108,112,118,123,133,146,151,168,170,
182,183,200,205,219,234,236,257,271,274,
293,297,303,310,334,336,340,347,355,382,
385,390,392,405,408,414,421,422,427,441,
445,447,451,456,466,479,484,501,503,515,
516,533,538,552,569,590,604,607,626,630,
636,643,667,669,673,680,706,718,723
   \endverbatim
   </li>
   <li> n=742 found satisfiable (seed=1162005655, osteps=1152687, run=3),
   with solution
   \verbatim
12,22,26,41,65,74,75,78,103,111,
112,116,123,137,140,152,170,174,176,179,
185,186,189,190,213,214,216,222,223,244,
251,271,281,285,290,301,324,327,338,345,
355,382,398,408,411,436,444,448,449,456,
470,473,481,503,507,509,512,523,546,547,
549,555,556,559,577,584,596,614,618,623,
634,657,660,667,671,692,715
   \endverbatim
   </li>
   <li> n=750 found satisfiable (seed=2611554683, osteps=5411045, run=5),
   with certificate
   \verbatim
10,36,47,56,59,85,93,110,118,128,
133,137,159,165,170,191,192,205,211,221,
244,258,265,269,270,276,302,303,306,307,
315,322,340,350,359,369,376,380,390,417,
418,426,443,451,461,463,464,466,470,492,
501,503,524,525,544,554,577,591,598,603,
609,635,636,639,640,648,649,673,676,683,
686,692,709,720,722,725
   \endverbatim
   </li>
   <li> Now with the new version of RunVdW3k, which uses the old solution as
   initial assignment, and with rots:
   \verbatim
OKplatform> RunVdW3k 28 750 rots 1000 50000000
   \endverbatim
   </li>
   <li> n=763 found satisfiable with certificate
   \verbatim
4,24,29,41,42,56,61,62,79,99,
106,110,112,115,130,135,138,152,173,180,
184,186,210,217,223,239,249,264,278,297,
301,323,332,337,350,352,374,375,387,389,
395,402,406,412,431,439,443,448,463,468,
471,485,498,506,513,517,519,543,545,550,
556,572,582,597,611,616,634,654,656,665,
667,683,685,690,704,720,735,739
   \endverbatim
   </li>
   <li> n=800 found satisfiable with certificate
   \verbatim
10,34,47,68,72,92,94,99,109,121,
140,166,168,173,197,202,203,205,219,234,
239,256,269,284,290,294,313,321,325,327,
330,331,343,345,350,362,380,388,390,401,
419,427,432,436,438,442,454,473,499,501,
506,530,535,536,538,552,567,589,602,617,
623,627,646,654,658,660,663,664,678,683,
695,700,721,738,752,769,771,794
   \endverbatim
   </li>
   <li> n=826:
    <ol>
     <li> Found satisfiable with certificate
     \verbatim
28,39,62,64,81,95,112,133,138,155,
157,169,170,173,175,179,186,187,206,210,
216,231,244,266,281,295,297,298,303,327,
332,334,360,379,391,395,397,401,406,414,
432,443,445,453,471,483,488,502,503,506,
508,512,520,539,543,549,564,577,594,599,
614,628,630,631,650,660,665,667,693,712,
724,734,739,741,765,776,786,799,816
     \endverbatim
     </li>
     <li> 1000 runs with rots and cutoff=5*10^6 found 2 solutions:
     \verbatim
  0   1   2   3   4   5   6   7   8  10  11  13  14  15  16  17  18  20 
  2  31 530 272  36   7   1   1   2   1   1  16  18  32  32  15   2   1 
1000 
    sat min  osteps  msteps       seed
400   1   0 3527090 3527090  249146123
751   1   0 4364843 4364843 2199943925
     \endverbatim
     Again none of the solution could be reproduced (due to the Ubcsat bug).
     </li>
     <li> With small modifications from a solution for n=825 a solution for
     n=826 was produced, the same as above but with vertex 823 added. </li>
    </ol>
   </li>
   <li> n=827:
    <ol>
     <li> cutoff=5*10^6, rots:
     \verbatim
  1   2   3   4   5   6   7   9  10  12  13  14  15  16  17  18  19 
 29 502 313  44   7   3   2   1   1   5  10  24  26  18   7   7   1 
1000 
     \endverbatim
     </li>
     <li> cutoff=10^7, rots:
     \verbatim
  1   2   3   4  13  14  15 
 58 763 151  14   4   7   3 
1000 
     \endverbatim
     </li>
     <li> cutoff=2*10^7, rots:
     \verbatim
  1   2   3   7
 57 430   9   1
497
     \endverbatim
     </li>
     <li> cutoff=5*10^7, rots:
     \verbatim
 1  2 
 7 14 
21 
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Palindromic numbers
  <ul>
   <li> 742 < vanderwaerden_g([3,28],"pd") =(conj) 743 =(conj)
   vanderwaerden([3,28]) - 84. </li>
   <li> vanderwaerden_g([3,28],"pdsat") =(conj) 728. </li>
   <li> Certificates:
    <ol>
     <li> n=727:
     \verbatim
27,33,47,56,76,90,101,121,134,150,
158,159,172,174,187,188,193,203,206,222,
245,261,264,274,280,293,295,309,324,333,
346,351,362
     \endverbatim
     </li>
     <li> n=742:
     \verbatim
22,45,47,52,78,89,91,115,127,128,
138,146,150,158,159,169,184,212,213,233,
251,252,256,262,275,287,295,305,306,314,
336,349,363,370
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 28 gsat-tabu 100 8000000":
   \verbatim
Break point 1: 728
Break point 2: 743
   \endverbatim
   All solutions found within the first 3 runs. </li>
  </ul>

*/
