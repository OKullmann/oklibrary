// Oliver Kullmann, 18.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak5.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 5 (length of arithmetic progressions)


  \todo Elementary statistics for k=5
  <ul>
   <li> The sequence r_5(n) (starting with n=1) is A003004:
   \verbatim
1, 2, 3, 4, 4, 5, 6, 7, 8, 8,
9, 10, 11, 12, 12, 13, 14, 15, 16, 16,
16, 16, 16, 17, 18, 18, 19, 20, 21, 21,
22, 22, 23, 24, 24, 25, 26, 27, 28, 28,
29, 30, 31, 32, 32, 32, 32, 32, 33, 33
   \endverbatim
   (covering the range 1 <= n <= 50). </li>
   <li> Some functions to organise the experiment:
   \verbatim
statistics_hg(G) := statistics_fcs(G)$
compute_transversals_hg(k,N) :=
 block([L : []], 
  for n : 0 thru N do block(
   [A : arithprog_hg(k,n), G],
    G : transversal_hg_rs(A), 
    L : endcons(G,L), 
    print(n,statistics_hg(A),statistics_hg(G),ncl_list_fcs(G))),
  return(L))$
   \endverbatim 
   </li>
   <li>
   \verbatim
L_0_20_5 : compute_transversals_hg(5,20);
0 [0,0,0,-1,inf] [0,1,0,0,0] [[0,1]]
1 [1,0,0,-1,inf] [1,1,0,0,0] [[0,1]]
2 [2,0,0,-1,inf] [2,1,0,0,0] [[0,1]]
3 [3,0,0,-1,inf] [3,1,0,0,0] [[0,1]]
4 [4,0,0,-1,inf] [4,1,0,0,0] [[0,1]]
5 [5,1,5,5,5] [5,5,5,1,1] [[1,5]]
6 [6,2,10,5,5] [6,5,6,2,1] [[1,4],[2,1]]
7 [7,3,15,5,5] [7,6,9,2,1] [[1,3],[2,3]]
8 [8,4,20,5,5] [8,8,14,2,1] [[1,2],[2,6]]
9 [9,6,30,5,5] [9,10,21,3,1] [[1,1],[2,7],[3,2]]
10 [10,8,40,5,5] [10,13,30,3,2] [[2,9],[3,4]]
11 [11,10,50,5,5] [11,20,55,4,2] [[2,6],[3,13],[4,1]]
12 [12,12,60,5,5] [12,28,88,4,2] [[2,4],[3,16],[4,8]]
13 [13,15,75,5,5] [13,57,217,5,2] [[2,2],[3,10],[4,42],[5,3]]
14 [14,18,90,5,5] [14,85,358,5,2] [[2,1],[3,6],[4,52],[5,26]]
15 [15,21,105,5,5] [15,124,563,6,3] [[3,5],[4,49],[5,68],[6,2]]
16 [16,24,120,5,5] [16,180,886,6,3] [[3,4],[4,27],[5,128],[6,21]]
17 [17,28,140,5,5] [17,306,1697,7,3] [[3,3],[4,12],[5,110],[6,177],[7,4]]
18 [18,32,160,5,5] [18,483,2920,8,3] [[3,2],[4,4],[5,84],[6,274],[7,118],[8,1]]
19 [19,36,180,5,5] [19,761,4934,8,3] [[3,1],[4,4],[5,33],[6,359],[7,316],[8,48]]
20 [20,40,200,5,5] [20,1199,8346,9,4] [[4,5],[5,24],[6,208],[7,740],[8,220],[9,2]]
   \endverbatim
   </li>
   <li> So the sequence tau_arithprog_hg(5,n) (starting with n=0) begins with
   0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4. </li>
   <li> It follows that the sequence vanderwaerden_{k+1}([2]_k, 5) (starting
   with k=0) begins with
   5,10,15,20. </li>
   <li> The sequence n - tau_arithprog_hg(5,n) = r_5(n):
   0,1,2,3,4,4,5,6,7,8,8,9,10,11,12,12,13,14,15,16,16. </li>
   <li> The sequence length(arithprog_hg(5,n)[2]) begins with
   1,1,1,1,1,5,5,6,8,10,13,20,28,57,85,124,180,306,483,761,1199. </li>
   <li> And the max-rank sequence begins with
   0,0,0,0,0,1,2,2,2,3,3,4,4,5,5,6,6,7,8,8,9. </li>
   <li> Computing the number of minimum transversals:
   \verbatim
A5(n):=arithprog_hg(5,n)$
L40_5 : minimum_transversals_mongen(40,A5,[{}])$
1 0 1
2 0 1
3 0 1
4 0 1
5 1 5
6 1 4
7 1 3
8 1 2
9 1 1
10 2 9
11 2 6
12 2 4
13 2 2
14 2 1
15 3 5
16 3 4
17 3 3
18 3 2
19 3 1
20 4 5
21 5 24
22 6 112
23 7 619
24 7 40
25 7 2
26 8 270
27 8 70
28 8 12
29 8 2
30 9 26
31 9 5
32 10 745
33 10 266
34 10 81
35 11 534
36 11 236
37 11 115
38 11 48
39 11 16
40 12 80
41 12 32
42 12 12
43 12 4
44 12 1
45 13 5
   \endverbatim
   </li>
  </ul>

*/
