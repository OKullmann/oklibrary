// Oliver Kullmann, 13.11.2005 (Swansea)

/*!
  \file HittingClauseSets/plans/ExtendingHittingClauseSets.hpp
  \brief Algorithms for extending hitting clause-sets

  \todo Write the following algorithm which systematically
  searches for an extension of an input hitting clause-set F:
  The input is enriched with Tr(F) (see (currently)
  HypergraphTransversals/DirectTransversalEnumeration.hpp),
  so that easily all interesting extensions by one clause
  can be considered by a visitor; of these only the non-isomorphic
  ones (that is, where the extended clause-sets are non-isomorphic)
  are considered, and then recursively the algorithm tries to extend
  the clause-set further, until the visitor is "satisfied" (or no
  suitable extension exists). Either only the first solution is output,
  or all.

  \todo The first application of the previous algorithm is to find
  for k >= 1 boolean hitting clause-sets with constant clause-length k and
  2^k clauses, which have as many variables as possible. Interesting
  also the structure of the extremal clause-sets (for example
  regarding symmetry). Instead of maximising the number of variables
  one can minimise the maximal variable occurrence. Also the total
  number of finally obtained clause-sets is of interest (without removing
  isomorphic ones) in the context of determining precise probabilities
  for satisfiability of random formulas.

  \todo An interesting question is also how to extend 1-regular
  hitting clause-sets --- here not just satisfying assignments
  are sought, but "exactly satisfying assignments".
  See HypergraphTransversals/plans/ExactTransversals.hpp.
  
*/

