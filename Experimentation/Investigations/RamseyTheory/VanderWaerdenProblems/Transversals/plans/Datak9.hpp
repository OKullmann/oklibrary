// Oliver Kullmann, 14.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak9.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 9 (length of arithmetic progressions)


  \todo Elementary statistics for k=9
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A9(n):=arithprog_hg(9,n)$
L70_9 : minimum_transversals_mongen(70,A9,[{}])$
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
7 0 1
8 0 1
9 1 9
10 1 8
11 1 7
12 1 6
13 1 5
14 1 4
15 1 3
16 1 2
17 1 1
18 2 25
19 2 20
20 2 16
21 2 12
22 2 9
23 2 6
24 2 4
25 3 79
26 3 22
27 3 6
28 3 4
29 3 3
30 3 2
31 3 1
32 4 122
33 4 68
34 4 23
35 4 7
36 4 3
37 4 2
38 4 1
39 5 108
40 5 62
41 5 22
42 5 8
43 5 4
44 5 2
45 5 1
46 6 47
47 6 30
48 6 15
49 6 8
50 6 3
51 6 2
52 6 1
53 7 53
54 7 30
55 7 15
56 7 8
57 7 1
58 8 29
59 9 499
60 9 52
61 9 3
   \endverbatim
   </li>
   <li> Using "VdWTransversalsInc 9 1 0 OutputFile" we obtain
   \verbatim
k n tau
9 1 0
9 2 0
9 3 0
9 4 0
9 5 0
9 6 0
9 7 0
9 8 0
9 9 1
9 10 1
9 11 1
9 12 1
9 13 1
9 14 1
9 15 1
9 16 1
9 17 1
9 18 2
9 19 2
9 20 2
9 21 2
9 22 2
9 23 2
9 24 2
9 25 3
9 26 3
9 27 3
9 28 3
9 29 3
9 30 3
9 31 3
9 32 4
9 33 4
9 34 4
9 35 4
9 36 4
9 37 4
9 38 4
9 39 5
9 40 5
9 41 5
9 42 5
9 43 5
9 44 5
9 45 5
9 46 6
9 47 6
9 48 6
9 49 6
9 50 6
9 51 6
9 52 6
9 53 7
9 54 7
9 55 7
9 56 7
9 57 7
9 58 8
9 59 9
9 60 9
9 61 9
9 62 10
9 63 10
9 64 10
9 65 10
9 66 11
9 67 11
9 68 11
9 69 11
9 70 11
9 71 11
9 72 12
9 73 12
9 74 13
9 75 13
9 76 13
9 77 14
9 78 14
9 79 14
9 80 14
9 81 15
9 82 15
9 83 15
9 84 15
9 85 15
9 86 15
9 87 16
9 88 16
9 89 16
9 90 16
9 91 17
9 92 17
9 93 17
9 94 17
9 95 17
9 96 17
9 97 18
9 98 18
9 99 18
9 100 18
9 101 18
9 102 19
9 103 19
9 104 19
9 105 19
9 106 20
9 107 20
9 108 20
9 109 20
9 110 21
9 111 21
9 112 21
9 113 21
9 114 21
9 115 21
   \endverbatim
   </li>
  </ul>

*/
