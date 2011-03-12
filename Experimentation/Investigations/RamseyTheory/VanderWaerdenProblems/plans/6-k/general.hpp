// Oliver Kullmann, 12.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/6-k/general.hpp
  \brief On investigations into vdW-numbers vdw_2(6,k)

  Instances created by output_vanderwaerden2nd_stdname(6,k,n) at Maxima-level,
  or by "VanderWaerden-O3-DNDEBUG 6 k n" at C++ level (instances differ by
  the order of negated literals in clauses).
  An ubcsat-experiment is run by "RunVdWk1k2 6 k n0 alg runs cutoff".

  The palindromic versions are created by
  output_pd_vanderwaerden2nd_stdname(6,k,n) at Maxima-level, or by
  "PdVanderWaerdenCNF-O3-DNDEBUG 6 k n" at C++ level (instances differ by
  the order of negated literals in clauses).
  An ubcsat-experiment is run by "RunPdVdWk1k2 6 k n0 alg runs cutoff".
  And running a complete solver by "CRunPdVdWk1k2 6 k solver".


  \todo Literature overview
  <ul>
   <li> Apparently nothing is known about vdw_2(6,k) ? </li>
   <li> Our conjecture is that it's polynomially bounded, </li>
   <li> The known values:
   \verbatim
create_list(vanderwaerden6k(k),k,1,1);
 [unknown]
   \endverbatim
   </li>
  </ul>


  \todo Best complete solver for palindromic problems


  \todo Best local-search solver for palindromic problems

*/
