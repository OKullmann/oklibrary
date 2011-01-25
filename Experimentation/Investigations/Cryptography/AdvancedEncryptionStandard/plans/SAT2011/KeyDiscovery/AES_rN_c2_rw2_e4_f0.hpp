// Matthew Gwynne, 23.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/AES_rN_c2_rw2_e4_f0.hpp
  \brief Investigations into small scale AES key discovery for N round AES with a 2x2 block and 4-bit field elements and including MixColumns (0 < N <= 10)

  \todo Using the canonical translation
  <ul>
   <li> Generating small scale AES for 1-10 rounds (with MixColumns):
   \verbatim
maxima> num_columns : 2$
maxima> num_rows : 2$
maxima> exp : 4$
maxima> final_round_b : false$
maxima> box_tran : aes_ts_box$
maxima> seed : 1$
maxima> mc_tran : aes_mc_bidirectional$
maxima> for i : 1 thru 10 do output_ss_fcl_std(i, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$
   \endverbatim
   and then we can generate random assignments with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> for i : 1 thru 10 do output_ss_random_pc_pair(seed,i,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignments with the translations:
   \verbatim
shell> for i in `seq 1 10`; do $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/merge_cnf.sh ssaes_r${i}_c2_rw2_e4_f0.cnf ssaes_pkpair_r${i}_c2_rw2_e4_f0_s1.cnf > r${i}_keyfind.cnf; done
   \endverbatim
   </li>
  </ul>
  

  \todo One round using the canonical translation
  <ul>
   <li> OKsolver solves this with very few decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r1_keyfind.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           540
c initial_number_of_clauses             3562
c initial_number_of_literal_occurrences 10228
c number_of_initial_unit-eliminations   36
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           36
c reddiff_number_of_clauses             116
c reddiff_number_of_literal_occurrences 372
c number_of_2-clauses_after_reduction   2880
c running_time(sec)                     0.0
c number_of_nodes                       5
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                37
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        4
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             r1_keyfind.cnf
   \endverbatim
   </li>
   <li> However, minisat2 and glucose need to branch a lot more:
   \verbatim
shell> minisat2 r1_keyfind.cnf 
<snip>
restarts              : 3
conflicts             : 333            (4757 /sec)
decisions             : 469            (1.28 % random) (6700 /sec)
propagations          : 36914          (527343 /sec)
conflict literals     : 8156           (21.96 % deleted)
Memory used           : 15.19 MB
CPU time              : 0.07 s
shell> minisat-2.2.0 r1_keyfind.cnf
restarts              : 3
conflicts             : 220            (22000 /sec)
decisions             : 366            (0.00 % random) (36600 /sec)
propagations          : 17092          (1709200 /sec)
conflict literals     : 3314           (24.44 % deleted)
Memory used           : 19.00 MB
CPU time              : 0.01 s
shell> glucose r1_keyfind.cnf 
<snip>
c restarts              : 2
c nb ReduceDB           : 0
c nb learnts DL2        : 29
c nb learnts size 2     : 2
c nb learnts size 1     : 0
c conflicts             : 809            (80900 /sec)
c decisions             : 1335           (1.57 % random) (133500 /sec)
c propagations          : 74563          (7456300 /sec)
c conflict literals     : 18931          (29.29 % deleted)
c Memory used           : 2.14 MB
c CPU time              : 0.01 s
   \endverbatim
   </li>
   <li> Again the OKsolver has far fewer decision nodes, but it's not
   clear how useful this is. </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r1_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 1 2 2 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>


  \todo Two rounds using the canonical translation
  <ul>
   <li> OKsolver solves this with very few decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r2_keyfind.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           1032
c initial_number_of_clauses             7028
c initial_number_of_literal_occurrences 20232
c number_of_initial_unit-eliminations   40
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           40
c reddiff_number_of_clauses             136
c reddiff_number_of_literal_occurrences 456
c number_of_2-clauses_after_reduction   5696
c running_time(sec)                     0.2
c number_of_nodes                       19
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                105
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        8
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             r2_keyfind.cnf
   \endverbatim
   </li>
   <li> However, minisat2 and glucose need to branch a lot more:
   \verbatim
shell> minisat2 r2_keyfind.cnf 
<snip>
restarts              : 9
conflicts             : 6420           (3309 /sec)
decisions             : 8436           (1.49 % random) (4348 /sec)
propagations          : 1168590        (602366 /sec)
conflict literals     : 180261         (41.05 % deleted)
Memory used           : 16.23 MB
CPU time              : 1.94 s
shell> minisat-2.2.0 r2_keyfind.cnf
<snip>
restarts              : 10
conflicts             : 1478           (29560 /sec)
decisions             : 2127           (0.00 % random) (42540 /sec)
propagations          : 311508         (6230160 /sec)
conflict literals     : 53001          (26.51 % deleted)
Memory used           : 19.00 MB
CPU time              : 0.05 s
shell> glucose r2_keyfind.cnf 
<snip>
c restarts              : 3
c nb ReduceDB           : 0
c nb learnts DL2        : 41
c nb learnts size 2     : 2
c nb learnts size 1     : 0
c conflicts             : 3535           (22094 /sec)
c decisions             : 5241           (1.45 % random) (32756 /sec)
c propagations          : 635324         (3970775 /sec)
c conflict literals     : 89260          (32.25 % deleted)
c Memory used           : 2.92 MB
c CPU time              : 0.16 s
   \endverbatim
   </li>
   <li> Again the OKsolver has far fewer decision nodes, but it's not
   clear how useful this is. </li>
   <li> Also note that minisat-2.2.0 does a lot better here than
   minisat2. </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r2_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 2 2 2 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>


  \todo Four rounds using the canonical translation
  <ul>
   <li> OKsolver solves this with very few decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r1_keyfind.cnf
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
