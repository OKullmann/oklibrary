// Oliver Kullmann, 9.8.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/CombinatorialMatrices/Lisp/plans/Isomorphisms.hpp
  \brief Plans for isomorphisms of (combinatorial) matrices


  \todo Isomorphism testing by backtracking
  <ul>
   <li> We need a backtracking isomorphism solver, as a generalised SAT
   solver. </li>
   <li> Compare
   ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp. </li>
  </ul>


  \todo Connections to other modules
  <ul>
   <li> Via isomorphism of combinatorial matrices we can decide var-isomorphism
   of labelled clause-sets. </li>
   <li> And isomorphism of general hypergraphs is the special case where only
   {0,1}-matrices are involved. </li>
  </ul>

*/

