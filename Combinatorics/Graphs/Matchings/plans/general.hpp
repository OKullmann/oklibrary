// Oliver Kullmann, 11.1.2006 (Swansea)

/*!
  \file Matchings/plans/general.hpp


  \todo Correction: DONE
  Find the date in the cvs-logs for Matchings/plans/milestones.hpp
  (originally the first version was 0.0.1, but this was changed to
  0.0.2, since there were already some more plans there).


  \todo Update namespaces.


  \todo Literature and software review on matching algorithms


  \todo Concepts for bipartite graphs are needed.


  \todo First implementations
  <ul>
   <li> As a first simple algorithm, a matching problem
   is reduced to a network flow problem, so that the maximum
   flow algorithms from Boost can be used. </li>
   <li> Hungarian method: Implement the basic direct algorithm
   for finding maximum matchings. </li>
  </ul>


  \todo Injectivity constraints
  <ul>
   <li> For the 1-strong active clause-sets INJ(V,D) (see
   InjectivityConstraints/plans/InjectivityConstraints.hpp)
   an algorithm is needed for a bipartite graph
   finding out the set of edges which are not element of some
   maximum matching. </li>
   <li> Concepts are then needed to employ such algorithms for implementing
   injectivity-constraints. </li>
  </ul>


  \todo Online: We need also "online"-versions of these
  algorithms, allowing to remove some edges and recomputing
  the maximum matching respectively the set of unusable edges.
  Removing edges corresponds to applying some partial assignment;
  adding some edges then would correspond to backtracking steps.


  \todo Counting:
  We need (exact) algorithms for counting of perfect matchings
  <ul>
   <li> in bipartite graphs </li>
   <li> in graphs </li>
   <li> in general graphs </li>
   <li> in hypergraphs </li>
   <li> in general hypergraphs. </li>
  </ul>
  Direct implementations as well as translations to \#SAT problems.


  \todo Surplus:
   <ul>
    <li> Efficient computations of the surplus of a bipartite graph. </li>
    <li> Efficient decision whether the surplus is positive. </li>
   </ul>
   With these algorithms it can be decided whether a clause-set is
   matching-lean, etc.; see MatchingAutarkies/plans/MatchingAutarkies.hpp.
  
*/

/*!
  \namespace OKlib::Matchings
  \brief Matching algorithms for graphs (in the Boost sense)
*/

namespace OKlib {
  namespace Matchings {
  }
}

