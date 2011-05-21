// Matthew Gwynne, 21.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/14_13.hpp
  \brief Investigations into small scale AES key discovery for 14 + 1/3 round AES with a 2x1 plaintext matrix and 8-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   14 + 1/3 round small scale AES with two rows, one columns, using the 8-bit
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
    <li> The following operations, the "round", iterated 14 times:
     <ul>
      <li> Application of SubBytes (Sbox to each 8-bit element) operation.
      </li>
      <li> Application of MixColumns operation.. </li>
      <li> Addition of round key i, resulting in the ciphertext. </li>
     </ul>
    </li>
   </ol>
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications
   as whole boxes and translating these boxes using the canonical translation.
   </li>
   <li> Generating small scale AES for 14 + 1/3 rounds:
   \verbatim
rounds : 14$
num_rows : 2$
num_columns : 1$
exp : 8$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r14_c1_rw2_e8_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
45072 734616 2160112 0 2160112 45073 1
 length count
1 112
2 688128
3 3200
17 43008
256 168
   \endverbatim
   </li>
   <li> In this translation, we have:
   <ul>
    <li> Fourteen full rounds (Key Addition, SubBytes, and MixColumn
    operation).
    </li>
    <li> 56 Sboxes (28 from SubBytes; 28 from key schedule). </li>
    <li> 800 additions (240 from key additions; 448 from MixColumns; 112 from
    key schedule). </li>
    <li> 56 multiplications by 02, 03 (from MixColumns).
    </li>
    <li> 112 bits for the constants in the key schedule. </li>
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
     <li> 112 unit-clauses (key schedule constant). </li>
     <li> 688128 binary clauses (56 Sboxes; 56 multiplications * 2). </li>
     <li> 3200 ternary clauses (800 arity two additions). </li>
     <li> 43008 clauses of length seventeen (56 Sboxes; 56
     multiplications * 2). </li>
     <li> 168 clauses of length 256 (56 Sboxes; 56 multiplications * 2). </li>
    </ul>
   </li>
   <li> Generate random assignments for the plaintext and ciphertext, leaving
   the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   Merge the assignments with the translations:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r14_c1_rw2_e4_f0.cnf ssaes_pkpair_r14_c1_rw2_e4_f0_s1.cnf > r14_keyfind.cnf; done
   \endverbatim
   </li>
   <li> All solvers solve in < 20 minutes:
    <ul>
     <li> precosat236
     \verbatim
shell> precosat236 r14_keyfind.cnf
c 56022 conflicts, 304526 decisions, 163 random
c 0 iterations, 172 restarts, 0 skipped
c 181.9 seconds, 148 MB max, 10 MB recycled
     \endverbatim
     </li>
     <li> cryptominisat:
     \verbatim
shell> cryptominisat r14_keyfind.cnf
c restarts                 : 168
c dynamic restarts         : 156
c static restarts          : 12
c conflicts                : 43829       (239.90    / sec)
c decisions                : 67335       (1.98      % random)
c bogo-props               : 8698446141  (47610542.64 / sec)
c conflict literals        : 7938218     (69.50     % deleted)
c Memory used              : 531.25      MB
c CPU time                 : 182.70      s
     \endverbatim
     </li>
     <li> minisat-2.2.0:
     \verbatim
shell> minisat-2.2.0 r14_keyfind.cnf
restarts              : 253
conflicts             : 82374          (368 /sec)
decisions             : 324814         (0.00 % random) (1452 /sec)
propagations          : 1071399042     (4790302 /sec)
conflict literals     : 27500184       (41.42 % deleted)
Memory used           : 207.00 MB
CPU time              : 223.66 s
     \endverbatim
     </li>
     <li> precosat-570.1:
     \verbatim
shell> precosat-570.1 r14_keyfind.cnf
c 90715 conflicts, 351716 decisions, 176 random
c 0 iterations, 1931 restarts, 0 skipped
c 570.7 seconds, 75 MB max, 27 MB recycled
     \endverbatim
     </li>
     <li> glucose:
     \verbatim
shell> glucose r14_keyfind.cnf
<snip>
c restarts              : 693
c nb ReduceDB           : 14
c nb learnts DL2        : 1319
c nb learnts size 2     : 570
c nb learnts size 1     : 5
c conflicts             : 404866         (478 /sec)
c decisions             : 2156003        (1.81 % random) (2545 /sec)
c propagations          : 1254906184     (1481240 /sec)
c conflict literals     : 165086655      (35.61 % deleted)
c Memory used           : 189.94 MB
c CPU time              : 847.2 s
     \endverbatim
     </li>
    </ul>
   </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r14_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 1 1 2 8 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>

*/
