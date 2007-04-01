// Oliver Kullmann, 31.8.2005 (Swansea)

/*!
  \file LocalSearch/plans/SupportTotalAssignments.hpp
  \brief Supporting data structures for total assignments to be used
  for efficient enumeration and local search (for satisfying assignments
  as well as autarkies). Operations to support variable flips.

  \todo See Concepts/plans/Assignments.hpp.

  \todo Design and implement. Should utilise the clause-literal graph.
  Default statistics: Number of falsified and satisfied literals per clause,
  and the total number of satisfied and critical clauses.
  Policies should allow for other statistics (for example when enumerating
  all assignments, all kinds of maximisations/minimisations could be
  performed; or when using the data structure for local search for
  autarkies, then the number of satisfied and critical clauses are needed
  for all levels).
*/

