// Oliver Kullmann, 23.11.2006 (Swansea)

/*!
  \file LatinSquares/plans/Sudoku.hpp
  \brief Plans for the Sudoku components

  \todo Input file format:
   - One possible format is:
   3
   .94...13..............76..2.8..1.....32.........2...6.....5.4.......8..7..63.4..8
   - Alternatively, a more human-readable format is:
   3
   .94...13.
   .........
   ....76..2
   .8..1....
   .32......
   ...2...6.
   ....5.4..
   .....8..7
   ..63.4..8
   - In both cases the first line gives the box size.

  \todo Complete implementation of OKlib::LatinSquares::SudokuProblem
  - Given n in N, let I_1 := {1, ..., n} and I_2 := I_1^2, I_2' := {1,...,n^2}.
  Variables are v_{i,j} for i, j in I_2 with domains D(v) = I_2'.
  The active clause-sets are
   1a) For all i in I_2: INJ({v_{i,j} : j in I_2})
   1b) For all j in I_2: INJ({v_{i,j} : i in I_2})
   2) For all i, j in I_1: INJ( { v_{(i,k), (k',j)} : k, k' in I_1 } ).
  Additionally a list of domain-restrictions can be specified.
  In terms of latin squares we can express 1a+1b as
   1) LS((v_ij)_ij).
  - Are all constraints stored together? Perhaps better we use the natural grouping
  into 3 groups?
  - Use the concepts of literals (what about entry_type?).
  - Compare with SATAlgorithms/plans/GenericBacktracking.hpp and with
    Concepts/plans/ClauseSets.hpp for the concept discussion.

  \todo Complete implementation of OKlib::LatinSquares::Trivial_reduction_Sudoku
  Likely this should be just a generic algorithm, applicable to any collection of
  constraints (active clause-sets)?!

  \todo Test OKlib::LatinSquares::SudokuProblem

  \todo Test OKlib::LatinSquares::Trivial_reduction_Sudoku

  \todo Strong hypergraph colouring
  It seems that everything we are doing can be done at the more general
  level of strong hypergraph colouring (see "Strong hypergraph colouring"
  in LatinSquares/plans/LatinSquares.hpp). So it seems sensible to generalise
  everything, and then to derive the Sudoku components by just instantiating
  the hypergraph colouring problem (using 3 N^2 hyperedges).
  Is there anything special for the Sudoku problem?

  \todo Another representation of Sudoku uses one Latin square plus N*N bijectivity constraints (for the boxes).

  \todo As a prototype, implement Sudoku-solvers.

*/

