// Oliver Kullmann, 27.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/PrimeImplicatesImplicants.hpp
  \brief Plans for Maxima-components regarding computation of prime implicates/implicants of boolean functions and clause-sets
  
  
  \todo min_2resolution_closure_cs
  <ul>
   <li> Currently the name likely is not corrected, since if the input is not
   a full clause-set, then likely we do not compute the min-closure under
   2-subsumption resolution (example?). </li>
  </ul>


  \todo Dualisation
  <ul>
   <li> Implement the algorithm given by "dual_cs" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Constructions.mac
   </li>
   <li> Or sh0uld this go somewhere else?? </li>
  </ul>


  \todo Connections
  <ul>
   <li> The above dualisation yields a representation of all
   satisfying assignments:
    <ol>
     <li> See
     ComputerAlgebra/Satisfiability/Lisp/Counting/plans/general.hpp
     </li>
     <li> See AllSolutions/plans/general.hpp, and especially
     AllSolutions/plans/MinimalAssignments.hpp. </li>
    </ol>
   </li>
  </ul>

*/

