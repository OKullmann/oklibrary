// Oliver Kullmann, 23.11.2006 (Swansea)

/*!
  \file LatinSquares/plans/Sudoku.hpp
  \brief Plans for the Sudoku components

  \todo As a prototype, implement Sudoku-solvers.

  \todo Complete implementation of OKlib::LatinSquares::SudokuProblem
  - Are all constraints stored together? Perhaps better we use the natural grouping
  into 3 groups?

  Compare with SATAlgorithms/plans/GenericBacktracking.hpp and with
  Concepts/plans/ClauseSets.hpp for the concept discussion.

  \todo Test OKlib::LatinSquares::SudokuProblem

  \todo Instead of just injectivity-constraints the stronger form of bijectivity-constraints
  should be used.

  \todo Complete implementation of OKlib::LatinSquares::Trivial_reduction_Sudoku
  Likely this should be just a generic algorithm, applicable to any collection of
  constraints (active clause-sets)?!

  \todo Test OKlib::LatinSquares::Trivial_reduction_Sudoku

  \todo Another representation of Sudoku uses one Latin square plus N*N bijectivity constraints (for the boxes).

*/

