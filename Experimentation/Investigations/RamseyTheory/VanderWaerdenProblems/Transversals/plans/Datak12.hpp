// Oliver Kullmann, 19.4.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak12.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 12 (length of arithmetic progressions)


  \todo Elementary statistics for k=12
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A12(n):=arithprog_hg(12,n)$
L110_12 : minimum_transversals_mongen(110,A12,[{}])$
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
12 1 12
13 1 11
14 1 10
15 1 9
16 1 8
17 1 7
18 1 6
19 1 5
20 1 4
21 1 3
22 1 2
23 2 63
24 2 36
25 2 30
26 2 25
27 2 20
28 2 16
29 2 12
30 2 9
31 2 6
32 2 4
33 2 2
34 3 120
35 3 48
36 3 20
37 3 16
38 3 12
39 3 9
40 3 6
41 3 5
42 3 4
43 3 3
44 3 2
45 4 91
46 4 31
47 4 18
48 4 9
49 4 8
50 4 7
51 4 6
52 4 5
53 4 4
54 4 3
55 4 2
56 5 74
57 5 34
58 5 16
59 5 11
60 5 8
61 5 7
62 5 6
63 5 5
64 5 4
65 5 3
66 5 2
67 6 54
68 6 23
69 6 14
70 6 11
71 6 8
72 6 7
73 6 6
74 6 5
75 6 4
76 6 3
77 6 2
78 7 54
79 7 23
80 7 12
81 7 9
82 7 8
83 7 7
84 7 6
85 7 5
86 7 4
87 7 3
88 7 2
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 200 12 | tee VanderWaerden_Trans_12_200_OUT
89 8 54
90 8 23
91 8 14
92 8 9
93 8 8
94 8 7
95 8 6
96 8 5
97 8 4
98 8 3
99 8 2
100 9 54
101 9 23
102 9 12
103 9 9
104 9 8
105 9 7
106 9 6
107 9 5
108 9 4
109 9 3
110 9 2
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation (as provided by
   "VdWTransversalsInc 12 1 0 OutputFile OutputSAT") we obtain
   \verbatim
k n tau
12 1 0
12 2 0
12 3 0
12 4 0
12 5 0
12 6 0
12 7 0
12 8 0
12 9 0
12 10 0
12 11 0
12 12 1
12 13 1
12 14 1
12 15 1
12 16 1
12 17 1
12 18 1
12 19 1
12 20 1
12 21 1
12 22 1
12 23 2
12 24 2
12 25 2
12 26 2
12 27 2
12 28 2
12 29 2
12 30 2
12 31 2
12 32 2
12 33 2
12 34 3
12 35 3
12 36 3
12 37 3
12 38 3
12 39 3
12 40 3
12 41 3
12 42 3
12 43 3
12 44 3
12 45 4
12 46 4
12 47 4
12 48 4
12 49 4
12 50 4
12 51 4
12 52 4
12 53 4
12 54 4
12 55 4
12 56 5
12 57 5
12 58 5
12 59 5
12 60 5
12 61 5
12 62 5
12 63 5
12 64 5
12 65 5
12 66 5
12 67 6
12 68 6
12 69 6
12 70 6
12 71 6
12 72 6
12 73 6
12 74 6
12 75 6
12 76 6
12 77 6
12 78 7
12 79 7
12 80 7
12 81 7
12 82 7
12 83 7
12 84 7
12 85 7
12 86 7
12 87 7
12 88 7
12 89 8
12 90 8
12 91 8
12 92 8
12 93 8
12 94 8
12 95 8
12 96 8
12 97 8
12 98 8
12 99 8
12 100 9
12 101 9
12 102 9
12 103 9
12 104 9
12 105 9
12 106 9
12 107 9
12 108 9
12 109 9
12 110 9
12 111 10
12 112 10
12 113 10
12 114 10
12 115 10
12 116 10
12 117 10
12 118 10
12 119 10
12 120 10
12 121 10
12 122 10
12 123 11
12 124 12
12 125 13
12 126 14
12 127 15
12 128 15
12 129 16
12 130 17
12 131 17
12 132 17
   \endverbatim
   </li>
  </ul>

*/
