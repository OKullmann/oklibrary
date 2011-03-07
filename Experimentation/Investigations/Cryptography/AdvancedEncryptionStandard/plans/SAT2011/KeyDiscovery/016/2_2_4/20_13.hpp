// Matthew Gwynne, 17.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_2_4/20_13.hpp
  \brief Investigations into small scale AES key discovery for 20 + 1/3 round AES with a 2x2 plaintext matrix and 4-bit field elements

  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   20 + 1/3 round small scale AES with two rows, two columns, using the 4-bit
   field size. </li>
   <li> The AES encryption scheme we model takes a 16-bit plaintext and
   16-bit key and outputs a 16-bit ciphertext. The plaintext, key and 
   ciphertext are all considered, column by column, as 2x2 matrices of 4-bit 
   elements. </li>
   <li> In other words, in the AES blocks (plaintext, key, ciphertext etc), 
   the 4-bit element at position (i,j) in the matrix is the ((i-1)*2 + j)-th 
   4-bit word of the 16-bits. </li>
   <li> The 4-bit element (b_0,b_1,b_2,b_3) is considered as the polynomial
   b_0 * x^3 + b_1 * x^2 + b_2 * x + b_3. Addition and multiplication
   on these polynomials is defined as usual, modulo the polynomial x^4+x+1. 
   </li>
   <li> The encryption scheme applies the following operations:
   <ol>
    <li> Round (iterated twenty times):
    <ol>
     <li> Addition of round key (n-1). </li>
     <li> Application of SubBytes (Sbox to each 4-bit element) operation. </li>
     <li> Application of linear diffusion operation. </li>
    </ol>
    </li>
    <li> Addition of round key 20 yielding the ciphertext. </li>
   </ol>
   </li>
   <li> The Sbox is non-linear permutation over the set of 4-bit elements,
   defined as inversion within the 4-bit field composed with an affine
   transformation. </li>
   <li> The linear diffusion operation applies a linear permutation to
   the input matrix, consisting of:
   <ol>
    <li> A shift of row i by i-1 to the left for all i from 1 to the number of
    rows. </li>
    <li> The AES MixColumns operation, which takes the input matrix and
    applies a matrix multiplication by the constant matrix 
    \verbatim
maxima> ss_mixcolumns_matrix(2,4,2);
 matrix([x+1,x],[x,x+1])
    \endverbatim
    over the 4-bit field. As it is a matrix multiplication, this operation can
    be broken down into a "MixColumn" operation on each column of the input
    matrix. </li>
   </ol>
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications 
   as whole boxes and translating these boxes using the canonical translation.
   </li>
   <li> Generating small scale AES for four rounds (with MixColumns):
   \verbatim
rounds : 20$
num_columns : 2$
num_rows : 2$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r20_c2_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
9888 69384 200272 0 200272 9889 1
 length count
1 80
2 56320
3 4864
4 640
9 7040
16 440
   \endverbatim
   </li>
   <li> In this translation, we have:
   <ul>
    <li> Twenty full rounds (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 80 Sboxes in the SubBytes operation
    (2 rows * 2 columns * 20 rounds = 80). </li>
    <li> 976 additions within the round and key additions, coming from:
     <ul>
      <li> 48 additions from key additions 
      (21 round keys * 16-bit additions = 336). </li>
      <li> 640 additions from the matrix multiplication in the diffusion 
      operation 
      (2 rows * 2 columns * 2 directions * 4 bits * 20 rounds = 640).
      </li>
     </ul>
    </li>
    <li> 160 multiplications by 02 from the MixColumns operation
    (2 rows * 2 columns * 2 directions * 20 rounds = 160). </li>
    <li> 160 multiplications by 03 from the MixColumns operation
    (2 rows * 2 columns * 2 directions * 20 rounds = 160). </li>
    <li> 40 Sboxes in the AES key schedule
    (2 rows * 20 rounds = 40). </li>
    <li> 320 additions in the key schedule:
    <ul>
     <li> 80 additions of arity three
     (1 row * 1 column * 4 bits * 20 rounds = 80). </li>
     <li> 24 additions of arity two 
     ((1 rows * 2 columns + 1 row * 1 column) * 4 bits * 20 rounds = 240). 
     </li>
    </ul>
    </li>
    <li> 80 bits for the constant in the key schedule. 
    (4 bits * 20 rounds = 80). </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(20,2,2,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,80],[2,56320],[3,4864],[4,640],[9,7040],[16,440]]
maxima> ncl_list_ss_gen(20,2,2,4,ss_mixcolumns_matrix(2,4,2),[[2,'s2],[9,'s9],[16,'s16]],[[x,[[2,'m2_2],[9,'m2_9],[16,'m2_16]]],[x+1,[[2,'m3_2],[9,'m3_9],[16,'m3_16]]]],false,aes_mc_bidirectional);
[[1,80],[2,120*s2+160*m3_2+160*m2_2],[3,4864],[4,640],[9,120*s9+160*m3_9+160*m2_9],
        [16,120*s16+160*m3_16+160*m2_16]]
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 80 unit clauses for the 4-bit constant in the key expansion. </li>
    <li> 56320 binary clauses, coming from 120 Sboxes and 160 of each of the 
    two multiplications (440 * 128 = 56320). </li>
    <li> 4864 ternary clauses, coming from 1216 additions of arity two
    (1216 * 4 = 4864). </li>
    <li> 640 clauses of length four, coming from 80 additions of arity three
    (80 * 8 = 640). </li>
    <li> 7040 clauses of length seven, coming from 120 Sboxes and 160 of each
    of the two multiplications (440 * 16 = 7040). </li>
    <li> 440 clauses of length sixteen, coming from from 120 Sboxes and 160
    of each of the two multiplications (440 * 1 = 440). </li>
   </ul>
   </li>
   <li> Then we can generate random assignments with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignments with the translations:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r20_c2_rw2_e4_f0.cnf ssaes_pkpair_r20_c2_rw2_e4_f0_s1.cnf > experiment_r20_k1.cnf
shell> AppendDimacs-O3-DNDEBUG ssaes_r20_c2_rw2_e4_f0.cnf ssaes_pkpair_r20_c2_rw2_e4_f0_s1.cnf > experiment_r20_k5.cnf
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves this in times ranging from 3.45s to 40s:
   \verbatim
shell> minisat-2.2.0 experiment_r20_k1.cnf
<snip>
restarts              : 18
conflicts             : 3513           (1018 /sec)
decisions             : 3838           (0.00 % random) (1112 /sec)
propagations          : 29196071       (8462629 /sec)
conflict literals     : 43580          (86.63 % deleted)
Memory used           : 24.00 MB
CPU time              : 3.45 s

shell> minisat-2.2.0 experiment_r20_k5.cnf
<snip>
restarts              : 158
conflicts             : 52416          (1426 /sec)
decisions             : 59697          (0.00 % random) (1624 /sec)
propagations          : 262216771      (7131269 /sec)
conflict literals     : 1656658        (69.32 % deleted)
Memory used           : 32.00 MB
CPU time              : 36.77 s
   \endverbatim
   </li>
  </ul>

*/
