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
   <li> We need to explore the different algorithms in minisat+ for
   translation from pseudo-boolean to CNF problems.
    <ol>
     <li> 4 basic translations are offered there ("-adders", "-sorters",
     "-bdds" and "-mixed"). </li>
     <li> Apparently in each case we have the possibility "-weak-off" to
     "Clausify with equivalences instead of implications" ? </li>
     <li> One question to be explored is why minisat+ performs badly:
     Something wrong with the translation or with the underlying SAT
     solver? </li>
     <li> For each of the translations we also need to re-examine other
     types of solvers. </li>
     <li> We should perform a systematic investigation.
      <ol>
       <li> Running all translation for a selection of solvers, in each
       case for a few days. </li>
       <li> Best to put these experiments on different machines. </li>
       <li> For automatisation perhaps for now it suffices just to have
       a script which each hour checks whether a certain process on a
       certain machine still runs, and if not restarts it (just using
       a given command). </li>
      </ol>
     </li>
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
   <li> probing yields: The OKsolver_2002 is helped by the
   minisat2-preprocessing, the march-solver harmed. </li>
   <li> So the solver-combinations to be investigated are:
    <ol>
     <li> OKsolver together with minisat2-preprocessor. </li>
     <li> The march-solver as it is. </li>
    </ol>
   </li>
   <li> OKsolver_2002 for k=3 on unsatisfiable instances:
   \verbatim
3 3
6 3
7 11
8 15
10 49
12 81
15 139
16 175
17 320
18 544
19 1225
21 1691
22 2908
23 4169
25 8132
27 9436
28 16129
29 23172
31 35036
33 60402
34 99084
35 145802
37 245581
38 360436
39 582174
42 725546
43 1098690
44 1253117
45 1827674
46 2176200
47 3392596
48 3247859
49 8061833
50 10156664
52 15104339
53 20972250
55 30743229
56 47396434
57 86667284
59 128855204
60 197693026
61 319053796
   \endverbatim
   and on satisfiable instances:
   \verbatim
1 0
2 1
4 3
5 2
9 10
11 13
13 28
14 82
20 849
24 3876
26 4089
30 7203
32 7829
36 56486
40 90824
41 47327
51 3103080
54 6534116
58 3311242
   \endverbatim
   while for OKsolver_2002-m2pp on unsatisfiable instances we get
   \verbatim
3 0
6 0
7 3
8 13
10 23
12 21
15 95
16 149
17 269
18 327
19 506
21 531
22 827
23 579
25 1422
27 1660
28 5231
29 11733
31 28568
33 50346
34 75476
35 89096
37 104528
38 127364
39 176162
42 147190
43 184838
44 160778
45 192573
46 147211
47 275624
48 270784
49 857768
50 1127490
52 2235868
53 2957250
55 6242319
56 8715038
57 23809039
59 38278372
60 84699156
61 200164136
62 328352918
64 548424851
   \endverbatim
   and on satisfiable instances
   \verbatim
1 0
2 0
4 0
5 0
9 4
11 12
13 10
14 46
20 34
24 902
26 723
30 4565
32 6806
36 30967
40 12782
41 17091
51 157123
54 704622
58 343357
63 48357413
   \endverbatim
   </li>
   <li> march_pl on unsatisfiable instances yields
   \verbatim
3 1
6 1
7 1
8 7
10 8
12 14
15 42
16 79
17 104
18 145
19 229
21 337
22 394
23 595
25 808
27 1830
28 2426
29 4902
31 8445
33 15912
34 19967
35 23350
37 36512
38 36060
39 67798
42 47424
43 93595
44 99402
45 143242
46 208265
47 199826
48 361802
49 537954
50 714290
52 1432507
53 1395913
55 5005466
56 2577348
57 13189902
59 6424070
60 40764011
61 36684789
62 127479802
64 135786906
65 151604456
66 147906156
67 255422670
   \endverbatim
   and on satisfiable instances
   \verbatim
