// Matthew Gwynne, 17.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_2_4/20_13.hpp
  \brief Investigations into small scale AES key discovery for one round AES with a 2x2 block and 4-bit field elements (20+1/3)


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   20 + 1/3 round small scale AES with two columns and two rows, including the
   MixColumns operation. </li>
   <li> The AES encryption scheme we model takes a 16-bit plaintext,
   16-bit key and applies the following operations:
   <ol>
    <li> Key schedule which takes the key and generates 21 16-bit round 
    keys. </li>
    <li> Addition of first round key (input key) to plaintext. </li>
    <li> Round (iterated n times):
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
   sboxes in the rounds, additions in the rounds, multiplications by each 
   field element, sboxes in the key expansion, additions in the key expansion 
   and constants in the key expansion:
   \verbatim
> component_statistics_ss(20,2,2,4,false,aes_mc_bidirectional);
[20,0,80,976,[[x,160],[x+1,160]],40,320,80]
> component_statistics_ss(20,2,2,4,false,aes_mc_forward);
[20,0,80,656,[[x,80],[x+1,80]],40,320,80]
   \endverbatim
   Note that the inverse MixColumns uses the same multiplication
   constants as the MixColumns.
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating small scale AES for four rounds (with MixColumns):
   \verbatim
rounds : 20$
num_columns : 2$
num_rows : 2$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r20_c2_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
9888 69384 200272 0 200272 9889 1
 length count
1 80
2 56320
3 4864
4 640
9 7040
16 440
   \endverbatim
   </li>
   <li> We have the following statistics (computed):
   \verbatim
maxima> ncl_list_ss(20,2,2,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,80],[2,56320],[3,4864],[4,640],[9,7040],[16,440]]
   \endverbatim
   </li>
   <li> Then we can generate random assignments with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignments with the translations:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r20_c2_rw2_e4_f0.cnf ssaes_pkpair_r20_c2_rw2_e4_f0_s1.cnf > experiment_r20_k1.cnf
shell> AppendDimacs-O3-DNDEBUG ssaes_r20_c2_rw2_e4_f0.cnf ssaes_pkpair_r20_c2_rw2_e4_f0_s1.cnf > experiment_r20_k5.cnf
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves this in times ranging from 3.45s to 40s:
   \verbatim
shell> minisat-2.2.0 experiment_r20_k1.cnf
<snip>
restarts              : 18
conflicts             : 3513           (1018 /sec)
decisions             : 3838           (0.00 % random) (1112 /sec)
propagations          : 29196071       (8462629 /sec)
conflict literals     : 43580          (86.63 % deleted)
Memory used           : 24.00 MB
CPU time              : 3.45 s

shell> minisat-2.2.0 experiment_r20_k5.cnf
<snip>
restarts              : 158
conflicts             : 52416          (1426 /sec)
decisions             : 59697          (0.00 % random) (1624 /sec)
propagations          : 262216771      (7131269 /sec)
conflict literals     : 1656658        (69.32 % deleted)
Memory used           : 32.00 MB
CPU time              : 36.77 s
   \endverbatim
   </li>
  </ul>

*/
