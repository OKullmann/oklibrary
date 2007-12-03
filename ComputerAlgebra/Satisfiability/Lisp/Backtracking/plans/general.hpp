// Oliver Kullmann, 30.11.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/general.hpp
  \brief Plans for Maxima-components regarding backtracking algorithms


  \todo Create milestones


  \todo Write tests


  \todo Write docus


  \todo optimal_splitting_tree in
  ComputerAlgebra/Satisfiability/Lisp/Backtracking/SplittingTrees.mac
  <ul>
   <li> Perhaps storing the trees also for the worst case. </li>
   <li> When searching only for the best case, then branches which
   cannot improve the current minimal size can be cut off. </li>
   <li> Instead of minimising the number of overall nodes, that is,
   minimising the number of leaves (since every inner node has exactly
   two successors), one can also minimise the number of true-leafes (the
   number of clauses in the DNF-representation) or the number of false-leaves
   (the number of clauses in the CNF-representation). </li>
   <li> Introduce an additional parameter d (from 0 to inf), and for depth < d
   then the orbits of the automorphism group acting on the set of 2n literals
   is computed, and instead of evaluated all 2n trees only one element of
   each orbit is chosen (evaluated appropriately).
    <ol>
     <li> See "Symmetries" in
     ComputerAlgebra/Satisfiability/Lisp/plans/general.hpp. </li>
    </ol>
   </li>
   <li> Instead of just getting the minimal size, a variations return
   the complete distribution (for every occurring tree size the number of trees
   for that size). </li>
  </ul>


  \todo Display splitting trees
  <ul>
   <li> Latex output of splitting trees </li>
  </ul>


  \todo constraint_backtracking in
  ComputerAlgebra/Satisfiability/Lisp/Backtracking/ConstraintSatisfaction.mac
  <ul>
   <li> Also output the corresponding splitting trees; perhaps they
   contain the enforced assignments. </li>
   <li> Given such splitting trees, again one can represent all solutions,
   and/or count all solutions. </li>
   <li> The simple code for "constraint_backtracking_counting" shows
   the big problem of the notion of "propagators": Only total solutions
   are recognised! The generalised SAT approach should overcome this. </li>
  </ul>


  \todo Heuristics in
  ComputerAlgebra/Satisfiability/Lisp/Backtracking/ConstraintSatisfaction.mac
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

