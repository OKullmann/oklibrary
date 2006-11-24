// Oliver Kullmann, 25.11.2005 (Swansea)

/*!
  \file HypergraphColouring/plans/HypergraphColouring.hpp
  \brief Plans for the module on hypergraph colouring

  \todo Translation :
  Given a (hyper)graph as input, translate the k-colouring problem into
  generalised or P-clause-sets.

  \todo For interesting systematically generated classes of (hyper)graphs,
  create active clause-sets translating k-colouring problems.

  \todo Investigate the problem of determining the chromatic number.

  \todo Implement the greedy colouring algorithm for graphs, and several strategies
  for finding a good vertex ordering:
   - use boost::sequential_vertex_coloring
   - run through all vertex orderings and record for the range of colours occurring how
     many orderings used that many colours
   - write a small applications, which reads a graph from a file, and outputs the statistics
     on vertex orderings and optionally all optimal orderings
   - search for literature on heuristics for vertex orderings; from CS-232 we have the
     traversing greedy colouring; it seems sensible to combine this with a largest degree first-
     stratey.

  How do these algorithms translate into SAT algorithms?

  \todo Erdoes and Hajnal introduced the function m(k) associated with property B:
  The minimal number of clauses of non-2-colourable k-uniform hypergraphs.
  Known are only m(0) = m(1) = 1, m(2) = 3, m(3) = 7 (see the latest OK-report (XXX)
  on autarkies and hypergraph colouring), while 19 <= m(4) <= 23. Given n, m, k,
  it should be possible to construct a nice SAT problem whose solutions are the k-uniform
  non-2-colourable hypergraphs G with n vertices and m edges. Then we can try to compute
  values for m(k).
  This can also be generalised to higher chromatic numbers (in the literature there is the
  generalised property B(s)? it seems more natural to consider c >= 2 for "non-c-colourable",
  and to consider m(c, k) (with m(2, k) = m(k))).
*/

/*!
  \namespace OKlib::HypergraphColouring
  \brief Components for hypergraph colouring
*/

namespace OKlib {
  namespace HypergraphColouring {
  }
}

