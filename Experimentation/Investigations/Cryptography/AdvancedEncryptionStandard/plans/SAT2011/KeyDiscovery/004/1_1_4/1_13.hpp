// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/004/1_1_4/1_13.hpp
  \brief Investigations into simplest small scale AES key discovery for one round AES (1+1/3)


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   one round small scale AES with one column, one row, using the 4-bit
   field size, and including the MixColumns operation. </li>
   <li> The AES encryption scheme we model takes a 4-bit plaintext,
   4-bit key and applies the following operations:
   <ol>
    <li> Key schedule which takes the key and generates two 4-bit round 
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
> component_statistics_ss(1,1,1,4,false,aes_mc_bidirectional);
[1,0,1,16,[[1,2]],1,4,4]
> component_statistics_ss(1,1,1,4,false,aes_mc_forward);
[1,0,1,12,[[1,1]],1,4,4]
   \endverbatim
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating simplest small scale AES for 1 round:
   \verbatim
num_rounds : 1$
num_columns : 1$
num_rows : 1$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c1_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
68 358 1012 0 1012 69 1
 length count
1 4
2 272
3 48
9 32
16 2
   \endverbatim
   </li>
   <li> The measured statistics match up to the computed statistics:
   \verbatim
maxima> ncl_list_ss(1,1,1,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,4],[2,272],[3,48],[9,32],[16,2]]
   \endverbatim
   </li>
   <li> Then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c1_rw1_e4_f0.cnf ssaes_pkpair_r1_c1_rw1_e4_f0_s1.cnf > r1_keyfind.cnf
   \endverbatim
   </li>
   <li> OKsolver solves this with no decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r1_keyfind.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           68
c initial_number_of_clauses             366
c initial_number_of_literal_occurrences 1020
c number_of_initial_unit-eliminations   12
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           12
c reddiff_number_of_clauses             36
c reddiff_number_of_literal_occurrences 108
c number_of_2-clauses_after_reduction   296
c running_time(sec)                     0.0
c number_of_nodes                       1
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                19
c number_of_pure_literals               0
c number_of_autarkies                   1
c number_of_missed_single_nodes         0
c max_tree_depth                        0
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             r1_keyfind.cnf
   \endverbatim
   </li>
   <li> However, minisat-2.2.0 and glucose need to branch:
   \verbatim
shell> minisat-2.2.0 r1_keyfind.cnf
<snip>
restarts              : 1
conflicts             : 7              (inf /sec)
decisions             : 14             (0.00 % random) (inf /sec)
propagations          : 198            (inf /sec)
conflict literals     : 83             (0.00 % deleted)
Memory used           : 18.00 MB
CPU time              : 0 s

shell> minisat2 r1_keyfind.cnf 
<snip>
restarts              : 1
conflicts             : 13             (inf /sec)
decisions             : 21             (0.00 % random) (inf /sec)
propagations          : 286            (inf /sec)
conflict literals     : 115            (5.74 % deleted)
Memory used           : 14.64 MB
CPU time              : 0 s

shell> glucose r1_keyfind.cnf 
<snip>
c restarts              : 1
c nb ReduceDB           : 0
c nb learnts DL2        : 5
c nb learnts size 2     : 0
c nb learnts size 1     : 2
c conflicts             : 12             (inf /sec)
c decisions             : 32             (0.00 % random) (inf /sec)
c propagations          : 215            (inf /sec)
c conflict literals     : 146            (11.52 % deleted)
c Memory used           : 1.88 MB
c CPU time              : 0 s
   \endverbatim
   </li>
   <li> It seems OKsolver is propagating purely by r_2. Perhaps the OKsolver
   is able to take advantage of the r_1-basedness of the canonical translation
   due to it's use of r_2 reductions? </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r1_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 1 1 1 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>

*/
