// Oliver Kullmann, 15.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/UsingPB.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs via PB methods


  \todo Using minisat+
  <ul>
   <li> The first impression, using 
   "VdWTransversalsIncPB 3 1 0 Out Out_solver" instead of 
   "VdWTransversalsInc 3 1 0 Out Out_solver", is that it is actually
   slower! </li>
   <li> Unsatisfiable instances:
   \verbatim
3 0 0
6 4 1
7 8 1
8 17 1
10 22 1
12 37 1
15 38 1
16 90 1
17 139 2
18 172 2
19 275 3
21 308 3
22 672 4
23 822 5
25 1201 5
27 1290 5
28 1605 6
29 2470 7
31 2875 7
33 4042 8
34 4484 8
35 6704 9
37 6381 9
38 10136 10
39 11659 11
42 11736 11
43 16557 11
44 19945 12
45 39287 14
46 46872 14
47 57433 14
48 92158 16
49 116998 16
50 158151 17
52 170787 17
53 226039 18
55 252913 18
56 265573 18
57 382610 19
59 427166 19
60 527362 20
61 678889 21
62 920175 21
64 863104 21
65 11794773 28
66 13054348 28
67 19986008 29
68 34636039 30
69 36363070 30
70 56168407 31
72 41009941 31
73 85001878 32
75 97860025 33
76 149272628 34
77 162966593 34
78 184709931 34
79 225132400 35
80 304577630 36
81 423280421 36
83 539904152 37
   \endverbatim
   </li>
   <li> Satisfiable instances:
   \verbatim
1 0 1
2 0 1
4 0 1
5 0 1
9 0 1
11 0 1
13 16 1
14 20 1
20 46 1
24 81 1
26 77 1
30 2981 7
32 2194 7
36 260 3
40 1665 6
41 13023 11
51 35230 13
54 4777 8
58 296209 19
63 1049682 22
71 33816965 30
74 107863942 33
82 150743227 34
84 132096556 34
   \endverbatim
   </li>
   <li> So performance of minisat+ is rather weak; one should try whether
   tuning the parameters helps, but the first impression is that it doesn't
   help. </li>
   <li> Now using equivalences instead of just implications:
   \verbatim
> VdWTransversalsIncPB 3 1 0 Out3 Out3_minisat+_w "minisat+ -weak-off"
   \endverbatim
   Evaluation via
   \verbatim
awk -f OKsystem/OKlib/Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/ExtractUNSATMinisatPB.awk Out2_minisat+_w
   \endverbatim
   yields
   \verbatim
3 0 0
6 4 1
7 7 1
8 15 1
10 20 1
12 37 1
15 38 1
16 90 1
17 139 2
18 174 2
19 275 3
21 308 3
22 672 4
23 897 5
25 1201 5
27 1359 6
28 1584 6
29 2421 7
31 3117 7
33 4368 8
34 5802 9
35 8549 10
37 6385 9
38 9291 10
39 14104 11
42 10772 10
43 18192 12
44 20013 12
45 30345 13
46 47436 14
47 59985 15
48 78150 15
49 95420 16
50 190255 17
52 136118 17
53 201930 18
55 230344 18
56 271419 18
57 423877 19
59 357975 19
60 591243 20
61 500858 20
62 1088319 22
64 832588 21
65 12023082 28
66 14854503 28
67 19401676 29
68 28797631 30
69 34358111 30
70 53419010 31
72 42447685 31
73 77436334 32
75 89104785 33
   \endverbatim
   while for SAT we get
   \verbatim
1 0 1
2 0 1
4 0 1
5 0 1
9 0 1
11 0 1
13 16 1
14 20 1
20 46 1
24 81 1
26 77 1
30 3233 8
32 1916 6
36 260 3
40 2126 7
41 10598 10
51 110626 16
54 262688 18
58 498968 20
63 706464 21
71 35836409 30
74 69072626 32
   \endverbatim
   It seems that this is a bit better (than the "weak form"). </li>
  </ul>

*/
