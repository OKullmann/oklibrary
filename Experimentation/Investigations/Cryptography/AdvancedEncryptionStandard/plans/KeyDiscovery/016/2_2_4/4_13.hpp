// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/016/2_2_4/4_13.hpp
  \brief Investigations into small-scale AES key discovery for 4 + 1/3 round AES with a 2x2 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> We investigate the 4 + 1/3 round small-scale AES with 2 row,
   2 column, using the 4-bit field size. </li>
   <li> We denote this AES instance by aes(4,2,2,4). </li>
   <li> aes(4,2,2,4) takes a 16-bit plaintext and 16-bit key and
   outputs a 16-bit ciphertext. </li>
   <li> For the full specification of this AES instance, see
   "Problem specification" in
   Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/016/2_2_4/general.hpp.
   </li>
   <li> Note that we consider the canonical CNF translation, as
   this is an example of the "hardest" representation without
   new variables. See "Hardness of boolean function representations"
   in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translation of aes(4,2,2,4):
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
   <li> Generating small-scale AES for 4 + 1/3 rounds:
   \verbatim
rounds : 4$
num_rows : 2$
num_columns : 2$
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
   <li> In this translation, we have:
    <ul>
     <li> 4 full rounds (Key Addition, SubBytes, and MixColumns operation).
     </li>
     <li> 24 Sboxes:
      <ul>
       <li> 16 from SubBytes = 4 byte * 4 rounds; </li>
       <li> 8 from key schedule = 2 row * 1 word * 4 rounds. </li>
      </ul>
     </li>
     <li> 32 multiplications by 02: 2 rows * 1 multiplication * 2 columns
     * 4 rounds * 2 directions (forward + inverse). </li>
     <li> 32 multiplications by 03: 2 rows * 1 multiplication * 2 columns
     * 4 rounds * 2 directions (forward + inverse). </li>
     <li> 144 additions:
      <ul>
       <li> 256 additions of arity 2:
        <ul>
         <li> 64 from key additions = 16 bits * 4 rounds; </li>
         <li> 16 from final key addition = 16 bits; </li>
         <li> 48 from the key schedule = (16 bits - 4 bits) * 4 rounds. </li>
         <li> 64 from forward MixColumns = 2 rows * 2 column * 4 bits *
         4 rounds; </li>
         <li> 64 from inverse MixColumns = 2 rows * 2 column * 4 bits * 4
         round. </li>
        </ul>
        <li> 16 additions of arity 3:
         <ul>
          <li> 16 from the key schedule = 4 bits * 4 round. </li>
         </ul>
        </li>
       </li>
      </ul>
     </li>
     <li> 16 bits for the constant in the key schedule = 4 bits * 4 rounds.
     </li>
    </ul>
   </li>
   <li> The number of clauses of each length in the canonical translation:
   \verbatim
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   </li>
   <li> This instances has 88 boxes = 24 S-boxes + 64 multiplications.
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 16 = key schedule constant * 1; </li>
     <li> 2 : 11264 = 88 boxes * 128; </li>
     <li> 3 : 1024 =  256 additions (arity 2) * 4; </li>
     <li> 4 : 128 = 16 additions (arity 3) * 8; </li>
     <li> 9 : 1408 = 88 boxes * 16; </li>
     <li> 16 : 88 = 88 boxes * 1. </li>
    </ul>
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
