// Matthew Gwynne, 17.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/AES_r1_c1_rw1_e4_f1.hpp
  \brief Investigations into AES key discovery for one round AES without MixColumns

  \todo Using the canonical translation (solvable)
  <ul>
   <li> Generating AES for 1 round (without MixColumns):
   \verbatim
maxima> num_rounds : 1$
maxima> num_columns : 4$
maxima> num_rows : 4$
maxima> exp : 8$
maxima> final_round_b : true$
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
shell> $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/merge_cnf.sh ssaes_r1_c4_rw4_e8_f1.cnf ssaes_pkpair_r1_c4_rw4_e8_f1_s1.cnf > ssaes_r1_c4_rw4_e8_f1_keyfind.cnf
   \endverbatim
   </li>
   <li> Using the OKsolver:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -D15 -M ssaes_r1_c4_rw4_e8_f1_keyfind.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         256
c initial_number_of_variables           5928
c initial_number_of_clauses             88892
c initial_number_of_literal_occurrences 261032
c number_of_initial_unit-eliminations   264
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           264
c reddiff_number_of_clauses             808
c reddiff_number_of_literal_occurrences 2472
c number_of_2-clauses_after_reduction   82432
c running_time(sec)                     362.9
c number_of_nodes                       646
c number_of_single_nodes                1
c number_of_quasi_single_nodes          0
c number_of_2-reductions                120548
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        14
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             ssaes_r1_c4_rw4_e8_f1_keyfind.cnf
   \endverbatim
   </li>
   <li> Using precosat236:
   \verbatim
shell> precosat236 ssaes_r1_c4_rw4_e8_f1_keyfind.cnf
   \endverbatim
   returns the correct result in 5.2 seconds.
   </li>
   <li> With "seed : 2$" and "seed : 3", these runtimes and statistics
   seem stable. </li>
   <li> This is slow, especially given we have no MixColumns included! </li>
  </ul>

*/
