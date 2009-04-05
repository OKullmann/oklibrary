// Oliver Kullmann, 5.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/UsingSAT.hpp
  \brief Investigating the transversal hypergraph of Green-Tao hypergraphs via SAT methods


  \todo General plans
  <ul>
   <li> We repeat the experiments from
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/UsingSAT.hpp
   with the aim of finding out whether something fundamental changes here.
   </li>
  </ul>


  \todo Performance of Minisat
  <ul>
   <li> "GTTransversalsInc 3 1 0 VAusGT3 VAusGT3_minisat2" yields on
   unsatisfiable instances (evaluation via
   "awk -f OKsystem/OKlib/Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/ExtractUNSATMinisat.awk VAusGT3_minisat2")
   \verbatim
4 0 0
7 0 0
9 10 1
13 36 1
14 83 1
16 91 1
18 181 2
21 258 3
22 586 4
23 679 4
28 993 5
29 1131 5
30 2097 7
32 4470 8
33 6019 9
36 7556 10
38 7781 10
39 10478 10
40 19661 12
42 28607 13
43 64165 15
47 30989 13
48 55408 14
49 88744 16
50 190662 17
52 265896 18
55 219050 18
56 264091 18
57 449269 20
59 938668 21
61 689152 21
62 1552229 23
65 963747 21
68 1904411 23
69 3153530 24
70 5273712 26
71 6252818 26
72 13854677 28
73 23942664 29
75 26210132 30
76 43790383 31
78 53865372 31
80 74487211 32
81 102485094 33
   \endverbatim
   and on satisfiable instances
   \verbatim
1 0 1
2 0 1
3 0 1
5 0 1
6 0 1
8 0 1
10 3 1
11 18 1
12 16 1
15 18 1
17 31 1
19 24 1
20 213 2
24 180 2
25 551 4
26 467 3
27 1146 5
31 4290 8
34 2381 7
35 899 5
37 7328 9
41 299 3
44 3561 8
45 29450 13
46 36301 13
51 80109 15
53 24810 12
54 17353 12
58 61912 15
60 155993 17
63 3892 8
64 91530 16
66 208579 18
67 1127101 22
74 3316787 24
77 3227446 24
79 19215552 29
   \endverbatim
   </li>
   <li> "GTTransversalsInc 4 1 0 VAusGT4 VAusGT4_minisat2" yields on
   unsatisfiable instances
   \verbatim
9 0 0
14 0 0
17 0 0
22 17 1
26 164 2
32 399 3
35 771 4
36 1928 6
37 4544 8
45 3155 7
46 9589 10
51 14092 11
56 21636 12
58 29684 13
61 54952 14
62 128635 16
71 123335 16
72 568287 20
73 909720 21
78 675100 21
79 2768664 24
83 3005219 24
   \endverbatim
   and on satisfiable instances
   \verbatim
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
7 0 1
8 0 1
10 0 1
11 0 1
12 0 1
13 0 1
15 2 1
16 10 1
18 0 1
19 0 1
20 3 1
21 0 1
23 40 1
24 76 1
25 116 2
27 178 2
28 121 2
29 75 1
30 149 2
31 321 3
33 212 2
34 88 1
38 91 1
39 1046 5
40 532 4
41 305 3
42 1429 6
43 3288 8
44 1679 6
47 884 5
48 5657 9
49 9852 10
50 5691 9
52 25360 12
53 4213 8
54 1956 6
55 7259 9
57 1986 6
59 24630 12
60 9352 10
63 9142 10
64 8228 10
65 33130 13
66 88391 16
67 47163 14
68 6244 9
69 2438 7
70 16834 11
74 304924 19
75 244464 18
76 102806 16
77 151229 17
80 336188 19
81 6411227 26
82 2877313 24
   \endverbatim
   </li>
   <li> "GTTransversalsInc 5 1 0 VAusGT5 VAusGT5_minisat2" yields on
   unsatisfiable instances
   \verbatim
