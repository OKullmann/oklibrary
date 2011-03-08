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


  \todo Remove linear diffusion from translation
  <ul>
   <li> Currently the AES linear diffusion operation creates equality
   clauses in the translation of this AES instance, as the translation
   does not check if the MixColumns matrix is the identity matrix. </li>
   <li> These equivalences clauses can only get in the way for the solvers
   and skew results, and therefore the translation should be updated to
   check for the identity matrix and rename variables instead. </li>
  </ul>


  \todo Comparison of box translations
  <ul>
   <li> All translations are solved by all solvers in less than 0.5s and so
   comparing these translations on solver time is unreasonable. </li>
   <li> The canonical translation does best in terms of number of
   decisions, followed by the r_1-base, followed by the small translation.
   </li>
   <li> Comparing the translations on the number of decisions:
   <ul>
    <li> OKsolver:
     <ul>
      <li> canonical box translation: 1 </li>
      <li> r_1-base box translation: 4 </li>
      <li> small box translation: 6 </li>
     </ul>
    </li>
    <li> minisat-2.2.0:
     <ul>
      <li> canonical box translation: 62 </li>
      <li> r_1-base box translation: 142 </li>
      <li> small box translation: 1225 </li>
     </ul>
    </li>
    <li> minisat2:
     <ul>
      <li> canonical box translation: 56 </li>
      <li> r_1-base box translation: 221 </li>
      <li> small box translation: 634 </li>
     </ul>
    </li>
    <li> glucose:
     <ul>
      <li> canonical box translation: 29 </li>
      <li> r_1-base box translation: 178 </li>
      <li> small box translation: 1652 </li>
     </ul>
    </li>
   </ul>
   </li>
  </ul>


  \todo Using the rbase box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications 
   as whole boxes and translating these boxes using the r_1-base translation.
   </li>
   <li> Generating simplest small scale AES for 20 rounds:
   \verbatim
num_rounds : 20$
num_columns : 1$
num_rows : 1$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
seed : 1$
mc_tran : aes_mc_forward$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r20_c1_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
492 1976 6208 0 6208 493 1
 length count
