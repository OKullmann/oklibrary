// Oliver Kullmann, 16.2.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/6-k/06.hpp
  \brief On investigations into vanderwaerden_2(6) = 1132

  Instance created by output_vanderwaerden2nd_stdname(6,6,1132) at
  Maxima-level, or by "VanderWaerden-O3-DNDEBUG 6 6 1132" at C++ level
  (instances differ by the order of negated literals in clauses).


  \todo Basic statistics for n=1132
  <ul>
   <li> Density = 225.6 </li>
  </ul>


  \todo Local search solvers
  <ul>
   <li> n = 1000
    <ul>
     <li> ubcsat-irots seems best. </li>
     <li> But finding a solution seems hopeless; less than 1400 falsified
     clauses seems to require billions of steps --- the difference between
     100,000 and 10,000,000 steps is very small. </li>
     <li> We need to look into the Heule-et-al article. </li>
     <li> We need to try it again with the new ubcsat-algorithms. </li>
    </ul>
   </li>
  </ul>


  \todo Upper bounds
  <ul>
   <li> n = 1132
    <ol>
     <li> OKsolver-2002
     \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D30 VanderWaerden_2_6_1132.cnf
     \endverbatim
     looks hopeless (no node of depth 30 solved in one hour). </li>
    </ol>
   </li>
  </ul>


  \todo Determining the best ubcsat-algorithm for palindromic problems
  <ul>
   <li> Cutoff=10^6:
   \verbatim
> E=run_ubcsat("VanDerWaerden_pd_2-6-6_567.cnf",runs=100,cutoff=1000000)
   \endverbatim
   </li>
  </ul>


  \todo vdw_2^pd(6,6) = (567,1132)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=566:
     \verbatim
1,2,3,5,8,9,10,12,14,15,
16,17,19,23,26,27,29,31,32,33,
37,42,45,50,51,52,53,54,57,58,
61,62,63,64,65,70,73,78,82,83,
84,86,88,89,92,96,98,99,100,101,
103,105,106,107,110,112,113,115,116,118,
121,122,123,125,127,128,129,130,132,136,
139,140,142,144,145,146,150,155,158,163,
164,165,166,167,170,171,174,175,176,177,
178,183,186,191,195,196,197,199,201,202,
205,209,211,212,213,214,216,218,219,220,
223,225,226,228,229,231,234,235,236,238,
240,241,242,243,245,249,252,253,255,257,
258,259,263,268,271,276,277,278,279,280,
283
     \endverbatim
     </li>
     <li> n=1131:
     \verbatim
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Established by "CRunPdVdWk1k2 6 6 minisat-2.2.0". </li>
   <li> Evaluation:
   \verbatim
> E=read_crunpdvdw_minisat()
$cfs:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
       0    13620   390600  1006000  1089000 51400000
sd= 2467837
     95%      96%      97%      98%      99%     100%
 3771146  4689686  5342741  7541277  9834239 51398334
sum= 851449200
   \endverbatim
   Not showing times, since apparently the processor switched frequencies
   (csltok). </li>
   <li> The even problems look harder (as usual, before the second phase where
   only one parity is considered). </li>
   <li> Interesting the large palindromic gap, and that the palindromic gap
   is zero here. </li>
   <li> So here using palindromic problems to get solutions for the ordinary
   problems is very successful. </li>
   <li> "RunPdVdWk1k2 6 6 gsat-tabu 100 10000000" yields
   \verbatim
Break point 1: 566
Break point 2: 567
> E=read_ubcsat("VanDerWaerden_pd_2-6-6_567.cnf_OUT",nrows=100)
125 138 139 140 141 142 143 144 145 146 147 148 149
  1   3   1  15   9  10  12  19  17   7   3   2   1
100
> E=read_ubcsat("VanDerWaerden_pd_2-6-6_568.cnf_OUT",nrows=100)
107 112 123 129 130 134 135 137 138 139 140 141 142 143 144 145 146 147 148 149
  2   1   3   1   2   1   1   1   5   2   4   6  16  12  15   5   7   6   5   3
151 155
  1   1
100
   \endverbatim
   n=246 seems to be the first harder problem, all other problems for n<=566
   are easy. n=567 is actually satisfiable, while n=568 is unsatisfiable.
   </li>
  </ul>

*/

