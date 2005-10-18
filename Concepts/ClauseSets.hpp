// Oliver Kullmann, 24.7.2005 (Swansea)

/*!
  \file ClauseSets.hpp
  \brief Concepts for clause-sets.
  \todo Concepts for clause-sets, as clauses where the literals are (again) clauses.
  "Clause-sets as special hypergraphs." --- see below.
  \todo An important refinement is the concept of a "clause-set with a partial
  assignment", which supports extending the partial assignment and reversing
  these operations.
  \todo Clause-sets should allow efficient access to the related hypergraphs
  and graphs (so that we can use general hypergraph and graph algorithms;
  examples for graphs are the resolution graph and the resolution-subsumption
  graph). This access should happen via simple wrapping of for example clauses
  or variables into node classes (the type of these node classes specifies
  the type of graph).
  \todo It is actually not clear, whether clause-sets should be regarded as
  *special hypergraphs* (with additional structures), or whether they should
  be regarded as *generalised* hypergraphs (from which hypergraphs and
  graphs can be derived). Likely the second view is the better one.
  \todo The bipartite clause-literal graph likely should be accessed through
  the hypergraph-interface. The two refinements "clause-sets with partial assignments"
  and "clause-sets with bipartite structure" should be independent of each other, and
  there should be a common refinement.
*/

#ifndef CLAUSESETS_0YHB5

#define CLAUSESETS_0YHB5

namespace OKlib {

  namespace Concepts {

  }

}

#endif
