// Oliver Kullmann, 22.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/Datak7.hpp
  \brief Investigating the transversal hypergraph of Green-Tao hypergraphs for k = 7 (length of arithmetic progressions)


  \todo Elementary statistics
  <ul>
   <li> Investigating tr_arithprog_hg(7,n). </li>
   <li> For orientation, those n-values where at least one hyperedge has been
   added (compared to n-1), and the number of hyperedges added:
   \verbatim
for n: 6 thru 1300 do block(
 [d:nhyp_arithprog_primes_ohg(7,n)-nhyp_arithprog_primes_ohg(7,n-1)],
  if d>0 then print(n, d));
155 1
214 1
228 1
232 1
263 1
289 1
316 1
323 1
349 1
396 2
570 1
641 2
665 1
690 2
715 1
789 1
796 1
827 1
872 1
875 1
1000 1
1048 1
1078 1
1125 1
1158 1
1176 1
1180 1
1188 1
1205 1
1240 1
   \endverbatim
   </li>
   <li> One would assume that the SAT problems for such small numbers of
   hyperedges should be very easy. </li>
   <li> At C++ level (showing only the changes):
   \verbatim
> MinimumTransversals_GreenTao-O3-DNDEBUG 1300 7 | tee GreenTao_Trans_7_1300_OUT

1 0 1
155 1 7
214 2 49
228 3 343
232 4 2401
263 4 2058
289 4 1715
316 4 1372
323 5 9604
349 5 8232
396 6 32928
570 7 230496
641 8 921984
665 8 691488
715 9 4840416
   \endverbatim
   (aborted after(!) the completion of this computation, since memory
   consumption reached 4 GB). </li>
   <li> The computation should be improved, so that without new hyperedges
   nothing needs to be computed. </li>
   <li> And it needs also to be considered, where the unnecessary memory
   usage comes from (perhaps some copy-operation). </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> GTTransversalsInc 7 1 0 GT_7 GT_7_SAT

transform_steps_l(map(third,rest(read_nested_list("GT_7"))));
  [154,213,227,231,322,395,569,640,714,795,826,871,874,1047,1124,1157,1179]
   \endverbatim
   Value 17 is attained for 1180 <= n <= 1187. </li>
   <li> The computation for n=1180 seems hard --- one should be able to do
   much better! </li>
  </ul>

*/
