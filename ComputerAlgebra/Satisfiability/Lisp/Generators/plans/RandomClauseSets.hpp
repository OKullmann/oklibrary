// Oliver Kullmann, 6.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/RandomClauseSets.hpp
  \brief Plans for generators for (various types of) random clause-sets


  \todo Random subsets, sublists etc.
  <ul>
   <li> In
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/InverseSingularDP.mac
   we have "random_sublist". </li>
   <li> And in ComputerAlgebra/Satisfiability/Lisp/Generators/RandomClauseSets.mac
   we have "random_sublist_l". </li>
   <li> Likely these functions should go to DataStructures/Lists.mac.
   </li>
  </ul>
  
  
  \todo AES random generator
  <ul>
   <li> Using our maxima-aes-implementation, implement the random generator
   as in Experimentation/RandomGenerator/plans/general.hpp. </li>
  </ul>


  \todo Filtering out
  <ul>
   <li> A natural extensions of random generators is by applying some filter,
   removing for example instances with repeated clauses. </li>
   <li> We need a framework for this. </li>
  </ul>

*/

