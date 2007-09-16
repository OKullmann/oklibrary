// Oliver Kullmann, 17.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file FailedLiteralReduction/plans/general.hpp
  \brief Plans for the module FailedLiteralReduction


  The general case is handled in module KLevelForcedAssignments (see
  KLevelForcedAssignments/plans/general.hpp), while this module considers
  the case K=2 and extensions (with special algorithms etc.), including
  k=3.


  \todo Update namespaces.


  \todo Foundations
  <ul>
   <li> Clarify the relations to modules
    <ul>
     <li> KLevelForcedAssignments: This module considers variations on K=2 and K=3,
     while KLevelForcedAssignments considers arbitrary K. </li>
     <li> Resolution </li>
    </ul>
   </li>
  </ul>


  \todo Transferring the implementation used in OKsolver.


  \todo The (old) OKsolver used a timestamp
  (see Transitional/Utilities/plans/Utilities.hpp),
  incremented with each new literal processed
  (attempting to find a failed literal), which was used in two ways:
   - assignments to variables used this timestamp, so that for a
     new round (with a new envisaged failed literal) assignments
     don't need to be undone);
   - When processing literal x, all implied literals y where marked
     with the timestamp, so that when processing y, where meanwhile
     no failed literal has been detected, we know that we can skip y.


  \todo More complicated: If some reduction was performed, which other
  reductions have to be tested? While the above trick uses forward-implications,
  here we need to trace implications backwards: We need to find out the clauses
  shortened by the performed reduction, and then we need to find all literals
  which imply the negations of the literals in those clauses --- those need to
  be (re)tested. Testing this could be expensive.


  \todo Reduction uses already lazy datastructures, and we can also employ
  watched literals here (which are integrated into the occurrences lists).


  \todo Local learning: The cleanest way seems to me to handle all
  2-clauses separately, with a directed graph structure, and to maintain
  the invariant that this graph is a strict order (irreflexive and transitive),
  and for every arc x -> y we have also the arc -y -> -x.
  UCP then proceeds in stages: For envisaged failed literal x -> 1
  we obtain all derived unit clauses in one go and transfer them to the
  partial assignment, and then we perform the unit clause propagation;
  all unit clauses derived from >2-clauses must necessarily be new,
  we collect all of them, and go to the next stage etc.


  \todo Independent of the implementation, the interface should be that
  for each literal the list of "obtainable" implied other literals is available
  (this handles the 2-clauses and more) ---  either, as discussed before, all
  implications are explicitely stored, or they are computed.
  The two extremes are either only r_2, or with full local learning.


  \todo The known implications can be stored directly (as under local learning),
  which is fastest, but can use a lot of space, or we use further distributing
  levels (which can save considerable memory).
  The general problem is that of a given transitive relation R, where for each
  source we want to know all reachable sinks: This can be stored directly
  as a table, or we use intermediate steps (employing a directed graph).
  Perhaps such algorithms are used for database-implementations?


  \todo Who is performing the r_2-reduction? One could imagine that every
  active clause-sets itself performs it, "flushing" at least when UCP has been
  completed the found unit clauses to the other active clause-sets --- but then
  each active clause-set had to know about the others. And since r_2-reduction
  is like branching ("more so" than just UCP), likely it is better that the alliance
  handles it: Choosing a literal for probing ("setting it to false"), performing UCP
  for the whole alliance where the active clause-sets are in probing mode, and
  performing the reduction if possible. This would also imply that the local learning
  is under the control of the alliance; seems to make good sense. (Alliances can
  themselves be considered as active clause-sets, and thus we also have the
  possibility to perform failed literal reduction in one active clause-set if we want.)


  \todo Perhaps at least in the presence of a local-learning-facility in the alliance,
  the alliance members should hand over all 2-clauses to this facility?! Perhaps better not,
  since in general this cannot be done reasonably. Perhaps initially, when the alliance is
  set up, then the members have the possibility to hand over 2-clauses to the local
  learning facility (and getting rid off if themselves --- if they want), and later there
  might still be the possibility that if an alliance member detects a global 2-clause that
  it can be handed over to the local learning facility.


  \todo There is the recurrent theme of "local vs. global": "Local" means relative to the
  current partial assignment, while "global" means relative to the initial (empty)
  partial assignment. There could be also forms in between, but they seem hard to handle.


  \todo Double look-ahead:
  What about the integration of partial double-look-ahead (partial r_3), as discussed
  for example in the SAT2007-paper by Heule/vanMaaren ? How much is missed with such a
  partial scheme? And what could be reasonable heuristics for the selection??

*/

/*!
  \namespace OKlib::FailedLiteralReduction
  \brief Components around the r_2-reduction
*/

namespace OKlib {
  namespace FailedLiteralReduction {
  }
}

