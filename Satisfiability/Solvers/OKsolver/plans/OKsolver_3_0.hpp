// Oliver Kullmann, 4.2.2006 (Swansea)
/* Copyright 2006 - 2007, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/plans/OKsolver_3_0.hpp
  \brief Plans for the module for the successor of OKsolver_2_0;
  perhaps for SAT 2013.

  OKsolver3.0 strengthens OKsolver2.0 by using more complex methods,
  managed by forms of global resource management.


  \todo Once we are ready to start, a new sub-module is needed.


  \todo Update namespaces


  \todo Parallelism
  <ul>
   <li> One main challenge here seems to be parallel computations:
   Running different algorithms at the same time (even if only one processor is
   available), for example splitting v -> 0 and v -> 1 in parallel, and using
   the learned clauses from both sides. </li>
   <li> There must be some kind of central
   resource management. This enables also to use more time-consuming
   methods "when the opportunity arises". </li>
  </ul>


  \todo Distributed solving
  <ul>
   <li> It should then also be possible to distribute a solution process for
   example via the Internet (for big problems). </li>
  </ul>


  \todo Progress measurement
  <ul>
   <li> A main topic for efficient resource management is the measurement of
   progress. </li>
   <li> While for OKsolver_2_0 for example likely the simplest prediction
   strategy for the remaining running time is used (see
   Statistics/TimeSeriesAnalysis), now we are trying to use more sophisticated
   methods. </li>
  </ul>


  \todo Module KLevelForcedAssignments is used, going (much) beyond r_2.
 

  \todo Methods based on graph splitting are used now.


  \todo Constraints
  <ul>
   <li> At least cardinality constraints and injectivity constraints are added
   to the repertoire of active clause-sets (including extraction of structure
   from ordinary clause-sets). </li>
   <li> See modules
    <ol>
     <li> PseudoBoolean </li>
     <li> LinearInequalities </li>
     <li> InjectivityConstraints </li>
     <li> SATModuloTheories </li>
    </ol>
   </li>
  </ul>


  \todo Hard examples
  <ul>
   <li> OKsolver3.0 should be strong enough to handle the main cases of hard
   structural example for resolution (Tseitin formulas, matching formulas) in
   polynomial time (also for the clause-translations). </li>
  </ul>


  \todo Autarkies
  <ul>
   <li> Matching autarkies (see module MatchingAutarkies) and linear autarkies
   are appropriately integrated. </li>
  </ul>


  \todo Belief propagation
  <ul>
   <li> On the side of random formulas and local search, module SumProduct
   ("belief  propagation") is integrated. </li>
  </ul>


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

