// Oliver Kullmann, 12.1.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/general.hpp
  \brief Plans for Maxima-components regarding propositional logic


  \todo DONE ("not" is now only used for logical evaluation at the
  Maxima-level, and replaced for literals by "neg")
  MAXIMA BUG
  <ul>
   <li> The code
   \verbatim
not f([1]);
   \endverbatim
   produces
   \verbatim
Maxima encountered a Lisp error:
*** - Lisp stack overflow. RESET
   \endverbatim
   </li>
  </ul>


  \todo Create milestones.


  \todo Simplifications
  <ul>
   <li> Discuss basic_simplification_ft. </li>
   <li> Write merging-transformation (eliminating chains). </li>
   <li> Contract repeated subtrees at a node. </li>
  </ul>

*/
