// Oliver Kullmann, 5.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/Datak6.hpp
  \brief Investigating the transversal hypergraph of Green-Tao hypergraphs for k = 6 (length of arithmetic progressions)


  \todo Elementary statistics
  <ul>
   <li> Investigating tr_arithprog_hg(6,n). </li>
   <li> The numbers of minimum hyperedges:
   \verbatim
A6p(n):=arithprog_primes_hg(6,n)$
oklib_monitor : true$
Lp100_6 : minimum_transversals_mongen(100,A6p,[{}])$
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
7 0 1
8 0 1
9 0 1
10 0 1
11 0 1
12 0 1
13 0 1
14 0 1
15 0 1
16 0 1
17 0 1
18 0 1
19 0 1
20 0 1
21 0 1
22 0 1
23 0 1
24 0 1
25 0 1
26 0 1
27 0 1
28 0 1
29 0 1
30 0 1
31 0 1
32 0 1
33 0 1
34 0 1
35 0 1
36 0 1
37 1 6
38 1 6
39 1 6
40 1 6
41 1 6
42 1 6
43 1 6
44 1 6
45 1 6
46 1 6
47 1 6
48 1 6
49 1 6
50 1 6
51 1 6
52 1 6
53 1 6
54 1 6
55 2 36
56 2 36
57 2 36
58 2 36
59 2 36
60 2 36
61 2 36
62 2 36
63 2 36
64 3 216
65 3 216
66 3 216
67 3 216
68 3 216
69 3 216
70 3 216
71 4 1296
72 4 1296
73 4 1296
74 4 1296
75 4 1296
76 4 1296
77 4 1296
78 4 1296
79 4 1296
80 4 1296
81 4 1296
82 4 1296
83 4 1296
84 4 1296
85 4 1296
86 4 1296
87 4 1296
88 4 1296
89 4 1296
90 5 7776
91 5 7776
92 5 7776
93 5 7776
94 5 7776
95 5 7776
96 5 7776
97 6 46656
98 6 46656
99 6 46656
100 6 46656
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation (as provided by "GTTransversalsInc 6 1 0 OutputFile"):
   \verbatim

   \endverbatim
   </li>
  </ul>

*/

