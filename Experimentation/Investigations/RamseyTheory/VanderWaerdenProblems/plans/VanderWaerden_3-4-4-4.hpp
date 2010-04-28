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
E = eval_ubcsat("VanDerWaerden_N_3-4-4-4_200.cnf", params=list(runs=100,cutoff=100000))
     \endverbatim
     evaluated by plot(E$alg,E$best) and eval_ubcsat_dataframe(E):

     </li>
    </ol>
   </li>
  </ul>


*/

