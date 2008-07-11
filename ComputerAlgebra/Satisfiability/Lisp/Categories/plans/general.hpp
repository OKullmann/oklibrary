// Oliver Kullmann, 11.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Categories/plans/general.hpp
  \brief General plans for categories of clause-sets


  \todo Relations to other modules
  <ul>
   <li> Symmetries (see
   ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp)
   specifically handles isomorphisms. </li>
   <li> A general module ComputerAlgebra/Categories should handle
   some general aspects. </li>
  </ul>


  \todo Fundamental notions
  <ul>
   <li> Likely homomorphisms are special cases of substitutions; see
   "Applying substitutions" in
   ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp. </li>
   <li> A morphism f: F -> G would then be a subsitution which maps the
   literals from F to the literals from G, and which translates the clauses
   of F into the clauses of G. </li>
   <li> Abbreviations like "morcls" (morphism for clause-sets). </li>
  </ul>

*/

