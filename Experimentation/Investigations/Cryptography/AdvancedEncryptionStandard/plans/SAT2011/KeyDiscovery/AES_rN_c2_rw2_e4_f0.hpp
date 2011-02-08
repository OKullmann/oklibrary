// Matthew Gwynne, 23.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/AES_rN_c2_rw2_e4_f0.hpp
  \brief Investigations into small scale AES key discovery for N round AES with a 2x2 block and 4-bit field elements and including MixColumns (0 < N <= 10)


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   n round small scale AES with two columns and two rows, including the 
   MixColumns operation (no special final round). </li>
   <li> The AES encryption scheme we model takes a 16-bit plaintext,
   16-bit key and applies the following operations:
   <ol>
    <li> Key schedule which takes the key and generates n+1 16-bit round 
    keys. </li>
    <li> Addition of first round key (input key) to plaintext. </li>
    <li> Round (iterated n times):
    <ol>
     <li> Application of SubBytes (Sbox to each byte) operation. </li>
     <li> Application of ShiftRows operation. </li>
     <li> Application of MixColumns operation. </li>
     <li> Addition of (n+1)-th round key (from key schedule), resulting in the
     ciphertext. </li>
    </ol>
    </li>
   </ol>
   </li>
   <li> Note we have the following number of full rounds, special rounds,
   sboxes in the rounds, additions in the rounds, multiplications by each 
   field element, sboxes in the key expansion, additions in the key expansion 
   and constants in the key expansion:
   \verbatim
> for r : 1 thru 10 do print(component_statistics_ss(r,2,2,4,false,aes_mc_bidirectional));
[1,0,4,64,[[x,8],[x+1,8]],2,16,4] 
[2,0,8,112,[[x,16],[x+1,16]],4,32,8] 
[3,0,12,160,[[x,24],[x+1,24]],6,48,12] 
[4,0,16,208,[[x,32],[x+1,32]],8,64,16] 
[5,0,20,256,[[x,40],[x+1,40]],10,80,20] 
[6,0,24,304,[[x,48],[x+1,48]],12,96,24] 
[7,0,28,352,[[x,56],[x+1,56]],14,112,28] 
[8,0,32,400,[[x,64],[x+1,64]],16,128,32] 
[9,0,36,448,[[x,72],[x+1,72]],18,144,36] 
[10,0,40,496,[[x,80],[x+1,80]],20,160,40] 
> for r : 1 thru 10 do print(component_statistics_ss(r,2,2,4,false,aes_mc_forward));
[1,0,4,48,[[x,4],[x+1,4]],2,16,4] 
[2,0,8,80,[[x,8],[x+1,8]],4,32,8] 
[3,0,12,112,[[x,12],[x+1,12]],6,48,12] 
[4,0,16,144,[[x,16],[x+1,16]],8,64,16] 
[5,0,20,176,[[x,20],[x+1,20]],10,80,20] 
[6,0,24,208,[[x,24],[x+1,24]],12,96,24] 
[7,0,28,240,[[x,28],[x+1,28]],14,112,28] 
[8,0,32,272,[[x,32],[x+1,32]],16,128,32] 
[9,0,36,304,[[x,36],[x+1,36]],18,144,36] 
[10,0,40,336,[[x,40],[x+1,40]],20,160,40]
   \endverbatim
   Note that the inverse MixColumns uses the same multiplication
   constants as the MixColumns.
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating small scale AES for 1-10 rounds (with MixColumns):
   \verbatim
