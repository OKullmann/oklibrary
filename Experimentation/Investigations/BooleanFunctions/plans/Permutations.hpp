// Oliver Kullmann, 5.12.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/Permutations.hpp
  \brief Investigating boolean functions representing permutations of {0,1}^n


  \todo Generalities
  <ul>
   <li> For a natural number n >= 0 one considers the set V_n = {0,1}^n of all
   bit-vectors. </li>
   <li> The functions to be studied here are permutations f of V_n, represented
   as relations by boolean functions f^*: {0,1}^(2n), that is, f^*(x,y) = 1
   iff f(x) = y. </li>
   <li> There are 2^(2^(2n)) boolean functions altogether of 2n arguments,
   while there are (2^n)! such permutations f. </li>
   <li> V_n can be considered as an n-dimensional algebra over F_2 (the
   two-element field), consisting of the n-dimensional vectorspace over F_2
   plus the field-structure of V_n as a field of order 2^n. </li>
   <li> Special permutations of interest are as follows (based on the
   algebraic structures).
    <ol>
     <li> Linear automorphisms, of which there are order_gl(n,2). </li>
     <li> Special linear automorphisms are given by multiplication with
     non-zero field elements, of which there are 2^n-1. </li>
     <li> Translations, of which there are 2^n. </li>
     <li> Affine automorphisms, of which there are order_gl(n,2) * 2^n. </li>
     <li> The multiplicative inverse x -> x^-1, extended by 0 -> 0. </li>
     <li> The compositions of inversions with affine automorphisms. </li>
     <li> For such compositions, is there a fundamental difference between
     first applying the inversion or first applying the affine automorphism?
     </li>
    </ol>
    We need general tools (including representations and conversions) to handle
    these objects.
   </li>
   <li> Of course, besides these "algebraic permutations" we need to study
   random permutations. </li>
   <li> Isomorphism of boolean functions:
    <ol>
     <li> Most powerful is to admit permutations of variables and individual
     flips of variables. </li>
     <li> A basic question is how this compares with conjugatedness of
     permutations (equivalent to having the same cycle type)? </li>
     <li> 
    </ol>
   </li>
  </ul>


  \todo The case n=2
  <ul>
   <li> Here we have just (2^2)! = 24 permutations altogether, so we can
   conveniently list them all. </li>
   <li> The number of linear automorphisms is order_gl(2,2) = 6, while there
   are 2^2=4 translationen, which makes 24 affine automorphisms altogether.
   </li>
   <li> So here every permutation is an affine automorphism. </li>
  </ul>


  \todo The case n=3
  <ul>
   <li> Here we have just (2^3)! = 40320 permutations altogether, so we
   can still consider them all (algorithmically). </li>
   <li> The number of linear automorphisms is order_gl(3,2) = 168, while
   there are 2^3=8 translations, which makes 1344 affine automorphisms
   altogether. </li>
  </ul>


  \todo The case n=8
  <ul>
   <li> This case is especially interesting because of AES; see
   Cryptography/AdvancedEncryptionStandard/plans/FieldMulInvestigations.hpp
   and
   Cryptography/AdvancedEncryptionStandard/plans/SboxInvestigations.hpp.
   </li>
   <li> Here we have (2^8)! ~ 8.578*10^506 permutations altogether (while
   there are ~ 2.003*10^19728 boolean functions (in 16 variables). </li>
   <li> The number of linear automorphisms is order_gl(8,2) ~ 5.348*10^18.
   </li>
   <li> Special linear (affine) automorphisms to consider are the ones
   involved in the S-box and its inverse. </li>
  </ul>
  
*/

