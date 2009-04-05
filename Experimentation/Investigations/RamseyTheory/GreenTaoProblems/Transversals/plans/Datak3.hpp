// Oliver Kullmann, 5.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/Datak3.hpp
  \brief Investigating the transversal hypergraph of Green-Tao hypergraphs for k = 3 (length of arithmetic progressions)


  \todo Elementary statistics
  <ul>
   <li> Investigating tr_arithprog_hg(k,n). </li>
   <li> The numbers of minimum hyperedges:
   \verbatim
A3p(n):=arithprog_primes_hg(3,n)$
oklib_monitor : true$
Lp60_3 : minimum_transversals_mongen(60,A3p,[{}])$
1 0 1
2 0 1
3 0 1
4 1 3
5 1 2
6 1 2
7 2 7
8 2 1
9 3 9
10 3 3
11 3 2
12 3 2
13 4 8
14 5 24
15 5 3
16 6 20
17 6 3
18 7 51
19 7 18
20 7 6
21 8 18
22 9 84
23 10 356
24 10 29
25 10 8
26 10 4
27 10 2
28 11 19
29 12 19
30 13 32
31 13 2
32 14 3
33 15 160
34 15 44
35 15 11
36 16 23
37 16 2
38 17 20
39 18 40
40 19 189
41 19 55
42 20 687
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation (as provided by "GTTransversalsInc 3 1 0 OutputFile"):
   \verbatim
k n tau
3 1 0
3 2 0
3 3 0
3 4 1
3 5 1
3 6 1
3 7 2
3 8 2
3 9 3
3 10 3
3 11 3
3 12 3
3 13 4
3 14 5
3 15 5
3 16 6
3 17 6
3 18 7
3 19 7
3 20 7
3 21 8
3 22 9
3 23 10
3 24 10
3 25 10
3 26 10
3 27 10
3 28 11
3 29 12
3 30 13
3 31 13
3 32 14
3 33 15
3 34 15
3 35 15
3 36 16
3 37 16
3 38 17
3 39 18
3 40 19
3 41 19
3 42 20
3 43 21
3 44 21
3 45 21
3 46 21
3 47 22
3 48 23
3 49 24
3 50 25
3 51 25
3 52 26
3 53 26
3 54 26
3 55 27
3 56 28
3 57 29
3 58 29
3 59 30
3 60 30
3 61 31
3 62 32
3 63 32
3 64 32
3 65 33
3 66 33
3 67 33
3 68 34
3 69 35
3 70 36
3 71 37
3 72 38
3 73 39
3 74 39
3 75 40
3 76 41
3 77 41
3 78 42
3 79 42
3 80 43
3 81 44
   \endverbatim
   </li>
  </ul>

*/

