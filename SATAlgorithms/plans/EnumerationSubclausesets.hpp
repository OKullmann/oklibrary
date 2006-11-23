// Oliver Kullmann, 13.11.2005 (Swansea)

/*!
  \file SATAlgorithms/plans/EnumerationSubclausesets.hpp
  \brief Tools for running through sub-clause-sets of a clause-sets
  and evaluating them.

  Useful for example for counting the number of satisfiable uniform 3-clause-sets
  with 4 variables and 0 <= m <= 24 clauses; running through all these cases
  should be done efficiently, updating the results from one sub-clause-set to the next.

  \todo A general algorithms works as follows. Three inputs:
   - the clause-set (perhaps actually supplied by the sub-clause-set-visitor)
   - a sequence of assignments (perhaps more generally a suitable visitor) to be evaluated
   - a visitor responsible for running through the sub-clause-sets and
     receiving their evaluations.
  For the visitor the components from Combinatorics/Subsets.hpp should be
  usable. In the initialisation phase a matrix is computed, which for each clause
  and each assignment yields whether the assignment satisfies or falsifies the
  clause (and in case of partial assignments we have the additional possibilities
  "critical" and "disjoint"), and also a vector, which for each assignments contains
  the number of satisfied or falsified clauses (and in case of partial assignments
  also the number of "critical" and "disjoint" clauses), and if exactly one clause is
  falsified, then an iterator to this clause. There should also be a vector, which for
  each clause contains the number of assignments falsifying exactly this clause.
  "Matrix" and "vector" are actually "maps" here, and should
  have a generic meaning here, so that on the one hand we actually can precompute
  and store these results, and on the other hand are able to compute them on the fly.
  There are also global counters for the number of satisfying assignments and the maximal
  number of (simulteneously) satisfiable clauses (in case of partial assignments perhaps
  also the minimal number of critical clauses). Furthermore the total number of clauses
  for which there is at least one assignment falsifying exactly them.
  When the visitor switches to the next sub-clause-set, it gives the sequence of flipped clauses,
  so that all assignments can be updated. For each sub-clause-set
  the visitor obtains the information about number of satisfying assignments,
  maximal number of satisfiable clauses, and number of irredundant clauses, potentially
  also whether the sub-clause-set is lean or not, and whether it is an autark subset
  of the whole clause-set.
  In a typical application the clause-set is the set of all clauses of a certain size of a given
  set of variables, while the sequence of assignments is the sequence of all total assignments
  for these variables, and the visitor runs in Gray-code manner through all sub-sets.
*/

