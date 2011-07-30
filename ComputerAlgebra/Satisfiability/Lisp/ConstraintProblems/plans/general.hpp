// Oliver Kullmann, 2.12.2007 (Swansea)
/* Copyright 2007, 2008, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/plans/general.hpp
  \brief Plans for Maxima-components regarding constraint satisfaction problems


  \todo Create milestones


  \todo Representing CSP problems
  <ul>
   <li> In
   ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/Conditions.mac
   the notions of "condition" and constraint are defined. </li>
   <li> A condition is a pair [V,e] where V is a set of variables and
   e is a function mapping total assignments over V to a value in {0,1}. </li>
   <li> A constraint is a pair [V,G] where V is a variable set and
   G is a non-boolean clause-set. </li>
   <li> A constraint is positive iff we consider G as a DNF clause-set when
   presenting the underlying condition. If G is considered a CNF, then the
   constraint is negative. </li>
   <li> How to represent conditions with large CNF/DNF representations
   but other simple representations and efficient algorithms? </li>
   <li> How to distinguish between different types of constraint
   which have special properties? For example hitting clause-sets.
   </li>
   <li> For example cardinality and linear constraints; see
   'The notion of a "linear constraint"' in
   ComputerAlgebra/Satisfiability/Lisp/LinearConditions/plans/general.hpp
   and
   'Representing "cardinality constraints"' in
   ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp.
   </li>
  </ul>


  \todo Translating systems of constraints to Minion
  <ul>
   <li> We must provide a translation of lists of constraints, to
   solver-specific CSP languages such as the Minion input language.
   </li>
   <li> We can then use these CSP solvers to solve our translated
   CSP problems. </li>
   <li> See "Minion" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp. </li>
   <li> See http://minion.sourceforge.net/files/Manual012.pdf for
   the specification of the Minion input language. </li>
   <li> See also "Representing CSP problems" and
   "Translating the constraint-system into SAT, CSP, ..." in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/plans/general.hpp.
   </li>
   <li> Simple systems of constraints, where the constraints are given
   as positive or negative constraints with full clause-sets can be
   represented in the Minion input language using "tables". </li>
  </ul>


  \todo Write tests


  \todo Write docus


  \todo Relations to other modules
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/general.hpp.
   </li>
  </ul>


  \todo SAT translations
  <ul>
   <li> See "Translations from CSP to SAT" in
   Experimentation/Investigations/plans/general.hpp. </li>
  </ul>

*/

