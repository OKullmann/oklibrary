// Oliver Kullmann, 16.1.2006 (Swansea)

/*!
  \file InjectivityConstraints/plans/InjectivityConstraints.hpp
  \brief Plans for the module on active clause-sets providing injectivity
  and bijectivity constraints.

  \todo See log files for the date of version 0.0.1.

  \todo Literature and software review

  \todo Concepts for injectivity, surjectivity and bijectivity constraints

  \todo Implementation: Important active clause-sets are given by INJ(V, D) for
  some set V of variables with domains D(v), expressing that
  different variables get different values, and BIJ(V, D, Val),
  refining INJ(V, D) by also requiring that every value in Val
  must be used. Active clause-sets can offer different levels
  of inference powers. For "full inference power" essentially INJ
  and BIJ are the same (in case D(v) <= Val and |V| = |Val|),
  but for weaker systems there might be a difference.
  The direct clause-representation of INJ(V,D) uses the clauses
  {(v != eps) or (v' != eps)} for v != v' and eligible values eps,
  while BIJ adds the surjectivity P-clauses {(v = eps)}_{v in V} for
  every eps in Val.
  Thus INJ(V,D) uses (|V| over 2) * |D| 2-clauses, SUR(V,D)
  uses |D| |V|-clauses, while BIJ(V,D) is the conjunction of
  INJ(V,D) and SUR(V,D).

  \todo INJ(V,D) yields a bipartite variable-value graph, with the
  variables v in V on the one side, the possible values eps on the other side,
  and an edge connecting v and eps iff eps in D(v). It is INJ(V,D) satisfiable
  iff there exists a matching covering all variables. The literal (v <> eps)
  follows from INJ(V,D) iff the edge {v,e} is not element of any matching
  covering all variables; assuming that INJ(V,D) is satisfiable, this is equivalent
  to the condition, that no maximum matching contains {v,e}. Thus for a 0-strong
  active clause-set INJ(V,D) we need an algorithm computing a maximum matching
  for a bipartite graph, while for a 1-strong active clause-set INJ(V,D) we need
  additional an algorithm computing the set of all edges which are not element
  of some maximum matching.
  Given these algorithms from the module Matchings, a further question is
  whether these algorithms can be made up-datable, so that we do not
  need to recompute everything from scratch after application of some partial
  assignment (that is, after removal of some edges from the bipartite graph).

  \todo For variables v, v' not  only " v <> v' " is needed, but more general
  " f(v) <> f(v') " for some function f.
*/

/*!
  \namespace OKlib::InjectivityConstraints
  \brief Active clause-sets expressing that variables must get different
  values, and generalisations and refinements.
*/

namespace OKlib {
  namespace InjectivityConstraints {
  }
}

