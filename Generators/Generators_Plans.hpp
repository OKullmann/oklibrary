// Oliver Kullmann, 20.11.2005 (Swansea)

/*!
  \file Generators_Plans.hpp
  \brief Plans for the module Generators.
  \todo Transferring (and updating):
   - OKsolver/Experimental/Transformationen/Ramsey
   - OKsolver/Experimental/Transformationen/VanderWaerden
   - OKsolver/Experimental/Transformationen/LinInequal
   - OKsolver/Experimental/Transformationen/FaronsPuzzle
  The main generators should produce generalised clause-sets.
  If output and/or boolean formulas is needed, then further
  transformers are to be used.
  The variables are tuples of values.
  \todo Especially for Ramsey- and Van der Waerden- formulas
  the generators should be virtual clause-sets usable by some SAT solver.
  \todo Important virtual clause-sets are given by INJ(V, D) for
  some set V of variables with domains D(v), expressing that
  different variables get different values, and BIJ(V, D, Val),
  refining INJ(V, D) by also requiring that every value in Val
  must be used. Virtual clause-sets can offer different levels
  of inference powers. For "full inference power" essentially INJ
  and BIJ are the same (in case D(v) <= Val and |V| = |Val|),
  but for weaker systems there might be a difference.
  The direct clause-representation of INJ(V,D) uses the clauses
  {(v != eps) or (v' != eps)} for v != v' and eligible values eps,
  while BIJ adds the surjectivity P-clauses {(v = eps)}_{v in V} for
  every eps in Val.
  \todo Transformers from non-boolean clause-sets to boolean
  clause-sets are needed (the standard translation, with or without
  the at-most-one-clauses, and also the other translations known from
  the literature).
  \todo Sudoku:
  Given n in N, let I_1 := {1, ..., n} and I_2 := I_1^2, I_2' := {1,...,n^2}.
  Variables are v_{i,j} for i, j in I_2 with domains D(v) = I_2'.
  The clauses are
   1a) For all i in I_2: INJ({v_{i,j} : j in I_2})
   1b) For all j in I_2: INJ({v_{i,j} : i in I_2})
   2) For all i, j in I_1: INJ( { v_{(i,k), (k',j)} : k, k' in I_1 } ).
  Additionally a list of domain-restrictions can be specified.
*/

