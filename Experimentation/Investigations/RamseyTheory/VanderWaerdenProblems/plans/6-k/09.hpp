// Oliver Kullmann, 22.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/6-k/09.hpp
  \brief On investigations into vdw_2(6,9)


  \todo Best complete solver for palindromic problems


  \todo Best local-search solver for palindromic problems


  \todo vdw_2^pd(6,9) >= (857,1382)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=856:
     \verbatim
2,5,6,7,10,12,14,18,23,25,
30,31,32,33,35,38,46,47,49,50,
52,53,56,58,60,61,69,70,73,74,
75,80,82,83,86,90,96,98,99,101,
105,107,108,109,113,116,118,119,124,125,
126,128,129,130,132,139,142,143,144,146,
148,150,156,157,159,164,167,168,170,172,
174,175,178,180,181,183,185,194,199,200,
201,205,207,208,209,211,215,217,222,223,
224,226,232,237,244,248,251,252,253,255,
256,257,261,262,264,267,268,269,271,273,
274,277,281,283,284,288,289,290,292,296,
298,299,302,304,305,306,307,309,310,311,
312,314,316,317,320,321,323,325,331,336,
341,347,349,350,351,353,362,364,365,366,
368,374,379,384,390,392,393,395,397,398,
399,403,404,406,408,409,410,411,413,415,
416,417,423,425,426,427
     \endverbatim
     </li>
     <li> n=1381:
     \verbatim
5,8,9,13,17,18,21,27,34,36,
41,42,43,45,46,49,56,58,60,63,
68,69,72,75,80,84,85,86,88,92,
95,101,106,111,113,115,116,117,119,120,
125,126,127,132,135,137,138,146,150,151,
157,160,164,166,167,168,172,174,175,177,
179,183,184,187,192,194,195,196,200,202,
203,204,206,207,209,210,211,212,213,216,
217,220,221,222,223,231,234,235,236,237,
241,242,247,253,255,256,257,258,260,261,
262,265,266,267,269,271,274,275,276,278,
282,283,284,289,291,294,295,298,299,301,
303,304,306,311,318,321,327,328,332,337,
339,340,341,342,345,352,353,358,359,361,
362,363,364,365,367,372,376,383,390,392,
393,394,398,400,401,403,405,406,410,413,
415,418,420,421,422,426,428,430,432,433,
436,437,438,439,442,443,444,446,447,448,
449,451,457,460,461,462,463,467,468,469,
470,479,481,482,483,484,486,487,488,491,
492,494,495,498,500,501,502,504,508,509,
512,515,517,521,524,525,527,529,530,536,
537,538,540,544,553,554,558,563,565,566,
568,569,571,572,577,579,584,585,587,588,
589,590,591,593,602,603,609,612,616,618,
620,624,626,627,631,632,635,636,639,641,
644,646,647,652,654,655,656,658,659,661,
662,663,664,665,669,670,672,673,674,675,
677,683,686,687,688,689,691
     \endverbatim
    </ol>
   </li>
   <li> "RunPdVdWk1k2 6 9 adaptg2wsat 100 80000000" yields
   \verbatim
Break point 1: 809
Break point 2: 1382
   \endverbatim
   using at most 96 runs. </li>
   <li> "RunPdVdWk1k2 6 9 adaptg2wsat 100 160000000" yields
   \verbatim
Break point 1: 857
Break point 2: 1382
   \endverbatim
   using at most 32 runs (for n=808). </li>
   <li> We have
   \verbatim
> E=read_ubcsat("Exp_PdVanderWaerden_2-6-9_adaptg2wsat-100-160000000_2011-03-22-190025/VanDerWaerden_pd_2-6-9_1383.cnf_OUT",nrows=100)
137 140 141 142 143 144 145 146 147 148 149 150 151 152 153
  1   1   1   2   7  11   4   8  13   8  17  12  11   2   2
100
   \endverbatim
   and thus perhaps the true value of vdw_2^pd(6,9)_2 is much larger, but
   these instances are very hard. After the first break point all solutions
   were found as modifications of the ones obtained before the first break
   point, with a maximum number of steps of 286466. </li>
  </ul>

*/
