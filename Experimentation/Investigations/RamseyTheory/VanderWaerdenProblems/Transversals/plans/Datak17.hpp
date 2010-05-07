// Oliver Kullmann, 23.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak17.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 17 (length of arithmetic progressions)


  \todo Elementary statistics for k=17
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A17(n):=arithprog_hg(17,n)$
L170_17 : minimum_transversals_mongen(170,A17,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 300 17 | tee VanderWaerden_Trans_17_300_OUT
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
17 1 17
18 1 16
19 1 15
20 1 14
21 1 13
22 1 12
23 1 11
24 1 10
25 1 9
26 1 8
27 1 7
28 1 6
29 1 5
30 1 4
31 1 3
32 1 2
33 1 1
34 2 81
35 2 72
36 2 64
37 2 56
38 2 49
39 2 42
40 2 36
41 2 30
42 2 25
43 2 20
44 2 16
45 2 12
46 2 9
47 2 6
48 2 4
49 2 2
50 2 1
51 3 69
52 3 60
53 3 51
54 3 42
55 3 35
56 3 28
57 3 24
58 3 20
59 3 16
60 3 12
61 3 9
62 3 6
63 3 5
64 3 4
65 3 3
66 3 2
67 3 1
68 4 37
69 4 32
70 4 27
71 4 22
72 4 17
73 4 12
74 4 11
75 4 10
76 4 9
77 4 8
78 4 7
79 4 6
80 4 5
81 4 4
82 4 3
83 4 2
86 5 16
87 5 15
88 5 14
89 5 13
90 5 12
91 5 11
92 5 10
93 5 9
94 5 8
95 5 7
96 5 6
97 5 5
98 5 4
99 5 3
100 5 2
101 5 1
102 6 17
103 6 16
104 6 15
105 6 14
106 6 13
107 6 12
108 6 11
109 6 10
110 6 9
111 6 8
112 6 7
113 6 6
114 6 5
115 6 4
116 6 3
117 6 2
118 6 1
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> VdWTransversalsInc 17 1 0 VDW_17 VDW_17_SAT

L : transform_steps_l(map(third,rest(read_nested_list("VDW_17"))));
 [16,33,50,67,84,101,118,135,152,169,186,203,220,237,254,271,272,273,274,275,276]
length(L);
 21

> tail -1 VDW_17
17 277 21

> tail VDW_17_SAT
|  25566639 |    1216     7298    55209 |    39070    19204     92 |  5.536 % |
|  38350045 |    1216     7298    55209 |    42977    27412     78 |  5.536 % |
|  57525151 |    1216     7298    55209 |    47275    19996     72 |  5.536 % |
|  86287812 |    1216     7298    55209 |    52003    21986     69 |  5.536 % |
| 129431800 |    1216     7298    55209 |    57203    40690    130 |  5.536 % |
| 194147783 |    1206     7253    55092 |    62923    54329     79 |  6.246 % |
| 291221757 |    1206     7253    55092 |    69216    33259     73 |  6.246 % |
| 436832720 |    1206     7253    55092 |    76137    44570     85 |  6.246 % |
| 655249164 |    1206     7253    55092 |    83751    62770    152 |  6.246 % |
| 982873826 |    1206     7253    55092 |    92126    35290     86 |  6.246 % |
   \endverbatim
   </li>
  </ul>

*/
