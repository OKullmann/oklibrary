// Oliver Kullmann, 10.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Competition/TwoSolverComparison/plans/TwoSolverComparison.hpp
  \brief Plans regarding the method for comparing two solvers according to [Mladen Nikolic, 2010]


  \todo Implementing [Mladen Nikolic, 2010]
  <ul>
   <li> [Mladen Nikolic, 2010, Statistical Methodology for Comparison of
   SAT Solvers] is to appear in SAT 2010 proceedings. </li>
   <li> The first stage is to implement the statistical evaluation in R, as
   part of the "SAT Evaluation Toolkit" (see
   Experimentation/Competition/plans/general.hpp). </li>
   <li> It appears that the input is just a data-frame file, which per row
   contains the times for the two solvers A, B to be compared. </li>
   <li> Is the knowledge of the cutoff-time needed for the evaluation? </li>
   <li> As discussed in the paper, there are actually dependencies between
   the rows, in that a block of rows belongs to a single instances and its
   shuffled variations --- however it seems for the computations performed
   this is irrelevant? </li>
  </ul>

*/
