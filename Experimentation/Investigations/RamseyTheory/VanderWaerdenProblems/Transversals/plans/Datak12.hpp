// Oliver Kullmann, 19.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
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
   <li> Using "VdWTransversalsInc 12 1 0 OutputFile" we obtain
   \verbatim
   \endverbatim
   </li>
  </ul>

*/