10 0 0
31 0 0
32 31 1
49 82 1
58 370 3
61 740 4
62 3640 8
78 3675 8
87 11609 11
98 7227 9
107 29454 13
112 135395 17
121 188692 17
123 185172 17
   \endverbatim
   and on satisfiable instances
   \verbatim
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
7 0 1
8 0 1
9 0 1
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
33 0 1
34 0 1
35 0 1
36 2 1
37 0 1
38 6 1
39 6 1
40 37 1
41 27 1
42 25 1
43 22 1
44 13 1
45 38 1
46 39 1
47 53 1
48 66 1
50 39 1
51 61 1
52 99 1
53 49 1
54 236 2
55 101 2
56 182 2
57 155 2
59 165 2
60 72 1
63 94 1
64 168 2
65 330 3
66 345 3
67 315 3
68 171 2
69 668 4
70 303 3
71 2429 7
72 170 2
73 206 2
74 143 2
75 5918 9
76 679 4
77 1096 5
79 684 4
80 3196 7
81 1797 6
82 877 5
83 1949 6
84 584 4
85 447 3
86 7180 9
88 13530 11
89 17264 12
90 10229 10
91 6089 9
92 2016 6
93 1210 5
94 761 4
95 5209 9
96 18607 12
97 18063 12
99 33781 13
100 6186 9
101 5447 9
102 6536 9
103 4395 8
104 8731 10
105 11652 11
106 11909 11
108 1122 5
109 47426 14
110 12026 11
111 16098 11
113 146333 17
114 169486 17
115 86303 15
116 25518 12
117 127695 16
118 716680 21
119 219329 18
120 11098 10
122 301303 19
124 80541 15
125 457306 20
126 154044 17
127 73660 15
128 176620 17
129 484215 20
130 222400 18
131 63005 15
132 411120 19
133 58014 14
134 77777 15
   \endverbatim
   </li>
  </ul>


  \todo Performance of OKsolver-2002
  <ul>
   <li> "GTTransversalsInc 3 1 0 VAusGT3 VAusGT3_oksolver OKsolver_2002-O3-DNDEBUG" yields on unsatisfiable instances (using
   "awk -f OKsystem/OKlib/Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/ExtractUNSATOKsolver.awk VAusGT3_oksolver")
   \verbatim
4 1
7 3
9 13
13 43
14 85
16 153
18 395
21 1422
22 2177
23 3469
28 12061
29 19225
30 22949
32 40482
33 120996
36 308354
38 685691
39 1161185
40 2086162
   \endverbatim
   and on satisfiable instances
   \verbatim
1 0
2 1
3 2
5 2
6 2
8 4
10 25
11 24
12 8
15 101
17 160
19 44
20 18
24 777
25 1204
26 948
27 625
31 29383
34 21980
35 26754
37 446974
41 1150499
   \endverbatim
   </li>
   <li> And with OKsolver_2002-m2pp on unsatisfiable instances (using
   "awk -f OKsystem/OKlib/Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/ExtractUNSATOKsolverm2pp.awk VAusGT3_oksolverm2pp")
   \verbatim
4 0
7 0
9 7
13 5
14 37
16 89
18 116
21 394
22 634
23 759
28 1619
29 6039
30 10787
32 35072
33 46479
36 57469
38 252570
39 317273
40 447429
42 548658
43 1019685
47 921177
48 1601393
49 2172900
50 4367024
52 7115447
   \endverbatim
   and on satisfiable instances
   \verbatim
1 0
2 0
3 0
5 0
6 0
8 5
10 4
11 6
12 6
15 32
17 44
19 8
20 115
24 67
25 173
26 15
27 185
31 3567
34 788
35 12792
37 111145
41 124928
44 21026
45 541888
46 217352
51 300980
53 1083819
54 1277176
   \endverbatim
   </li>
  </ul>

*/

