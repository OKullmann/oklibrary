// Oliver Kullmann, 20.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/SchurProblems/Transversals/plans/Datak3.hpp
  \brief Investigating the transversal hypergraph of Schur hypergraphs for k = 3 (length of arithmetic progressions)


  \todo Elementary statistics
  <ul>
   <li> Just using clause-set statistics-functions:
   \verbatim
S3(n) := schurtriples_hg(n);
for n : 0 thru 20 do block(
 [G : S3(n), T],
  T : transversal_hg_rs(G),
  print(n, statistics_fcs(G), statistics_fcs(T), ncl_list_fcs(T))
)$
   \endverbatim
   we obtain
   \verbatim
0 [0,0,0,-1,inf] [0,1,0,0,0] [[0,1]]
1 [1,0,0,-1,inf] [1,1,0,0,0] [[0,1]]
2 [2,1,2,2,2] [2,2,2,1,1] [[1,2]]
3 [3,2,5,3,2] [3,2,2,1,1] [[1,2]]
4 [4,4,10,3,2] [4,4,8,2,2] [[2,4]]
5 [5,6,16,3,2] [5,5,13,3,2] [[2,2],[3,3]]
6 [6,9,24,3,2] [6,6,19,4,3] [[3,5],[4,1]]
7 [7,12,33,3,2] [7,8,30,4,3] [[3,2],[4,6]]
8 [8,16,44,3,2] [8,13,61,5,4] [[4,4],[5,9]]
9 [9,20,56,3,2] [9,17,89,6,4] [[4,2],[5,9],[6,6]]
10 [10,25,70,3,2] [10,23,137,7,5] [[5,3],[6,18],[7,2]]
11 [11,30,85,3,2] [11,29,194,7,5] [[5,2],[6,5],[7,22]]
12 [12,36,102,3,2] [12,37,272,8,6] [[6,3],[7,18],[8,16]]
13 [13,42,120,3,2] [13,51,414,9,6] [[6,2],[7,4],[8,31],[9,14]]
14 [14,49,140,3,2] [14,66,588,10,7] [[7,3],[8,11],[9,41],[10,11]]
15 [15,56,161,3,2] [15,86,820,11,7] [[7,2],[8,2],[9,36],[10,40],[11,6]]
16 [16,64,184,3,2] [16,118,1219,12,8] [[8,3],[9,5],[10,63],[11,44],[12,3]]
17 [17,72,208,3,2] [17,158,1750,12,8] [[8,2],[9,2],[10,19],[11,94],[12,41]]
18 [18,81,234,3,2] [18,201,2388,13,9] [[9,3],[10,5],[11,43],[12,112],[13,38]]
19 [19,90,261,3,2] [19,265,3341,14,9] [[9,2],[10,2],[11,12],[12,96],[13,123],[14,30]]
20 [20,100,290,3,2] [20,359,4834,15,10] [[10,3],[11,3],[12,30],[13,131],[14,172],[15,20]]
   \endverbatim
   </li>
   <li> The number of minimum transversals:
      \verbatim
oklib_monitor : true;
SL60_3 : minimum_transversals_mongen(60,S3,[{}])$
1 0 1
2 1 2
3 1 2
4 2 4
5 2 2
6 3 5
7 3 2
8 4 4
9 4 2
10 5 3
11 5 2
12 6 3
13 6 2
14 7 3
15 7 2
16 8 3
17 8 2
18 9 3
19 9 2
20 10 3
21 10 2
22 11 3
23 11 2
24 12 3
25 12 2
26 13 3
27 13 2
28 14 3
29 14 2
30 15 3
31 15 2
32 16 3
33 16 2
34 17 3
35 17 2
36 18 3
37 18 2
38 19 3
39 19 2
40 20 3
41 20 2
42 21 3
43 21 2
44 22 3
45 22 2
46 23 3
47 23 2
48 24 3
49 24 2
50 25 3
51 25 2
52 26 3
53 26 2
54 27 3
55 27 2
56 28 3
57 28 2
58 29 3
59 29 2
60 30 3
   \endverbatim
   </li>
   <li> This looks like the following law (which shouldn't be so difficult
   to prove):
    <ol>
     <li> For n >= 0 the transversal number is floor(n/2). </li>
     <li> For n >= 10 and even n we have 3 minimum transversals, and for odd n
     we have 2 (one dropped). </li>
     <li> Actually for odd n this seems to hold for n >= 3. </li>
     <li> For even n the minimum transversals seem to be
     {2 i : 1 <= i <= n/2}, {1,2, ..., n/2}, {1, ..., n/2-1, n}. </li>
     <li> And for odd n the first two transversal still work, but
     not the last one. </li>
    </ol>
   </li>
  </ul>

*/
