// Oliver Kullmann, 23.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak18.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 18 (length of arithmetic progressions)


  \todo Elementary statistics for k=18
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A18(n):=arithprog_hg(18,n)$
L180_18 : minimum_transversals_mongen(180,A18,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 300 18 | tee VanderWaerden_Trans_18_300_OUT
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
18 1 18
19 1 17
20 1 16
21 1 15
22 1 14
23 1 13
24 1 12
25 1 11
26 1 10
27 1 9
28 1 8
29 1 7
30 1 6
31 1 5
32 1 4
33 1 3
34 1 2
35 2 135
36 2 81
37 2 72
38 2 64
39 2 56
40 2 49
41 2 42
42 2 36
43 2 30
44 2 25
45 2 20
46 2 16
47 2 12
48 2 9
49 2 6
50 2 4
51 2 2
52 3 320
53 3 141
54 3 60
55 3 51
56 3 42
57 3 35
58 3 28
59 3 24
60 3 20
61 3 16
62 3 12
63 3 9
64 3 6
65 3 5
66 3 4
67 3 3
68 3 2
69 4 308
70 4 155
71 4 61
72 4 27
73 4 22
74 4 17
75 4 12
76 4 11
77 4 10
78 4 9
79 4 8
80 4 7
81 4 6
82 4 5
83 4 4
84 4 3
85 4 2
86 5 158
87 5 76
88 5 38
89 5 21
90 5 14
91 5 13
92 5 12
93 5 11
94 5 10
95 5 9
96 5 8
97 5 7
98 5 6
99 5 5
100 5 4
101 5 3
102 5 2
103 6 102
104 6 50
105 6 32
106 6 24
107 6 16
108 6 13
109 6 12
110 6 11
111 6 10
112 6 9
113 6 8
114 6 7
115 6 6
116 6 5
117 6 4
118 6 3
119 6 2
120 7 98
121 7 47
122 7 26
123 7 17
124 7 14
125 7 13
126 7 12
127 7 11
128 7 10
129 7 9
130 7 8
131 7 7
132 7 6
133 7 5
134 7 4
135 7 3
136 7 2
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> VdWTransversalsInc 18 1 0 VDW_18 VDW_18_SAT

L : transform_steps_l(map(third,rest(read_nested_list("VDW_18"))));
[17,34,51,68,85,102,119,136,153,170,187,204,221,238,255,272,290,291,292,293,294]
length(L);
 21

> tail -1 VDW_18
18 295 21
   \endverbatim
   </li>
  </ul>

*/
