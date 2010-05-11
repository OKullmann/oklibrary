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
   <li> The numbers of minimum hyperedges:
   \verbatim
L10 : [];
minimum_transversals_decomp_gen(inf,lambda([n],arithprog_primes_hg(10,n)),'L10)$

T : transform_steps_l(map(lambda([d],d[4][1]),reverse(L10)));
   \endverbatim
   </li>
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
11548 1
11897 1
12651 1
12744 1
13462 1
13561 1
13646 1
16525 1
16536 1
16596 1
19135 1
19602 1
   \endverbatim
   </li>
   <li> These hyperedges are nearly disjoint, only number 4 and number 6 have
   one element in common:
   \verbatim
G : arithprog_primes_ohg(10,20000)$
non_disjoint_pairs(G[2]);
 [[4,6]]
intersection(G[2][4], G[2][6]);
 {69067}
   \endverbatim
   All our current hypergraph-transversal approaches should have big problems
   with disjoint (or nearly disjoint) set-systems. </li>
   <li> While it follows from the above disjointness-property, that tau always
   increases by one when a new hyperedge aries in the above list, except for
   the sixth hyperedge which is left out. </li>
   <li> Starting with the Maxima-level, we must first compute the connected
   components, and then handle each component separately. </li>
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
 [315,5481,5743]
length(L);
 3

> tail -1 GT10

   \endverbatim
   </li>
  </ul>

*/
