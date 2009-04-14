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
   \endverbatim
   </li>
   <li> Using "VdWTransversalsInc 9 1 0 OutputFile" we obtain
   \verbatim
k n tau
   \endverbatim
   </li>
  </ul>

*/
