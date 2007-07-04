// Oliver Kullmann, 6.11.2006 (Swansea)

/*!
  \file Hypergraphs/Transversals/plans/general.hpp
  \brief Plans for the module on hypergraph transversals


  \todo Update namespaces.


  \todo Literature exploration:
  <ul>
   <li>What is known meanwhile about the existence of polynomial
   delay enumeration of Tr(G) for hypergraphs G? </li>
   <li> For example Wahlstroem has an algorithm for enumerating
   Tr(G) for 3-uniform G --- worth implementing? Likely one needs
   to extract a heuristical generalisation. </li>
   <li> The dual problem to hypergraph transversal enumeration
   is enumeration of all edge covers of a hypergraph ---
   has this formulation advantages / disadvantages? </li>
   <li> The complementary problem (by hyperedge-complementation)
   to hypergraph transversal enumeration is enumeration of all
   independent sets --- again, has this formulation advantages /
   disadvantages? </li>
   <li> The graph case:
    <ul>
    <li> Transversals for graphs are called "vertex covers"; are
    there special algorithms? </li>
    <li> Also the independent sets in graph deserve attention. </li>
    <li> For graphs, finding a minimum edge cover can be done
    in polynomial time by means of maximum matching; worth
    investigating? Dualisation yields, that minimum transversals
    can be computed for hypergraphs with vertex degrees at most
    2. </li>
    </ul>
   </li>
  </ul>


  \todo So the three problems
  <ol>
   <li> enumerating transversals </li>
   <li> enumerating edge covers </li>
   <li> enumerating independent sets </li>
  </ol>
  for hypergraphs can each be reduced to each other by a
  combination of dualisation and hyperedge-complementation.
  We should develop hypergraph adapters, which do not need to
  actually perform the transformation, but provide a "view".
  Perhaps this should be supported by the hypergraph concepts
  (otherwise we have to handle expression templates).

*/

/*!
  \namespace OKlib::HypergraphTransversals
  \brief Components for handling hypergraph transversals
*/

namespace OKlib {
  namespace HypergraphTransversals {
  }
}

