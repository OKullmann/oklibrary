// Oliver Kullmann, 7.8.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/ConstraintSatisfaction.hpp
  \brief Plans on (classical) backtracking techniques for constraint solving


  \todo constraint_backtracking
  <ul>
   <li> Likely the constraint-related backtracking should go into its own
   module? </li>
   <li> Also output the corresponding splitting trees; perhaps they
   contain the enforced assignments. </li>
   <li> Given such splitting trees, again one can represent all solutions,
   and/or count all solutions. </li>
   <li> The simple code for "constraint_backtracking_counting" shows
   the big problem of the notion of "propagators": Only total solutions
   are recognised! The generalised SAT approach should overcome this. </li>
  </ul>


  \todo Heuristics
  <ul>
   <li> For the purpose of comparisons implement also the 3 other heuristics
   from [Beek, 2006, Backtracking Search Algorithms] (which use only the
   the domain sizes from the look-ahead).
    <ol>
     <li> Brown and Purdom: Minimise the remaining domain size + the
     minimum over all other variables of the sum of their domain
     sizes in the branches. </li>
     <li> Geelen: Minimise for a variable the sum over its values
     of the product of the remaining domain sizes over all other
     variables. (This is closest to our, but it replaces the tau-rule
     by the sum, which should be a bad approximation.) </li>
     <li> Freeman: As Geelen, but instead of taking the product again
     the sum is chosen. </li>
    </ol>
    To me (OK) this all looks like guessing around, where the true rule is
    the tau-rule of variable_heuristics_tau.
   </li>
   <li> The heuristics should be optimal, given that only the domain sizes
   for the potential branches are available ?! </li>
  </ul>


  \todo Further propagators
  <ul>
   <li> "Enigma 1277" (see
   Satisfiability/Transformers/Generators/FaronsPuzzle.cpp) </li>
  </ul>

*/

