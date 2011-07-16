// Matthew Gwynne, 16.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/064/1_16_4/4_13.hpp
  \brief Investigations into small scale AES key discovery for 4 + 1/3 round AES with a 1x16 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   4 + 1/3 round small scale AES with one row, sixteen columns, using the 4-bit
   field size. </li>
   <li> In this file, we denote this AES instance by aes(4,1,16,4). </li>
   <li> The AES encryption scheme we model takes a 64-bit plaintext and
   64-bit key and outputs a 64-bit ciphertext. </li>
   <li> The 4-bit element (b_0,b_1,b_2,b_3) is considered as the polynomial
   b_0 * x^3 + b_1 * x^2 + b_2 * x + b_3. Addition and multiplication
   on these polynomials is defined as usual, modulo the polynomial x^4+x+1.
   </li>
   <li> The encryption scheme applies the following operations:
   <ol>
    <li> Round (iterated four times):
    <ol>
     <li> Addition of round key (n-1). </li>
     <li> Application of SubBytes (Sbox to each 4-bit element) operation. </li>
    </ol>
    </li>
    <li> Addition of round key 4 yielding the ciphertext. </li>
   </ol>
   </li>
   <li> The Sbox is non-linear permutation over the set of 4-bit elements,
   defined as inversion within the 4-bit field composed with an affine
   transformation. </li>
   <li> For a full list of the possible translations, see
   "Investigating dimensions" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Experimentation.hpp.
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translation of aes(4,1,16,4):
    <ul>
     <li> We treat S-boxes and additions as boxes.
     </li>
     <li> S-boxes are translated using the canonical translation;
     see dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
     </li>
     <li> Additions are translated using their prime implicates. </li>
    </ul>
   </li>
   <li> Generating small scale AES for 4 + 1/3 rounds:
   \verbatim
num_rounds : 4$
num_rows : 1$
num_columns : 16$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
oklib_monitor : true$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r4_c16_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
2336 13268 37584 0 37584 2337 1
 length count
1 16
2 9728
3 2240
4 128
9 1088
16 68
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 4 full rounds (Key Addition, SubBytes, and MixColumns operation).
     </li>
     <li> 68 S-boxes:
      <ul>
       <li> 64 from SubBytes = 16 byte * 4 rounds; </li>
       <li> 4 from key schedule = 1 row * 1 byte * 4 rounds. </li>
      </ul>
     </li>
     <li> 1088 additions:
      <ul>
       <li> 512 additions of arity 1:
        <ul>
         <li> 512 from MixColumns = 1 row * 16 columns * 2 directions
         (forward + inverse) * 4 bits * 4 rounds. </li>
        </ul>
       </li>
       <li> 560 additions of arity 2:
        <ul>
         <li> 256 from key additions = 64 bits * 4 round; </li>
         <li> 64 from final key addition = 64 bits; </li>
         <li> 240 from the key schedule = (64 bits - 4 bits) * 4 round. </li>
        </ul>
       </li>
       <li> 16 additions of arity 3:
        <ul>
         <li> 16 from the key schedule = 4 bits * 4 rounds. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 16 bits for the constant in the key schedule = 4 bits * 4 rounds.
     </li>
    </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 16 = key schedule constant * 1; </li>
     <li> 2 : 9728 = 68 S-boxes * 128; </li>
     <li> 3 : 2240 = 560 additions (arity 2) * 4; </li>
     <li> 4 : 128 = 16 additions (arity 3) * 8; </li>
     <li> 9 : 1088 = 68 S-boxes * 16; </li>
     <li> 16 : 68 = 68 S-boxes * 1. </li>
    </ul>
   </li>
   <li> Then we can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r4_c16_rw1_e4_f0.cnf ssaes_pkpair_r4_c16_rw1_e4_f0_s1.cnf > experiment_r4_k1.cnf
   \endverbatim
   </li>
   <li> march_pl solves it in 6s:
   \verbatim
shell> march_pl experiment_r4_k.1.cnf
<snip>
c main():: nodeCount: 2509
c main():: dead ends in main: 2
c main():: lookAheadCount: 539606
c main():: unitResolveCount: 48512
c main():: time=6.010000
<snip>
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves it in 19s:
   \verbatim
shell> minisat-2.2.0 experiment_r4_k1.cnf
<snip>
restarts              : 354
conflicts             : 130444         (6942 /sec)
decisions             : 159038         (0.00 % random) (8464 /sec)
propagations          : 94165695       (5011479 /sec)
CPU time              : 18.79 s
   \endverbatim
   </li>
  </ul>


  \todo Using the 1-base box translation
  <ul>
   <li> Translation of aes(4,1,16,4):
    <ul>
     <li> We treat S-boxes and additions as boxes. </li>
     <li> S-boxes are translated using 1-bases; see ss_sbox_rbase_cnfs in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/data/SmallScaleSboxCNF.mac.
     </li>
     <li> Additions are translated using their prime implicates. </li>
    </ul>
   </li>
   <li> Generating small scale AES for 4 + 1/3 rounds:
   \verbatim
num_rounds : 4$
num_rows : 1$
num_columns : 16$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r4_c16_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1248 5244 15824 0 15824 1249 1
 length count
1 16
2 1024
3 3056
4 1148
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 4 full rounds (Key Addition, SubBytes, and MixColumns operation).
     </li>
     <li> 68 S-boxes:
      <ul>
       <li> 64 from SubBytes = 16 byte * 4 rounds; </li>
       <li> 4 from key schedule = 1 row * 1 byte * 4 rounds. </li>
      </ul>
     </li>
     <li> 1088 additions:
      <ul>
       <li> 512 additions of arity 1:
        <ul>
         <li> 256 from MixColumns = 1 row * 16 columns * 2 directions
         (forward + inverse) * 4 bits * 4 rounds. </li>
        </ul>
       </li>
       <li> 560 additions of arity 2:
        <ul>
         <li> 256 from key additions = 64 bits * 4 round; </li>
         <li> 64 from final key addition = 64 bits; </li>
         <li> 240 from the key schedule = (64 bits - 4 bits) * 4 round. </li>
        </ul>
       <li> 16 additions of arity 3:
        <ul>
         <li> 16 from the key schedule = 4 bits * 4 rounds. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 16 bits for the constant in the key schedule = 4 bits * 4 rounds.
     </li>
    </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_fcs(ev_hm(ss_sbox_rbase_cnfs,4));
[[3,12],[4,15]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 16 = key schedule constant * 1; </li>
     <li> 2 : 1024 = 512 additions (arity 1) * 2; </li>
     <li> 3 : 3056 = 68 S-boxes * 12 + 560 additions (arity 2) * 4; </li>
     <li> 4 : 1148 = 68 S-boxes * 15 + 16 additions (arity 3) * 8; </li>
    </ul>
   </li>
   <li> Then we can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r4_c16_rw1_e4_f0.cnf ssaes_pkpair_r4_c16_rw1_e4_f0_s1.cnf > experiment_r4_k1.cnf
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves it in 8s:
   \verbatim
shell> minisat-2.2.0 experiment_r4_k1.cnf
<snip>
restarts              : 325
conflicts             : 122695         (15356 /sec)
decisions             : 137095         (0.00 % random) (17158 /sec)
propagations          : 42281428       (5291793 /sec)
CPU time              : 7.99 s
   \endverbatim
   </li>
  </ul>

*/
