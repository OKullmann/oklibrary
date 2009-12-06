// Oliver Kullmann, 22.12.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
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
   <li> See the investigations in
   Experimentation/Investigations/LatinSquares/plans/general.hpp. </li>
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
   <li> alo_cnf_cs(L) and amo_cnf_cs(L):
    <ol>
     <li> The simplest way of making the similarities more visible is to use
     "at most one" and "at least one" functions, which express, for a given set
     L of literals, the boolean function on the variables involved that at most
     one resp. at least one of the literals is true. </li>
     <li> Such functions obviously are connected to
     Satisfiability/FiniteFunctions; shall we place them there or here? </li>
     <li> Perhaps best in this module only to put "real" generators, that is,
     real satisfiability problems, while more basic functions like alo_cs and
     amo_cs go to FiniteFunctions. </li>
     <li> See "Cardinality constraints" in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/Thresholds.hpp
     for the implementation. </li>
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

