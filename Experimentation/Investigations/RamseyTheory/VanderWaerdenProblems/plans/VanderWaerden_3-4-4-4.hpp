// Oliver Kullmann, 28.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_3-4-4-4.hpp
  \brief On investigations into vdW-number vdw_3(4,4,4)

  Generated as non-boolean formal clause-set with uniform domain via
  vanderwaerden_nbfcsud([4,4,4],n) at Maxima-level.

  Using the (strong) standard translation, a standardised boolean clause-set
  is obtained by vanderwaerden_aloamo_stdfcs([4,4,4],n).

  Using the (weak) standard nested translation, a standardised boolean
  clause-set is obtained by vanderwaerden_standnest_stdfcs([4,4,4],n).

  File-output is obtained by output_vanderwaerden_stdname([4,4,4],n) resp.
  output_vanderwaerden_standnest_stdname([4,4,4],n).


  \todo Known from the literature
  <ul>
   <li> [Rabung 1979] shows via special constructions vdw_3(4,4,4) > 292. </li>
  </ul>


  \todo vdw_3(4,4,4) > 150
  <ul>
   <li> First using the weak standard nested translation with rnovelty+. </li>
   <li> n=100: very easy. </li>
   <li> n=150: with cutoff=10^5 in 100 runs one solution was found. </li>
   <li> n=200
    <ol>
     <li> rnovelty+ with cutoff=10^6 seems far off. </li>
     <li> saps with cutoff=10^6 seems much stronger. </li>
     <li> Best local search algorithm from Ubcsat-suite:
     \verbatim
E = run_ubcsat("VanDerWaerden_N_3-4-4-4_200.cnf", runs=100,cutoff=100000)
     \endverbatim
     evaluated by plot(E$alg,E$best) and eval_ubcsat_dataframe(E):
     \verbatim
rnoveltyp :
 3  9 26 29 30 31 33 34 35 36 37 38 39 40
 1  1  1  1  2  6 16 14 14 20 11 10  2  1
adaptnoveltyp :
 4  8 21 22 24 25 26 27 28 29 30 31
 1  1  3  2 14 14 16 14 19  7  6  3

saps :
31 32 33 34 35 36 37
 5 12 16 20 27 17  3
     \endverbatim
     So it seems that rnovelty+ is actually better, due to the outliers. </li>
     <li> rnovelty+ with cutoff=10^6:
     \verbatim
 3  4 10 18 22 25 26 27 28 29 30 31 32 33 34
14 16  5  1  1  3  3 10 17 21 25 29 27 17 11
200
     \endverbatim
     </li>
     <li> rnovelty+ with cutoff=10^7:
     \verbatim
 3  4  8  9 10 14 23 24 25 26 27 28 29
51 67  7  8 11  2  8  4 11  9 12  8  2
200
     \endverbatim
     </li>
     <li> adaptnovelty+ with cutoff=10^6:
     \verbatim
 3  4  8  9 10 11 14 15 18 19 20 21 22 23 24 25 26
 7 19  5  2  7  2  1  1  1  4  6 16 32 46 33 14  4
200
     \endverbatim
     </li>
     <li> adaptnovelty+ with cutoff=10^7:
     \verbatim
 3  4  5  8  9 10 11 13 14 15 19 20 21 22
64 65  1 18 11 16  4  1  2  1  4  6  5  2
200
     \endverbatim
     </li>
     <li> Again, best local search algorithm from Ubcsat-suite (now cutoff
     10^6):
     \verbatim
E = run_ubcsat("VanDerWaerden_N_3-4-4-4_200.cnf", runs=100,cutoff=1000000)
     \endverbatim
     evaluated by plot(E$alg,E$best) and eval_ubcsat_dataframe(E):
     \verbatim
rnoveltyp :
 3  4 19 22 24 25 26 27 28 29 30 31 32 33 34 35
10  3  1  1  2  1  3  3  7  8 14 20 18  7  1  1
adaptnoveltyp :
 3  4  8  9 10 11 13 20 21 22 23 24 25 26
 9  7  1  1  1  1  1  2  4 17 14 26 13  3
rnovelty :
 3  4 24 26 27 28 29 30 31 32 33 34
 3  3  3  1 10  8  8 15 25  8 12  4
novelty :
 3  4 34 36 37 38 39 40 41 42 43 44 45
 1  2  1  2  4  6  9 12 18 20 16  8  1
rots :
 3  4 10 22 23 24 25 26 27 28 29 30
 1  1  1  4  2  4  9 17 31 18  9  3
gwsat :
 4 10 23 25 26 27 28 29 30 31
 1  1  2  4  6  9 15 31 23  8
saps :
 5 28 29 30 31 32 33
 1  1  6  9 23 39 21
rsaps :
 6 22 28 29 30 31 32 33 34
 1  1  3 10 14 19 27 22  3
     \endverbatim
     </li>
     <li> Curious the barrier at min=3. </li>
     <li> adaptnovelty+ with cutoff=10^8:
     \verbatim
  3   4  8
550 548  2
1100
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


*/

