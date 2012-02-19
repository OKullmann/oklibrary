// Oliver Kullmann, 4.9.2009 (Swansea)
/* Copyright 2009, 2010, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/UHittingCatalogue/plans/general.hpp
  \brief Plans regarding building the catalogue of unsatisfiable non-singular hitting clause-sets of low deficiency


  \todo Connections
  <ul>
   <li> W.r.t. general investigations into minimal unsatisfiability see
   Experimentation/Investigations/MinimalUnsatisfiability/plans/general.hpp.
   </li>
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
   <li> In the OK+XSZ report we have the conjecture 2^(k-1): The upper bound
   is 2^(k-1) --- can this bound be attained? </li>
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
   <li> The first open case is for k=4:
    <ol>
     <li> Currently we just have lmax(min_nssplittree_def(k))=6 < 8. </li>
     <li> The task is to glue the 3 instances with deficiency 3 and complexity
     4 to obtain an instance of complexity 8. </li>
     <li> Ad-hoc construction, two times the first instance:
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
     <li> In general, we need a function
     \verbatim
search_gluing(F1,F2,k)
     \endverbatim
     which has two uhit-clause-sets F1, F2 as inputs and a natural number k,
     and searches through all possibilities for gluing F1,F2 (using renaming)
     to obtain a uhit-clause-set of deficiency k: the result is a list of such
     gluings, containing each possible isomorphism-type exactly once.
     </li>
     <li> Let v be the gluing-variable. XXX </li>
    </ol>
   </li>
  </ul>

*/
