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
   <li> Is the new scheme (using in functions "php_var(i,j)" instead
   of "php(i,j)", where php_var(i,j) := nounify(php)(i,j), now right?
   Why didn't the old scheme work?? (It kept a memory for the created
   symbols about their place of creation!!) Why is nounify(php) needed,
   where php is already a "noun" ?? </li>
   <li> Given a formal clause-set FF, how to create *new* variables?
    <ol>
     <li> The need arises in constructions. </li>
     <li> Perhaps for each construction a new type of variable is introduced,
     like "php" for the pigeonhole formulas. </li>
     <li> Transformers which use a given clause-set F should at least
     check that F doesn't contain the dedicated "new" variables --- otherwise
     perhaps the computation is just aborted with an error message. </li>
     <li> This test can be performed with e.g. <code>freeof(php,F)</code>.
     </li>
    </ol>
   </li>
   <li> DONE It seems that "declare(php, noun)" cannot be used twice, and thus
   we cannot reload a file containing such a declaration?? Is just to kill
   "php" before the solution? </li>
  </ul>


  \todo Sudoku
  <ul>
   <li> Compare with Applications/LatinSquares/plans/Sudoku.hpp. </li>
   <li> We need an easy method for specifying partial assignments to
   fields. </li>
   <li> One should try whether using "create_list" in sdk_different_boxes,
   without storing the intermediate value, really slows down the computation
   (possibly the optimisation is detected?). </li>
   <li> For the creation of random Sudoku-problems the easiest approach
   seems to fix the number f of fixed fields, and then choose f random
   fields from the N^2 fields, and for each chosen field choose a random
   value from {1,...,N}. </li>
   <li> Such random Sudoku-problems should also undergo a phase transition
   (for fixed p and appropriate f) ?! </li>
   <li> We also need generators for Latin square completion problems,
   etc. Compare Applications/LatinSquares/plans/general.hpp. </li>
   <li> Add the conditions, that a field doesn't git several colours.
   DONE. </li>
   <li> DONE (except of sdk_different_boxes, where it doesn't seem possible
   to store an intermediate value)
   Replace, if possible, all loops by the use of "create_list". </li>
  </ul>


  \todo Translations to MUSAT
  <ul>
   <li> Implement the other elements of the reduction. </li>
   <li> DONE Implement the basic UNSAT-MUSAT reduction from
   [Papadimitriou, Wolfe; 1988]. </li>
  </ul>

*/

