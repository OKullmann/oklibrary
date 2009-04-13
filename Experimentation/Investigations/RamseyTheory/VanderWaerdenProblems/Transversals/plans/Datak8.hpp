// Oliver Kullmann, 18.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak8.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 8 (length of arithmetic progressions)


  \todo Elementary statistics for k=8
  <ul>
   <li>
   \verbatim
L_0_24_8 : compute_transversals_hg(8,24)$
0 [0,0,0,-1,inf] [0,1,0,0,0] [[0,1]]
1 [1,0,0,-1,inf] [1,1,0,0,0] [[0,1]]
2 [2,0,0,-1,inf] [2,1,0,0,0] [[0,1]]
3 [3,0,0,-1,inf] [3,1,0,0,0] [[0,1]]
4 [4,0,0,-1,inf] [4,1,0,0,0] [[0,1]]
5 [5,0,0,-1,inf] [5,1,0,0,0] [[0,1]]
6 [6,0,0,-1,inf] [6,1,0,0,0] [[0,1]]
7 [7,0,0,-1,inf] [7,1,0,0,0] [[0,1]]
8 [8,1,8,8,8] [8,8,8,1,1] [[1,8]]
9 [9,2,16,8,8] [9,8,9,2,1] [[1,7],[2,1]]
10 [10,3,24,8,8] [10,9,12,2,1] [[1,6],[2,3]]
11 [11,4,32,8,8] [11,11,17,2,1] [[1,5],[2,6]]
12 [12,5,40,8,8] [12,14,24,2,1] [[1,4],[2,10]]
13 [13,6,48,8,8] [13,18,33,2,1] [[1,3],[2,15]]
14 [14,7,56,8,8] [14,23,44,2,1] [[1,2],[2,21]]
15 [15,9,72,8,8] [15,50,120,3,2] [[2,30],[3,20]]
16 [16,11,88,8,8] [16,76,212,3,2] [[2,16],[3,60]]
17 [17,13,104,8,8] [17,98,292,4,2] [[2,12],[3,76],[4,10]]
18 [18,15,120,8,8] [18,121,384,4,2] [[2,9],[3,82],[4,30]]
19 [19,17,136,8,8] [19,152,508,5,2] [[2,6],[3,89],[4,56],[5,1]]
20 [20,19,152,8,8] [20,203,722,5,2] [[2,4],[3,84],[4,113],[5,2]]
21 [21,21,168,8,8] [21,263,976,5,2] [[2,2],[3,81],[4,171],[5,9]]
22 [22,24,192,8,8] [22,474,2002,5,3] [[3,64],[4,240],[5,170]]
23 [23,27,216,8,8] [23,737,3347,6,3] [[3,30],[4,310],[5,365],[6,32]]
24 [24,30,240,8,8] [24,1097,5222,6,3] [[3,8],[4,330],[5,676],[6,83]]
   \endverbatim
   </li>
   <li> So the sequence tau_arithprog_hg(8,n) (starting with n=0) begins with
   0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3. </li>
   <li> It follows that the sequence vanderwaerden_{k+1}([2]_k, 8) (starting
   with k=0) begins with
   8,15,22. </li>
   <li> The sequence n - tau_arithprog_hg(8n) = r_8(n):
   0,1,2,3,4,5,6,7,7,8,9,10,11,12,13,13,14,15,16,17,18,19,19,20,21. </li>
   <li> The sequence length(arithprog_hg(8,n)[2]) begins with
   1,1,1,1,1,1,1,1,8,8,9,11,14,18,23,50,76,98,121,152,203,263,474,737,1097.
   </li>
   <li> And the max-rank sequence begins with
   0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,3,3,4,4,5,5,5,5,6,6. </li>
<li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A8(n):=arithprog_hg(8,n)$
L60_8 : minimum_transversals_mongen(80,A8,[{}])$
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
7 0 1
8 1 8
9 1 7
10 1 6
11 1 5
12 1 4
13 1 3
14 1 2
15 2 30
16 2 16
17 2 12
18 2 9
19 2 6
20 2 4
21 2 2
22 3 64
23 3 30
24 3 8
25 3 6
26 3 4
27 3 3
28 3 2
29 4 48
30 4 17
31 4 8
32 4 5
33 4 4
34 4 3
35 4 2
36 5 36
37 5 15
38 5 6
39 5 5
40 5 4
41 5 3
42 5 2
43 6 32
44 6 13
45 6 6
46 6 5
47 6 4
48 6 3
49 6 2
50 6 1
51 7 4
52 8 13
53 9 112
54 9 2
55 10 126
56 10 1
   \endverbatim
   </li>
   <li> Using "VdWTransversalsInc 8 1 0 OutputFile" we obtain
   \verbatim
k n tau
8 1 0
8 2 0
8 3 0
8 4 0
8 5 0
8 6 0
8 7 0
8 8 1
8 9 1
8 10 1
8 11 1
8 12 1
8 13 1
8 14 1
8 15 2
8 16 2
8 17 2
8 18 2
8 19 2
8 20 2
8 21 2
8 22 3
8 23 3
8 24 3
8 25 3
8 26 3
8 27 3
8 28 3
8 29 4
8 30 4
8 31 4
8 32 4
8 33 4
8 34 4
8 35 4
8 36 5
8 37 5
8 38 5
8 39 5
8 40 5
8 41 5
8 42 5
8 43 6
8 44 6
8 45 6
8 46 6
8 47 6
8 48 6
8 49 6
8 50 6
8 51 7
8 52 8
8 53 9
8 54 9
8 55 10
8 56 10
8 57 11
8 58 11
8 59 11
8 60 12
8 61 12
8 62 12
8 63 12
8 64 13
8 65 13
8 66 13
8 67 13
8 68 13
8 69 13
8 70 14
8 71 14
8 72 14
8 73 15
8 74 15
8 75 15
8 76 15
8 77 15
8 78 15
8 79 16
8 80 16
8 81 17
8 82 17
8 83 17
8 84 17
8 85 17
8 86 18
8 87 18
8 88 18
8 89 18
8 90 18
8 91 18
8 92 18
8 93 19
8 94 19
8 95 19
8 96 19
8 97 19
8 98 19
8 99 19
8 100 20
8 101 21
   \endverbatim
   </li>
  </ul>

*/
