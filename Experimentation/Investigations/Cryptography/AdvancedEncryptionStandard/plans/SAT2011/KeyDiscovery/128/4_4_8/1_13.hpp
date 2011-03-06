// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/128/4_4_8/1_13.hpp
  \brief Investigations into AES key discovery for one round AES (1+1/3)


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   1 + 1/3 round small scale AES with four rows, two columns, using the 8-bit
   field size. </li>
   <li> The AES encryption scheme we model takes a 128-bit plaintext and
   128-bit key and outputs a 128-bit ciphertext. The plaintext, key and 
   ciphertext are all considered, column by column, as 4x4 matrices of 8-bit 
   elements. </li>
   <li> In other words, in the AES blocks (plaintext, key, ciphertext etc), 
   the 8-bit element at position (i,j) in the matrix is the ((i-1)*4 + j)-th 
   8-bit word of the 128-bits. </li>
   <li> The 8-bit element (b_0,b_1,b_2,b_3,b_4,b_5,b_6,b_7) is considered as 
   the polynomial b_0 * x^7 + b_1 * x^6 + b_2 * x^5 + b_4 * x^3 + b_5 * x^2 + 
   b^6 * x + b_7. Addition and multiplication on these polynomials is defined
   as usual, modulo the polynomial x^8+x^4+x^3+x+1. </li>
   <li> The encryption scheme applies the following operations:
   <ol>
    <li> Addition of round key 0 (input key) to plaintext. </li>
    <li> Application of SubBytes (Sbox to each 8-bit element) operation. </li>
    <li> Application of linear diffusion operation. </li>
    <li> Addition of round key 1, resulting in the ciphertext. </li>
   </ol>
   </li>
   <li> The Sbox is non-linear permutation over the set of 8-bit elements,
   defined as inversion within the 8-bit field composed with an affine
   transformation. </li>
   <li> The linear diffusion operation applies a linear permutation to
   the input matrix, consisting of:
   <ol>
    <li> A shift of row i by i-1 to the left for all i from 1 to the number of
    rows. </li>
    <li> The AES MixColumns operation, which takes the input matrix and
    applies a matrix multiplication by the constant matrix 
    \verbatim
maxima> ss_mixcolumns_matrix(2,8,4);
 matrix([x,x+1,1,1],[1,x,x+1,1],[1,1,x,x+1],[x+1,1,1,x])
    \endverbatim
    over the 8-bit field. As it is a matrix multiplication, this operation can
    be broken down into a "MixColumn" operation on each column of the input
    matrix. </li>
   </ol>
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating AES for 1 + 1/3 round:
   \verbatim
num_rounds : 1$
num_columns : 4$
num_rows : 4$
exp : 8$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c4_rw4_e8_f0.cnf | ExtendedDimacsFullStatistics n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
31400 510620 1510056 0 1510056 31401 1
 length count
1 8
2 475136
3 1504
4 64
5 4096
17 29696
256 116
   \endverbatim
   </li>
   <li> The measured statistics match up to the computed statistics:
   \verbatim
maxima> ncl_list_ss(1,4,4,8,false,aes_ts_box,aes_mc_bidirectional);
[[1,8],[2,475136],[3,1504],[4,64],[5,4096],[17,29696],[256,116]]
   \endverbatim
   </li>
   <li> Then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw4_e8_f0.cnf ssaes_pkpair_r1_c4_rw4_e8_f0_s1.cnf > ssaes_r1_c4_rw4_e8_f0_keyfind.cnf
   \endverbatim
   </li>
   <li> MG is running experiments with various solvers, but all are ongoing 
   (after a day). </li>
   <li> The next thing to try is replacing the boxes with the r_1 bases we 
   have. </li>
  </ul>

*/
