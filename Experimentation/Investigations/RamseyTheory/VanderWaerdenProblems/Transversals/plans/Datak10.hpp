// Oliver Kullmann, 15.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak10.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 10 (length of arithmetic progressions)


  \todo Elementary statistics for k=10
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A10(n):=arithprog_hg(10,n)$
L70_10 : minimum_transversals_mongen(70,A10,[{}])$
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
7 0 1
8 0 1
9 0 1
10 1 10
11 1 9
12 1 8
13 1 7
14 1 6
15 1 5
16 1 4
17 1 3
18 1 2
19 2 45
20 2 25
21 2 20
22 2 16
23 2 12
24 2 9
25 2 6
26 2 4
27 2 2
28 2 1
29 3 38
30 3 4
31 3 3
32 3 2
33 3 1
34 4 300
35 4 200
36 4 123
37 4 52
38 4 25
39 4 7
40 4 1
41 5 318
42 5 214
43 5 128
44 5 68
45 5 38
46 5 4
47 5 1
48 6 165
49 6 60
50 6 31
51 6 14
52 6 9
53 6 4
54 6 1
55 7 279
56 7 116
57 7 35
58 7 14
59 7 9
60 7 4
61 7 1
   \endverbatim
   </li>
   <li> Extended:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 100 10
62 8 253
63 8 130
64 8 11
65 9 1427
66 9 304
67 9 30
68 9 4
69 10 581
70 10 32
71 10 17
72 10 6
73 10 2
   \endverbatim
   </li>
   <li> Using "VdWTransversalsInc 10 1 0 OutputFile" we obtain
   \verbatim
k n tau
10 1 0
10 2 0
10 3 0
10 4 0
10 5 0
10 6 0
10 7 0
10 8 0
10 9 0
10 10 1
10 11 1
10 12 1
10 13 1
10 14 1
10 15 1
10 16 1
10 17 1
10 18 1
10 19 2
10 20 2
10 21 2
10 22 2
10 23 2
10 24 2
10 25 2
10 26 2
10 27 2
10 28 2
10 29 3
10 30 3
10 31 3
10 32 3
10 33 3
10 34 4
10 35 4
10 36 4
10 37 4
10 38 4
10 39 4
10 40 4
10 41 5
10 42 5
10 43 5
10 44 5
10 45 5
10 46 5
10 47 5
10 48 6
10 49 6
10 50 6
10 51 6
10 52 6
10 53 6
10 54 6
10 55 7
10 56 7
10 57 7
10 58 7
10 59 7
10 60 7
10 61 7
10 62 8
10 63 8
10 64 8
10 65 9
10 66 9
10 67 9
10 68 9
10 69 10
10 70 10
10 71 10
10 72 10
10 73 10
10 74 11
10 75 11
10 76 11
10 77 11
10 78 11
10 79 12
10 80 12
10 81 12
10 82 12
10 83 12
10 84 12
10 85 13
10 86 13
10 87 13
10 88 13
10 89 14
10 90 14
10 91 14
10 92 15
10 93 15
10 94 15
10 95 15
10 96 16
10 97 16
10 98 16
10 99 16
10 100 16
10 101 17
10 102 17
10 103 17
10 104 17
10 105 17
10 106 18
10 107 18
10 108 18
10 109 18
10 110 19
10 111 19
10 112 19
10 113 19
10 114 19
10 115 19
   \endverbatim
   </li>
  </ul>

*/
