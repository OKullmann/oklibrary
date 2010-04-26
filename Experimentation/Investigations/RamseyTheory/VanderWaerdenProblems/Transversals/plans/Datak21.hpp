// Oliver Kullmann, 26.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak21.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 21 (length of arithmetic progressions)


  \todo Elementary statistics for k=21
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A21(n):=arithprog_hg(21,n)$
L210_21 : minimum_transversals_mongen(210,A21,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 300 21 | tee VanderWaerden_Trans_21_300_OUT
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
21 1 21
22 1 20
23 1 19
24 1 18
25 1 17
26 1 16
27 1 15
28 1 14
29 1 13
30 1 12
31 1 11
32 1 10
33 1 9
34 1 8
35 1 7
36 1 6
37 1 5
38 1 4
39 1 3
40 1 2
41 1 1
42 2 121
43 2 110
44 2 100
45 2 90
46 2 81
47 2 72
48 2 64
49 2 56
50 2 49
51 2 42
52 2 36
53 2 30
54 2 25
55 2 20
56 2 16
57 2 12
58 2 9
59 2 6
60 2 4
61 3 546
62 3 228
63 3 84
64 3 72
65 3 63
66 3 54
67 3 45
68 3 36
69 3 30
70 3 24
71 3 20
72 3 16
73 3 12
74 3 8
75 3 6
76 3 4
77 3 3
78 3 2
79 3 1
80 4 799
81 4 462
82 4 196
83 4 76
84 4 33
85 4 28
86 4 23
87 4 18
88 4 16
89 4 14
90 4 12
91 4 10
92 4 8
93 4 6
94 4 5
95 4 4
96 4 3
97 4 2
98 4 1
99 5 501
100 5 316
101 5 137
102 5 60
103 5 26
104 5 20
105 5 18
106 5 16
107 5 14
108 5 12
109 5 10
110 5 8
111 5 7
112 5 6
113 5 5
114 5 4
115 5 3
116 5 2
117 5 1
118 6 186
119 6 133
120 6 84
121 6 52
122 6 30
123 6 24
124 6 18
125 6 14
126 6 12
127 6 10
128 6 9
129 6 8
130 6 7
131 6 6
132 6 5
133 6 4
134 6 3
135 6 2
136 6 1
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> VdWTransversalsInc 21 1 0 VDW_21 VDW_21_SAT

L : transform_steps_l(map(third,rest(read_nested_list("VDW_21"))));
 [20,41,60,79,98,117,136,155,174,193,212,231,250,269,288]
length(L);
 15

> tail -1 VDW_21

   \endverbatim
   </li>
  </ul>

*/
