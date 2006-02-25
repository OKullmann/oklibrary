// Oliver Kullmann, 24.2.2006 (Swansea)

/*!
  \file LinearInequalities_Plans.hpp
  \brief Plans for the module providing active clause-sets for linear inequalities
  \todo Transferring (and updating):
   OKsolver/Experimental/Transformationen/LinInequal.
  \todo The relations to the module SATModuloTheory have to
  be investigated.
  \todo A very basic (and important) is the constraint
  "v_1 + ... + v_k <= K" (call it UPPER({v_1, ..., v_k}, K)
  for a set {v_1, ..., v_k} of boolean variables and a constant K (meaning that
  at most K of the variables v_i are true; one would assume that
  not allowing a multi-set {v_1, ..., v_k} should enable more
  efficient algorithms; if {v_1, ..., v_k} is a multi-set with multiplicities
  from ZZ, then we have the general case of linear inequalities).
  \todo For non-boolean variables v_i the constraint
  "|{v_1, ..., v_k}| <= K" (call it UPPERV({v_1, ..., v_k}, K)
  seems to be the "right form" (meaning that the set of (different)
  values of variables v_i has at most K members). Using the standard
  translation to boolean variables, we get the above boolean form.
*/

/*!
  \namespace OKlib::LinearInequalities
  \brief Module with active clause-sets for linear inequalities
*/

namespace OKlib {
  namespace LinearInequalities {
  }
}

