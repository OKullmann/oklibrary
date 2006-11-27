// Oliver Kullmann, 23.11.2006 (Swansea)

/*!
  \file LatinSquares/plans/Sudoku.hpp
  \brief Plans for the Sudoku components

  \todo Complete implementation of OKlib::LatinSquares::SudokuProblem
  - Are all constraints stored together? Perhaps better we use the natural grouping
  into 3 groups?

  Compare with SATAlgorithms/plans/GenericBacktracking.hpp and with
  Concepts/plans/ClauseSets.hpp for the concept discussion.

  \todo Strong hypergraph colouring
  It seems that everything we are doing can be done at the more general
  level of strong hypergraph colouring (see "Strong hypergraph colouring"
  in LatinSquares/plans/LatinSquares.hpp). So it seems sensible to generalise
  everything, and then to derive the Sudoku components by just instantiating
  the hypergraph colouring problem (using 3 N^2 hyperedges).
  Is there anything special for the Sudoku problem?

  \todo Test OKlib::LatinSquares::SudokuProblem

  \todo Complete implementation of OKlib::LatinSquares::Trivial_reduction_Sudoku
  Likely this should be just a generic algorithm, applicable to any collection of
  constraints (active clause-sets)?!

  \todo Test OKlib::LatinSquares::Trivial_reduction_Sudoku

  \todo Another representation of Sudoku uses one Latin square plus N*N bijectivity constraints (for the boxes).

  \todo As a prototype, implement Sudoku-solvers.

*/

