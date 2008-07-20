// Oliver Kullmann, 23.12.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/general.hpp
  \brief Plans for Maxima-generators for clause-sets


  \todo Split Generators/Generators.mac
  <ul>
   <li> Pigeonhole formulas </li>
   <li> %Ramsey problems </li>
   <li> Special minimally unsatisfiable clause-sets
    <ol>
     <li> The documentation should mention random_splitting_mus in
     ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/SplittingAnalysis.mac
     </li>
    </ol>
   </li>
   <li> The translations are not generators, and should go somewhere else.
    <ol>
     <li> Perhaps to modules related to the problems they solve (by
     reduction). </li>
     <li> Or as kind of transformer-generators to the module where the
     transformed problem is solved. </li>
     <li> In any case, links should be provided. </li>
    </ol>
   </li>
   <li> Sudoku-problems : DONE </li>
  </ul>


  \todo Accompanying statistics
  <ul>
   <li> We need to establish the scheme how to supply measurements for
   generators (i.e., computing measures without creating the problems). </li>
  </ul>


  \todo %Variables
  <ul>
   <li> Compare "Global variables" in
   ComputerAlgebra/plans/Maxima.hpp. </li>
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


  \todo Colouring problems
  <ul>
   <li> Translate graph colouring problems into SAT. </li>
   <li> Translate hypergraph colouring problems (strong and weak) into
   SAT.
    <ol>
     <li> Strong colouring problems are already handled in
     ComputerAlgebra/Hypergraphs/Lisp/Colouring.mac. </li>
     <li> DONE (see tcol2sat_stdohg2stdofcs in
     ComputerAlgebra/Hypergraphs/Lisp/Colouring.mac)
     We also need the ability to handle standardised hypergraphs,
     yielding standardised clause-sets. </li>
    </ol>
   </li>
  </ul>


  \todo Ramsey problems
  <ul>
   <li> One needs to think about the extreme cases; it seems there are
   inconsistencies.
    <ol>
     <li> Compare "Ramsey graphs" in
     ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
    </ol>
   </li>
   <li> For larger Ramsey problems, we need the ability to compute directly
   the standardised clause-set (which is just the hypergraph edges in two
   versions, positive (as they are) and negative). </li>
   <li> Compare "Colouring problems" above. </li>
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
   <li> We need to establish links in the docus to the generators mentioned
   below from other modules. </li>
   <li> DONE (see ComputerAlgebra/Trees/Lisp/Basics.mac)
   The basic question is how to represent trees. </li>
   <li> DONE (see
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/HittingClauseSets.mac)
   And we need two variants: One where the inner nodes are
   labelled with variables, and one without (here standard variables
   are supplied). </li>
   <li> DONE The construction works also in case of "regular variable
   labellings", where variables may occur at many places, but only
   once on a path. </li>
   <li> DONE In this general form we get exactly the "tree hitting clause-sets".
   </li>
   <li> DONE (see
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/general.hpp
   and
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/DeficiencyOne.mac)
   How to get the MUSAT(1)-clause-sets from SMUSAT(1) ? </li>
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
   <li> Again the request to establish links. </li>
   <li> DONE (see
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Constructions.mac)
   Product and Sum </li>
   <li> Xorification [Buresh-Oppenheim, Pitassi; 2007] </li>
  </ul>


  \todo Hiding small unsatisfiable clause-sets
  <ul>
   <li> In [Ouyang, 1999, Implementations of the DPLL algorithm] an
   MUSAT(1) clause-set with small variable degree (see
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/DeficiencyOne.hpp)
   is hidden inside a "misleading" combination of other clauses (variable-
   disjoint). We should implement these clause-sets. </li>
  </ul>
  
*/

