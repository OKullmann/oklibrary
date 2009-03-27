// Oliver Kullmann, 18.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/UsingSAT.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs via SAT methods


  \todo Translation via addition
  <ul>
   <li> We have already implemented (as an old C++ program) the translation
   of linear inequalities into CNF, and so we can also use (ordinary) SAT
   solvers.
    <ol>
     <li> By "VdWTransversals 3 54 35" we create a Dimacs file, containing the
     hypergraph clauses for n=53 plus the upper bound (here b=35) on the
     transversal size (so that in this case the clause-set is unsatisfiable,
     while for b=36 it is satisfiable). </li>
     <li> And via "VdWTransversalsInc 3 1 0 OutputFile" we compute all the
     transversal number for k=3, starting with n=1. </li>
    </ol>
   </li>
   <li> DONE
   VdWTransversals uses LinInequal-O3-DNDEBUG, which uses only an
   upper bound on the transversal size: It should be more efficient to
   use an (exact) equality; see "Complete LinInequal.cpp" in
   Transformers/Generators/plans/LinInequal.hpp. </li>
  </ul>


  \todo Look-ahead solvers with translation via addition
  <ul>
   <li> It seems that OKsolver_2002 and march_pl need a long time.
   \verbatim
> VdWTransversals 3 40 24

> OKsolver_2002-O3-DNDEBUG vdw_trans_3_40_24.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=4 initial_number_of_variables=169 initial_number_of_clauses=1067 initial_number_of_literal_occurrences=3459 running_time(s)=84.4 number_of_nodes=870214 number_of_single_nodes=3257 number_of_quasi_single_nodes=483 number_of_2-reductions=2279568 number_of_pure_literals=0 number_of_autarkies=4500 number_of_missed_single_nodes=6084 max_tree_depth=43 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=2 reduced_number_of_clauses=6 reduced_number_of_literal_occurrences=17 number_of_1-autarkies=74022 number_of_initial_unit-eliminations=2 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=77 file_name=vdw_trans_3_40_24.cnf

> march_pl vdw_trans_3_40_24.cnf
 main():: nodeCount: 98655
c main():: dead ends in main: 6199
c main():: lookAheadCount: 5794240
c main():: unitResolveCount: 1044234
c main():: time=28.300000
c main():: necessary_assignments: 54362
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 201867, succes #: 120077
c main():: doublelook: overall 3.646 of all possible doublelooks executed
c main():: doublelook: succesrate: 59.483, average DL_trigger: 85.056
s UNSATISFIABLE

> OKsolver_2002-O3-DNDEBUG vdw_trans_3_40_25.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=4 initial_number_of_variables=169 initial_number_of_clauses=1066 initial_number_of_literal_occurrences=3456 running_time(s)=13.5 number_of_nodes=149781 number_of_single_nodes=1054 number_of_quasi_single_nodes=155 number_of_2-reductions=372636 number_of_pure_literals=0 number_of_autarkies=1084 number_of_missed_single_nodes=1602 max_tree_depth=40 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=2 reduced_number_of_clauses=6 reduced_number_of_literal_occurrences=17 number_of_1-autarkies=17208 number_of_initial_unit-eliminations=2number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=77 file_name=vdw_trans_3_40_25.cnf

> march_pl vdw_trans_3_40_25.cnf
c main():: nodeCount: 32051
c main():: dead ends in main: 1503
c main():: lookAheadCount: 1972878
c main():: unitResolveCount: 369319
c main():: time=8.890000
c main():: necessary_assignments: 21652
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 63523, succes #: 37370
c main():: doublelook: overall 3.371 of all possible doublelooks executed
c main():: doublelook: succesrate: 58.829, average DL_trigger: 83.574
c main():: SOLUTION VERIFIED :-)
s SATISFIABLE
   \endverbatim
   </li>
   <li> These used the "<="-translation, while using the "=="-translation
   makes the problem somewhat easier:
   \verbatim
> VdWTransversals 3 40 24

