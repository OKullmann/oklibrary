// Oliver Kullmann, 23.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/Datak8.hpp
  \brief Investigating the transversal hypergraph of Green-Tao hypergraphs for k = 8 (length of arithmetic progressions)


  \todo Elementary statistics
  <ul>
   <li> Investigating tr_arithprog_hg(8,n). </li>
   <li> For orientation, those n-values where at least one hyperedge has been
   added (compared to n-1), and the number of hyperedges added:
   \verbatim
for n: 7 thru 3000 do block(
 [d:nhyp_arithprog_primes_ohg(8,n)-nhyp_arithprog_primes_ohg(8,n-1)],
  if d>0 then print(n, d));
263 1
289 1
316 1
349 1
665 1
690 2
789 1
1323 1
1428 1
1447 1
1453 1
1473 1
1497 1
1523 1
1555 1
1569 1
1801 1
1857 1
1881 1
1915 1
1935 1
1979 1
2022 1
2117 1
2261 1
2285 1
2406 1
2437 1
2499 1
2515 1
2568 1
2573 1
2651 1
2672 1
2738 1
2816 1
2823 1
2870 1
2915 1
   \endverbatim
   </li>
   <li> One would assume that the SAT problems for such small numbers of
   hyperedges should be very easy. </li>
   <li> At C++ level (showing only the changes):
   \verbatim
> MinimumTransversals_GreenTao-O3-DNDEBUG 2000 8 | tee GreenTao_Trans_8_2000_OUT
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> GTTransversalsInc 8 1 0 GT8 GT8_SAT

transform_steps_l(map(third,rest(read_nested_list("GT_8"))));
   \endverbatim
   </li>
  </ul>

*/
