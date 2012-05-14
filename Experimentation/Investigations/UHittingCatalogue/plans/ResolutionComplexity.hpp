// Oliver Kullmann, 13.5.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/UHittingCatalogue/plans/ResolutionComplexity.hpp
  \brief Plans regarding resolution-complexity of unsatisfiable hitting clause-sets


 \todo Finding hard unsatisfiable hitting clause-sets
  <ul>
   <li> What is lmax(min_nssplittree_def(k)) ? </li>
   <li> In the OK+XSZ report we have the conjecture 2^(k-1), i.e., the upper
   bound is 2^(k-1) --- can this bound be attained? </li>
   <li> For k=3 the bound is attained thrice:
   \verbatim
L : min_nssplittree_def(3);
Evaluation took 3.5750 seconds (3.5840 elapsed)
  [2,2,2,2,3,2,2,2,3,2,2,2,3,4,2,3,3,3,3,3,3,3,4,3,4]
S : sublist_indices_el(L,4);
  [14,23,25]
A : all_uhit_def(3)$
for i in S do print(i,A[i]);
 14 {{-5,-4,-3},{-5,-4,3},{-5,-3,4},{-5,3,4},{-2,-1,5},{-2,1,5},{-1,2,5},
    {1,2,5}}
 23 {{-6,-5,-4,-3},{-6,-5,3},{-6,-4,5},{-6,-3,4},{-6,3,4,5},{-2,-1,6},
    {-2,1,6},{-1,2,6},{1,2,6}}
 25
  {{-7,-6,-5,-4},{-7,-6,4},{-7,-5,6},{-7,-4,5},{-7,4,5,6},{-3,-2,-1,7},
  {-3,1,7},{-2,3,7},{-1,2,7},{1,2,3,7}}

# For comparisons, which of the 25 uhit-clause-sets are tree-like:
map(treehittingcsp,A);
  [true,true,true,false,true,false,false,true,true,false,
   false,false,true,true,true,false,false,false,false,true,
   false,true,false,false,false]
# so the first above example is tree-like, the two others aren't (but they
# both have a full variable).
   \endverbatim
   </li>
   <li> Let the above instances be F0,F1,F2. </li>
   <li> F0 is full variable-disjoint gluing of musatd2_cs(2) with itself,
   F1 the full variable-disjoint gluing of musatd2_s(2) with musatd2_cs(3),
   and F2 full variable-disjoint gluing of musatd2_cs(3) with itself. </li>
   <li> The first open case is for k=4:
    <ol>
     <li> Currently we just have lmax(min_nssplittree_def(k))=6 < 8. </li>
     <li> The task is to glue the 3 instances with deficiency 3 and complexity
     4 to obtain an instance of complexity 8. </li>
     <li> Ad-hoc construction, two times F0:
     \verbatim
{-5,-4,-3},{-5,-4,3},{-5,-3,4},{-5,3,4},
v: {-2,-1,5},{-2,1,5},{-1,2,5},{1,2,5}

{-5,-4,-3},{-5,-4,3},{-5,-3,4},{-5,3,4},
-v: {-2',-1',5},{-2',1',5},{-1',2',5},{1',2',5}

1' -> 6
2' -> 7
v -> 8

F : {
{-2,-1,5,8},{-2,1,5,8},{-1,2,5,8},{1,2,5,8},
{-5,-4,-3},{-5,-4,3},{-5,-3,4},{-5,3,4},
{-7,-6,5,-8},{-7,6,5,-8},{-6,7,5,-8},{6,7,5,-8}
};
hittingcsp(F);
  true
sat_decision_hitting_cs(F);
  false
classify_candidates_uhit_def([F]);
  [[[4,8],"new"]]
# entered into uhit-catalogue
classify_candidates_uhit_def([F]);
  [[[4,8],1]]