> OKsolver_2002-O3-DNDEBUG vdw_trans_3_40_24.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=4 initial_number_of_variables=169 initial_number_of_clauses=1069 initial_number_of_literal_occurrences=3455 running_time(s)=58.7 number_of_nodes=630223 number_of_single_nodes=3054 number_of_quasi_single_nodes=939 number_of_2-reductions=1611149 number_of_pure_literals=0 number_of_autarkies=2893 number_of_missed_single_nodes=5124 max_tree_depth=38 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=7 reduced_number_of_clauses=29 reduced_number_of_literal_occurrences=110 number_of_1-autarkies=15018 number_of_initial_unit-eliminations=7 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=79 file_name=vdw_trans_3_40_24.cnf

> march_pl vdw_trans_3_40_24.cnf
c main():: nodeCount: 62984
c main():: dead ends in main: 2917
c main():: lookAheadCount: 3604760
c main():: unitResolveCount: 701244
c main():: time=17.710000
c main():: necessary_assignments: 37281
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 128284, succes #: 76098
c main():: doublelook: overall 3.731 of all possible doublelooks executed
c main():: doublelook: succesrate: 59.320, average DL_trigger: 88.877
s UNSATISFIABLE

> VdWTransversals 3 40 25

> OKsolver_2002-O3-DNDEBUG vdw_trans_3_40_25.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=4 initial_number_of_variables=169 initial_number_of_clauses=1069 initial_number_of_literal_occurrences=3455 running_time(s)=7.7 number_of_nodes=90824 number_of_single_nodes=727 number_of_quasi_single_nodes=275 number_of_2-reductions=224556 number_of_pure_literals=0 number_of_autarkies=579 number_of_missed_single_nodes=1044 max_tree_depth=40 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=7 reduced_number_of_clauses=29 reduced_number_of_literal_occurrences=110 number_of_1-autarkies=3763 number_of_initial_unit-eliminations=7 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=79 file_name=vdw_trans_3_40_25.cnf

> march_pl vdw_trans_3_40_25.cnf
c main():: nodeCount: 19596
c main():: dead ends in main: 1051
c main():: lookAheadCount: 1139792
c main():: unitResolveCount: 182775
c main():: time=6.290000
c main():: necessary_assignments: 9716
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 42820, succes #: 26742
c main():: doublelook: overall 3.941 of all possible doublelooks executed
c main():: doublelook: succesrate: 62.452, average DL_trigger: 106.485
c main():: SOLUTION VERIFIED :-)
s SATISFIABLE
   \endverbatim
   </li>
   <li> One should investigate whether the minisat-preprocessor makes
   things easier:
   \verbatim
> minisat2 -dimacs=vdw_trans_3_40_24_m2pp.cnf vdw_trans_3_40_24.cnf

> OKsolver_2002-O3-DNDEBUG vdw_trans_3_40_24_m2pp.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=5 initial_number_of_variables=150 initial_number_of_clauses=992 initial_number_of_literal_occurrences=3159 running_time(s)=28.4 number_of_nodes=278736 number_of_single_nodes=39 number_of_quasi_single_nodes=307 number_of_2-reductions=846245 number_of_pure_literals=0 number_of_autarkies=3786 number_of_missed_single_nodes=208 max_tree_depth=34 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=85839 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=52 file_name=vdw_trans_3_40_24_m2pp.cnf

> march_pl vdw_trans_3_40_24_m2pp.cnf
c main():: nodeCount: 581980
c main():: dead ends in main: 101283
c main():: lookAheadCount: 8918023
c main():: unitResolveCount: 8120863
c main():: time=64.440000
c main():: necessary_assignments: 364624
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 504762, succes #: 147097
c main():: doublelook: overall 6.214 of all possible doublelooks executed
c main():: doublelook: succesrate: 29.142, average DL_trigger: 80.024
s UNSATISFIABLE

> minisat2 -dimacs=vdw_trans_3_40_25_m2pp.cnf vdw_trans_3_40_25.cnf

