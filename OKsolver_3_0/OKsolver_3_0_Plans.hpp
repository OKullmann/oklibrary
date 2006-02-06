// Oliver Kullmann, 4.2.2006 (Swansea)

/*!
  \file OKsolver_3_0_Plans.hpp
  \brief Plans for the module for the successor of OKsolver_2_0;
  perhaps for SAT 2009.
  \todo The main challenge here seems to be parallel computations:
  Running different algorithms at the same time (even if only one precessor is
  available), for example splitting v -> 0 and v -> 1 in parallel, and using the
  learned clauses from both sides. There must be some kind of central
  resource management.
  \todo It should then also be possible to distribute a solution process for
  example via the Internet (for big problems).
  \todo A main topic here is the measurement of progress; while for OKsolver_2_0
  for example likely the simplest predicition strategy for the remaining running time
  is used (see Statistics/TimeSeriesAnalysis), now we are trying to use more sophisticated
  methods.
  \todo More complicated methods (based for example on graph splitting) are used now,
  decided by the resource management.
*/

/*!
  \namespace OKlib::OKsolver_3_0
  \brief Applications implementing the successor of OKsolver_2_0
*/

namespace OKlib {
  namespace OKsolver_3_0 {
  }
}

