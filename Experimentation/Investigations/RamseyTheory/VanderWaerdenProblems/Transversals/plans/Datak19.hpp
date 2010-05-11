// Oliver Kullmann, 23.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak19.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 19 (length of arithmetic progressions)


  \todo Elementary statistics for k=19
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A19(n):=arithprog_hg(19,n)$
L190_19 : minimum_transversals_mongen(190,A19,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 300 19 | tee VanderWaerden_Trans_19_300_OUT
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
19 1 19
20 1 18
21 1 17
22 1 16
23 1 15
24 1 14
25 1 13
26 1 12
27 1 11
28 1 10
29 1 9
30 1 8
31 1 7
32 1 6
33 1 5
34 1 4
35 1 3
36 1 2
37 1 1
38 2 100
39 2 90
40 2 81
41 2 72
42 2 64
43 2 56
44 2 49
45 2 42
46 2 36
47 2 30
48 2 25
49 2 20
50 2 16
51 2 12
52 2 9
53 2 6
54 2 4
55 2 2
56 2 1
57 3 112
58 3 96
59 3 84
60 3 72
61 3 63
62 3 54
63 3 45
64 3 36
65 3 30
66 3 24
67 3 20
68 3 16
69 3 12
70 3 8
71 3 6
72 3 4
73 3 3
74 3 2
75 3 1
76 4 57
77 4 48
78 4 43
79 4 38
80 4 33
81 4 28
82 4 23
83 4 18
84 4 16
85 4 14
86 4 12
87 4 10
88 4 8
89 4 6
90 4 5
91 4 4
92 4 3
93 4 2
94 4 1
95 5 30
96 5 28
97 5 26
98 5 24
99 5 22
100 5 20
101 5 18
102 5 16
103 5 14
104 5 12
105 5 10
106 5 8
107 5 7
108 5 6
109 5 5
110 5 4
111 5 3
112 5 2
113 5 1
114 6 28
115 6 26
116 6 24
117 6 22
118 6 20
119 6 18
120 6 16
121 6 14
122 6 12
123 6 10
124 6 9
125 6 8
126 6 7
127 6 6
128 6 5
129 6 4
130 6 3
131 6 2
132 6 1
133 7 26
134 7 24
135 7 22
136 7 20
137 7 18
138 7 16
139 7 14
140 7 12
141 7 11
142 7 10
143 7 9
144 7 8
145 7 7
146 7 6
147 7 5
148 7 4
149 7 3
150 7 2
151 7 1
152 8 24
153 8 22
154 8 20
155 8 18
156 8 16
157 8 14
158 8 13
159 8 12
160 8 11
161 8 10
162 8 9
163 8 8
164 8 7
165 8 6
166 8 5
167 8 4
168 8 3
169 8 2
170 8 1
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> VdWTransversalsInc 19 1 0 VDW_19 VDW_19_SAT

L : transform_steps_l(map(third,rest(read_nested_list("VDW_19"))));
 [18,37,56,75,94,113,132,151,170,189,208,227,246,265,284,303,322,341,342,343,344,345,346]
length(L);
 23

> tail -1 VDW_19
19 347 23
> tail -1 VDW_19_SAT
| 436832715 |    1293     8727    79364 |    90594    78210    100 |  4.801 % |
   \endverbatim
   </li>
  </ul>

*/
