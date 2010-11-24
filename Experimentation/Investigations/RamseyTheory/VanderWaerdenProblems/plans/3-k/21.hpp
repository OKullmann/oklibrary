/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/21.hpp
  \brief On investigations into vdw_2(3,21)


  \todo vanderwaerden_2(3,21) >= 416
  <ul>
   <li> The conjecture is vanderwaerden_2(3,21) = 416. </li>
   <li> Experience with k=20 is that gsat-tabu is best, however we should
   have a look again --- this might change with different k's or with
   different cutoff's. </li>
   <li> n=394, gsat-tabu, cutoff=10^8: in run 1 a solution was found. </li>
   <li> n=396, gsat-tabu, cutoff=10^8: in run 2 a solution was found. </li>
   <li> n=398, gsat-tabu, cutoff=10^8: in 100 runs 73 solutions were found.
    </li>
   <li> n=410, gsat-tabu, cutoff=10^8: in 100 runs 36 solutions were found.
   </li>
   <li> n=415
    <ol>
     <li> gsat-tabu, cutoff=10^8: in run 3 a solution was found
     (seed=174271267, osteps=91827849). Further statistics:
     \verbatim
 0   1  2 3 4 5
17 147 28 5 2 1
200
     \endverbatim
     </li>
     <li> RunVdWk3 with cutoff=10^7 and 100 runs finds a solution (run 69),
     with certificate
     \verbatim
9,27,29,36,38,48,61,62,82,101,
105,107,110,123,124,130,131,133,144,163,
167,169,170,179,192,213,232,236,238,246,
255,268,269,271,289,308,312,314,315,324,
330,337,338,351,358,374,376,381,383,395
     \endverbatim
     </li>
     <li> RunVdWk3 with cutoff=2*10^7 finds a solution (run 73), same as
     above, but with vertex 133 removed. </li>
     <li> Is this the only solution (up to small variations)? </li>
    </ol>
   </li>
   <li> n=416, gsat-tabu
    <ol>
     <li> cutoff=10^7:
     \verbatim
  1   2   3   4   5
405 572  20   2   1
1000
 1  2 
39 61 
100
     \endverbatim
     </li>
     <li> cutoff=2*10^7:
     \verbatim
  1   2   3   4 
123  70   5   2 
200 
     \endverbatim
     </li>
     <li> cutoff=10^8:
     \verbatim
  1  2 3 4
245 48 6 1
300
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=420, gsat-tabu, cutoff=10^8:
   \verbatim
  1  2  3 6
249 40 10 1
300
   \endverbatim
   </li>
   <li> n=423, gsat-tabu, cutoff=10^8:
   \verbatim
 1  2  3  4
74 19  5  2
100
   \endverbatim
   </li>
   <li> n=424, gsat-tabu, cutoff=10^8:
   \verbatim
  1  2  3  4 6
138 52  5  4 1
200
   \endverbatim
   </li>
   <li> n=425, gsat-tabu, cutoff=10^8:
   \verbatim
  1  2  3 6
117 41 13 1
172
   \endverbatim
   </li>
   <li> n=426, gsat-tabu, cutoff=10^8:
   \verbatim
  1  2 3 4
120 71 8 1
200
   \endverbatim
   </li>
  </ul>


  \todo Palindromic numbers
  <ul>
   <li> vanderwaerden_g([3,21],"pd") < 416 =(conj) vanderwaerden([3,21]). </li>
   <li> precosat236:
    <ol>
     <li> n=415: unsat, 1452s. </li>
     <li> n=414: ?sat, s. </li>
    </ol>
   </li>
  </ul>

*/
