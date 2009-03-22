// Oliver Kullmann, 18.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak3.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 3 (length of arithmetic progressions)


  \todo Elementary statistics for k=3
  <ul>
   <li> Just using clause-set statistics-functions:
   \verbatim
L_0_20 : create_list(transversal_hg_rs(arithprog_hg(3,n)),n,0,20)$
for i : 0 thru 20 do (print(i, statistics_fcs(arithprog_hg(3,i)), 
 statistics_fcs(L_0_20[i+1]), ncl_list_fcs(L_0_20[i+1])));
   \endverbatim
   we obtain
   \verbatim
0 [0,0,0,-1,inf] [0,1,0,0,0] [[0,1]]
1 [1,0,0,-1,inf] [1,1,0,0,0] [[0,1]]
2 [2,0,0,-1,inf] [2,1,0,0,0] [[0,1]]
3 [3,1,3,3,3] [3,3,3,1,1] [[1,3]]
4 [4,2,6,3,3] [4,3,4,2,1] [[1,2],[2,1]]
5 [5,4,12,3,3] [5,3,5,2,1] [[1,1],[2,2]]
6 [6,6,18,3,3] [6,4,8,2,2] [[2,4]]
7 [7,9,27,3,3] [7,10,30,3,3] [[3,10]]
8 [8,12,36,3,3] [8,25,100,4,4] [[4,25]]
9 [9,16,48,3,3] [9,38,186,5,4] [[4,4],[5,34]]
10 [10,20,60,3,3] [10,48,264,6,5] [[5,24],[6,24]]
11 [11,25,75,3,3] [11,58,362,7,5] [[5,7],[6,30],[7,21]]
12 [12,30,90,3,3] [12,73,498,8,6] [[6,25],[7,36],[8,12]]
13 [13,36,108,3,3] [13,86,642,9,6] [[6,6],[7,41],[8,32],[9,7]]
14 [14,42,126,3,3] [14,109,878,10,6] [[6,1],[7,14],[8,76],[9,14],[10,4]]
15 [15,49,147,3,3] [15,146,1284,11,7] [[7,4],[8,32],[9,102],[10,6],[11,2]]
16 [16,56,168,3,3] [16,210,2018,12,8] [[8,14],[9,60],[10,131],[11,4],[12,1]]
17 [17,64,192,3,3] [17,285,2959,12,9] [[9,43],[10,92],[11,148],[12,2]]
18 [18,72,216,3,3] [18,411,4578,12,10] [[10,97],[11,160],[12,154]]
19 [19,81,243,3,3] [19,614,7294,13,11] [[11,220],[12,248],[13,146]]
20 [20,90,270,3,3] [20,957,12090,14,11] [[11,2],[12,451],[13,400],[14,104]]
   \endverbatim
   </li>
   <li> So the sequence tau_arithprog_hg(3,n) (starting with n=0) begins with
   0,0,0,1,1,1,2,3,4,4,5,5,6,6,6,7,8,9,10,11,11. </li>
   <li> It follows that the sequence vanderwaerden_{k+1}([2]_k, 3) (starting
   with k=0) begins with
   3,6,7,8,10,12,15,16,17,18,19. </li>
   <li> The sequence n - tau_arithprog_hg(3,n) = alpha_arithprog_hg(3,n) =
   r_3(n):
   0,1,2,2,3,4,4,4,4,5,5,6,6,7,8,8,8,8,8,8,9. </li>
   <li> Excluding the first term, this is sequence A003002;
   see [S. S. Wagstaff, Jr., On k-free sequences of integers, Math. Comp., 26
   (1972), 767-771]:
   \verbatim
1, 2, 2, 3, 4, 4, 4, 4, 5, 5, 
6, 6, 7, 8, 8, 8, 8, 8, 8, 9, 
9, 9, 9, 10, 10, 11, 11, 11, 11, 12,
12, 13, 13, 13, 13, 14, 14, 14, 14, 15,
16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
17, 17, 17
   \endverbatim
   (covering the range 1 <= n <= 53, see below for extended data).
   </li>
   <li> So the sequence tau_arithprog_hg(3,n) (starting with n=0) begins with
   \verbatim
