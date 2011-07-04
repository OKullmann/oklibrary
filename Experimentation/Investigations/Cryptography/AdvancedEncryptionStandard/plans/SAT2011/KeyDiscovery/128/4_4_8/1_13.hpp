// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/128/4_4_8/1_13.hpp
  \brief Investigations into AES key discovery for one round AES (1+1/3)


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   1 + 1/3 round small scale AES with four rows, four columns, using the 8-bit
   field size.
   </li>
   <li> Note that this is not the standard AES, as all rounds are the same;
   there is no special final round. </li>
   <li> The AES encryption scheme we model takes a 128-bit plaintext and
   128-bit key and outputs a 128-bit ciphertext. The plaintext, key and
   ciphertext are all considered, column by column, as 4x4 matrices of 8-bit
   elements.
   ??? what is the meaning of "we model" ??? are there several AES encryption
   schemes ???
   </li>
   <li> In other words, in the AES blocks (plaintext, key, ciphertext etc),
   the 8-bit element at position (i,j) in the matrix is the ((i-1)*4 + j)-th
   8-bit word of the 128-bits. </li>
   <li> The 8-bit element (b_0,b_1,b_2,b_3,b_4,b_5,b_6,b_7) is considered as
   the polynomial b_0 * x^7 + b_1 * x^6 + b_2 * x^5 + b_4 * x^3 + b_5 * x^2 +
   b^6 * x + b_7. Addition and multiplication on these polynomials is defined
   as usual, modulo the polynomial x^8+x^4+x^3+x+1. </li>
   <li> The encryption scheme applies the following operations:
   <ol>
    <li> Addition of round key 0 (input key) to plaintext. </li>
    <li> Application of SubBytes (Sbox to each 8-bit element) operation. </li>
    <li> Application of MixColumns operation. </li>
    <li> Addition of round key 1, resulting in the ciphertext. </li>
   </ol>
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications
   as whole boxes and translating these boxes using the canonical translation.
   </li>
   <li> Generating AES for 1 + 1/3 round:
   \verbatim
num_rounds : 1$
num_rows : 4$
num_columns : 4$
exp : 8$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

???
Maxima encountered a Lisp error:

 Memory limit reached. Please jump to an outer pointer, quit program and enlarge the
memory limits before executing the program again.
???

shell> cat ssaes_r1_c4_rw4_e8_f0.cnf | ExtendedDimacsFullStatistics n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
31400 510620 1510056 0 1510056 31401 1
 length count
1 8
2 475136
3 1504
4 64
5 4096
17 29696
256 116
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> One full round. </li>
     <li> 20 Sboxes:
      <ul>
       <li> 16 from SubBytes = 16 bytes * 1 round. </li>
       <li> 4 from key schedule = 1 column * 4 bytes * 1 round. </li>
      </ul>
     <li> 640 additions:
      <ol>
       <li> 376 arity two additions:
        <ul>
         <li> 128 from key additions = 128 bits * 1 round. </li>
         <li> 128 from final key addition = 128 bits. </li>
         <li> 120 from key schedule = (128 - 8) bits *  1 round. </li>
        </ul>
       </li>
       <li> 8 arity three additions: 8 from key schedule = 8 bits * 1 round.
       </li>
       <li> 256 arity four additions: 256 from summation in
       MixColumns matrix multiplication = 4 additions * 4 columns * 16 bits *
       1 round. </li>
      </ul>
     </li>
     <li> 16 multiplications by 02: 4 rows * 1 multiplication * 4 columns
     * 1 round. </li>
     <li> 16 multiplications by 03: 4 rows * 1 multiplication * 4 columns
     * 1 round. </li>
     <li> 16 multiplications by 09: 4 rows * 1 multiplication * 4 columns
     * 1 round. </li>
     <li> 16 multiplications by 11: 4 rows * 1 multiplication * 4 columns
     * 1 round. </li>
     <li> 16 multiplications by 13: 4 rows * 1 multiplication * 4 columns
     * 1 round. </li>
     <li> 16 multiplications by 14: 4 rows * 1 multiplication * 4 columns
     * 1 round. </li>
     <li> 8 bits set for the constant in the key schedule. </li>
    </ul>
   </li>
   <li> The number of clauses of each length in the canonical box
   translations:
   \verbatim
maxima> ncl_list_full_dualts(16,256);
[[2,4096],[17,256],[256,1]]
   \endverbatim
   </li>
   <li> The instance has 20 S-boxes + 16 * 6 multiplications = 116 boxes
   (excluding additions). </li>
   <li> The instance has the following number of clauses of the
   following size:
    <ul>
     <li> 8 unit-clauses (key schedule constant). </li>
     <li> 475136 binary clauses (116 boxes * 4096 = 475136). </li>
     <li> 1504 ternary clauses (376 arity two additions * 4 clauses = 1504).
     </li>
     <li> 64 clauses of length four (8 arity three additions * 8 clauses = 64).
     </li>
     <li> 4096 clauses of length five (256 arity four additions * 16 clauses =
     4096). </li>
     <li> 29696 clauses of length seventeen (116 boxes * 256 clauses = 29656).
     </li>
     <li> 116 clauses of length 256 (116 boxes * 1 clause = 116). </li>
    </ul>
   </li>
   <li> Then we can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw4_e8_f0.cnf ssaes_pkpair_r1_c4_rw4_e8_f0_s1.cnf > ssaes_r1_c4_rw4_e8_f0_keyfind.cnf
   \endverbatim
   </li>
   <li> MG is running experiments with various solvers, but all are ongoing
   (after a day). </li>
   <li> The next thing to try is replacing the boxes with the r_1 bases we
   have. </li>
  </ul>

*/
