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
     <li> The main tool yet is e.g. "all_unsinghitting(5, 'all_n5)" and
     "all_unsinghitting_mvd(5, 'all_n5)" for searching through all
     cases for n=5. </li>
     <li> And then applying e.g.
     "d_all_n5 : analyse_isorepo_defset_mvd(all_n5)"
     to get the sharp cases obtained. </li>
     <li> While by "d_all_n5 : analyse_isorepo_defset_imprmvd(all_n5)"
     we get the cases which improve upon the catalogue; this belongs
     more to the "deficiency-oriented investigations" below. </li>
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
     <li> For n=5 we only have incomplete information.
      <ol>
       <li> The general cases for sharpness are k = 27, 26,25,24,23,22. </li>
       <li> No other sharp cases are known. </li>
      </ol>
     </li>
     <li> For n=6 we only have incomplete information.
      <ol>
       <li> The general cases for sharpness are k = 58, 56,55,54,53,52,51.
       </li>
       <li> And then we have sporadic cases for k = 43. </li>
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
     {2, 3,4,5, 7, 8,9,10,11,12, 20, 22,23,24,25,26,27, 43, 51,52,53,54,55,56,58},
     </li>
     <li> For the other deficiencies we have the maxima:
      <ul>
       <li> k = 6 : 8 = 9-1 </li>
       <li> k = 13 : 16 = 17-1 </li>
       <li> k = 14 : 16 = 18-2 </li>
       <li> k = 15 : 17 = 19-2 </li>
       <li> k = 16 : 19 = 20-1 </li>
       <li> k = 17 : 20 = 21-1 </li>
       <li> k = 18 : 21 = 22-1 </li>
       <li> k = 19 : 22 = 23-1 </li>
       <li> k = 21 : 24 = 25-1 </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>

*/
