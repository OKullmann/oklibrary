// Oliver Kullmann, 25.2.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_2-3-7.hpp
  \brief Investigations on greentao_2(3,7)


  \todo Establishing the target
  <ul>
   <li> Analysing the sequence 4,7,23,79,528,2072 by linear regressions seems
   to suggest it is linear in the log-log, and a linear model then is
   exp(exp(-0.05899 + i * 0.38269)), which would yield 11691 as the
   next term. </li>
   <li> I would guess that this is too big (more like 6000). </li>
  </ul>


  \todo Finding the best algorithm from ubcsat
  <ul>
   <li> adaptnovelty+ seems best:
  </ul>


  \todo Survey propagation
  <ul>
   <li> For n=5000, 7500, 10000 survey propagation apparently thinks the
   problem is unsatisfiable, i.e., we always get
   \verbatim
> survey_propagation -l GreenTao_2-3-7_XX000.cnf
<bias>:nan
fixed 1 biased var (+0 ucp)
.:-)
<bias>:nan
contradiction
   \endverbatim
   However these problems are all satisfiable. </li>
  </ul>


  \todo greentao_2(3,7) > 10000
  <ul>
   <li> n=5000: ubcsat::adaptnovelty+ finds it satisfiable with just a
   cutoff=10000. </li>
   </li>
   <li> n=7500: ubcsat::adaptnovelty+ finds it satisfiable with just a
   cutoff=100000. </li>
   </li>
   <li> n=10000: ubcsat::adaptnovelty+ finds it satisfiable with just a
   cutoff=1000000 (seed=3610283050). </li>
   <li> n=12500 </li>
  </ul>

*/

