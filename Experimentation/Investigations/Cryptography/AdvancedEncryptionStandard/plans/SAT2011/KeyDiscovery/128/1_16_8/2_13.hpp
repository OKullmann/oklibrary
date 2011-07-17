// Matthew Gwynne, 18.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/128/1_16_8/2_13.hpp
  \brief Investigations into small scale AES key discovery for 2+1/3 round AES with one row and 16 columns


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   2 + 1/3 round small scale AES with one row, sixteen columns, using the
   8-bit field size. </li>
   <li> The AES encryption scheme we model takes a 128-bit plaintext and
   128-bit key and outputs a 128-bit ciphertext. </li>
   <li> Each consequtive 8-bits (b_0,b_1,b_2,b_3,b_4,b_5,b_6,b_7) is
   considered as the polynomial b_0 * x^7 + b_1 * x^6 + b_2 * x^5 + b_4 * x^3
   + b_5 * x^2 + b^6 * x + b_7. Addition and multiplication on these
   polynomials is defined as usual, modulo the polynomial
   x^8+x^4+x^3+x+1. </li>
   <li> The encryption scheme applies the following operations:
   <ol>
    <li> Addition of round key 0 (input key) to plaintext. </li>
    <li> Application of SubBytes (Sbox to each 8-bit element) operation. </li>
    <li> Addition of round key 1. </li>
    <li> Application of SubBytes (Sbox to each 8-bit element) operation. </li>
    <li> Addition of round key 2, resulting in the ciphertext. </li>
   </ol>
   </li>
   <li> The Sbox is non-linear permutation over the set of 4-bit elements,
   defined as inversion within the 4-bit field composed with an affine
   transformation. </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications
   as whole boxes and translating these boxes using the canonical translation.
   </li>
   <li> Generating AES for 2 + 1/3 round:
   \verbatim
num_rounds : 2$
num_rows : 1$
num_columns : 16$
exp : 8$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r2_c16_rw1_e8_f0.cnf | ExtendedDimacsFullStatistics n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10144 151666 445264 0 445264 10145 1
 length count
1 16
2 140288
3 2496
4 128
17 8704
256 34
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> Two full rounds (Key Addition, SubBytes, and MixColumns operation).
     </li>
     <li> 34 S-boxes:
      <ul>
       <li> 32 from SubBytes = 16 bytes * 2 rounds; </li>
       <li> 2 from key schedule = 1 row * 1 word * 2 rounds. </li>
      </ul>
     </li>
     <li> 1156 additions:
      <ul>
       <li> 512 additions of arity 1:
        <ul>
         <li> 512 from MixColumns = 128 bits * 2 round * 2 directions; </li>
        </ul>
       </li>
       <li> 624 additions of arity 2:
        <ul>
         <li> 256 from key additions = 128 bits * 2 round; </li>
         <li> 128 from final key addition = 128 bits; </li>
         <li> 240 from the key schedule = (128 bits - 8 bits) * 2 round. </li>
        </ul>
       </li>
       <li> 16 additions of arity 3:
        <ul>
         <li> 16 from the key schedule = 8 bits * 2 rounds. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 16 bits for the constant in the key schedule = 8 bits * 2 rounds.
     </li>
    </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_full_dualts(8,16);
[[2,4096],[17,256],[256,1]]
   \endverbatim
   </li>
   <li> This instance has 60 boxes = 34 S-boxes. </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 16 = key schedule constant * 1; </li>
     <li> 2 : 140288 = 34 boxes * 4096 + 512 additions (arity 1) * 2; </li>
     <li> 3 : 2496 = 624 additions (arity 2) * 4; </li>
     <li> 4 : 128 = 16 additions (arity 3) * 8; </li>
     <li> 17 : 8704 = 34 boxes * 256; </li>
     <li> 256 : 34 = 34 boxes * 1. </li>
    </ul>
   </li>
   <li> Then we can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r2_c16_rw1_e8_f0.cnf ssaes_pkpair_r2_c16_rw1_e8_f0_s1.cnf > experiment_r2_k1.cnf_minisat22
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves it in 40s:
   \verbatim
shell> minisat-2.2.0 experiment_r2_k1.cnf
<snip>
restarts              : 127
conflicts             : 40088          (1009 /sec)
decisions             : 143275         (0.00 % random) (3606 /sec)
propagations          : 135629939      (3413792 /sec)
CPU time              : 39.73 s
   \endverbatim
   </li>
  </ul>

*/
