// Oliver Kullmann, 24.7.2005 (Swansea)

/*!
  \file Concepts/ClauseSets.hpp
  \brief Concepts for clause-sets, clause-sets with history, virtual clause-sets and active
  clause-sets.
  \todo Concepts for clause-sets as collections of clauses, which are collections of
  literals, holding a reference to a domain-assignment for the variables.
  \todo An important refinement is the concept of a "clause-set with history",
  which holds a reference to a structure consisting of a domain-assignment
  together with a partial assignment refining it, and the state of the update
  process (how much of the partial assignment has been processed). Processed
  parts of the partial assignment may be transferred to the domain assignment.
  Clause-sets with history support the reversal of the assignment process (controlled
  by the assignment structure).
  \todo Actually clause-sets with history are only special cases of *virtual clause-sets*,
  which support sat_status() (may return "unknown"), and which in general do
  not enable access to the clauses (which in general are "not there").
  Active clause-sets refine virtual clause-sets by returning a partial assignment themselves
  (the set of unit-clauses which the inference mechanism found; "strong active
  clause-sets" infer all clauses of length <= 1). More generally for k-active clause-sets
  sequences of derived clauses of length at most k are computed (so virtual clause-sets
  are 0-active clause-sets).
  \todo Clause-sets should allow efficient access to the related hypergraphs
  and graphs (so that we can use general hypergraph and graph algorithms;
  examples for graphs are the resolution graph and the resolution-subsumption
  graph). This access should happen via simple wrapping of for example clauses
  or variables into node classes (the type of these node classes specifies
  the type of graph; no additional computations should be necessary).
  \todo The bipartite clause-literal graph likely should be accessed through
  the hypergraph-interface. The two refinements "clause-sets with history"
  and "clause-sets with bipartite structure" should be independent of each other, and
  there should be a common refinement.
  \todo A virtual clause-set is like a dynamic constraint with the additional possibility of
  handling partial assignments; the sat_status must be 0 or 1 in case all variables have
  been fixed.
  \todo A "Verbund" of active clause-sets owns the structure consisting of the domain
  assignment and the partial assignment (but this is controlled by external
  algorithms); by some strategy it processes (and potentially enhances) a given
  partial assignment (only when the partial assignment is completely processed by
  all active clause-sets and no decision was reached, then the domain assignment finally
  gets updated). Learning (local or global) is controlled from the outside;
  in case of a "conflict" a resolvent may be computed (potentially compressed).
*/

#ifndef CLAUSESETS_0YHB5

#define CLAUSESETS_0YHB5

namespace OKlib {

  namespace Concepts {

  }

}

#endif
