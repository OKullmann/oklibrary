// Matthew Gwynne, 21.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/016/2_1_8/14_13.hpp
  \brief Investigations into small scale AES key discovery for 14 + 1/3 round AES with a 2x1 plaintext matrix and 8-bit field elements


  \todo Problem specification
  <ul>
   <li> We investigate the 14 + 1/3 round small scale AES with 2 row,
   1 column, using the 8-bit field size. </li>
   <li> We denote this AES instance by aes(14,2,1,8). </li>
   <li> aes(14,2,1,8) takes a 16-bit plaintext and 16-bit key and
   outputs a 16-bit ciphertext. </li>
   <li> For the full specification of this AES instance, see
   "Problem specification" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/016/2_1_8/general.hpp.
   </li>
   <li> Note that we consider the canonical CNF translation, as
   this is an example of the "hardest" representation without
   new variables. See "Hardness of boolean function representations"
   in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translation of aes(14,2,1,8):
    <ul>
     <li> The MixColumns operation is decomposed into its field
     multiplications (02 and 03) and addition operations. </li>
     <li> The MixColumns operation is translated by translating both
     the MixColumns operation and its inverse (it is self-inverse). </li>
     <li> We treat S-boxes, field multiplications and additions as boxes.
     </li>
     <li> The S-box and field multiplications are considered as a 16x1
     boolean functions, translated using the canonical translation;
     see dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
     </li>
     <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
     boolean functions; translated using their prime implicates. </li>
    </ul>
   </li>
   <li> Generating small scale AES for 14 + 1/3 rounds:
   \verbatim
rounds : 14$
num_rows : 2$
num_columns : 1$
exp : 8$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r14_c1_rw2_e8_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
45072 734616 2160112 0 2160112 45073 1
 length count
1 112
2 688128
3 3200
17 43008
256 168
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 14 full rounds (Key Addition, SubBytes, and MixColumns
     operation). </li>
     <li> 56 Sboxes:
      <ul>
       <li> 28 from SubBytes = 2 byte * 14 rounds; </li>
       <li> 28 from key schedule = 2 row * 1 byte * 14 rounds. </li>
      </ul>
     </li>
     <li> 56 multiplications by 02: 2 rows * 1 multiplication * 1 columns *
     14 rounds * 2 directions (forward + inverse). </li>
     <li> 56 multiplications by 03: 2 rows * 1 multiplication * 1 columns *
     14 rounds * 2 directions (forward + inverse). </li>
     <li> 800 additions of arity 2:
      <ul>
       <li> 224 from key additions = 16 bits * 14 rounds; </li>
       <li> 16 from final key addition = 16 bits; </li>
       <li> 112 from the key schedule = 1 rows * 8 bits * 14 rounds. </li>
       <li> 224 from forward MixColumns = 2 rows * 1 column * 8 bits *
       14 rounds; </li>
       <li> 224 from inverse MixColumns = 2 rows * 1 column * 8 bits * 14
       rounds. </li>
      </ul>
     </li>
     <li> 112 bits for the constant in the key schedule = 8 bits * 14 rounds.
     </li>
    </ul>
   </li>
   <li> Note that as this variant has only one column, the key schedule
   applies Sbox(K_i) + C rather than Sbox(K_i) + K_j + C where K_i and
   K_j are key words from the previous round key. </li>
   <li> The Sboxes and multiplications use the canonical translation,
   which has the following number of clauses of each length:
   \verbatim
maxima> ncl_list_full_dualts(16,256);
[[2,4096],[17,256],[256,1]]
   \endverbatim
   </li>
  <li> This instances has 168 boxes = 56 S-boxes + 112 multiplications.
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 112 = key schedule constant * 1; </li>
     <li> 2 : 688128 = 168 boxes * 4096; </li>
     <li> 3 : 320 = 448 additions (arity 2) * 4; </li>
     <li> 4 : 640 = 40 multiplications by 04 * 16; </li>
     <li> 6 : 5720 = 40 S-boxes * 143; </li>
     <li> 7 : 5080 = 40 S-boxes * 127; </li>
     <li> 8 : 960 = 40 S-boxes * 24. </li>
    </ul>
   </li>
   <li> The number of clauses of each length in the translation are:
    <ul>
     <li> 112 unit-clauses (key schedule constant). </li>
     <li> 688128 binary clauses (56 Sboxes; 56 multiplications * 2). </li>
     <li> 3200 ternary clauses (800 arity two additions). </li>
     <li> 43008 clauses of length seventeen (56 Sboxes; 56
     multiplications * 2). </li>
     <li> 168 clauses of length 256 (56 Sboxes; 56 multiplications * 2). </li>
    </ul>
   </li>
   <li> Generate random assignments for the plaintext and ciphertext, leaving
   the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   Merge the assignments with the translations:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r14_c1_rw2_e4_f0.cnf ssaes_pkpair_r14_c1_rw2_e4_f0_s1.cnf > r14_keyfind.cnf; done
   \endverbatim
   </li>
   <li> All solvers solve in < 20 minutes:
    <ul>
     <li> precosat236
     \verbatim
