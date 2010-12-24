// Oliver Kullmann, 27.3.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/TseitinTranslation.hpp
  \brief Plans for Maxima-components translating from propositional logic to CNF via the Tseitin translation (and variations)


  \todo Connections to other modules
  <ul>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/TseitinTranslation.hpp
   </li>
   <li> Compare with
   Satisfiability/Transformers/Generators/plans/TseitinTransformation.hpp.
   </li>
   <li> Likely most developments should take place in module
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions, while in this module
   only special considerations regarding formulas (not general circuits)
   and applications of the general methods take place. </li>
  </ul>


  \todo Implementing the Tseitin-transformation (with variations)
  <ul>
   <li> Given a propositional formula F built from negation, conjunction
   and disjunction (for simplicity we consider only binary operations
   here), by the process specified below, a refinement
   of the Tseitin-translation, we obtain a 3-CNF F' over the variables
   from F plus new auxiliary variable such that
   <ol>
    <li> every satisfying assignment for F can (easily) be extended
    to a satisfying assignment for F'; </li>
    <li> every satisfying assignment for F' is a satisfying assignment
     for F (when restricted to the variables in F). </li>
   </ol>
   </li>
   <li> The process F -> F' works, using auxiliary variables v(G) for
   subformulas G of F, by collecting clauses (to avoid renaming
   of existing variables, let v(a) = a for variables a from F):
    <ol>
     <li> For a sub-formula G of F, where G is of the form
     op G_1 ... G_n
     (for us n <= 2), with op one of negation, conjunction or disjunction
     (written for convenience in prefix form), add the clauses
     <dl>
      <dt>(i)</dt> (op v(G_1) ... v(G_n)) -> v(G) 
      <dt>(ii)</dt> v(G) -> (op v(G_1) ... v(G_n))
     </dl>
     where in case G occurs positively in F, (i) can be dropped,
     while if G occurs negatively in F, (ii) can be dropped
     (so the process might be non-deterministic).
     </li>
     <li> F' is obtained by adding the unit clause {v(F)} to all these
     clauses. </li>
    </ol>
   </li>
   <li> Is there a general input format for boolean formulas?
   See "General propositional formulas and circuits" in
   Interfaces/InputOutput/plans/general.hpp. </li>
   <li> Some obvious variations on the transformation F -> F':
    <ol>
     <li> handling of arbitrary and-or-arities </li>
     <li> performing simplifications on the clauses </li>
     <li> either using always (i)+(ii), or exactly one of them, or using some
     heuristic. </li>
    </ol>
   </li>
   <li> All of the above should be first handled at the Maxima/Lisp level
   (in module ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions). </li>
  </ul>


  \todo Horn specialities
  <ul>
   <li> Is the following useful? </li>
   <li> Let us denote by t(F) the result where always
   one of (i), (ii) is dropped (appropriately). </li>
   <li> A class of horn-like formulas is the class of formulas
   F = C_1 and ... and C_k
   where each C_i is of the form
   (P_1 and ... and P_m) -> Q
   where the P_i are arbitrary positive and-or-formulas, while
   Q is a positive and-formula. So the P_i and all their sub-occurrences
   occur negatively, while Q occurs positively. </li>
   <li> Thus t(F) is a 3-CNF with (essentially) clauses of the form
    <ol>
     <li> (a or b) -> c </li>
     <li> (a and b) -> c </li>
     <li> a -> (b and c) </li>
    </ol>
   where a, b, c are variables (atomic formulas). </li>
   <li> Types 1 and 3 translate into 2 binary Horn-clauses each, while
   type 2 is already a Horn clause. </li>
   <li> Thus t(F) is a Horn formula. Since t(F) is computable in linear time,
   and SAT decision for Horn formulas is achievable in linear time,
   SAT decision for those generalised formulas F is achievable in linear
   time, </li>
  </ul>


  \todo Translations to CNF/DNF
  <ul>
   <li> Write direct translation (without new variables). </li>
   <li> Complete the Tseitin translation. </li>
  </ul>


  \todo Reorganisation
  <ul>
   <li> Move respective code in
   OKlib/ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/Formulas.mac
   (see also the tests) to PropositionalLogic/TseitinTranslation.mac. </li>
  </ul>


  \todo Optimisation
  <ul>
   <li> Implement optimisation by sharing of subexpressions. </li>
   <li> But likely this has its proper home in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/StraightLinePrograms.hpp?
   </li>
  </ul>

*/

