// Oliver Kullmann, 15.4.2011 (Guangzhou)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/Basics.hpp
  \brief Plans for basic Maxima-components regarding minimally unsatisfiable clause-sets


  \todo Maximal min-var-degrees
  <ul>
   <li> The functions should go to a dedicated file:
    <ol>
     <li> DONE (renamed to MaxVarDegrees.hpp/mac)
     There is confusion with
     MinimalUnsatisfiability/plans/SmallVariableDegrees.hpp. </li>
    </ol>
   </li>
   <li> The general quest here is for example which show that the bound
   nonmersenne is sharp. </li>
   <li> See Experimentation/Investigations/plans/MaximiseMinVarDegrees.hpp
   for the general investigation. </li>
   <li> Perhaps the considerations regarding hitting clause-sets should go
   to a module regarding hitting clause-sets ?
    <ol>
     <li> The smallest deficiency where we do not have an example is 6. </li>
     <li> See "derived_hitting_cs_pred_isoelim" in
     ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp
     for how to find examples. </li>
    </ol>
   </li>
  </ul>


*/

