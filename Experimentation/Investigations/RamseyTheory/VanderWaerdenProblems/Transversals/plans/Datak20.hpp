// Oliver Kullmann, 25.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak20.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 20 (length of arithmetic progressions)


  \todo Elementary statistics for k=20
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A20(n):=arithprog_hg(20,n)$
L200_20 : minimum_transversals_mongen(200,A20,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 300 20 | tee VanderWaerden_Trans_20_300_OUT
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
20 1 20
21 1 19
22 1 18
23 1 17
24 1 16
25 1 15
26 1 14
27 1 13
28 1 12
29 1 11
30 1 10
31 1 9
32 1 8
33 1 7
34 1 6
35 1 5
36 1 4
37 1 3
38 1 2
39 2 165
40 2 100
41 2 90
42 2 81
43 2 72
44 2 64
45 2 56
46 2 49
47 2 42
48 2 36
49 2 30
50 2 25
51 2 20
52 2 16
53 2 12
54 2 9
55 2 6
56 2 4
57 2 2
58 3 504
59 3 268
60 3 96
61 3 84
62 3 72
63 3 63
64 3 54
65 3 45
66 3 36
67 3 30
68 3 24
69 3 20
70 3 16
71 3 12
72 3 8
73 3 6
74 3 4
75 3 3
76 3 2
77 4 383
78 4 147
79 4 76
80 4 43
81 4 38
82 4 33
83 4 28
84 4 23
85 4 18
86 4 16
87 4 14
88 4 12
89 4 10
90 4 8
91 4 6
92 4 5
93 4 4
94 4 3
95 4 2
96 5 186
97 5 74
98 5 38
99 5 28
100 5 24
101 5 22
102 5 20
103 5 18
104 5 16
105 5 14
106 5 12
107 5 10
108 5 8
109 5 7
110 5 6
111 5 5
112 5 4
113 5 3
114 5 2
115 6 116
116 6 60
117 6 36
118 6 30
119 6 24
120 6 20
121 6 18
122 6 16
123 6 14
124 6 12
125 6 10
126 6 9
127 6 8
128 6 7
129 6 6
130 6 5
131 6 4
132 6 3
133 6 2
134 7 110
135 7 54
136 7 32
137 7 24
138 7 20
139 7 18
140 7 16
141 7 14
142 7 12
143 7 11
144 7 10
145 7 9
146 7 8
147 7 7
148 7 6
149 7 5
150 7 4
151 7 3
152 7 2
153 8 104
154 8 50
155 8 32
156 8 22
157 8 18
158 8 16
159 8 14
160 8 13
161 8 12
162 8 11
163 8 10
164 8 9
165 8 8
166 8 7
167 8 6
168 8 5
169 8 4
170 8 3
171 8 2
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> VdWTransversalsInc 20 1 0 VDW_20 VDW_20_SAT

L : transform_steps_l(map(third,rest(read_nested_list("VDW_20"))));
 [19,38,57,76,95,114,133,152,171,190,209,228,247,266,285,304,323,342,362,363,364,365,366]
length(L);
 23

> tail -1 VDW_20
20 367 23
> tail  VDW_20_SAT
|  17044345 |    1324     9109    87110 |    43849    22025    110 |  4.003 % |
|  25566616 |    1324     9109    87110 |    48234    38887    106 |  4.003 % |
|  38350020 |    1324     9109    87110 |    53057    18782     89 |  4.003 % |
|  57525125 |    1324     9109    87110 |    58363    22232    106 |  4.003 % |
|  86287785 |    1324     9109    87110 |    64199    46620     87 |  4.003 % |
| 129431774 |    1324     9109    87110 |    70619    59737    104 |  4.003 % |
| 194147760 |    1324     9109    87110 |    77681    30665     79 |  4.003 % |
| 291221733 |    1324     9109    87110 |    85450    36042     90 |  4.003 % |
| 436832694 |    1314     9063    86991 |    93995    34845    101 |  4.670 % |
| 655249134 |    1314     9063    86991 |   103394    52626     89 |  4.670 % |
   \endverbatim
   </li>
  </ul>

*/
