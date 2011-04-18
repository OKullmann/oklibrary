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
     <li> weak_php_fcs(n+1,n) </li>
    </ol>
   </li>
  </ul>

*/

