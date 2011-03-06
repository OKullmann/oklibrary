// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/004/1_1_4/1_13.hpp
  \brief Investigations into simplest small scale AES key discovery for one round AES (1+1/3)


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   1 + 1/3 round small scale AES with one rows, one columns, using the 4-bit
   field size. </li>
   <li> The AES encryption scheme we model takes a 4-bit plaintext and
   4-bit key and outputs a 4-bit ciphertext. 
   </li>
   <li> The 4-bit element (b_0,b_1,b_2,b_3) is considered as the polynomial
   b_0 * x^3 + b_1 * x^2 + b_2 * x + b_3. Addition and multiplication
   on these polynomials is defined as usual, modulo the polynomial x^4+x+1. 
   </li>
   <li> The encryption scheme applies the following operations:
   <ol>
    <li> Addition of round key 0 (input key) to plaintext. </li>
    <li> Application of Sbox operation. </li>
    <li> Addition of round key 1, resulting in the ciphertext. </li>
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
   <li> Generating simplest small scale AES for 1 round:
   \verbatim
num_rounds : 1$
num_columns : 1$
num_rows : 1$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c1_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
68 358 1012 0 1012 69 1
 length count
1 4
2 272
3 48
9 32
16 2
   \endverbatim
   </li>
   <li> In this translation, we have:
   <ul>
    <li> One full round (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 1 Sbox in the SubBytes operation (1 rows * 1 columns = 1). </li>
    <li> 16 additions within the round and key additions, coming from:
     <ul>
      <li> 8 additions of arity 2 from key additions 
      (2 round keys * 4-bit additions = 8). </li>
      <li> 8 additions of arity one from the identity matrix multiplication
      in the diffusion operation 
      (1 rows * 1 columns * 2 directions * 4 bits = 8).
      </li>
     </ul>
    </li>
    <li> 1 Sbox in the AES key schedule (1 rows). </li>
    <li> 4 additions in the key schedule:
    <ul>
     <li> 4 additions of arity three (1 row * 1 column * 4 bits = 4). </li>
    </ul>
    </li>
    <li> 4 bits for the constant in the key schedule. </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(1,1,1,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,4],[2,272],[3,48],[9,32],[16,2]]
maxima> ncl_list_ss_gen(1,1,1,4,ss_mixcolumns_matrix(2,4,1),[[2,'s2],[9,'s9],[16,'s16]],[],false,aes_mc_bidirectional);
[[1,4],[2,2*s2+16],[3,48],[9,2*s9],[16,2*s16]]
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 4 unit clauses for the 4-bit constant in the key expansion. </li>
    <li> 272 binary clauses, coming from 2 Sboxes and 8 additions of arity
    one (2 * 128 + 8 * 2 = 272). </li>
    <li> 48 ternary clauses, coming from 12 additions of arity two
    (12 * 4 = 48). </li>
    <li> 32 clauses of length four, coming from 4 additions of arity three
    (4 * 8 = 32). </li>
    <li> 2 clauses of length sixteen, coming from from 2 Sboxes
    (2 * 1 = 2). </li>
   </ul>
   </li>
   <li> Then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c1_rw1_e4_f0.cnf ssaes_pkpair_r1_c1_rw1_e4_f0_s1.cnf > r1_keyfind.cnf
   \endverbatim
   </li>
   <li> OKsolver solves this with no decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r1_keyfind.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           68
c initial_number_of_clauses             366
c initial_number_of_literal_occurrences 1020
c number_of_initial_unit-eliminations   12
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           12
c reddiff_number_of_clauses             36
c reddiff_number_of_literal_occurrences 108
c number_of_2-clauses_after_reduction   296
c running_time(sec)                     0.0
c number_of_nodes                       1
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                19
c number_of_pure_literals               0
c number_of_autarkies                   1
c number_of_missed_single_nodes         0
c max_tree_depth                        0
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             r1_keyfind.cnf
   \endverbatim
   </li>
   <li> However, minisat-2.2.0 and glucose need to branch:
   \verbatim
shell> minisat-2.2.0 r1_keyfind.cnf
<snip>
restarts              : 1
conflicts             : 7              (inf /sec)
decisions             : 14             (0.00 % random) (inf /sec)
propagations          : 198            (inf /sec)
conflict literals     : 83             (0.00 % deleted)
Memory used           : 18.00 MB
CPU time              : 0 s

shell> minisat2 r1_keyfind.cnf 
<snip>
restarts              : 1
conflicts             : 13             (inf /sec)
decisions             : 21             (0.00 % random) (inf /sec)
propagations          : 286            (inf /sec)
conflict literals     : 115            (5.74 % deleted)
Memory used           : 14.64 MB
CPU time              : 0 s

shell> glucose r1_keyfind.cnf 
<snip>
c restarts              : 1
c nb ReduceDB           : 0
c nb learnts DL2        : 5
c nb learnts size 2     : 0
c nb learnts size 1     : 2
c conflicts             : 12             (inf /sec)
c decisions             : 32             (0.00 % random) (inf /sec)
c propagations          : 215            (inf /sec)
c conflict literals     : 146            (11.52 % deleted)
c Memory used           : 1.88 MB
c CPU time              : 0 s
   \endverbatim
   </li>
   <li> It seems OKsolver is propagating purely by r_2. Perhaps the OKsolver
   is able to take advantage of the r_1-basedness of the canonical translation
   due to it's use of r_2 reductions? </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r1_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 1 1 1 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>

*/
