// Oliver Kullmann, 23.12.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
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


  \todo Add tests for parity generators


  \todo Update milestones
  <ul>
   <li> Some general development strategies are to be developed. </li>
   <li> Perhaps we should have a sub-module "LatinSquares" (including
   Sudoku). </li>
   <li> What happened to
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/LinearInequality.hpp
   (mentioned in milestone 0.1.4) ? </li>
   <li> DONE
   Generators/plans/RamseyProblems.hpp needs to be re-thought and
   completely updated. </li>
   <li> DONE
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/HindmanProblems.hpp
   is not integrated at all. </li>
  </ul>


  \todo Accompanying statistics
  <ul>
   <li> We need to establish the scheme how to supply measurements for
   generators (i.e., computing measures without creating the problems). </li>
   <li> The scheme in ComputerAlgebra/Satisfiability/Lisp/Generators/Sudoku.mac
   is to use the respective statistics-function-names as prefixes. </li>
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
   <li> DONE
   It seems that "declare(php, noun)" cannot be used twice, and thus
   we cannot reload a file containing such a declaration?? Is just to kill
   "php" before the solution? </li>
  </ul>


  \todo Flood-it
  <ul>
   <li> Motivated by a talk at BCTCS 2010 (Markus Jalsenius). </li>
   <li> Their Internet resource is http://floodit.cs.bris.ac.uk . </li>
   <li> Rules:
    <ol>
     <li> The parameters are n (the dimension of the board) and c (the number
     of colours). </li>
     <li> Additionally we use k for the maximal number of moves. </li>
     <li> We have the graph G_{n,n}, the nxn grid, with vertex set
     {1,...,n}^2, and with the horizontal and vertical links (so the degree
     of a corner vertex is 2, of a border vertex is 3, and of an inner vertex
     is 4). </li>
     <li> Furthermore we have a map f_0: V -> {1, ..., c}. </li>
     <li> The moves will construct further maps
     f_1, ..., f_k: V -> {1,...,c}. </li>
     <li> For round 0 <= r <= k let C(r) be the connected component of
     (1,1), where all vertices w with f_r(w) # f_r((1,1)) have been
     removed. </li>
     <li> A move in round r in {1,...,k} now consists in choosing a colour i
     in {1,...,c}, which results in the colour of all vertices in C(r-1)
     being changed to i, that is, a transition f_{r-1} -> f_r is performed.
     </li>
     <li> The aim is to have all vertices having the same colour, that is,
     f_k is a constant function, which is equivalent to C(k) = V. </li>
    </ol>
   </li>
   <li> Translation to non-boolean SAT, encoding the round when a vertex
   has been reached:
    <ol>
     <li> Using variable m_1, ..., m_k, each of domain {1,...,c}, for the
     moves. </li>
     <li> And variables a_v for v in V, each of domain {0,...,k}, for the
     round at which a_v has been "reached" (that is, the minimal r such that
     v in C(r)). </li>
     <li> The whole problem is given by the set of conditions C(v,r) for
     vertices v in V and rounds r in {1,...,k}, where C(v,r) is true iff at
     least one neighbour of v has been reached at round r-1 and the initial
     colouring f_0(v) is equal to the value of m_r. </li>
     <li> (1,1) and neighbours of (1,1) are best handled separately. </li>
     <li> And likely one should explicitly state that once a node has been
     reached, it stays reached; this needs new variables (see the
     b-variables below). </li>
     <li> The domain of the condition C(v,r) is {m_r,a_v} plus a_w for the
     neighbours w of v. </li>
     <li> C(v,r) is a conjunction of c implications, each having as
     premisse that m_r = i for i in {1,...,c}, and having as conclusion
     that a_v has value r iff at least one of its neighbours of colour i
     (initially) has value r-1. </li>
     <li> This is not yet complete, since "reached stays reached" is not
     expressed. One could handle that by relaxing the condition "has value
     r-1" to "has value at most r-1". But this seems inefficient, since
     we should keep the domain of C(v,r) small. </li>
     <li> One could introduce new boolean variables b_{v,r} with the meaning
     that v has been reached in one of the rounds 1, ..., r, that is, v is
     equivalent to the conjunction of a_v=i for i in {1,...,r}. </li>
    </ol>
   </li>
   <li> MG had the idea to express the problem via an iterated condition
   system. </li>
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

