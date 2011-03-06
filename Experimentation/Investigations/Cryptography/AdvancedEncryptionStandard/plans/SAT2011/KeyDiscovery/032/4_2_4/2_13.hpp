// Matthew Gwynne, 18.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/032/4_2_4/2_13.hpp
  \brief Investigations into small scale AES key discovery for 2 + 1/3 round AES with a 4x2 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   2 + 1/3 round small scale AES with four rows, two columns, using the 4-bit
   field size. </li>
   <li> The AES encryption scheme we model takes a 32-bit plaintext and
   32-bit key and outputs a 32-bit ciphertext. The plaintext, key and 
   ciphertext are all considered, column by column, as 4x2 matrices of 4-bit 
   elements. </li>
   <li> In other words, in the AES blocks (plaintext, key, ciphertext etc), 
   the 4-bit element at position (i,j) in the matrix is the ((i-1)*4 + j)-th 
   4-bit word of the 32-bits. </li>
   <li> The 4-bit element (b_0,b_1,b_2,b_3) is considered as the polynomial
   b_0 * x^3 + b_1 * x^2 + b_2 * x + b_3. Addition and multiplication
   on these polynomials is defined as usual, modulo the polynomial x^4+x+1. 
   </li>
   <li> The encryption scheme applies the following operations:
   <ol>
    <li> Addition of round key 0 (input key) to plaintext. </li>
    <li> Application of SubBytes (Sbox to each 4-bit element) operation. </li>
    <li> Application of linear diffusion operation. </li>
    <li> Addition of round key 1, resulting in the ciphertext. </li>
    <li> Application of SubBytes (Sbox to each 4-bit element) operation. </li>
    <li> Application of linear diffusion operation. </li>
    <li> Addition of round key 2, resulting in the ciphertext. </li>
   </ol>
   </li>
   <li> The Sbox is non-linear permutation over the set of 4-bit elements,
   defined as inversion within the 4-bit field composed with an affine
   transformation. </li>
   <li> The linear diffusion operation applies a linear permutation to
   the input matrix, consisting of:
   <ol>
    <li> A shift of row i by i-1 to the left for all i from 1 to the number of rows.. </li>
    <li> The AES MixColumns operation, which takes the input matrix and
    applies a matrix multiplication by the constant matrix 
    \verbatim
maxima> ss_mixcolumns_matrix(2,4,4);
 matrix([x,x+1,1,1],[1,x,x+1,1],[1,1,x,x+1],[x+1,1,1,x])
    \endverbatim
    over the 4-bit field. As it is a matrix multiplication, this operation can
    be broken down into a "MixColumn" operation on each column of the input
    matrix. </li>
   </ol>
   </li>
  </ul>


  \todo Using the canonical translation ??? INAPPROPRIATE: what type of translation ???
  <ul>
   <li> Generating small scale AES for 2 + 1/3 round:
   \verbatim
num_rounds : 2$
num_columns : 2$
num_rows : 4$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r2_c2_rw4_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
2696 20128 62248 0 62248 2697 1
 length count
1 8
2 15360
3 608
4 64
5 2048
9 1920
16 120
   \endverbatim
   </li>
   <li> The computed statistics for the clause sizes:
   \verbatim
maxima> ncl_list_ss(2,2,4,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,8],[2,15360],[3,608],[4,64],[5,2048],[9,1920],[16,120]]
   \endverbatim
   </li>
   <li> Then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r2_c2_rw4_e4_f0.cnf ssaes_pkpair_r2_c2_rw4_e4_f0_s1.cnf > r2_keyfind.cnf
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves it in just over 45 minutes:
   \verbatim
shell> minisat-2.2.0 r2_keyfind.cnf
<snip>
restarts              : 32761
conflicts             : 21348407       (7855 /sec)
decisions             : 23781237       (0.00 % random) (8751 /sec)
propagations          : 9574903606     (3523177 /sec)
conflict literals     : 731565401      (54.72 % deleted)
Memory used           : 77.00 MB
CPU time              : 2717.69 s
   \endverbatim
   </li>
  </ul>

*/