0,
0,0,1,1,1,2,3,4,4,5,
5,6,6,6,7,8,9,10,11,11,
12,13,14,14,15,15,16,17,18,18,
19,19,20,21,22,22,23,24,25,25,
25,26,27,28,29,30,31,32,33,34,
34,35,36
   \endverbatim
   </li>
   <li> It follows that the sequence vanderwaerden_{k+1}([2]_k, 3) (starting
   with k=0) begins with
   \verbatim
3,
6,7,8,10,12,15,16,17,18,19,
21,22,23,25,27,28,29,31,33,34,
35,37,38,39,42,43,44,45,46,47,
48,49,50,52,53
   \endverbatim
   (covering the range 0 <= k <= 35. </li>
   <li> Another sequence of interest is the number of hyperedges:
   1,1,1,3,3,3,4,10,25,38,48,58,73,86,109,146,210,285,411,614,957. </li>
   <li> Doesn't seem to be in the eis-database (also after excluding the first
   terms). </li>
   <li> Perhaps also the maximum size of a hyperedge is of interest:
   0,0,0,1,2,2,2,3,4,5,6,7,8,9,10,11,12,12,12,13,14. </li>
   <li> Doesn't seem to be in the eis-database (also after excluding the first
   terms). </li>
   <li> Most natural (and relatively easiest to compute) amongst additional
   data on the transversal hypergraph seems to be the number of minimum
   hyperedges:
   \verbatim
A3(n):=arithprog_hg(3,n)$
L60_3 : minimum_transversals_mongen(60,A3,[{}])$
1 0 1
2 0 1
3 1 3
4 1 2
5 1 1
6 2 4
7 3 10
8 4 25
9 4 4
10 5 24
11 5 7
12 6 25
13 6 6
14 6 1
15 7 4
16 8 14
17 9 43
18 10 97
19 11 220
20 11 2
21 12 18
22 13 62
23 14 232
24 14 2
25 15 33
26 15 2
27 16 12
28 17 36
29 18 106
30 18 1
31 19 11
32 19 2
33 20 4
34 21 14
35 22 40
36 22 2
37 23 4
38 24 86
39 25 307
40 25 20
41 25 1
42 26 4
43 27 14
44 28 41
45 29 99
46 30 266
47 31 674
48 32 1505
49 33 3510
50 34 7726
51 34 14
52 35 50
53 36 156
54 36 2
55 37 8
56 38 26
57 39 56
58 39 2
59 40 4
   \endverbatim
   </li>
   <li> With a C++ implementation this should be extendible to values quite
   a bit beyond the known numbers (see above "Efficient computation of the
   transversal number"). </li>
   <li> Just computing the transversal numbers, using minisat and the
   direct translation (as provided by "VdWTransversalsInc 3 1 0 OutputFile";
   see above):
   \verbatim
60 41
61 42
62 43
63 43
64 44
65 45
66 46
67 47
68 48
69 49
70 50
71 50
72 51
73 52
74 52
75 53
76 54
77 55
78 56
79 57
80 58
81 59
82 59
83 60
84 60
85 61
86 62
87 63
88 64
89 65
90 66
91 67
92 67
93 68
94 69
95 69
96 70
97 71
98 72
99 73
100 73
101 74
   \endverbatim
   </li>
   <li> Actually, the values of tau_arithprog_hg(3,n) are known for n <= 194,
   so there must be some more specialised (and more efficient) computations
   possible.
    <ol>
     <li> This is given by sequence A065825, as explained in "Relations to
     vanderwaerden_k(n_1, ..., n_k)" in
     RamseyTheory/VanderWaerdenProblems/Transversals/plans/general.hpp. </li>
     <li> There only the first 36 terms are mentioned, while at
     http://www.math.uni.wroc.pl/~jwr/non-ave.htm one finds 5 more terms;
     the complete list, containing for n > 41 apparently only upper bounds,
     is in Bestk3.txt. </li>
     <li> One should be careful here, since the various computations
     for n <= 41 seem to be "ad-hoc" computations; but the upper bounds
     should be all correct. </li>
     <li> In Methodsk3.html various methods for establishing bounds are
     given (from http://www.math.uni.wroc.pl/~jwr/non-ave.htm), using
     bounds for the similar problem considering arithmetic progressions of
     size 3 in the cyclic groups of order n (the size of the vertex set). </li>
     <li> The main conjecture at http://www.math.uni.wroc.pl/~jwr/non-ave.htm
     is that for a(n) (that sequence A065825) we have a(n) <= n^{3/2}. </li>
    </ol>
   </li>
  </ul>

*/
