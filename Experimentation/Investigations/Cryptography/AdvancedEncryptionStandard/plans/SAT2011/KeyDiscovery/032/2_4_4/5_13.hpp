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
    <li> A shift of row i by i-1 to the left for all i from 1 to the number of
    rows. </li>
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

  
  \todo Using the rbase box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications 
   as whole boxes and translating these boxes using r_1-base translations.
   </li>
   <li> Generating small scale AES for 5 + 1/3 round:
   \verbatim
num_rounds : 5$
num_columns : 4$
num_rows : 2$
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
    <li> Five full rounds (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 40 Sboxes in the SubBytes operation 
    (2 rows * 4 columns * 5 rounds = 8). </li>
    <li> 512 additions within the round and key additions, coming from:
     <ul>
      <li> 192 additions from key additions 
      (6 round keys * 32-bit additions = 192). </li>
      <li> 320 additions from the matrix multiplication in the diffusion 
      operation (2 rows * 4 columns * 2 directions * 4 bits * 5 rounds = 320).
      </li>
     </ul>
    </li>
    <li> 80 multiplications by 02 from the MixColumns operation
    (2 rows * 4 columns * 2 directions * 5 rounds = 80). </li>
    <li> 80 multiplications by 03 from the MixColumns operation
    (2 rows * 4 columns * 2 directions = 80). </li>
    <li> 10 Sboxes in the AES key schedule (2 rows * 5 rounds = 10). </li>
    <li> 160 additions in the key schedule:
    <ul>
     <li> 20 additions of arity three 
     (1 row * 1 column * 4 bits * 5 rounds = 20). </li>
     <li> 140 additions of arity two 
     ((1 rows * 3 columns + 2 rows * 2 columns) * 4 bits * 5 rounds = 140). 
     </li>
    </ul>
    </li>
    <li> 20 bits for the constant in the key schedule 
    (4 bits * 5 rounds = 20). </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(1,4,2,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,20],[2,480],[3,4808],[4,1550]]
maxima> ncl_list_ss_gen(5,4,2,4,ss_mixcolumns_matrix(2,4,2),[[2,'s2],[3,'s3],[4,'s4]],[[x,[[2,'m2_2],[3,'m2_3],[4,'m2_4]]],[x+1,[[2,'m3_2],[3,'m3_4],[4,'m3_4]]]],false,aes_mc_bidirectional);
[[1,20],[2,50*s2+80*m3_2+80*m2_2],[3,50*s3+80*m3_4+80*m2_3+2608],
        [4,50*s4+80*m3_4+80*m2_4+160]]
maxima> ncl_list_fcs(ev_hm(ss_sbox_rbase_cnfs,4));
[[3,12],[4,15]]
maxima> ncl_list_fcs(ev_hm(ss_field_rbase_cnfs,[4,2]));
[[2,6],[3,4]]
maxima> ncl_list_fcs(ev_hm(ss_field_rbase_cnfs,[4,3]));
[[3,16],[4,8]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 20 unit clauses for the 4-bit constants in the key expansion. </li>
    <li> 480 binary clauses, coming from 50 Sboxes and 80 of each of the two 
    multiplications ((50 * 0) + (80 * 6) + (80 * 0) = 480). </li>
    <li> 4808 ternary clauses, coming from 652 additions of arity two,
    50 Sboxes and 80 of each multiplication
    ((652 * 4) + (50*12) + (80*4) + (80*16) = 4808). </li>
    <li> 1550 clauses of length four, coming from 20 additions of arity three,
    50 Sboxes and 80 multiplications by 03
    ((20 * 8) + (50 * 15) + (80 * 8) = 1550). </li>
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
c 0 iterations
c 0 restarts
c 0 failed literals
c 4596920 conflicts
c 4789770 decisions
c 84 fixed variables
c 177868241 learned literals
c 40.4% deleted literals
c 2809104667 propagations
c 100.0% variables used
c 903.5 seconds in library
c 3.1 megaprops/second
c 1 simplifications
c 371 reductions
c 761.7 MB recycled
c 5.6 MB maximally allocated
c 903.5 seconds total run time
   \endverbatim
   </li>
   <li> All other solvers run in the experiment were unable to solve the 
   instance within 10 hours (cryptominisat, precosat236, precosat-570.1, 
   minisat-2.2.0). </li>
  </ul>

*/
