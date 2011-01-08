// Matthew Gwynne, 2.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/general.hpp
  \brief Investigations into representations for components of the AES related to the SAT 2011 paper


  \todo Overview
  <ul>
   <li> One needs to systematically explore CNF representations, with and
   without new variables. </li>
   <li> We consider methods for computing the following representations
   for the AES boxes:
   <ul>
    <li> the prime implicates.
    <li> minimum CNF representations. </li>
    <li> small (minimal) CNF representations 
    (in cases where minimum representations are infeasible). </li>
   </ul>
   These are described in 
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Methods.hpp .
   </li>
   <li> For R-based representations see "R-based representations". </li>
  </ul>


  \todo Combining linear components
  <ul>
   <li> A full description of all of the possibilities for recombining
   AES and small scale linear components (from the Sbox and MixColumns)
   needs to be provided. </li>
   <li> The idea here is that, due to the linearity of the Sbox's affine
   transformation, and the MixColumns operations, as well as the
   fact the Shiftrows simply permutes bytes, the linear aspects of the Sbox
   can be moved out, and seperated (in the case of the affine addition),
   or merged into the boxes for the MixColumn. </li>
   <li> For the Sbox, we have 3 possibilities:
    <ul>
     <li> Full Sbox (M . s^(-1) + A) . </li>
     <li> Sbox minus addition of the affine constant (M . s^(-1)). </li>
     <li> Sbox minus affine transform entirely (s^(-1)). </li>
    </ul>
   </li>
   <li> For the MixColumn multiplications, we have 2 possibilities:
   <ul>
    <li> Standard byte-field multiplications. </li>
    <li> Sbox linear component (M) and Standard byte-field multiplications
    together. </li>
   </ul>
   </li>
   <li> A description of how to generate such translation is needed. </li>
   <li> See also "Rearranging linear components of Sbox and MixColumns" in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp. 
   </li>
  </ul>


  \todo Standard naming scheme for experiment files
  <ul>
   <li> We need to think of a naming scheme for the AES boxes so
   we can create the hpp files discussing them in this directory. </li>
   <li> We have the following boxes to investigate:
   <ul>
    <li> Sboxes with the following variants and parameters:
    <ul>
     <li> Sboxes with exponent / number of bits ranging from 1-8. </li>
     <li> Sboxes without and without the affine constant addition
     and linear multiplication (see "Combining linear components"). </li>
     <li> random permutations for the Sbox. </li>
     <li> random linear maps inside and outside the Sbox. </li>
    </ul>
    At the simplest level we vary the exponent (as in the 
    [Small Scale Variants of the AES; Cid, Murphy, Robshaw]) and
    keep the rest as defaults.
    </li>
    <li> Multiplication within the field with following variants:
    <ul>
     <li> the field element to multiply by. </li>
     <li> the exponent / number of bits ranging from 1-8. </li>
     <li> multiplications with and without the combination of
     the Sbox linear map (see "Combining linear components").
    </ul>
    </li>
   </ul>
   </li>
   <li> For now, we name:
   <ul>
    <li> the small scale Sboxes: Sbox_${e}.cnf , where ${e} is
    the field of the Sbox is a GF(2^e) finite field. </li>
    <li> the small scale multiplications by element "a", in the
    default field with exponent e: Mul_${a}_${e}.cnf . </li>
   </ul>
  </ul>


  \todo R-based representations
  <ul>
   <li> r-bases for r in {r_1,r_2}? (see rand_rbase_cs(F,r) in
   ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac.) </li>
   <li> One could consider certain prime implicates more important than others;
   for example ensuring that at least given a full input and/or a full output
   to one permutation the output resp. input can be inferred.
    <ol>
     <li> Can one formulate (relatively efficiently) the minimisation target
     that these inferences are "easily" available while otherwise using the
     smallest representation? </li>
     <li> We could generalise the notion of r-base w.r.t. specific clauses
     which have to be deducible via r, while all (other) removed clauses just
     need to follow logically, or perhaps using some stronger reduction. </li>
    </ol>
   </li>
   <li> When investigations begin fully in this area, this todo should be
   moved to a new file, and most likely a new sub-module. </li>
  </ul>


  \todo Understanding prime implicates after any partial assignment
  <ul>
   <li> To consider the AES boxes as an "active clause", we want to first be 
   able, given a partial assignment, to infer as many forced assignments
   as possible. This can be done simply with the DNF representation. </li>
   <li> However, secondly one needs, given a partial assignment, to be able to
   determine various measures for heuristics. </li>
   <li> Therefore, investigating several statistics (most notably the number of
   clauses for a given variable) of the prime implicates of the clause-set, 
   formed after taking the Sbox and applying each partial assignment, is 
   necessary to try and discern a pattern. </li>
   <li> If such patterns can be deduced for particular clause-set measures,
   then the active clause can use this pattern, given a partial assignment, 
   to return reasonable values for these measures which can be used for 
   statistics. </li>
   <li> A C++ implementation of such a system whereby the set of prime 
   implicates is taken as input, and each partial assignment along with
   the relevant statistics is returned is necessary. </li>
   <li> Such a C++ implementation would need to be able to apply a partial
   assignment to a clause-set and then compute various statistics on the 
   result. This would need to be done for every partial assignment. </li>
   <li> After applying the partial assignment, to gain the prime implicates
   of the new boolean function, one must simply apply subsumption elimination
   to the new clause-set (which is just result of applying a partial assignment
   to the prime implicates of the original function). This can be done using 
   functionality already in the library (MG: Where?). </li>
  </ul>

*/
