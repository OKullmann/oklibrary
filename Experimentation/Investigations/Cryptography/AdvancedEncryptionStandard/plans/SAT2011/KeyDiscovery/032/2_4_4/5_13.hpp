// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/032/2_4_4/5_13.hpp
  \brief Investigations into small scale AES key discovery for 5+1/3 round AES with a 2x4 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   5 + 1/3 round small scale AES with two rows, four columns, using the 4-bit
   field size. </li>
   <li> The AES encryption scheme we model takes a 32-bit plaintext and
   32-bit key and outputs a 32-bit ciphertext. The plaintext, key and
   ciphertext are all considered, column by column, as 2x4 matrices of 4-bit
   elements. </li>
   <li> In other words, in the AES blocks (plaintext, key, ciphertext etc),
   the 4-bit element at position (i,j) in the matrix is the ((i-1)*2 + j)-th
   4-bit word of the 32-bits. </li>
   <li> The 4-bit element (b_0,b_1,b_2,b_3) is considered as the polynomial
   b_0 * x^3 + b_1 * x^2 + b_2 * x + b_3. Addition and multiplication
   on these polynomials is defined as usual, modulo the polynomial x^4+x+1.
   </li>
   <li> The encryption scheme applies the following operations:
   <ol>
    <li> The round function iterated five times, consisting of (for
    round 0 <= i < 5):
    <ol>
     <li> Addition of round key i-1 to plaintext. </li>
     <li> Application of SubBytes (Sbox to each 4-bit element) operation. </li>
     <li> Application of linear diffusion operation. </li>
    </ol>
    <li> Addition of round key 5, resulting in the ciphertext. </li>
   </ol>
   </li>
   <li> The Sbox is non-linear permutation over the set of 4-bit elements,
   defined as inversion within the 4-bit field composed with an affine
   transformation. </li>
   <li> The linear diffusion operation applies a linear permutation to
   the input matrix, consisting of:
   <ol>
    <li> A cyclical shift of row 2 of the matrix by one 4-bit element to the
    left, that is, the matrix matrix([1,2],[3,4]) would map to
    matrix([1,2],[4,3]). </li>
    <li> The AES MixColumns operation, which takes the input matrix and
    applies a matrix multiplication by the constant matrix
    \verbatim
maxima> ss_mixcolumns_matrix(2,4,2);
 matrix([x+1,x],[x,x+1]
    \endverbatim
    over the 4-bit field. As it is a matrix multiplication, this operation can
    be broken down into a "MixColumn" operation on each column of the input
    matrix. </li>
   </ol>
   </li>
   <li> In this file, we collect:
   <ul>
    <li> Solvable in 903.5 seconds by picosat, see "Using the rbase box
    translation". </li>
   </ul>
   </li>
  </ul>


  \todo Using the 1-base box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications
   as whole boxes and translating these boxes using r_1-base translations.
   </li>
   <li> Generating small scale AES for 5 + 1/3 round:
   \verbatim
num_rounds : 5$
num_rows : 2$
num_columns : 4$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r5_c4_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1436 6858 21604 0 21604 1437 1
 length count
1 20
2 480
3 4808
4 1550
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> Five full rounds (Key Addition, SubBytes, and MixColumns operation).
     </li>
     <li> 50 Sboxes:
      <ul>
       <li> 40 from SubBytes = 8 byte * 5 rounds; </li>
       <li> 10 from key schedule = 2 row * 1 word * 5 rounds. </li>
      </ul>
     </li>
     <li> 80 multiplications by 02: 2 rows * 1 multiplication * 4 columns
     * 5 rounds * 2 directions (forward + inverse). </li>
     <li> 80 multiplications by 03: 2 rows * 1 multiplication * 4 columns
     * 5 rounds * 2 directions (forward + inverse). </li>
     <li> 652 additions:
      <ul>
       <li> 652 additions of arity 2:
        <ul>
         <li> 160 from key additions = 32 bits * 5 rounds; </li>
         <li> 32 from final key addition = 32 bits; </li>
         <li> 140 from the key schedule = (32 bits - 4 bits) * 5 rounds. </li>
         <li> 160 from forward MixColumns = 2 rows * 4 column * 4 bits *
         5 rounds; </li>
         <li> 160 from inverse MixColumns = 2 rows * 4 column * 4 bits * 5
         rounds. </li>
        </ul>
       </li>
       <li> 20 additions of arity 3:
        <ul>
         <li> 20 from the key schedule = 4 bits * 5 round. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 20 bits for the constant in the key schedule = 4 bits * 5 rounds.
     </li>
    </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_fcs(ev_hm(ss_sbox_rbase_cnfs,4));
[[3,12],[4,15]]
maxima> ncl_list_fcs(ev_hm(ss_field_rbase_cnfs,[4,2]));
[[2,6],[3,4]]
maxima> ncl_list_fcs(ev_hm(ss_field_rbase_cnfs,[4,3]));
[[3,16],[4,8]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 20 = key schedule constant * 1; </li>
     <li> 2 : 480 = 80 multiplications by 02 * 6; </li>
     <li> 3 : 4808 = 50 S-boxes * 12 + 80 multiplications by 02 * 4 +
     80 multiplications by 03 * 16 + 652 additions (arity 2) * 4; </li>
     <li> 4 : 1550 = 50 S-boxes * 15 + 80 multiplications by 03 * 8 +
     20 additions (arity 3) * 8. </li>
    </ul>
   </li>
   <li> Then we can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r5_c4_rw2_e4_f0.cnf ssaes_pkpair_r5_c4_rw2_e4_f0_s1.cnf > r5_keyfind.cnf
   \endverbatim
   </li>
   <li> Picosat solves it in ~15 minutes:
   \verbatim
c 0 restarts
c 4596920 conflicts
c 4789770 decisions
c 2809104667 propagations
c 903.5 seconds total run time
   \endverbatim
   </li>
   <li> All other solvers run in the experiment were unable to solve the
   instance within 10 hours (cryptominisat, precosat236, precosat-570.1,
   minisat-2.2.0). </li>
  </ul>

*/
