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
   <li> Using "VdWTransversalsInc 10 1 0 OutputFile" we obtain
   \verbatim
k n tau
   \endverbatim
   </li>
  </ul>

*/
