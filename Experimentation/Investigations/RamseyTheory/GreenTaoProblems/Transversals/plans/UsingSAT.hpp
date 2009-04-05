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
  </ul>

*/

