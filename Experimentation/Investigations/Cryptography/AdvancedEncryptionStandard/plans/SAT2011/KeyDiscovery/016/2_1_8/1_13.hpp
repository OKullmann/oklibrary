// Matthew Gwynne, 20.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/1_13.hpp
  \brief Investigations into small scale AES key discovery for 1 + 1/3 round AES with a 2x1 plaintext matrix and 8-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   1 + 1/3 round small scale AES with two rows, one columns, using the 8-bit
   field size. </li>
   <li> The AES encryption scheme we model takes a 16-bit plaintext and
   16-bit key and outputs a 16-bit ciphertext. </li>
   <li> The plaintext, key and ciphertext are all considered, column by
   column, as 2x1 matrices of 8-bit elements. </li>
   <li> In other words, the 8-bit element at position (i,1) in the AES matrix
   is the i-th 8-bit word of the 16-bits. </li>
   <li> The 8-bit element (b_0,b_1,b_2,b_3,b_4,b_5,b_6,b_7) is considered as
   the polynomial b_0 * x^3 + b_1 * x^2 + b_2 * x + b_3. </li>
   <li> Addition and multiplication on these polynomials is defined as usual,
   modulo the polynomial x^8+x^4+x^3+x+1. </li>
   <li> The encryption scheme applies the following operations:
   <ol>
    <li> Addition of round key 0 (input key) to plaintext. </li>
    <li> Application of SubBytes (Sbox to each 8-bit element) operation. </li>
    <li> Application of MixColumns operation.. </li>
    <li> Addition of round key 1, resulting in the ciphertext. </li>
   </ol>
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications
   as whole boxes and translating these boxes using the canonical translation.
   </li>
   <li> Generating small scale AES for 1 + 1/3 rounds:
   \verbatim
rounds : 1$
num_rows : 2$
num_columns : 1$
exp : 8$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c1_rw2_e8_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
3264 52532 154472 0 154472 3265 1
 length count
1 8
2 49152
3 288
17 3072
256 12
   \endverbatim
   </li>
   <li> In this translation, we have:
   <ul>
    <li> One full round (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 4 Sboxes (2 from SubBytes; 2 from key schedule). </li>
    <li> 72 additions (32 from key additions; 32 from MixColumns; 8 from
    key schedule). </li>
    <li> 4 multiplications by 02, 03 (from MixColumns).
    </li>
    <li> 8 bits for the constant in the key schedule. </li>
   </ul>
   </li>
   <li> Note that as this variant has only one column, the key schedule
   applies Sbox(K_i) + C rather than Sbox(K_i) + K_j + C where K_i and
   K_j are key words from the previous round key. </li>
   <li> The Sboxes and multiplications use the canonical translation,
   which has the following number of clauses of each length:
   \verbatim
maxima> ncl_list_full_dualts(16,256);
[[2,4096],[17,256],[256,1]]
   \endverbatim
   </li>
   <li> The number of clauses of each length in the translation are:
    <ul>
     <li> 8 unit-clauses (key schedule constant). </li>
     <li> 49152 binary clauses (4 Sboxes; 4 multiplications * 2). </li>
     <li> 288 ternary clauses (72 arity two additions). </li>
     <li> 3072 clauses of length seventeen (4 Sboxes; 4 multiplications * 2).
     </li>
     <li> 12 clauses of length 256 (4 Sboxes; 4 multiplications * 2). </li>
    </ul>
   </li>
   <li> Generate random assignments for the plaintext and ciphertext, leaving
   the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   Merge the assignments with the translations:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c1_rw2_e4_f0.cnf ssaes_pkpair_r1_c1_rw2_e4_f0_s1.cnf > r1_keyfind.cnf; done
   \endverbatim
   </li>
   <li> All solvers solve in <1s:
    <ul>
     <li> minisat-2.2.0:
     \verbatim
shell> minisat-2.2.0 r1_keyfind.cnf
restarts              : 6
conflicts             : 647            (5882 /sec)
decisions             : 7432           (0.00 % random) (67564 /sec)
propagations          : 641324         (5830218 /sec)
conflict literals     : 145083         (27.42 % deleted)
Memory used           : 22.00 MB
CPU time              : 0.11 s
     \endverbatim
     </li>
     <li> glucose:
     \verbatim
shell> glucose r1_keyfind.cnf
<snip>
c restarts              : 1
c conflicts             : 653            (5023 /sec)
c decisions             : 4658           (1.70 % random) (35831 /sec)
c propagations          : 380621         (2927854 /sec)
c conflict literals     : 202983         (17.84 % deleted)
c Memory used           : 5.82 MB
c CPU time              : 0.13 s
     \endverbatim
     </li>
     <li> cryptominisat:
     \verbatim
shell> cryptominisat r1_keyfind.cnf
c restarts                 : 1
c dynamic restarts         : 0
c static restarts          : 1
c conflicts                : 0           (0.00      / sec)
c decisions                : 1           (0.00      % random)
c bogo-props               : 39978329    (105206128.95 / sec)
c conflict literals        : 0           (-nan      % deleted)
c Memory used              : 32.11       MB
c CPU time                 : 0.38        s
     \endverbatim
     </li>
     <li> precosat236
     \verbatim
shell> precosat236 r1_keyfind.cnf
c 0 conflicts, 0 decisions, 0 random
c prps: 851728 propagations, 7.74 megaprops
c
c 0.1 seconds, 4 MB max, 2 MB recycled
     \endverbatim
     </li>
     <li> precosat-570.1:
     \verbatim
shell> precosat-570.1 r1_keyfind.cnf
c 414 conflicts, 10588 decisions, 2 random
c 10 iterations, 22 restarts, 0 skipped
c 0.4 seconds, 4 MB max, 0 MB recycled
     \endverbatim
     </li>
     <li> OKsolver_2002:
     \verbatim
shell> OKsolver_2002-O3-DNDEBUG r1_keyfind.cnf
c running_time(sec)                     0.7
c number_of_nodes                       1
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                255
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
     \endverbatim
     </li>
     <li> Cryptominisat and the OKsolver solve the instance by UCP. Why
     don't the others? Is this just luck or stronger reductions? </li>
    </ul>
   </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r1_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 1 1 2 8 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>

*/
