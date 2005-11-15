// Oliver Kullmann, 26.8.2005 (Swansea)

/*!
  \file DirectTransversalEnumeration.hpp
  \brief Implementations of enumerations of hypergraph
  transversals based on the direct method.
  \todo Creating the concepts (the output, the transversal
  hypergraph, as container; for incrementell methods the
  iterator in it creates the next transversal "on the fly").
  \todo Implementing the naive method.
  \todo Apply it to compute all MUS's, given all MSS's.
  \todo Hypergraph traversal is a special case of "clause-set
  traversal", where for a clause-set the set of minimal satisfying
  partial assignments has to be computed. Via the embedding
  of hypergraphs into clause-sets as positive clause-sets, perhaps
  hypergraph transversal can be obtained as special case ?!
  For a clause-set F, let's call the set of minimal satisfying
  partial assignment also "Tr(F)".
  Computation of Tr(F) for clause-sets F should go into
  SATAlgorithms. The fundamental formula is
    Tr(F_1 union F_2) = min(Tr(F_1) * Tr(F_2))
  where for sets P_1, P_2 of partial assignments we define
    P_1 * P_2 = { phi_1 composition phi_2 : phi_i in P_i and
      phi_1, phi_2 compatible }
  (interpreting P_1, P_2 as clause-sets this corresponds to the
  computation of all possible unions, while suppressing tautological
  clauses).
  The simplest way to apply this formula is to enumerate all
  clauses, and then adding clauses one by one. Even here the
  order of clauses might be important, and perhaps using more
  general splitting strategies might be worth considering.
  It might be also interesting to investigate storing Tr(F)
  via (variations on) OBDD's (see Laurent Simon's work).
  For computing min efficiently, subsumption elimination
  is crucial (see SetAlgorithms/Subsumption.hpp), especially
  using the literal-clause graph.
*/

#ifndef DIRECTRANSVERSALENUMERATION_ujJJ54

#define DIRECTRANSVERSALENUMERATION_ujJJ54

namespace OKlib {

  namespace HypergraphTransversals {

  }

}

#endif
