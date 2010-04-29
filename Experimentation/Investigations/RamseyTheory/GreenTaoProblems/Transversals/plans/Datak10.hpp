// Oliver Kullmann, 29.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/Datak10.hpp
  \brief Investigating the transversal hypergraph of Green-Tao hypergraphs for k = 10 (length of arithmetic progressions)


  \todo Elementary statistics
  <ul>
   <li> Investigating tr_arithprog_hg(10,n). </li>
   <li> For orientation, those n-values where at least one hyperedge has been
   added (compared to n-1), and the number of hyperedges added:
   \verbatim
block([oldnhyp:0], for n: 100 thru 20000 do block(
 [newnhyp : nhyp_arithprog_primes_ohg(10,n), d],
  d : newnhyp-oldnhyp,
  if d>0 then print(n, d),
  oldnhyp : newnhyp))$

316 1
5482 1
5744 1
7294 1
9458 1
   \endverbatim
   </li>
   <li> At C++ level (showing only the changes):
   \verbatim
> MinimumTransversals_GreenTao-O3-DNDEBUG 2000 10 | tee GreenTao_Trans_10_2000_OUT
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> GTTransversalsInc 10 1 0 GT_10 GT_10_SAT

L : transform_steps_l(map(third,rest(read_nested_list("GT_10"))));
 
length(L);
 

> tail -1 GT10

   \endverbatim
   </li>
  </ul>

*/
