// Oliver Kullmann, 17.10.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/Generators/plans/GreenTao.hpp
  \brief Plans regarding generators for Green-Tao hypergraphs


  \todo Connections
  <ul>
   <li> See Combinatorics/Hypergraphs/Generators/plans/GreenTao.hpp for
   C++ generators. </li>
  </ul>


  \todo Arithmetic progressions in prime numbers
  <ul>
   <li> It seems that statistics on the number of hyperedges in
   arithprog_primes_ohg are of interest (and non-trivial); see
   Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/AdditiveNumberTheory.hpp
   . </li>
   <li> The basic facts for arithprog_primes_finish, namely that for an
   arithmetic progression P of length k of primes with start a and slope d
   we must have a >= k, and if a <> k then product_primes(k) divides d,
   must be documented. </li>
   <li> DONE (now reasonable fast, and for faster computations more
   complex algorithms are needed)
   The function arithprog_primes_ohg is very slow.
    <ol>
     <li> DONE
     Since the list of primes is sorted, we can stop trying to extend
     the progression in case the difference gets bigger than d. </li>
     <li> DONE
     Implement arithprog_primes_finish[k,n] (an array-function), which
     computes all arithmetic progressions of length k amongst the first n
     prime numbers which finish in n. </li>
     <li> DONE
     Then arithprog_primes_ohg is computed by a simple append. </li>
     <li> DONE
     So results are re-used; a certain inefficiency is still caused
     by the permanent re-computation of the set of the first primes, however
     this should be a rather small amount.
      <ul>
       <li> arithprog_primes should set variable primes_init_seg,
       by appending iteratively next_prime(last_prime). </li>
       <li> And arithprog_primes_finish inherits this variable. </li>
      </ul>
     </li>
     <li> DONE
     More efficient access to the primes:
      <ul>
       <li> One should additionally use a boolean array of length p_max, where
       p_max is the n-th prime number, containing true if the index is prime,
       and then after having fixed the initial p and p2 (and thus d), instead
       of running through all z we can directly investigate the primes in the
       right distance. </li>
       <li> This array is then created by arithprog_primes through a function
       from number theory. </li>
       <li> But we still need primes_rev_init_seg so that the available
       distances can be quickly calculated. </li>
       <li> Instead of using this array, we could just provide a memoising
       wrapper "mprimep" around primep (this is what the array would achieve,
       within the given bounds). This seems best. </li>
    </ol>
   </li>
  </ul>

*/
