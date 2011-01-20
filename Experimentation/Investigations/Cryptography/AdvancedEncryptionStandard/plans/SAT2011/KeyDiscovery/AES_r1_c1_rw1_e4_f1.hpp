// Matthew Gwynne, 17.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/AES_r1_c1_rw1_e4_f1.hpp
  \brief Investigations into AES key discovery

  \todo Using the canonical translation and bidirectional MixColumns
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
shell> OKsolver_2002-O3-DNDEBUG -D30 -M ssaes_r1_c4_rw4_e8_f1_keyfind.cnf
   \endverbatim
   takes just under an hour.
   </li>
   <li> Using precosat236:
   \verbatim
shell> precosat236 ssaes_r1_c4_rw4_e8_f1_keyfind.cnf
   \endverbatim
   returns the correct result in 5.2 seconds.
   </li>
   <li> This seems to take a very long time, especially given
   we have no MixColumns included! </li>
  </ul>

*/
