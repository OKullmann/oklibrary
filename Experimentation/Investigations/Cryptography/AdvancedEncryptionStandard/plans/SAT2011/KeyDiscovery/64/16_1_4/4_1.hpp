// Matthew Gwynne, 16.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/64/16_1_4/4_1.hpp
  \brief Investigations into small scale AES key discovery for 4 + 1/3 round AES with a 16x1 block and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   4 + 1/3 round small scale AES with sixteen columns, one row, using the 
   4-bit field size. </li>
   <li> The AES encryption scheme we model takes a 64-bit plaintext,
   64-bit key and applies the following operations:
   <ol>
    <li> Key schedule which takes the key and generates two 64-bit round 
    keys. </li>
    <li> Addition of first round key (input key) to plaintext. </li>
    <li> Round (iterated four times):
    <ol>
     <li> Application of SubBytes (Sbox to each byte) operation. </li>
     <li> Application of ShiftRows operation. </li>
     <li> Application of MixColumns operation. </li>
     <li> Addition of (n+1)-th round key (from key schedule), resulting in the
     ciphertext. </li>
    </ol>
    </li>
   </ol>
   </li>
   <li> Note we have the following number of full rounds, special rounds,
   sboxes in the rounds, multiplications by each field element, sboxes in
   the key expansion, additions in the key expansion and constants in the
   key expansion:
   \verbatim
> component_statistics_ss(4,16,1,4,false,aes_mc_bidirectional);
[4,0,64,832,[[1,128]],4,256,16]
> component_statistics_ss(4,16,1,4,false,aes_mc_forward);
[4,0,64,576,[[1,64]],4,256,16]
   \endverbatim
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating small scale AES for 4 + 1/3 rounds:
   \verbatim
num_rounds : 4$
num_columns : 16$
num_rows : 1$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r4_c16_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1248 5244 15824 0 15824 1249 1
 length count
1 16
2 1024
3 3056
4 1148
   \endverbatim
   </li>
   <li> Then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r4_c16_rw1_e4_f0.cnf ssaes_pkpair_r4_c16_rw1_e4_f0_s1.cnf > experiment_r4_k1.cnf
   \endverbatim
   </li>
   <li> march_pl solves it in 6s:
   \verbatim
shell> march_pl experiment_r4_k.1.cnf
<snip>
c main():: nodeCount: 2509
c main():: dead ends in main: 2
c main():: lookAheadCount: 539606
c main():: unitResolveCount: 48512
c main():: time=6.010000
c main():: necessary_assignments: 10106
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 12092, succes #: 6168
c main():: doublelook: overall 2.285 of all possible doublelooks executed
c main():: doublelook: succesrate: 51.009, average DL_trigger: 126.297
<snip>
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves it in 19s:
   \verbatim
shell> minisat-2.2.0 experiment_r4_k1.cnf
<snip>
restarts              : 354
conflicts             : 130444         (6942 /sec)
decisions             : 159038         (0.00 % random) (8464 /sec)
propagations          : 94165695       (5011479 /sec)
conflict literals     : 3281874        (47.79 % deleted)
Memory used           : 25.00 MB
CPU time              : 18.79 s
   \endverbatim
   </li>
  </ul>


  \todo Using the rbase translation
  <ul>
   <li> Generating small scale AES for 4 + 1/3 rounds:
   \verbatim
num_rounds : 4$
num_columns : 16$
num_rows : 1$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r4_c16_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1248 5244 15824 0 15824 1249 1
 length count
1 16
2 1024
3 3056
4 1148
   \endverbatim
   </li>
   <li> Then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r4_c16_rw1_e4_f0.cnf ssaes_pkpair_r4_c16_rw1_e4_f0_s1.cnf > experiment_r4_k1.cnf
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves it in 8s:
   \verbatim
shell> minisat-2.2.0 experiment_r4_k1.cnf
<snip>
restarts              : 325
conflicts             : 122695         (15356 /sec)
decisions             : 137095         (0.00 % random) (17158 /sec)
propagations          : 42281428       (5291793 /sec)
conflict literals     : 1862830        (38.38 % deleted)
Memory used           : 19.00 MB
CPU time              : 7.99 s
   \endverbatim
   </li>
  </ul>

*/
