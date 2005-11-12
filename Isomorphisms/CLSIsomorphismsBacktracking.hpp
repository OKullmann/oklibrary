// Oliver Kullmann, 12.11.2005 (Swansea)

/*!
  \file CLSIsomorphismsBacktracking.hpp
  \brief Backtracking algorithms for deciding whether two clause-sets are isomorphic.
  \todo Given two clause-sets F_1, F_2, the variable sets var(F_1), var(F_2)
  are partioned into equivalence classes of variables with the same domain size,
  and bijections between corresponding equivalence classes of F_1 and F_2 are
  sought together with bijections between the associated domains, such that F_1
  is transported into F_2. A natural restriction of the search space size is given by
  considering F_1/2[k], the sub-clause-sets of clauses of size exactly k, together with
  the literal interaction multigraphs G_1/2[k] with loops of F_1/2[k], and considering
  only mappings v_1 -> v_2 if for all k the sorted degree lists of v_1 in G_1[k] equals
  the sorted degree lists of v_2 in G_2[k]. Furthermore if v_1' -> v_2', then for all
  k the number of edges joining v_1 and v_1' in G_1[k] must be equal to the number
  of edges joining v_2 and v_2' in G_2[k].
*/

#ifndef CLSISOMORPHISMSBACKTRACKING_uahGGt9

#define CLSISOMORPHISMSBACKTRACKING_uahGGt9

namespace OKlib {

  namespace Isomorphims {

  }

}

#endif
