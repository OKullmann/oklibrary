// Matthew Gwynne, 23.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/AES_r10_c1_rw1_e4_f0.hpp
  \brief Investigations into simplest small scale AES key discovery for ten rounds AES with MixColumns

  \todo Using the canonical translation
  <ul>
   <li> Generating simplest small scale AES for 10 round (with MixColumns):
   \verbatim
maxima> num_rounds : 10$
maxima> num_columns : 1$
maxima> num_rows : 1$
maxima> exp : 4$
maxima> final_round_b : false$
maxima> box_tran : aes_ts_box$
maxima> seed : 1$
maxima> mc_tran : aes_mc_bidirectional$
maxima> output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$
   \endverbatim
   and then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/merge_cnf.sh ssaes_r10_c1_rw1_e4_f0.cnf ssaes_pkpair_r10_c1_rw1_e4_f0_s1.cnf > r10_keyfind.cnf
   \endverbatim
   </li>
   <li> OKsolver solves this with no decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r10_keyfind.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           572
c initial_number_of_clauses             3444
c initial_number_of_literal_occurrences 9696
c number_of_initial_unit-eliminations   48
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           48
c reddiff_number_of_clauses             144
c reddiff_number_of_literal_occurrences 432
c number_of_2-clauses_after_reduction   2816
c running_time(sec)                     0.0
c number_of_nodes                       1
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                148
c number_of_pure_literals               0
c number_of_autarkies                   1
c number_of_missed_single_nodes         0
c max_tree_depth                        0
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             r10_keyfind.cnf
   \endverbatim
   </li>
   <li> Again the OKsolver finds the result with no decisions! </li>
   <li> However, minisat2 and glucose need to branch:
   \verbatim
shell> minisat2 r10_keyfind.cnf 
restarts              : 1
conflicts             : 29             (1450 /sec)
decisions             : 67             (0.00 % random) (3350 /sec)
propagations          : 5397           (269850 /sec)
conflict literals     : 364            (37.67 % deleted)
Memory used           : 15.18 MB
CPU time              : 0.02 s
shell> glucose r10_keyfind.cnf 
c restarts              : 1
c nb ReduceDB           : 0
c nb learnts DL2        : 15
c nb learnts size 2     : 8
c nb learnts size 1     : 1
c conflicts             : 48             (inf /sec)
c decisions             : 168            (2.38 % random) (inf /sec)
c propagations          : 5738           (inf /sec)
c conflict literals     : 714            (15.40 % deleted)
c Memory used           : 2.00 MB
c CPU time              : 0 s
   \endverbatim
   </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r10_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 10 1 1 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>

*/