shell> precosat236 r14_keyfind.cnf
c 56022 conflicts, 304526 decisions, 163 random
c 0 iterations, 172 restarts, 0 skipped
c 181.9 seconds, 148 MB max, 10 MB recycled
     \endverbatim
     </li>
     <li> cryptominisat:
     \verbatim
shell> cryptominisat r14_keyfind.cnf
c restarts                 : 168
c conflicts                : 43829       (239.90    / sec)
c decisions                : 67335       (1.98      % random)
c CPU time                 : 182.70      s
     \endverbatim
     </li>
     <li> minisat-2.2.0:
     \verbatim
shell> minisat-2.2.0 r14_keyfind.cnf
restarts              : 253
conflicts             : 82374          (368 /sec)
decisions             : 324814         (0.00 % random) (1452 /sec)
propagations          : 1071399042     (4790302 /sec)
CPU time              : 223.66 s
     \endverbatim
     </li>
     <li> precosat-570.1:
     \verbatim
shell> precosat-570.1 r14_keyfind.cnf
c 90715 conflicts, 351716 decisions, 176 random
c 0 iterations, 1931 restarts, 0 skipped
c 570.7 seconds, 75 MB max, 27 MB recycled
     \endverbatim
     </li>
     <li> glucose:
     \verbatim
shell> glucose r14_keyfind.cnf
<snip>
c restarts              : 693
c conflicts             : 404866         (478 /sec)
c decisions             : 2156003        (1.81 % random) (2545 /sec)
c propagations          : 1254906184     (1481240 /sec)
c CPU time              : 847.2 s
     \endverbatim
     </li>
    </ul>
   </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r14_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 1 1 2 8 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>


  \todo Using the "minimum" box translation
  <ul>
   <li> Translation of aes(14,2,1,8):
    <ul>
     <li> The MixColumns operation is decomposed into its field
     multiplications (02 and 03) and addition operations. </li>
     <li> The MixColumns operation is translated by translating both
     the MixColumns operation and its inverse (it is self-inverse). </li>
     <li> We treat S-boxes, field multiplications and additions as boxes.
     </li>
     <li> The S-box and field multiplications are considered as a 16x1
     boolean function, translated using the "minimum" translation;
     see ss_sbox_cnfs in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/data/SmallScaleSboxCNF.mac
     and ss_field_cnfs in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/data/SmallScaleFieldMulCNF.mac.
     </li>
     <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
     boolean functions; translated using their prime implicates. </li>
    </ul>
   </li>
   <li> The CNFs for the Sbox and multiplications:
   \verbatim
/* Multiplication by 02: */
maxima> FieldMul2CNF : [{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},{{-9,2},{-2,9},{-10,3},{-3,10},{-11,4},{-4,11},{-12,-5,-1},{-12,1,5},{-5,1,12},{-1,5,12},{-13,-6,-1},{-1,6,13},{-14,7},{-7,14},{-15,1,8},{-8,1,15},{-16,-15,-8},{-16,8,15},{-13,6,16},{-6,13,16}}]$
set_hm(ss_field_cnfs,[8,2], FieldMul2CNF));
/* Multiplication by 03: */
maxima> FieldMul3CNF :
 [[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16],
  [{-9,-2,-1},{-2,1,9},{-10,2,3},{-10,-9,-3,1},{-10,-3,-1,9},{-3,2,10},{-9,1,3,10},{-1,3,9,10},{-11,-4,-3},{-11,3,4},{-4,3,11},{-3,4,11},{-12,-5,-4,1},{-12,-4,-1,5},{-5,1,4,12},{-1,4,5,12},{-13,-5,-1,6},{-13,1,5,6},{-13,-12,-6,4},{-13,-6,-4,12},{-6,-5,-1,13},{-6,1,5,13},
   {-12,4,6,13},{-4,6,12,13},{-14,-7,-6},{-14,6,7},{-7,6,14},{-6,7,14},{-16,-8,-1},{-16,1,8},{-16,-15,-7},{-16,7,15},{-8,1,16},{-1,8,16},{-15,7,16},{-7,15,16}]]$
