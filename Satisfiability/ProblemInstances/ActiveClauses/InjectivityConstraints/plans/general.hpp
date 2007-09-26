// Oliver Kullmann, 16.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ActiveClauses/InjectivityConstraints/plans/general.hpp
  \brief Plans for the module on active clauses providing injectivity
  and bijectivity constraints.


  \todo Update namespaces. DONE


  \todo Move to ProblemInstances/ActiveClauses. DONE


  \todo Update:
  <ul>
   <li> Get an update on the services (possibly) provided efficiently
   by these active clauses. </li>
   <li> Update all todos and milestones.
  </ul>


  \todo First Concepts:
  Concept prototypes for injectivity, surjectivity and bijectivity constraints


  \todo Literature and software review


  \todo Basic implementations:
  <ul>
   <li> Important active clauses are given by INJ(V, D) for
   some set V of variables with domains D(v) (so D is a map), expressing that
   different variables get different values, and BIJ(V, D, Val),
   refining INJ(V, D) by also requiring that every value in Val
   must be used.
   </li>
   <li> Active clauses can offer different levels
   of inference powers. For "full inference power" essentially INJ
   and BIJ are the same in case of D(v) <= Val for all v in V and |V| = |Val|,
   but for weaker systems there might be a difference, and, of course,
   for INJ(V, D) nothing in general is known about the domains.
   </li>
   <li> The direct clause-representation of INJ(V,D) uses the clauses
   {(v != eps) or (v' != eps)} for v != v' and eligible values eps
   (see UPPERO in LinearInequalities/plans/CardinalityConstraints.hpp)
   while BIJ adds the surjectivity P-clauses {(v = eps)}_{v in V} for
   every eps in Val.
   Thus INJ(V,D) uses (|V| over 2) * |D| 2-clauses, SUR(V,D)
   uses |D| |V|-clauses, while BIJ(V,D) is the conjunction of
   INJ(V,D) and SUR(V,D).
   </li>
   <li> For the boolean translation, INJ(V,D) is easily representable
   via AMO's (see Generators/plans/AMO.hpp), and SUR by ALO's (see
   Generators/plans/AMO.hpp). </li>
   <li> INJ(V, D) is equivalent to LOWERV(V, |V|) (see LinearInequalities/plans/CardinalityConstraints.hpp). </li>
  </ul>


  \todo Exploiting matching algorithms; see Matchings/plans/Matchings.hpp
  <ul>
   <li> INJ(V,D) yields a bipartite variable-value graph, with the
   variables v in V on the one side, the possible values eps on the other side,
   and an edge connecting v and eps iff eps in D(v). It is INJ(V,D) satisfiable
   iff there exists a matching covering all variables. The literal (v <> eps)
   follows from INJ(V,D) iff the edge {v,e} is not element of any matching
   covering all variables; assuming that INJ(V,D) is satisfiable, this is equivalent
   to the condition, that no maximum matching contains {v,e}. Thus for a 0-strong
   active clause INJ(V,D) we need an algorithm computing a maximum matching
   for a bipartite graph, while for a 1-strong active clause INJ(V,D) we need
   additional an algorithm computing the set of all edges which are not element
   of some maximum matching.
   </li>
   <li> Satisfying partial assignments for INJ(V,D) are the matchings in
  the bipartite variable-value graph which cover all variables, while
  satisfying partial assignments for BIJ(V,D) are the perfect matchings in
  the bipartite variable-value graph. For BIJ(V,D) besides the existence
  of a perfect matching the edges which cannot be used (and those
  which must be used) in a perfect matching are of primary interest.
  </li>
   <li> Given these algorithms from the module Matchings, a further question is
   whether these algorithms can be made up-datable, so that we do not
   need to recompute everything from scratch after application of some partial
   assignment (that is, after removal of some edges from the bipartite graph).
   </li>
   <li> For a 1-strong active clause INJ(V,D) a matching covering all
   variables has to be maintained; given such a matching, in [Regin 1994] a
   linear-time algorithm is given that finds out all edges not part of some
   matching covering all variables.
   </li>
   <li> For INJ(V,D) it might be worth not only to consider edges in the
   bipartite variable-value graph which cannot be used, but also to consider
   edges which must be used (of course, if one edge must be used, than the
   others cannot, but it might be more efficient to realise in certain cases,
   that an edge must be used.
   </li>
  </ul>


  \todo Complexity measures:
  The natural complexity-measure for an injectivity constraint is the sum
  of the logarithms of the domain sizes of the variables involved.
  Also the measures for the simulating clause-sets (boolean or not) can
  be easily computed. But in general these measures might not be very
  useful.


  \todo Autarkies:
  Autarkies for injectivity constraints (and the other types) are likely best
  understood by using the boolean CNF-representation.


  \todo Extraction:
  We should also investigate how to extract injectivity constraints
  (from ordinary clause-sets for example, and in general from active clause-sets).


  \todo For variables v, v' not  only " v <> v' " is needed, but more general
  " f(v) <> f(v') " for some function f.
*/

/*!
  \namespace OKlib::Satisfiability::ProblemInstances::ActiveClauses::InjectivityConstraints
  \brief Active clauses expressing that variables must get different
  values, and generalisations and refinements.
*/

namespace OKlib {
  namespace Satisfiability {
    namespace ProblemInstances {
      namespace ActiveClauses {
        namespace InjectivityConstraints {
        }
        namespace InC = InjectivityConstraints;
      }
    }
  }
}

