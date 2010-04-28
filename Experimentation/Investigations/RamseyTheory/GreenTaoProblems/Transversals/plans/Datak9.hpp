// Oliver Kullmann, 25.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/Datak9.hpp
  \brief Investigating the transversal hypergraph of Green-Tao hypergraphs for k = 9 (length of arithmetic progressions)


  \todo Elementary statistics
  <ul>
   <li> Investigating tr_arithprog_hg(9,n). </li>
   <li> For orientation, those n-values where at least one hyperedge has been
   added (compared to n-1), and the number of hyperedges added:
   \verbatim
for n: 8 thru 5000 do block(
 [d:nhyp_arithprog_primes_ohg(9,n)-nhyp_arithprog_primes_ohg(9,n-1)],
  if d>0 then print(n, d));
289 1
316 1
690 1
1453 1
1497 1
1523 1
2022 1
2499 1
2823 1
2915 1
3315 1
3602 1
3775 1
4137 1
4551 1
4653 1
4711 1
4774 1
   \endverbatim
   </li>
   <li> At C++ level (showing only the changes):
   \verbatim
> MinimumTransversals_GreenTao-O3-DNDEBUG 2000 9 | tee GreenTao_Trans_9_2000_OUT
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> GTTransversalsInc 9 1 0 GT_9 GT_9_SAT

L : transform_steps_l(map(third,rest(read_nested_list("GT_9"))));
 [288,689,1452,1496,1522,2021,2498,2822,2914,3314,3601]
length(L);
 11

> tail -1 GT9
9 3774 11
   \endverbatim
   </li>
  </ul>

*/
