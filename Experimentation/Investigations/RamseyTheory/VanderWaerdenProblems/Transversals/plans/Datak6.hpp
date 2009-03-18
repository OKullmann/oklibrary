// Oliver Kullmann, 18.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak6.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 6 (length of arithmetic progressions)


  \todo Elementary statistics for k=6
  <ul>
   <li> The sequence r_6(n) (starting with n=1) is A003005:
   \verbatim
1, 2, 3, 4, 5, 5, 6, 7, 8, 9,
9, 10, 11, 12, 13, 13, 14, 15, 16, 17,
17, 18, 19, 20, 21, 22, 22, 22, 23, 23,
23, 24, 25, 25, 26, 27, 28, 28, 29, 30,
31, 31, 31, 32, 33, 34, 34, 35, 36, 37
   \endverbatim
   (covering the range 1 <= n <= 50). </li>
   <li>
   \verbatim
L_0_20_6 : compute_transversals_hg(6,20)$
0 [0,0,0,-1,inf] [0,1,0,0,0] [[0,1]]
1 [1,0,0,-1,inf] [1,1,0,0,0] [[0,1]]
2 [2,0,0,-1,inf] [2,1,0,0,0] [[0,1]]
3 [3,0,0,-1,inf] [3,1,0,0,0] [[0,1]]
4 [4,0,0,-1,inf] [4,1,0,0,0] [[0,1]]
5 [5,0,0,-1,inf] [5,1,0,0,0] [[0,1]]
6 [6,1,6,6,6] [6,6,6,1,1] [[1,6]]
7 [7,2,12,6,6] [7,6,7,2,1] [[1,5],[2,1]]
8 [8,3,18,6,6] [8,7,10,2,1] [[1,4],[2,3]]
9 [9,4,24,6,6] [9,9,15,2,1] [[1,3],[2,6]]
10 [10,5,30,6,6] [10,12,22,2,1] [[1,2],[2,10]]
11 [11,7,42,6,6] [11,26,60,3,2] [[2,18],[3,8]]
12 [12,9,54,6,6] [12,37,102,3,2] [[2,9],[3,28]]
13 [13,11,66,6,6] [13,49,147,4,2] [[2,6],[3,37],[4,6]]
14 [14,13,78,6,6] [14,62,202,4,2] [[2,4],[3,38],[4,20]]
15 [15,15,90,6,6] [15,80,277,5,2] [[2,2],[3,40],[4,37],[5,1]]
16 [16,18,108,6,6] [16,177,720,5,3] [[3,26],[4,113],[5,38]]
17 [17,21,126,6,6] [17,346,1601,6,3] [[3,9],[4,120],[5,208],[6,9]]
18 [18,24,144,6,6] [18,543,2746,6,3] [[3,4],[4,74],[5,352],[6,113]]
19 [19,27,162,6,6] [19,697,3733,7,3] [[3,3],[4,44],[5,389],[6,224],[7,37]]
20 [20,30,180,6,6] [20,933,5314,8,3] [[3,2],[4,30],[5,354],[6,421],[7,116],[8,10]]
   \endverbatim
   </li>
   <li> So the sequence tau_arithprog_hg(6,n) (starting with n=0) begins with
   0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3. </li>
   <li> It follows that the sequence vanderwaerden_{k+1}([2]_k, 6) (starting
   with k=0) begins with
   6,11,16. </li>
   <li> The sequence n - tau_arithprog_hg(6n) = r_6(n):
   0,1,2,3,4,5,5,6,7,8,9,9,10,11,12,13,13,14,15,16,17. </li>
   <li> The sequence length(arithprog_hg(6,n)[2]) begins with
   1,1,1,1,1,1,6,6,7,9,12,26,37,49,62,80,177,346,543,697,933. </li>
   <li> And the max-rank sequence begins with
   0,0,0,0,0,0,1,2,2,2,2,3,3,4,4,5,5,6,6,7,8. </li>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A6(n):=arithprog_hg(6,n)$
L60_6 : minimum_transversals_mongen(60,A6,[{}])$
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 1 6
7 1 5
8 1 4
9 1 3
10 1 2
11 2 18
12 2 9
13 2 6
14 2 4
15 2 2
16 3 26
17 3 9
18 3 4
19 3 3
20 3 2
21 4 29
22 4 12
23 4 5
24 4 3
25 4 2
26 4 1
27 5 4
28 6 27
29 6 2
30 7 34
31 8 889
32 8 140
33 8 19
34 9 1214
35 9 325
36 9 48
37 9 8
38 10 325
39 10 38
40 10 6
41 10 2
42 11 28
43 12 2810
44 12 725
45 12 262
46 12 53
47 13 1221
48 13 210
49 13 64
50 13 32
51 13 16
52 14
   \endverbatim
   </li>
  </ul>

*/
