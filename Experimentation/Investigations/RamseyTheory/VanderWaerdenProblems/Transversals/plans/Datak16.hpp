// Oliver Kullmann, 22.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak16.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 16 (length of arithmetic progressions)


  \todo Elementary statistics for k=16
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A16(n):=arithprog_hg(16,n)$
L160_16 : minimum_transversals_mongen(160,A16,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 300 16 | tee VanderWaerden_Trans_16_300_OUT
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
16 1 16
17 1 15
18 1 14
19 1 13
20 1 12
21 1 11
22 1 10
23 1 9
24 1 8
25 1 7
26 1 6
27 1 5
28 1 4
29 1 3
30 1 2
31 2 108
32 2 64
33 2 56
34 2 49
35 2 42
36 2 36
37 2 30
38 2 25
39 2 20
40 2 16
41 2 12
42 2 9
43 2 6
44 2 4
45 2 2
46 2 1
47 3 120
48 3 24
49 3 20
50 3 16
51 3 12
52 3 8
53 3 6
54 3 4
55 3 3
56 3 2
57 3 1
58 4 721
59 4 501
60 4 326
61 4 103
62 4 30
63 4 14
64 4 8
65 4 6
66 4 5
67 4 4
68 4 3
69 4 2
70 4 1
71 5 458
72 5 310
73 5 174
74 5 80
75 5 39
76 5 22
77 5 9
78 5 6
79 5 5
80 5 4
81 5 3
82 5 2
83 5 1
84 6 158
85 6 116
86 6 78
87 6 48
88 6 35
89 6 22
90 6 11
91 6 6
92 6 5
93 6 4
94 6 3
95 6 2
96 6 1
97 7 129
98 7 96
99 7 67
100 7 40
101 7 29
102 7 18
103 7 9
104 7 6
105 7 5
106 7 4
107 7 3
108 7 2
109 7 1
110 8 127
111 8 96
112 8 67
113 8 40
114 8 29
115 8 18
116 8 9
117 8 6
118 8 5
119 8 4
120 8 3
121 8 2
122 8 1
123 9 127
124 9 96
125 9 67
126 9 40
127 9 29
128 9 18
129 9 9
130 9 6
131 9 5
132 9 4
133 9 3
134 9 2
135 9 1
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> VdWTransversalsInc 16 1 0 VDW_16 VDW_16_SAT

transform_steps_l(map(third,rest(read_nested_list("VDW_16"))));
 [15,30,46,57,70,83,96,109,122,135,148,161,174,187,196,198,199,201]
length(L);
 18

> tail -1 VDW_16
16 203 18

> tail -3 VDW_16_SAT
| 129431776 |     724     4328    30889 |    34411    15458     66 |  5.869 % |
| 194147759 |     720     4305    30810 |    37853    30677     51 |  6.330 % |
| 291221733 |     720     4305    30810 |    41638    27950     53 |  6.330 % |
   \endverbatim
   </li>
  </ul>

*/
