// Oliver Kullmann, 5.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/Datak3.hpp
  \brief Investigating the transversal hypergraph of Green-Tao hypergraphs for k = 3 (length of arithmetic progressions)


  \todo Elementary statistics
  <ul>
   <li> Investigating tr_arithprog_hg(k,n). </li>
   <li> Statistics:
   \verbatim
Lp_0_20 : create_list(transversal_hg_rs(arithprog_primes_hg(3,n)),n,0,20)$
for i : 0 thru 20 do (print(i, statistics_fcs(arithprog_primes_hg(3,i)), 
 statistics_fcs(Lp_0_20[i+1]), ncl_list_fcs(Lp_0_20[i+1])));
0 [0,0,0,-1,inf] [0,1,0,0,0] [[0,1]]
1 [1,0,0,-1,inf] [1,1,0,0,0] [[0,1]]
2 [2,0,0,-1,inf] [2,1,0,0,0] [[0,1]]
3 [3,0,0,-1,inf] [3,1,0,0,0] [[0,1]]
4 [4,1,3,3,3] [4,3,3,1,1] [[1,3]]
5 [5,2,6,3,3] [5,3,4,2,1] [[1,2],[2,1]]
6 [6,2,6,3,3] [6,3,4,2,1] [[1,2],[2,1]]
7 [7,3,9,3,3] [7,7,14,2,2] [[2,7]]
8 [8,5,15,3,3] [8,13,38,3,2] [[2,1],[3,12]]
9 [9,7,21,3,3] [9,16,55,4,3] [[3,9],[4,7]]
10 [10,9,27,3,3] [10,21,81,4,3] [[3,3],[4,18]]
11 [11,11,33,3,3] [11,23,102,5,3] [[3,2],[4,9],[5,12]]
12 [12,11,33,3,3] [12,23,102,5,3] [[3,2],[4,9],[5,12]]
13 [13,13,39,3,3] [13,41,208,6,4] [[4,8],[5,22],[6,11]]
14 [14,16,48,3,3] [14,72,409,7,5] [[5,24],[6,47],[7,1]]
15 [15,17,51,3,3] [15,115,702,7,5] [[5,3],[6,97],[7,15]]
16 [16,20,60,3,3] [16,193,1342,8,6] [[6,20],[7,162],[8,11]]
17 [17,23,69,3,3] [17,238,1785,9,6] [[6,3],[7,128],[8,92],[9,15]]
18 [18,24,72,3,3] [18,382,3113,10,7] [[7,51],[8,225],[9,104],[10,2]]
19 [19,26,78,3,3] [19,493,4310,11,7] [[7,18],[8,158],[9,251],[10,65],[11,1]]
20 [20,30,90,3,3] [20,615,5844,12,7] [[7,6],[8,56],[9,234],[10,262],[11,56],[12,1]]
   \endverbatim
   </li>
   <li> The numbers of minimum hyperedges:
   \verbatim
A3p(n):=arithprog_primes_hg(3,n)$
oklib_monitor : true$
Lp60_3 : minimum_transversals_mongen(60,A3p,[{}])$
1 0 1
2 0 1
3 0 1
4 1 3
5 1 2
6 1 2
7 2 7
8 2 1
9 3 9
10 3 3
11 3 2
12 3 2
13 4 8
14 5 24
15 5 3
16 6 20
17 6 3
18 7 51
19 7 18
20 7 6
21 8 18
22 9 84
23 10 356
24 10 29
25 10 8
26 10 4
27 10 2
28 11 19
29 12 19
30 13 32
31 13 2
32 14 3
33 15 160
34 15 44
35 15 11
36 16 23
37 16 2
38 17 20
39 18 40
40 19 189
41 19 55
42 20 687
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_GreenTao-O3-DNDEBUG 70 3
43 21 2465
44 21 807
45 21 34
46 21 4
47 22 6
48 23 32
49 24 245
50 25 1044
51 25 150
52 26 1224
53 26 154
54 26 18
55 27 435
56 28 753
57 29 1294
58 29 726
59 30 2614
60 30 18
61 31 35
62 32 568
63 32 311
64 32 57
65 33 119
66 33 39
67 33 1
  \endverbatim
   </li>
   <li> Information on the number of hyperedges and the number of connected
   components:
   \verbatim
L3 : [];
minimum_transversals_decomp_gen(inf,lambda([n],arithprog_primes_hg(3,n)), 'L3)$

1 1 0 [0,1,1]
4 4 1 [1,3,2]
5 5 2 [1,2,2]
7 7 3 [2,7,3]
8 8 5 [2,1,2]
9 9 7 [3,9,2]
10 10 9 [3,3,2]
11 11 11 [3,2,2]
13 13 13 [4,8,3]
14 14 16 [5,24,2]
15 15 17 [5,3,2]
16 16 20 [6,20,2]
17 17 23 [6,3,2]
18 18 24 [7,51,2]
19 19 26 [7,18,2]
20 20 30 [7,6,2]
21 21 32 [8,18,2]
22 22 36 [9,84,2]
23 23 40 [10,356,2]
24 24 44 [10,29,2]
25 25 46 [10,8,2]
26 26 49 [10,4,2]
27 27 53 [10,2,2]
28 28 56 [11,19,2]
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation (as provided by "GTTransversalsInc 3 1 0 OutputFile"):
   \verbatim
k n tau
3 1 0
3 2 0
3 3 0
3 4 1
3 5 1
3 6 1
3 7 2
3 8 2
3 9 3
3 10 3
3 11 3
3 12 3
3 13 4
3 14 5
3 15 5
3 16 6
3 17 6
3 18 7
3 19 7
3 20 7
3 21 8
3 22 9
3 23 10
3 24 10
3 25 10
3 26 10
3 27 10
3 28 11
3 29 12
3 30 13
3 31 13
3 32 14
3 33 15
3 34 15
3 35 15
3 36 16
3 37 16
3 38 17
3 39 18
3 40 19
3 41 19
3 42 20
3 43 21
3 44 21
3 45 21
3 46 21
3 47 22
3 48 23
3 49 24
3 50 25
3 51 25
3 52 26
3 53 26
3 54 26
3 55 27
3 56 28
3 57 29
3 58 29
3 59 30
3 60 30
3 61 31
3 62 32
3 63 32
3 64 32
3 65 33
3 66 33
3 67 33
3 68 34
3 69 35
3 70 36
3 71 37
3 72 38
3 73 39
3 74 39
3 75 40
3 76 41
3 77 41
3 78 42
3 79 42
3 80 43
3 81 44
   \endverbatim
   </li>
  </ul>


  \todo Local search
  <ul>
   <li> saps seems to find fastest the near-solutions (min=1). </li>
   <li> However it seems we have the same behaviour as with vdW-problems (see
   "Local search" in
   Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak3.hpp),
   namely that finding a solution seems very hard (while min=1 is very easily
   reached). For tau_arithprog_primes(3,81)=44 we have not been successful.
   </li>
   <li> Likely the translation via addition is not appropriate here, and some
   true optimisation approach should be taken. </li>
  </ul>

*/

