// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_2_4/1_13.hpp
  \brief Investigations into small scale AES key discovery for one round AES with a 2x2 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   1 + 1/3 round small scale AES with two rows, two columns, using the 4-bit
   field size. </li>
   <li> The AES encryption scheme we model takes a 16-bit plaintext and
   16-bit key and outputs a 16-bit ciphertext. The plaintext, key and 
   ciphertext are all considered, column by column, as 2x2 matrices of 4-bit 
   elements. </li>
   <li> In other words, in the AES blocks (plaintext, key, ciphertext etc), 
   the 4-bit element at position (i,j) in the matrix is the ((i-1)*2 + j)-th 
   4-bit word of the 16-bits. </li>
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
 matrix([x+1,x],[x,x+1])
    \endverbatim
    over the 4-bit field. As it is a matrix multiplication, this operation can
    be broken down into a "MixColumn" operation on each column of the input
    matrix. </li>
   </ol>
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating small scale AES for one rounds:
   \verbatim
rounds : 1$
num_columns : 2$
num_rows : 2$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

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
maxima> print(ncl_list_ss(1,2,2,4,false,aes_ts_box,aes_mc_bidirectional));
[[1,4],[2,2816],[3,304],[4,32],[9,352],[16,22]] 
   \endverbatim
   </li>
   <li> Then we can generate random assignments with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignments with the translations:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c2_rw2_e4_f0.cnf ssaes_pkpair_r1_c2_rw2_e4_f0_s1.cnf > r1_keyfind.cnf; done
   \endverbatim
   </li>
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
   <li> However, minisat-2.2.0 and glucose need to branch a lot more:
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


  \todo Using the rbase translation
  <ul>
   <li> Generating small scale AES for 1,...,10 rounds:
   \verbatim
round : 1$
num_columns : 2$
num_rows : 2$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(round, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

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
maxima> print(ncl_list_ss(1,2,2,4,false,aes_rbase_box,aes_mc_bidirectional));
[[1,4],[2,48],[3,536],[4,186]] 
   \endverbatim
   </li>
   <li> Then we can generate random assignments with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,round,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignments with the translations:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c2_rw2_e4_f0.cnf ssaes_pkpair_r1_c2_rw2_e4_f0_s1.cnf > r1_keyfind.cnf; done
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
   <li> However, minisat2, minisat-2.2.0 and glucose need a considerable 
   number of decisions (given the key is only 16-bit):
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

*/
