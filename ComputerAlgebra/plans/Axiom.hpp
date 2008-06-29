// Oliver Kullmann, 29.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/plans/Axiom.hpp
  \brief General plans regarding the computer algebra system Axiom (and SAT)


  \todo Naming conventions
  <ul>
   <li> How to postfix Axiom-files? </li>
   <li> How to postfix Aldor-files? </li>
  </ul>
  

  \todo Aldor integration
  <ul>
   <li> Literature on Aldor? </li>
   <li> How to integrate Axiom with Aldor? </li>
   <li> We should only write Aldor-code, using the Axiom-library ---
   how to do this? </li>
  </ul>

  
  \todo Test system
  <ul>
   <li> We need a test system similar to the C++ and the Maxima system. </li>
   <li> Likely Aldor and/or Axiom has a test system? </li>
  </ul>


  \todo Documenting the code
  <ul>
   <li> Can we instrument Doxygen to handle Aldor/Axiom code? </li>
   <li> Are there special methods for creating documentation? </li>
  </ul>


  \todo Input checking
  <ul>
   <li> Axiom/Aldor has a strong type system, so automatically inputs
   experience stronger checks than at the Maxima/Lisp level. </li>
   <li> The axioms include semantic checks: Does this mean that inputs
   are automatically fully validated? (Always?) </li>
   <li> Perhaps here, at this planning/prototyping level, it would be
   appropriate to have always all validation applied. </li>
  </ul>

*/

