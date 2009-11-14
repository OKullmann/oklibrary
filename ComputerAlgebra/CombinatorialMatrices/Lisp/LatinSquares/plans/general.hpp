// Oliver Kullmann, 30.12.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/CombinatorialMatrices/Lisp/LatinSquares/plans/general.hpp
  \brief Plans regarding latin squares in general


  \todo Relations to other modules
  <ul>
   <li> See
   ComputerAlgebra/Algebra/Lisp/Groupoids/Quasigroups/plans/general.hpp. </li>
   <li> Compare with Applications/LatinSquares/plans/general.hpp. </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/LatinSquares.hpp.
   </li>
   <li> See the investigations in
   Experimentation/Investigations/LatinSquares/plans/general.hpp. </li>
   <li> See especially in
   Experimentation/Investigations/LatinSquares/plans/OrthogonalSquares.hpp.
   </li>
  </ul>


  \todo Organisation
  <ul>
   <li> The main question is why we have two modules, this one and
   Groupoids/Quasigroups ? </li>
   <li> Perhaps Sudoku (and related structures) are more naturally
   considered combinatorial, and thus belong to here, while all those
   algebraic considerations belong there. </li>
   <li> What about counting and enumeration? </li>
   <li> Since here as basic structure we have combinatorial matrices, one might
   consider as notion of isomorphism principal isotopism here (the natural
   notion of isomorphism for combinatorial matrices), while most other notions
   go to module Quasigroups ? </li>
   <li> Counting latin squares is the same as counting quasigroups, while
   counting reduced latin squares is the same as counting loops (unital
   quasigroups) with fixed neutral element (while for order n the total
   number of loops has to be multiplied with n, since every element could
   be the neutral element). </li>
  </ul>

*/

