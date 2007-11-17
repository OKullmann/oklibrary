// Oliver Kullmann, 23.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Algorithms/Backtracking/plans/GenericBacktracking.hpp
  \brief Plans for the old module on generic backtracking algorithms

  Needs update.


  \todo Class OKlib::SATAlgorithms::Backtracking
  <ul>
   <li> Design and implement this generic algorithm skeleton. </li>
   <li> Parameter ExtProblem of OKlib::SATAlgorithms::Backtracking
    <ol>
     <li> What is the concept of ExtProblem, and how does it compare to "active
     clause-sets" ? </li>
     <li> An example is OKlib::LatinSquares::SudokuProblem in
     LatinSquares/Sudoku.hpp. </li>
     <li> The point here seems to be that out of several "active clause-sets",
     as for example OKlib::InjectivityConstraints::TrivialAllDifferent,
     one builds than "alliances of active clause-sets", 
     which integrate the different constraints, and kind of organise them. </li>
     <li> It seems unlikely, that for example a Sudoku problem is used as an
     active clause-set for another problem. </li>
     <li> See Concepts/plans/ClauseSets.hpp for the concept of active clause-sets.
     </li>
     <li> The operations for ExtProblem used in OKlib::SATAlgorithms::Backtracking
     are:
      <ol>
       <li> the reduction can be run on it </li>
       <li> the visitor can extract satisfying assignments from it (and also
       falsifying ones) </li>
       <li> the heuristics can run on it </li>
       <li> P.undo() </li>
       <li> P.add_assignment(l) </li>
       <li> P.add_negated_assignment(l) </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Undo:
    <ol>
     <li> The semantics of the undo-operation needs to be clarified. </li>
    </ol>
   </li>
   <li> Return value of the reduction
    <ol>
     <li> Perhaps the reduction should return something more informative and
     general, as discussed under "Sat status" in Concepts/plans/ClauseSets.hpp.
     </li>
     <li> On the other hand, the reduction for an alliance doesn't need to give
     these details? </li>
     <li> We just need to know whether it was already decided. </li>
    </ol>
   </li>
  </ul>


  \todo Intelligent backtracking and learning
  <ul>
   <li> Yet OKlib::SATAlgorithms::Backtracking does not support intelligent
   backtracking or learning? </li>
   <li> Perhaps the visitor can do it? </li>
   <li> The role of the visitor yet is to do something when a falsifying or
   satisfying assignment is detected; perhaps one should speak of a
   "decision-visitor". </li>
  </ul>


  \todo Integration
  <ul>
   <li> The integration of the reduction and the heuristics is problematic
   (they need to know about each other and the problem). </li>
  </ul>


  \todo Goal
  <ul>
   <li> OKlib::SATAlgorithms::Backtracking should be usable to process
    <ol>
     <li> Sudoku and latin squares problem (see
     LatinSquares/plans/Sudoku.hpp) </li>
     <li> the main part of OKsolver_2.0
     (see OKsolver_2_0/plans/OKsolver_2_0.hpp) </li>
    </ol>
   </li>
  </ul>


  \todo Test OKlib::SATAlgorithms::Backtracking


  \todo More flexible splitting
  <ul>
   <li> Yet we only split by (v = e, v<> e). Now it seems that we could achieve
   an exponential speed-up by using splittings (v in A, v in B) for partitions
   (A,B) of D_v; the problem is only how to find good candidates? </li>
   <li> Perhaps first we perform some kind of exhaustive tests, to judge the
   potential. </li>
   <li> A reasonable heuristics good be obtained as follows: For performing
   r_2 we check all partial assignments v = e; now we need to value each such
   branch, and then via some balancing scheme we distribute the values of A
   and B such that the sums of values in A and B are as close as possible. </li>
  </ul>

*/

