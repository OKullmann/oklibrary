// Oliver Kullmann, 16.2.2007 (Swansea)

/*!
  \file Resolution/plans/Resolution.hpp
  \brief Plans for the modules providing components for resolution proof search

  \todo Whatever remains of module DPv shall move to here.

  \todo Exact tree resolution complexity:
  Implement the dynamic-programming algorithm for computing
  the tree resolution complexity for all possible clauses
  over var(F) for a given clause-set F:
  - Let trc(C) be the tree-resolution complexity of C w.r.t. F,
    that is, the minimal number of nodes in a tree resolution
    derivation of (exactly) C from F. So trc(C) is an odd
    natural number. At the beginning trc(C) is "open" for
    all C over F. Then in round i = 1, 2, ... we determine
    all clauses C with trc(C) = 2i-1 (by marking exactly those
    clauses with 2i-1). In round 1 exactly the
    clauses of F are marked with 1. In round i > 1 for a yet
    unmarked clause C one checks whether it is the resolvent
    of marked clauses C',C'' such that
      trc(C') + trc(C'') + 1 = 2i-1
    in which case clause C gets marked.
  - For extracting an optimal tree resolution refutation for the
    empty clause, when marking a clause one should also store
    pointers to the parent clauses.
  - Can further restrictions be handled for boolean resolution?
  - For non-boolean clause-sets there is still only one form
    of resolution, however if signed literals are allowed, then
    more forms are possible.
  - If only interested in the empty clause, then we can stop
    as soon as we found it (and the reached level then is
    exactly the tree-resolution complexity of F). Are there
    other strategies to make the procedure a bit more efficient?
  - One possibility is to restrict the maximal clause-size
    considered (for 2-CNF it is actually sufficient to consider
    only clauses of length at most 2).

  \todo How to compute the minimal regular-resolution complexity?

  \todo How to compute the minimal DP-resolution complexity?

  \todo How to compute the minimal resolution complexity?

  \todo Implement the Buresh-Oppenheim/Mitchell algorithm
  for computing the minimal resolution complexity of F in
  boolean 2-CNF in polynomial time.
  
*/

/*!
  \namespace OKlib::Resolution
  \brief Components for resolution proof search
*/

namespace OKlib {
  namespace Resolution {
  }
}

