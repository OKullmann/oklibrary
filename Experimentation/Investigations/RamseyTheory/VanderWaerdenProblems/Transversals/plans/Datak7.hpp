// Oliver Kullmann, 18.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak7.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 7 (length of arithmetic progressions)


  \todo Elementary statistics for k=7
  <ul>
   <li>
   \verbatim
L_0_22_7 : compute_transversals_hg(7,22)$
0 [0,0,0,-1,inf] [0,1,0,0,0] [[0,1]]
1 [1,0,0,-1,inf] [1,1,0,0,0] [[0,1]]
2 [2,0,0,-1,inf] [2,1,0,0,0] [[0,1]]
3 [3,0,0,-1,inf] [3,1,0,0,0] [[0,1]]
4 [4,0,0,-1,inf] [4,1,0,0,0] [[0,1]]
5 [5,0,0,-1,inf] [5,1,0,0,0] [[0,1]]
6 [6,0,0,-1,inf] [6,1,0,0,0] [[0,1]]
7 [7,1,7,7,7] [7,7,7,1,1] [[1,7]]
8 [8,2,14,7,7] [8,7,8,2,1] [[1,6],[2,1]]
9 [9,3,21,7,7] [9,8,11,2,1] [[1,5],[2,3]]
10 [10,4,28,7,7] [10,10,16,2,1] [[1,4],[2,6]]
11 [11,5,35,7,7] [11,13,23,2,1] [[1,3],[2,10]]
12 [12,6,42,7,7] [12,17,32,2,1] [[1,2],[2,15]]
13 [13,8,56,7,7] [13,24,55,3,1] [[1,1],[2,15],[3,8]]
14 [14,10,70,7,7] [14,32,80,3,2] [[2,16],[3,16]]
15 [15,12,84,7,7] [15,47,132,4,2] [[2,12],[3,32],[4,3]]
16 [16,14,98,7,7] [16,63,194,4,2] [[2,9],[3,40],[4,14]]
17 [17,16,112,7,7] [17,80,260,5,2] [[2,6],[3,49],[4,24],[5,1]]
18 [18,18,126,7,7] [18,115,406,5,2] [[2,4],[3,48],[4,61],[5,2]]
19 [19,21,147,7,7] [19,213,868,5,2] [[2,2],[3,35],[4,121],[5,55]]
20 [20,24,168,7,7] [20,387,1730,6,2] [[2,1],[3,16],[4,199],[5,142],[6,29]]
21 [21,27,189,7,7] [21,624,3022,7,3] [[3,12],[4,159],[5,369],[6,83],[7,1]]
22 [22,30,210,7,7] [22,848,4436,7,3] [[3,8],[4,141],[5,386],[6,273],[7,40]]
   \endverbatim
   </li>
   <li> So the sequence tau_arithprog_hg(7,n) (starting with n=0) begins with
   0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3. </li>
   <li> It follows that the sequence vanderwaerden_{k+1}([2]_k, 7) (starting
   with k=0) begins with
   7,14,21. </li>
   <li> The sequence n - tau_arithprog_hg(7n) = r_7(n):
   0,1,2,3,4,5,6,6,7,8,9,10,11,12,12,13,14,15,16,17,18,18,19. </li>
   <li> The sequence length(arithprog_hg(7,n)[2]) begins with
   1,1,1,1,1,1,1,7,7,8,10,13,17,24,32,47,63,80,115,213,387,624,848. </li>
   <li> And the max-rank sequence begins with
   0,0,0,0,0,0,0,1,2,2,2,2,2,3,3,4,4,5,5,5,6,7,7. </li>
  </ul>

*/
