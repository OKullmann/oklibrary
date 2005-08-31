// Oliver Kullmann, 31.8.2005 (Swansea)

/*!
  \file AnalyseTotalAssignment.hpp
  \brief Decision, whether a given partial assignment contains an autarky, and
  applications for autarky search (via enumeation of all total assignments, or
  local search through total assignments).
  \todo The basic algorithm is: Given a partial assignment phi, unassign all
  variables in critical clauses, and repeat this process until no critical clauses
  are left --- the maximal autarky contained in phi is obtained. Design and
  implement, using the data structures from SupportTotalAssignment.
  \todo Combination with enumeration of all total assignments yields
  autarky search in time O(2^n).
  \todo Combination with local search yields local search for autarkies (using
  total assignments). As the value of a total assignments the minimal number
  of critical clauses over all (non-empty) levels could be used. (If a low number
  of variables is involved, then a low number of critical clauses is to be expected;
  so perhaps better to use the ratio (number of variables in level) / (number of
  critical clauses for the level) --- the biggger this number the better.)
*/

#ifndef ANALYSETOTALASSIGNMENT_7yHgzz

#define ANALYSETOTALASSIGNMENT_7yHgzz

namespace OKlib {

  namespace AutarkySearch {

  }

}

#endif
