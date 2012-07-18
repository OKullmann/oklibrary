// Oliver Kullmann, 18.7.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/SchurProblems/plans/Schur6/general.hpp
  \brief On computing schur(6) and variations


  \todo Local search
  <ul>
   <li> It seems the best current lower bound is schur(6) >= 537 (from
   [Fredricksen, Sweet, 2000]). </li>
   <li> They use the palindromic form; for n=536 we have the special case where
   3 divides n+1. </li>
   <li> Let's start with n=535 (though from n=536 being satisfiable nothing
   follows for n=535). </li>
   <li> First we assume that saps is the best algorithm (from
   Schur5/LocalSearch/general.hpp). </li>
   <li>
   \verbatim
> ubcsat-okl -alg saps -runs 100 -cutoff 10000000 -i Schur_pd_6_535.cnf | tee Schur_pd_6_535.cnf_OUT
Clauses = 146344
Variables = 1608
TotalLiterals = 434214
FlipsPerSecond = 70076
BestStep_Mean = 1099320.1
Steps_Mean = 10000000
Steps_Max = 10000000
PercentSuccess = 0.00
BestSolution_Mean = 1.68
BestSolution_Median = 2
BestSolution_Min = 1
BestSolution_Max = 3
> E=read_ubcsat("Schur_pd_6_535.cnf_OUT",nrows=100)
 1  2  3
36 60  4
100
   \endverbatim
   </li>
  </ul>

*/
