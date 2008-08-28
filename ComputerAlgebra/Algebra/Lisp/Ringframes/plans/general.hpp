// Oliver Kullmann, 15.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Ringframes/plans/general.hpp
  \brief Plans in general regarding ringframes


  \todo Organisation
  <ul>
   <li> Move ComputerAlgebra/Algebra/Lisp/FiniteFields.mac to
   Ringframes/Fields. </li>
  </ul>


  \todo Write tests


  \todo Homomorphisms


  \todo Module "Operations"
  <ul>
   <li> Here operations *of* semirings are considered (while in module
   "Actions" a set acts *on* a semiring). </li>
   <li> A ringframe can operate on a groupoid as a homomorphism into the
   transformation-prering; these constitute "premodules". </li>
   <li> And a ringframe can operate on a commutative groupoid as a
   homomorphism into the endomorphism-semiring; these constitute "semimodules".
   </li>
   <li> We need a plan what could go into "Operations", what into "Premodules",
   what into "Semimodules", what into "Modules", what into "Vectorspaces".
   </li>
   <li> Since likely we want "Semimodules", "Modules" and "Vectorspaces",
   perhaps then we do not have "Operations", but only "Premodules" ? </li>
   <li> Examples:
    <ol>
     <li> "The natural operation of N" in
     ComputerAlgebra/Algebra/Lisp/Groupoids/Semigroups/plans/general.hpp. </li>
    </ol>
   </li>
  </ul>

*/

