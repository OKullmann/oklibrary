// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/19.hpp
  \brief On investigations into vdw_2(3,19) = 349


  \todo vanderwaerden_2(3,19) >= 349
  <ul>
   <li> This value has been verified by Tanbir Ahmed. </li>
   <li> Experience with k=18 is that samd is best, however we should
   have a look again --- this might change with different k's or with
   different cutoff's. </li>
   <li> Evaluating
   \verbatim
E = run_ubcsat("VanDerWaerden_2-3-19_345.cnf", runs=100,cutoff=10000,monitor=TRUE)
   \endverbatim
   by plot(E$alg,E$best), the best algorithms seem gsat_tabu, rnovelty and
   rsaps (all three found solutions). </li>
   </li>
   <li> Evaluating
   \verbatim
E = run_ubcsat("VanDerWaerden_2-3-19_347.cnf", runs=100,cutoff=10000,monitor=TRUE)
   \endverbatim
   by plot(E$alg,E$best), now the best algorithms seem gsat_tabu, rnovelty,
   rnoveltyp, saps, rsaps and samd. </li>
   <li> Using cutoff=100000:
   \verbatim
E = run_ubcsat("VanDerWaerden_2-3-19_347.cnf", runs=100,cutoff=100000,monitor=TRUE)
   \endverbatim
   by plot(E$alg,E$best), now the best algorithms seem gsat_tabu,
   adaptnoveltyp, rnoveltyp, rsaps, rots and samd (all found a solution).
   gsat_tabu seems the best, followed by samd. </li>
   <li> Evaluating
   \verbatim
E = run_ubcsat("VanDerWaerden_2-3-19_348.cnf", runs=100,cutoff=100000,monitor=TRUE)
   \endverbatim
   now only two algorithms found a solution, gsat_tabu and samd, and these
   two seem to be best (hard to distinguish between them). </li>
   <li> Evaluating
   \verbatim
> E = run_ubcsat("VanDerWaerden_2-3-19_349.cnf", runs=100,cutoff=100000,monitor=TRUE)
   \endverbatim
   adaptnoveltyp, rnovelty, rnoveltyp hard to distinguish, gsat_tabu has higher
   variance, the bulk of rsaps' results are far worse, but outliers reach also
   min=1, rots seems a bit worse than the novelty's and gsat_tabu, and finally
   samd seems best. </li>
   <li> Using cutoff=10^6: Now gsat_tabu is best, followed by samd (all
   just reaching min=1). </li>
   <li> So let's assume that gsat-tabu is the best. </li>
   <li> n=349:
    <ol>
     <li> Using gsat-tabu, with cutoff=10^6:
     \verbatim
   1    2    3    4    9
1143  834   20    2    1
2000
     \endverbatim
     </li>
     <li> gsat-tabu with cutoff=4*10^6:
     \verbatim
   1    2    3
1909   88    3
2000
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Palindromic numbers
  <ul>
   <li> vanderwaerden_g([3,19],"pd") = 347 = vanderwaerden([3,19])-2. </li>
   <li> picosat913:
    <ol>
     <li> n=348: unsat, 707s. </li>
     <li> n=347: unsat, 85s. </li>
     <li> n=346: sat, 524s. </li>
    </ol>
   </li>
   <li> Other running times (for n=348,347,346):
    <ol>
     <li> minisat2 (960s, 243s, 932s) </li>
     <li> precosat236 (408s, 68s, 202s) </li>
     <li> march_pl (3600s, 1384s, 2598s) </li>
     <li> satz215 (1476s, 407s, 869s) </i>
     <li> OKsolver_2002 (11757s, 2295s, 963s) </li>
    </ol>
   </li>
   <li> So apparently actually precosat236 is best. </li>
  </ul>

*/
