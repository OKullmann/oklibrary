// Oliver Kullmann, 17.12.2005 (Swansea)
/* Copyright 2005 - 2007, 2008, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Reductions/FailedLiteralReduction/plans/general.hpp
  \brief Plans for the module FailedLiteralReduction


  The general case is handled in module KLevelForcedAssignments (see
  KLevelForcedAssignments/plans/general.hpp), while this module considers
  the case K=2 and extensions (with special algorithms etc.), including
  k=3.


  \todo Update namespaces : DONE


  \todo Foundations
  <ul>
   <li> Clarify the relations to modules
    <ul>
     <li> KLevelForcedAssignments: This module considers variations on K=2 and
     K=3, while KLevelForcedAssignments considers arbitrary K. </li>
     <li> Resolution </li>
    </ul>
   </li>
   <li> See the ComputerAlgebra-part. </li>
  </ul>


  \todo First simple implementation
  <ul>
   <li> Just using Reductions::UnitClausePropagation::CLSAdaptorUcpW. </li>
   <li> However, this adaptor might not be usable for the iterated reduction
   attempts, since it is an adaptor, reading clauses and transferring
   them into some other data structure. It would only be helpful for handling
   the input? </li>
   <li> Or perhaps CLSAdaptorUcpW::perform_ucp can be used with various
   partial assignments in the buffer?! </li>
   <li> Compare "First implementation, based on watched literals" in
   Reductions/KLevelForcedAssignments/plans/general.hpp. </li>
   <li> In the first implementation for simplicity not considering whether
   some reduction attempts might be superfluous. </li>
   <li> If a reduction was found, the assignment is simply permanently
   added to the total assignment. </li>
  </ul>


  \todo Transferring the implementation used in OKsolver_2002


  \todo Timestamp
  <ul>
   <li> The (old) OKsolver used a timestamp
   (see OKlib/Programming/Utilities/TimeStamps/plans/general.hpp),
   incremented with each new literal processed (attempting to find a failed
   literal), which was used in two ways:
   <ol>
    <li> Assignments to variables used this timestamp, so that for a
    new round (with a new envisaged failed literal) assignments
    don't need to be undone). </li>
    <li> When processing literal x, all implied literals y where marked
    with the timestamp, so that when processing y, where meanwhile
    no failed literal has been detected, we know that we can skip y. </li>
   </ol>
   </li>
  </ul>


  \todo Avoiding repetition
  <ul>
   <li> If some reduction was performed, which other
   reductions have to be tested? </li>
   <li> While the timestamp trick uses forward-implications,
   here we need to trace implications backwards: We need to find out the
   clauses shortened by the performed reduction, and then we need to find all
   literals which imply the negations of the literals in those clauses ---
   those need to be (re)tested. Testing this could be expensive. </li>
   <li> In [Heule et al, SAT 2004] "tree-based look-ahead" needs
   investigation. </li>
  </ul>
   

  \todo Local learning
  <ul>
   <li> The cleanest way seems to me to handle all
   2-clauses separately, with a directed graph structure, and to maintain
   the invariant that this graph is a strict order (irreflexive and
   transitive), and for every arc x -> y we have also the arc -y -> -x. </li>
   <li> UCP then proceeds in stages: For envisaged failed literal x -> 1
   we obtain all derived unit clauses in one go and transfer them to the
   partial assignment, and then we perform the unit clause propagation;
   all unit clauses derived from >2-clauses must necessarily be new,
   we collect all of them, and go to the next stage etc. </li>
   <li> In [Bacchus, SAT 2003] HyperBinRes needs to be investigated. </li>
  </ul>


  \todo Interface
  <ul>
   <li> Independent of the implementation, the interface should be that
   for each literal the list of "obtainable" implied other literals is
   available (this handles the 2-clauses and more) ---  either, as discussed
   before, all implications are explicitely stored, or they are computed. </li>
   <li> The two extremes are either only r_2, or with full local learning.
   </li>
  </ul>


  \todo Graph representation
  <ul>
   <li> The known implications can be stored directly (as under local
   learning), which is fastest, but can use a lot of space, or we use further
   distributing levels (which can save considerable memory). </li>
   <li> The general problem is that of a given transitive relation R, where
   for each source we want to know all reachable sinks: This can be stored
   directly as a table, or we use intermediate steps (employing a directed
   graph). Perhaps such algorithms are used for database-implementations? </li>
  </ul>
   

  \todo Responsibility
  <ul>
   <li> Who is performing the r_2-reduction? </li>
   <li> One could imagine that every
   active clause-sets itself performs it, "flushing" at least when UCP has been
   completed the found unit clauses to the other active clause-sets --- but
   then each active clause-set had to know about the others. </li>
   <li> And since r_2-reduction is like branching ("more so" than just UCP),
   likely it is better that the alliance handles it: Choosing a literal for
   probing ("setting it to false"), performing UCP for the whole alliance
   where the active clause-sets are in probing mode, and performing the
   reduction if possible. </li>
   <li> This would also imply that the local learning is under the control of
   the alliance; seems to make good sense. (Alliances can themselves be
   considered as active clause-sets, and thus we also have the possibility to
   perform failed literal reduction in one active clause-set if we want.) </li>
  </ul>


  \todo 2-clauses
  <ul>
   <li> Perhaps at least in the presence of a local-learning-facility in the
   alliance, the alliance members should hand over all 2-clauses to this
   facility?! </li>
   <li> Perhaps better not, since in general this cannot be done reasonably.
   </li>
   <li> Perhaps initially, when the alliance is set up, then the members have
   the possibility to hand over 2-clauses to the local learning facility (and
   getting rid off if themselves --- if they want), and later there might
   still be the possibility that if an alliance member detects a global
   2-clause that it can be handed over to the local learning facility. </li>
  </ul>


  \todo There is the recurrent theme of "local vs. global"
  <ul>
   <li> "Local" means relative to the current partial assignment, while
   "global" means relative to the initial (empty) partial assignment. </li>
   <li> There could be also forms in between, but they seem hard to handle.
   </li>
  </ul>


  \todo Double look-ahead
  <ul>
   <li> What about the integration of partial double-look-ahead (partial
   r_3), as discussed for example in the SAT2007-paper by Heule/vanMaaren ?
   </li>
   <li> How much is missed with such a partial scheme? And what could be
   reasonable heuristics for the selection?? </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Reductions::FailedLiteralReduction
  \brief Components around the r_2-reduction

  Alias "Flr".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Reductions {
      namespace FailedLiteralReduction 
      }
      namespace Flr = FailedLiteralReduction;
    }
  }
}

