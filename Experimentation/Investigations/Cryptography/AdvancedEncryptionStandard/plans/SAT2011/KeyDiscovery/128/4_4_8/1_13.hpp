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
   1+1/3 round AES. </li>
   <li> The AES encryption scheme we model takes a 128-bit plaintext,
   128-bit key and applies the following operations:
   <ol>
    <li> Addition of round key 0 (input key) to plaintext. </li>
    <li> Application of SubBytes (Sbox to each byte) operation. </li>
    <li> Application of linear diffusion operation. </li>
    <li> Addition of round key 1, resulting in the ciphertext. </li>
   </ol>
   </li>
   <li> The linear diffusion operation applies a shift of row i by i-1 
   bytes to the left and then applies the AES MixColumns operation
   (a matrix multiplication at the byte level). </li>
   <li> Note we have the following number of full rounds, special rounds,
   sboxes in the rounds, multiplications by each field element, Sboxes in
   the key expansion, additions in the key expansion and constants in the
   key expansion:
   \verbatim
> component_statistics_ss(1,4,4,8,false,aes_mc_bidirectional);
[1,0,16,512,[[1,32],[x,16],[x+1,16],[x^3+1,16],[x^3+x+1,16],[x^3+x^2+1,16],[x^3+x^2+x,16]],4,128,8]
> component_statistics_ss(1,4,4,8,false,aes_mc_forward);
[1,0,16,384,[[1,32],[x,16],[x+1,16]],4,128,8]
   \endverbatim
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating AES for 1 + 1/3 round:
   \verbatim
num_rounds : 1$
num_columns : 4$
num_rows : 4$
exp : 8$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

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
   <li> The measured statistics match up to the computed statistics:
   \verbatim
maxima> ncl_list_ss(1,4,4,8,false,aes_ts_box,aes_mc_bidirectional);
[[1,8],[2,475136],[3,1504],[4,64],[5,4096],[17,29696],[256,116]]
   \endverbatim
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