min_nssplittree_isot[[[4,8],1]];
  6
     \endverbatim
     </li>
     <li> Using the general construction below, with delta(F0)=delta(F1)=3
     and k=4, where then thus c(G')-n(i)=1 has to be realised:
      <ol>
       <li> Gluing of F1 with F1:
       \verbatim
F1 : {{-6,-5,-4,-3},{-6,-5,3},{-6,-4,5},{-6,-3,4},{-6,3,4,5},{-2,-1,6},
    {-2,1,6},{-1,2,6},{1,2,6}}
# n(i)=3, c(G')=4: choose 1,2,6 as identifications, and take all four
# common clauses {-2,-1,6}, {-2,1,6}, {-1,2,6}, {1,2,6}}
# so rename in the second instance: 3 -> 7, 4 -> 8, 5 -> 9; v -> 10
# obtain:
F : {
{10,-6,-5,-4,-3},{10,-6,-5,3},{10,-6,-4,5},{10,-6,-3,4},{10,-6,3,4,5},
{-2,-1,6},{-2,1,6},{-1,2,6},{1,2,6},
{-10,-6,-9,-8,-7},{-10,-6,-9,7},{-10,-6,-8,9},{-10,-6,-7,8},{-10,-6,7,8,9}
};
hittingcsp(F);
  true
sat_decision_hitting_cs(F);
  false
classify_candidates_uhit_def([F]);
  [[[4,10],"new"]]
# entered into uhit-catalogue
classify_candidates_uhit_def([F]);
  [[[4,10],1]]
min_nssplittree_isot[[[4,10],1]];
  6

# The second possibility:
# n(i) = 4, c(G') = 5: choose 3,4,5,6 as identifications, and take all five
# common clauses {-6,-5,-4,-3},{-6,-5,3},{-6,-4,5},{-6,-3,4},{-6,3,4,5}
# so rename in the second instance: 1 -> 7, 2 -> 8; v -> 9
# obtain:
F : {
{9,-2,-1,6},{9,-2,1,6},{9,-1,2,6},{9,1,2,6},
{-6,-5,-4,-3},{-6,-5,3},{-6,-4,5},{-6,-3,4},{-6,3,4,5},
{-9,-8,-7,6},{-9,-8,7,6},{-9,-7,8,6},{-9,7,8,6}
};
hittingcsp(F);
  true
sat_decision_hitting_cs(F);
  false
classify_candidates_uhit_def([F]);
  [[[4,9],"new"]]
# entered into uhit-catalogue
classify_candidates_uhit_def([F]);
  [[[4,9],2]]
min_nssplittree_isot[[[4,9],2]];
  6
       \endverbatim
       </li>
       <li> Gluing of F2 with F2:
       \verbatim
F2 :  {{-7,-6,-5,-4},{-7,-6,4},{-7,-5,6},{-7,-4,5},{-7,4,5,6},{-3,-2,-1,7},
  {-3,1,7},{-2,3,7},{-1,2,7},{1,2,3,7}}
# n(i)=4, c(G')=5: choose 1,2,3,7 as identifications, and take all 5
# common clauses containing these variables
# so rename in the second instance: 4 -> 8, 5 -> 9, 6 -> 10, v -> 11
# obtain:
F : {
{11,-7,-6,-5,-4},{11,-7,-6,4},{11,-7,-5,6},{11,-7,-4,5},{11,-7,4,5,6},
{-3,-2,-1,7},{-3,1,7},{-2,3,7},{-1,2,7},{1,2,3,7},
{-11,-7,-10,-9,-8},{-11,-7,-10,8},{-11,-7,-9,10},{-11,-7,-8,9},{-11,-7,8,9,10}
};
hittingcsp(F);
  true
sat_decision_hitting_cs(F);
  false
classify_candidates_uhit_def([F]);
  [[[4,11],1]]
# for another i choose 4,5,6,7 as identifications; should be the same XXX

# that seems to be all regarding glueing F2 with F2
       \endverbatim
       </li>
       <li> Gluing of F0 with F1:
       \verbatim
F0 : {{-5,-4,-3},{-5,-4,3},{-5,-3,4},{-5,3,4},{-2,-1,5},{-2,1,5},{-1,2,5},
    {1,2,5}}
F1 : {{-6,-5,-4,-3},{-6,-5,3},{-6,-4,5},{-6,-3,4},{-6,3,4,5},{-2,-1,6},
    {-2,1,6},{-1,2,6},{1,2,6}}
# identify 1 -> 1, 2 -> 2, 5 -> 6
# so rename in the second instance: 3 -> 6, 4 -> 7, 5 -> 8, 6 -> 5, v -> 9
F : {
{9,-5,-4,-3},{9,-5,-4,3},{9,-5,-3,4},{9,-5,3,4},
{-2,-1,5},{-2,1,5},{-1,2,5},{1,2,5},
{-9,-5,-8,-7,-6},{-9,-5,-8,6},{-9,-5,-7,8},{-9,-5,-6,7},{-9,-5,6,7,8}
};
hittingcsp(F);
  true
sat_decision_hitting_cs(F);
  false
classify_candidates_uhit_def([F]);
  [[[4,9],"new"]]
# entered into uhit-catalogue
classify_candidates_uhit_def([F]);
  [[[4,9],1]]

  6
# that seems to be all regarding glueing F0 with F1
       \endverbatim
       </li>
       <li> Gluing of F0 with F2:
       \verbatim
F0 : {{-5,-4,-3},{-5,-4,3},{-5,-3,4},{-5,3,4},{-2,-1,5},{-2,1,5},{-1,2,5},
    {1,2,5}}
F2 : {{-7,-6,-5,-4},{-7,-6,4},{-7,-5,6},{-7,-4,5},{-7,4,5,6},{-3,-2,-1,7},
  {-3,1,7},{-2,3,7},{-1,2,7},{1,2,3,7}}
       \endverbatim
       It doesn't seem possible.
       </li>
       <li> Gluing of F1 with F2:
       \verbatim
F1 : {{-6,-5,-4,-3},{-6,-5,3},{-6,-4,5},{-6,-3,4},{-6,3,4,5},{-2,-1,6},
    {-2,1,6},{-1,2,6},{1,2,6}}
F2 : {{-7,-6,-5,-4},{-7,-6,4},{-7,-5,6},{-7,-4,5},{-7,4,5,6},{-3,-2,-1,7},
  {-3,1,7},{-2,3,7},{-1,2,7},{1,2,3,7}}

# identify 3 -> 4, 4 -> 5, 5 -> 6, 6 -> 7
# so rename in the second instance: 1 -> 7, 2 -> 8, 3 -> 9, 4 -> 3, 5 -> 4
# 6 -> 5, 7 -> 6; v -> 10
F : {
{10,-2,-1,6},{10,-2,1,6},{10,-1,2,6},{10,1,2,6},
{-6,-5,-4,-3},{-6,-5,3},{-6,-4,5},{-6,-3,4},{-6,3,4,5},
{-10,-9,-8,-7,6},{-10,-9,7,6},{-10,-8,9,6},{-10,-7,8,6},{-10,7,8,9,6}
};
hittingcsp(F);
  true
sat_decision_hitting_cs(F);
  false
classify_candidates_uhit_def([F]);
  [[[4,10],"new"]]
# entered into uhit-catalogue
classify_candidates_uhit_def([F]);
  [[[4,10],2]]
min_nssplittree_isot[[[4,10],2]];
  6
# that seems to be all regarding glueing F1 with F2
       \endverbatim
       </li>
      </ol>
     </li>
     <li> Splitting can create also singular clause-sets; not clear how to
     systematically handle them. </li>
    </ol>
     </li>
     <li> In general, we need a function
     \verbatim
search_gluing(F1,F2,k)
     \endverbatim
     which has two uhit-clause-sets F0, F1 as inputs and a natural number k,
     and searches through all possibilities for gluing F0,F1 (using renaming)
     to obtain a uhit-clause-set of deficiency k: the result is a list of such
     gluings F, containing each possible isomorphism-type exactly once.
     </li>
     <li> Let v be the gluing-variable. </li>
     <li> Assume var(F0) disjoint var(F1). </li>
     <li> W.l.o.g.: n(F0) <= n(F1). </li>
     <li> To run through all possibilities, run through all
      <ol>
       <li> injections i: var(F0) -> var(F1) </li>
       <li> using F1' := result of renaming i(v) to v, and G := F0 intersection
       F1', choose a subset G' <= G; the meaning is that those clauses in G'
       will occur only once, while the clauses in G-G' are realised via two
       clauses (with v, -v). </li>
      </ol>
     </li>
     <li> The resulting F has deficiency delta(F0)+delta(F1)-1-c(G')+n(i).
     </li>
    </ol>
   </li>
  </ul>


  \todo Hardness
  <ul>
   <li> What is the hardness of nonsingular unsatisfiable hitting clause-sets
   of deficiency k? </li>
   <li> Printing the current maximal hardness in the catalogue:
   \verbatim
for k in uhit_def[0,"info"] do print(k, lmax(map(hardness_u_cs,all_uhit_def(k))));
1 0
2 2
3 3
4 3
5 4
6 3
7 3
8 3
9 5
10 4
11 4
12 4
13 4
14 3
15 4
16 3
17 6
18 4
19 4
20 4
21 4
22 4
23 4
24 5
25 4
26 4
27 5
28 5
43 5
58 6
59 6
   \endverbatim
   </li>
   <li> Analysing deficiency 3 (where for have complete information) more
   closely:
   \verbatim
block([L:all_uhit_def(3),F], for i : 1 thru length(L) do (F: L[i], printf(true, "~3d ~2d ~2d~%", i, nvar_cs(F), hardness_u_cs(F))));
  1  3  2
  2  3  2
  3  3  2
  4  4  2
  5  4  2
  6  4  2
  7  4  2
  8  4  2
  9  4  2
 10  4  2
 11  4  2
 12  4  2
 13  4  2
 14  5  3
 15  5  3
 16  5  2
 17  5  2
 18  5  2
 19  5  2
 20  5  2
 21  5  2
 22  5  2
 23  6  3
 24  6  2
 25  7  3
   \endverbatim
   So the minimum n with hardness=4 is n=5.
   </li>
   <li> A conjecture is that for nonsingular unsatisfiable hitting clause-sets
   F holds hardness(F) <= order_deficiency(deficiency(F)). Checking this
   conjecture:
   \verbatim
for k in uhit_def[0,"info"] do block([m:lmax(map(hardness_u_cs,all_uhit_def(k)))], printf(true, "~3d ~2d ~2d ~2d~%", k, m, order_deficiency(k), order_deficiency(k)-m));
  1  0  0  0
  2  2  2  0
  3  3  3  0
  4  3  3  0
  5  4  3  -1
  6  3  4  1
  7  3  4  1
  8  3  4  1
  9  5  4  -1
 10  4  4  0
 11  4  4  0
 12  4  4  0
 13  4  5  1
 14  3  5  2
 15  4  5  1
 16  3  5  2
 17  6  5  -1
 18  4  5  1
 19  4  5  1
 20  4  5  1
 21  4  5  1
 22  4  5  1
 23  4  5  1
 24  5  5  0
 25  4  5  1
 26  4  5  1
 27  5  5  0
 28  5  6  1
 43  5  6  1
 58  6  6  0
 59  6  7  1
   \endverbatim
   We see that it's false, due to the sequence max_var_hittingdef2_cs (see
   below). </li>
   <li> We need to go through all our generators:
    <ol>
     <li> nearly_full_hitting_fcs:
     \verbatim
for n : 1 thru 7 do block([F:nearly_full_hitting_fcs(n)[2]], printf(true,"~2d ~5a ~6a ~6a ~3d~%",n,check_hitting_nsing_def(F), uuhittingcsp(F), treehittingcsp(F), hardness_u_cs(F)));
 1 [1]   true   true     0
 2 []    false  true     1
 3 [4]   false  true     2
 4 [11]  false  true     3
 5 [26]  false  true     4
 6 [57]  false  true     5
 7 [120] false  true     6
     \endverbatim
     </li>
     <li> over_full_hitting_fcs:
     \verbatim
for n : 1 thru 7 do block([F:over_full_hitting_fcs(n)[2]], printf(true,"~2d ~5a ~6a ~6a ~3d~%",n,check_hitting_nsing_def(F), uuhittingcsp(F), treehittingcsp(F), hardness_u_cs(F)));
 1 [2]   true   true     2
 2 [3]   false  true     2
 3 [6]   false  true     3
 4 [13]  false  true     4
 5 [28]  false  true     5
 6 [59]  false  true     6
 7 [122] false  true     7
     \endverbatim
     </li>
     <li> max_var_hitting_def:
     \verbatim
for k : 2 thru 10 do block([F:max_var_hitting_def(k)[2]], printf(true,"~2d ~5a ~6a ~6a ~3d~%",k,check_hitting_nsing_def(F), uuhittingcsp(F), treehittingcsp(F), hardness_u_cs(F)));
 2 [2]   false  false    2
 3 [3]   false  false    3
 4 [4]   false  false    3
 5 [5]   false  false    3
 6 [6]   false  false    3
 7 [7]   false  false    3
 8 [8]   false  false    3
 9 [9]   false  false    3
10 [10]  false  false    3
     \endverbatim
     </li>
     <li> max_var_hittingdef2_cs:
     \verbatim
for h : 2 thru 4 do block([F:max_var_hittingdef2_cs(h)[2]], printf(true,"~2d ~5a ~6a ~6a ~3d~%",h,check_hitting_nsing_def(F), uuhittingcsp(F), treehittingcsp(F), hardness_u_cs(F)));
 2 [2]   false  false    2
 3 [3]   false  false    3
 4 [5]   false  false    4
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>

*/
