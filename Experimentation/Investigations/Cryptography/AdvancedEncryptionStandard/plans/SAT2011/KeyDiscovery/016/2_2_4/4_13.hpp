// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_2_4/4_13.hpp
  \brief Investigations into small scale AES key discovery for one round AES with a 2x2 block and 4-bit field elements (4+1/3)


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   4 + 1/3 round small scale AES with two columns and two rows, including the 
   MixColumns operation (no special final round). </li>
   <li> The AES encryption scheme we model takes a 16-bit plaintext,
   16-bit key and applies the following operations:
   <ol>
    <li> Key schedule which takes the key and generates 5 16-bit round 
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
> component_statistics_ss(4,2,2,4,false,aes_mc_bidirectional);
[4,0,16,208,[[x,32],[x+1,32]],8,64,16] 
> component_statistics_ss(4,2,2,4,false,aes_mc_forward);
[4,0,16,144,[[x,16],[x+1,16]],8,64,16] 
   \endverbatim
   Note that the inverse MixColumns uses the same multiplication
   constants as the MixColumns.
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating small scale AES for four rounds (with MixColumns):
   \verbatim
rounds : 4$
num_columns : 2$
num_rows : 2$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r4_c2_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
2016 13928 40208 0 40208 2017 1
 length count
1 16
2 11264
3 1024
4 128
9 1408
16 88
   \endverbatim
   </li>
   <li> We have the following statistics (computed):
   \verbatim
maxima> for r : 1 thru 10 do print(ncl_list_ss(r,2,2,4,false,aes_ts_box,aes_mc_bidirectional));
[[1,16],[2,11264],[3,1024],[4,128],[9,1408],[16,88]] 
   \endverbatim
   </li>
   <li> Then we can generate random assignments with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignments with the translations:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r4_c2_rw2_e4_f0.cnf ssaes_pkpair_r4_c2_rw2_e4_f0_s1.cnf > r4_keyfind.cnf
   \endverbatim
   </li>
   <li> OKsolver solves this with very few decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r4_keyfind.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           2016
c initial_number_of_clauses             13960
c initial_number_of_literal_occurrences 40240
c number_of_initial_unit-eliminations   48
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           48
c reddiff_number_of_clauses             176
c reddiff_number_of_literal_occurrences 624
c number_of_2-clauses_after_reduction   11328
c running_time(sec)                     20.9
c number_of_nodes                       1246
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                44291
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        17
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             r4_keyfind.cnf
   \endverbatim
   </li>
   <li> However, minisat2 and glucose need to branch a lot more:
   \verbatim
shell> minisat2 r4_keyfind.cnf 
<snip>
Verified 13576 original clauses.
Verified 976 eliminated clauses.   
restarts              : 12
conflicts             : 18318          (1104 /sec)
decisions             : 20892          (1.78 % random) (1259 /sec)
propagations          : 14693302       (885672 /sec)
conflict literals     : 514079         (63.22 % deleted)
Memory used           : 18.46 MB   
CPU time              : 16.59 s
shell> minisat-2.2.0 r4_keyfind.cnf
<snip>
restarts              : 127
conflicts             : 40470          (7125 /sec)
decisions             : 45214          (0.00 % random) (7960 /sec)
propagations          : 36303086       (6391388 /sec)
conflict literals     : 1250043        (71.04 % deleted)
Memory used           : 20.00 MB
CPU time              : 5.68 s
shell> glucose r4_keyfind.cnf 
<snip>
c restarts              : 2
c nb ReduceDB           : 1
c nb learnts DL2        : 55
c nb learnts size 2     : 1
c nb learnts size 1     : 0
c conflicts             : 6731           (8742 /sec)
c decisions             : 7756           (1.77 % random) (10073 /sec)
c propagations          : 2519893        (3272588 /sec)
c conflict literals     : 286835         (44.78 % deleted)
c Memory used           : 3.98 MB  
c CPU time              : 0.77 s   
   \endverbatim
   </li>
   <li> The number of decision nodes used by the OKsolver has jumped
   drastically here! What has changed? </li>
   <li> Also note that minisat-2.2.0 does a lot better here than
   minisat2. </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r4_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 4 2 2 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>

*/
