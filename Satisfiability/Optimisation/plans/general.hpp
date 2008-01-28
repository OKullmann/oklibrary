// Oliver Kullmann, 22.9.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Optimisation/plans/general.hpp
  \brief General plans on optimisation variants of (generalised) satisfiability


  \todo Update namespaces.


  \todo Create module MinOnes (or "WeightedSAT" ?)


  \todo Create module MinSAT.
  <ul>
   <li> This is about finding a satisfying partial assignment
   using a minimal number of variables, a natural
   generalisation of finding a minimum size transversal
   of a hypergraph. </li>
   <li> The parameterised case is discussed in
   AllSolutions/plans/MinimalAssignments.hpp ---
   should this move to this new module? </li>
   <li> Or does this module actually belong to
   Satisfiability/Algorithms/AllSolutions ? </li>
   <li> It seems that the minimisation problem is different
   also from the parameterised version, and thus
   perhaps the two different modules are justifed:
   Though the minimisation problem can be solved by running
   through the parameter levels, there are many other
   possibilities (for example regarding approximation). </li>
   <li> So perhaps also for finding a transversal of minimum
   size we need a module? </li>
  </ul>


  \todo Create module MaxSat.

*/

