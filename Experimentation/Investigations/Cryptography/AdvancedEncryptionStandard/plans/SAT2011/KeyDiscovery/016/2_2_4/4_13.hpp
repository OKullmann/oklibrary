// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_2_4/4_13.hpp
  \brief Investigations into small scale AES key discovery for 4 + 1/3 round AES with a 2x2 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   4 + 1/3 round small scale AES with two rows, two columns, using the 4-bit
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
    <li> Round (iterated four times):
    <ol>
     <li> Addition of round key (n-1). </li>
     <li> Application of SubBytes (Sbox to each 4-bit element) operation. </li>
     <li> Application of linear diffusion operation. </li>
    </ol>
    </li>
    <li> Addition of round key 4 yielding the ciphertext. </li>
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
   <li> Generating small scale AES for 4 + 1/3 rounds:
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
    <li> Four full rounds (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 16 Sboxes in the SubBytes operation
    (2 rows * 2 columns * 4 rounds = 16). </li>
    <li> 208 additions within the round and key additions, coming from:
     <ul>
      <li> 80 additions from key additions
      (5 round keys * 16-bit additions = 80). </li>
      <li> 128 additions from the matrix multiplication in the diffusion
      operation (2 rows * 2 columns * 2 directions * 4 bits * 4 rounds = 128).
      </li>
     </ul>
    </li>
    <li> 32 multiplications by 02 from the MixColumns operation
    (2 rows * 2 columns * 2 directions * 4 rounds = 32). </li>
    <li> 32 multiplications by 03 from the MixColumns operation
    (2 rows * 2 columns * 2 directions * 4 rounds = 32). </li>
    <li> 8 Sboxes in the AES key schedule
    (2 rows * 4 rounds = 8). </li>
    <li> 64 additions in the key schedule:
    <ul>
     <li> 16 additions of arity three
     (1 row * 1 column * 4 bits * 4 rounds = 16). </li>
     <li> 48 additions of arity two
     ((1 rows * 2 columns + 1 row * 1 column) * 4 bits * 4 rounds = 48).
     </li>
    </ul>
    </li>
    <li> 16 bits for the constant in the key schedule.
    (4 bits * 4 rounds = 16). </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(4,2,2,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,16],[2,11264],[3,1024],[4,128],[9,1408],[16,88]]
maxima> ncl_list_ss_gen(4,2,2,4,ss_mixcolumns_matrix(2,4,2),[[2,'s2],[9,'s9],[16,'s16]],[[x,[[2,'m2_2],[9,'m2_9],[16,'m2_16]]],[x+1,[[2,'m3_2],[9,'m3_9],[16,'m3_16]]]],false,aes_mc_bidirectional);
[[1,16],[2,24*s2+32*m3_2+32*m2_2],[3,1024],[4,128],[9,24*s9+32*m3_9+32*m2_9],
        [16,24*s16+32*m3_16+32*m2_16]]
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 16 unit-clauses for the 4-bit constants in the key expansion. </li>
    <li> 11264 binary clauses, coming from 24 Sboxes and 32 of each of the two
    multiplications (88 * 128 = 11264). </li>
    <li> 1024 ternary clauses, coming from 136 additions of arity two
    (256 * 4 = 1024). </li>
    <li> 128 clauses of length four, coming from 16 additions of arity three
    (16 * 8 = 128). </li>
    <li> 1408 clauses of length seven, coming from 24 Sboxes and 32 of each of
    the two multiplications (88 * 16 = 1408). </li>
    <li> 88 clauses of length sixteen, coming from from 24 Sboxes and 32 of
    each of the two multiplications (88 * 1 = 88). </li>
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
