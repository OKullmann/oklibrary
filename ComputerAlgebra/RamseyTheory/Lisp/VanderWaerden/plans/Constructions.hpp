// Oliver Kullmann, 23.10.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/Constructions.hpp
  \brief Plans regarding constructions of van der Waerden certificates


  \todo Rabung's method
  <ul>
   <li> rabung_search(2,11,300000) has as last element in the list
   [17863,178631], while according to [Rabung 1979] it should reach
   [19681,196811]:
   \verbatim
[m,k,n] : [2,11,196811];
rabung_valid_param(m,k,n);
  true
rabung_derived_parameters(m,k,n);
  [19681,11]
rabung_checkcriterion(m,k,n);
  false
   \endverbatim
   So this seems to be an error in the paper. </li>
   <li> Confirmation/improvement of other values in the table of [Rabung 1979]:
   \verbatim
rabung_search(2,7,5000);
  ... [617,3703]] SAME
rabung_search(2,8,10000);
  ... [1069,7484]] SAME
rabung_search(2,9,60000);
  ... [3389,27113]] SAME
rabung_search(2,10,200000);
  ... [11497,103474]] SAME
rabung_search(2,11,300000);
  ... [17863,178631]]
rabung_search(2,12,300000);
  ... [26849,295340]] IMPROVED
rabung_search_(2,12,400000,26849);
  ... [35053,385584]]
rabung_search_(2,12,500000,35053);
  [[37223,409454],[37831,416142],[38167,419838],[38431,422742],[38833,427164],[44753,492284]]
rabung_search_(2,12,600000,44753);
  [[51197,563168]]
rabung_search_(2,12,700000,51197);
  [[55333,608664],[58013,638144]]
rabung_final_prime(2,12,58013,700000);
  63629
rabung_search_(2,12,900000,63629);
  []

rabung_search(3,4,1000);
  ... [97,292]] SAME
rabung_search(3,5,2000);
  ... [241,965]] SAME
rabung_search(3,6,10000);
  ... [1777,8886]] SAME
rabung_search(3,7,60000);
  ... [7309,43855]] SAME
rabung_search(3,8,200000);
  ... [23053,161372]] IMPROVED
rabung_final_prime(3,8,23053,200000);
  28549
rabung_search_(3,8,400000,28549);
  [[28927,202490],[30757,215300],[31477,220340],[34057,238400]]
rabung_search(3,9,250000);
  ... [31249,249993]] IMPROVED
rabung_search_(3,9,350000,31249);
  ... [43291,346329]]
rabung_search_(3,9,450000,43291);
  ... [55117,440937]]
rabung_search_(3,9,600000,55117);
  ... [74959,599673]]
rabung_search_(3,9,700000,74959);
  ... [85513,684105]]
rabung_search_(3,9,900000,85513);
  [[93169,745353],[103237,825897]]
rabung_final_prime(3,9,103237,900000);
  112459
rabung_search_(3,9,1000000,112459);
  [[115807,926457],[116593,932745]]
rabung_final_prime(3,9,116593,1000000);
  124987
rabung_search_(3,9,1100000,124987);
  []

rabung_search(4,3,200);
  ... [37,75]] SAME
rabung_search(4,4,2000);
  ... [349,1048]] SAME
rabung_search(4,5,20000);
  ... [2609,10437]] SAME
rabung_search(4,6,200000);
  ... [18061,90306]] SAME
rabung_search(4,7,200000);
  ... [33317,199903]] IMPROVED
rabung_search_(4,7,300000,33317);
  ... [49789,298735]]
rabung_search_(4,7,400000,49789);
  [[50177,301063],[59957,359743],[64661,387967]]
rabung_search_(4,7,500000,64661);
  []


rabung_search(5,4,10000);
  ... [751,2254]] SAME
rabung_search(5,5,100000);
 ... [6011,24045]] SAME
rabung_search(5,6,200000);
  ... [35591,177956]] IMPROVED
rabung_search_(5,6,300000,35591);
  [[49391,246956]]
rabung_final_prime(5,6,49391,300000);
  59981
rabung_search_(5,6,400000,59981);
  []
rabung_search(5,7,400000);
  ... [66301,397807]] IMPROVED
rabung_final_prime(5,7,66301,400000);
  66601
rabung_search_(5,7,600000,66601);
  ... [99761,598567]]
rabung_final_prime(5,7,66601,600000);
  99991
rabung_search_(5,7,800000,99991);
  ... [130841,785047]]
rabung_final_prime(5,7,130841,800000);
  133321
rabung_search_(5,7,1100000,133321);
  ... [173291,1039747]]
rabung_final_prime(5,7,173291,1100000);
  183301
rabung_search_(5,7,1300000,183301);
  ... [215801,1294807]]
rabung_final_prime(5,7,215801,1300000);
  216661
rabung_search_(5,7,1600000,216661); cs-wsok
  [[230281,1381687]]

rabung_search(6,3,1000);
  ... [103,207]] SAME
rabung_search(6,4,30000);
  ... [3259,9778]] SAME
rabung_search(6,5,150000);
  ... [14173,56693]] IMPROVED
rabung_search(6,6,400000);
  ... [79549,397746]] IMPROVED
rabung_final_prime(6,6,79549,400000);
  79999
rabung_search_(6,6,700000,79999);
  ... [120097,600486]]
   \endverbatim
   </li>
  </ul>

*/
