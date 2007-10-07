// Oliver Kullmann, 29.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/plans/MinimalSatisfyingAssignments.hpp
  \brief Plans for computing the set of minimal satisfying assignments


  \todo Implement the trivial algorithm
  <ul>
   <li> The algorithm is as follows:
    <ol>
     <li> Input is a set F of conditions. </li>
     <li> Split F into F_1, F_2. </li>
     <li> Solve (recursively) F_1, F_2. </li>
     <li> Compute all consistent compositions. </li>
     <li> Perform subsumption-elimination. </li>
    </ol>
  </ul>

*/

