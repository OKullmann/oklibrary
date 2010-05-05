// Oliver Kullmann, 2.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak22.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 22 (length of arithmetic progressions)


  \todo Elementary statistics for k=22
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A22(n):=arithprog_hg(22,n)$
L220_22 : minimum_transversals_mongen(220,A22,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 300 22 | tee VanderWaerden_Trans_22_300_OUT

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
22 1 22
23 1 21
24 1 20
25 1 19
26 1 18
27 1 17
28 1 16
29 1 15
30 1 14
31 1 13
32 1 12
33 1 11
34 1 10
35 1 9
36 1 8
37 1 7
38 1 6
39 1 5
40 1 4
41 1 3
42 1 2
43 2 198
44 2 121
45 2 110
46 2 100
47 2 90
48 2 81
49 2 72
50 2 64
51 2 56
52 2 49
53 2 42
54 2 36
55 2 30
56 2 25
57 2 20
58 2 16
59 2 12
60 2 9
61 2 6
62 2 4
63 2 2
64 2 1
65 3 276
66 3 72
67 3 63
68 3 54
69 3 45
70 3 36
71 3 30
72 3 24
73 3 20
74 3 16
75 3 12
76 3 8
77 3 6
78 3 4
79 3 3
80 3 2
81 3 1
82 4 1427
83 4 1026
84 4 701
85 4 364
86 4 209
87 4 76
88 4 23
89 4 18
90 4 16
91 4 14
92 4 12
93 4 10
94 4 8
95 4 6
96 4 5
97 4 4
98 4 3
99 4 2
100 4 1
101 5 955
102 5 680
103 5 445
104 5 260
105 5 155
106 5 86
107 5 40
108 5 24
109 5 16
110 5 12
111 5 10
112 5 8
113 5 7
114 5 6
115 5 5
116 5 4
117 5 3
118 5 2
119 5 1
120 6 361
121 6 271
122 6 191
125 6 60
126 6 35
127 6 20
128 6 14
129 6 10
130 6 9
131 6 8
132 6 7
133 6 6
134 6 5
135 6 4
136 6 3
137 6 2
138 6 1
139 7 218
140 7 168
141 7 122
142 7 78
143 7 58
144 7 38
145 7 22
146 7 16
147 7 13
148 7 10
149 7 9
150 7 8
151 7 7
152 7 6
153 7 5
154 7 4
155 7 3
156 7 2
157 7 1
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> VdWTransversalsInc 22 1 0 VDW_22 VDW_22_SAT

L : transform_steps_l(map(third,rest(read_nested_list("VDW_22"))));
 [21,42,64,81,100,119,138,157,176,195,214,233,252,271,290,309,328,347,366,385]
length(L);
 20

> tail -1 VDW_22
22 386 20
   \endverbatim
   </li>
  </ul>

*/
