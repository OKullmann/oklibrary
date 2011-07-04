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
    <li> Application of MixColumns operation. </li>
    <li> Addition of round key 1, resulting in the ciphertext. </li>
    <li> Application of SubBytes (Sbox to each 4-bit element) operation. </li>
    <li> Application of MixColumns operation. </li>
    <li> Addition of round key 2, resulting in the ciphertext. </li>
   </ol>
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications
   as whole boxes and translating these boxes using the canonical translation.
   </li>
   <li> Generating small scale AES for 2 + 1/3 round:
   \verbatim
num_rounds : 2$
num_rows : 4$
num_columns : 2$
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
[2,0,16,224,[[1,32],[x,16],[x+1,16],[x^3+1,16],[x^3+x+1,16],[x^3+x^2+1,16],[x^3+x^2+x,16]],
        8,64,8]
   <li> In this translation we have:
   <ul>
    <li> Two full rounds (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 16 Sboxes in the SubBytes operation 
    (4 rows * 2 columns * 2 rounds = 16). </li>
    <li> 224 additions within the round and key additions, coming from:
     <ul>
      <li> 64 additions from key additions 
      (3 round keys * 32-bit additions = 96). </li>
      <li> 64 additions from the matrix multiplication in the diffusion 
      operation (4 rows * 2 columns * 2 directions * 4 bits * 2 rounds = 128).
      </li>
     </ul>
    </li>
    <li> 16 multiplications by 02 from the MixColumns operation
    (4 rows * 2 columns * 2 rounds = 8). </li>
    <li> 16 multiplications by 03 from the MixColumns operation
    (4 rows * 2 columns * 2 rounds = 16). </li>
    <li> 16 multiplications by 09 from the inverse MixColumns operation
    (4 rows * 2 columns * 2 rounds = 16). </li>
    <li> 16 multiplications by 11 from the inverse MixColumns operation
    (4 rows * 2 columns * 2 rounds = 16). </li>
    <li> 16 multiplications by 13 from the inverse MixColumns operation
    (4 rows * 2 columns * 2 rounds = 16). </li>
    <li> 16 multiplications by 14 from the inverse MixColumns operation
    (4 rows * 2 columns * 2 rounds = 16). </li>
    <li> 8 Sboxes in the AES key schedule (4 rows * 2 rounds = 8). </li>
    <li> 64 additions in the key schedule:
    <ul>
     <li> 8 additions of arity three 
     (1 row * 1 column * 4 bits * 2 rounds = 8). </li>
     <li> 56 additions of arity two 
     ((3 rows * 1 columns + 4 rows * 1 columns) * 4 bits * 2 rounds = 56). 
     </li>
    </ul>
    </li>
    <li> 8 bits for the constant in the key schedule
    (4 bits * 2 rounds = 8). </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(1,2,4,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,8],[2,15360],[3,608],[4,64],[5,2048],[9,1920],[16,120]]
maxima> mul_map(epoly) := block([e:poly2nat(epoly,2)], 
  [epoly,[[2,'m(e,2)],[9,'m(e,9)],[16,'m(e,16)]]])$
maxima> ncl_list_ss_gen(1,4,2,4,ss_mixcolumns_matrix(2,4,2),[[2,'s2],[9,'s9],[16,'s16]],create_list(mul_map(p),p,[x,x+1,x^3+1,x^3+x+1,x^3+x^2+1,x^3+x^2+x]),false,aes_mc_bidirectional);
[[1,4],
 [2,12*s2+8*'m(14,2)+8*'m(13,2)+8*'m(11,2)+8*'m(9,2)+8*'m(3,2)+8*'m(2,2)],
 [3,368],[4,32],[5,1024],
 [9,12*s9+8*'m(14,9)+8*'m(13,9)+8*'m(11,9)+8*'m(9,9)+8*'m(3,9)+8*'m(2,9)],
 [16,12*s16+8*'m(14,16)+8*'m(13,16)+8*'m(11,16)+8*'m(9,16)+8*'m(3,16)+8*'m(2,16)]]
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 8 unit-clauses for the 4-bit constants in the Key schedule
    (4 bits * 2 rounds = 8). </li>
    <li> 15360 binary clauses, coming from 24 Sboxes and 16 of each of the six
    multiplications (120 * 128 = 7680). </li>
    <li> 608 ternary clauses, coming from 152 additions of arity two
    (152 * 4 clauses per xor = 624). </li>
    <li> 64 clauses of length four, coming from 8 additions of arity three
    (8 * 8 clauses per xor = 32). </li>
    <li> 2048 clauses of length 5, coming from 128 additions of arity 4
    from the diffusion operation (128 * 16 = 2048). </li>
    <li> 1920 clauses of length 9, coming from 24 Sboxes and 16 of each of
    the six multiplications (120 * 16 = 1920). </li>
    <li> 120 clauses of length sixteen, coming from from 24 Sboxes and 16 of 
    each of the six multiplications (120 * 1 = 60). </li>
   </ul>
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
