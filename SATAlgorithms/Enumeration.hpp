// Oliver Kullmann, 26.7.2005 (Swansea)

/*!
  \file Enumeration.hpp
  \brief SAT algorithms using enumeration.
  \todo Writing trivial SAT algorithm, using any enumeration; likely some kind
  of visitor concept is useful here.
  \todo Specialising the trivial algorithm by using Gray codes and a "flip operation"
  of the formula (which can be implemented efficiently using literal-clause graphs
  and storing information about the assignments for each clause; this data structure
  is also fundamental for local-search algorithms (and in general any algorithm using
  total assignments)).
  \todo Another variation is the search for autarkies.
  \todo The trivial algorithm should at least compute the number of satisfying assignments
  (if not stopped after finding the first one) and the maximal number of simultaneously
  satisfiable clauses. Optionally he can also decide minimal unsatisfiability of F by the following
  process: Each time an assignment satisfying c(F)-1 clauses is found, the falsified clause
  is marked. F is minimally unsatisfiable iff F is unsatisfiable and at the end all clauses have
  been marked.
*/

#ifndef ENUMERATION_nncvtR9

#define ENUMERATION_nncvtR9

namespace OKlib {

  namespace SATAlgorithms {

  }

}

#endif
