// Oliver Kullmann, 12.2.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/docus/general.hpp
  \brief User documentation for the conflict combinatorics of SAT (in Lisp/Maxima)


  <h2> Hitting clause-sets </h2>

  See
  ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/HittingClauseSets.mac.

  <h3> Uniform hitting clause-sets </h3>

  The following properties are equivalent for a uniform clause-set F with
  clause-length k:
  <ul>
   <li> F is unsatisfiable and has 2^k clauses </li>
   <li> F is a hitting clause-set with 2^k clauses </li>
   <li> F is an unsatisfiable hitting clause-set. </li>
  </ul>
  %Generators for unsatisfiable uniform hitting clause-sets with given k:
  <ul>
   <li> <code>uniform_usat_hitting_min</code> creates the (up to renaming
   unique elements with minimal deficiency 1 (i.e., maximal number of
   variables 2^k - 1). </li>
   <li> <code>uniform_usat_hitting_max</code> creates the (up to renaming
   unique elements with maximal deficiency 2^k - k (i.e., minimal number of
   variables k). </li>
  </ul>
  In both cases we have actually tree hitting clause-sets. The first case
  represents also the only regular unsatisfiable uniform hitting clause-sets.

*/

