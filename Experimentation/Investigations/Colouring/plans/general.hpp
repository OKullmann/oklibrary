// Oliver Kullmann, 10.3.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Colouring/plans/general.hpp
  \brief General plans regarding investigations on colouring problems


  \todo Connections
  <ul>
   <li> Ramsey theory yields a rich reservoir of (hypergraph) colouring
   problems; see Experimentation/Investigations/RamseyTheory/plans/general.hpp.
   </li>
  </ul>


  \todo Graph colouring benchmarks
  <ul>
   <li> http://mat.gsia.cmu.edu/COLOR04/ is perhaps the largest benchmark
   suite. </li>
   <li> The file-format has the following possible lines:
    <ol>
     <li> "p edges n m" </li>
     <li> "e n1 n2" or "e n1 n2 d" to enforce distance d for the colours of
     the endpoints (default=1) </li>
     <li> "f n1 c1 c2 c3 ..." to restrict the colour choice of vertex n1 </li>
     <li> "n n1 c1" for "multicolouring problems", stating that at least c1
     colours must be assigned to n1; likely best to abort with an error in
     this case. </li>
    </ol>
   </li>
   <li> [Tamura, Taga, Kitagawa, Banbara; Compiling finite linear {CSP} into
   {SAT}, Constraints, 2009, 14, 2, 254-272] can solve with their "order
   encoding" 44 of 119 benchmark problems from there (under the heading
   "Graph Coloring Instances". </li>
   <li> Apparently the information given on http://mat.gsia.cmu.edu/COLOR04/,
   for which problems the chromatic numbers are known, is not up-to-date.
   Is there somewhere better information? </li>
  </ul>


  \todo Searching for small hard planar graphs
  <ul>
   <li> We have search_planar3col(n,d,N) for searching for d-regular
   3-colourable planar graphs with n vertices; but this approach is perhaps
   too simple:
   \verbatim
R:search_planar3col(12,4,1000);
  [1000,893,2,709,
   [[1,2,3,4,5,6,7,8,9,10,11,12],
    [{8,12},{3,11},{5,7},{7,11},{9,10},{5,8},{4,7},{1,10},{6,9},{1,9},{4,5},{3,12},{2,6},{3,4},
     {8,11},{6,10},{4,11},{5,6},{1,12},{3,10},{2,9},{7,8},{1,2},{2,12}]]]
G : R[5];
draw_g(G);
greedy_colouring_og(G);
  [4,[1,2,1,2,1,3,3,2,4,2,4,3]]
# is this graph interesting? what is the proportion of successful greedy
# colourings?
dimacs_export(og2mg(G), "Reg_4_12.gr");
# transformed by hand (xemacs, reg-expr) into "dot-format":
OKplatform> cat Reg_4_12.gr
graph G {
2 -- 12
1 -- 2
7 -- 8
2 -- 9
3 -- 10
1 -- 12
5 -- 6
4 -- 11
6 -- 10
8 -- 11
3 -- 4
2 -- 6
3 -- 12
4 -- 5
1 -- 9
6 -- 9
1 -- 10
4 -- 7
5 -- 8
9 -- 10
7 -- 11
5 -- 7
3 -- 11
8 -- 12
}
> cat Reg_4_12.gr | system_directories/bin/GreedyColouring-O3-DNDEBUG v
lexicographical order = 5
1 10 11 12 2 3 4 5 6 7 8 9
1 -> 0, 10 -> 1, 11 -> 0, 12 -> 1, 2 -> 2, 3 -> 2, 4 -> 1, 5 -> 0, 6 -> 3, 7 -> 2, 8 -> 3, 9 -> 4,
smallest degrees first = 5
1 10 11 12 2 3 4 5 6 7 8 9
1 -> 0, 10 -> 1, 11 -> 0, 12 -> 1, 2 -> 2, 3 -> 2, 4 -> 1, 5 -> 0, 6 -> 3, 7 -> 2, 8 -> 3, 9 -> 4,
largest degrees first = 4
9 8 7 6 5 4 3 2 12 11 10 1
9 -> 0, 8 -> 0, 7 -> 1, 6 -> 1, 5 -> 2, 4 -> 0, 3 -> 1, 2 -> 2, 12 -> 3, 11 -> 2, 10 -> 2, 1 -> 1,

0 : 0
1 : 0
2 : 0
3 : 55075788
4 : 360497626
5 : 63428186
6 : 0
7 : 0
8 : 0
9 : 0
10 : 0
11 : 0
12 : 0
min numbers of colours = 3
1 10 2 12 3 4 11 6 5 7 8 9
1 -> 0, 10 -> 1, 2 -> 1, 12 -> 2, 3 -> 0, 4 -> 1, 11 -> 2, 6 -> 0, 5 -> 2, 7 -> 0, 8 -> 1, 9 -> 2,
# not too bad