1 1
2 1
4 3
5 2
9 7
11 5
13 12
14 13
20 280
24 152
26 266
30 2492
32 9269
36 17568
40 19596
41 23352
51 600247
54 2009327
58 1253491
63 14444240
   \endverbatim
   </li>
   <li> march_pl (without additional preprocessing) appears somewhat faster
   (say 50%) than the OKsolver (with additional preprocessing; on satisfiable
   instances the OKsolver is faster, except of the last one). The OKsolver
   finds a lot of autarkies, single nodes and quasi-single nodes (while never
   a pure literal!), but it doesn't seem to help. march_pl needs for n=67
   around 17 hours (255422670 nodes). </li>
   <li> Both solvers perform much worse than Minisat (1/2), also w.r.t. to
   node count. </li>
   <li> Satz ("VdWTransversalsInc 3 1 0 Out3 Out3_satz satz215"):
    <ol>
     <li> The solver needs to be amended, to output the DIMACS codes. </li>
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
98 280870577 35
99 353806066 36
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
100 338749424 36
     \endverbatim
     </li>
     <li> On unsatisfiable instances the performance gets somewhat worse,
     while on satisfiable instances it gets much better --- that should
     indicate that minisat is somehow mislead. </li>
     <li> Though perhaps with higher n the advantages of the ==-translation
     also shows for unsatisfiable instances. </li>
     <li> But at n=100 some explosion happens --- random noise?. </li>
    </ol>
   </li>
   <li> Is minisat2 better than minisat?
   It seems that actually minisat might be better on these instances
   than minisat2 (using more space, but less conflicts and less time). </li>
   <li> But one needs to consider the details:
    <ol>
     <li> Running it until around n=100, now using the "=="-translation.
      <ol>
       <li> Unsatisfiable instances
       \verbatim
3 0 0
6 0 0
7 11 1
8 22 1
10 49 1
12 59 1
15 111 2
16 166 2
17 313 3
18 417 3
19 661 4
21 779 4
22 968 5
23 1258 5
25 1418 6
27 1553 6
28 2397 7
29 4036 8
31 6440 9
33 4615 8
34 7860 10
35 10573 10
37 8603 10
38 11288 10
39 15200 11
42 10638 10
43 14437 11
44 23362 12
45 25874 13
46 38588 13
47 43570 14
48 73681 15
49 96170 16
50 113364 16
52 118687 16
53 220007 18
55 146065 17
56 236707 18
57 349943 19
59 403677 19
60 586168 20
61 664491 20
62 830131 21
64 882967 21
65 960923 21
66 2904850 24
67 2563297 24
68 4148332 25
69 3968840 25
70 7464135 26
72 6005056 26
73 9404277 27
75 6478533 26
76 9867033 27
77 8677632 27
78 20829112 29
79 15687689 28
80 28991192 30
81 45051479 31
83 41223492 31
85 22436493 29
86 34905948 30
87 59246155 32
88 79807052 32
89 73091913 32
90 139033008 34
91 140973507 34
93 87490458 33
94 171567132 34
96 135155444 34
97 158075013 34
98 231135131 35
99 287801453 35
       \endverbatim
       <li> Satisfiable instances
       \verbatim
