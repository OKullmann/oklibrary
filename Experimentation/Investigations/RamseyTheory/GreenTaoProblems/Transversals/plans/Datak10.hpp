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
 [315,5481,5743,7293,9457,11896,12650,12743,13461,13560,13645,16524,16535,16595,19134,19601,
       21271,23059,23688,24145,24255,24351,25006,25326,25348,25491,27020,27790,29149,29618,30162,
       30700,30833,31088,34889,34940,35244,36443,38331,38791,38930,39009,39275,39676,40148,40504,
       41533,41621,42526,43143]
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
