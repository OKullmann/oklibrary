// Oliver Kullmann, 3.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_te_m-3-3.hpp
  \brief Investigations regarding the "transversal extension" numbers greentao_{m+2}([2,...,2,3,3])


  \todo greentao_3(2,3,3) = 31
  <ul>
   <li> n=30 trivially satisfiable by adaptnovelty+. </li>
   <li> Also trivial for the OKsolver_2002, which also determines that n=31
   is unsatisfiable with 171 nodes. </li>
  </ul>


  \todo greentao_4(2,2,3,3) = 39
  <ul>
   <li> Very easy for OKsolver_2002. </li>
  </ul>


  \todo greentao_5(2,2,2,3,3) = 41
  <ul>
   <li> n=40 trivially satisfiable by adaptnovelty+. </li>
   <li> Also still easy for OKsolver_2002. </li>
   <li> And n=41 needs 163846 nodes (less than a minute) with the OKsolver.
   </li>
   <li> minisat2 needs only less than a second. </li>
   <li> While satz215 seems to have some bigger problems. </li>
   <li> And also march_pl seems to have some problems. </li>
  </ul>

  
  \todo greentao_6(2,2,2,2,3,3) = 47
  <ul>
   <li> n=46 trivially satisfiable by adaptnovelty+. </li>
   <li> n=47 found unsatisfiable by minisat2 in 30 seconds. </li>
  </ul>


  \todo greentao_7(2,...,2,3,3) >= 53
  <ul>
   <li> n=52 trivially satisfiable by adaptnovelty+. </li>
   <li> n=53 poses some difficulties for minisat2 (learning
   very long clauses). </li>
  </ul>

*/

