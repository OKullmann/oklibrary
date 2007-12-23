// Oliver Kullmann, 23.12.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Generators.hpp
  \brief Plans for Maxima-generators for clause-sets


  \todo Random generator
  <ul>
   <li> Using the maxima-aes-implementation, implement the random generator
   as in Experimentation/RandomGenerator/plans/general.hpp. </li>
  </ul>


  \todo Variables
  <ul>
   <li> Given a formal clause-set FF, how to create *new* variables? </li>
   <li> DONE It seems that "declare(php, noun)" cannot be used twice, and thus
   we cannot reload a file containing such a declaration?? Is just to kill
   "php" before the solution? </li>
  </ul>


  \todo Sudoku
  <ul>
   <li> Compare with Applications/LatinSquares/plans/Sudoku.hpp. </li>
   <li> DONE (except of sdk_different_boxes, where it doesn't seem possible
   to store an intermediate value)
   Replace, if possible, all loops by the use of "create_list". </li>
  </ul>

*/