1 80
2 160
3 1136
4 600
   \endverbatim
   </li>
   <li> In this translation, we have:
   <ul>
    <li> Twenty full rounds (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 20 Sboxes in the SubBytes operation 
    (1 rows * 1 columns * 20 rounds = 10). </li>
    <li> 244 additions within the round and key additions, coming from:
     <ul>
      <li> 84 additions of arity 2 from key additions 
      (21 round keys * 4-bit additions = 84). </li>
      <li> 80 additions of arity one from the identity matrix multiplication
      in the diffusion operation 
      (1 rows * 1 columns * 1 directions * 4 bits * 10 rounds = 80).
      </li>
     </ul>
    </li>
    <li> 20 Sboxes in the AES key schedule 
    (1 rows * 20 rounds = 20). </li>
    <li> 80 additions in the key schedule:
    <ul>
     <li> 80 additions of arity two
     (1 row * 1 column * 4 bits * 20 rounds = 80). </li>
    </ul>
    </li>
    <li> 80 bits for the constant in the key schedule
    (4 bits * 20 rounds = 80).
    </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(20,1,1,4,false,aes_rbase_box,aes_mc_forward);
[[1,80],[2,160],[3,1136],[4,600]]
maxima> ncl_list_ss_gen(20,1,1,4,ss_mixcolumns_matrix(2,4,1),[[3,'s3],[4,'s4]],[],false,aes_mc_forward);
[[1,80],[2,160],[3,40*s3+656],[4,40*s4]]
maxima> ncl_list_fcs(ev_hm(ss_sbox_rbase_cnfs,4));
[[3,12],[4,15]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 80 unit clauses for the 4-bit constant in the key expansion. </li>
    <li> 160 binary clauses, coming from 80 additions of arity one 
    (80 * 2 = 160). </li>
    <li> 1136 ternary clauses, coming from 40 Sboxes and 164 additions of 
    arity two (40 * 12 + 164 * 4 = 1136). </li>
    <li> 600 clauses of length four, coming from 40 Sboxes
    (40 * 15 = 600). </li>
   </ul>
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
   <li> OKsolver solves this with very few decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG experiment_r20_k1.cnf
<snip>
c number_of_initial_unit-eliminations   88
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           88
c reddiff_number_of_clauses             264
c reddiff_number_of_literal_occurrences 792
c number_of_2-clauses_after_reduction   336
c running_time(sec)                     0.0
c number_of_nodes                       4
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                7
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        2
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
   \endverbatim
   </li>
   <li> However, minisat-2.2.0 and glucose need to branch more:
   \verbatim
shell> minisat-2.2.0 experiment_r20_k1.cnf
<snip>
restarts              : 2
conflicts             : 126            (inf /sec)
decisions             : 142            (0.00 % random) (inf /sec)
propagations          : 9029           (inf /sec)
conflict literals     : 612            (35.65 % deleted)
Memory used           : 19.00 MB
CPU time              : 0 s

shell> minisat2 experiment_r20_k1.cnf
<snip>
restarts              : 2
conflicts             : 182            (18200 /sec)
decisions             : 221            (2.26 % random) (22100 /sec)
propagations          : 12249          (1224900 /sec)
conflict literals     : 892            (36.78 % deleted)
Memory used           : 14.89 MB
CPU time              : 0.01 s

shell> glucose experiment_r20_k1.cnf
c restarts              : 1
c nb ReduceDB           : 0
c nb learnts DL2        : 32
c nb learnts size 2     : 4
c nb learnts size 1     : 1
c conflicts             : 142            (inf /sec)
c decisions             : 178            (2.25 % random) (inf /sec)
c propagations          : 10968          (inf /sec)
c conflict literals     : 699            (37.87 % deleted)
c Memory used           : 2.02 MB
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


  \todo Using the small box translation 
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications 
   as whole boxes and translating these boxes using the smallest CNF 
   translations. </li>
   <li> Generating simplest small scale AES for 20 rounds:
   \verbatim
num_rounds : 20$
num_columns : 1$
num_rows : 1$
exp : 4$
final_round_b : false$
box_tran : aes_small_box$
seed : 1$
mc_tran : aes_mc_forward$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r20_c1_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
492 1776 5648 0 5648 493 1
 length count
1 80
2 160
3 976
4 480
5 80
   \endverbatim
   </li>
   <li> In this translation, we have:
   <ul>
    <li> Twenty full rounds (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 20 Sboxes in the SubBytes operation 
    (1 rows * 1 columns * 20 rounds = 10). </li>
    <li> 244 additions within the round and key additions, coming from:
     <ul>
      <li> 84 additions of arity 2 from key additions 
      (21 round keys * 4-bit additions = 84). </li>
      <li> 80 additions of arity one from the identity matrix multiplication
      in the diffusion operation 
      (1 rows * 1 columns * 1 directions * 4 bits * 20 rounds = 80).
      </li>
     </ul>
    </li>
    <li> 20 Sboxes in the AES key schedule 
    (1 rows * 20 rounds = 20). </li>
    <li> 80 additions in the key schedule:
    <ul>
     <li> 80 additions of arity two
     (1 row * 1 column * 4 bits * 20 rounds = 80). </li>
    </ul>
    </li>
    <li> 80 bits for the constant in the key schedule
    (4 bits * 20 rounds = 80).
    </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(20,1,1,4,false,aes_small_box,aes_mc_forward);
[[1,80],[2,160],[3,976],[4,480],[5,80]]
maxima> ncl_list_ss_gen(20,1,1,4,ss_mixcolumns_matrix(2,4,1),[[3,'s3],[4,'s4],[5,'s5]],[],false,aes_mc_forward);
[[1,80],[2,160],[3,40*s3+656],[4,40*s4],[5,40*s5]]
maxima> ncl_list_fcs(ev_hm(ss_sbox_cnfs,4));
[[3,8],[4,12],[5,2]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 80 unit clauses for the 4-bit constant in the key expansion. </li>
    <li> 160 binary clauses, coming from 80 additions of arity one 
    (80 * 2 = 160). </li>
    <li> 1136 ternary clauses, coming from 40 Sboxes and 164 additions of 
    arity two (40 * 12 + 164 * 4 = 1136). </li>
    <li> 480 clauses of length four, coming from 40 Sboxes
    (40 * 12 = 480). </li>
    <li> 80 clauses of length five, coming from 40 Sboxes
    (40 * 2 = 80). </li>
   </ul>
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
   <li> OKsolver solves this with very few decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG experiment_r20_k1.cnf
<snip>
c number_of_initial_unit-eliminations   88
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           88
c reddiff_number_of_clauses             264
c reddiff_number_of_literal_occurrences 792
c number_of_2-clauses_after_reduction   336
c running_time(sec)                     0.0
c number_of_nodes                       6
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                58
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        4
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
   \endverbatim
   </li>
   <li> However, minisat-2.2.0 and glucose need to branch more:
   \verbatim
shell> minisat-2.2.0 experiment_r20_k1.cnf
<snip>
restarts              : 6
conflicts             : 680            (inf /sec)
decisions             : 1225           (0.00 % random) (inf /sec)
propagations          : 24639          (inf /sec)
conflict literals     : 4691           (19.27 % deleted)
Memory used           : 19.00 MB
CPU time              : 0 s

shell> minisat2 experiment_r20_k1.cnf
<snip>
restarts              : 3
conflicts             : 306            (15300 /sec)
decisions             : 634            (1.58 % random) (31700 /sec)
propagations          : 9686           (484300 /sec)
conflict literals     : 1946           (18.44 % deleted)
Memory used           : 14.89 MB
CPU time              : 0.02 s

shell> glucose experiment_r20_k1.cnf
c restarts              : 1
c nb ReduceDB           : 0
c nb learnts DL2        : 236
c nb learnts size 2     : 17
c nb learnts size 1     : 3
c conflicts             : 910            (91000 /sec)
c decisions             : 1652           (1.39 % random) (165200 /sec)
c propagations          : 42103          (4210300 /sec)
c conflict literals     : 5786           (24.28 % deleted)
c Memory used           : 2.01 MB
c CPU time              : 0.01 s
   \endverbatim
   </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O experiment_r20_k1.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 20 1 1 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications 
   as whole boxes and translating these boxes using the canonical translation.
   </li>
   <li> Generating simplest small scale AES for 20 rounds:
   \verbatim
num_rounds : 20$
num_columns : 1$
num_rows : 1$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_forward$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r20_c1_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1132 6696 19008 0 19008 1133 1
 length count
1 80
2 5280
3 656
9 640
16 40
   \endverbatim
   </li>
   <li> In this translation, we have:
   <ul>
    <li> Twenty full rounds (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 20 Sboxes in the SubBytes operation 
    (1 rows * 1 columns * 20 rounds = 10). </li>
    <li> 164 additions within the round and key additions, coming from:
     <ul>
      <li> 84 additions of arity 2 from key additions 
      (21 round keys * 4-bit additions = 84). </li>
      <li> 80 additions of arity one from the identity matrix multiplication
      in the diffusion operation 
      (1 rows * 1 columns * 1 directions * 4 bits * 20 rounds = 80).
      </li>
     </ul>
    </li>
    <li> 20 Sboxes in the AES key schedule 
    (1 rows * 20 rounds = 20). </li>
    <li> 80 additions in the key schedule:
    <ul>
     <li> 80 additions of arity two
     (1 row * 1 column * 4 bits * 20 rounds = 80). </li>
    </ul>
    </li>
    <li> 80 bits for the constant in the key schedule
    (4 bits * 20 rounds = 80).
    </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(20,1,1,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,80],[2,5280],[3,656],[9,640],[16,40]]
maxima> ncl_list_ss_gen(20,1,1,4,ss_mixcolumns_matrix(2,4,1),[[2,'s2],[9,'s9],[16,'s16]],[],false,aes_mc_forward);
[[1,80],[2,40*s2+160],[3,656],[9,40*s9],[16,40*s16]]
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 80 unit clauses for the 4-bit constant in the key expansion. </li>
    <li> 5280 binary clauses, coming from 40 Sboxes and 80 additions of arity
    one (40 * 128 + 80 * 2 = 5280). </li>
    <li> 656 ternary clauses, coming from 164 additions of arity two
    (164 * 4 = 656). </li>
    <li> 640 clauses of length nine, coming from 40 Sboxes
    (40 * 16 = 640). </li>
    <li> 40 clauses of length sixteen, coming from from 40 Sboxes
    (40 * 1 = 40). </li>
   </ul>
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
   <li> OKsolver solves this with one decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG experiment_r20_k1.cnf
<snip>
c number_of_initial_unit-eliminations   88
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           88
c reddiff_number_of_clauses             264
c reddiff_number_of_literal_occurrences 792
c number_of_2-clauses_after_reduction   5456
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
   \endverbatim
   </li>
   <li> However, minisat-2.2.0 and glucose need to branch:
   \verbatim
shell> minisat-2.2.0 experiment_r20_k1.cnf
<snip>
restarts              : 1
conflicts             : 26             (2600 /sec)
decisions             : 62             (0.00 % random) (6200 /sec)
propagations          : 12396          (1239600 /sec)
conflict literals     : 284            (43.65 % deleted)
Memory used           : 19.00 MB
CPU time              : 0.01 s

shell> minisat2 experiment_r20_k1.cnf
<snip>
restarts              : 1
conflicts             : 36             (514 /sec)
decisions             : 56             (0.00 % random) (800 /sec)
propagations          : 17061          (243729 /sec)
conflict literals     : 339            (50.87 % deleted)
Memory used           : 15.61 MB
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
