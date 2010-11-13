// Oliver Kullmann, 22.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak15.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 15 (length of arithmetic progressions)


  \todo Elementary statistics for k=15
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A15(n):=arithprog_hg(15,n)$
L150_15 : minimum_transversals_mongen(150,A15,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 250 15 | tee VanderWaerden_Trans_15_250_OUT
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
15 1 15
16 1 14
17 1 13
18 1 12
19 1 11
20 1 10
21 1 9
22 1 8
23 1 7
24 1 6
25 1 5
26 1 4
27 1 3
28 1 2
29 1 1
30 2 64
31 2 56
32 2 49
33 2 42
34 2 36
35 2 30
36 2 25
37 2 20
38 2 16
39 2 12
40 2 9
41 2 6
42 2 4
43 3 245
44 3 90
45 3 30
46 3 24
47 3 20
48 3 16
49 3 12
50 3 8
51 3 6
52 3 4
53 3 3
54 3 2
55 3 1
56 4 362
57 4 161
58 4 71
59 4 31
60 4 12
61 4 10
62 4 8
63 4 6
64 4 5
65 4 4
66 4 3
67 4 2
68 4 1
69 5 271
70 5 160
71 5 43
72 5 14
73 5 10
74 5 8
75 5 7
76 5 6
77 5 5
78 5 4
79 5 3
80 5 2
81 5 1
82 6 96
83 6 66
84 6 38
85 6 24
86 6 13
87 6 10
88 6 7
89 6 6
90 6 5
91 6 4
92 6 3
93 6 2
94 6 1
95 7 94
96 7 64
97 7 37
98 7 24
99 7 11
100 7 8
101 7 7
102 7 6
103 7 5
104 7 4
105 7 3
106 7 2
107 7 1
108 8 93
109 8 64
110 8 37
111 8 24
112 8 13
113 8 10
114 8 7
115 8 6
116 8 5
117 8 4
118 8 3
119 8 2
120 8 1
121 9 93
122 9 64
123 9 37
124 9 24
125 9 13
126 9 10
127 9 7
128 9 6
129 9 5
130 9 4
131 9 3
132 9 2
133 9 1
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation (as provided by
   "VdWTransversalsInc 15 1 0 OutputFile OutputSAT") we obtain
   \verbatim
k n tau
15 1 0
15 2 0
15 3 0
15 4 0
15 5 0
15 6 0
15 7 0
15 8 0
15 9 0
15 10 0
15 11 0
15 12 0
15 13 0
15 14 0
15 15 1
15 16 1
15 17 1
15 18 1
15 19 1
15 20 1
15 21 1
15 22 1
15 23 1
15 24 1
15 25 1
15 26 1
15 27 1
15 28 1
15 29 1
15 30 2
15 31 2
15 32 2
15 33 2
15 34 2
15 35 2
15 36 2
15 37 2
15 38 2
15 39 2
15 40 2
15 41 2
15 42 2
15 43 3
15 44 3
15 45 3
15 46 3
15 47 3
15 48 3
15 49 3
15 50 3
15 51 3
15 52 3
15 53 3
15 54 3
15 55 3
15 56 4
15 57 4
15 58 4
15 59 4
15 60 4
15 61 4
15 62 4
15 63 4
15 64 4
15 65 4
15 66 4
15 67 4
15 68 4
15 69 5
15 70 5
15 71 5
15 72 5
15 73 5
15 74 5
15 75 5
15 76 5
15 77 5
15 78 5
15 79 5
15 80 5
15 81 5
15 82 6
15 83 6
15 84 6
15 85 6
15 86 6
15 87 6
15 88 6
15 89 6
15 90 6
15 91 6
15 92 6
15 93 6
15 94 6
15 95 7
15 96 7
15 97 7
15 98 7
15 99 7
15 100 7
15 101 7
15 102 7
15 103 7
15 104 7
15 105 7
15 106 7
15 107 7
15 108 8
15 109 8
15 110 8
15 111 8
15 112 8
15 113 8
15 114 8
15 115 8
15 116 8
15 117 8
15 118 8
15 119 8
15 120 8
15 121 9
15 122 9
15 123 9
15 124 9
15 125 9
15 126 9
15 127 9
15 128 9
15 129 9
15 130 9
15 131 9
15 132 9
15 133 9
15 134 10
15 135 10
15 136 10
15 137 10
15 138 10
15 139 10
15 140 10
15 141 10
15 142 10
15 143 10
15 144 10
15 145 10
15 146 10
15 147 11
15 148 11
15 149 11
15 150 11
15 151 11
15 152 11
15 153 11
15 154 11
15 155 11
15 156 11
15 157 11
15 158 11
15 159 11
15 160 12
15 161 12
15 162 12
15 163 12
15 164 12
15 165 12
15 166 12
15 167 12
15 168 12
15 169 12
15 170 12
15 171 12
15 172 12
15 173 13
15 174 13
15 175 13
15 176 13
15 177 13
15 178 13
15 179 13
15 180 13
15 181 13
15 182 13
15 183 13
15 184 14
15 185 15
15 186 16
15 187 16
15 188 17
15 189 18
   \endverbatim
   </li>
  </ul>

*/
