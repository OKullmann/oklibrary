// Oliver Kullmann, 18.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak4.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 4 (length of arithmetic progressions)


  \todo Elementary statistics for k=4
  <ul>
   <li> The sequence r_4(n) (starting with n=1) is A003003:
   \verbatim
1, 2, 3, 3, 4, 5, 5, 6, 7, 8,
8, 8, 9, 9, 10, 10, 11, 11, 12, 12,
13, 13, 14, 14, 15, 15, 16, 17, 17, 18,
18, 18, 19, 20, 20, 20, 21, 21, 21, 22,
22, 22, 23, 23, 24, 24, 24, 25, 25, 26,
26
   \endverbatim
   (covering the range 1 <= n <= 51). </li>
   <li> Just using clause-set statistics-functions:
   \verbatim
L_0_20_4 : create_list(transversal_hg_rs(arithprog_hg(4,n)),n,0,20)$
for i : 0 thru 20 do (print(i, statistics_fcs(arithprog_hg(4,i)), statistics_fcs(L_0_20_4[i+1]), ncl_list_fcs(L_0_20_4[i+1])));
0 [0,0,0,-1,inf] [0,1,0,0,0] [[0,1]]
1 [1,0,0,-1,inf] [1,1,0,0,0] [[0,1]]
2 [2,0,0,-1,inf] [2,1,0,0,0] [[0,1]]
3 [3,0,0,-1,inf] [3,1,0,0,0] [[0,1]]
4 [4,1,4,4,4] [4,4,4,1,1] [[1,4]]
5 [5,2,8,4,4] [5,4,5,2,1] [[1,3],[2,1]]
6 [6,3,12,4,4] [6,5,8,2,1] [[1,2],[2,3]]
7 [7,5,20,4,4] [7,11,24,3,2] [[2,9],[3,2]]
8 [8,7,28,4,4] [8,14,38,3,2] [[2,4],[3,10]]
9 [9,9,36,4,4] [9,19,58,4,2] [[2,2],[3,14],[4,3]]
10 [10,12,48,4,4] [10,26,94,4,2] [[2,1],[3,8],[4,17]]
11 [11,15,60,4,4] [11,40,164,5,3] [[3,6],[4,24],[5,10]]
12 [12,18,72,4,4] [12,58,258,5,4] [[4,32],[5,26]]
13 [13,22,88,4,4] [13,130,672,6,4] [[4,3],[5,102],[6,25]]
14 [14,26,104,4,4] [14,277,1646,7,5] [[5,30],[6,233],[7,14]]
15 [15,30,120,4,4] [15,404,2621,8,5] [[5,2],[6,209],[7,187],[8,6]]
16 [16,35,140,4,4] [16,661,4742,9,6] [[6,45],[7,458],[8,156],[9,2]]
17 [17,40,160,4,4] [17,961,7478,9,6] [[6,10],[7,260],[8,621],[9,70]]
18 [18,45,180,4,4] [18,1324,11028,10,7] [[7,100],[8,714],[9,484],[10,26]]
19 [19,51,204,4,4] [19,1920,17189,11,7] [[7,24],[8,380],[9,1189],[10,317],[11,10]]
20 [20,57,228,4,4] [20,2904,27602,12,8] [[8,104],[9,1388],[10,1257],[11,152],[12,3]]
   \endverbatim
   </li>
   <li> So the sequence tau_arithprog_hg(4,n) (starting with n=0) begins with
   0,0,0,0,1,1,1,2,2,2,2,3,4,4,5,5,6,6,7,7,8. </li>
   <li> It follows that the sequence vanderwaerden_{k+1}([2]_k, 4) (starting
   with k=0) begins with
   4,7,11,12,14,16,18,20. </li>
   <li> The sequence n - tau_arithprog_hg(4,n) = r_4(n):
   0,1,2,3,3,4,5,5,6,7,8,8,8,9,9,10,10,11,11,12,12. </li>
   <li> Another sequence of interest is the number of hyperedges:
   1,1,1,1,4,4,5,11,14,19,26,40,58,130,277,404,661,961,1324,1920,2904. </li>
   <li> Doesn't seem to be in the eis-database (also after excluding the first
   terms). </li>
   <li> Perhaps also the maximum size of a hyperedge is of interest:
   0,0,0,0,1,2,2,3,3,4,4,5,5,6,7,8,9,9,10,11,12. </li>
   <li> Computing the number of minimum transversals:
   \verbatim
A4(n):=arithprog_hg(4,n)$
L60_4 : minimum_transversals_mongen(60,A4,[{}])$
1 0 1
2 0 1
3 0 1
4 1 4
5 1 3
6 1 2
7 2 9
8 2 4
9 2 2
10 2 1
11 3 6
12 4 32
13 4 3
14 5 30
15 5 2
16 6 45
17 6 10
18 7 100
19 7 24
20 8 104
21 8 10
22 9 210
23 9 12
24 10 115
25 10 2
26 11 80
27 11 8
28 11 4
29 12 12
30 12 2
31 13 17
32 14 121
33 14 6
34 14 2
35 15 10
36 16 62
37 16 6
38 17 64
39 18 359
40 18 14
   \endverbatim
   </li>
   <li> Using "VdWTransversalsInc 4 1 0 OutputFile" we obtain
   \verbatim
k n tau
1 0
2 0
3 0
4 1
5 1
6 1
7 2
8 2
9 2
10 2
11 3
12 4
13 4
14 5
15 5
16 6
17 6
18 7
19 7
20 8
21 8
22 9
23 9
24 10
25 10
26 11
27 11
28 11
29 12
30 12
31 13
32 14
33 14
34 14
35 15
36 16
37 16
38 17
39 18
40 18
41 19
42 20
43 20
44 21
45 21
46 22
47 23
48 23
49 24
50 24
51 25
52 26
53 26
54 26
55 27
56 28
57 29
58 29
59 30
60 30
61 31
62 32
63 33
64 33
65 34
66 34
67 35
68 35
69 36
70 36
71 37
72 38
73 39
74 39
75 40
76 41
77 41
   \endverbatim
   </li>
  </ul>

*/
