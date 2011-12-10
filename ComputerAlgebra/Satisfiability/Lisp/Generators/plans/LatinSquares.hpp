// Oliver Kullmann, 22.12.2008 (Swansea)
/* Copyright 2008, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/LatinSquares.hpp
  \brief Plans for Maxima-generators (of SAT-problems) related to latin squares problems


  \todo Relations to other modules
  <ul>
   <li> See
   ComputerAlgebra/CombinatorialMatrices/Lisp/LatinSquares/plans/general.hpp.
   </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/Sudoku.hpp. </li>
   <li> See the investigations in
   Experimentation/Investigations/LatinSquares/plans/general.hpp. </li>
  </ul>


  \todo Organisation
  <ul>
   <li> Perhaps we need a new sub-module "LatinSquares". </li>
   <li> This file would then be called "general". </li>
   <li> And also Lisp/Generators/Sudoku would be moved there. </li>
  </ul>


  \todo Different encodings
  <ul>
   <li> In "SAT translations" in
   Experimentation/Investigations/LatinSquares/plans/SelfOrthogonal.hpp
   the unary encoding of fields is mentioned (used to easily translate
   sum-conditions). </li>
   <li> This encoding should also be made available (in general) in this
   module. </li>
  </ul>


  \todo Further conditions
  <ul>
   <li> In "SAT translations" in
   Experimentation/Investigations/LatinSquares/plans/SelfOrthogonal.hpp further
   conditions for latin squares are mentioned. </li>
   <li> See
   ComputerAlgebra/CombinatorialMatrices/Lisp/LatinSquares/BasicNotions.mac
   for the precise formulation of these conditions. </li>
   <li> These conditions need SAT-translations. </li>
  </ul>


  \todo Update
  <ul>
   <li> Update ComputerAlgebra/Satisfiability/Lisp/Generators/LatinSquares.mac
   according to
   ComputerAlgebra/Satisfiability/Lisp/Generators/Sudoku.mac. </li>
   <li> Compare "Systematisation" below. </li>
  </ul>


  \todo Systematisation
  <ul>
   <li> We do very similar things for latin squares, Sudoku problems and
   %e.g., pigeonhole formulas. </li>
   <li> Cardinality constraints [1,L,inf] and [0,L,1]:
    <ol>
     <li> The simplest way of making the similarities more visible is to use
     "at most one" and "at least one" cardinality constraints, which express,
     for a given set L of literals, the boolean function on the variables
     involved that at most one resp. at least one of the literals is true.
     </li>
     <li> See
     Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp. </li>
    </ol>
   </li>
   <li> Strong hypergraph colouring </li>
   <li> Bijectivity constraints </li>
  </ul>


  \todo Extensions
  <ul>
   <li> The accompanying statistics functions are needed. </li>
   <li> Standardisation of variable names should be done in a systematic
   and efficient way; compare "Formulating 'Sudoku problem instances'" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/Sudoku.hpp. </li>
  </ul>


  \todo %Generators for latin square completion problems
  <ul>
   <li> Compare Applications/LatinSquares/plans/general.hpp. </li>
  </ul>

*/

