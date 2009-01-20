// Oliver Kullmann, 23.12.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
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
   <li> DONE %Ramsey problems </li>
   <li> Special minimally unsatisfiable clause-sets
    <ol>
     <li> The documentation should mention random_splitting_mus in
     ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/SplittingAnalysis.mac
     </li>
    </ol>
   </li> DONE (moved to Satisfiability/Lisp/MinimalUnsatisfiability)
   <li>
   The translations are not generators, and should go somewhere else.
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


  \todo Write basic docus
  <ul>
   <li> We need basic docus for all generators. </li>
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


  \todo Generalised colouring problems
  <ul>
   <li> For non-diagonal Ramsey-problems and van-der-Waerden-type problems,
   we do not need to go directly to non-boolean clause-sets, but we can
   use an intermediate form, with "mixed clauses", as discussed in the
   following. </li>
   <li> The problem would be given as a triple [V,C,L], where V is a set
   (the vertices), C is a set (the colours), and each L is a pair [H,c],
   where H is a set of hyperedges on V (i.e., [V,H] is a hypergraph), while
   c is an element of C. </li>
   <li> The problem is to find a map f: V -> C such that for every [H,c]
   and every E in H there exists v in E with f(v) <> c. </li>
   <li> A standard hypergraph colouring problem with colour set C for
   hypergraph [V,E] is translated in [V,C,L], where L consists of the
   pairs [E,c] for c in C. </li>
   <li> Compared with non-boolean clause-sets (clause-sets, where the
   clauses contain literals [v,c]), in this way we express the information
   that the clause-set is organised in layers. </li>
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

