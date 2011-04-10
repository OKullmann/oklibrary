// Oliver Kullmann, 19.2.2008 (Swansea)
/* Copyright 2008, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/general.hpp
  \brief Plans for Maxima-components regarding minimally unsatisfiable clause-sets


  \todo Create milestones


  \todo Create tests


  \todo Connections to other modules
  <ul>
   <li> In
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/HittingClauseSets.mac
   we have 2 generators for elements of SMUSAT(1). </li>
   <li> DONE (moved to
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/Transformations.mac)
   Further generators in
   ComputerAlgebra/Satisfiability/Lisp/Generators/Generators.mac. </li>
   <li> See Experimentation/Investigations/plans/general.hpp. </li>
  </ul>


  \todo Overview on generators for MUSAT
  <ul>
   <li> We need an overview on all possibilities to create elements of
   MUSAT. </li>
   <li> In Satisfiability/Lisp/MinimalUnsatisfiability/Transformations.mac we
   have
    <ol>
     <li> usat_musat(FF) for unsatisfiable FF </li>
     <li> sat_musat(FF) for satisfiable FF </li>
    </ol>
   </li>
   <li> In ComputerAlgebra/Satisfiability/Lisp/Generators/Generators.mac
   we have, besides the hitting clause-sets:
    <ol>
     <li> weak_ph(n+1,n) </li>
    </ol>
   </li>
  </ul>


  \todo Maximal min-var-degrees
  <ul>
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

