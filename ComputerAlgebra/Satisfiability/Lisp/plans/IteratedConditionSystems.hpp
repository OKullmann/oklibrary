// Oliver Kullmann, 21.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/plans/IteratedConditionSystems.hpp
  \brief Plans for satisfaction problems given by transition functions


  \todo Basic notions
  <ul>
   <li> Given a set V of variables and a (uniform) domain D of values; yielding
   the set TASS(V,D) of all total assignments f: V -> D. </li>
   <li> An "iterated condition system" (ics) is specified by three conditions
   (see 'The notion of "condition"' in
   ComputerAlgebra/Satisfiability/Lisp/plans/SatisfactionProblems.hpp):
    <ol>
     <li> The initial condition S(f). </li>
     <li> The transition condition T(f,g). </li>
     <li> An invariant I(f). </li>
    </ol>
   </li>
   <li> Given an ics (V,D,S,T,I), for k in NN_0 the set R_k of total
   assignments reachable in k steps is defined by induction as follows:
    <ol>
     <li> R_0 is the set of f with S(f) and I(f). </li>
     <li> R_{k+1} is the set of g such that f in R_k exists, and we have
     T(f,g) and I(g). </li>
    </ol>
   </li>
   <li> The task is, for a given condition C to find k in NN_0 and f in R_k
   with not C(f). Call it a "falsifying solution". </li>
   <li> If no "falsifying solution" exists (for the given C), then C is
   called "valid" for the ics. </li>
   <li> Typically one also wants (f_0, ..., f_k), f_k = f, the "trace",
   witnessing f in R_k, that is,
    <ol>
     <li> S(f_0), </li>
     <li> for all i < k we have T(f_i,f_{i+1}) and I(f_i), </li>
     <li> and finally we have I(f_k) and not C(f_k). </li>
    </ol>
   </li>
   <li> Additionally typically one wants the smallest such k, that is, for
   i < k we C(f_i). </li>
  </ul>


  \todo Finding literature
  <ul>
   <li> The above notion of an "ics" is likely rather common. </li>
   <li> We need literature. </li>
  </ul>


  \todo The trivial algorithm
  <ul>
   <li> Given an ics (V,D,S,T,I) and the target condition C, the trivial
   algorithm is to compute R_0, R_1, ..., until either we find a k such that
   R_k contains an element fulfilling not C, or we finally find k and k' < k
   such that R_k = R_{k'}, in which case C is valid for the ics. </li>
   <li> There should exist some complexity result on solving ics showing
   that "in general" this is the best what can be done. </li>
  </ul>


  \todo AES as an ics
  <ul>
   <li> Is it possible to formulate AES as an ics (in a natural way), or
   do we need some enhancement? </li>
   <li> See
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/AdvancedEncryptionStandard.mac
   for our implementation of AES. </li>
   <li> The only obstacle should be the key schedule. </li>
   <li> The process of attacking AES (see
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
   for the general plans) should yield a first framework for solving ics
   via generalised SAT. </li>
  </ul>


  \todo Applications from railway safety
  <ul>
   <li> Discuss with PJ and Markus Roggenbach potential collaborations. </li>
  </ul>
  
*/

