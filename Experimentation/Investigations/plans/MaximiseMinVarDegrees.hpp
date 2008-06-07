// Oliver Kullmann, 2.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/plans/MaximiseMinVarDegrees.hpp
  \brief Plans on investigations into the maximum of the min-var-degree of parameterised classes of unsatisfiable clause-sets


  \todo Minimally unsatisfiable clause-sets
  <ul>
   <li> See under "Maximal min-var-degrees" in
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/Basics.mac
   </li>
  </ul>


  \todo Unsatisfiable non-singular hitting clause-sets
  <ul>
   <li> See under "Maximal min-var-degrees" in
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/Basics.mac
   </li>
   <li> And see todo "Maximal min-var-degrees" in
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/general.hpp
   </li>
   <li> The function "sharp_uhit_catalogue_maxminvardeg" investigates
   the catalogue for the sharp cases w.r.t. the general upper bound. </li>
   <li> n-oriented investigations:
    <ol>
     <li> One tool is e.g. "all_unsinghitting(5, 'all_n5)" and
     "all_unsinghitting_mvd(5, 'all_n5)" for searching through all
     cases for n=5. </li>
     <li> And then applying e.g.
     "d_all_n5 : analyse_isorepo_defset_mvd(all_n5)"
     to get the sharp cases obtained. </li>
     <li> While by "d_all_n5 : analyse_isorepo_defset_imprmvd(all_n5)"
     we get the cases which improve upon the catalogue; this belongs
     more to the "deficiency-oriented investigations" below. </li>
     <li> However in this way it seems very unlikely to get sharp
     examples for deficiencies much smaller than the gap-deficiencies. </li>
     <li> While determining the possible distribution of clause-lengths
     due to the condition sum_C 2^(-|C|) = 1 by all_cld_uhit_maxminvd
     and then either determining that there are no examples for the given
     n, or constructing them by hand seems to be more succesful. </li>
     <li> See "all_cld_uhit_minvd" in
     ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp
     for further strengthenings of this approach (especially regarding
     the use of (generalised) SAT solving). </li>
     <li> We know that the bound max_min_var_degree_def is sharp
     for given n and deficiencies k = 2^n-n - i for 0 <= i <= n. </li>
     <li> For n <= 3 this cannot be applied fully, and there we also
     have full information. </li>
     <li> For n=4 we obtain k = 12, 11,10,9,8.
      <ul>
       <li> And actually cases for k=7 are known where the bound is sharp.
       </li>
       <li> While this cannot be the case for k = 6. </li>
       <li> And also not for k = 5. </li>
       <li> But we have again sharpness for k=3,4. </li>
      </ul>
      This information is complete. </li>
     </li>
     <li> n = 5:
      <ol>
       <li> The general cases for sharpness are k = 27, 26,25,24,23,22. </li>
       <li> Also for k = 18,19,20,21 we have sharp cases. </li>
       <li> One could conjecture that there is no sharp case for k <= 17,
       however k=4 below is a counter-example. </li>
       <li> For k = 17 we have all_cld_uhit_maxminvd(k,5) =
       {{[2,1],[3,1],[5,20]}}, thus there exists a variable not
       occurring in the first two clauses, and thus occurring only
       22 - 2 = 20 < 21 times, so here there is no sharp case. </li>
       <li> For k = 13,14,15,16 due to all_cld_uhit_maxminvd(k,5) = {}
       we know there are no sharp cases. </li>
       <li> For k = 12 we have all_cld_uhit_maxminvd(k,5) =
       {{[1,1],[5,16]}}, and so there is no non-singular sharp case. </li>
       <li> k = 11 ? </li>
       <li> k = 10 ? </li>
       <li> k = 9 ? </li>
       <li> k = 8 ? </li>
       <li> k = 7 ? </li>
       <li> For k = 6 we have the only the non-singular case
       {{[2,3],[5,8]}}, and so there is a variable not occurring in the
       first three clauses, thus occurring 11 - 3 = 8 < 9 time, so
       here is no sharp case. </li>
       <li> For k = 5 the only non-singular case is {[2,3],[4,1],[5,6]}},
       so again in variable is not occurring in the first three clauses,
       thus occurring 10 - 3 = 7 < 8 times, so no sharp case here. </li>
       <li> For k = 4 we have a sharp case. </li>
       <li> For k = 3 the non-singular cases are
        <ul>
         <li> {[2,1],[3,5],[4,2]} ? </li>
         <li> {[2,2],[3,2],[4,4]} ? </li>
         <li> {[2,2],[3,3],[4,1],[5,2]} ? </li>
         <li> {[2,3],[3,1],[5,4]} ? </li>
         <li> {[2,3],[4,3],[5,2]} ? </li>
        </ul>
       </li>
      </ol>
     </li>
     <li> For n=6 we only have incomplete information.
      <ol>
       <li> The general cases for sharpness are k = 58, 56,55,54,53,52,51.
       </li>
       <li> And then we have sporadic cases for k = 43. </li>
       <li> We should also get this information into the catalogue,
       but likely only optional. </li>
      </ol>
     </li>
      <li> For n=7 we only have incomplete information.
      <ol>
       <li> The general cases for sharpness are
       k = 121, 120,119,118,117,116,115,114. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Deficiency-oriented investigations:
    <ol>
     <li> max_min_var_deg_uhit_def(k) yields the maximal min-var-deg
     realised in the uhit_def-catalogue for deficiency k. </li>
     <li> We have sharpness w.r.t. the bound for k in
     {2, 3,4,5, 7, 8,9,10,11,12, 18,19,20,21, 22,23,24,25,26,27,
     43, 51,52,53,54,55,56,58},
     </li>
     <li> So one could conjecture that always directly after the "gap"
     we have an interval where the bound is not sharp. </li>
     <li> To obtain a sharp bound for k=6, n >= 6 is
     necessary. </li>
     <li> To obtain a sharp bound for k in {13,...,17},
     n >= 6 is necessary. </li>
     <li> For the other deficiencies we have the maxima:
      <ul>
       <li> k = 6 : 8 = 9-1 </li>
       <li> k = 13 : 16 = 17-1 </li>
       <li> k = 14 : 17 = 18-1 </li>
       <li> k = 15 : 18 = 19-1 </li>
       <li> k = 16 : 19 = 20-1 </li>
       <li> k = 17 : 20 = 21-1 </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>

*/
