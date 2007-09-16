// Oliver Kullmann, 4.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/plans/OKsolver_3_0.hpp
  \brief Plans for the module for the successor of OKsolver_2_0;
  perhaps for SAT 2010.

  OKsolver3.0 strengthens OKsolver2.0 by using more complex methods,
  managed by forms of global resource management.


  \todo Once we are ready to start, a new sub-module is needed.


  \todo Update namespaces


  \todo One main challenge here seems to be parallel computations:
  Running different algorithms at the same time (even if only one processor is
  available), for example splitting v -> 0 and v -> 1 in parallel, and using the
  learned clauses from both sides. There must be some kind of central
  resource management. This enables also to use more time-consuming
  methods "when the opportunity arises".


  \todo It should then also be possible to distribute a solution process for
  example via the Internet (for big problems).


  \todo A main topic for efficient resource management is the measurement of progress;
  while for OKsolver_2_0 for example likely the simplest prediction strategy for the remaining
  running time is used (see Statistics/TimeSeriesAnalysis), now we are trying to use more
  sophisticated methods.


  \todo Module KLevelForcedAssignments is used, going (much) beyond r_2.
 

  \todo Methods based on graph splitting are used now.


  \todo And at least cardinality constraints and injectivity constraints are added to the
  repertoire of active clause-sets (including extraction of structure from ordinary clause-sets).
  See modules
   - PseudoBoolean
   - LinearInequalities
   - InjectivityConstraints
   - SATModuloTheories.


  \todo OKsolver3.0 should be strong enough to handle the main cases of hard structural example for
  resolution (Tseitin formulas, matching formulas) in polynomial time (also for the clause-translations).


  \todo Matching autarkies (see module MatchingAutarkies) and linear autarkies are appropriately
  integrated.


  \todo On the side of random formulas and local search, module SumProduct ("belief  propagation")
  is integrated.


  \todo Finally, OKsolver3.0 handles also quantifiers.

*/

/*!
  \namespace OKlib::OKsolver_3_0
  \brief Applications implementing the successor of OKsolver_2_0
*/

namespace OKlib {
  namespace OKsolver_3_0 {
  }
}

