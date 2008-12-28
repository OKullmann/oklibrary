// Oliver Kullmann, 23.11.2006 (Swansea)
/* Copyright 2006 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/LatinSquares/plans/Sudoku.hpp
  \brief Plans for the Sudoku components


  \todo Relations
  <ul>
   <li> See "Relations" in Hypergraphs/Matchings/plans/general.hpp. </li>
   <li> See "Sudoku" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Generators.hpp.
   </li>
   <li> See investigations in
   Experimentation/Investigations/LatinSquares/plans/Sudoku.hpp. </li>
  </ul>


  \todo Input file format:
  <ul>
   <li>
    <ol>
    <li> One possible format is:
    \verbatim
3
.94...13..............76..2.8..1.....32.........2...6.....5.4.......8..7..63.4..8
    \endverbatim
    </li>
    <li> Alternatively, a more human-readable format is:
    \verbatim
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
     \endverbatim
    </li>
    <li> In both cases the first line gives the box size. </li>
    </ol>
   </li>
   <li> OK: I do not understand the meaning of the above. One also needs to
   be more precise:
    <ol>
     <li> Obviously numbers are involved, so their syntax must be
     specified. </li>
     <li> Then the syntax of the separation must be specified (spaces, commas
     (dots??); what about end-of-lines?). </li>
     <li> Finally, for such simple file formats, the order and semantics of
     entries is to be specified. </li>
    </ol>
   </li>
   <li> Likely XML is more approriate; a DTD-specification should be fully
   sufficient here, and tag-names can be (very) short. </li>
   <li> Perhaps there are two formats: A "professional" XML format, and a
   "quick-and-dirty" text format. </li>
  </ul>


  \todo Complete implementation of OKlib::LatinSquares::SudokuProblem
  <ul>
   <li> Compare the handling of coordinates with
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Generators.mac. </li>
   <li> Given n in N, let I_1 := {1, ..., n} and I_2 := I_1^2,
   I_2' := {1,...,n^2}. </li>
   <li> Variables are v_{i,j} for i, j in I_2 with domains D(v) = I_2'. </li>
   <li> The active clause-sets are
    <ol>
     <li> Inj(rows): For all i in I_2: INJ({v_{i,j} : j in I_2}). </li>
     <li> Inj(columns): For all j in I_2: INJ({v_{i,j} : i in I_2}). </li>
     <li> Inj(boxes): For all i, j in I_1:
     INJ( { v_{(i,k), (k',j)} : k, k' in I_1 } ). </li>
    </ol>
   </li>
   <li> Additionally a list of domain-restrictions can be specified. </li>
   <li> In terms of latin squares we can express Inj(rows)+Inj(columns) as
   LS((v_ij)_ij). </li>
   <li> Are all constraints stored together? Perhaps better we use the natural
   grouping into 3 groups? </li>
   <li> Use the concepts of literals (what about entry_type?). </li>
   <li> Compare with SATAlgorithms/plans/GenericBacktracking.hpp and with
   Concepts/plans/ClauseSets.hpp for the concept discussion. </li>
  </ul>


  \todo Complete implementation of
  OKlib::LatinSquares::Trivial_reduction_Sudoku
  <ul>
   <li> Likely this should be just a generic algorithm, applicable to any
   collection of constraints (active clause-sets)?! </li>
  </ul>


  \todo Test OKlib::LatinSquares::SudokuProblem


  \todo Test OKlib::LatinSquares::Trivial_reduction_Sudoku


  \todo Strong hypergraph colouring
  <ul>
   <li> It seems that everything we are doing can be done at the more general
   level of strong hypergraph colouring (see "Strong hypergraph colouring"
   in LatinSquares/plans/general.hpp). </li>
   <li> So it seems sensible to generalise
   everything, and then to derive the Sudoku components by just instantiating
   the hypergraph colouring problem (using 3 N^2 hyperedges).
   Is there anything special for the Sudoku problem? </li>
  </ul>


  \todo Another representation of Sudoku uses one Latin square plus
  N*N bijectivity constraints (for the boxes).


  \todo As a prototype, implement Sudoku-solvers.


  \todo Visualisation of the runs of solvers
  <ul>
   <li> Boolean SAT-solvers
    <ol>
     <li> We have the general possibilities of visualising the run of
     a SAT solver. See part Visualisation (especially
     GraphDrawing/plans/general.hpp). </li>
     <li> Additionally, we should try to exploit the special geometrical
     structure. </li>
     <li> One could visualise the current partial assignment for a node
     (online):
      <ul>
       <li> One could represent the p^6 variables by a p^3 x p^3 board. </li>
       <li> A variable, of course, can be unassigned, 0 or 1. </li>
       <li> And it can be a decision variable on the first branch, on
       the second branch, or inferred (this for look-ahead solvers). </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> Solvers with variables with domains {1,...,p^2} (assuming that this
   represents also the splitting possibilities, i.e., the literals are
   monosigned)
    <ol>
     <li> Again we have the general possibilities from part Visualisation.
     </li>
     <li> Again representing the partial assignments, one could use a
     more condensed representation "unassigned vs. assigned" plus the
     above distinctions regarding decision variables and inferred
     variables. </li>
    </ol>
   </li>
  </ul>

*/

