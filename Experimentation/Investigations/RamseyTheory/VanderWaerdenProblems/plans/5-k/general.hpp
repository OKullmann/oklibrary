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
   <li> Apparently nothing is known about vdw_2(5,k) ? Could there also be
   a conjecture that it's polynomially bounded? </li>
   <li> The known values:
   \verbatim
create_list(vanderwaerden5k(k),k,1,1);
 [unknown]
   \endverbatim
   </li>
  </ul>


  \todo Best complete solver
  <ul>
   <li> Considering k=8, it seems that from the conflict-driven solvers
   minisat-2.2.0 is best, from the look-ahead solvers march_pl (followed
   by OKsolver_2002, then satz215), and that overall minisat-2.2.0 is best.
   </li>
  </ul>


  \todo Best local-search solver
  <ul>
   <li> Best ubcsat-algorithm:
   \verbatim
> PdVanderWaerdenCNF-O3-DNDEBUG 5 8 312

> E=run_ubcsat("VanDerWaerden_pd_2-5-8_312.cnf",runs=100,cutoff=1000000)

ag2wsat
 0  1  2
57 36  7
FlipsPerSecond = 208908
paws
 0  1  2  3
52 22 25  1
FlipsPerSecond = 204942
nov
 0  1  2  3
43 20 28  9
FlipsPerSecond = 169408
novp_p
 0  1  2  3  4
39 18 27 15  1
FlipsPerSecond = 129391
novpc
 0  1  2  3  4
36 11 29 23  1
FlipsPerSecond = 206143
anovp
 0  1  2
35 48 17
FlipsPerSecond = 172125
novp
 0  1  2  3
34 16 35 15
FlipsPerSecond = 169951
anovpp
 0  1  2
33 41 26
FlipsPerSecond = 170963
ddfw
 0  1  2
29 59 12
FlipsPerSecond = 89456
dano
 0  1  2
27 44 29
FlipsPerSecond = 169269
g2wsat
 0  1  2  3
27 23 44  6
FlipsPerSecond = 221298
   \endverbatim
   (segmentation faults with saps and sapsnr).So adaptg2wsat seems clearly
   strongest. </li>
  </ul>

*/
