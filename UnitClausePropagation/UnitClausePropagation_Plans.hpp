// Oliver Kullmann, 25.11.2005 (Swansea)

/*!
  \file UnitClausePropagation_Plans.hpp
  \brief Plans for the module UnitClausePropagation
  \todo Transferring (and updating)
  OKsolver/Experimental/AllgKlassen200203/UnitPropagation.hpp.
  \todo Implementing the standard linear time UP-algorithm
  (based on the bipartite structure) for P-clause-sets.
  \todo Investigate how to specialise the general UP-algorithm
  for clause-sets and boolean clause-sets.
  \todo The basic algorithm should assume an "active clause-set" and
  an "active partial assignment":
  The clause-sets receives the variable-domains from the partial assignment.
  Based on this, it computes unit-clauses (i.e. literals), which further restrict the partial
  assignment; these are transmitted to the partial assignment, which reacts
  with a list of further restricted literals, etc. At each point the clause-set
  as well as the partial assignment can report "unsatisfiability", and the
  clause-set can report "satisfied".
  Since unit-clause propagation is necessary, it doesn't seem necessary that it is
  remembered whether for example a power literal has been reduced several
  times --- only the final result is needed.
  However it seems more efficient if the partial assignment notifies the clause-set
  only about the updates (what has changed compared to last time), and this
  is different for different active clause-sets connected to the partial assignment.
  If in the alliance of active clause-set actually each active clause-set is just a single
  clause, then it would be good if the communication scheme could ensure that
  the whole process is linear time; to do so the partial assignment must now for
  each literal which (subset of) clause-sets to notify.
  \todo Partial assignments phi have a public member function
  phi.add_literals(range),
  returning a boolean, which can also be obtained by
  phi.inconsistent()
  (telling whether the associated set of total assignments contains elements or not).
  After returning, the internal sequence of literals has been updated (including standardisation).
  This sequence is available via phi as a container. If partial assignments are variable-based,
  then phi(v) for a variable v will return the current literal (in constant time).
  Additionally phi makes also the sequence of all update-literals available (changed only
  by adding to the end); in case of variable-based literals additionally the changes might be
  made available (perhaps the list of eliminated values).
  \todo A clause-set F can be constructed with a binding to a partial assignment F.phi(). With
  F.sat_status()
  we get whether F with the current value of F.phi() is unsatisfiable, satisfied or unknown (we need
  also the autarky-information somehow). With
  F.implied_literals()
  a sequence of implied literals is returned.
  \todo With
  phi.update_iterator()
  an active clause-set F can obtain an input iterator i_F from phi (specifically to be used by F).
  With
  phi.new_end(i_F)
  one gets the current past-the-end iterator (of the same type as i_F), so that from i_F to this iterator
  the update-literals can be found; two models:
   - i_F is independent of F, and is just the end-iterator of the list of all update-literals (for boolean clause-sets
     this is fully enough, since here this sequence is just a standardised partial assignment);
   - phi processes all the update-literals from i_F to the current end, and maintains a compressed form of the
     corresponding partial assignment (which should be just the standardisation).
   We see that effectively phi maintains *views* for every clause-set F, providing partial assignments
   which show F the "difference" between the last state of phi and the new state.
   \todo Regarding unit-clause-propagation for an alliance of active clause-sets:
   There is a "variable-constraint graph" (or "factor-graph"; possibly trivial) which enables access
   for every variable to the set of relevant active clause-sets (of course, assuming here variable-based
   literals). When active clause-sets apply partial assignments, they can update this graph.
*/

/*!
  \namespace OKlib::UnitClausePropagation
  \brief Generic unit clause propagation (and non-parameterised generalisations)
*/

namespace OKlib {
  namespace UnitClausePropagation {
  }
}

