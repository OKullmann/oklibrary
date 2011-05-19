// Matthew Gwynne, 4.8.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/EncryptionDecryption.hpp
  \brief On investigating encryption and decryption using AES SAT-translation


  \todo Overview
  <ul>
   <li> Question here is: Given the current translation, how well can a SAT
   solver perform a simple AES computation (encryption/decryption)? </li>
   <li> It should be that through unit-clause propagation etc, the SAT solver
   should be very quick, as there should be no decisions, just simple
   computation. </li>
   <li> See "Encryption" and "Decryption". </li>
  </ul>


  \todo Encryption
  <ul>
   <li> Generating AES for 1 round:
   \verbatim
num_rounds : 1$
num_columns : 4$
num_rows : 4$
exp : 8$
final_round_b : true$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$
   \endverbatim
   and then we can generate a random assignment with the plaintext and key,
   leaving the ciphertext unknown:
   \verbatim
output_ss_random_pk_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and then we can merge the random plaintext, key assignment (as
   a clause-set containing the relevant unit-clauses) with:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw4_e8_f1.cnf ssaes_pkpair_r1_c4_rw4_e8_f1_s1.cnf > ssaes_r1_c4_rw4_e8_f1_encrypt.cnf
   \endverbatim
   </li>
   <li> Testing encryption using minisat:
   \verbatim
shell> minisat ssaes_r1_c4_rw4_e8_f1_encrypt.cnf 
==================================[MINISAT]===================================
| Conflicts |     ORIGINAL     |              LEARNT              | Progress |
|           | Clauses Literals |   Limit Clauses Literals  Lit/Cl |          |
==============================================================================
|         0 |   21044   170112 |    7014       0        0    -nan |  0.000 % |
==============================================================================
restarts              : 1
conflicts             : 0              (0 /sec)
decisions             : 1              (2 /sec)
propagations          : 5928           (12098 /sec)
conflict literals     : 0              (-nan % deleted)
Memory used           : 17.28 MB
CPU time              : 0.49 s

SATISFIABLE
   \endverbatim
   Notice, satisfiability is found purely through unit-clause propagation.
   </li>
  </ul>

  
  \todo Decryption
  <ul>
   <li> We generate the AES CNF translation as in "Encryption", except
   instead of generating a plaintext and key assignment, we generate a
   key and ciphertext assignment:
   \verbatim
output_ss_random_kc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and then we can merge the random plaintext, key assignment (as
   a clause-set containing the relevant unit-clauses) with:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw4_e8_f1.cnf ssaes_kcpair_r1_c4_rw4_e8_f1_s1.cnf > ssaes_r1_c4_rw4_e8_f1_decrypt.cnf
   \endverbatim
   </li>
   <li> Testing decryption using minisat:
   \verbatim
shell> minisat ssaes_r1_c4_rw4_e8_f1_decrypt.cnf 
==================================[MINISAT]===================================
| Conflicts |     ORIGINAL     |              LEARNT              | Progress |
|           | Clauses Literals |   Limit Clauses Literals  Lit/Cl |          |
==============================================================================
|         0 |   21044   170112 |    7014       0        0    -nan |  0.000 % |
==============================================================================
restarts              : 1
conflicts             : 0              (0 /sec)
decisions             : 1              (2 /sec)
propagations          : 5928           (12098 /sec)
conflict literals     : 0              (-nan % deleted)
Memory used           : 17.28 MB
CPU time              : 0.49 s

SATISFIABLE
   \endverbatim
   Notice, satisfiability is found purely through unit-clause propagation.
   </li>
  </ul>


  \todo Update instructions
  <ul>
   <li> DONE We need instructions here on how to generate the various
   CNFs which represent AES encryption and decryption with all the 
   various parameters. </li>
   <li> The instructions also need to be improved to explain more fully
   how everything works. </li>
   <li> See 
   ComputerAlgebra/Cryptography/Lisp/Cryptanalysis/Rijndael/Translations.mac. 
   </li>
  </ul>

*/
