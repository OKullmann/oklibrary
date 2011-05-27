// Matthew Gwynne, 21.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/14_13.hpp
  \brief Investigations into small scale AES key discovery for 14 + 1/3 round AES with a 2x1 plaintext matrix and 8-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   14 + 1/3 round small scale AES with two rows, one columns, using the 8-bit
   field size. </li>
   <li> The AES encryption scheme we model takes a 16-bit plaintext and
   16-bit key and outputs a 16-bit ciphertext. </li>
   <li> The plaintext, key and ciphertext are all considered, column by
   column, as 2x1 matrices of 8-bit elements. </li>
   <li> In other words, the 8-bit element at position (i,1) in the AES matrix
   is the i-th 8-bit word of the 16-bits. </li>
   <li> The 8-bit element (b_0,b_1,b_2,b_3,b_4,b_5,b_6,b_7) is considered as
   the polynomial b_0 * x^3 + b_1 * x^2 + b_2 * x + b_3. </li>
   <li> Addition and multiplication on these polynomials is defined as usual,
   modulo the polynomial x^8+x^4+x^3+x+1. </li>
   <li> The encryption scheme applies the following operations:
   <ol>
    <li> Addition of round key 0 (input key) to plaintext. </li>
    <li> The following operations, the "round", iterated 14 times:
     <ul>
      <li> Application of SubBytes (Sbox to each 8-bit element) operation.
      </li>
      <li> Application of MixColumns operation.. </li>
      <li> Addition of round key i, resulting in the ciphertext. </li>
     </ul>
    </li>
   </ol>
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications
   as whole boxes and translating these boxes using the canonical translation.
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
    <li> Fourteen full rounds (Key Addition, SubBytes, and MixColumn
    operation).
    </li>
    <li> 56 Sboxes (28 from SubBytes; 28 from key schedule). </li>
    <li> 800 additions (240 from key additions; 448 from MixColumns; 112 from
    key schedule). </li>
    <li> 56 multiplications by 02, 03 (from MixColumns).
    </li>
    <li> 112 bits for the constants in the key schedule. </li>
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
c dynamic restarts         : 156
c static restarts          : 12
c conflicts                : 43829       (239.90    / sec)
c decisions                : 67335       (1.98      % random)
c bogo-props               : 8698446141  (47610542.64 / sec)
c conflict literals        : 7938218     (69.50     % deleted)
c Memory used              : 531.25      MB
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
conflict literals     : 27500184       (41.42 % deleted)
Memory used           : 207.00 MB
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
c nb ReduceDB           : 14
c nb learnts DL2        : 1319
c nb learnts size 2     : 570
c nb learnts size 1     : 5
c conflicts             : 404866         (478 /sec)
c decisions             : 2156003        (1.81 % random) (2545 /sec)
c propagations          : 1254906184     (1481240 /sec)
c conflict literals     : 165086655      (35.61 % deleted)
c Memory used           : 189.94 MB
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
   <li> Translating the AES cipher treating Sboxes and field multiplications
   as whole boxes and translating these boxes using small CNFs.
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
    <li> Fourteen full rounds (Key Addition, SubBytes, and MixColumn
    operation).
    </li>
    <li> 56 Sboxes (28 from SubBytes; 28 from key schedule). </li>
    <li> 800 additions (240 from key additions; 448 from MixColumns; 112 from
    key schedule). </li>
    <li> 56 multiplications by 02, 03 (from MixColumns).
    </li>
    <li> 112 bits for the constants in the key schedule. </li>
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
   <li> The number of clauses of each length in the translation are:
    <ul>
     <li> 112 unit-clauses (key schedule constant). </li>
     <li> 448 binary clauses (56 multiplications by 02). </li>
     <li> 4992 ternary clauses (800 arity two additions; 56 multiplications *
     2). </li>
     <li> 896 clauses of length four (56 multiplications by 03). </li>
     <li> 8008 clauses of length six (56 Sboxes). </li>
     <li> 7112 clauses of length seven (56 Sboxes). </li>
     <li> 1344 clauses of length eight (56 Sboxes). </li>
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
c dynamic restarts         : 9403
c static restarts          : 30
c full restarts            : 4
c conflicts                : 2030565     (4696.47   / sec)
c decisions                : 4350327     (1.04      % random)
c conflict literals        : 28915735    (12.47     % deleted)
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
c Memory used           : 47.67 MB
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
