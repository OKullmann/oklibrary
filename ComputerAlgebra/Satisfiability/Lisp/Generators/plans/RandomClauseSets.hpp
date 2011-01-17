// Oliver Kullmann, 6.7.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
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
   <li> And in
   ComputerAlgebra/Satisfiability/Lisp/Generators/RandomClauseSets.mac
   we have "random_sublist_l". </li>
   <li> Likely these functions should go to DataStructures/Lists.mac.
   </li>
   <li> DONE (see random_full_fcs)
   We need also a true random clause-set generator, at least for
   full clause-sets, i.e., one which doesn't require us to specify the number 
   of clauses. </li>
  </ul>


  \todo Move random_full_fcs
  <ul>
   <li> random_full_fcs in  
   ComputerAlgebra/Satisfiability/Lisp/Generators/RandomClauseSets.mac
   actually considers random boolean functions and should actually be moved
   to ComputerAlgebra/Satisfiability/Lisp/Generators/ . </li>
   <li> The name and documentation should also be reconsidered. </li>
  </ul>
  
  
  \todo Specifying the AES random generator
  <ul>
   <li> The specification was given in [Oliver Kullmann, CSR19-2002]. </li>
   <li> For k in NNZ let W(k) := {0, ..., 2^k - 1} and W^*(k) := W(k) - {0}.
   </li>
   <li> Input specification: OKgenerator(s,k,n,p,c), where
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
   <li> The AES-function is considered as aes: W(128) x W(128) -> W(128),
   given by "aes_int(p,k,10)" (see
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/AdvancedEncryptionStandard.mac).
   </li>
   <li> The central helper function is the OKlitgenerator(s,k,n,p,c),
   where s,k,n as above, p in W(31), c in W(64), defined as follows:
    <ol>
     <li> First let alpha_n be the bijection from {0, ..., 2n-1} ->
     {-n, ..., n} - {0}, given by 0 -> 1, 1 -> 2, ..., n-1 -> n, and
     then n -> -1, n+1 -> -2, ..., 2n-1 -> -n. </li>
     <li> Now OKlitgenerator(s,k,n,p,c) :=
      alpha_n(aes_int(n * 2^96 + p * 2^64 + c, s * 2^64 + k,10) mod 2n)). </li>
     <li> Thus -2^31 < OKlitgenerator(s,k,n,p,c) < 2^31, where the value 0
     is excluded. </li>
    </ol>
   </li>
   <li> Now the OKgenerator for m = 1 is defined as follows:
    OKgenerator(s,k,n,[p],[c]) := [C_1, ..., C_c], where
    for 1 <= i <= c we define:
     <ol>
      <li> C_i := [l_{i,1}, ..., l_{i,p}], where the l_{i,j} are defined
      as follows. </li>
      <li> For 1 <= j <= p let x_{i,j} be defined by
      x_{i,j} := OKlitgenerator(s, k, n-j+1, p, (i-1)*p+j-1). </li>
      <li> The sign of l_{i,j} is now the sign of x_{i,j}. </li>
      <li> The variable of l_{i,1} is the variable of x_{i,1}. </li>
      <li> Regarding the other variables, we can not use the x_{i,j} for
      j > 1, since we must avoid repetitions. </li>
      <li> This is now achieved by using first x_{i,2}, then x_{i,3}, and so
      on, however not as absolute values, but only relative, that is, the
      variable l_{i,2} is value number |x_{i,2}| in the variables left from
      {1, ..., n} after having removed |x_{i,1}|, and so on. </li>
     </ol>
   </li>
   <li> The general OKgenerator(s,k,n,p,c) for m >= 1 is now defined as the
   concatenation of the values OKgenerator(s,k,n,[p[i]],[c[i]]) for i from 1
   to m. </li>
   <li> For generating non-boolean clause-sets there exists an additional
   parameter d in W^*(32), d >= 2, specifying the (uniform) domain size. </li>
   <li> How to call the generalised OKgenerator (and the generalised
   OKlitgenerator)? Perhaps we use "OKgenerator_cs" and "OKgenerator_nbcs".
   </li>
   <li> Finally, where to insert the additional parameter d in the parameter
   list? In the paper it is inserted after n, which seems reasonable. </li>
  </ul>


  \todo Implementing the AES random generator
  <ul>
   <li> DONE
   Using our maxima-aes-implementation, implement the random generator
   as in Experimentation/RandomGenerator/plans/general.hpp. </li>
   <li> How to name the generalised (non-boolean) versions? </li>
  </ul>


  \todo Filtering out
  <ul>
   <li> A natural extensions of random generators is by applying some filter,
   removing for example instances with repeated clauses. </li>
   <li> We need a framework for this. </li>
  </ul>

*/

