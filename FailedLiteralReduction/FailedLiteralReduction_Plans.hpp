// Oliver Kullmann, 17.12.2005 (Swansea)

/*!
  \file FailedLiteralReduction_Plans.hpp
  \brief Plans for the module FailedLiteralReduction
  \todo Transferring the implementation used in OKsolver.
  \todo The (old) OKsolver used a timestamp (see Utilities/
  Timestamp.hpp), incremented with each new literal processed
  (attempting to find a failed literal), which was used in two ways:
   - assignments to variables used this timestamp, so that for a
     new round (with a new envisaged failed literal) assignments
     don't need to be undone);
   - When processing literal x, all implied literals y where marked
     with the timestamp, so that when processing y, where meanwhile
     no failed literal has been detected, we know that we can skip y.
  \todo Local learning: The cleanest way seems to me to handle all
  2-clauses separately, with a directed graph structure, and to maintain
  the invariant that this graph is a strict order (irreflexiv and transitiv),
  and for every arc x -> y we have also the arc -y -> -x.
  UCP then proceeds in stages: For envisaged failed literal x -> 1
  we obtain all derived unit clauses in one go and transfer them to the
  partial assignment, and then we perform the unit clause propagation;
  all unit clauses derived from >2-clauses must necessarily be new,
  we collect all of them, and go to the next stage etc.
  \todo Who is performing the r_2-reduction? One could imagine that every
  active clause-sets itselfs performs it, "flushing" at least over UCP has been
  completed the found unit clauses to the other active clause-sets --- but then
  each active clause-set had to know about the others. And since r_2-reduction
  is like branching ("more so" than just UCP), likely it is better that the alliance
  handles it: Choosing a literal for probing ("setting it to false"), performing UCP
  for the whole alliance where the active clause-sets are in probing mode, and
  performing the reduction if possible. This would also imply that the local learning
  is under the control of the alliance; seems to make good sense. (Alliances can
  themselves be considererd as active clause-sets, and thus we also have the
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
  partial assignment. There could be also forms inbetween, but they seem hard to handle.
*/

/*!
  \namespace OKlib::FailedLiteralReduction
  \brief Components around the r_2-reduction
*/

namespace OKlib {
  namespace FailedLiteralReduction {
  }
}