> OKsolver_2002-O3-DNDEBUG vdw_trans_3_40_25_m2pp.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=149 initial_number_of_clauses=997 initial_number_of_literal_occurrences=3184 running_time(s)=1.9 number_of_nodes=19311 number_of_single_nodes=19 number_of_quasi_single_nodes=18 number_of_2-reductions=64340 number_of_pure_literals=0 number_of_autarkies=239 number_of_missed_single_nodes=43 max_tree_depth=33 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=13171 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=47 file_name=vdw_trans_3_40_25_m2pp.cnf

> march_pl vdw_trans_3_40_25_m2pp.cnf
c main():: nodeCount: 360128
c main():: dead ends in main: 47865
c main():: lookAheadCount: 5723996
c main():: unitResolveCount: 4829333
c main():: time=39.310000
c main():: necessary_assignments: 205107
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 323254, succes #: 100136
c main():: doublelook: overall 6.250 of all possible doublelooks executed
c main():: doublelook: succesrate: 30.977, average DL_trigger: 78.028
c main():: SOLUTION VERIFIED :-)
s SATISFIABLE
   \endverbatim
   </li>
   <li> It seems to help OKsolver_2002 (quite a bit, but likely still much
   worse than minisat), while march_pl seems to perform quite a bit worse on
   the preprocessed problems. </li>
   <li> The same now with the "=="-translation:
   \verbatim
> minisat2 -dimacs=vdw_trans_3_40_24_m2pp.cnf vdw_trans_3_40_24.cnf

> OKsolver_2002-O3-DNDEBUG vdw_trans_3_40_24_m2pp.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=4 initial_number_of_variables=146 initial_number_of_clauses=984 initial_number_of_literal_occurrences=3116 running_time(s)=17.3 number_of_nodes=168264 number_of_single_nodes=105 number_of_quasi_single_nodes=300 number_of_2-reductions=546095 number_of_pure_literals=0 number_of_autarkies=855 number_of_missed_single_nodes=238 max_tree_depth=32 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=62300 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=47 file_name=vdw_trans_3_40_24_m2pp.cnf

> march_pl vdw_trans_3_40_24_m2pp.cnf
c main():: nodeCount: 360770
c main():: dead ends in main: 57575
c main():: lookAheadCount: 5808457
c main():: unitResolveCount: 5197568
c main():: time=45.580000
c main():: necessary_assignments: 239519
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 320914, succes #: 102595
c main():: doublelook: overall 6.106 of all possible doublelooks executed
c main():: doublelook: succesrate: 31.970, average DL_trigger: 86.064
s UNSATISFIABLE

> minisat2 -dimacs=vdw_trans_3_40_25_m2pp.cnf vdw_trans_3_40_25.cnf

> OKsolver_2002-O3-DNDEBUG vdw_trans_3_40_25_m2pp.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=4 initial_number_of_variables=144 initial_number_of_clauses=975 initial_number_of_literal_occurrences=3094 running_time(s)=1.2 number_of_nodes=12782 number_of_single_nodes=17 number_of_quasi_single_nodes=25 number_of_2-reductions=43489 number_of_pure_literals=0 number_of_autarkies=79 number_of_missed_single_nodes=27 max_tree_depth=33 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=8110 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=50 file_name=vdw_trans_3_40_25_m2pp.cnf

> march_pl vdw_trans_3_40_25_m2pp.cnf
c main():: nodeCount: 109063
c main():: dead ends in main: 21515
c main():: lookAheadCount: 1793098
c main():: unitResolveCount: 1494749
c main():: time=13.520000
c main():: necessary_assignments: 59067
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 103076, succes #: 33210
c main():: doublelook: overall 6.341 of all possible doublelooks executed
c main():: doublelook: succesrate: 32.219, average DL_trigger: 82.792
c main():: SOLUTION VERIFIED :-)
s SATISFIABLE
   \endverbatim
   </li>
   <li> Again the same picture: The OKsolver is helped by the translation, the
   march-solver harmed. </li>
   <li> So the solver-combinations to be further investigated are:
    <ol>
     <li> The new generator (using "=="). </li>
     <li> OKsolver together with minisat2-preprocessor (perhaps creating
     a little wrapper script). </li>
     <li> The march-solver as it is. </li>
    </ol>
   </li>
  </ul>


  \todo Conflict-driven solvers with translation via addition
  <ul>
   <li> minisat solves the problems (unsatisfiable as well as
   satisfiable) relatively easily (though with n=80 around 15 minutes are
   needed). </li>
   <li> Perhaps the number of conflicts can be used as the primary indicator
   of difficulty. </li>
   <li> Using "<=" for the bound:
    <ol>
     <li> Extracting the following data from the VdWTransversalsInc-output
     happens via
     \verbatim
