// Matthew Gwynne, 23.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/AES_r1_c2_rw4_e4_f0.hpp
  \brief Investigations into small scale AES key discovery for one round AES with a 2x4 block and 4-bit field elements, including MixColumns


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   one round small scale AES with two columns, four rows, using the 4-bit
   field size, and including the MixColumns operation. </li>
   <li> The AES encryption scheme we model takes a 32-bit plaintext,
   32-bit key and applies the following operations:
   <ol>
    <li> Key schedule which takes the key and generates two 32-bit round 
    keys. </li>
    <li> Addition of first round key (input key) to plaintext. </li>
    <li> Application of SubBytes (Sbox to each byte) operation. </li>
    <li> Application of ShiftRows operation. </li>
    <li> Application of MixColumns operation. </li>
    <li> Addition of second round key (from key schedule), resulting in the 
    ciphertext. </li>
   </ol>
   </li>
   <li> Note we have the following number of full rounds, special rounds,
   sboxes in the rounds, multiplications by each field element, sboxes in
   the key expansion, additions in the key expansion and constants in the
   key expansion:
   \verbatim
> component_statistics_ss(1,2,4,4,false,aes_mc_bidirectional);
[1,0,8,128,[[1,16],[x,8],[x+1,8],[x^3+1,8],[x^3+x+1,8],[x^3+x^2+1,8],[x^3+x^2+x,8]],4,32,4]
> component_statistics_ss(1,2,4,4,false,aes_mc_forward);
[1,0,8,96,[[1,16],[x,8],[x+1,8]],4,32,4]
   \endverbatim
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating small scale AES for 1 round (with MixColumns):
   \verbatim
num_rounds : 1$
num_columns : 2$
num_rows : 4$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c2_rw4_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1396 10128 31316 0 31316 1397 1
 length count
1 4
2 7680
3 368
4 32
5 1024
9 960
16 60
   \endverbatim
   </li>
   <li> The measured statistics match up to the computed statistics:
   \verbatim
maxima> ncl_list_ss(1,2,4,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,4],[2,7680],[3,368],[4,32],[5,1024],[9,960],[16,60]]
   \endverbatim
   </li>
   <li> Then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c2_rw4_e4_f0.cnf ssaes_pkpair_r1_c2_rw4_e4_f0_s1.cnf > r1_keyfind.cnf
   \endverbatim
   </li>
   <li> OKsolver solves this without backtracking:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r1_keyfind.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           1396
c initial_number_of_clauses             10192
c initial_number_of_literal_occurrences 31380
c number_of_initial_unit-eliminations   68
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           68
c reddiff_number_of_clauses             212
c reddiff_number_of_literal_occurrences 660
c number_of_2-clauses_after_reduction   7808
c running_time(sec)                     42.8
c number_of_nodes                       2915
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                25478
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        12
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             r1_keyfind.cnf
   \endverbatim
   </li>
   <li> minisat and glucose:
   \verbatim
shell> minisat2 r1_keyfind.cnf 
<snip>
restarts              : 14
conflicts             : 43604          (1401 /sec)
decisions             : 47364          (1.34 % random) (1522 /sec)
propagations          : 17037471       (547477 /sec)
conflict literals     : 1360321        (61.63 % deleted)
Memory used           : 18.44 MB
CPU time              : 31.12 s
shell> glucose r1_keyfind.cnf 
<snip>
c restarts              : 8
c nb ReduceDB           : 2
c nb learnts DL2        : 232
c nb learnts size 2     : 44
c nb learnts size 1     : 0
c conflicts             : 16554          (13035 /sec)
c decisions             : 21834          (1.56 % random) (17192 /sec)
c propagations          : 3407020        (2682693 /sec)
c conflict literals     : 705068         (42.14 % deleted)
c Memory used           : 4.88 MB
c CPU time              : 1.27 s
   \endverbatim
   </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r1_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 1 2 4 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>

*/
