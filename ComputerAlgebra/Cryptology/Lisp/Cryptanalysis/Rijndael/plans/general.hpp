// Matthew Gwynne, 19.2.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
  \brief Plans for the Cryptanalysis of Rijndael in Maxima/Lisp


  \todo Connections
  <ul>
   <li> See Applications/Cryptanalysis/plans/Rijndael.hpp for the C++ level.
   </li>
   <li> See
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp
   for investigations. </li>
  </ul>


  \todo Functions should not cache return values
  <ul>
   <li> Functions such as ss_mul_ts_gen and ss_sbox_ts_gen should
   not cache their result (see
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/ConstraintTemplateSmallScaleRewriteRules.mac). 
   </li>
   <li> While this may save a considerable amount of time in
   some computations, it is poor practice as it causes these functions
   to have unintended side-effects. We should try to make as many functions as
   possible side-effect free. </li>
   <li> Instead, we should use functionality that allows us to cache the 
   results of arbitrary function calls. See "Introduce memoise function 
   wrapper" in ComputerAlgebra/DataStructures/Lisp/plans/HashMaps.hpp . </li>
  </ul>


  \todo Update
  <ul>
   <li> It seems that these todos as well as the milestones are out-of-date.
   </li>
   <li> Especially the small-scale generalisation is not mentioned. </li>
   <li> Likely the version number should be advanced. </li>
   <li> The todos "Review and tidy todos" and "Create / update milestones"
   are not mentioned in the milestones, which must not happen! MG must
   look up these todos in this history, and either remove them (potentially
   adding to the milestones-history), or add them to the current milestones.
   </li>
  </ul>


  \todo Rewrite translation functions using ss_field_op_fulldnf_gen_fcl etc
  <ul>
   <li> Functions such as ss_sbox_fulldnf_gen_fcl should be rewritten using
   ss_field_op_fulldnf_gen_fcl and passing in the appropriate operations.
   </li>
  </ul>

  
  \todo Discussion on Generalisation/Parameterisation based on [Algebraic
  Aspects of the AES]
  <ul>
   <li> [Algebraic Aspects of the AES] discusses several generalisations of
   Rijndael. </li>
   <li> Rather than using GF(2^8), a parameter <em>e</em> is introduced, which 
   specifies 4 or 8 to indicate whether the block should deal in elements in 
   GF(2^4) or elements in GF(2^8) where appropriate modulo polynomials and
   S_rd affine transforms are defined for GF(2^4). This seems interesting
   because inversion within GF(2^8) can be expressed as operations on the
   inversion of the two GF(2^4) elements comprising it (see discussion on
   efficient implementation  of AES in [Design of Rijndael]). </li>
   <li> <em>n_R</em> is the number of rows in the block and may range over
   {1,2,4} where the normal AES/Rijndael default is 4. Clearly here the main
   issue is with Mixcolumns which works on the columns of size 4 and so
   different constants over these 1, 2 or 4 element polynomials but with the
   same basic operation involved (multiplying each column by a constant in
   that Quotient Ring). </li>
   <li> <em>n_C</em> is the number of rows in the block and may range over
   {1,2,4}. This only affects ShiftRows and as with n_R, variants are defined
   for each of these. </li>
   <li> <em>r</em> is the number of rounds as normal. </li>
   <li> Such abstractions seem to offer more interesting ways of generalising
   and producing AES/Rijndael variants with reduced complexity, which might
   offer better and possibly interesting results with translations and the
   relationships between these variants and the full AES seems less explored in
   previous research than simple reduced round variants of the cipher. </li>
   <li> So we get a more general parameterised AES function of the form
   AES(r,n_R, n_C,e)(P,K,C), where
    <ol>
     <li> r is the number of rounds, </li>
     <li> n_R is the number of rows in the block, </li>
     <li> n_C is the number of columns of length n_R, </li>
     <li> and e is the word size, normally 8 (ie GF(2^8) elements), but
     extended to include values of e of 4 or 8. </li>
    </ol>
   </li>
   <li> It would be nice to include such generalisations (more than the obvious
   round variable r, which is fairly trivial to include) as including a variety
   of parameters which can be reduced to make more easily attackable, and more
   thoroughly analysable AES variants is advantageous, as most likely the full
   AES will not be broken and simple reduced round variants seem less
   interesting, than reducing parameters such as e. </li>
   <li> Especially because inversion within GF(2^8) can be expressed as
   operations on the inversion of the two GF(2^4) elements comprising it (see
   discussion on efficient implementation of AES in [Design of Rijndael]), and
   the relationships between these variants and the full AES seems less
   explored in previous research than simple reduced round variants of the
   cipher. </li>
  </ul>


  \todo DONE Randomly choosing "unknown" block bits
  <ul>
   <li> This is unnecessary. One should always specify the seed themselves, and
   there is no advantage to having some randomly generated seed here. </li>
   <li> One may randomly choose bits to leave "unknown" using 
   "random_sublist_l" (see 
   ComputerAlgebra/Satisfiability/Lisp/Generators/RandomClauseSets.mac) . </li>
   <li> Given a block (plaintext, key or ciphertext), a method is needed of
   randomly choosing bits from this block to "unset" (that is, leave unknown). 
   </li>
   <li> Two functions should perform this task, one taking the standard 
   arguments and additionally a random seed and returning a partial assignment, 
   and another which takes only the standard arguments and returns a list with 
   both the partial assignment and a randomly chosen seed. </li>
   <li> So one function should take:
   <ul> 
    <li> a block "B" as a list of integers (as specified in the AES 
    implementation). </li>
    <li> a list of variables "V" representing the bits for this block. </li> 
    <li> the number of bits "k" to leave unspecified in the SAT problem. </li>
   </ul>
   and should then return a list [pa,r_s] where:
   <ul>
    <li> "pa" is a partial assignment to the variables in V other than the "k" 
    randomly chosen variables. Any variable that the partial assignment assigns
    a value to should have the value associated with that "bit" in block "B" 
    that the variable represents. </li>
    <li> "r_s" is the random seed used to choose which block bits are left
    unspecified. </li>
   </ul>
   </li>
   <li> The other function should take:
   <ul> 
    <li> A random seed "r_s". </li>
    <li> a block "B" as a list of integers (as specified in the AES 
    implementation). </li>
    <li> a list of variables "V" representing the bits for this block. </li> 
    <li> the number of bits "k" to leave unspecified in the SAT problem. </li>
   </ul>
   and should then return a partial assignment "pa" where "pa" is a partial 
   assignment to the variables in V other than the "k" randomly chosen 
   variables. Any variable that the partial assignment assigns a value to, 
   should have the value associated with that "bit" in block "B" that
   the variable represents. 
   </li>
  </ul>


  \todo Review and tidy todos DONE


  \todo Create / update milestones DONE
  <ul>
   <li> The first goal is with milestone 0.1, where we have a working
   translation. </li>
  </ul>


  \bug DONE (caused by lack of uniqueness of hash keys used to
  cache the values)
  Test failure at level "full"
  <ul>
   <li> For Rijndael/tests/ConstraintTemplateSmallScaleRewriteRules.mac we get
   \verbatim
okltest_ss_mul_ts_var_l(ss_mul_ts_var_l)
Evaluation took 0.0900 seconds (0.1800 elapsed)
true
okltest_ss_mul_ts_cst_cl(ss_mul_ts_cst_cl)
map: arguments must be the same length.
   \endverbatim
   </li>
  </ul>

*/

