// Matthew Gwynne, 31.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_4.hpp
  \brief Investigations into the 4-bit AES S-box


  \bug Update
  <ul>
   <li> Commit b47ed2bac74daaf648852bba0c61f26b3e7f7c8f added some
   representation --- where does this come from, and where it is represented
   here? </li>
  </ul>


  \todo Basic data
  <ul>
   <li> Generating the full CNF representation:
    <ol>
     <li> The CNF-file "AES_Sbox_full.cnf" is created by the Maxima-function
     output_ss_sbox_fullcnf_stdname(2,4,ss_polynomial_2_4) in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/SboxAnalysis.mac,
     which is a full clause-set with 8
     variables and 2^8 - 2^4 = 240 clauses:
     \verbatim
> cat AES_sbox_2_4_full.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 240 1920 0 1920 1 1
 length count
8 240
     \endverbatim
     </li>
     <li> The underlying clause-set is
     ss_sbox_fullcnf_fcs(2,4,ss_polynomial_2_4). </li>
     <li> This clause-set is also computed by
     bf2relation_fullcnf_fcs(lambda([V],ss_sbox_bf(V,2,4)),4). </li>
    </ol>
   </li>
   <li> Prime implicates:
    <ol>
     <li> There are 147 prime implicates, with 581 literals in total, and
     with clause-length-distribution as follows:
     \verbatim
> QuineMcCluskey-n16-O3-DNDEBUG AES_sbox_2_4_full.cnf > AES_sbox_2_4_full.cnf_primes
> cat AES_sbox_2_4_full.cnf_primes | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 147 581 0 581 1 1
 length count
3 22
4 110
5 15
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Minimum representations:
    <ul>
     <li> A minimum representation can be computed by:
     \verbatim
maxima> oklib_load_all()$
maxima> output_ss_sbox_fullcnf_stdname(2,4,ss_polynomial_2_4);
shell> OKP=~/Work/OKlibrary/OKplatform/ ${OKP}/OKsystem/OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/minimise_cnf_cryptominisat AES_sbox_2_4_full.cnf | tee Sbox_4_min.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 22 82 0 82 1 1
 length count
3 8
4 12
5 2
       \endverbatim
       </li>
       <li> The hardness of this representation is 2:
       \verbatim
shell> Sbox_4_min : read_fcl_f("Sbox_4_min.cnf")$
shell> hardness(setify(Sbox_4_min[2]));
2
     \endverbatim
     </li>
     <li> See "Hardness of boolean function representations" in
     Experimentation/Investigations/BooleanFunctions/plans/general.hpp
     for discussion on the notions of hardness and methods
     for computing it. </li>
    </ul>
   </li>
  </ul>


  \todo Overview
  <ul>
   <li> We do *not* currently know the minimum CNF size for the 4-bit Sbox.
   </li>
   <li> Here we should have an overview of the current state of this
   investigation and open problems. </li>
  </ul>


  \todo Combining Sbox with field addition
  <ul>
   <li> The AES key schedule includes various field additions which applying
   associativity and commutativity rules can be combined with the Sbox of the
   round *after* the key schedule. </li>
   <li> In particular, the round key is output by the key schedule, and then
   added (XORed) to the result of the previous round, and given as input to
   the next round. </li>
   <li> Within the key schedule, each word is the result of the addition
   of one key word, with the result of the Sbox operation and addition of
   certain other key words from the previous round key. </li>
   <li> As each round key results from the addition of a key word from the
   previous round, one should investigate combining this key word addition
   with the the Sbox for the next round. </li>
   <li> For this case (4-bit boxes), this would yield a 12-bit relation,
   8-bits input (round input and key) and 4-bits output. </li>
   <li> Such a function should be fully analysed. </li>
   <li> Prime implicates:
   \verbatim
maxima> FF : bf2relation_fullcnf_fcs(lambda([V],ss_sbox_w_add_bf(V,2,4)),8)$
maxima> output_fcs("SboxPlusKey4.cnf", FF, "SboxPlusKey4.cnf")$
> cat SboxPlusKey4.cnf_primes | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
12 634 4020 0 4020 1 1
 length count
4 20
5 100
6 262
7 180
8 40
9 32
   \endverbatim
   </li>
   <li> See ss_sbox_w_add_bf in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/SmallScaleAdvancedEncryptionStandard.mac.
   </li>
  </ul>

*/
