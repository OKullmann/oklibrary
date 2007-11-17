// Oliver Kullmann, 23.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file SATAlgorithms/plans/GenericBacktracking.hpp
  \brief Plans for the module on generic backtracking algorithms

  \todo OKlib::SATAlgorithms::Backtracking
  Design and implement this generic algorithm skeleton.

  \todo Parameter ExtProblem of OKlib::SATAlgorithms::Backtracking:
  What is the concept of ExtProblem, and how does it compare to "active clause-sets" ?
  An example is OKlib::LatinSquares::SudokuProblem in LatinSquares/Sudoku.hpp.
  The point here seems to be that out of several "active clause-sets", as for example
  OKlib::InjectivityConstraints::TrivialAllDifferent, one builds than

     "alliances of active clause-sets",

  which integrate the different constraints, and kind of organise them.
  It seems unlikely, that for example a Sudoku problem is used as an active clause-set
  for another problem.

  See Concepts/plans/ClauseSets.hpp for the concept of active clause-sets.

  The operations for ExtProblem used in OKlib::SATAlgorithms::Backtracking are:
   - the reduction can be run on it
   - the visitor can extract satisfying assignments from it (and also falsifying ones)
   - the heuristics can run on it
   - P.undo()
   - P.add_assignment(l)
   - P.add_negated_assignment(l)

  \todo Undo:
  The semantics of the undo-operation of OKlib::SATAlgorithms::Backtracking needs to be
  clarified.

  \todo Return value of the reduction
  Perhaps the reduction should return something more informative and general, as
  discussed under "Sat status" in Concepts/plans/ClauseSets.hpp.
  On the other hand, the reduction for an alliance doesn't need to give these details?
  We just need to know whether it was already decided.

  \todo Intelligent backtracking and learning:
  Yet OKlib::SATAlgorithms::Backtracking does not support intelligent backtracking
  or learning? Perhaps the visitor can do it?
  The role of the visitor yet is to do something when a falsifying or satisfying assignment
  is detected; perhaps one should speak of a "decision-visitor".

  \todo Integration:
  The integration of the reduction and the heuristics is problematic (they need to know
  about each other and the problem).

  \todo Goal:
  OKlib::SATAlgorithms::Backtracking should be usable to process
   - Sudoku and latin squares problem (see LatinSquares/plans/Sudoku.hpp)
   - the main part of OKsolver_2.0 (see OKsolver_2_0/plans/OKsolver_2_0.hpp)

  \todo Test OKlib::SATAlgorithms::Backtracking

  \todo More flexible splitting:
  Yet we only split by (v = e, v<> e). Now it seems that we could achieve an exponential speed-up
  by using splittings (v in A, v in B) for partitions (A,B) of D_v; the problem is only how to find
  good candidates?
   - Perhaps first we perform some kind of exhaustive tests, to judge the potential.
   - A reasonable heuristics good be obtained as follows: For performing r_2 we check all
     partial assignments v = e; now we need to value each such branch, and then via some
     balancing scheme we distribute the values of A and B such that the sums of values in A
     and B are as close as possible.

*/

