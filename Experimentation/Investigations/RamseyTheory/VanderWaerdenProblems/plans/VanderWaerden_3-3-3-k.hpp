// Oliver Kullmann, 22.4.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_3-3-3-k.hpp
  \brief On investigations into vdW-numbers vdw_3(3,3,k)

  Generated as non-boolean formal clause-set with uniform domain via
  vanderwaerden_nbfcsud([3,3,k],n) at Maxima-level.

  Using the (strong) standard translation, a standardised boolean clause-set
  is obtained by vanderwaerden_aloamo_stdfcs([3,3,k],n).

  Using the standard nested translation, a standardised boolean clause-set
  is obtained by vanderwaerden_standnest_stdfcs([3,3,k],n).

  File-output is obtained by output_vanderwaerden_stdname([3,3,k],n) resp.
  output_vanderwaerden_standnest_stdname([3,3,k],n).
  
  
  \todo Literature overview
  <ul>
   <li> Apparently nothing is known about vdw_3(3,3,k) ? Could there also be
   a conjecture that it's polynomially bounded? </li>
   <li> The known values:
   \verbatim
create_list(vanderwaerden33k(k),k,1,6);
 [9,14,27,51,80,unknown]
   \endverbatim
   </li>
   <li> [Ahmed 2009] states
    <ol>
     <li> vanderwaerden33k(6) > 105 </li>
     <li> vanderwaerden33k(7) > 142 </li>
     <li> vanderwaerden33k(8) > 160 </li>
    </ol>
   </li>
   <li> In [Landman, Robertson; Ramsey Theory on the Integers] we find
   "vanderwaerden_3(3,3,5) = 77", however below we see
   vanderwaerden_3(3,3,5) > 79. </li>
  </ul>


  \todo vanderwaerden_3(3,3,3) = 27
  <ul>
   <li> Trivial for OKsolver_2002; easier in standard translation (707 nodes)
   than in standard nested translation (2107 nodes). </li>
  </ul>


  \todo vanderwaerden_3(3,3,4) = 51
  <ul>
   <li> OKsolver_2002
    <ol>
     <li> Standard translation: Finds a satisfying assignment for n=50 using
     31531 nodes (8s), and determines unsatisfiability using 46857 nodes. </li>
     <li> Standard nested translation: Finds a satisfying assignment for n=50
     using 192947 nodes (27s), and determines unsatisfiability using 551396
     nodes. </li>
    </ol>
   </li>
  </ul>


  \todo vanderwaerden_3(3,3,5) = 80
  <ul>
   <li> First let's consider n=79 in the standard translation. </li>
   <li> Best Ubcsat-solver for cutoff=10^5:
   \verbatim
> E = run_ubcsat("VanDerWaerden_3-3-3-5_79.cnf")
   \endverbatim
   evaluated by plot(E$alg,E$best): No solver found a solution, best is saps:
   \verbatim
table(E$best[E$alg=="saps"])
 1  2  3
 7 58 35
table(E$best[E$alg=="sapsnr"])
 1  2  3
 5 55 40
 table(E$best[E$alg=="hwsat"])
 1  2  3  4
 3 67 28  2
table(E$best[E$alg=="adaptnoveltyp"])
 1  2
 2 98
table(E$best[E$alg=="gwsat"])
 1  2
 2 98
table(E$best[E$alg=="rsaps"])
 1  2  3  4
 2 37 60  1
table(E$best[E$alg=="novelty"])
 1  2  3
 1 72 27
   \endverbatim
   </li>
   <li> saps:
    <ol>
     <li> cutoff=2*10^5
     \verbatim
 1  2  3
 9 82  9
100
     \endverbatim
     </li>
     <li> cutoff=4*10^5
     \verbatim
 1  2  3
17 80  3
100
     \endverbatim
     </li>
     <li> cutoff=8*10^5
     \verbatim
 1  2
30 70
100
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Now let's consider n=79 in the standard nested translation. </li>
   <li> Best Ubcsat-solver for cutoff=10^5:
   \verbatim
> E = run_ubcsat("VanDerWaerden_N_3-3-3-5_79.cnf")
   \endverbatim
   evaluated by plot(E$alg,E$best), shows again saps as best: still no solution
   found, but this translation seems clearly superior:
   \verbatim
table(E$best[E$alg=="saps"])
 1  2  3
24 73  3
table(E$best[E$alg=="rsaps"])
 1  2  3
22 69  9
table(E$best[E$alg=="sapsnr"])
 1  2  3
20 73  7
table(E$best[E$alg=="rnoveltyp"])
 1  2  3  4  5
12 57 17  8  6
table(E$best[E$alg=="rnovelty"])
 1  2  3  4  5
12 54 23  9  2
table(E$best[E$alg=="adaptnoveltyp"])
 1  2  3  4  5
 7 57 22 11  3
table(E$best[E$alg=="irots"])
 1  2  3
 5 47 48
table(E$best[E$alg=="noveltyp"])
 1  2  3  4  5  6
 3 17 32 33 14  1
table(E$best[E$alg=="rots"])
 1  2  3
 2 44 54
table(E$best[E$alg=="novelty"])
 1  2  3  4  5  6
 2 19 34 29 11  5
table(E$best[E$alg=="gwsat"])
 1  2  3  4
 1 22 59 18
table(E$best[E$alg=="gsat_tabu"])
 1  3  4  5  6  7  8  9 10 11 12 13 15
 1  2  7 20 18 20 16  4  8  1  1  1  1
   \endverbatim
   </li>
   <li> saps:
    <ol>
     <li> cutoff=2*10^5
     \verbatim
 1  2
41 59
100
     \endverbatim
     </li>
     <li> cutoff=4*10^5
     \verbatim
 1  2
72 28
100
     \endverbatim
     </li>
     <li> cutoff=8*10^5
     \verbatim
 1  2
87 13
100
     \endverbatim
     </li>
    </ol>
   </li>
   <li> So the standard nested translation seems definitely an improvement,
   however vdW-instances still seem very hard for local search solvers. </li>
   <li> minisat2:
    <ol>
     <li> Determines satisfiability for n=79 with standard translation
     in ?? restarts; aborted for now after 21 restarts. </li>
     <li> Determines unsatisfiability for n=80 with standard translation
     in ?? restarts; aborted for now after 23 restarts. </li>
     <li> Determines satisfiability for n=79 with standard nested translation
     in 13 restarts (1s). </li>
     <li> Determines unsatisfiability for n=80 with standard nested translation
     in ?? restarts; aborted for now after 24 restarts. </li>
    </ol>
   </li>
   <li> OKsolver_2002
    <ol>
     <li> Without preprocessing and without symmetry breaking with
     standard translation for n=79: seems feasible within say 10 hours,
     but aborted for now. </li>
     <li> Without preprocessing and without symmetry breaking with
     standard nested translation for n=79: looks hard (after 62000 nodes
     no monitoring-node completed at depth 24). </li>
    </ol>
   </li>
   <li> Now considering the standard strong nested translation. </li>
  </ul>

*/