> awk -f ExtractUNSATMinisat.awk Outputfile > ExtractedOutput
     \endverbatim
     respectively
     \verbatim
> awk -f ExtractSATMinisat.awk Outputfile > ExtractedOutput
     \endverbatim
     </li>
     <li> Here is data for unsatisfiable instances (n, conflicts,
     restarts; for "VdWTransversalsInc 3 1 0 Out Out_solver"):
     \verbatim
3 4 1
6 11 1
7 25 1
8 43 1
10 86 1
12 99 1
15 97 1
16 181 2
17 233 2
18 545 4
19 557 4
21 636 4
22 1160 5
23 1421 6
25 1479 6
27 1680 6
28 2360 7
29 3942 8
31 4088 8
33 4488 8
34 5502 9
35 7530 10
37 9834 10
38 10005 10
39 13944 11
42 17397 12
43 22704 12
44 25351 12
45 29664 13
46 49335 14
47 69063 15
48 75729 15
49 121943 16
50 145079 17
52 151059 17
53 211669 18
55 111724 16
56 259305 18
57 219192 18
59 404939 19
60 498037 20
61 914499 21
62 1307871 22
64 932740 21
65 1331275 22
66 1301712 22
67 2478009 24
68 4726248 25
69 5403825 26
70 4823856 25
72 5314011 26
73 8068276 27
75 4711514 25
76 7340008 26
77 13886228 28
78 16254105 28
79 12598859 28
80 28392274 30
81 33461573 30
83 37012916 30
85 33630739 30
86 34700752 30
87 57979981 32
88 75782756 32
89 89125366 33
90 114978748 33
91 187332219 34
93 135049502 34
94 200530660 35
96 159284031 34
97 189564517 34
98 262181870 35
99 270869866 35
101 273927337 35
102 463959621 37
     \endverbatim
     </li>
     <li> And here for satisfiable instances:
     \verbatim
1 0 1
2 0 1
4 2 1
5 6 1
9 37 1
11 40 1
13 63 1
14 87 1
20 232 2
24 180 2
26 200 2
30 3996 8
32 987 5
36 6084 9
40 431 3
41 9802 10
51 19665 12
54 137346 17
58 251601 18
63 1626755 23
71 4375702 25
74 6475725 26
82 19016547 29
84 44490254 31
92 46027971 31
95 6012583 26
100 263163048 35
     \endverbatim
     </li>
     <li> The sequence of satisfiable n's is A065825 (see "Relations to
     vanderwaerden_k(n_1, ..., n_k)" in
     Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/general.hpp). </li>
    </ol>
   </li>
   <li> Now the same using "==" for the bound:
    <ol>
     <li> Unsatisfiable instances
     \verbatim
3 4 1
6 11 1
7 22 1
8 31 1
10 58 1
12 76 1
15 111 2
16 202 2
17 268 3
18 470 3
19 876 5
21 988 5
22 1143 5
23 1659 6
25 1607 6
27 2156 7
28 3237 8
29 4040 8
31 3163 7
33 4316 8
34 5998 9
35 8444 10
37 9220 10
38 12725 11
39 12062 11
42 11024 10
43 24603 12
44 22589 12
45 32092 13
46 39311 14
47 79098 15
48 85369 15
49 76239 15
50 146929 17
52 186607 17
53 283582 18
55 179814 17
56 294938 18
57 312389 19
59 299569 19
60 508953 20
61 720857 21
62 809134 21
64 933486 21
65 1700010 23
66 2358123 24
67 2841162 24
68 3467254 25
69 4416351 25
70 7291075 26
72 5383543 26
73 9170506 27
75 6773645 26
76 8172029 27
77 14650318 28
78 15018182 28
79 23301728 29
80 36629003 30
81 38282115 30
83 39718269 31
85 25522920 29
86 38044615 30
87 53885461 31
88 80804713 32
89 100756897 33
90 145686053 34
91 164020742 34
93 137096599 34
94 147255027 34
96 185614681 34
97 185526254 34
     \endverbatim
     </li>
     <li> Satisfiable instances
     \verbatim
