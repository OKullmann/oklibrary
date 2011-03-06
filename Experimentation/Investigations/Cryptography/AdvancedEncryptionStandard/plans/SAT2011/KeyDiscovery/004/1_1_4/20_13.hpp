// Matthew Gwynne, 18.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/004/1_1_4/20_13.hpp
  \brief Investigations into simplest small scale AES key discovery for 20+1/3 round AES


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   20 + 1/3 round small scale AES with one rows, one columns, using the 4-bit
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
    <li> Key schedule which takes the key and generates twenty-one 4-bit round
    keys. </li>
    <li> Application of the following operation (the "round") twenty times:
     <ol>
      <li> Addition of round key n-1. </li>
      <li> Application of Sbox operation. </li>
     </ol>
    </li>
    <li> Addition of round key n. </li>
    <li> The result of the last round key addition is then the ciphertext. 
    </li>
   </ol>
   </li>
   <li> The Sbox is non-linear permutation over the set of 4-bit elements,
   defined as inversion within the 4-bit field composed with an affine
   transformation. </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating simplest small scale AES for 10 rounds (with MixColumns):
   \verbatim
num_rounds : 20$
num_columns : 1$
num_rows : 1$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r20_c1_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1132 6856 19328 0 19328 1133 1
 length count
1 80
2 5440
3 656
9 640
16 40
   \endverbatim
   </li>
   <li> The measured statistics match up to the computed statistics:
   \verbatim
maxima> ncl_list_ss(20,1,1,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,80],[2,5440],[3,656],[9,640],[16,40]]
   \endverbatim
   </li>
   <li> Then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r20_c1_rw1_e4_f0.cnf ssaes_pkpair_r20_c1_rw1_e4_f0_s1.cnf > experiment_r20_k1.cnf
   \endverbatim
   </li>
   <li> OKsolver solves this with no decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG experiment_r20_k1.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           1132
c initial_number_of_clauses             6864
c initial_number_of_literal_occurrences 19336
c number_of_initial_unit-eliminations   88
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           88
c reddiff_number_of_clauses             264
c reddiff_number_of_literal_occurrences 792
c number_of_2-clauses_after_reduction   5616
c running_time(sec)                     0.0
c number_of_nodes                       1
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                15
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        0
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             experiment_r20_k1.cnf
   \endverbatim
   </li>
   <li> However, minisat-2.2.0 and glucose need to branch:
   \verbatim
shell> minisat-2.2.0 experiment_r20_k1.cnf
<snip>
restarts              : 2
conflicts             : 107            (10700 /sec)
decisions             : 252            (0.00 % random) (25200 /sec)
propagations          : 33586          (3358600 /sec)
conflict literals     : 1149           (32.96 % deleted)
Memory used           : 19.00 MB
CPU time              : 0.01 s

shell> minisat2 experiment_r20_k1.cnf
<snip>
Verified 6192 original clauses.
Verified 888 eliminated clauses.
restarts              : 1
conflicts             : 46             (657 /sec)
decisions             : 108            (0.00 % random) (1543 /sec)
propagations          : 17942          (256314 /sec)
conflict literals     : 553            (27.71 % deleted)
Memory used           : 15.59 MB
CPU time              : 0.07 s

shell> glucose experiment_r20_k1.cnf
c restarts              : 1
c nb ReduceDB           : 0
c nb learnts DL2        : 3
c nb learnts size 2     : 1
c nb learnts size 1     : 1
c conflicts             : 13             (inf /sec)
c decisions             : 29             (0.00 % random) (inf /sec)
c propagations          : 2132           (inf /sec)
c conflict literals     : 152            (39.68 % deleted)
c Memory used           : 2.30 MB
c CPU time              : 0 s
   \endverbatim
   </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O experiment_r20_k1.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 20 1 1 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>

*/
