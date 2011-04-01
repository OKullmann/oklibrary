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


  \todo Best complete solver for vdw_2(5,k)
  <ul>
   <li> For vdw_2(4,k) in
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/general.hpp
   picosat913 seems strongest. </li>
   <li> k=5, n=178:
    <ol>
     <li> picosat913 needs 15.5 s and 491198 conflicts. </li>
     <li> precosat236 needs 55.5 seconds and 541873 conflicts. </li>
     <li> precosat-570.1 needs 65.1 seconds and 630130 conflicts. </li>
     <li> march_pl needs 97.129997 s and 51527 nodes. </li>
     <li> minisat-2.2.0 needs 119.879 s and 3698746 conflict. </li>
     <li> OKsolver_2002 needs 179.0 s and 109721 nodes. </li>
     <li> glucose needs 183.496 s and 1163241 conflicts. </li>
     <li> minisat2 needs 279.645 s and 881363 conflicts. </li>
     <li> cryptominisat needs 334.18 s and 1485531 conflicts. </li>
     <li> satz215 aborted after 800 s. </li>
    </ol>
   </li>
   <li> k=6, n=206: Aborted picosat913 after 6 days without visible progress.
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
