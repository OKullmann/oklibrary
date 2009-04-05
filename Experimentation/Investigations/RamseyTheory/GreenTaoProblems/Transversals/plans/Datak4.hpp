// Oliver Kullmann, 5.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/Datak4.hpp
  \brief Investigating the transversal hypergraph of Green-Tao hypergraphs for k = 4 (length of arithmetic progressions)


  \todo Elementary statistics
  <ul>
   <li> Investigating tr_arithprog_hg(4,n). </li>
   <li> The numbers of minimum hyperedges:
   \verbatim
A4p(n):=arithprog_primes_hg(4,n)$
oklib_monitor : true$
Lp60_4 : minimum_transversals_mongen(60,A4p,[{}])$
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
7 0 1
8 0 1
9 1 4
10 1 3
11 1 3
12 1 3
13 1 1
14 2 4
15 2 4
16 2 4
17 3 16
18 3 16
19 3 16
20 3 16
21 3 16
22 4 64
23 4 16
24 4 16
25 4 3
26 5 48
27 5 16
28 5 4
29 5 4
30 5 1
31 5 1
32 6 6
33 6 6
34 6 6
35 7 116
36 8 464
37 9 3248
38 9 1044
39 9 171
40 9 153
41 9 153
42 9 85
43 9 15
44 9 12
45 10 132
46 11 1254
47 11 1254
48 11 33
49 11 12
50 11 12
51 12 87
52 12 62
53 12 62
54 12 52
55 12 1
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation (as provided by "GTTransversalsInc 4 1 0 OutputFile"):
   \verbatim
k n tau
4 1 0
4 2 0
4 3 0
4 4 0
4 5 0
4 6 0
4 7 0
4 8 0
4 9 1
4 10 1
4 11 1
4 12 1
4 13 1
4 14 2
4 15 2
4 16 2
4 17 3
4 18 3
4 19 3
4 20 3
4 21 3
4 22 4
4 23 4
4 24 4
4 25 4
4 26 5
4 27 5
4 28 5
4 29 5
4 30 5
4 31 5
4 32 6
4 33 6
4 34 6
4 35 7
4 36 8
4 37 9
4 38 9
4 39 9
4 40 9
4 41 9
4 42 9
4 43 9
4 44 9
4 45 10
4 46 11
4 47 11
4 48 11
4 49 11
4 50 11
4 51 12
4 52 12
4 53 12
4 54 12
4 55 12
4 56 13
4 57 13
4 58 14
4 59 14
4 60 14
4 61 15
4 62 16
4 63 16
4 64 16
4 65 16
4 66 16
4 67 16
4 68 16
4 69 16
4 70 16
4 71 17
4 72 18
4 73 19
4 74 19
4 75 19
4 76 19
4 77 19
4 78 20
4 79 21
4 80 21
4 81 21
4 82 21
4 83 22
   \endverbatim
   </li>
  </ul>

*/

