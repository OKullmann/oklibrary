// Oliver Kullmann, 21.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
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


  \todo Finding matching autarkies


  \todo Deciding matching leanness
  <ul>
   <li> Using the definition (no matching autarky exists). </li>
   <li> Using the surplus. </li>
   <ol> DONE Using the criterion, that every strict subset has a smaller
   deficiency. </li>
  </ul>


  \todo Computing the matching-lean kernel
  <ul>
   <li> DONE
   Implement brute-force search. </li>
   <li> Do we have special properties regarding generation? </li>
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