num_columns : 2$
num_rows : 2$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
for i : 1 thru 10 do output_ss_fcl_std(i, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c2_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
540 3530 10196 0 10196 541 1
 length count
1 4
2 2816
3 304
4 32
9 352
16 22
   \endverbatim
   </li>
   <li> We have the following statistics (computed):
   \verbatim
maxima> for r : 1 thru 10 do print(ncl_list_ss(r,2,2,4,false,aes_ts_box,aes_mc_bidirectional));
[[1,4],[2,2816],[3,304],[4,32],[9,352],[16,22]] 
[[1,8],[2,5632],[3,544],[4,64],[9,704],[16,44]] 
[[1,12],[2,8448],[3,784],[4,96],[9,1056],[16,66]] 
[[1,16],[2,11264],[3,1024],[4,128],[9,1408],[16,88]] 
[[1,20],[2,14080],[3,1264],[4,160],[9,1760],[16,110]] 
[[1,24],[2,16896],[3,1504],[4,192],[9,2112],[16,132]] 
[[1,28],[2,19712],[3,1744],[4,224],[9,2464],[16,154]] 
[[1,32],[2,22528],[3,1984],[4,256],[9,2816],[16,176]] 
[[1,36],[2,25344],[3,2224],[4,288],[9,3168],[16,198]] 
[[1,40],[2,28160],[3,2464],[4,320],[9,3520],[16,220]] 
   \endverbatim
   </li>
   <li> Then we can generate random assignments with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> for i : 1 thru 10 do output_ss_random_pc_pair(seed,i,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignments with the translations:
   \verbatim
shell> for i in `seq 1 10`; do $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/merge_cnf.sh ssaes_r${i}_c2_rw2_e4_f0.cnf ssaes_pkpair_r${i}_c2_rw2_e4_f0_s1.cnf > r${i}_keyfind.cnf; done
   \endverbatim
   </li>
  </ul>
  

  \todo One round using the canonical translation
  <ul>
   <li> OKsolver solves this with very few decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r1_keyfind.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           540
c initial_number_of_clauses             3562
c initial_number_of_literal_occurrences 10228
c number_of_initial_unit-eliminations   36
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           36
c reddiff_number_of_clauses             116
c reddiff_number_of_literal_occurrences 372
c number_of_2-clauses_after_reduction   2880
c running_time(sec)                     0.0
c number_of_nodes                       5
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                37
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        4
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             r1_keyfind.cnf
   \endverbatim
   </li>
   <li> However, minisat2 and glucose need to branch a lot more:
   \verbatim
shell> minisat2 r1_keyfind.cnf 
<snip>
restarts              : 3
conflicts             : 333            (4757 /sec)
decisions             : 469            (1.28 % random) (6700 /sec)
propagations          : 36914          (527343 /sec)
conflict literals     : 8156           (21.96 % deleted)
Memory used           : 15.19 MB
CPU time              : 0.07 s
shell> minisat-2.2.0 r1_keyfind.cnf
restarts              : 3
conflicts             : 220            (22000 /sec)
decisions             : 366            (0.00 % random) (36600 /sec)
propagations          : 17092          (1709200 /sec)
conflict literals     : 3314           (24.44 % deleted)
Memory used           : 19.00 MB
CPU time              : 0.01 s
shell> glucose r1_keyfind.cnf 
<snip>
c restarts              : 2
c nb ReduceDB           : 0
c nb learnts DL2        : 29
c nb learnts size 2     : 2
c nb learnts size 1     : 0
c conflicts             : 809            (80900 /sec)
c decisions             : 1335           (1.57 % random) (133500 /sec)
c propagations          : 74563          (7456300 /sec)
c conflict literals     : 18931          (29.29 % deleted)
c Memory used           : 2.14 MB
c CPU time              : 0.01 s
   \endverbatim
   </li>
   <li> Again the OKsolver has far fewer decision nodes, but it's not
   clear how useful this is. </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r1_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 1 2 2 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>


  \todo Two rounds using the canonical translation
  <ul>
   <li> OKsolver solves this with very few decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r2_keyfind.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           1032
c initial_number_of_clauses             7028
c initial_number_of_literal_occurrences 20232
c number_of_initial_unit-eliminations   40
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           40
c reddiff_number_of_clauses             136
c reddiff_number_of_literal_occurrences 456
c number_of_2-clauses_after_reduction   5696
c running_time(sec)                     0.2
c number_of_nodes                       19
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                105
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        8
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             r2_keyfind.cnf
   \endverbatim
   </li>
   <li> However, minisat2 and glucose need to branch a lot more:
   \verbatim
shell> minisat2 r2_keyfind.cnf 
<snip>
restarts              : 9
conflicts             : 6420           (3309 /sec)
decisions             : 8436           (1.49 % random) (4348 /sec)
propagations          : 1168590        (602366 /sec)
conflict literals     : 180261         (41.05 % deleted)
Memory used           : 16.23 MB
CPU time              : 1.94 s
shell> minisat-2.2.0 r2_keyfind.cnf
<snip>
restarts              : 10
conflicts             : 1478           (29560 /sec)
decisions             : 2127           (0.00 % random) (42540 /sec)
propagations          : 311508         (6230160 /sec)
conflict literals     : 53001          (26.51 % deleted)
Memory used           : 19.00 MB
CPU time              : 0.05 s
shell> glucose r2_keyfind.cnf 
<snip>
c restarts              : 3
c nb ReduceDB           : 0
c nb learnts DL2        : 41
c nb learnts size 2     : 2
c nb learnts size 1     : 0
c conflicts             : 3535           (22094 /sec)
c decisions             : 5241           (1.45 % random) (32756 /sec)
c propagations          : 635324         (3970775 /sec)
c conflict literals     : 89260          (32.25 % deleted)
c Memory used           : 2.92 MB
c CPU time              : 0.16 s
   \endverbatim
   </li>
   <li> Again the OKsolver has far fewer decision nodes, but it's not
   clear how useful this is. </li>
   <li> Also note that minisat-2.2.0 does a lot better here than
   minisat2. </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r2_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 2 2 2 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>


  \todo Four rounds using the canonical translation
  <ul>
   <li> OKsolver solves this with very few decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r1_keyfind.cnf
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


  \todo Using the rbase translation
  <ul>
   <li> Generating small scale AES for 1-10 rounds (with MixColumns):
   \verbatim
num_columns : 2$
num_rows : 2$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
for i : 1 thru 10 do output_ss_fcl_std(i, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c2_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
188 774 2452 0 2452 189 1
 length count
1 4
2 48
3 536
4 186
   \endverbatim
   </li>
   <li> We have the following statistics (computed):
   \verbatim
maxima> for r : 1 thru 10 do print(ncl_list_ss(r,2,2,4,false,aes_rbase_box,aes_mc_bidirectional));
[[1,4],[2,48],[3,536],[4,186]] 
[[1,8],[2,96],[3,1008],[4,372]] 
[[1,12],[2,144],[3,1480],[4,558]] 
[[1,16],[2,192],[3,1952],[4,744]] 
[[1,20],[2,240],[3,2424],[4,930]] 
[[1,24],[2,288],[3,2896],[4,1116]] 
[[1,28],[2,336],[3,3368],[4,1302]] 
[[1,32],[2,384],[3,3840],[4,1488]] 
[[1,36],[2,432],[3,4312],[4,1674]] 
[[1,40],[2,480],[3,4784],[4,1860]]
   \endverbatim
   </li>
   <li> Then we can generate random assignments with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> for i : 1 thru 10 do output_ss_random_pc_pair(seed,i,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignments with the translations:
   \verbatim
shell> for i in `seq 1 10`; do $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/merge_cnf.sh ssaes_r${i}_c2_rw2_e4_f0.cnf ssaes_pkpair_r${i}_c2_rw2_e4_f0_s1.cnf > r${i}_keyfind.cnf; done
   \endverbatim
   </li>
  </ul>


  \todo r_1 base translation and bidirectional MixColumns (One round)
  <ul>
   <li> Generating simplest small scale AES for 1 round (with MixColumns):
   \verbatim
num_rounds : 1$
num_columns : 2$
num_rows : 2$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c2_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
188 774 2452 0 2452 189 1
 length count
1 4
2 48
3 536
4 186
   \endverbatim
   </li>
   <li> The measured statistics match up to the computed statistics:
   \verbatim
maxima> ncl_list_ss(1,2,2,4,false,aes_rbase_box,aes_mc_bidirectional);
[[1,4],[2,48],[3,536],[4,186]]
   \endverbatim
   </li>
   <li> Then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/merge_cnf.sh ssaes_r1_c2_rw2_e4_f0.cnf ssaes_pkpair_r1_c2_rw2_e4_f0_s1.cnf > r1_keyfind.cnf
   \endverbatim
   </li>
   <li> OKsolver:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r1_keyfind.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         4
c initial_number_of_variables           188
c initial_number_of_clauses             806
c initial_number_of_literal_occurrences 2484
c number_of_initial_unit-eliminations   36
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           36
c reddiff_number_of_clauses             116
c reddiff_number_of_literal_occurrences 372
c number_of_2-clauses_after_reduction   112
c running_time(sec)                     0.0
c number_of_nodes                       32
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                54
c number_of_pure_literals               0
c number_of_autarkies                   1
c number_of_missed_single_nodes         0
c max_tree_depth                        7
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             r1_keyfind.cnf
   \endverbatim
   </li>
   <li> However, minisat2, minisat2.2 and glucose need a considerable number
   of decisions (given the key is only 16-bit):
   \verbatim
shell> minisat2 r1_keyfind.cnf 
<snip>
restarts              : 2
conflicts             : 242            (13447 /sec)
decisions             : 262            (1.91 % random) (14558 /sec)
propagations          : 9647           (536034 /sec)
conflict literals     : 1622           (34.78 % deleted)
Memory used           : 4.11 MB
CPU time              : 0.017997 s
SATISFIABLE
shell> minisat-2.2.0 r1_keyfind.cnf
restarts              : 3
conflicts             : 264            (66017 /sec)
decisions             : 287            (0.00 % random) (71768 /sec)
propagations          : 11341          (2835959 /sec)
conflict literals     : 1920           (35.00 % deleted)
Memory used           : 8.00 MB
CPU time              : 0.003999 s
shell> glucose r1_keyfind.cnf 
<snip>
c restarts              : 1
c nb ReduceDB           : 0
c nb learnts DL2        : 19
c nb learnts size 2     : 9
c nb learnts size 1     : 1
c conflicts             : 317            (79270 /sec)
c decisions             : 341            (1.17 % random) (85271 /sec)
c propagations          : 18670          (4668667 /sec)
c conflict literals     : 2408           (31.26 % deleted)
c Memory used           : 1.87 MB
c CPU time              : 0.003999 s
   \endverbatim
   </li>
   <li> Cryptominisat still again uses a lot of decisions:
   \verbatim
shell> cryptominisat r1_keyfind.cnf
c restarts                 : 3
c dynamic restarts         : 0
c static restarts          : 3
c full restarts            : 0
c total simplify time      : 0.00
c learnts DL2              : 0
c learnts size 2           : 120
c learnts size 1           : 37          (19.68     % of vars)
c filedLit time            : 0.00        (16.67     % time)
c v-elim SatELite          : 8           (4.26      % vars)
c SatELite time            : 0.00        (16.67     % time)
c v-elim xor               : 0           (0.00      % vars)
c xor elim time            : 0.00        (0.00      % time)
c num binary xor trees     : 44
c binxor trees' crown      : 56          (1.27      leafs/tree)
c bin xor find time        : 0.00
c OTF clause improved      : 32          (0.11      clauses/conflict)
c OTF impr. size diff      : 35          (1.09       lits/clause)
c OTF cl watch-shrink      : 52          (0.18      clauses/conflict)
c OTF cl watch-sh-lit      : 58          (1.12       lits/clause)
c tried to recurMin cls    : 89          (30.58      % of conflicts)
c updated cache            : 0           (0.00       lits/tried recurMin)
c clauses over max glue    : 0           (0.00      % of all clauses)
c conflicts                : 291         (48508.08  / sec)
c decisions                : 315         (0.00      % random)
c bogo-props               : 341817      (56978996.50 / sec)
c conflict literals        : 1975        (30.09     % deleted)
c Memory used              : 13.31       MB
c CPU time                 : 0.01        s
   \endverbatim
   Doesn't this solver do some non-trivial reductions? Apparently they don't 
   help here?
   </li>
   <li> precosat:
   \verbatim
shell> precosat236 r1_keyfind.cnf
c 184 conflicts, 217 decisions, 1 random
c 6 iterations, 1 restarts, 0 skipped
c 0 enlarged, 6 shrunken, 1 rescored, 0 rebiased
c 1 simplifications, 0 reductions
c
c vars: 88 fixed, 60 equiv, 8 elim, 70 merged
c elim: 2170 resolutions, 1 phases, 1 rounds
c sbst: 100% substituted, 0.0% nots, 0.0% ands, 100.0% xors, 0.0% ites
c arty: 0.00 and 2.00 xor average arity
c prbe: 240 probed, 1 phases, 1 rounds
c prbe: 0 failed, 0 lifted, 0 merged
c sccs: 0 non trivial, 0 fixed, 0 merged
c hash: 0 units, 70 merged
c mins: 1034 learned, 23% deleted, 1 strong, 8 depth
c subs: 0 forward, 0 backward, 171 doms
c strs: 0 forward, 0 backward
c doms: 179 dominators, 76 high, 39 low
c prps: 5803 propagations, 1.93 megaprops
c
c 0.0 seconds, 0 MB max, 0 MB recycled
shell> precosat-570.1 r1_keyfind.cnf
c 211 conflicts, 221 decisions, 0 random
c 1 iterations, 0 restarts, 0 skipped
c 0 enlarged, 0 shrunken, 1 rescored, 0 rebiased
c 1 simplifications, 0 reductions
c 
c arty: 0.00 ands 2.00 xors average arity
c autk: 0 autarkies of 0.0 avg size
c autk: dhs 0 0 0 0 0 0
c back: 206 track with 0.2 avg cuts, 8 jumps of 2.4 avg len
c blkd: 2700 resolutions, 1 phases, 1 rounds
c blkd: 0 = 0 implicit + 0 explicit
c clss: 272 recycled, 0 pure, 0 autark
c doms: 114 dominators, 5 high, 75 low
c elim: 2530 resolutions, 1 phases, 1 rounds
c extd: 9 forced, 19 assumed, 0 flipped
c glue: 3.29 original glue, 0.369 slimmed on average
c mins: 1406 lrnd, 32% del, 0 strng, 0 inv, 8 dpth
c otfs: dynamic 6 = 1 bin + 1 trn + 4 large
c otfs: static 0 = 0 bin + 0 trn + 0 large
c prbe: 108 probed, 1 phases, 1 rounds
c prbe: 0 failed, 0 lifted, 0 merged
c prps: 6315 srch props, 0.00 megaprops per second
c pure: 0 = 0 explicit + 0 elim + 0 blkd + 0 autark
c sbst: 100% subst, 71.4% nots, 0.0% ands, 28.6% xors, 0.0% ites
c sccs: 24 non trivial, 0 fixed, 36 merged
c strs: 0 forward, 0 backward, 0 dynamic, 0 org, 0 asym
c subs: 0 fw, 0 bw, 0 dynamic, 0 org, 36 doms, 0 gc
c time: 0.0 = 0.0 srch (0%) + 0.0 simp (0%) + 0.0 othr (0%)
c vars: 37 fxd, 36 eq, 28 elim, 0 pure, 0 zmbs, 0 autk
c zmbs: 0 = 0 explicit + 0 elim + 0 blkd + 0 autark
c
c 0.0 seconds, 0 MB max, 0 MB recycled
   \endverbatim
   </li>
   <li> GRASP:
   \verbatim
        Done searching.... SATISFIABLE INSTANCE. Elapsed time: 0.015


Output Statistics:

  Initial number of variables                                        188
  Initial number of clauses                                          806
  Initial number of literals                                        2484

  Final number of clauses                                            964
  Final number of literals                                          5773

  Total number of implied assignments                               3775

  Total number of UIPs                                               105
  Maximum number of UIPs                                               3

  Total number of conflicting clauses                                158
  Largest conflicting clause                                          38
  Smallest conflicting clause                                          3
  Conflicting clause average size                                     20

  Total number of conflicts                                           56
  Number of conflict clauses in conflicts                             60

  Number of pure literals                                              0

  Total number of backtracks                                          53
  Number of non-chronological backtracks                               9
  Highest non-chronological jump                                       5
  Number of conflicts diagnosed                                      109

  Total number of decisions                                           74
  Largest depth in decision tree                                      14
   \endverbatim
   </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r1_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 1 2 2 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>


  \todo r_1 base translation and bidirectional MixColumns (Two rounds)
  <ul>
   <li> Generating simplest small scale AES for 2 round (with MixColumns):
   \verbatim
num_rounds : 2$
num_columns : 2$
num_rows : 2$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$
   \endverbatim
   <li> The measured statistics match up to the computed statistics:
   \verbatim
maxima> ncl_list_ss(1,2,2,4,false,aes_rbase_box,aes_mc_bidirectional);
[[1,4],[2,48],[3,536],[4,186]]
   \endverbatim
   </li>
   <li> Then we can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/merge_cnf.sh ssaes_r2_c2_rw2_e4_f0.cnf ssaes_pkpair_r2_c2_rw2_e4_f0_s1.cnf > r2_keyfind.cnf
   \endverbatim
   </li>
   <li> OKsolver:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r2_keyfind.cnf
c number_of_initial_unit-eliminations   40
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           40
c reddiff_number_of_clauses             136
c reddiff_number_of_literal_occurrences 456
c number_of_2-clauses_after_reduction   160
c running_time(sec)                     0.3
c number_of_nodes                       2095
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                3119
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         1
c max_tree_depth                        16
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
   \endverbatim
   </li>
   <li> However, minisat2, minisat2.2 and glucose need a considerable number
   of decisions (given the key is only 16-bit):
   \verbatim
shell> minisat2 r2_keyfind.cnf 
<snip>
restarts              : 6
conflicts             : 1746           (10846 /sec)
decisions             : 2014           (1.69 % random) (12511 /sec)
propagations          : 104483         (649064 /sec)
conflict literals     : 21421          (22.93 % deleted)
Memory used           : 4.23 MB
CPU time              : 0.160975 s
SATISFIABLE
shell> minisat-2.2.0 r2_keyfind.cnf
restarts              : 77
conflicts             : 21426          (61934 /sec)
decisions             : 22752          (0.00 % random) (65767 /sec)
propagations          : 1467934        (4243234 /sec)
conflict literals     : 251650         (31.90 % deleted)
Memory used           : 8.00 MB
CPU time              : 0.345947 s
shell> glucose r2_keyfind.cnf 
<snip>
c restarts              : 1
c nb ReduceDB           : 1
c nb learnts DL2        : 46
c nb learnts size 2     : 2
c nb learnts size 1     : 0
c conflicts             : 5573           (31670 /sec)
c decisions             : 5903           (1.68 % random) (33545 /sec)
c propagations          : 468677         (2663346 /sec)
c conflict literals     : 74747          (28.57 % deleted)
c Memory used           : 2.51 MB
c CPU time              : 0.175973 s
   \endverbatim
   Interestly, minisat-2.2.0 gets a lot worse here compared to similar 
   solvers?
   </li>
   <li> Cryptominisat still again uses a lot of decisions:
   \verbatim
shell> cryptominisat r2_keyfind.cnf
c restarts                 : 28
c dynamic restarts         : 22
c static restarts          : 6
c full restarts            : 0
c total simplify time      : 0.00
c learnts DL2              : 0
c learnts size 2           : 160
c learnts size 1           : 40          (12.20     % of vars)
c filedLit time            : 0.00        (0.44      % time)
c v-elim SatELite          : 16          (4.88      % vars)
c SatELite time            : 0.00        (0.00      % time)
c v-elim xor               : 0           (0.00      % vars)
c xor elim time            : 0.00        (0.00      % time)
c num binary xor trees     : 68
c binxor trees' crown      : 80          (1.18      leafs/tree)
c bin xor find time        : 0.00
c OTF clause improved      : 384         (0.05      clauses/conflict)
c OTF impr. size diff      : 396         (1.03       lits/clause)
c OTF cl watch-shrink      : 1685        (0.24      clauses/conflict)
c OTF cl watch-sh-lit      : 2189        (1.30       lits/clause)
c tried to recurMin cls    : 1161        (16.57      % of conflicts)
c updated cache            : 0           (0.00       lits/tried recurMin)
c clauses over max glue    : 0           (0.00      % of all clauses)
c conflicts                : 7006        (30598.56  / sec)
c decisions                : 7560        (0.00      % random)
c bogo-props               : 34158593    (149186963.07 / sec)
c conflict literals        : 88102       (28.60     % deleted)
c Memory used              : 13.87       MB
c CPU time                 : 0.23        s
   \endverbatim
   Doesn't this solver do some non-trivial reductions? Apparently they don't 
   help here?
   </li>
   <li> precosat:
   \verbatim
shell> precosat236 r1_keyfind.cnf
c 1726 conflicts, 1842 decisions, 1 random
c 0 iterations, 0 restarts, 11 skipped
c 2 enlarged, 0 shrunken, 16 rescored, 1 rebiased
c 1 simplifications, 0 reductions
c
c vars: 40 fixed, 80 equiv, 16 elim, 80 merged
c elim: 4572 resolutions, 1 phases, 1 rounds
c sbst: 100% substituted, 0.0% nots, 0.0% ands, 100.0% xors, 0.0% ites
c arty: 0.00 and 2.00 xor average arity
c prbe: 464 probed, 1 phases, 1 rounds
c prbe: 0 failed, 0 lifted, 0 merged
c sccs: 0 non trivial, 0 fixed, 0 merged
c hash: 0 units, 80 merged
c mins: 21224 learned, 27% deleted, 1 strong, 12 depth
c subs: 0 forward, 0 backward, 64 doms
c strs: 0 forward, 0 backward
c doms: 64 dominators, 0 high, 0 low
c prps: 120422 propagations, 2.94 megaprops
c
c 0.0 seconds, 0 MB max, 0 MB recycled
shell> precosat-570.1 r2_keyfind.cnf
c 6349 conflicts, 7074 decisions, 1 random
c 2 iterations, 6 restarts, 107 skipped
c 0 enlarged, 1 shrunken, 61 rescored, 5 rebiased
c 3 simplifications, 0 reductions
c 
c arty: 0.00 ands 2.00 xors average arity
c autk: 0 autarkies of 0.0 avg size
c autk: dhs 0 0 0 0 0 0
c back: 6301 track with 0.0 avg cuts, 424 jumps of 2.6 avg len
c blkd: 5736 resolutions, 1 phases, 1 rounds
c blkd: 0 = 0 implicit + 0 explicit
c clss: 336 recycled, 0 pure, 0 autark
c doms: 182 dominators, 0 high, 148 low
c elim: 4932 resolutions, 1 phases, 1 rounds
c extd: 15 forced, 21 assumed, 0 flipped
c glue: 5.36 original glue, 0.204 slimmed on average
c mins: 62710 lrnd, 34% del, 0 strng, 0 inv, 21 dpth
c otfs: dynamic 50 = 2 bin + 2 trn + 46 large
c otfs: static 0 = 0 bin + 0 trn + 0 large
c prbe: 202 probed, 1 phases, 1 rounds
c prbe: 0 failed, 0 lifted, 0 merged
c prps: 341272 srch props, 2.44 megaprops per second
c pure: 0 = 0 explicit + 0 elim + 0 blkd + 0 autark
c sbst: 100% subst, 55.6% nots, 0.0% ands, 44.4% xors, 0.0% ites
c sccs: 49 non trivial, 0 fixed, 61 merged
c strs: 0 forward, 0 backward, 0 dynamic, 0 org, 0 asym
c subs: 0 fw, 0 bw, 0 dynamic, 0 org, 40 doms, 0 gc
c time: 0.2 = 0.1 srch (87%) + 0.0 simp (6%) + 0.0 othr (6%)
c vars: 42 fxd, 61 eq, 36 elim, 0 pure, 0 zmbs, 0 autk
c zmbs: 0 = 0 explicit + 0 elim + 0 blkd + 0 autark
c 
c 0.2 seconds, 1 MB max, 0 MB recycled
   \endverbatim
   </li>
   <li> GRASP:
   \verbatim
        Done searching.... SATISFIABLE INSTANCE. Elapsed time: 50.659


Output Statistics:

  Initial number of variables                                        328
  Initial number of clauses                                         1516
  Initial number of literals                                        4744

  Final number of clauses                                          13616
  Final number of literals                                        476569

  Total number of implied assignments                             401587

  Total number of UIPs                                              7981
  Maximum number of UIPs                                               3

  Total number of conflicting clauses                              12100
  Largest conflicting clause                                          82
  Smallest conflicting clause                                          3
  Conflicting clause average size                                     38

  Total number of conflicts                                         4199
  Number of conflict clauses in conflicts                           4759

  Number of pure literals                                              0

  Total number of backtracks                                        4119
  Number of non-chronological backtracks                             269
  Highest non-chronological jump                                       5
  Number of conflicts diagnosed                                     8318

  Total number of decisions                                         4448
  Largest depth in decision tree                                      27
   \endverbatim
   This is a considerable amount of time here, especially considering the
   performance of other solvers!!
   </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r2_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 2 2 2 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>

*/
