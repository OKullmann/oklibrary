// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/29.hpp
  \brief On investigations into vdw_2(3,29)


  \todo vanderwaerden_2(3,29) >= 868
  <ul>
   <li> The (weak) conjecture is vanderwaerden_2(3,29) = 868. </li>
   <li> Search starting with n=620:
   \verbatim
OKplatform> RunVdW3k 29 750 rots 1000 5000000
   \endverbatim
   </li>
   <li> n=808 found satisfiable with certificate
   \verbatim
7,30,50,61,87,103,104,123,126,135,
155,161,172,174,190,198,205,214,227,229,
242,248,271,285,288,301,311,316,338,345,
348,353,359,363,377,394,399,400,414,420,
422,431,456,457,474,488,493,505,507,511,
531,538,542,547,568,575,581,604,618,621,
634,636,644,649,658,671,678,681,686,692,
710,732,733,745,755,764,782,792
   \endverbatim
   </li>
   <li> n=852 found satisfiable with certificate
   \verbatim
18,37,59,64,75,92,98,119,136,144,
145,149,173,181,186,207,222,240,246,256,
259,260,277,281,286,292,304,318,320,323,
330,333,341,358,366,395,397,408,415,431,
434,441,452,477,478,482,499,506,514,519,
540,555,573,579,589,592,593,610,614,619,
625,637,651,653,656,663,666,674,691,699,
725,728,748,767,774,795,801,802,814,832
   \endverbatim
   </li>
   <li> n=854 found satisfiable with certificate
   \verbatim
26,39,61,66,77,94,100,121,131,138,
146,147,151,175,183,188,209,224,242,248,
258,261,262,279,283,288,294,306,320,322,
325,332,335,343,360,368,397,399,410,417,
433,436,443,454,471,479,480,484,501,508,
516,521,542,557,575,581,591,594,595,612,
616,621,627,639,653,655,658,665,668,676,
693,701,727,730,750,769,776,797,803,816,
834
   \endverbatim
   </li>
   <li> n=859 found satisfiable with certificate
   \verbatim
28,35,61,74,81,98,102,111,128,136,
139,165,183,186,188,209,214,222,229,239,
247,257,260,262,276,283,294,296,297,303,
321,324,325,340,361,368,371,373,394,399,
410,414,431,432,435,461,472,498,510,516,
519,521,542,547,555,572,590,593,595,609,
616,627,629,630,636,654,657,658,666,673,
691,704,706,732,740,743,765,784,802,821,
843
   \endverbatim
   </li>
   <li> n=867:
    <ol>
     <li> Found satisfiable with certificate
     \verbatim
16,38,57,75,94,116,119,127,153,155,
168,186,193,201,202,205,223,229,230,232,
243,250,264,266,269,287,304,312,317,338,
340,343,349,361,387,398,424,427,428,445,
449,460,465,486,488,491,498,519,534,535,
538,556,562,563,565,576,583,597,599,602,
612,620,630,637,645,650,671,673,676,694,
720,723,731,757,761,778,785,798,824,831,
859
     \endverbatim
     </li>
     <li> 416 runs with rots and cutoff=2*10^7 didn't find a solution:
     \verbatim
  1   2   3   4   5  11  14
 74 303  35   1   1   1   1
416 
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=868:
    <ol>
     <li> cutoff=5*10^6, rots:
     \verbatim
  1   2   3   4   5   6   7   8  10  11  12  13  14  15  16  17  18  19  20 
 21 336 294  67  13   7   2   2   3   9  18  27  50  46  55  37   9   2   2 
1000 
     \endverbatim
     </li>
     <li> cutoff=10^7, rots:
     \verbatim
  1   2   3   4   5   6   7   9  11  12  13  14  15  16  17  18 
 67 609 210  24   9   2   2   1   3  13  13  21  15   8   2   1 
1000 
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Palindromic numbers
  <ul>
   <li> 820 < pdvanderwaerden([3,29])[2] =(conj) 821 =(conj)
   vanderwaerden([3,29]) - 47. </li>
   <li> pdvanderwaerden([3,29])[1] =(conj) 810. </li>
   <li> Certificates:
    <ol>
     <li> n=809:
     \verbatim
23,44,73,87,91,98,103,120,143,160,
172,185,186,189,197,203,214,226,230,236,
259,261,265,290,302,306,313,323,335,348,
360,363,388,389,393
     \endverbatim
     </li>
     <li> n=820:
     \verbatim
14,16,17,34,50,65,67,72,75,103,
108,109,124,137,154,161,169,180,188,195,
212,225,246,256,261,283,299,307,314,328,
335,336,348,369,391,393,399,406
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 29 gsat-tabu 100 10000000" yields
   \verbatim
Break point 1: 810
Break point 2: 821
   \endverbatim
   where all solutions were found in the first 22 runs.  </li>
   <li> Further testing unsatisfiability for n=822:
   \verbatim
> ubcsat-okl -alg gsat-tabu -runs 200 -cutoff 20000000 -i AltExp/Exp_PdVanderWaerden_2-3-29_gsat-tabu-100-10000000_2010-12-02-002800/VanDerWaerden_pd_2-3-29_822.cnf | tee VanDerWaerden_pd_2-3-29_822.cnf_OUT

> E = read_ubcsat("VanDerWaerden_pd_2-3-29_822.cnf_OUT",nrows=200)
  1   2
161  39
200
   \endverbatim
   </li>
  </ul>

*/