set_hm(ss_field_cnfs,[8,2], FieldMul3CNF));
/* Sbox: */
maxima> output_rijnsbox_fullcnf_stdname();
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_Sbox_shg.cnf
shell> cat AES_Sbox_shg.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > AES_Sbox_shg.wcnf
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 40000000 -wtarget 294 -solve 1 -seed 3213901809 -i AES_Sbox_shg.wcnf -r model AES_Sbox_s294.ass;
shell> cat  AES_Sbox_full.cnf_primes | FilterDimacs AES_Sbox_s294.ass > AES_Sbox_s294.cnf
maxima> SboxMinCNF : read_fcl_f("AES_Sbox_s294.cnf");
maxima> set_hm(ss_sbox_cnfs,8, SboxMinCNF));
   \endverbatim
   </li>
   <li> Generating small scale AES for 14 + 1/3 rounds:
   \verbatim
rounds : 14$
num_rows : 2$
num_columns : 1$
exp : 8$
final_round_b : false$
box_tran : aes_small_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r14_c1_rw2_e8_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
2064 22912 128152 0 128152 2065 1
 length count
1 112
2 448
3 4992
4 896
6 8008
7 7112
8 1344
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 14 full rounds (Key Addition, SubBytes, and MixColumns
     operation). </li>
     <li> 56 Sboxes:
      <ul>
       <li> 28 from SubBytes = 2 byte * 14 rounds; </li>
       <li> 28 from key schedule = 2 row * 1 byte * 14 rounds. </li>
      </ul>
     </li>
     <li> 56 multiplications by 02: 2 rows * 1 multiplication * 1 columns *
     14 rounds * 2 directions (forward + inverse). </li>
     <li> 56 multiplications by 03: 2 rows * 1 multiplication * 1 columns *
     14 rounds * 2 directions (forward + inverse). </li>
     <li> 800 additions of arity 2:
      <ul>
       <li> 224 from key additions = 16 bits * 14 rounds; </li>
       <li> 16 from final key addition = 16 bits; </li>
       <li> 112 from the key schedule = 1 rows * 8 bits * 14 rounds. </li>
       <li> 224 from forward MixColumns = 2 rows * 1 column * 8 bits *
       14 rounds; </li>
       <li> 224 from inverse MixColumns = 2 rows * 1 column * 8 bits * 14
       rounds. </li>
      </ul>
     </li>
     <li> 112 bits for the constant in the key schedule = 8 bits * 14 rounds.
     </li>
    </ul>
   </li>
   <li> Note that as this variant has only one column, the key schedule
   applies Sbox(K_i) + C rather than Sbox(K_i) + K_j + C where K_i and
   K_j are key words from the previous round key. </li>
   <li> The Sboxes and multiplications use the "minimum" translations,
   which have the following number of clauses of each length:
   \verbatim
maxima> ncl_list_fcs(ev_hm(ss_sbox_cnfs,8));
[[6,143],[7,127],[8,24]]
maxima> ncl_list_fcs(ev_hm(ss_field_cnfs,[8,2]))
[[2,8],[3,12]]
maxima> ncl_list_fcs(ev_hm(ss_field_cnfs,[8,3]))
[[3,20],[4,16]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 112 = key schedule constant * 1; </li>
     <li> 2 : 448 = 56 multiplications by 02 * 8; </li>
     <li> 3 : 4992 = 56 multiplications by 02 * 12 + 56 multiplications by 03
     * 20 + 800 additions (arity 2) * 4; </li>
     <li> 4 : 896 = 56 multiplications by 04 * 16; </li>
     <li> 6 : 8008 = 56 S-boxes * 143; </li>
     <li> 7 : 7112 = 56 S-boxes * 127; </li>
     <li> 8 : 1344 = 56 S-boxes * 24. </li>
    </ul>
   </li>
   <li> Generate random assignments for the plaintext and ciphertext, leaving
   the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   Merge the assignments with the translations:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r14_c1_rw2_e4_f0.cnf ssaes_pkpair_r14_c1_rw2_e4_f0_s1.cnf > r14_keyfind.cnf; done
   \endverbatim
   </li>
   <li> Solver runs:
    <ul>
     <li> cryptominisat:
     \verbatim
shell> cryptominisat r14_keyfind.cnf
<snip>
c restarts                 : 9433
c conflicts                : 2030565     (4696.47   / sec)
c decisions                : 4350327     (1.04      % random)
c CPU time                 : 432.36      s
     \endverbatim
     </li>
     <li> glucose:
     \verbatim
shell> glucose r14_keyfind.cnf
<snip>
c restarts              : 918
c conflicts             : 2226769        (2807 /sec)
c decisions             : 2781657        (1.75 % random) (3507 /sec)
c propagations          : 136974745      (172675 /sec)
c CPU time              : 793.25 s
     \endverbatim
     </li>
    </ul>
   </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r14_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 1 1 2 8 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>

*/
