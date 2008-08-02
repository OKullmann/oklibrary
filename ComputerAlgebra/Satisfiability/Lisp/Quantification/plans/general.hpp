// Oliver Kullmann, 29.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Quantification/plans/general.hpp
  \brief Plans on quantified satisfiability problems


  \todo Create milestones


  \todo Connections to other modules
  <ul>
   <li> See Satisfiability/Quantification/plans/general.hpp. </li>
  </ul>


  \todo Basic concepts
  <ul>
   <li> "Quantifiers" are perhaps best represented by pairs [q,x], where
   q is a string "fa" ("for all") or "ex" ("there exists"), while x is
   a variable. </li>
   <li> A "quantifier prefix" is a list of quantifiers. </li>
   <li> Quantified problems are then (in the simplest form) just pairs
   [P,I], where P is a quantifier prefix, while I for example is a
   formal clause-set. Such problems are in "prenex normal form". </li>
   <li> We also need to represent "dependent quantifiers", which perhaps
   are based on an underlying set of "universal variables", and are then
   just represented by a set of such universal variables --- so
   dependent quantifiers just speak about existential variables. </li>
   <li> The matrix of such a "dependent problem" is a condition depending
   on all universal and existential variables, and a solution is a (partial)
   assignment of boolean functions to the existential variables, formally
   depending on exactly the universal variables the existential variable
   depends on, such that substituting these functions into the matrix yields
   a tautology (in the universal variables). </li>
   <li> Since we want to treat CNF and DNF equally, we need also the
   corresponding formulations seeking for counterexamples for the universal
   variables. </li>
  </ul>


  \todo Basic backtracking algorithms
  <ul>
   <li> Given problems in prenex normal form, perform the basic and-or search,
   either just returning the result ("true" or "false"), or the whole
   tree. </li>
   <li> This can be done for arbitrary conditions which allow the application
   of partial assignments. </li>
   <li> Such a condition additionally should allow to evaluate satisfiability
   and the existence of autarkies in the existential variables only, crossing
   out the universal variables. </li>
   <li> Analogously, the conditions additionally should allow to evaluate
   contradictions and the existence of "anti-autarkies" in the universal
   variables only, crossing out the existential variables. </li>
  </ul>


  \todo Representing solutions by finite functions
  <ul>
   <li> For delivering solutions, a rich tool set for representing boolean
   functions, and finite functions in general, is needed (simplest with
   (truth) tables). See "New module FiniteFunctions" in
   ComputerAlgebra/Satisfiability/Lisp/plans/general.hpp. </li>
   <li> The trivial algorithm finding some solution runs through all total
   assignments to the universal variables, for each case determines all total
   assignments to the existential variables which fulfil the condition, and
   then for each existential variable v determines a realiser for v which only
   depends on its universal variables. </li>
   <li> For the last step the intersection over all other universal variables
   of assignments to v are needed (roughly). </li>
  </ul>


  \todo Generators
  <ul>
   <li> The translation of "Positional games" (in
   ComputerAlgebra/Hypergraphs/Lisp/plans/Colouring.hpp) yields a source of
   generators, for families of hypergraphs where the solutions are known
   (possibly only probabilistically). </li>
  </ul>

*/

