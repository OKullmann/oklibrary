// Oliver Kullmann, 15.11.2005 (Swansea)

/*!
  \file TimeSeriesAnalysis.hpp
  \brief Tools for the analysis of the progress made by a SAT solver.
  \todo The following application should be handled: A complete SAT
  solver can with every new node processed compute the number of total
  assignments realised to be unsatisfiable. So one can query, say, every second
  this total number (or the proportion of the whole search space covered)
  and, via a time series analysis, make a prediction how long it will take
  to solve the problem.
  \todo SAT algorithms should use this prediction power for example
  as follows (but these algorithms should go into a different module):
   - Optimising parameter values (like clause weights) by running the
     algorithm for some time, and then changing the parameter values,
     trying it again, etc.
   - This form of optimisation should be possible also for a list of problems
     (to be "learned"), or for a class of problems given by some generator.
*/

#ifndef TIMESERIESANALYSIS_1019m6tg7

#define TIMESERIESANALYSIS_1019m6tg7

namespace OKlib {

  namespace Statistics {

  }

}

#endif
