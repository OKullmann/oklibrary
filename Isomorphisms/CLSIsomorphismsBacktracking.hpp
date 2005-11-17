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
  \todo In another submodule the transformation of a cls-isomorphism problem into a
  SAT problem shall be implemented. Natural choice for variables are the literals in
  F_1, where each literal has the values as discussed above (also possibly using
  degree information etc.). Possibilities for clauses:
   - excluding the clauses not in F_2, using the standard "negative literals"
   (for most cases this will not be feasible, since there are too many clauses to be excluded);
   or
   - using a level-3 CNF with "positive literals", expressing that for each clause in F_2 the translated
     clause is in F_2.
  Both possibilities don't look too attractive for the general case. Perhaps one has another kind of
  variables for each clause in F_1, with uniform domain the clauses in F_2, plus consistency clauses,
  connecting the literal-variables and the clause-variables ("if C_1 is mapped to C_2, then we have
  one of the |C_1|! bijections between the literals"). This also yields a higher-order CNF. A main
  advantage is that information about the clause-graphs (the clause-coincidence multigraph and
  the conflict-multigraph) can be used to restrict the assignment of clauses. If the (original) clauses are large,
  then we get many clauses, but in nice cases we should get much less due to the restrictions on
  the literal assignments as discussed above. Here we can use standard literals.
  Very attractive seems to be to just use these two types of variables, and binary powerset-clauses expressing
  that if literal x maps to y, then all clauses containing x must map to clauses containing y,
  and that if clause C maps to clause D, then every literal x in C must map to some literal y in D.
  This is likely not complete, but perhaps a good approximation. And we avoid higher levels.
  \todo It should be useful to employ (multi-)graph isomorphism algorithms on the various (multi-)graphs
  associated with clause-sets, either as a preselection for potential isomorphisms or as a test.
  \todo We also need the determination of the automorphism group of a clause-set, either explicitely
  listing all elements or giving a more compact representation (using some group theory software).
*/

#ifndef CLSISOMORPHISMSBACKTRACKING_uahGGt9

#define CLSISOMORPHISMSBACKTRACKING_uahGGt9

namespace OKlib {

  namespace Isomorphims {

  }

}

#endif
