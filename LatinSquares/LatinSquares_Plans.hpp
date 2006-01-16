// Oliver Kullmann, 16.1.2006 (Swansea)

/*!
  \file LatinSquares_Plans.hpp
  \brief Plans for the module on latin squares and generalisations
  \todo By combinining injectivity (bijectivity) constraints,
  the extension problem for latin squares and the problem of finding
  mutually orthogonal sets of latin squares shall be provided as
  an alliance of active clause-sets.
  \todo Sudoku:
  Given n in N, let I_1 := {1, ..., n} and I_2 := I_1^2, I_2' := {1,...,n^2}.
  Variables are v_{i,j} for i, j in I_2 with domains D(v) = I_2'.
  The clauses are
   1a) For all i in I_2: INJ({v_{i,j} : j in I_2})
   1b) For all j in I_2: INJ({v_{i,j} : i in I_2})
   2) For all i, j in I_1: INJ( { v_{(i,k), (k',j)} : k, k' in I_1 } ).
  Additionally a list of domain-restrictions can be specified.
*/

/*!
  \namespace OKlib::LatinSquares
  \brief Tools for latin squares and generalisations and refinements.
*/

namespace OKlib {
  namespace LatinSquares {
  }
}

