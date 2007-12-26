// Oliver Kullmann, 29.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/plans/Literals.hpp
  \brief Plans for literals ("concrete")


  \todo Boolean literals, and more general literals
  <ul>
   <li> We have a notion of "variables" and "boolean literals" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac;
   seems sufficient, and needs documentation. </li>
   <li> A "signed literal" is just a pair [v,S], where S is a set
   of values. But we should allow the possibility that S is interpreted
   positive (for "DNF") or negative (for "CNF"). And also the singleton
   should get some extra syntax. </li>
   <li> Perhaps there is a third entry? With tags? </li>
   <li> We need to provide the standard promotion of literals (to more
   general literals). </li>
   <li> Compare "Domain association and allowed total assignments"
   in ComputerAlgebra/Satisfiability/Lisp/plans/SatisfactionProblems.hpp. </li>
  </ul>


  \todo Signed literals as conditions

*/