R:search_planar3col(13,4,10000);
[10000,932,2,714,
 [[1,2,3,4,5,6,7,8,9,10,11,12,13],
  [{10,13},{8,10},{11,13},{4,5},{2,12},{5,7},{3,5},{2,11},{1,2},{4,8},{6,8},{1,3},{4,7},
   {6,7},{2,3},{1,11},{5,10},{6,9},{8,9},{6,12},{4,9},{1,13},{10,12},{7,9},{11,12},{3,13}]]]
G : R[5];
draw_g(G);
greedy_colouring_og(G);
  [4,[1,2,3,1,2,1,3,2,4,1,3,4,2]]
dimacs_export(og2mg(G), "Reg_4_13.gr");
# transformed by hand (xemacs, reg-expr) into "dot-format":
OKplatform> cat Reg_4_13.gr
graph G {
3 -- 13
11 -- 12
7 -- 9
10 -- 12
1 -- 13
4 -- 9
6 -- 12
8 -- 9
6 -- 9
5 -- 10
1 -- 11
2 -- 3
6 -- 7
4 -- 7
1 -- 3
6 -- 8
4 -- 8
1 -- 2
2 -- 11
3 -- 5
5 -- 7
2 -- 12
4 -- 5
11 -- 13
8 -- 10
10 -- 13
}
> time cat Reg_4_13.gr | system_directories/bin/GreedyColouring-O3-DNDEBUG v
lexicographical order = 4
1 10 11 12 13 2 3 4 5 6 7 8 9
1 -> 0, 10 -> 0, 11 -> 1, 12 -> 2, 13 -> 2, 2 -> 3, 3 -> 1, 4 -> 0, 5 -> 2, 6 -> 0, 7 -> 1, 8 -> 1, 9 -> 2,
smallest degrees first = 4
1 10 11 12 13 2 3 4 5 6 7 8 9
1 -> 0, 10 -> 0, 11 -> 1, 12 -> 2, 13 -> 2, 2 -> 3, 3 -> 1, 4 -> 0, 5 -> 2, 6 -> 0, 7 -> 1, 8 -> 1, 9 -> 2,
largest degrees first = 4
9 8 7 6 5 4 3 2 13 12 11 10 1
9 -> 0, 8 -> 1, 7 -> 1, 6 -> 2, 5 -> 0, 4 -> 2, 3 -> 1, 2 -> 0, 13 -> 0, 12 -> 1, 11 -> 2, 10 -> 2, 1 -> 3,
0 : 0
1 : 0
2 : 0
3 : 886477608
4 : 4490584546
5 : 849958646
6 : 0
7 : 0
8 : 0
9 : 0
10 : 0
11 : 0
12 : 0
13 : 0
min numbers of colours = 3
1 11 12 10 13 2 3 5 4 6 7 9 8
1 -> 0, 11 -> 1, 12 -> 0, 10 -> 1, 13 -> 2, 2 -> 2, 3 -> 1, 5 -> 0, 4 -> 1, 6 -> 1, 7 -> 2, 9 -> 0, 8 -> 2,
max number of colours = 5
1 10 11 12 13 2 3 7 8 4 5 6 9
1 -> 0, 10 -> 0, 11 -> 1, 12 -> 2, 13 -> 2, 2 -> 3, 3 -> 1, 7 -> 0, 8 -> 1, 4 -> 2, 5 -> 3, 6 -> 3, 9 -> 4,
user    42m4.458s


R:search_planar3col(14,4,10000);
[10000,2053,1,1859,
       [[1,2,3,4,5,6,7,8,9,10,11,12,13,14],
        [{6,8},{5,10},{3,4},{9,10},{7,8},{3,13},{8,12},{4,5},{7,12},{6,13},{2,11},{8,10},{11,12},
         {4,13},{2,13},{6,14},{3,14},{5,11},{3,6},{10,14},{1,2},{2,7},{1,12},{9,14},{1,7},{5,9},
         {1,11},{4,9}]]]
G:R[5];
greedy_colouring_og(G);
  [4,[1,2,1,2,1,2,3,1,3,2,3,2,3,4]]