1 0 1
2 0 1
4 0 1
5 4 1
9 2 1
11 23 1
13 22 1
14 80 1
20 146 2
24 369 3
26 452 3
30 2045 6
32 331 3
36 7683 10
40 8671 10
41 4261 8
51 48492 14
54 80191 15
58 96254 16
63 40079 14
71 4309528 25
74 5822376 26
82 5732060 26
84 11797126 28
92 3716660 25
95 2902643 24
     \endverbatim
     </li>
     <li> On unsatisfiable instances the performance gets somewhat worse,
     while on satisfiable instances it gets much better --- that should
     indicate that minisat is somehow mislead. Though perhaps with higher
     n the advantages of the ==-translation also show for unsatisfiable
     instances. </li>
    </ol>
   </li>
   <li> Is minisat2 better than minisat?
   \verbatim
minisat2 vdw_trans_3_64_43.cnf
restarts              : 22
conflicts             : 1261598        (21895 /sec)
decisions             : 1423468        (1.35 % random) (24704 /sec)
propagations          : 68517361       (1189120 /sec)
conflict literals     : 17080977       (44.36 % deleted)
Memory used           : 3.25 MB
CPU time              : 57.6202 s
UNSATISFIABLE
> minisat vdw_trans_3_64_43.cnf
restarts              : 21
conflicts             : 932740         (19276 /sec)
decisions             : 1048977        (21679 /sec)
propagations          : 53595491       (1107628 /sec)
conflict literals     : 12450770       (45.50 % deleted)
Memory used           : 4.73 MB
CPU time              : 48.3876 s
UNSATISFIABLE
> minisat2 vdw_trans_3_65_44.cnf
restarts              : 22
conflicts             : 1324767        (21908 /sec)
decisions             : 1484394        (1.39 % random) (24547 /sec)
propagations          : 71723828       (1186090 /sec)
conflict literals     : 17963070       (42.79 % deleted)
Memory used           : 3.39 MB
CPU time              : 60.4708 s
UNSATISFIABLE
> minisat vdw_trans_3_64_43.cnf
restarts              : 22
conflicts             : 1331275        (18706 /sec)
decisions             : 1490338        (20941 /sec)
propagations          : 76617377       (1076568 /sec)
conflict literals     : 17680839       (44.17 % deleted)
Memory used           : 4.97 MB
CPU time              : 71.1682 s
UNSATISFIABLE
> minisat vdw_trans_3_71_50.cnf
restarts              : 25
conflicts             : 4375702        (15250 /sec)
decisions             : 4893653        (17055 /sec)
propagations          : 265492436      (925292 /sec)
conflict literals     : 65592198       (43.40 % deleted)
Memory used           : 6.57 MB
CPU time              : 286.928 s
SATISFIABLE
> minisat2 vdw_trans_3_71_50.cnf
restarts              : 26
conflicts             : 5730961        (17171 /sec)
decisions             : 6427303        (1.39 % random) (19257 /sec)
propagations          : 320252313      (959508 /sec)
conflict literals     : 86355473       (42.23 % deleted)
Memory used           : 4.94 MB
CPU time              : 333.767 s
SATISFIABLE
   \endverbatim
   It seems that actually minisat might be better on these instances
   than minisat2 (using more space, but less conflicts and less time). </li>
   <li> But one needs to consider the details:
    <ol>
     <li> Running it until around n=100, now using the "=="-translation. </li>
     <li> Then the influence of the preprocessor has to be determined, that is,
     minisat2 with "-pre=none", and minisat with the minisat2-preprocessor.
     </li>
     <li> Then there are other parameters one should try out; we need a little
     script for running through these possibilities. </li>
    </ol>
   </li>
  </ul>


  \todo Local search solvers with translation via addition
  <ul>
   <li> The satisfiable instances seem rather hard for local search solvers:
   \verbatim
