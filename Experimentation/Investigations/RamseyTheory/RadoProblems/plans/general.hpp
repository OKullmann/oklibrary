// Oliver Kullmann, 11.7.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/RadoProblems/plans/general.hpp
  \brief General plans regarding investigations on Rado problems

  The standard generalised Rado problems are given by Rado parameter lists
  [A_1, ..., A_m], where each A_i is a matrix of integers of dimension
  r_i x p_i, demanding a p_i-tuple x with A_i * x = 0.

  The two main cases are the "full form" with m=1, and the "generalised form"
  where all r_i = 1. For these two cases it is known when the Ramsey-property
  holds and when not.

  For the injective versions we demand the components of x to be different.

  The numbers are called rado_m(A_1, ..., A_m) resp. radoi_m(A_1, ..., A_r).


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/Generators/plans/Rado.hpp for
   plans regarding the hypergraph generators. </li>
   <li> Schur numbers are special cases; see
   Experimentation/Investigations/RamseyTheory/SchurProblems/plans/general.hpp.
   </li>
   <li> And also Folkman numbers are special cases; see
   Experimentation/Investigations/RamseyTheory/FolkmanProblems/plans/general.hpp.
   </li>
  </ul>


  \todo Density versions
  <ul>
   <li> What is known about the independence numbers of the hypergraphs? </li>
  </ul>

*/

