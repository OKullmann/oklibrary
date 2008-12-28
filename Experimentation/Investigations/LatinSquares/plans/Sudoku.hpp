// Oliver Kullmann, 1.11.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/plans/Sudoku.hpp
  \brief On investigations regarding Sudoku problems (standard, and extended)

  Besides experimentations in creating and solving big Sudoku problems (with
  or without the uniqueness constraint) one should harness the popularity
  of Sudoku to teach (generalised) satisfiability.
  
  
  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Generators/plans/Sudoku.hpp.
   </li>
   <li> See Applications/LatinSquares/plans/Sudoku.hpp. </li>
  </ul>


  \todo Finding hard instances for box-dimension 3
  <ul>
   <li> A source for "hard" instances is [extreme Su Doku; Wayne Gould]. </li>
   <li> The cover-text says "... Wayne Gould, who'll take you deep inside his
   genius psyche.", and "..., he has been named by Time Magazine as one of
   the 100 people who shaped the world.". </li>
   <li> One sees that enlightenment is needed. </li>
   <li> As one can see in
   ComputerAlgebra/Satisfiability/Lisp/Generators/docus/Sudoku.hpp, apparently
   most problems only need one r_2-reduction, while perhaps the most difficult
   instance in that collection needs five r_2-reductions (without
   backtracking, and thus also uniqueness is confirmed). </li>
   <li> We should search for problems which are as hard as possible; see
   "Sampling of minimally uniquely satisfiable problems" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/Sudoku.hpp. </li>
   <li> For the "human" solution of such problems, "explanations" are needed
   for the solution, which is just a demonstration of the sequence of forced
   assignments.
    <ol>
     <li> In that book the condition "without reductio ad absurdum"
     is stated. Can this be made precise? </li>
     <li> Perhaps r_2-reduction would not be eligible here, and perhaps
     one should use full-strength bijectivity-constraints (where then
     r_2-reduction might be applicable). </li>
     <li> Of course, one can simulate r_2-reduction via resolution, which
     is "forward reasoning"; so perhaps one should search for short
     resolution derivations of the forced assignments. </li>
    </ol>
   </li>
  </ul>

*/

