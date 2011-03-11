// Oliver Kullmann, 9.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/general.hpp
  \brief On investigations into vdW-numbers vdw_2(5,k)

  Instances created by output_vanderwaerden2nd_stdname(5,k,n) at Maxima-level,
  or by "VanderWaerden-O3-DNDEBUG 5 k n" at C++ level (instances differ by
  the order of negated literals in clauses).
  An ubcsat-experiment is run by "RunVdWk1k2 5 k n0 alg runs cutoff".

  The palindromic versions are created by
  output_pd_vanderwaerden2nd_stdname(5,k,n) at Maxima-level, or by
  "PdVanderWaerdenCNF-O3-DNDEBUG 5 k n" at C++ level (instances differ by
  the order of negated literals in clauses).
  An ubcsat-experiment is run by "RunPdVdWk1k2 5 k n0 alg runs cutoff".
  And running a complete solver by "CRunPdVdWk1k2 5 k solver".


  \todo Literature overview
  <ul>
   <li> Apparently nothing is known about vdw_2(5,k) ? </li>
   <li> Our conjecture is that it's polynomially bounded, </li>
   <li> The known values:
   \verbatim
create_list(vanderwaerden5k(k),k,1,1);
 [unknown]
   \endverbatim
   </li>
  </ul>


  \todo Best complete solver for palindromic problems
  <ul>
   <li> Considering k=8, it seems that from the conflict-driven solvers
   minisat-2.2.0 is best, from the look-ahead solvers march_pl (followed
   by OKsolver_2002, then satz215), and that overall minisat-2.2.0 is best.
   See Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/08.hpp.
   </li>
  </ul>


  \todo Best local-search solver for palindromic problems
  <ul>
   <li> adaptg2wsat seems clearly strongest for k=8 (see
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/08.hpp). </li>
  </ul>

*/
