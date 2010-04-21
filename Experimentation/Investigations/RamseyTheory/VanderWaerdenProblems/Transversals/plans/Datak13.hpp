// Oliver Kullmann, 21.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak13.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 13 (length of arithmetic progressions)


  \todo Elementary statistics for k=13
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A13(n):=arithprog_hg(13,n)$
L130_13 : minimum_transversals_mongen(130,A13,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 200 13 | tee VanderWaerden_Trans_13_200_OUT
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
13 1 13
14 1 12
15 1 11
16 1 10
17 1 9
18 1 8
19 1 7
20 1 6
21 1 5
22 1 4
23 1 3
24 1 2
25 1 1
26 2 49
27 2 42
28 2 36
29 2 30
30 2 25
31 2 20
32 2 16
33 2 12
34 2 9
35 2 6
36 2 4
37 2 2
38 2 1
39 3 45
40 3 36
41 3 30
42 3 24
43 3 20
44 3 16
45 3 12
46 3 8
47 3 6
48 3 4
49 3 3
50 3 2
51 3 1
52 4 23
53 4 18
54 4 16
55 4 14
56 4 12
57 4 10
58 4 8
59 4 6
60 4 5
61 4 4
62 4 3
63 4 2
64 4 1
65 5 18
66 5 16
67 5 14
68 5 12
69 5 10
70 5 8
71 5 7
72 5 6
73 5 5
74 5 4
75 5 3
76 5 2
77 5 1
78 6 16
79 6 14
80 6 12
81 6 10
82 6 9
83 6 8
84 6 7
85 6 6
86 6 5
87 6 4
88 6 3
89 6 2
90 6 1
91 7 14
92 7 12
93 7 11
94 7 10
95 7 9
96 7 8
97 7 7
98 7 6
99 7 5
100 7 4
101 7 3
102 7 2
103 7 1
104 8 13
105 8 12
106 8 11
107 8 10
108 8 9
109 8 8
110 8 7
111 8 6
112 8 5
113 8 4
114 8 3
115 8 2
116 8 1
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation (as provided by
   "VdWTransversalsInc 13 1 0 OutputFile OutputSAT") we obtain
   \verbatim
k n tau
13 1 0
13 2 0
13 3 0
13 4 0
13 5 0
13 6 0
13 7 0
13 8 0
13 9 0
13 10 0
13 11 0
13 12 0
13 13 1
13 14 1
13 15 1
13 16 1
13 17 1
13 18 1
13 19 1
13 20 1
13 21 1
13 22 1
13 23 1
13 24 1
13 25 1
13 26 2
13 27 2
13 28 2
13 29 2
13 30 2
13 31 2
13 32 2
13 33 2
13 34 2
13 35 2
13 36 2
13 37 2
13 38 2
13 39 3
13 40 3
13 41 3
13 42 3
13 43 3
13 44 3
13 45 3
13 46 3
13 47 3
13 48 3
13 49 3
13 50 3
13 51 3
13 52 4
13 53 4
13 54 4
13 55 4
13 56 4
13 57 4
13 58 4
13 59 4
13 60 4
13 61 4
13 62 4
13 63 4
13 64 4
13 65 5
13 66 5
13 67 5
13 68 5
13 69 5
13 70 5
13 71 5
13 72 5
13 73 5
13 74 5
13 75 5
13 76 5
13 77 5
13 78 6
13 79 6
13 80 6
13 81 6
13 82 6
13 83 6
13 84 6
13 85 6
13 86 6
13 87 6
13 88 6
13 89 6
13 90 6
13 91 7
13 92 7
13 93 7
13 94 7
13 95 7
13 96 7
13 97 7
13 98 7
13 99 7
13 100 7
13 101 7
13 102 7
13 103 7
13 104 8
13 105 8
13 106 8
13 107 8
13 108 8
13 109 8
13 110 8
13 111 8
13 112 8
13 113 8
13 114 8
13 115 8
13 116 8
13 117 9
13 118 9
13 119 9
13 120 9
13 121 9
13 122 9
13 123 9
13 124 9
13 125 9
13 126 9
13 127 9
13 128 9
13 129 9
13 130 10
13 131 10
13 132 10
13 133 10
13 134 10
13 135 10
13 136 10
13 137 10
13 138 10
13 139 10
13 140 10
13 141 10
13 142 10
13 143 11
13 144 11
13 145 11
13 146 11
13 147 11
13 148 11
13 149 11
13 150 11
13 151 11
13 152 11
13 153 11
13 154 11
13 155 11
13 156 12
13 157 13
13 158 14
13 159 15
13 160 16
13 161 16
13 162 17
   \endverbatim
   </li>
  </ul>

*/
