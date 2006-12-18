// Oliver Kullmann, 25.11.2005 (Swansea)

/*!
  \file HypergraphColouring/plans/HypergraphColouring.hpp
  \brief Plans for the module on hypergraph (and graph) colouring

  \todo Translation :
  Given a (hyper)graph as input, translate the k-colouring problem into
  generalised or P-clause-sets.

  \todo For interesting systematically generated classes of (hyper)graphs,
  create active clause-sets translating k-colouring problems.

  \todo Investigate the problem of determining the chromatic number:
  <ul>
   <li> For finding an optimal colouring for a graph G the following strategy seems natural:
    <ol>
     <li> Find a reasonable upper bound chi(G) <= u by some quick heuristics (different forms
              of greedy colouring).
     </li>
     <li> Find a reasonable lower bound l <= chi(G) by some quick heuristics, among them
          finding cliques in G.
     </li>
     <li> Comprising the found cliques into hyperedges, a hypergraph G' is constructed (with
          chi_s(G') = chi(G), where chi_s is the strong colouring number), and then via
          a (generalised) SAT solver the interval l <= chi_s(G') <= u is narrowed to a point
          (perhaps here bisection is sensible?!); see HypergraphColouring/plans/StrongColouring.hpp
          for strong colourings of hypergraphs.
     </li>
    </ol>
   </li>
   <li> For finding an optimal colouring for a hypergraph G we should try the same strategy:
    <ol>
     <li> Find some lower bound l and some upper bound u. </li>
     <li> Close the gap with a (generalised) SAT solver. </li>
    </ol>
   </li>
   <li> And the same for strong hypergrap colouring. </li>
  </ul>

  \todo Erdoes and Hajnal introduced the function m(k) associated with property B:
  The minimal number of hyperedges of non-2-colourable k-uniform hypergraphs.
  Known are only m(0) = m(1) = 1, m(2) = 3, m(3) = 7 (see the latest OK-report (XXX)
  on autarkies and hypergraph colouring), while 19 <= m(4) <= 23. Given n, m, k,
  it should be possible to construct a nice SAT problem whose solutions are the k-uniform
  non-2-colourable hypergraphs G with n vertices and m edges. Then we can try to compute
  values for m(k).

  This can also be generalised to higher chromatic numbers (in the literature there is the
  generalised property B(s)? it seems more natural to consider c >= 2 for "non-c-colourable",
  and to consider m(c, k) (with m(2, k) = m(k))).

  \todo In [Robertson, Seymour, Thomas: Permanents, Pfaffian orientations, and even directed circuits]
  a poly-time algorithm for deciding whether a square hypergraph is minimal non-2-colourable is
  given; very interesting to investigate this algorithm.

*/

/*!
  \namespace OKlib::HypergraphColouring
  \brief Components for hypergraph colouring
*/

namespace OKlib {
  namespace HypergraphColouring {
  }
}

