/* Oliver Kullmann, 3.2.2008 (Swansea) */
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Counting/plans/general.hpp
  \brief General plans for counting satisfying assignments


  \todo Create milestones.


  \todo Write the direct algorithm (running through all total assignments)
  <ul>
   <li> Perhaps in "Counting/Basics.mac". </li>
  </ul>


  \todo Write a scheme which uses r_k-splitting trees


  \todo okltest_satprob
  <ul>
   <li> We should perhaps create Counting/tests/Basics.mac, and move this
   general testfunction there. </li>
   <li> DONE
   This test-function should be a general test function for the counting
   of satisfiying assignments of boolean clause-sets. </li>
  </ul>


  \todo satprob_mcind
  <ul>
   <li> See "Conflicts.mac" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/general.hpp. </li>
   <li> Given an upper bound on the independence number, compute the hypergraph
   of independent subsets more directly. </li>
  </ul>


  \todo satprob_incexl_hitting
  <ul>
   <li> Similar to satprob_mcind, compute approximations. </li>
   <li> One would assume, that this computation is always less efficient
   than satprob_mcind? </li>
  </ul>


  \todo Connect to the counting-module in part Satisfiability.

*/

