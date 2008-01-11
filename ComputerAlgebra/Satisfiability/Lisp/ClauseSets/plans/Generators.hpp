// Oliver Kullmann, 23.12.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
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
   value from {1,...,N}. In other words, using the natural notion of
   (non-boolean) literals a random problem is just given by a random
   partial assignment with a fixed number of f variables.
    <ol>
     <li> Such random Sudoku-problems should also undergo a phase transition
     (for fixed p and appropriate f) ?! </li>
     <li> Creation of the partial assignment (with non-boolean variables)
     simply as a random clause by the OKgenerator. </li>
     <li> For the analysis one can filter out partial assignments which
     are not r_k-consistent (so r_0-consistency just means that the
     partial assignment does not (directly) falsify a clause); no
     consistency check then would be "-1-consistency". </li>
     <li> To speed up the creation, filtering out of r_k-inconsistent
     partial assignments can be done already during the creation (without
     disturbing the probability distribution). </li>
     <li> However, even r_1-reduction for box-dimension 2 takes a long
     time in Maxima (90 s). Thus this filtering cannot be done in Maxima,
     but we need C++ components. </li>
    </ol>
   </li>
   <li> We also need generators for Latin square completion problems,
   etc. Compare Applications/LatinSquares/plans/general.hpp. </li>
   <li> Can something be done about
   \verbatim
(%i184) output_strong_sdk(6,"~/sdk_s_6.cnf");
Maxima encountered a Lisp error:
*** - Program stack overflow. RESET
[1]>
[CTRL D]
*** - handle_fault error2 ! address = 0x80a30f2a not in [0x4e0dcbf0,0x68095000) !
SIGSEGV cannot be cured. Fault address = 0x80a30f2a.
Permanently allocated: 92096 bytes.
Currently in use: 673594256 bytes.
Free space: 16453216 bytes.
   \endverbatim
   ? Try it on cs-wsok. </li>
   <li> Surjectivity constraints: DONE
    <ol>
     <li> For rows, columns, and boxes translate the surjectivity
     constraints. </li>
     <li> Create a "dual weak sdk". </li>
     <li> The union of weak sdk and dual weak sdk then deserves the
     name "strong sdk". </li>
    </ol>
   </li>
   <li> Add the conditions, that a field doesn't get several numbers.
   DONE. </li>
   <li> DONE (except of sdk_different_boxes, where it doesn't seem possible
   to store an intermediate value)
   Replace, if possible, all loops by the use of "create_list". </li>
  </ul>
   
   
  \todo Colouring problems
  <ul>
   <li> Translate graph colouring problems into SAT. </li>
   <li> Translate hypergraph colouring problems (strong and weak) into
   SAT. </li>
  </ul>

  \todo Translations to MUSAT
  <ul>
   <li> Implement the other elements of the reduction.
    <ol>
     <li> The reduction of SAT to MUSAT. DONE </li>
     <li> The product-construction. </li>
    </ol>
   </li>
   <li> DONE Implement the basic UNSAT-MUSAT reduction from
   [Papadimitriou, Wolfe; 1988]. </li>
  </ul>


  \todo SMUSAT(1) and tree hitting clause-sets
  <ul>
   <li> The basic question is how to represent trees. </li>
   <li> And we need two variants: One where the inner nodes are
   labelled with variables, and one without (here standard variables
   are supplied). </li>
   <li> The construction works also in case of "regular variable
   labellings", where variables may occur at many places, but only
   once on a path. </li>
   <li> In this general form we get exactly the "tree hitting clause-sets".
   </li>
   <li> How to get the MUSAT(1)-clause-sets from SMUSAT(1) ? </li>
  </ul>


  \todo Hard clause-sets (for resolution and refinements)
  <ul>
   <li> Tseitin's formulas </li>
   <li> IMP and IMP' [Buresh-Oppenheim, Pitassi; 2007] </li>
   <li> GT and GT' [Buresh-Oppenheim, Pitassi; 2007] </li>
   <li> SP and SP' [Buresh-Oppenheim, Pitassi; 2007] </li>
  </ul>


  \todo Operations (new clause-sets from old ones)
  <ul>
   <li> Product </li>
   <li> Sum </li>
   <li> Xorification [Buresh-Oppenheim, Pitassi; 2007] </li>
  </ul>
*/