> ubcsat-okl -alg samd -runs 100 -i vdw_trans_3_40_25.cnf
Clauses = 1066
Variables = 169
TotalLiterals = 3456
FlipsPerSecond = 724638
BestStep_Mean = 1531.870000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
> ubcsat-okl -alg walksat -runs 100 -i vdw_trans_3_40_25.cnf
Clauses = 1066
Variables = 169
TotalLiterals = 3456
FlipsPerSecond = 1727116
BestStep_Mean = 2559.370000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
> ubcsat-okl -alg walksat-tabu -runs 100 -i vdw_trans_3_40_25.cnf
Clauses = 1066
Variables = 169
TotalLiterals = 3456
FlipsPerSecond = 1700680
BestStep_Mean = 6494.100000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
> ubcsat-okl -alg rsaps -runs 100 -i vdw_trans_3_40_25.cnf
Clauses = 1066
Variables = 169
TotalLiterals = 3456
FlipsPerSecond = 1050420
BestStep_Mean = 576.900000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
   \endverbatim
   So perhaps rsaps is best here.
   </li>
   <li> Higher cutoffs:
   \verbatim
> ubcsat-okl -alg rsaps -runs 100 -cutoff 1000000 -i vdw_trans_3_40_25.cnf
Clauses = 1066
Variables = 169
TotalLiterals = 3456
FlipsPerSecond = 1098388
BestStep_Mean = 35359.710000
Steps_Mean = 964824.390000
Steps_Max = 1000000.000000
PercentSuccess = 7.00
BestSolution_Mean = 0.930000
BestSolution_Median = 1.000000
BestSolution_Min = 0.000000
BestSolution_Max = 1.000000
   \endverbatim
   Finding a local-minimum=1 is very easy (a few hundred steps), but finding
   a solution takes around 400000 steps. Similar for bigger instances:
   \verbatim
> ubcsat-okl -alg rsaps -runs 100 -i vdw_trans_3_71_50.cnf
Clauses = 2593
Variables = 342
TotalLiterals = 8172
FlipsPerSecond = 974659
BestStep_Mean = 3914.710000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
> ubcsat-okl -alg rsaps -runs 100 -cutoff 1000000 -i vdw_trans_3_71_50.cnf
Clauses = 2593
Variables = 342
TotalLiterals = 8172
FlipsPerSecond = 908430
BestStep_Mean = 3887.970000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
> ubcsat-okl -alg rsaps -runs 100 -cutoff 10000000 -i vdw_trans_3_71_50.cnf
Clauses = 2593
Variables = 342
TotalLiterals = 8172
FlipsPerSecond = 907663
BestStep_Mean = 3736.860000
Steps_Mean = 10000000.000000
Steps_Max = 10000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
> ubcsat-okl -alg rsaps -runs 100 -cutoff 100000000 -i vdw_trans_3_71_50.cnf
Clauses = 2593
Variables = 342
TotalLiterals = 8172
FlipsPerSecond = 916484
BestStep_Mean = 4043.990000
Steps_Mean = 100000000.000000
Steps_Max = 100000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
   \endverbatim
   One sees that the (very few) solutions are very hard to find for a local
   search algorithm. </li>
   <li> UnitMarch seems not to perform:
   \verbatim
> UnitMarch_32_bits vdw_trans_3_40_25.cnf
   \endverbatim
   seems unsuccesful (unfortunately, there are no statistics available, and
   also the seed can't be changed).
   </li>
   <li> However, all the above is for "<="-translation, and furthermore there
   are unit-clauses in the input --- we need to apply such simple preprocessing
   first! </li>
  </ul>


  \todo Translation to pseudo-boolean problems
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
   <li> But the feature to write the CNF to a file, and then to use
   some other solvers, needs to be explored! </li>
  </ul>


  \todo Using CSP solvers

*/
