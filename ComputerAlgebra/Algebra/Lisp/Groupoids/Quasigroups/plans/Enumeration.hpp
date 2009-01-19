// Oliver Kullmann, 15.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/Quasigroups/plans/Enumeration.hpp
  \brief Plans regarding enumeration of quasi-groups


  \todo Enumeration and classification
  <ul>
   <li> For small order (say, n <= 5) it should be possible to enumerate
   all (isomorphism types) of quasigroups (and also the total count is
   interesting), and providing a catalogue. </li>
   <li> Of course, we also need the numbers (total and non-isomorphic) of
   unital quasigroups, groups, commutative quasigroups, unital commutative
   quasigroups, and commutative groups. </li>
   <li> Implement enumeration functions according to "All isomorphism types of
   small groupoids" in
   ComputerAlgebra/Algebra/Lisp/Groupoids/plans/Enumeration.hpp. </li>
   <li> What is the relation to the module for latin squares (see
   ComputerAlgebra/CombinatorialMatrices/Lisp/LatinSquares/plans/general.hpp)?
    <ol>
     <li> The general idea would be that here we enumerate everything with
     an "algebraic flavour", while there we handle everything with a
     "combinatorial flavour". </li>
    </ol>
   </li>
  </ul>


  \todo Random generation of quasigroups and unital quasigroups
  <ul>
   <li> Producing random quasigroups and random unital quasigroups of
   bigger order must have been studied in the literature. </li>
   <li> One could run a (generalised) SAT solver, for sampling the
   solutions of the condition "(unital) quasigroup". </li>
   <li> Given a quasigroup, one can consider random isotopic images (choosing
   three random permutations); in this way one can sample the isotopy class.
   </li>
  </ul>

*/

