// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/15.hpp
  \brief On investigations into vdw_2(3,16) = 238


  \todo OKsolver
  <ul>
   <li> k=16, n=238: Running it with monitoring level 20 appears hopeless.
   And using the m2pp-variant doesn't make a difference, since the
   preprocessor has no effect. </li>
  </ul>


  \todo satz215
  <ul>
   <li> n=238:
   \verbatim
****the instance is unsatisfiable *****
NB_MONO= 62751, NB_UNIT= 17632939133, NB_BRANCHE= 600383827, NB_BACK= 304409259
Program terminated in 990144.910 seconds.
satz215 VanDerWaerden_2-3-16_238.cnf 990144.910 600383827 304409259 99369777769 3501709276 0 238 15812 0 3305729737 824297599
   \endverbatim
   </li>
   <li> Stably a factor of 10 (when increasing k by one). </li>
   <li> So with some parallelisation k=17 should be feasible
   (the unsatisfiable case; around 3 months on a single processor). </li>
  </ul>


  \todo minisat2
  <ul>
   <li> n=238: Apparently after 30 restarts and 36906145 conflicts
   no progress was made. </li>
  </ul>


  \todo Palindromic numbers
  <ul>
   <li> pdvanderwaerden([3,16])[2] = 237 = vanderwaerden([3,16])-1 . </li>
   <li> pdvanderwaerden([3,16])[1] = 232. </li>
   <li> OKsolver_2002 rather easily determines n=236 as satisfiable and
   n=233, 237,238 as unsatisfiable. </li>
   <li> So does satz215 (somewhat faster), march_pl and minisat2 (somewhat
   faster). </li>
   <li> "RunPdVdWk1k2 3 16 gsat-tabu 100 300000" yields
   \verbatim
Break point 1: 232
Break point 2: 237
   \endverbatim
   where all solutions were found in the first 5 runs. </li>
  </ul>

*/

