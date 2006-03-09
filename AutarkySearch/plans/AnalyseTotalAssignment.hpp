// Oliver Kullmann, 31.8.2005 (Swansea)

/*!
  \file AutarkySearch/plans/AnalyseTotalAssignment.hpp
  \brief Decision, whether a given partial assignment contains an autarky, and
  applications for autarky search (via enumeration of all total assignments, or
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
  A general approach is as follows: The total assignment phi yields a sequence
  phi_0 = phi, phi_1, ..., phi_m = empty assignment, m > 0,
  of partial assignments (in the unsuccessful case; following the stages of
  unassigning variables). This gives us a list (phi_0, ..., phi_{m-1})
  of candidates for autarkies. Now we need an evaluation of phi_i
  regarding its autarky-goodness, say eval(phi_i) >= 0, where eval(phi_i) = 0
  iff phi_i is an autarky. So we get a (non-empty) list
  (eval(phi_0), ..., eval(phi_{m-1}))
  of non-negative real numbers, and one can use measures of this list like
  the min for obtaining an evaluation of phi (the smaller the better).
  So basically the problem of evaluating phi is delegates to the evaluation of
  partial assignments; see EvaluatePartialAssignments.
*/

