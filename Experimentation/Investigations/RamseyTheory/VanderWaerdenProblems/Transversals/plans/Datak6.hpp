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
G6 : lambda([n], arithprog_hg(6,n))$
oklib_monitor:true;
L_6_20 : transversals_mongen_rs(20,G6)$
for i : 0 thru 20 do print(i, statistics_fcs(G6(i)), rest(extended_statistics_fcs(cs2fcs(L_6_20[i+1])),-2));
5 [5,0,0,-1,inf] [0,1,0,0,0,[[0,1]],inf,minf]
6 [6,1,6,6,6] [6,6,6,1,1,[[1,6]],1,1]
7 [7,2,12,6,6] [7,6,7,1,2,[[1,5],[2,1]],1,1]
8 [8,3,18,6,6] [8,7,10,1,2,[[1,4],[2,3]],1,2]
9 [9,4,24,6,6] [9,9,15,1,2,[[1,3],[2,6]],1,3]
10 [10,5,30,6,6] [10,12,22,1,2,[[1,2],[2,10]],1,4]
11 [11,7,42,6,6] [11,26,60,2,3,[[2,18],[3,8]],4,7]
12 [12,9,54,6,6] [12,37,102,2,3,[[2,9],[3,28]],7,9]
13 [13,11,66,6,6] [13,49,147,2,4,[[2,6],[3,37],[4,6]],9,21]
14 [14,13,78,6,6] [14,62,202,2,4,[[2,4],[3,38],[4,20]],12,25]
15 [15,15,90,6,6] [15,80,277,2,5,[[2,2],[3,40],[4,37],[5,1]],12,31]
16 [16,18,108,6,6] [16,177,720,3,5,[[3,26],[4,113],[5,38]],38,64]
17 [17,21,126,6,6] [17,346,1601,3,6,[[3,9],[4,120],[5,208],[6,9]],72,127]
18 [18,24,144,6,6] [18,543,2746,3,6,[[3,4],[4,74],[5,352],[6,113]],132,191]
19 [19,27,162,6,6] [19,697,3733,3,7,[[3,3],[4,44],[5,389],[6,224],[7,37]],164,256]
20 [20,30,180,6,6] [20,933,5314,3,8,[[3,2],[4,30],[5,354],[6,421],[7,116],[8,10]],200,366]
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
   \endverbatim
   </li>
   <li> Extended:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 100 6

   \endverbatim
   </li>
   <li> Using "VdWTransversalsInc 6 1 0 OutputFile" we obtain (on csltok)
   \verbatim
k n tau
6 1 0
6 2 0
6 3 0
6 4 0
6 5 0
6 6 1
6 7 1
6 8 1
6 9 1
6 10 1
6 11 2
6 12 2
6 13 2
6 14 2
6 15 2
6 16 3
6 17 3
6 18 3
6 19 3
6 20 3
6 21 4
6 22 4
6 23 4
6 24 4
6 25 4
6 26 4
6 27 5
6 28 6
6 29 6
6 30 7
6 31 8
6 32 8
6 33 8
6 34 9
6 35 9
6 36 9
6 37 9
6 38 10
6 39 10
6 40 10
6 41 10
6 42 11
6 43 12
6 44 12
6 45 12
6 46 12
6 47 13
6 48 13
6 49 13
6 50 13
6 51 13
6 52 14
6 53 15
6 54 15
6 55 16
6 56 16
6 57 17
6 58 17
6 59 17
6 60 18
6 61 18
6 62 18
6 63 19
6 64 19
6 65 19
6 66 19
6 67 20
6 68 20
6 69 21
6 70 21
6 71 21
6 72 22
6 73 22
6 74 22
6 75 22
6 76 22
6 77 23
6 78 24
6 79 25
6 80 25
6 81 26
6 82 26
6 83 26
6 84 27
6 85 27
6 86 27
   \endverbatim
   </li>
  </ul>

*/
