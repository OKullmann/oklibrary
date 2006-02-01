// Oliver Kullmann, 1.2.2006 (Swansea)

/*!
  \file Equivalences_Plans.hpp
  \brief Plans for the module on equivalence handling
  \todo Given a boolean clause-set, the first problem is to
  extract the equivalences.
  The direct approach looks only
  at the clauses as given: We consider the equivalence relation ~
  on F given by C_1 ~ C_2 iff var(C_1) = var(C_2). Then we
  examine an equivalence classes F'. All clauses in F' have size k;
  If c(F') < 2^{k-1}, then F' doesn't contain an equivalence. Otherwise
  we consider the partioning of F' into F'_0 and F'_1 containing all
  clauses of parity 0 resp. 1 (regarding the number of negations).
  Now there are 0,1 or 2 equivalences in case F'_0 resp. F'_1 has
  size 2^{k-1}.
  Stronger approaches find also "hidden equivalences".
  \todo Then we need an active clause-set representing systems of
  equations over ZZ_2: "Falsified" means that one equation is falsified
  by the current (partial) assignment, "satisfied" means that all equations
  are satisfied by the current assignment (for an equation to be falsified
  or satisfied, all variables must have assigned a value). "Unsatisfiability"
  means that the system has no solution, and finally the implied unit
  clauses are the derived equalities v=a for a variable v and constant
  a in {0,1}. Naturally also implications v=v' arise here (but they are
  better just kept "inside", as discussed in the next point).
  We need efficient algorithms ("online").
  \todo A fundamental problem arises here: How to handle equivalences
  v=v' in the context of an alliance of active clause-sets? It seems
  most natural not to do anything special here, but just let the
  active clause-set managing equivalences handle this (if v is set to
  some value, then v' is set accordingly, and vice versa).
  So this is not a problem; but we have the problem for the heuristics
  to integrate from the various active clause-sets their complexity
  measurements.
  \todo How to generalise the concept of equivalences to non-boolean
  variables? Systems of equations over ZZ_m ?
*/

/*!
  \namespace OKlib::Equvalences
  \brief Active clause-sets for equivalence reasoning
*/

namespace OKlib {
  namespace Equivalences {
  }
}

