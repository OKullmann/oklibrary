// Oliver Kullmann, 4.2.2006 (Swansea)

/*!
  \file AutarkySearch/plans/AutarkySearch.hpp
  \brief Plan for the module on autarky search
  \todo Basically there are three methods for autarky search:
   - directly searching for the partial assignment, either systematically
     or via local search (see EvaluatePartialAssignments.hpp);
   - searching through the space of total assignments (systematically
     or via local search) as in AnalyseTotalAssignment.hpp;
   - cross out some variables and search for a satisfying assignment (a good
     source for the variables crossed out is the (global) learning component).
  The third possibility together with the resolution-pruning algorithm from
  the old OKsolver gives an attractive possibility to combine DPLL algorithms
  with search for (local) autarkies at each node (and can be combined with
  the second method: in case the assignment found by local search is
  not satisfying, one can still consider the largest autarky contained in
  it --- also this is an autarky for the clause-set under investigation).
*/

/*!
  \namespace OKlib::AutarkySearch
  \brief Components for autarky search
*/

namespace OKlib {
  namespace AutarkySearch {
  }
}

