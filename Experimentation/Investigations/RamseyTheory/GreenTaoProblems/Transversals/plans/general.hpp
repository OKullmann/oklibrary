// Oliver Kullmann, 5.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/general.hpp
  \brief Investigating the transversal hypergraph of hypergraphs of arithmetic progressions in the primes

  Similar to
  Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/general.hpp,
  but now for given n we do not consider the set {1,...,n}, but the set of
  the first n prime numbers.


  \todo Basic considerations
  <ul>
   <li> Let tau_arithprog_primes(k,n) be the transversal number of the
   hypergraph arithprog_primes_hg(k,n) with vertex set the first n prime
   numbers and the arithmetic progressions of size k in that set as
   hyperedges. </li>
   <li> The Green-Tao theorem states that for every k >= 1 the limit of
   tau_arithprog_primes(k,n)/n is 1. </li>
   <li> Let the transversal hypergraph of arithprog_primes_hg be called
   tr_arithprog_hg(k,n). </li>
  </ul>


  \todo Investigating greentao_2(2,k)
  <ul>
   <li> Simple function for computing greentao_2(2,k):
   \verbatim
greentao2(k) := block([break:false,res],
 for n : 0 thru inf unless break do block([G : arithprog_primes_hg(k,n)[2]],
 if not emptyp(G) and emptyp(lintersection(G)) then (break:true,res:n)),
 res)$
   \endverbatim
   </li>
   <li> Easy to compute for k <= 10:
   \verbatim
for k : 1 thru 10 do print(k, greentao2(k));
    1  2
    2  3
    3  7
    4  14
    5  31
    6  55
    7  214
    8  349
    9  690
   10  5482
   \endverbatim
   </li>
  </ul>

*/

