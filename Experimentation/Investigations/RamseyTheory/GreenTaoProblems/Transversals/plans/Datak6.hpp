// Oliver Kullmann, 5.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/Datak6.hpp
  \brief Investigating the transversal hypergraph of Green-Tao hypergraphs for k = 6 (length of arithmetic progressions)


  \todo Elementary statistics
  <ul>
   <li> Investigating tr_arithprog_hg(6,n). </li>
   <li> The numbers of minimum hyperedges:
   \verbatim
A6p(n):=arithprog_primes_hg(6,n)$
oklib_monitor : true$
Lp200_6 : minimum_transversals_mongen(200,A6p,[{}])$
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
22 0 1
23 0 1
24 0 1
25 0 1
26 0 1
27 0 1
28 0 1
29 0 1
30 0 1
31 0 1
32 0 1
33 0 1
34 0 1
35 0 1
36 0 1
37 1 6
38 1 6
39 1 6
40 1 6
41 1 6
42 1 6
43 1 6
44 1 6
45 1 6
46 1 6
47 1 6
48 1 6
49 1 6
50 1 6
51 1 6
52 1 6
53 1 6
54 1 6
55 2 36
56 2 36
57 2 36
58 2 36
59 2 36
60 2 36
61 2 36
62 2 36
63 2 36
64 3 216
65 3 216
66 3 216
67 3 216
68 3 216
69 3 216
70 3 216
71 4 1296
72 4 1296
73 4 1296
74 4 1296
75 4 1296
76 4 1296
77 4 1296
78 4 1296
79 4 1296
80 4 1296
81 4 1296
82 4 1296
83 4 1296
84 4 1296
85 4 1296
86 4 1296
87 4 1296
88 4 1296
89 4 1296
90 5 7776
91 5 7776
92 5 7776
93 5 7776
94 5 7776
95 5 7776
96 5 7776
97 6 46656
98 6 46656
99 6 46656
100 6 46656
101 6 46656
102 6 46656
103 6 46656
104 6 46656
105 6 46656
106 6 46656
107 6 46656
108 6 46656
109 6 46656
110 6 46656
111 6 46656
112 6 46656
113 6 46656
114 6 46656
115 6 46656
116 6 46656
117 6 46656
118 6 46656
119 6 46656
120 6 46656
121 6 46656
122 6 46656
123 6 46656
124 6 46656
125 7 279936
126 7 279936
127 7 279936
128 7 279936
129 7 279936
130 7 279936
131 7 279936
132 7 279936
133 7 279936
134 7 93312
135 7 93312
136 7 93312
137 7 93312
138 7 93312
139 7 93312
140 7 93312
141 7 93312
142 7 93312
143 7 15552
144 7 15552
145 7 15552
146 7 5184
147 7 5184
148 7 5184
149 7 5184
150 7 5184
151 7 5184
152 8 31104
153 8 31104
154 8 31104
155 8 15552
156 8 15552
157 8 15552
158 8 15552
159 8 15552
160 8 7776
161 8 7776
162 9 46656
163 9 46656
164 9 46656
165 9 46656
166 9 46656
167 9 46656
168 9 46656
169 9 46656
170 9 46656
171 9 46656
172 9 46656
173 9 46656
174 9 46656
175 9 46656
176 9 46656
177 9 46656
178 9 46656
179 10 699840
180 10 699840
181 10 699840
182 10 699840
183 10 699840
184 10 116640
185 10 116640
186 10 116640
187 10 116640
188 10 116640
189 10 116640
190 10 116640
191 10 116640
192 10 116640
193 10 116640
194 10 116640
195 10 116640
196 10 116640
197 10 116640
198 10 116640
199 10 116640
200 10 19440
   \endverbatim
   </li>
   <li> All counts have a rather simple structure when represented using
   base 6 (using int2polyadic(count,6))! One should investigate this. </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_GreenTao-O3-DNDEBUG 400 6 | tee GreenTao_Trans_6_400_OUT

   \endverbatim
   </li>
   <li> For orientation, those n-values where at least one hyperedge has been
   added (compared to n-1):
   \verbatim
for n: 5 thru 300 do block(
 [d:nhyp_arithprog_primes_ohg(6,n)-nhyp_arithprog_primes_ohg(6,n-1)],
  if d>0 then print(n));
37
55
64
71
90
97
125
134
143
146
152
155
160
162
179
184
200
201
204
211
212
214
228
232
242
250
263
269
275
289
292
293
294
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation (as provided by "GTTransversalsInc 6 1 0 OutputFile"):
   \verbatim

   \endverbatim
   </li>
  </ul>

*/

