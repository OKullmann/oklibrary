// Oliver Kullmann, 21.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak14.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 14 (length of arithmetic progressions)


  \todo Elementary statistics for k=14
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A14(n):=arithprog_hg(14,n)$
L150_14 : minimum_transversals_mongen(150,A14,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 250 14 | tee VanderWaerden_Trans_14_250_OUT
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
14 1 14
15 1 13
16 1 12
17 1 11
18 1 10
19 1 9
20 1 8
21 1 7
22 1 6
23 1 5
24 1 4
25 1 3
26 1 2
27 2 84
28 2 49
29 2 42
30 2 36
31 2 30
32 2 25
33 2 20
34 2 16
35 2 12
36 2 9
37 2 6
38 2 4
39 2 2
40 3 218
41 3 111
42 3 36
43 3 30
44 3 24
45 3 20
46 3 16
47 3 12
48 3 8
49 3 6
50 3 4
51 3 3
52 3 2
53 4 204
54 4 104
55 4 43
56 4 16
57 4 14
58 4 12
59 4 10
60 4 8
61 4 6
62 4 5
63 4 4
64 4 3
65 4 2
66 5 104
67 5 44
68 5 20
69 5 14
70 5 12
71 5 10
72 5 8
73 5 7
74 5 6
75 5 5
76 5 4
77 5 3
78 5 2
79 6 68
80 6 32
81 6 18
82 6 14
83 6 10
84 6 9
85 6 8
86 6 7
87 6 6
88 6 5
89 6 4
90 6 3
91 6 2
92 7 66
93 7 28
94 7 14
95 7 11
96 7 10
97 7 9
98 7 8
99 7 7
100 7 6
101 7 5
102 7 4
103 7 3
104 7 2
105 8 64
106 8 27
107 8 16
108 8 11
109 8 10
110 8 9
111 8 8
112 8 7
113 8 6
114 8 5
115 8 4
116 8 3
117 8 2
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation (as provided by
   "VdWTransversalsInc 14 1 0 OutputFile OutputSAT") we obtain
   \verbatim
k n tau
14 1 0
14 2 0
14 3 0
14 4 0
14 5 0
14 6 0
14 7 0
14 8 0
14 9 0
14 10 0
14 11 0
14 12 0
14 13 0
14 14 1
14 15 1
14 16 1
14 17 1
14 18 1
14 19 1
14 20 1
14 21 1
14 22 1
14 23 1
14 24 1
14 25 1
14 26 1
14 27 2
14 28 2
14 29 2
14 30 2
14 31 2
14 32 2
14 33 2
14 34 2
14 35 2
14 36 2
14 37 2
14 38 2
14 39 2
14 40 3
14 41 3
14 42 3
14 43 3
14 44 3
14 45 3
14 46 3
14 47 3
14 48 3
14 49 3
14 50 3
14 51 3
14 52 3
14 53 4
14 54 4
14 55 4
14 56 4
14 57 4
14 58 4
14 59 4
14 60 4
14 61 4
14 62 4
14 63 4
14 64 4
14 65 4
14 66 5
14 67 5
14 68 5
14 69 5
14 70 5
14 71 5
14 72 5
14 73 5
14 74 5
14 75 5
14 76 5
14 77 5
14 78 5
14 79 6
14 80 6
14 81 6
14 82 6
14 83 6
14 84 6
14 85 6
14 86 6
14 87 6
14 88 6
14 89 6
14 90 6
14 91 6
14 92 7
14 93 7
14 94 7
14 95 7
14 96 7
14 97 7
14 98 7
14 99 7
14 100 7
14 101 7
14 102 7
14 103 7
14 104 7
14 105 8
14 106 8
14 107 8
14 108 8
14 109 8
14 110 8
14 111 8
14 112 8
14 113 8
14 114 8
14 115 8
14 116 8
14 117 8
14 118 9
14 119 9
14 120 9
14 121 9
14 122 9
14 123 9
14 124 9
14 125 9
14 126 9
14 127 9
14 128 9
14 129 9
14 130 9
14 131 10
14 132 10
14 133 10
14 134 10
14 135 10
14 136 10
14 137 10
14 138 10
14 139 10
14 140 10
14 141 10
14 142 10
14 143 10
14 144 11
14 145 11
14 146 11
14 147 11
14 148 11
14 149 11
14 150 11
14 151 11
14 152 11
14 153 11
14 154 11
14 155 11
14 156 11
14 157 12
14 158 12
14 159 12
14 160 12
14 161 12
14 162 12
14 163 12
14 164 12
14 165 12
14 166 12
14 167 12
14 168 12
14 169 12
14 170 12
14 171 13
14 172 14
14 173 15
14 174 16
14 175 17
14 176 18
14 177 18
   \endverbatim
   </li>
  </ul>

*/
