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

