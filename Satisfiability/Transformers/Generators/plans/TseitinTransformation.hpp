// Oliver Kullmann, 10.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Generators/plans/TseitinTransformation.hpp
  \brief Plans regarding the Tseitin-transformation


  \todo Implementing the Tseitin-transformation (with variations):
  <ol>
   <li>
   Given a propositional formula F built from negation, conjunction
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
   The process F -> F' works, using auxiliary variables v(G) for
   subformulas G of F, by collecting clauses (to avoid renaming
   of existing variables, let v(a) = a for variables a from F):

   For a sub-formula G of F, where G is of the form
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

   F' is obtained by adding the unit clause {v(F)} to all these clauses.
   </li>
   <li> Is the following useful?

   Let us denote by t(F) the result where always
   one of (i), (ii) is dropped (appropriately).
   A class of horn-like formulas is the class of formulas

     F = C_1 and ... and C_k

   where each C_i is of the form

     (P_1 and ... and P_m) -> Q

   where the P_i are arbitrary positive and-or-formulas, while
   Q is a positive and-formula. So the P_i and all their sub-occurrences
   occur negatively, while Q occurs positively.
   Thus t(F) is a 3-CNF with (essentially) clauses of the form
   <ol>
   <li> (a or b) -> c </li>
   <li> (a and b) -> c </li>
   <li> a -> (b and c) </li>
   </ol>
   where a, b, c are variables (atomic formulas).
   Types 1 and 3 translate into 2 binary Horn-clauses each, while type 2 is
   already a Horn clause.

   Thus t(F) is a Horn formula. Since t(F) is computable in linear time,
   and SAT decision for Horn formulas is achievable in linear time,
   SAT decision for those generalised formulas F is achievable in linear time,
   </li>
   <li> Is there a general input format for boolean formulas? </li>
   <li> Some obvious variations on the transformation F -> F':
    - handling of arbitrary and-or-arities
    - performing simplifications on the clauses
    - either using always (i)+(ii), or exactly one of them, or using some
      heuristic.
   </li>
  </ol>

*/

