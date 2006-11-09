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

  \todo Local installation of UBCSAT.

  \todo Successfully include and use all triggers and their data structures
     needed from UBCSAT library in AutarkySearch.
   - These include: 
     ReadCNF, LitOccurence, CandidateList, DefaultProcedures and Flip+FalseClauseList.

  \todo First implementation for AnalyseTotalAssignment (for ML)
   - As a first step, analysing total assignments (and autarky reduction,
     if successful) is added to UBCSAT, without further changes.
     We have to find out how to do this, creating reasonable components
     for the OKlib.
   - The second step then is to change the heuristics.
   - The third step could then be to supply more appropriate data structures (this
     all integrated with the OKlibrary, but using the UBCSAT framework and components).
   - The fourth step then is to do everything within OKlibrary (using the generic
     components, and thus enjoying greater generality and re-usability).
*/

/*!
  \namespace OKlib::AutarkySearch
  \brief Components for autarky search
*/

namespace OKlib {
  namespace AutarkySearch {
  }
}

