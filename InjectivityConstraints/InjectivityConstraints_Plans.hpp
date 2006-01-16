// Oliver Kullmann, 16.1.2006 (Swansea)

/*!
  \file InjectivityConstraints_Plans.hpp
  \brief Plans for the module on active clause-sets providing injectivity
  and bijectivity constraints.
  \todo Important active clause-sets are given by INJ(V, D) for
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
    \todo Important active clause-sets are given by INJ(V, D) for
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
  For variables v, v' not  only " v <> v' " is needed, but more general
  " f(v) <> f(v') " for some function v.
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

