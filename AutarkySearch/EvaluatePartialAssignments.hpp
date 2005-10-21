// Oliver Kullmann, 25.9.2005 (Swansea)

/*!
  \file EvaluatePartialAssignments.hpp
  \brief Evaluation of partial assignments, computing a heuristical
  value of how close the assignment is to an autarky for a given
  clause-set.
  \todo The two most basic measures are the number of critical clauses
  and the number of variables of (assigned) variables involved in critical
  clauses. Since this gives preferences to small partial assignments, one
  can standardise these numbers by dividing through the number of
  all touched clauses resp. the number of all assigned variables.
  \todo The main design issue for evaluation concepts is how to
  integrate the evaluation process into the updating mechanism (so
  that unnecessary runs over the clause-set can be avoided).
*/

#ifndef EVALUATEPARTIALASSIGNMENT_9ijKc

#define EVALUATEPARTIALASSIGNMENT_9ijKc

namespace OKlib {

  namespace AutarkySearch {

  }

}

#endif
