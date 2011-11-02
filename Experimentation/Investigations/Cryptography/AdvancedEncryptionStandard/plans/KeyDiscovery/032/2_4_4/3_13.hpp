// Matthew Gwynne, 13.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/032/2_4_4/3_13.hpp
  \brief Investigations into small-scale AES key discovery for 3+1/3 round AES with a 2x4 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> We investigate the 3 + 1/3 round small-scale AES with 2 row,
   4 column, using the 4-bit field size. </li>
   <li> We denote this AES instance by aes(3,2,4,4). </li>
   <li> aes(3,2,4,4) takes a 32-bit plaintext and 32-bit key and
   outputs a 32-bit ciphertext. </li>
   <li> For the full specification of this AES instance, see
   "Problem specification" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/032/2_4_4/general.hpp.
   </li>
   <li> Note that we consider the canonical CNF translation, as
   this is an example of the "hardest" representation without
   new variables. See "Hardness of boolean function representations"
   in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translation of aes(3,2,4,4):
    <ul>
     <li> The MixColumns operation is decomposed into its field
     multiplications (02 and 03) and addition operations. </li>
     <li> The MixColumns operation is translated by translating both
     the MixColumns operation and its inverse (it is self-inverse). </li>
     <li> We treat S-boxes, field multiplications and additions as boxes.
     </li>
     <li> The S-box and field multiplications are considered as a 8x1
     boolean functions, translated using the canonical translation;
     see dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
     </li>
     <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
     boolean functions; translated using their prime implicates. </li>
    </ul>
   </li>
   <li> Generating small-scale AES for 1 + 1/3 round:
   \verbatim
num_rounds : 3$
num_rows : 2$
num_columns : 4$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r3_c4_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
2916 19994 57660 0 57660 2917 1
 length count
1 12
2 16128
3 1616
4 96
9 2016
16 126
   \endverbatim
   </li>
 <li> In this translation, we have:
    <ul>
     <li> 3 full rounds (Key Addition, SubBytes, and MixColumns operation).
     </li>
     <li> 30 Sboxes:
      <ul>
       <li> 24 from SubBytes = 8 byte * 3 rounds; </li>
       <li> 6 from key schedule = 2 row * 1 word * 3 rounds. </li>
      </ul>
     </li>
     <li> 48 multiplications by 02: 2 rows * 1 multiplication * 4 columns *
     3 rounds * 2 directions (forward + inverse). </li>
     <li> 48 multiplications by 03: 2 rows * 1 multiplication * 4 columns *
     3 rounds * 2 directions (forward + inverse). </li>
     <li> 416 additions:
      <ul>
       <li> 404 additions of arity 2:
        <ul>
         <li> 96 from key additions = 32 bits * 3 rounds; </li>
         <li> 32 from final key addition = 32 bits; </li>
         <li> 84 from the key schedule = (32 bits - 4 bits) * 3 rounds. </li>
         <li> 96 from forward MixColumns = 2 rows * 4 column * 4 bits *
         3 rounds; </li>
         <li> 96 from inverse MixColumns = 2 rows * 4 column * 4 bits * 3
         rounds. </li>
        </ul>
       </li>
       <li> 12 additions of arity 3:
        <ul>
         <li> 12 from the key schedule = 4 bits * 3 rounds. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 12 bits for the constant in the key schedule = 4 bits * 3 rounds.
     </li>
    </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   </li>
   <li> This instance has 126 boxes = 30 S-boxes + 96 multiplications.
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 12 = key schedule constant * 1; </li>
     <li> 2 : 16128 = 42 boxes * 128; </li>
     <li> 3 : 1616 = 404 additions (arity 2) * 4; </li>
     <li> 4 : 96 = 12 additions (arity 3) * 8; </li>
     <li> 9 : 2016 = 126 boxes * 16; </li>
     <li> 16 : 126 = 126 boxes * 1. </li>
    </ul>
   </li>
   <li> Then we can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw2_e4_f0.cnf ssaes_pkpair_r1_c4_rw2_e4_f0_s1.cnf > r1_keyfind.cnf
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves it in 65s:
   \verbatim
shell> minisat-2.2.0 experiment_r3_k1.cnf
restarts              : 36632
conflicts             : 26578774       (406788 /sec)
decisions             : 28721930       (0.00 % random) (439590 /sec)
propagations          : 12638199080    (193427808 /sec)
conflict literals     : 690572739      (62.54 % deleted)
Memory used           : 63.00 MB
CPU time              : 65.3381 s
   \endverbatim
   </li>
   <li> cryptominisat solves it in 61.4s:
   \verbatim
shell> cryptominisat experiment_r3_k1.cnf
<snip>
c restarts                 : 105
c conflicts                : 5370243     (87472.26  / sec)
c decisions                : 5741789     (0.26      % random)
c CPU time                 : 61.39       s
   \endverbatim
   </li>
   <li> glucose solves it in 65s:
   \verbatim
shell> glucose experiment_r3_k1.cnf
<snip>
c restarts              : 50
c nb ReduceDB           : 164
c conflicts             : 14316868       (219560 /sec)
c decisions             : 15063464       (1.57 % random) (231010 /sec)
c propagations          : 4612945014     (70743001 /sec)
c CPU time              : 65.2071 s
   \endverbatim
   </li>
   <li> precosat236 solves it in 1246s:
   \verbatim
shell> precosat236 experiment_r3_k1.cnf
<snip>
c 5597931 conflicts, 6207027 decisions, 1 random
c 0 iterations, 58 restarts, 8769 skipped
c 47 simplifications, 2 reductions
c prps: 1627135948 propagations, 1.31 megaprops
c 1246.0 seconds, 22 MB max, 635 MB recycled
   \endverbatim
   </li>
  </ul>

*/
