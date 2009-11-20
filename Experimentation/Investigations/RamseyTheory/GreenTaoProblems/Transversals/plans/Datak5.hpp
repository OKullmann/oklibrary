// Oliver Kullmann, 5.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/Datak5.hpp
  \brief Investigating the transversal hypergraph of Green-Tao hypergraphs for k = 5 (length of arithmetic progressions)


  \todo Elementary statistics
  <ul>
   <li> Investigating tr_arithprog_hg(5,n). </li>
   <li> The numbers of minimum hyperedges:
   \verbatim
A5p(n):=arithprog_primes_hg(5,n)$
oklib_monitor : true$
Lp120_5 : minimum_transversals_mongen(120,A5p,[{}])$
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
7 0 1
8 0 1
9 0 1
10 1 5
11 1 5
12 1 5
13 1 5
14 1 5
15 1 5
16 1 3
17 1 3
18 1 3
19 1 3
20 1 3
21 1 3
22 1 3
23 1 3
24 1 3
25 1 3
26 1 3
27 1 3
28 1 3
29 1 3
30 1 3
31 2 15
32 3 90
33 3 90
34 3 90
35 3 90
36 3 90
37 3 72
38 3 72
39 3 72
40 3 28
41 3 28
42 3 28
43 3 28
44 3 28
45 3 20
46 3 20
47 3 20
48 3 20
49 4 108
50 4 108
51 4 108
52 4 108
53 4 108
54 4 68
55 4 56
56 4 56
57 4 56
58 5 280
59 5 280
60 5 280
61 6 1400
62 7 10100
63 7 10100
64 7 6600
65 7 6600
66 7 6600
67 7 6600
68 7 6600
69 7 6600
70 7 6600
71 7 4200
72 7 4200
73 7 4200
74 7 1680
75 7 1680
76 7 1680
77 7 1680
78 8 8400
79 8 8400
80 8 8400
81 8 8400
82 8 3360
83 8 3360
84 8 3360
85 8 3360
86 8 960
87 9 4800
88 9 4800
89 9 4800
90 9 4800
91 9 4800
92 9 1920
93 9 1920
94 9 1920
95 9 1920
96 9 1920
97 9 80
98 10 400
99 10 400
100 10 400
101 10 400
102 10 400
103 10 400
104 10 400
105 10 400
106 10 400
107 11 12000
108 11 12000
109 11 12000
110 11 12000
111 11 2400
112 12 14400
113 12 480
114 12 480
115 12 480
116 12 480
117 12 480
118 12 480
119 12 480
120 12 480
121 13 
   \endverbatim
   </li>
   <li> Very peculiar, these numbers.
    <ol>
     <li> The segments of constant numbers are mostly explained by the fact
     that no new arithmetic progression gets added, except of the transitions
     76 -> 77 and 89 -> 90, where in both cases one new arithmetic progression
     is added, but it has no effect (at least at this time). </li>
    </ol>
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_GreenTao-O3-DNDEBUG 180 5 | tee GreenTao_Trans_5_180_OUT
121 13 2400
122 13 200
123 14 15000
124 14 15000
125 14 12000
126 14 12000
127 14 12000
128 14 2560
129 14 2560
130 14 2560
131 14 2560
132 14 2560
133 14 2560
134 14 2560
135 14 1024
136 14 1024
137 14 1024
138 14 1024
139 14 1024
140 14 1024
141 14 512
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation (as provided by "GTTransversalsInc 5 1 0 OutputFile"):
   \verbatim
k n tau
5 1 0
5 2 0
5 3 0
5 4 0
5 5 0
5 6 0
5 7 0
5 8 0
5 9 0
5 10 1
5 11 1
5 12 1
5 13 1
5 14 1
5 15 1
5 16 1
5 17 1
5 18 1
5 19 1
5 20 1
5 21 1
5 22 1
5 23 1
5 24 1
5 25 1
5 26 1
5 27 1
5 28 1
5 29 1
5 30 1
5 31 2
5 32 3
5 33 3
5 34 3
5 35 3
5 36 3
5 37 3
5 38 3
5 39 3
5 40 3
5 41 3
5 42 3
5 43 3
5 44 3
5 45 3
5 46 3
5 47 3
5 48 3
5 49 4
5 50 4
5 51 4
5 52 4
5 53 4
5 54 4
5 55 4
5 56 4
5 57 4
5 58 5
5 59 5
5 60 5
5 61 6
5 62 7
5 63 7
5 64 7
5 65 7
5 66 7
5 67 7
5 68 7
5 69 7
5 70 7
5 71 7
5 72 7
5 73 7
5 74 7
5 75 7
5 76 7
5 77 7
5 78 8
5 79 8
5 80 8
5 81 8
5 82 8
5 83 8
5 84 8
5 85 8
5 86 8
5 87 9
5 88 9
5 89 9
5 90 9
5 91 9
5 92 9
5 93 9
5 94 9
5 95 9
5 96 9
5 97 9
5 98 10
5 99 10
5 100 10
5 101 10
5 102 10
5 103 10
5 104 10
5 105 10
5 106 10
5 107 11
5 108 11
5 109 11
5 110 11
5 111 11
5 112 12
5 113 12
5 114 12
5 115 12
5 116 12
5 117 12
5 118 12
5 119 12
5 120 12
5 121 13
5 122 13
5 123 14
5 124 14
5 125 14
5 126 14
5 127 14
5 128 14
5 129 14
5 130 14
5 131 14
5 132 14
5 133 14
5 134 14
   \endverbatim
   </li>
  </ul>

*/