1 0 1
2 0 1
4 0 1
5 0 1
9 7 1
11 10 1
13 6 1
14 74 1
20 715 4
24 512 4
26 404 3
30 3329 8
32 1071 5
36 5158 9
40 7446 9
41 9710 10
51 4965 9
54 155961 17
58 128937 16
63 303739 19
71 1358304 22
74 1629926 23
82 2819762 24
84 43183378 31
92 43962158 31
95 97919296 33
100 193857162 34
       \endverbatim
      </ol>
     </li>
     <li> Then the influence of the preprocessor has to be determined, that is,
     minisat2 with "-pre=none", and minisat1 with the minisat2-preprocessor.
      <ol>
       <li> minisat2 without preprocessor (using
       \verbatim
VdWTransversalsInc 3 1 0 Minisat2_none "minisat2 -pre=none"
       \endverbatim
       on unsatisfiable instances yields
       \verbatim
3 4 1
6 11 1
7 22 1
8 31 1
10 58 1
12 77 1
15 123 2
16 180 2
17 314 3
18 492 4
19 701 4
21 753 4
22 1160 5
23 1838 6
25 1571 6
27 1637 6
28 2725 7
29 3182 7
31 4617 8
33 4599 8
34 6966 9
35 7722 10
37 6967 9
38 13386 11
39 16244 11
42 17183 12
43 22332 12
44 18786 12
45 36011 13
46 46071 14
47 66219 15
48 82063 15
49 92510 16
50 129594 16
52 155439 17
53 243689 18
55 311815 19
56 314003 19
57 402141 19
59 411697 19
60 579606 20
61 891435 21
62 1201876 22
64 1053969 22
65 1238232 22
66 2074040 23
67 2718106 24
68 4682121 25
69 3832260 25
70 6859097 26
72 4851310 25
73 10448639 27
75 6128915 26
76 9857998 27
77 11057088 27
78 20644504 29
79 22957641 29
80 31666348 30
81 37091572 30
83 45906305 31
85 31694228 30
86 49034139 31
87 57898597 32
88 71480396 32
89 94294163 33
90 118363177 33
91 158391424 34
93 141312459 34
94 185615113 34
96 108643168 33
97 157111594 34
98 181210179 34
99 289218887 35
       \endverbatim
       while on satisfiable instances we get
       \verbatim
1 0 1
2 0 1
4 0 1
5 4 1
9 2 1
11 23 1
13 22 1
14 80 1
20 983 5
24 369 3
26 1192 5
30 1857 6
32 2555 7
36 2780 7
40 9710 10
41 20508 12
51 69553 15
54 164892 17
58 78855 15
63 702744 21
71 822313 21
74 7521194 26
82 1243316 22
84 6380425 26
92 5617159 26
95 135709998 34
       \endverbatim
       </li>
      </ol>
     </li>
     <li> Then there are other parameters one should try out; we need a little
     script for running through these possibilities. </li>
    </ol>
   </li>
   <li> It seems minisat2 (default) is alright, and so here is the data for
   higher k (length of arithmetic progressions):
    <ol>
     <li> k=4: UNSAT
     \verbatim
4 0 0
7 0 0
11 21 1
12 41 1
14 99 1
16 213 2
18 269 3
20 415 3
22 901 5
24 1724 6
26 2480 7
29 1997 6
31 2828 7
32 6540 9
35 5931 9
36 12962 11
38 10062 10
39 25714 12
41 38580 13
42 78134 15
44 80260 15
46 133921 17
47 245653 18
49 303124 19
51 374836 19
52 916748 21
55 729448 21
56 1574152 23
57 2036477 23
59 3614356 25
61 3631979 25
62 8085182 27
63 16139756 28
65 19822935 29
67 24259989 29
69 30449744 30
71 50039767 31
72 88157277 33
73 142420019 34
75 185575511 34
76 283528710 35
78 311740142 36
80 430323469 36
     \endverbatim
     SAT:
     \verbatim
1 0 1
2 0 1
3 0 1
5 0 1
6 0 1
8 0 1
9 1 1
10 3 1
13 55 1
15 29 1
17 229 2
19 72 1
21 62 1
23 100 2
25 873 5
27 1118 5
28 1052 5
30 1270 5
33 4431 8
34 5519 9
37 2223 7
40 11055 10
43 16846 11
45 19505 12
48 17041 11
50 33658 13
53 181544 17
54 228740 18
58 284490 18
60 265315 18
64 574639 20
66 5748214 26
68 628409 20
70 18792317 29
74 2204633 23
77 3025860 24
79 167509159 34
     \endverbatim
     </li>
     <li> k=5: UNSAT
     \verbatim
5 0 0
10 0 0
15 23 1
20 41 1
21 92 1
22 235 2
23 669 4
26 1633 6
30 2348 7
32 4646 8
35 7362 9
40 4183 8
45 5797 9
46 10603 10
47 20996 12
48 31424 13
50 80934 15
53 159754 17
55 306347 19
60 183496 17
65 438949 19
70 220183 18
71 578068 20
72 1458443 22
73 2153008 23
74 4365056 25
75 11351133 27
80 13428946 28
85 6455659 26
90 2419969 24
95 414839 19
96 1804748 23
97 12492407 28
98 1959272 23
99 55797301 31
100 81560970 32
101 105473100 33
102 205001722 35
103 445026053 37
104 1468250693 39
     \endverbatim
     SAT:
     \verbatim
1 0 1
2 0 1
3 0 1
4 0 1
6 0 1
7 0 1
8 0 1
9 0 1
11 0 1
12 2 1
13 4 1
14 7 1
16 10 1
17 17 1
18 13 1
19 14 1
24 110 2
25 380 3
27 640 4
28 213 2
29 196 2
31 2128 7
33 659 4
34 435 3
36 112 2
37 1015 5
38 403 3
39 1614 6
41 670 4
42 1926 6
43 4623 8
44 4989 9
49 24684 12
51 1142 5
52 24131 12
54 12102 11
56 3043 7
57 3972 8
58 3190 7
59 50919 14
61 27360 13
62 5645 9
63 99868 16
64 171373 17
66 46411 14
67 48442 14
68 238306 18
69 181807 17
76 884 5
77 317126 19
78 10016 10
79 1241964 22
81 10146 10
82 3078 7
83 19417 12
84 22675 12
86 137960 17
87 1001028 22
88 32321 13
89 1634604 23
91 143890 17
92 757993 21
93 1614067 23
94 14432 11
     \endverbatim
     </li>
     <li> k=6: UNSAT
     \verbatim
6 0 0
11 0 0
16 41 1
21 74 1
27 200 2
28 392 3
30 799 4
31 2352 7
34 4324 8
38 7004 9
42 9618 10
43 21397 12
47 30840 13
52 35246 13
53 91760 16
55 202854 18
57 504262 20
60 1211411 22
63 2123270 23
67 2790392 24
69 6714438 26
72 12751705 28
77 13442970 28
78 33797428 30
79 76529095 32
81 170347363 34
84 300013697 36
     \endverbatim
     SAT:
     \verbatim
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
7 0 1
8 0 1
9 0 1
10 0 1
12 0 1
13 0 1
14 5 1
15 3 1
17 8 1
18 35 1
19 36 1
20 27 1
22 69 1
23 50 1
24 81 1
25 106 2
26 109 2
29 336 3
32 394 3
33 164 2
35 535 4
36 28 1
37 1616 6
39 2374 7
40 2759 7
41 10286 10
44 906 5
45 452 3
46 280 3
48 2809 7
49 10379 10
50 344 3
51 12989 11
54 5527 9
56 23251 12
58 34575 13
59 256590 18
61 213087 18
62 1417603 22
64 517024 20
65 159555 17
66 2634105 24
68 692110 21
70 648665 20
71 303009 19
73 758561 21
74 13064 11
75 445235 20
76 7267066 26
80 72878179 32
82 2837072 24
83 57564233 32
85 8480670 27
86 300003261 36
     \endverbatim
     </li>
     <li> k=7: UNSAT
     \verbatim
7 0 0
14 0 0
21 38 1
28 85 1
35 285 3
42 301 3
43 564 4
44 2116 7
45 6008 9
47 22014 12
49 59864 15
54 75870 15
58 97939 16
62 229252 18
66 394951 19
70 643579 20
77 939031 21
84 950362 21
91 1109771 22
92 4529562 25
93 13131153 28
94 28635930 30
96 64237673 32
97 317546088 36
99 618532925 37
105 469397307 37
108 1091186770 39
     \endverbatim
     SAT:
     \verbatim
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
8 0 1
9 0 1
10 0 1
11 0 1
12 0 1
13 0 1
15 4 1
16 2 1
17 3 1
18 7 1
19 10 1
20 24 1
22 40 1
23 7 1
24 42 1
25 74 1
26 85 1
27 44 1
29 70 1
30 93 1
31 167 2
32 177 2
33 57 1
34 159 2
36 181 2
37 210 2
38 190 2
39 151 2
40 60 1
41 308 3
46 1003 5
48 14643 11
50 14380 11
51 3817 8
52 5068 9
53 62985 15
55 850 5
56 24107 12
57 107492 16
59 16320 11
60 12962 11
61 44454 14
63 32078 13
64 20026 12
65 147807 17
67 999 5
68 6350 9
69 25898 13
71 2234 7
72 12107 11
73 52416 14
74 12323 11
75 5172 9
76 51284 14
78 9961 10
79 63839 15
80 5655 9
81 48500 14
82 98160 16
83 10102 10
85 412305 19
86 242178 18
87 7029 9
88 413901 19
89 1122951 22
90 1157756 22
95 4845109 25
98 31186822 30
100 77907 15
101 225173 18
102 1596157 23
103 3635619 25
104 42575635 31
106 145262220 34
107 218596449 35
109 413489 19
110 118717853 33
111 205451 18
     \endverbatim
     </li>
     <li> k=8: UNSAT
     \verbatim
8 0 0
15 0 0
22 46 1
29 72 1
36 253 2
43 642 4
51 665 4
52 1922 6
53 5021 9
55 20569 12
57 48443 14
60 153913 17
64 374838 19
70 451447 20
73 1438452 22
79 1326698 22
81 4414647 25
86 8626206 27
93 9383753 27
100 19998691 29
101 61493438 32
102 171617680 34
103 550474608 37
     \endverbatim
     SAT:
     \verbatim
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
7 0 1
9 0 1
10 0 1
11 0 1
12 0 1
13 0 1
14 0 1
16 7 1
17 5 1
18 5 1
19 7 1
20 11 1
21 7 1
23 12 1
24 10 1
25 19 1
26 40 1
27 56 1
28 31 1
30 129 2
31 141 2
32 187 2
33 122 2
34 226 2
35 201 2
37 200 2
38 121 2
39 127 2
40 285 3
41 231 2
42 278 3
44 108 2
45 363 3
46 190 2
47 106 2
48 348 3
49 546 4
50 413 3
54 9264 10
56 34156 13
58 2952 7
59 36577 13
61 815 4
62 6007 9
63 14423 11
65 8290 10
66 29837 13
67 28737 13
68 24014 12
69 191836 17
71 24323 12
72 585131 20
74 4094 8
75 29341 13
76 27468 13
77 191139 17
78 604978 20
80 476824 20
82 125410 16
83 5052 9
84 804014 21
85 541619 20
87 15532 11
88 124804 16
89 142716 17
90 1552278 23
91 158584 17
92 2290075 24
94 40208 14
95 726332 21
96 6374 9
97 2922890 24
98 6835055 26
99 6143 9
104 57118444 31
     \endverbatim
     </li>
     <li> k=9: UNSAT
     \verbatim
9 0 0
18 0 0
25 64 1
32 123 2
39 264 3
46 591 4
53 1247 5
58 3733 8
59 7687 10
62 23712 12
66 63323 15
72 60666 15
74 495099 20
77 1980265 23
81 5414534 26
87 7792366 27
91 20037377 29
97 29619058 30
102 63863403 32
106 143242348 34
110 141530802 34
     \endverbatim
     SAT:
     \verbatim
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
7 0 1
8 0 1
10 0 1
11 0 1
12 0 1
13 0 1
14 0 1
15 0 1
16 0 1
17 0 1
19 5 1
20 10 1
21 6 1
22 3 1
23 7 1
24 9 1
26 43 1
27 22 1
28 62 1
29 76 1
30 34 1
31 52 1
33 27 1
34 54 1
35 169 2
36 159 2
37 69 1
38 336 3
40 387 3
41 406 3
42 224 2
43 297 3
44 470 3
45 247 2
47 594 4
48 506 4
49 235 2
50 366 3
51 665 4
52 697 4
54 989 5
55 658 4
56 899 5
57 3190 7
60 703 4
61 11335 10
63 15016 11
64 20165 12
65 45961 14
67 1159 5
68 13365 11
69 4215 8
70 3676 8
71 8419 10
73 9265 10
75 5312 9
76 28222 13
78 43114 14
79 40009 14
80 437828 19
82 45512 14
83 267782 18
84 31016 13
85 441535 19
86 2704946 24
88 496432 20
89 186373 17
90 4746175 25
92 1647053 23
93 19296 12
94 653091 20
95 1148434 22
96 16944369 28
98 2314783 24
99 32862927 30
100 2973521 24
101 11152146 27
103 376698 19
104 25578923 30
105 85490078 32
107 10077527 27
108 60469378 32
109 95424570 33
111 18563069 29
112 72021682 32
113 26586596 30
114 195887406 35
115 139485801 34
     \endverbatim
     </li>
     <li> k=10: UNSAT
     \verbatim
10 0 0
19 0 0
29 58 1
34 145 2
41 294 3
48 932 5
55 2049 6
62 3625 8
65 7958 10
69 27317 13
74 42364 14
79 153059 17
85 368036 19
89 1585013 23
92 5126566 26
96 13711561 28
101 31377020 30
106 67732138 32
110 239197206 35
     \endverbatim
     SAT:
     \verbatim
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
7 0 1
8 0 1
9 0 1
11 0 1
12 0 1
13 0 1
14 0 1
15 0 1
16 0 1
17 0 1
18 0 1
20 5 1
21 6 1
22 7 1
23 11 1
24 6 1
25 11 1
26 6 1
27 11 1
28 50 1
30 9 1
31 47 1
32 78 1
33 63 1
35 75 1
36 75 1
37 128 2
38 164 2
39 255 3
40 286 3
42 83 1
43 231 2
44 238 2
45 371 3
46 195 2
47 629 4
49 636 4
50 1097 5
51 1125 5
52 233 2
53 987 5
54 1135 5
56 1381 6
57 1050 5
58 1655 6
59 2206 7
60 2957 7
61 2867 7
63 2056 6
64 3197 7
66 2425 7
67 9420 10
68 8497 10
70 13125 11
71 15514 11
72 52371 14
73 78059 15
75 12152 11
76 71096 15
77 16651 11
78 117794 16
80 8086 10
81 8454 10
82 29534 13
83 54713 14
84 115388 16
86 16848 11
87 46891 14
88 57450 14
90 54729 14
91 95526 16
93 96610 16
94 391398 19
95 216655 18
97 129100 16
98 19641 12
99 871006 21
100 12569066 28
102 1142496 22
103 7443364 26
104 3066680 24
105 4696223 25
107 172523 17
108 3625093 25
109 28150540 30
111 15095115 28
112 56959012 31
113 69328523 32
114 6396562 26
115 150097470 34
     \endverbatim
     </li>
     <li> k=11: UNSAT
     \verbatim
11 0 0
22 0 0
33 67 1
44 99 1
55 337 3
66 585 4
77 1022 5
88 1201 5
99 931 5
110 1867 6
111 8965 10
112 9520 10
113 49631 14
114 400117 19
116 4371365 25
118 19258114 29
119 143004281 34
121 608944908 37
     \endverbatim
     SAT:
     \verbatim
1 0 1
2 0 1
3 0 1
4 0 1
5 0 1
6 0 1
7 0 1
8 0 1
9 0 1
10 0 1
12 0 1
13 0 1
14 0 1
15 0 1
16 0 1
17 0 1
18 0 1
19 0 1
20 0 1
21 0 1
23 7 1
24 3 1
25 2 1
26 7 1
27 14 1
28 21 1
29 18 1
30 25 1
31 38 1
32 37 1
34 28 1
35 60 1
36 57 1
37 38 1
38 67 1
39 60 1
40 134 2
41 90 1
42 114 2
43 116 2
45 132 2
46 149 2
47 143 2
48 134 2
49 232 2
50 192 2
51 257 3
52 193 2
53 170 2
54 436 3
56 205 2
57 164 2
58 140 2
59 400 3
60 144 2
61 373 3
62 686 4
63 213 2
64 860 5
65 597 4
67 334 3
68 771 4
69 645 4
70 621 4
71 618 4
72 272 3
73 206 2
74 605 4
75 494 4
76 494 4
78 309 3
79 672 4
80 929 5
81 597 4
82 456 3
83 652 4
84 399 3
85 246 2
86 443 3
87 1339 6
89 568 4
90 1532 6
91 901 5
92 550 4
93 955 5
94 1203 5
95 710 4
96 777 4
97 2237 7
98 2170 7
100 1181 5
101 1394 6
102 1195 5
103 998 5
104 1788 6
105 893 5
106 1855 6
107 1625 6
108 1039 5
109 1754 6
115 293488 18
117 2498959 24
120 14128419 28
122 4688813 25
123 1928805 23
124 4834600 25
125 31639990 30
126 3407763 25
127 22271062 29
128 21308919 29
     \endverbatim
     </li>
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


  \todo Using CSP solvers

*/
