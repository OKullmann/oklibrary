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
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications 
   as whole boxes and translating these boxes using the canonical translation.
   </li>
   <li> Generating small scale AES for 4 + 1/3 rounds:
   \verbatim
num_rounds : 4$
num_columns : 16$
num_rows : 1$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
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
    <li> Four full rounds (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 64 Sboxes in the SubBytes operation 
    (1 rows * 16 columns * 4 rounds = 64). </li>
    <li> 832 additions within the round and key additions, coming from:
     <ul>
      <li> 320 additions from key additions 
      (5 round keys * 64-bit additions = 320). </li>
      <li> 512 additions from the identity matrix multiplication in the 
      diffusion operation of arity one
      (1 rows * 16 columns * 2 directions * 4 bits * 4 rounds = 512).
      </li>
     </ul>
    </li>
    <li> 4 Sboxes in the AES key schedule 
    (1 rows * 4 rounds = 4). </li>
    <li> 256 additions in the key schedule:
    <ul>
     <li> 16 additions of arity three 
     (1 row * 1 column * 4 bits * 4 rounds = 16). </li>
     <li> 240 additions of arity two 
     (15 columns * 4 bits * 4 rounds = 240). </li>
    </ul>
    </li>
    <li> 16 bits for the constant in the key schedule
    (1 rows * 4 bit * 4 rounds = 16). </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(4,16,1,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,16],[2,9728],[3,2240],[4,128],[9,1088],[16,68]]
maxima> ncl_list_ss_gen(4,16,1,4,ss_mixcolumns_matrix(2,4,1),[[2,'s2],[9,'s9],[16,'s16]],[],false,aes_mc_bidirectional);
[[1,16],[2,68*s2+1024],[3,2240],[4,128],[9,68*s9],[16,68*s16]]
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 16 unit-clauses for the 4-bit constants in the key expansion
    (4 bits  * 4 rounds = 16). </li>
    <li> 9728 binary clauses, coming from 68 Sboxes and 512 additions of
    arity one (68 * 128 + 512 * 2 = 9728). </li>
    <li> 2240 ternary clauses, coming from 560 additions of arity two
    (560 * 4 = 2240). </li>
    <li> 128 clauses of length four, coming from 16 additions of arity three
    (16 * 8 = 128). </li>
    <li> 1088 clauses of length nine, coming from 68 Sboxes 
    (68 * 16 = 1088). </li>
    <li> 68 clauses of length sixteen, coming from from 68 Sboxes 
    (68 * 1 = 68). </li>
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
c main():: necessary_assignments: 10106
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 12092, succes #: 6168
c main():: doublelook: overall 2.285 of all possible doublelooks executed
c main():: doublelook: succesrate: 51.009, average DL_trigger: 126.297
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
conflict literals     : 3281874        (47.79 % deleted)
Memory used           : 25.00 MB
CPU time              : 18.79 s
   \endverbatim
   </li>
  </ul>


  \todo Using the rbase box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications 
   as whole boxes and translating these boxes using the canonical translation.
   </li>
   <li> Generating small scale AES for 4 + 1/3 rounds:
   \verbatim
num_rounds : 4$
num_columns : 16$
num_rows : 1$
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
    <li> Four full rounds (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 64 Sboxes in the SubBytes operation 
    (1 rows * 16 columns * 4 rounds = 64). </li>
    <li> 832 additions within the round and key additions, coming from:
     <ul>
      <li> 320 additions from key additions 
      (5 round keys * 64-bit additions = 320). </li>
      <li> 512 additions from the identity matrix multiplication in the 
      diffusion operation of arity one
      (1 rows * 16 columns * 2 directions * 4 bits * 4 rounds = 512).
      </li>
     </ul>
    </li>
    <li> 4 Sboxes in the AES key schedule 
    (1 rows * 4 rounds = 4). </li>
    <li> 256 additions in the key schedule:
    <ul>
     <li> 16 additions of arity three 
     (1 row * 1 column * 4 bits * 4 rounds = 16). </li>
     <li> 240 additions of arity two 
     (15 columns * 4 bits * 4 rounds = 240). </li>
    </ul>
    </li>
    <li> 16 bits for the constant in the key schedule
    (1 rows * 16 bit = 16). </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(4,16,1,4,false,aes_rbase_box,aes_mc_bidirectional);
[[1,16],[2,1024],[3,3056],[4,1148]]
maxima> ncl_list_ss_gen(4,16,1,4,ss_mixcolumns_matrix(2,4,1),[[3,'s3],[4,'s4]],[],false,aes_mc_bidirectional);
[[1,16],[2,1024],[3,68*s3+2240],[4,68*s4+128]]
maxima> ncl_list_fcs(ev_hm(ss_sbox_rbase_cnfs,4));
[[3,12],[4,15]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 16 unit-clauses for the 4-bit constants in the key expansion
    (4 bits  * 4 rounds = 16). </li>
    <li> 1024 binary clauses, coming from 512 additions of arity one 
    (512 * 2 = 1024). </li>
    <li> 3056 ternary clauses, coming from 68 Sboxes and 560 additions of 
    arity two (68 * 12 + 560 * 4 = 3056). </li>
    <li> 1148 clauses of length four, coming from 68 Sboxes and 16 additions
    of arity three (68 * 15 + 16 * 8 = 1148). </li>
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
conflict literals     : 1862830        (38.38 % deleted)
Memory used           : 19.00 MB
CPU time              : 7.99 s
   \endverbatim
   </li>
  </ul>

*/
