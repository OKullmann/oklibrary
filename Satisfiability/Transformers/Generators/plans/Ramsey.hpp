// Matthew Gwynne, 11.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/plans/Ramsey.hpp
  \brief Plans for generators for %Ramsey problems


  \todo Extend generator to generate non-diagonal %Ramsey instances
  <ul>
   <li> While the current %Ramsey generator (see Ramsey.cpp) allows
   non-diagonal arguments (q1 and q2) for the problem  "ramsey_2^2(q1,q2) < n?",
   currently only the first argument is considered (that is, we have 
   "ramsey_2^2(q,q) < n?". </li>
   <li> Some %Ramsey numbers such as ramsey_2^2(4,6) have small bounds (35-41) 
   but are still unknown, making them potentially easy targets for a SAT solver
   to find new bounds. </li>
   <li> The generator should be extended to accommodate non-diagonal instances.
   </li>
  </ul>


  \todo Generator should produce standardised variables using colexicographical 
  ordering
  <ul>
   <li> The C++ %Ramsey generator (see Ramsey.cpp) should use the same variable
   names as in 
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyProblems.mac . </li>
   <li> This would allow simple combination of these generated clause-sets with
   symmetry breaking techniques etc. </li>
  </ul>


  \todo Add generator to generate hypergraph %Ramsey problems
  <ul>
   <li> Currently the C++ %Ramsey generator (see Ramsey.cpp) only generates 
   %Ramsey problem instances where the hyperedge length is 2. </li>
   <li> A new generator should be written which generates CNF representations
   for %Ramsey problem instances of the form "ramsey_s^r(q_1,...,q_s) < n?". 
   </li>
   <li> See 
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyProblems.mac for how to 
   handle the translation of the non-boolean nature of the problem when 
   considering %Ramsey problems with more than 2 labels. </li>
  </ul>

*/

