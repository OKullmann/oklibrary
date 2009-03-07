// Oliver Kullmann, 21.3.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Autarkies/plans/MatchingAutarkies.hpp
  \brief Plans for Maxima-components regarding matching autarkies


  \todo Connections to other modules
  <ul>
   <li> See Algorithms/Autarkies/MatchingAutarkies/plans/general.hpp. </li>
  </ul>


  \todo Matching satisfiability
  <ul>
   <li> Deciding matching satisfiability of F likely happens most efficiently
   by the criterion delta^*(F) = 0. </li>
   <li> A matching satisfying assignment can then be simple read off a maximum
   matching in the clause-variable graph. </li>
  </ul>


  \todo Deciding matching leanness
  <ul>
   <li> Using the definition (no matching autarky exists). </li>
   <li> Using the surplus. </li>
   <ol> DONE Using the criterion, that every strict subset has a smaller
   deficiency. </li>
  </ul>


  \todo Finding matching autarkies
  <ul>
   <li> DONE Simple enumeration. </li>
  </ul>


  \todo Computing the matching-lean kernel
  <ul>
   <li> DONE
   Implement brute-force search. </li>
   <li> Apply the general procedure (see "Lean kernel via oracle for
   leanness-decision" in
   ComputerAlgebra/Satisfiability/Lisp/Autarkies/plans/LeanKernel.hpp),
   based on deciding matching leanness. </li>
   <li> Can the characterisation of the matching-lean kernel as the smallest
   tight sub-clause-set be exploited (using matching techniques)? </li>
  </ul>


  \todo Computing a quasi-maximal matching autarky
  <ul>
   <li> This is a matching autarky yielding the matching-lean kernel. </li>
   <li> It can be obtained by iteratively computing some non-trivial matching
   autarky until none exists anymore, and then taking the composition. </li>
   <li> Alternatively one first computes the matching-lean kernel, crosses
   the variables in it out and removes the arising empty clause, and computes
   a matching-satisfying assignment for the resulting clause-set. </li>
  </ul>


  \todo Surplus
  <ul>
   <li> Compute the surplus. </li>
   <li> Implement the related strengthening of matching
   autarkies (see [Kullmann, CSR 12-2007], Lemma 4.32). </li>
   <li> Implement the reduction r from Corollary 4.31 in
   [Kullmann, CSR 12-2007]. </li>
   <li> Implement the reduction S from Lemma 4.33 in [Kullmann, CSR 12-2007].
   </li>
  </ul>

*/

