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
   <li> n=1132, c=255154, l=1530924, all clauses of length 6. </li>
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
     <li> Finding the best ubcsat-algorithm for new version:
     \verbatim
> E=run_ubcsat("VanDerWaerden_2-6-6_1131.cnf",runs=100,cutoff=1000000)

     \endverbatim
     </li>
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
WARNING[run_ubcsat]: There have been 2 errors with the following algorithms:
 saps, sapsnr .

> plot(E$alg,E$min)
> eval_ubcsat_dataframe(E)
rsaps :
  0   1 139 148 155 157 159 160 161 163 164 165 166 168 169
 45  31   1   1   1   2   3   2   2   5   2   1   2   1   1
paws :
  0   1 150 153 155 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171
  4   7   1   1   1   1   1   1   2   1   3   6   2  16  12  10  17   9   3   1
172
  1
ddfw :
  0   1 111 116 117 118 119 120 121 122 123 124 128 134 141 142 143 144 145 146
  3  12   1   1  12   2   4   5   3   3   3   3   1   1   1   1   3   2   1   2
147 148 149 150 151 152
  8  11   5   7   3   2
ag2wsat :
  0   1 109 111 116 117 118 119 120 121 122 123 124 125 138 142 143 144 145 146
  2   7   3   4   2   6   5   5   4   4   2   5   4   1   1   1   1   4   6   6
147 148 149 150 151 152 153
  6   9   4   2   2   3   1
rots :
  0   1 117 119 120 121 122 123 124 136 137 138 139 140 141 142 143 144 145 146
  1   3   1   1   1   1   1   2   1   1   2   3   4   9   6  13  18  17   5  10
g2wsat :
  0   1 114 119 120 122 123 124 125 126 127 128 129 130 131 132 146 148 151 153
  1   2   1   2   2   4   1   1   2   2   2   2   1   1   1   2   1   1   3   4
154 155 156 157 158 159 160 161
  5  14   9  11  12   8   4   1
rnov :
  0   1 128 130 132 133 134 138 140 141 149 158 171 176 177 178 179 180 181 182
  1   2   1   2   1   1   2   2   1   1   1   1   1   1   3   2   4   2   4   2
183 184 185 186 187 188 189
  8  18   3   9  18   5   4
rnovp :
  0   1 131 133 138 139 140 141 143 154 173 176 177 178 179 180 181 182 183 184
  1   1   1   1   1   2   2   1   1   1   2   3   6   5   6   1   4   8  11   9
185 186 187 188 189 191
 11   4   9   7   1   1
irots :
  1 110 111 113 114 115 118 119 120 121 122 123 124 125 126 128 130 133 145 146
  4   1   3   1   1   1   1   1   7   4   3   5   4   2   1   2   1   1   4   5
dano :
  1 123 124 125 126 131 134 135 137 138 139 141 146 153 162 174 175 177 178 179
  5   1   1   2   1   1   1   3   1   1   2   1   2   1   1   1   1   1   2   7
180 181 182 183 184 185 186 187 188 189 190
  6   7   6   8   8  10   6   6   4   1   2
anovpp :
  1 120 128 132 133 134 137 143 166 168 170 171 172 173 174 175 176 177 178 179
  3   1   1   3   2   2   1   1   1   1   1   2   2   1   3   8   3   7  11   9
anovp :
  1 117 120 121 122 127 129 132 133 134 137 146 151 157 159 168 169 171 172 173
  2   2   1   1   1   1   1   1   3   3   2   1   1   1   1   1   1   3   1   1
174 175 176 177 178 179 180 181 182 183 184 185 186 187
  1   3   2   7   5   8   8  11   9   4   7   4   1   1
   \endverbatim
   rsaps the clear winner (ignoring for now the old problems with segfaulting
   saps, sapsnr). </li>
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
     or
     \verbatim
4,6,7,11,13,18,20,21,22,24,
25,28,30,34,35,36,38,39,40,41,
43,44,46,47,48,49,55,56,59,60,
66,67,68,69,71,72,74,75,76,77,
79,80,81,85,87,90,91,93,94,95,
97,102,104,108,109,111,114,117,119,120,
124,126,131,133,134,135,137,138,141,143,
147,148,149,151,152,153,154,156,157,159,
160,161,162,168,169,172,173,179,180,181,
182,184,185,187,188,189,190,192,193,194,
198,200,203,204,206,207,208,210,215,217,
221,222,224,227,230,232,233,237,239,244,
246,247,248,250,251,254,256,260,261,262,
264,265,266,267,269,270,272,273,274,275,
281,282
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
   <li> "RunPdVdWk1k2 6 6 rsaps 100 10000000" yields --- unfortunately we get
   segmentation faults. We need to see whether there are news from the
   Ubcsat-group:
   \verbatim
Break point 1: 567
Break point 2: 788
# both breakpoints due to segmentation faults (also on the runs without
# initial solutions, and also with ubcsat version beta-10)
   \endverbatim
   </li>
  </ul>

*/

