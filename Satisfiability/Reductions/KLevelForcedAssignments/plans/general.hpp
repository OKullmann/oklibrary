// Oliver Kullmann, 17.12.2005 (Swansea)
/* Copyright 2005 - 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Reductions/KLevelForcedAssignments/plans/general.hpp
  \brief Plans for the module KLevelForcedAssignments

  The special case K=2 (and extensions) is handled in module
  FailedLiteralReduction (see FailedLiteralReduction/plans/general.hpp),
  while "real" resolution reductions are handled in module Resolution
  (see Resolution/plans/Reductions.hpp). This module considers
  algorithms and data structures for the general case (arbitrary K)
  of generalised unit-clause-propagation.


  \todo Update namespaces.


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Reductions/GeneralisedUCP.mac
   for Maxima-specifications. </li>
  </ul>


  \todo Create milestones.


  \todo First implementation, based on watched literals
  <ul>
   <li> It seems that UCP for the input is considered as preprocessing,
   and that we should consider computing r_k(F) for k >= 2, where all
   clauses of F have at least two literals. </li>
   <li> Binary clauses are taken out of F: for every literal x there are
   vectors U(x) of literals, and for {x,y} we have U(x) containing y, and
   U(y) containing x. </li>
   <li> Clauses with length >= 3 support watched literals; see "Watched
   literals" in Satisfiability/ProblemInstances/Clauses/concepts/general.hpp.
    <ol>
     <li> The invariant is that watched literals are (currently) not set
     to false w.r.t. those assignments in the partial assignment which
     have been processed. </li>
     <li> The only exception is when a clause has been falsified. </li>
     <li> So processing a falsified literal means going to all the clauses
     on its watch-list and find other literals to watch in those clauses. </li>
     <li> So normally for a falsified literal the list of its watched clauses
     is just emptied, and these clauses are distributed to other literals.
     </li>
     <li> The only exception is when a unit-clause was found, or when the
     empty clause was found: in both cases that clause stays on the list.
     </li>
     <li> The reason is that if not the empty clause has been produced, then
     that clause has been satisfied, and satisfied clauses can be ignored,
     while in the case of the empty clause backtracking is performed. </li>
     <li> So the invariant should be more precise that either the watched
     literal is not set to false, or else another literal in the clause
     is set to true, and that literal is either "older" than the watched
     literal, or it is forced at the same level as the watched literal. </li>
     <li> So that when backtracking, it can never happen that the watched
     literal is still falsified while all satisfied literal in the clause
     vanished. </li>
     </li>
    </ol>
   </li>
   <li> We also need a component containing the current partial assignment,
   its presentation as total assignment, and the sequence of unassigned
   variables:
    <ol>
     <li> One can add "forced literals" or "decision literals". </li>
     <li> For a literal x we can request its state (via the total assignment).
     </li>
     <li> And one can reset the state to just before the last decision
     literal. </li>
     <li> That means all forced literals from the back of the partial
     assignment as stack are taken away, until possibly one hits a decision
     literal - also this is taken away, and then the process stops. </li>
    </ol>
   </li>
  </ul>

*/

/*!
  \namespace OKlib::KLevelForcedAssignments
  \brief Components for generalised unit clause elimination
*/

namespace OKlib {
  namespace KLevelForcedAssignments {
  }
}

