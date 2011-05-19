// Oliver Kullmann, 17.4.2011 (Guangzhou)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/MinVarDegrees.hpp
  \brief Plans regarding the minimal variable-degree of minimally unsatisfiable clause-sets

  <ul>
   <li> We consider upper bounds (functions nonmersenne in various forms, and
   strengthenings). </li>
   <li> We consider lower bounds (like Sma_S2 in various forms). </li>
   <li> And we provide examples showing that the upper bounds are sharp
   (or not). </li>
   <li> Besides the maximal min-var-degree for minimally unsatisfiabible
   clause-sets of fixed deficiency or sub-classess, we consider related
   notions like the maximal number of full-clauses (for the classes). </li>
  </ul>


  \todo Organisation
  <ul>
   <li> The functions should go to a dedicated file:
    <ol>
     <li> Perhaps we should have a submodule MaxVarDegrees. </li>
     <li> With files FullClauses.mac, Degrees.mac, NonMersenne.mac. </li>
     <li> And perhaps the functionality related to the min-var-degree in
     ConflictCombinatorics/HittingClauseSets.mac should be moved there,
     yielding a file HittingClauseSets.mac. </li>
     <li> DONE (renamed to MaxVarDegrees.hpp/mac)
     There is confusion with
     MinimalUnsatisfiability/plans/SmallVariableDegrees.hpp. </li>
    </ol>
   </li>
   <li> See Experimentation/Investigations/plans/MaximiseMinVarDegrees.hpp
   for the general investigation. </li>
   <li> Perhaps the considerations regarding hitting clause-sets should go
   to a module regarding hitting clause-sets ?
    <ol>
     <li> The conjecture is that the maximal min-var-degree for a fixed
     deficiency is the same for all of MU and for the subsets of
     (unsatisfiable) hitting clause-sets. </li>
     <li> See "derived_hitting_cs_pred_isoelim" in
     ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp
     for how to find examples. </li>
     <li> However we already have the uhit_def-catalogue here. </li>
     <li> Above we actually propose to move the functionality from
     ConflictCombinatorics here (that is, to a submodule). </li>
    </ol>
   </li>
   <li> Naming of bounds-functions:
    <ol>
     <li> We have fullclauses_dmu and minvardegree_dmu (for all of MU). </li>
     <li> In ConflictCombinatorics/HittingClauseSets.mac we have the function
     max_min_var_deg_uhit_def, which considers just the uhit_def-catalogue.
     </li>
     <li> Perhaps this function should be renamed to minvardegree_uhit_def.
     </li>
     <li> And we should introduce a bounds-function minvardegree_duhit. </li>
    </ol>
   </li>
  </ul>

*/

