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
L5 : [];
minimum_transversals_decomp_gen(inf,lambda([n],arithprog_primes_hg(5,n)), 'L5)$

1 1 0 [0,1,1]
10 10 1 [1,5,6]
16 16 2 [1,3,10]
31 31 3 [2,15,21]
32 32 4 [3,90,19]
37 37 5 [3,72,23]
40 40 6 [3,28,23]
45 45 7 [3,20,26]
49 49 8 [4,108,26]
54 54 9 [4,68,29]
55 55 10 [4,56,29]
58 58 11 [5,280,28]
61 61 12 [6,1400,27]
62 62 13 [7,10100,25]
64 64 14 [7,6600,26]
71 71 15 [7,4200,32]
74 74 16 [7,1680,32]
77 77 17 [7,1680,34]
78 78 18 [8,8400,31]
82 82 19 [8,3360,32]
86 86 20 [8,960,33]
87 87 21 [9,4800,30]
90 90 22 [9,4800,32]
92 92 23 [9,1920,32]
97 97 26 [9,80,35]
98 98 27 [10,400,32]
107 107 28 [11,12000,38]
111 111 29 [11,2400,39]
112 112 30 [12,14400,38]
113 113 31 [12,480,37]
121 121 32 [13,2400,41]
122 122 33 [13,200,39]
123 123 34 [14,15000,37]
125 125 35 [14,12000,38]
128 128 36 [14,2560,38]
134 134 37 [14,2560,43]
135 135 38 [14,1024,41]
141 141 39 [14,512,45]
142 142 40 [15,7104,44]
143 143 42 [16,333888,43]
146 146 43 [16,330880,45]
147 147 44 [16,140800,44]
149 149 45 [17,704000,42]
150 150 47 [18,704000,37]
152 152 49 [18,204800,37]
154 154 50 [19,4211200,38]
155 155 51 [19,4211200,38]

T : transform_steps_l(map(lambda([d],d[4][1]),reverse(L5)));
 [9,30,31,48,57,60,61,77,86,97,106,111,120,122,141,142,148,149,153]
length(T);
 19
   \endverbatim
   </li>
   <li> So here we are definitely better than what minisat2 can compute (see
   below --- which of course uses a very simple problem representation). </li>
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

