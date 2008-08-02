// Oliver Kullmann, 29.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Autarkies/plans/LeanKernel.hpp
  \brief Plans regarding computations of lean kernels


  \todo Lean kernel via variable-elimination
  <ul>
   <li> Implement the computation of the lean kernel via a SAT-solver,
   which in the unsat-case returns a set of variables used by some
   resolution refutation. </li>
  </ul>


  \todo Lean kernel via oracle for leanness-decision
  <ul>
   <li> See [Kullmann 2003 (DAM)], Lemma 8.6. </li>
   <li> In [Kullmann, CSR 12-2007], Lemma 3.1 this is generalised to
   non-boolean clause-sets. </li>
  </ul>

*/

