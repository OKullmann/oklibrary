// Oliver Kullmann, 1.2.2006 (Swansea)

/*!
  \file OKsolver_2_0_Plans.hpp
  \brief Plans for the module for the new OKsolver;
  the goal is that OKsolver_2 is among the best three solvers
  for all 3 * 3 subcompetitions.
  \todo OKsolver_2 extends OKsolver_1 by using alliances of
  active clause-sets with (at least) three members :
   - the original clause-set
   - the active clause-set for compressed r_2 learning
     (see module Learning)
   - the active clause-set for equivalence reasoning
     (see module Equivalences).
  \todo Furthermore we perform reductions on the input (at
  least r_1, subsumption elimination and equivalence
  extraction; perhaps some forms of DP-reductions; and some
  forms of autarky reductions).
  \todo Finally some form of local search must be included;
  the best would be if we could handle this via autarky search
  (then we would cover also unsatisifable problems).
*/

/*!
  \namespace OKlib::OKsolver_2_0
  \brief Applications implementing the new OKsolver (for
  SAT 2007)
*/

namespace OKlib {
  namespace OKsolver_2_0 {
  }
}

