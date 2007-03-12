// Oliver Kullmann, 11.1.2006 (Swansea)

/*!
  \file Matchings/plans/Matchings.hpp

  \todo Literature and software review

  \todo Concepts for bipartite graphs are needed.

  \todo First Implementation: As a first simple algorithm, a matching problem
  is reduced to a network flow problem, so that the maximum
  flow algorithms from Boost can be used.

  \todo Hungarian method: Implement the basic direct algorithm
  for finding maximum matchings.

  \todo Injectivity constraints:
  For the 1-strong active clause-sets INJ(V,D) (see module
  InjectitivyConstraints) an algorithm is needed for a bipartite graph
  finding out the set of edges which are not element of some
  maximum matching.

  \todo Online: We need also "online"-versions of these
  algorithms, allowing to remove some edges and recomputing
  the maximum matching respectively the set of unusable edges.
  Removing edges corresponds to applying some partial assignment;
  adding some edges then would correspond to backtracking steps.

  \todo Counting:
  We need (exact) algorithms for counting of perfect matchings
   - in bipartite graphs
   - in graphs
   - in general graphs
   - in hypergraphs
   - in general hypergraphs.
  Direct implementations as well as translations to #SAT problems.
*/

/*!
  \namespace OKlib::Matchings
  \brief Matching algorithms for graphs (in the Boost sense)
*/

namespace OKlib {
  namespace Matchings {
  }
}

