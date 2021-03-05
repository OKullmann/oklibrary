// Oliver Kullmann, 25.11.2005 (Swansea)
/* Copyright 2005 - 2007, 2008, 2009, 2010, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Reductions/UnitClausePropagation/plans/general.hpp
  \brief Plans for the module on unit-clause propagation


  \todo Update to new system:
  <ul>
   <li> Makefile </li>
   <li> application-tests </li>
   <li> unit-tests </li>
   <li> timings </li>
   <li> fuzzing </li>
  </ul>


  \todo Update namespace usage


  \todo Connections
  <ul>
   <li> Old first prototype:
   OKsolver/Experimental/AllgKlassen200203/UnitPropagation.hpp. </li>
   <li> See Satisfiability/Lisp/Reductions/plans/UnitClausePropagation.hpp
   for the Maxima/Lisp level. </li>
   <li> See
   Experimentation/Investigations/Reductions/UnitClausePropagation/plans/general.hpp
   for investigations. </li>
  </ul>


  \todo Update the plans.


  \todo Create milestones.


  \todo Implement trivial unit-propagation (quadratic time)
  <ul>
   <li> We only need to update function
   UnitPropagation::unit_propagation_simple in
   Reductions/UnitClausePropagation/UnitPropagation.hpp. </li>
   <li> However, also the concepts used need to be updated. </li>
  </ul>


  \todo Implement generic standard linear-time algorithm for boolean
  clause-sets
  <ul>
   <li> We have the generic function
   UnitPropagation::unit_propagation_improved in
   Reductions/UnitClausePropagation/UnitPropagation.hpp. </li>
   <li> Here the clause-literal graph is assumed to be given. </li>
   <li> A more lightweight approach constructs the necessary data structures
   by the algorithm itself. </li>
   <li> Reductions/UnitClausePropagation/UnitClausePropagation.cpp implements
   currently two such approaches; see /plans/UnitClausePropagation.hpp. </li>
  </ul>


  \todo Implement initial UCP based on head-tail clauses
  <ul>
   <li> See "Head-tail clauses" in ProblemInstances/Clauses/plans/general.hpp
   for the underlying clause-implementation. </li>
   <li> Binary clauses should be taken out, and treated directly. </li>
  </ul>


  \todo Implementing the standard linear-time UCP-algorithm
  (based on the bipartite structure) for Power-clause-sets.


  \todo Investigate how to specialise the general UCP-algorithm
  for clause-sets and boolean clause-sets.


  \todo The basic algorithm should assume an "active clause-set" and
  an "active partial assignment"
  <ul>
   <li> The clause-sets receives the variable-domains from the partial
   assignment. </li>
   <li> Based on this, it computes unit-clauses (i.e. literals), which further
   restrict the partial assignment; these are transmitted to the partial
   assignment, which reacts with a list of further restricted literals, etc.
   </li>
   <li> At each point the clause-set as well as the partial assignment can
   report "unsatisfiability", and the clause-set can report "satisfied". </li>
   <li> Since unit-clause propagation is necessary, it doesn't seem necessary
   that it is remembered whether for example a power literal has been reduced
   several times --- only the final result is needed. </li>
   <li> However it seems more efficient if the partial assignment notifies the
   clause-set only about the updates (what has changed compared to last time),
   and this is different for different active clause-sets connected to the
   partial assignment. </li>
   <li> If in the alliance of active clause-set actually each active clause-set
   is just a single clause, then it would be good if the communication scheme
   could ensure that the whole process is linear time; to do so the partial
   assignment must know for each literal which (subset of) clause-sets to
   notify. </li>
  </ul>


  \todo A clause-set F can be constructed with a binding to a partial
  assignment F.phi().
  <ul>
   <li> With F.sat_status() we get whether F with the current value of F.phi()
   is unsatisfiable, satisfied or unknown (we need also the autarky-information
   somehow). </li>
   <li> With F.implied_literals() a sequence of implied literals is returned.
   </li>
  </ul>


  \todo Regarding unit-clause-propagation for an alliance of active clause-sets
  <ul>
   <li> There is a "variable-constraint graph" (or "factor-graph"; possibly
   trivial) which enables access for every variable to the set of relevant
   active clause-sets (of course, assuming here variable-based literals). </li>
   <li> When active clause-sets apply partial assignments, they can update this
   graph. </li>
  </ul>


  \todo UCP for clause-sets has the following interesting property
  <ul>
   <li> When applying an assignment v -> eps, running through all relevant
   clauses, then when evaluating the clauses the current partial assignment is
   used, which involves all assignments from the buffer, not just the
   assignments already performed. </li>
   <li> This can be exploited by using a timestamp for the buffer
   (see Utilities/TimeStamps/plans/general.hpp), incremented each time a new
   assignment is added; and when processing a clause the current timestamp is
   stored with this clause, so that we can always check whether the last
   evaluation of this clause already included the currently processed literal
   (in which case we don't have to go through this clause again). </li>
  </ul>


  \todo A variation point is whether we want to go also through the satisfied
  clauses or not (marking them as satisfied)
  <ul>
   <li> This makes a difference for example for associated statistics
   gathering. </li>
   <li> In the (old) OKsolver there was a clear separation: UCP didn't look at
   satisfied clauses and didn't gather statistics; this was the responsibility
   of the branching heuristics look-ahead (together with autarky reduction).
   </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Reductions::UnitClausePropagation
  \brief Components for unit-clause propagation ("UCP")

  Alias "Ucp".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Reductions {
      namespace UnitClausePropagation {
      }
      namespace Ucp = UnitClausePropagation;
    }
  }
}
