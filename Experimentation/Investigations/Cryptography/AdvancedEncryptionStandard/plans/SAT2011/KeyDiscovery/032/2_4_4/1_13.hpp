// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/032/2_4_4/1_13.hpp
  \brief Investigations into small scale AES key discovery for 1+1/3 round AES with a 2x4 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   1 + 1/3 round small scale AES with two rows, four columns, using the 4-bit
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
    <li> Addition of round key 0 (input key) to plaintext. </li>
    <li> Application of SubBytes (Sbox to each 4-bit element) operation. </li>
    <li> Application of linear diffusion operation. </li>
    <li> Addition of round key 1, resulting in the ciphertext. </li>
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
    <li> Solvable in 0.9 seconds by the OKsolver. See "Using the canonical
    translation". </li>
   </ul>
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating small scale AES for 1 + 1/3 round:
   \verbatim
num_rounds : 1$
num_columns : 4$
num_rows : 2$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c4_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1036 6750 19476 0 19476 1037 1
 length count
1 4
2 5376
3 624
4 32
9 672
16 42
   \endverbatim
   </li>
   <li> Note we have the following numbers of each type of box in this 
   translation:
   \verbatim
maxima> component_statistics_ss(1,4,2,4,false,aes_mc_bidirectional);
[1,0,8,128,[[x,16],[x+1,16]],2,32,4]
   \endverbatim
   That is, we have:
   <ul>
    <li> One full round (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> No special rounds (Key Addition, SubBytes and ShiftRows). </li>
    <li> 8 Sboxes in the AES round components. This comes from the four 
    columns and two rows of the plaintext matrix with one round. </li>
    <li> 128 additions within the round and key additions, coming from:
     <ul>
      <li> Two 32-bit key additions (adding two bits), yielding 
      64 additions of arity two in total. </li>
      <li> Two additions for the MixColumn operation over four columns, 
      applied twice (forward and backward), yielding 64 additions of arity two
      in total. </li>
     </ul>
    </li>
    <li> 16 multiplications each by 02 and 03 across the MixColumns 
    operations. Two 02 multiplications and two 03 from each MixColumn
    matrix mulitiplication, across four columns, applied twice (once
    forward and once in for the inverse MixColumn), giving 2 x 4 x 2 = 16
    instances of each multiplication. </li>
    <li> 2 Sboxes in the AES key schedule. </li>
    <li> 32 additions in the key schedule. One addition of arity three
    for each bit in the element in the AES key, and one addition of arity two
    for all remaining bits in the key schedule. </li>
    <li> 4 bits for the constant in the key schedule. </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(1,4,2,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,4],[2,5376],[3,624],[4,32],[9,672],[16,42]]
maxima> ncl_list_ss_gen(1,4,2,4,ss_mixcolumns_matrix(2,4,2),[[2,'s2],[9,'s9],[16,'s16]],[[x,[[2,'m2_2],[9,'m2_9],[16,'m2_16]]],[x+1,[[2,'m3_2],[9,'m3_9],[16,'m3_16]]]],false,aes_mc_bidirectional);
[[1,4],[2,10*s2+16*m3_2+16*m2_2],[3,624],[4,32],[9,10*s9+16*m3_9+16*m2_9],
         [16,10*s16+16*m3_16+16*m2_16]]
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 4 unit clauses for the 4-bit constant in the Key schedule. </li>
    <li> 5376 binary clauses, coming from 10 Sboxes and 16 of each of the two 
    multiplications (42 * 128 = 5376). </li>
    <li> 624 ternary clauses, coming from 156 additions of arity two
    (156 * 4 = 624). </li>
    <li> 32 clauses of length four, coming from 4 additions of arity three
    (4 * 8 = 32). </li>
    <li> 672 clauses of length seven, coming from 10 Sboxes and 16 of each of 
    the two multiplications (42 * 16 = 672). </li>
    <li> 42 clauses of length sixteen, coming from from 10 Sboxes and 16 of 
    each of the two multiplications (42 * 1 = 42). </li>
   </ul>
   </li>
   <li> Then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw2_e4_f0.cnf ssaes_pkpair_r1_c4_rw2_e4_f0_s1.cnf > r1_keyfind.cnf
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves it in 0.03s:
   \verbatim
restarts              : 11
conflicts             : 1654           (55133 /sec)
decisions             : 2581           (0.00 % random) (86033 /sec)
propagations          : 211137         (7037900 /sec)
conflict literals     : 36509          (24.97 % deleted)
Memory used           : 19.00 MB
CPU time              : 0.03 s
   \endverbatim
   </li>
   <li> cryptominisat solves it in 0.03s:
   \verbatim
c Verified 1017 clauses.
c num threads              : 1
c restarts                 : 3
c dynamic restarts         : 0
c static restarts          : 3
c full restarts            : 0
c total simplify time      : 0.00
c learnts DL2              : 0
c learnts size 2           : 5507
c learnts size 1           : 68          (6.56      % of vars)
c filedLit time            : 0.01        (33.33     % time)
c v-elim SatELite          : 32          (3.09      % vars)
c SatELite time            : 0.01        (33.33     % time)
c v-elim xor               : 0           (0.00      % vars)
c xor elim time            : 0.00        (0.00      % time)
c num binary xor trees     : 306
c binxor trees' crown      : 448         (1.46      leafs/tree)
c bin xor find time        : 0.00
c OTF clause improved      : 1           (0.00      clauses/conflict)
c OTF impr. size diff      : 1           (1.00       lits/clause)
c OTF cl watch-shrink      : 119         (0.33      clauses/conflict)
c OTF cl watch-sh-lit      : 573         (4.82       lits/clause)
c tried to recurMin cls    : 82          (22.65      % of conflicts)
c updated cache            : 0           (0.00       lits/tried recurMin)
c clauses over max glue    : 0           (0.00      % of all clauses)
c conflicts                : 362         (12066.67  / sec)
c decisions                : 581         (0.00      % random)
c bogo-props               : 1559491     (51983033.33 / sec)
c conflict literals        : 6178        (21.18     % deleted)
c Memory used              : 30.70       MB
c CPU time                 : 0.03        s
   \endverbatim
   </li>
   <li> OKsolver solves it in 0.9s:
   \verbatim
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           1036
c initial_number_of_clauses             6814
c initial_number_of_literal_occurrences 19540
c number_of_initial_unit-eliminations   68
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           68
c reddiff_number_of_clauses             212
c reddiff_number_of_literal_occurrences 660
c number_of_2-clauses_after_reduction   5504
c running_time(sec)                     0.9
c number_of_nodes                       103
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                3705
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        11
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             experiment_r1_k1.cnf
   \endverbatim
   </li>
  </ul>

*/
