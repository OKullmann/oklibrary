// Oliver Kullmann, 1.11.2008 (Swansea)
/* Copyright 2008, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/PlantedSolutions.hpp
  \brief Plans for Maxima-generators for satisfiable instances with given solutions


  \todo Uniquely satisfiable problems
  <ul>
   <li> Input is a formal clause-set FF and a partial assignment phi. </li>
   <li> We want to find a minimal sub-assignment phi' of phi such that the 
   assignments of phi - phi' are all enforced. </li>
   <li> If phi * FF is uniquely satisfiable, then phi' * FF, or, equivalent,
   FF together with the unit-clauses representing the assignments from phi',
   is still uniquely satisfiable. </li>
   <li> DONE (see minUNISAT)
   The easiest approach is to choose for each step a random ordering
   of the variables in phi, go through them in this order, check whether
   after removal of this variable FF + {comp_sl(phi)} still is unsatisfiable,
   and remove the first such variable, repeating the whole procedure until
   no variable can be removed anymore. </li>
   <li> A generalisation finds all variables which can be removed, and gives
   them to some heuristics to decide which to choose. </li>
   <li> We always just call a Maxima SAT solver, but this can be a wrapper
   to some external SAT solver. </li>
   <li> So that we need a generic wrapper for calling an external SAT solver
   (assuming extended DIMACS input and output). </li>
  </ul>

*/

