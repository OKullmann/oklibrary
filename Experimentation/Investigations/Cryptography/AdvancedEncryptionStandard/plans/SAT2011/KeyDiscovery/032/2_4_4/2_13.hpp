// Matthew Gwynne, 18.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/032/2_4_4/2_13.hpp
  \brief Investigations into small scale AES key discovery for 2+1/3 round AES with a 2x4 block and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   2 + 1/3 round small scale AES with two columns, four rows,
   using the 4-bit field size. </li>
   <li> The AES encryption scheme we model takes a 32-bit plaintext,
   32-bit key and applies the following operations:
   <ol>
    <li> Key schedule which takes the key and generates three 32-bit round 
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
   sboxes in the rounds, multiplications by each field element, sboxes in
   the key expansion, additions in the key expansion and constants in the
   key expansion:
   \verbatim
> component_statistics_ss(2,2,4,4,false,aes_mc_bidirectional);
[2,0,16,224,[[1,32],[x,16],[x+1,16],[x^3+1,16],[x^3+x+1,16],[x^3+x^2+1,16],[x^3+x^2+x,16]],8,64,8]
> component_statistics_ss(2,2,4,4,false,aes_mc_forward);
[2,0,16,160,[[1,32],[x,16],[x+1,16]],8,64,8]
   \endverbatim
   </li>
  </ul>


  \todo Using the canonical translation ??? INAPPROPRIATE: what type of translation ???
  <ul>
   <li> Generating small scale AES for 2 + 1/3 round:
   \verbatim
num_rounds : 2$
num_columns : 2$
num_rows : 4$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r2_c2_rw4_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
2696 20128 62248 0 62248 2697 1
 length count
1 8
2 15360
3 608
4 64
5 2048
9 1920
16 120
   \endverbatim
   </li>
   <li> The computed statistics for the clause sizes:
   \verbatim
maxima> ncl_list_ss(2,2,4,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,8],[2,15360],[3,608],[4,64],[5,2048],[9,1920],[16,120]]
   \endverbatim
   </li>
   <li> Then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r2_c2_rw4_e4_f0.cnf ssaes_pkpair_r2_c2_rw4_e4_f0_s1.cnf > r2_keyfind.cnf
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves it in just over 45 minutes:
   \verbatim
shell> minisat-2.2.0 r2_keyfind.cnf
<snip>
restarts              : 32761
conflicts             : 21348407       (7855 /sec)
decisions             : 23781237       (0.00 % random) (8751 /sec)
propagations          : 9574903606     (3523177 /sec)
conflict literals     : 731565401      (54.72 % deleted)
Memory used           : 77.00 MB
CPU time              : 2717.69 s
   \endverbatim
   </li>
  </ul>

*/
