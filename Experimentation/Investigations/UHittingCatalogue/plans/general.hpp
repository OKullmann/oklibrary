// Oliver Kullmann, 4.9.2009 (Swansea)
/* Copyright 2009, 2010, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/UHittingCatalogue/plans/general.hpp
  \brief Plans regarding building the catalogue of unsatisfiable non-singular hitting clause-sets of low deficiency


  \todo Scope of the catalogue
  <ul>
   <li> Regarding the deficiency:
    <ol>
     <li> For deficiencies k=1,2 we definitely have all isomorphism types
     (counts 1 and 2). </li>
     <li> According to unpublished results of XSZ and me, also for k=3 we have
     all isomorphism types (count 25). </li>
     <li> For k=4 we should collect everything, in order to build up also here
     a complete collection. </li>
     <li> This is the most important goal; currently there are 229 elements.
     </li>
    </ol>
   </li>
   <li> Regarding n:
    <ol>
     <li> For n <= 4 the catalogue is complete (determined computationally).
     </li>
     <li> Perhaps we should aim at getting everything for n <= 5. </li>
     <li> Counts: n=0 -> 1, n=1 -> 0, n=2 -> 1, n=3 -> 6, n=4 -> 368. </li>
     <li> So let's just run all_unsinghitting(5, H5) (cs-oksvr XXX). </li>
    </ol>
   </li>
   <li> Regarding closure under partial assignments:
    <ol>
     <li> We have closure under the operation of partial assignments for
     n <= 9 (determined by closure_uhit_def_pass(9, H)). </li>
    </ol>
   </li>
  </ul


  \todo Connections
  <ul>
   <li> W.r.t. general investigations into minimal unsatisfiability see
   Experimentation/Investigations/MinimalUnsatisfiability/plans/general.hpp.
   </li>
  </ul>


  \todo Integer sequences of interest
  <ul>
   <li> Once the finiteness-conjecture has been established, the sequence
   k >= 1 -> number of isomorphism types of nonsingular unsatisfiable hitting
   clause-sets of deficiency k
   is of great interest. Its first three terms are 1,2,25. </li>
   <li> An easier sequence is
   n >= 0 -> number of isomorphism types of nonsingular unsatisfiable hitting
   clause-sets with n variables.
   Its first 5 terms are 1,0,1,6,368. </li>
   <li> We should submit these sequences to OEIS, once we have something
   published on them. </li>
   <li> For a given deficiency k, the maximal min-variable-degree is also of
   interest; once we get the journal version of the SAT2011 paper published,
   such sequences should be submitted to OEIS. </li>
  </ul>


  \todo Deficiency = 3, n = 5
  <ul>
   <li> It should be possible to completely determine these instances. </li>
   <li>
   \verbatim
oklib_monitor:true;
oklib_monitor_level:1;
all_unsinghitting_def(3,5,'Result_3_5);
   \endverbatim
   </li>
   <li> What does the monitor-output mean? Several times the same clause-sets
   are output? It seems that the isomorphism check takes only place
   afterwards. But we don't need to see these outputs. </li>
   <li> We need the output into a file. </li>
   <li> We need to incorporate the already known instances (from the
   catalogue). </li>
   <li> And we need the possibility to stop and re-start the whole
   computation, so that we have a chance of finally finishing this case.
   </li>
   <li> See "derived_hitting_cs_pred_isoelim" in
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp.
   </li>
  </ul>


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
last(min_nssplittree_def(4));
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
# n(i)=3, c(G')=4: waehle 1,2,6 as identifications, and take all four
# common clauses {-2,-1,6}, {-2,1,6}, {-1,2,6}, {1,2,6}}
# so rename in the second instance: 3 -> 7, 4 -> 8, 5 -> 9, v -> 10
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
last(min_nssplittree_def(4));
6

# that seems to be all regarding glueing F1 with F1
       \endverbatim
       </li>
       <li> Gluing of F2 with F2:
       \verbatim
F2 :  {{-7,-6,-5,-4},{-7,-6,4},{-7,-5,6},{-7,-4,5},{-7,4,5,6},{-3,-2,-1,7},
  {-3,1,7},{-2,3,7},{-1,2,7},{1,2,3,7}}
# n(i)=4, c(G')=5: waehle 1,2,3,7 as identifications, and take all 5
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
last(min_nssplittree_def(4));
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
last(min_nssplittree_def(4));
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

*/
