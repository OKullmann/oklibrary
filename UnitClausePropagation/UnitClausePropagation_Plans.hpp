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
  On the side of the partial assignment literals are interpreted "positively" (as literals
  in a DNF, restricting the assignment possibilities), while on the side of the
  clause-set the literals are interpreted "negatively" (as literals in a CNF,
  excluding assignments).
  In order to guarantee that always all unit-clauses can be processed by the
  partial assignment, all literals must be clopen.
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
*/

/*!
  \namespace OKlib::UnitClausePropagation
  \brief Generic unit clause propagation (and non-parameterised generalisations)
*/

namespace OKlib {
  namespace UnitClausePropagation {
  }
}

