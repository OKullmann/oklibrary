// Oliver Kullmann, 27.1.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Autarkies/plans/general.hpp
  \brief Plans for Maxima-components regarding autarkies


  \todo Create milestones


  \todo Write tests


  \todo Write docus


  \todo Translation to SAT according to Liffiton and Sakallah
  <ul>
   <li> Implement the back-translation, satisfying assignments
   to autarkies. </li>
  </ul>


  \todo Basic translation
  <ul>
   <li> Implement the basic translation, which just translates
   the boolean F into a G with variables over {-1,0,1}. </li>
  </ul>


  \todo Lean kernel via variable-elimination
  <ul>
   <li> Implement the computation of the lean kernel via a SAT-solver,
   which in the unsat-case returns a set of variables used by some
   resolution refutation. </li>
  </ul>


  \todo Matching autarkies
  <ul>
   <li> Finding some matching autarky. </li>
   <li> Computing the matching-lean kernel. </li>
  </ul>


  \todo Linear autarkies


  \todo Balanced autarkies


  \todo Pure autarkies


  \todo Autarky search via running through all total assignments


  \todo Implement the translation of USAT to LEAN according the our
  article with Victor and Mirek


  \todo The autarky monoid
  <ul>
   <li> Implement the full representation, all essential autarkies
   together with the composition of partial assignments. </li>
   <li> Is there some computer-algebra-support for representing
   semigroups (and monoids)? </li>
  </ul>

*/

