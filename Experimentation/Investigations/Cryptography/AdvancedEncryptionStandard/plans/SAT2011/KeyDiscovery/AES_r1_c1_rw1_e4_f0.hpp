// Matthew Gwynne, 23.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/AES_r1_c1_rw1_e4_f0.hpp
  \brief Investigations into simplest small scale AES key discovery for one round AES with MixColumns

  \todo Using the canonical translation
  <ul>
   <li> Generating simplest small scale AES for 1 round (with MixColumns):
   \verbatim
maxima> num_rounds : 1$
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
shell> $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/merge_cnf.sh ssaes_r1_c1_rw1_e4_f0.cnf ssaes_pkpair_r1_c1_rw1_e4_f0_s1.cnf > r1_keyfind.cnf
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
   <li> However, minisat2 and glucose need to branch:
   \verbatim
shell> minisat2 r1_keyfind.cnf 
<snip>
restarts              : 1
conflicts             : 13             (inf /sec)
decisions             : 21             (0.00 % random) (inf /sec)
propagations          : 286            (inf /sec)
conflict literals     : 115            (5.74 % deleted)
Memory used           : 14.64 MB
CPU time              : 0 s

SATISFIABLE
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
