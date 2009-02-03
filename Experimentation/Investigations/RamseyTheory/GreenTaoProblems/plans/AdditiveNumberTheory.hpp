// Oliver Kullmann, 26.9.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/AdditiveNumberTheory.hpp
  \brief On investigations into additive number theory
  
  
  \todo The distribution of arithmetic progressions amongst primes
  <ul>
   <li> The task is to find a nice (thus very likely approximated) law for
   the values in the list ln_arithprog_primes_c(k,n) (see
   ComputerAlgebra/NumberTheory/Lisp/PrimeNumbers.mac) for fixed k >= 1. </li>
   <li> Ploted via %e.g.
   \verbatim
plot2d([discrete,create_list(i,i,1,1000),ln_arithprog_primes_c(3,1000)]);
   \endverbatim
   </li>
   <li> For k = 1,2 this is trivial. </li>
   <li> For k >= 3 regression is to be performed; most powerful is using R,
   but for initial considerations also simple_linear_regression (use
   'load("stats")') can be used. </li>
   <li> There are role models for the regression function in the literature.
   </li>
   <li> One should also consider n_arithprog_primes_nc[k,n] (the non-cumulative
   data, i.e., as list the difference list of the above list). </li>
   <li> And one can consider the densities
   ln_arithprog_primes_c(k,n) / create_list(i,i,1,n). </li>
   <li> Hard to believe that there is nothing in the literature / on the
   Internet: We should enter for example ln_arithprog_primes_c(3,30) =
   [0,0,0,1,2,2,3,5,7,9,11,11,13,16,17,20,23,24,26,30,32,36,40,44,46,49,53,56,59,64]
   into that database of integer sequences and see whether there is information
   in it. </li>
   <li> Yes, this sequence is A125505 in
   http://www.research.att.com/~njas/sequences/Seis.html. </li>
   <li> There it is only listed for n=64; this we can easily extend, and
   perhaps we should do so. </li>
   <li> And apparently for k >= 4 there is nothing entered there --- we
   should change this.
    <ol>
     <li> Say, up to k=10. </li>
     <li> For k=10 for example the first 315 values are 0, and then
     at least until index 3000 the value is constant 1; for such sequences
     we need a compressed representation. </li>
    </ol>
   </li>
   <li> Of interest is also
   http://www.research.att.com/~njas/sequences/Sindx_Pri.html#primes_AP
   which gives an overview. It contains for given k the smallest starting
   term, smallest difference and smallest end term for arithmetical
   progressions of primes of length k. However I do not understand
   these sequences (strange explanations). </li>
   <li> What seems very natural to me is k -> how many first primes are
   needed to get an progression of length k. </li>
   <li> This yields the sequence (starting with k=1)
   1,2,4,9,10,37,155,263,289,316,21966,23060,58464,2253121. </li>
   <li> Until k=10 this is easily computed by first_arithprog_primes. </li>
   <li> This is apparently not available at that Internet database; we should
   submit it. </li>
   <li> The related sequence A120302 is 2,3,7,23,29,157,907,8471621 which is
   obtained from the previous sequence by unranking, %e.g.,
   unrank_primes(37) = 157. However the last entry "8471621"
   seems nonsense?? It should be 1669. So this must be another seuence? </li>
   <li> Ah, this seems to be A005115: 2, 3, 7, 23, 29, 157, 907, 1669, 1879,
   2089, 249037, 262897, 725663, 36850999 (available up to k=21). </li>
   <li> From this sequence S via map(rank_primes,S) we obtain the above
   sequence (created by first_arithprog_primes). </li>
   <li> There were considerations on the Maxima mailing list to make
   this collection available: We should encourage them. </li>
  </ul>

*/

