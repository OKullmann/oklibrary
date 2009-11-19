// Oliver Kullmann, 5.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/Datak4.hpp
  \brief Investigating the transversal hypergraph of Green-Tao hypergraphs for k = 4 (length of arithmetic progressions)


  \todo Elementary statistics
  <ul>
   <li> Investigating tr_arithprog_hg(4,n). </li>
   <li> Staticstics:
   \verbatim
block([G0 : [{},{}]],
 for n: 1 thru 30 do block([G : arithprog_primes_hg(4,n), T],
  if G[2]=G0[2] then print(n)
  else {
    T : transversal_hg_rs(G), G0 : G,
    print(n, statistics_fcs(G), statistics_fcs(T), ncl_list_fcs(T))
  }))$
1
2
3
4
5
6
7
8
9 [9,1,4,4,4] [9,4,4,1,1] [[1,4]]
10 [10,2,8,4,4] [10,4,5,2,1] [[1,3],[2,1]]
11
12
13 [13,3,12,4,4] [13,8,15,2,1] [[1,1],[2,7]]
14 [14,4,16,4,4] [14,32,92,3,2] [[2,4],[3,28]]
15
16 [16,5,20,4,4] [16,32,100,4,2] [[2,4],[3,20],[4,8]]
17 [17,7,28,4,4] [17,72,272,4,3] [[3,16],[4,56]]
18
19
20
21
22 [22,8,32,4,4] [22,288,1376,5,4] [[4,64],[5,224]]
23 [23,9,36,4,4] [23,592,3232,6,4] [[4,16],[5,288],[6,288]]
24
25 [25,11,44,4,4] [25,1036,6581,7,4] [[4,3],[5,79],[6,504],[7,450]]
26 [26,12,48,4,4] [26,1512,10226,8,5] [[5,48],[6,487],[7,752],[8,225]]
27 [27,13,52,4,4] [27,3078,23145,9,5] [[5,16],[6,317],[7,1139],[8,1264],[9,342]]
   \endverbatim
   </li>
   <li> The numbers of minimum hyperedges:
   \verbatim
A4p(n):=arithprog_primes_hg(4,n)$
oklib_monitor : true$
Lp60_4 : minimum_transversals_mongen(60,A4p,[{}])$
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
7 0 1
8 0 1
9 1 4
10 1 3
11 1 3
12 1 3
13 1 1
14 2 4
15 2 4
16 2 4
17 3 16
18 3 16
19 3 16
20 3 16
21 3 16
22 4 64
23 4 16
24 4 16
25 4 3
26 5 48
27 5 16
28 5 4
29 5 4
30 5 1
31 5 1
32 6 6
33 6 6
34 6 6
35 7 116
36 8 464
37 9 3248
38 9 1044
39 9 171
40 9 153
41 9 153
42 9 85
43 9 15
44 9 12
45 10 132
46 11 1254
47 11 1254
48 11 33
49 11 12
50 11 12
51 12 87
52 12 62
53 12 62
54 12 52
55 12 1
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_GreenTao-O3-DNDEBUG 100 4
56 13 35
57 13 6
58 14 78
59 14 78
60 14 13
61 15 60
62 16 2280
63 16 912
64 16 840
65 16 315
66 16 252
67 16 224
68 16 224
69 16 152
70 16 38
71 17 58
72 18 416
73 19 416
74 19 416
75 19 416
76 19 104
77 19 2
78 20 55
   \endverbatim
   So we are not far off what minisat2 can compute (see below). </li>
   <li> For orientation, those n-values where no hyperedge has been added
   (compared to n-1):
   \verbatim
for n: 4 thru 200 do block(
 [d:nhyp_arithprog_primes_ohg(4,n)-nhyp_arithprog_primes_ohg(4,n-1)],
  if d=0 then print(n));
4
5
6
7
8
11
12
15
18
19
20
21
24
29
33
41
47
59
68
75
95
102
   \endverbatim
   So not all phases of stagnation are due to no hyperedge being added. </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation (as provided by "GTTransversalsInc 4 1 0 OutputFile"):
   \verbatim
k n tau
4 1 0
4 2 0
4 3 0
4 4 0
4 5 0
4 6 0
4 7 0
4 8 0
4 9 1
4 10 1
4 11 1
4 12 1
4 13 1
4 14 2
4 15 2
4 16 2
4 17 3
4 18 3
4 19 3
4 20 3
4 21 3
4 22 4
4 23 4
4 24 4
4 25 4
4 26 5
4 27 5
4 28 5
4 29 5
4 30 5
4 31 5
4 32 6
4 33 6
4 34 6
4 35 7
4 36 8
4 37 9
4 38 9
4 39 9
4 40 9
4 41 9
4 42 9
4 43 9
4 44 9
4 45 10
4 46 11
4 47 11
4 48 11
4 49 11
4 50 11
4 51 12
4 52 12
4 53 12
4 54 12
4 55 12
4 56 13
4 57 13
4 58 14
4 59 14
4 60 14
4 61 15
4 62 16
4 63 16
4 64 16
4 65 16
4 66 16
4 67 16
4 68 16
4 69 16
4 70 16
4 71 17
4 72 18
4 73 19
4 74 19
4 75 19
4 76 19
4 77 19
4 78 20
4 79 21
4 80 21
4 81 21
4 82 21
4 83 22
   \endverbatim
   </li>
  </ul>

*/

