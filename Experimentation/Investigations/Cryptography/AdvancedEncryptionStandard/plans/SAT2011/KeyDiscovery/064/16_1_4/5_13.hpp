// Matthew Gwynne, 16.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/064/16_1_4/5_13.hpp
  \brief Investigations into small scale AES key discovery for 5 + 1/3 round AES with a 16x1 block and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   5 + 1/3 round small scale AES with sixteen columns, one row, using the 
   4-bit field size. </li>
   <li> The AES encryption scheme we model takes a 64-bit plaintext,
   64-bit key and applies the following operations:
   <ol>
    <li> Round (iterated five times):
    <ol>
     <li> Addition of round key n-1. </li>
     <li> Application of SubBytes (Sbox to each byte) operation. </li>
     <li> Application of MixColumns' operation. </li>
    </ol>
    </li>
    <li> Addition of round key n yielding the ciphertext. </li>
   </ol>
   </li>
   <li> Note we have the following number of full rounds, special rounds,
   sboxes in the rounds, multiplications by each field element, sboxes in
   the key expansion, additions in the key expansion and constants in the
   key expansion:
   \verbatim
> component_statistics_ss(5,16,1,4,false,aes_mc_bidirectional);
[5,0,80,1024,[[1,160]],5,320,20]
> component_statistics_ss(5,16,1,4,false,aes_mc_forward);
[5,0,80,704,[[1,80]],5,320,20]
   \endverbatim
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating small scale AES for 5 + 1/3 rounds:
   \verbatim
num_rounds : 5$
num_columns : 16$
num_rows : 1$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r5_c16_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
2872 16521 46788 0 46788 2873 1
 length count
1 20
2 12160
3 2736
4 160
9 1360
16 85
   \endverbatim
   </li>
   <li> Then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r5_c16_rw1_e4_f0.cnf ssaes_pkpair_r5_c16_rw1_e4_f0_s1.cnf > experiment_r5_k1.cnf
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves it in 25.4s:
   \verbatim
shell> minisat-2.2.0 experiment_r5_k1.cnf
<snip>
restarts              : 404
conflicts             : 151945         (5982 /sec)
decisions             : 184854         (0.00 % random) (7278 /sec)
propagations          : 147381578      (5802424 /sec)
conflict literals     : 4313436        (57.69 % deleted)
Memory used           : 26.00 MB
CPU time              : 25.4 s
   \endverbatim
   </li>
  </ul>


  \todo Using the rbase translation
  <ul>
   <li> Generating small scale AES for 5 + 1/3 rounds:
   \verbatim
num_rounds : 5$
num_columns : 16$
num_rows : 1$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r5_c16_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1512 6491 19588 0 19588 1513 1
 length count
1 20
2 1280
3 3756
4 1435
   \endverbatim
   </li>
   <li> Then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r5_c16_rw1_e4_f0.cnf ssaes_pkpair_r5_c16_rw1_e4_f0_s1.cnf > experiment_r5_k1.cnf
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves it in 239s:
   \verbatim
shell> minisat-2.2.0 experiment_r5_k1.cnf
<snip>
restarts              : 5115
conflicts             : 2877957        (12075 /sec)
decisions             : 3273542        (0.00 % random) (13735 /sec)
propagations          : 869271128      (3647189 /sec)
conflict literals     : 59660707       (32.79 % deleted)
Memory used           : 37.00 MB
CPU time              : 238.34 s
   \endverbatim
   </li>
  </ul>

*/
