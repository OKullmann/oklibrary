// Oliver Kullmann, 6.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/RandomClauseSets.hpp
  \brief Plans for generators for (various types of) random clause-sets


  \todo Random subsets, sublists etc.
  <ul>
   <li> In
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/InverseSingularDP.mac
   we have "random_sublist". </li>
   <li> And in ComputerAlgebra/Satisfiability/Lisp/Generators/RandomClauseSets.mac
   we have "random_sublist_l". </li>
   <li> Likely these functions should go to DataStructures/Lists.mac.
   </li>
  </ul>
  
  
  \todo AES random generator
  <ul>
   <li> Using our maxima-aes-implementation, implement the random generator
   as in Experimentation/RandomGenerator/plans/general.hpp. </li>
   <li> The specification was given in [Oliver Kullmann, CSR19-2002]. </li>
   <li> For k in NNZ let W(k) := {0, ..., 2^k - 1} and W^*(k) := W(k) - {0}.
   </li>
   <li> Input specification: OKgenerator(s,k,n,m,p,c), where
    <ol>
     <li> s in W(64) (the seed) </li>
     <li> k in W(64) (the formula number) </li>
     <li> n in W^*(31) (the number of variables) </li>
     <li> p a list of length m, where m in W^*(31) (number of clause-sizes),
     with entries natural numbers, strictly increasing, and with p[i] in
     W^*(31) (the clause-lengths). </li>
     <li> c a list of length m, with entries c[i] in W^*(32) (the number of
     clauses of length p[i]). </li>
    </ol>
   </li>
   <li> According to our general philosophy, the function OKgenerator does not
   check its input, but we have a special test function for this. </li>
   <li> The AES-function is considered as aes: W(128) x W(128) -> W(128)
   (see XXX). </li>
   <li> The central helper function is the OKlitgenerator(s,k,n,p,c),
   where s,k,n as above, p in W(31), c in W(64), defined as follows:
    <ol>
     <li> First let alpha_n be the bijection from {0, ..., 2n-1} ->
     {-n, ..., n} - {0}, given by 0 -> 1, 1 -> 2, ..., n-1 -> n, and
     then n -> -1, n+1 -> -2, ..., 2n-1 -> -n. </li>
     <li> Now OKlitgenerator(s,k,n,p,c) :=
      alpha_n(aes(s * 2^64 + k, (n * 2^96 + p * 2^64 + i) mod 2n)). </li>
     <li> Thus -2^31 < OKlitgenerator(s,k,n,p,c) < 2^31, where the value 0
     is excluded. </li>
    </ol>
   </li>
   <li> Now the OKgenerator for m = 1 is defined as follows:
    OKgenerator(s,k,n,m,[p],[c]) := [C_1, ..., C_c], where
     <ol>
      <li> C_i := [l_{i,1}, ..., l_{i,p}] </li>
      <li> XXX </li>
     </ol>
   </li>
  </ul>


  \todo Filtering out
  <ul>
   <li> A natural extensions of random generators is by applying some filter,
   removing for example instances with repeated clauses. </li>
   <li> We need a framework for this. </li>
  </ul>

*/

