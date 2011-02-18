// Matthew Gwynne, 18.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/128/16_1_8/2_13.hpp
  \brief Investigations into small scale AES key discovery for 2+/13 round AES with 16 columns


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   2+1/3 round AES. </li>
   <li> The AES encryption scheme we model takes a 128-bit plaintext,
   128-bit key and applies the following operations:
   <ol>
    <li> Key schedule which takes the key and generates three 128-bit round 
    keys. </li>
    <li> Addition of first round key (input key) to plaintext. </li>
    <li> Application of SubBytes (Sbox to each byte) operation. </li>
    <li> Application of ShiftRows operation. </li>
    <li> Application of MixColumns operation. </li>
    <li> Addition of second round key (from key schedule). </li>
    <li> Application of SubBytes (Sbox to each byte) operation. </li>
    <li> Application of ShiftRows operation. </li>
    <li> Application of MixColumns operation. </li>
    <li> Addition of third round key (from key schedule), resulting in the 
    ciphertext. </li>
   </ol>
   </li>
   <li> Note we have the following number of full rounds, special rounds,
   sboxes in the rounds, multiplications by each field element, Sboxes in
   the key expansion, additions in the key expansion and constants in the
   key expansion:
   \verbatim
> component_statistics_ss(2,16,1,8,false,aes_mc_bidirectional);
[2,0,32,896,[[1,64]],2,256,16]
> component_statistics_ss(2,16,1,8,false,aes_mc_forward);
[2,0,32,640,[[1,32]],2,256,16]
   \endverbatim
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating AES for 2 + 1/3 round:
   \verbatim
num_rounds : 2$
num_columns : 16$
num_rows : 1$
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
   <li> The computed statistics:
   \verbatim
maxima> ncl_list_ss(2,16,1,8,false,aes_ts_box,aes_mc_bidirectional);
[[1,16],[2,140288],[3,2496],[4,128],[17,8704],[256,34]]
   \endverbatim
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
conflict literals     : 7255186        (47.08 % deleted)
Memory used           : 77.00 MB
CPU time              : 39.73 s
   \endverbatim
   </li>
  </ul>

*/