dimacs_export(og2mg(G), "Reg_4_14.gr");
# transformed by hand (xemacs, reg-expr) into "dot-format":
> time cat Reg_4_14.gr | system_directories/bin/GreedyColouring-O3-DNDEBUG v
lexicographical order = 5
1 10 11 12 13 14 2 3 4 5 6 7 8 9
1 -> 0, 10 -> 0, 11 -> 1, 12 -> 2, 13 -> 0, 14 -> 1, 2 -> 2, 3 -> 2, 4 -> 1, 5 -> 2, 6 -> 3, 7 -> 1, 8 -> 4, 9 -> 3,
smallest degrees first = 5
1 10 11 12 13 14 2 3 4 5 6 7 8 9
1 -> 0, 10 -> 0, 11 -> 1, 12 -> 2, 13 -> 0, 14 -> 1, 2 -> 2, 3 -> 2, 4 -> 1, 5 -> 2, 6 -> 3, 7 -> 1, 8 -> 4, 9 -> 3,
largest degrees first = 5
9 8 7 6 5 4 3 2 14 13 12 11 10 1
9 -> 0, 8 -> 0, 7 -> 1, 6 -> 1, 5 -> 1, 4 -> 2, 3 -> 0, 2 -> 0, 14 -> 2, 13 -> 3, 12 -> 2, 11 -> 3, 10 -> 3, 1 -> 4,
0 : 0
1 : 0
2 : 0
3 : 10218310610
4 : 67042655492
5 : 9917325098
6 : 0
7 : 0
8 : 0
9 : 0
10 : 0
11 : 0
12 : 0
13 : 0
14 : 0
min numbers of colours = 3
1 11 12 13 14 10 2 4 3 5 6 7 8 9
1 -> 0, 11 -> 1, 12 -> 2, 13 -> 0, 14 -> 0, 10 -> 1, 2 -> 2, 4 -> 1, 3 -> 2, 5 -> 0, 6 -> 1, 7 -> 1, 8 -> 0, 9 -> 2,
max number of colours = 5
1 10 11 12 13 14 2 3 4 5 6 7 9 8
1 -> 0, 10 -> 0, 11 -> 1, 12 -> 2, 13 -> 0, 14 -> 1, 2 -> 2, 3 -> 2, 4 -> 1, 5 -> 2, 6 -> 3, 7 -> 1, 9 -> 3, 8 -> 4,
user    1012m41.891s


R:search_planar3col(15,4,10000);
[10000,1395,1,1257,
  [[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15],
   [{5,9},{3,11},{3,13},{3,15},{2,7},{1,13},{5,6},{12,14},{2,8},{4,15},{3,7},{1,12},{6,9},
    {4,7},{13,15},{1,10},{5,12},{2,10},{6,12},{2,9},{5,10},{7,11},{8,9},{1,14},{8,14},{6,8},
    {13,14},{11,15},{4,10},{4,11}]]]
G:R[5];
greedy_colouring_og(G);
  [4,[1,1,1,1,1,2,2,3,4,2,3,3,2,4,4]]
dimacs_export(og2mg(G), "Reg_4_15.gr");
# transformed by hand (xemacs, reg-expr) into "dot-format":
> time cat Reg_4_15.gr | system_directories/bin/GreedyColouring-O3-DNDEBUG v
lexicographical order = 4
1 10 11 12 13 14 15 2 3 4 5 6 7 8 9
1 -> 0, 10 -> 1, 11 -> 0, 12 -> 1, 13 -> 1, 14 -> 2, 15 -> 2, 2 -> 0, 3 -> 3, 4 -> 3, 5 -> 0, 6 -> 2, 7 -> 1, 8 -> 1, 9 -> 3,
smallest degrees first = 4
1 10 11 12 13 14 15 2 3 4 5 6 7 8 9
1 -> 0, 10 -> 1, 11 -> 0, 12 -> 1, 13 -> 1, 14 -> 2, 15 -> 2, 2 -> 0, 3 -> 3, 4 -> 3, 5 -> 0, 6 -> 2, 7 -> 1, 8 -> 1, 9 -> 3,
largest degrees first = 4
9 8 7 6 5 4 3 2 15 14 13 12 11 10 1
9 -> 0, 8 -> 1, 7 -> 0, 6 -> 2, 5 -> 1, 4 -> 1, 3 -> 1, 2 -> 2, 15 -> 0, 14 -> 0, 13 -> 2, 12 -> 3, 11 -> 2, 10 -> 0, 1 -> 1,
0 : 0
1 : 0
2 : 0
3 : 46889661941
4 : 1128787440320
5 : 131997265739
6 : 0
7 : 0
8 : 0
9 : 0
10 : 0
11 : 0
12 : 0
13 : 0
14 : 0
15 : 0

min numbers of colours = 3
1 10 12 13 14 15 11 3 4 5 6 7 2 8 9
1 -> 0, 10 -> 1, 12 -> 1, 13 -> 1, 14 -> 2, 15 -> 0, 11 -> 1, 3 -> 2, 4 -> 2, 5 -> 0, 6 -> 2, 7 -> 0, 2 -> 2, 8 -> 0, 9 -> 1,
max number of colours = 5
1 10 11 12 14 2 3 13 4 15 5 6 7 8 9
1 -> 0, 10 -> 1, 11 -> 0, 12 -> 1, 14 -> 2, 2 -> 0, 3 -> 1, 13 -> 3, 4 -> 2, 15 -> 4, 5 -> 0, 6 -> 2, 7 -> 3, 8 -> 1, 9 -> 3,

user    15697m41.015s (cs-oksvr)

float(46889661941/15!)*100;
  3.5857292219250723


R:search_planar3col(10,5,20000);
 [20000,20000,0,1020]

   \endverbatim
   The assumption is that the proportion of 3-colouring-orders is an indication
   for hardness w.r.t. human-search. One should try this out in the CS-270
   tutorial. </li>
  </ul>

*/

