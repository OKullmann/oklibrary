// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_2_4/2_13.hpp
  \brief Investigations into small scale AES key discovery for 2 + 1/3 round AES with a 2x2 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   2 + 1/3 round small scale AES with two rows, two columns, using the 4-bit
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
    <li> Round (iterated twice):
    <ol>
     <li> Addition of round key (n-1). </li>
     <li> Application of SubBytes (Sbox to each 4-bit element) operation. </li>
     <li> Application of linear diffusion operation. </li>
    </ol>
    </li>
    <li> Addition of round key 2 yielding the ciphertext. </li>
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


  \todo Using the canonical box translation
  <ul>
   <li> Generating small scale AES for 2 + 1/3 rounds:
   \verbatim
rounds : 2$
num_columns : 2$
num_rows : 2$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r2_c2_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG  n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1032 6996 20200 0 20200 1033 1
 length count
1 8
2 5632
3 544
4 64
9 704
16 44
   \endverbatim
   </li>
   <li> In this translation, we have:
   <ul>
    <li> Two full rounds (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 8 Sboxes in the SubBytes operation
    (2 rows * 2 columns * 2 rounds = 8). </li>
    <li> 112 additions within the round and key additions, coming from:
     <ul>
      <li> 48 additions from key additions 
      (3 round keys * 16-bit additions = 48). </li>
      <li> 64 additions from the matrix multiplication in the diffusion 
      operation (2 rows * 2 columns * 2 directions * 4 bits * 2 rounds = 64).
      </li>
     </ul>
    </li>
    <li> 16 multiplications by 02 from the MixColumns operation
    (2 rows * 2 columns * 2 directions * 2 rounds = 16). </li>
    <li> 16 multiplications by 03 from the MixColumns operation
    (2 rows * 2 columns * 2 directions * 2 rounds = 16). </li>
    <li> 4 Sboxes in the AES key schedule
    (2 rows * 2 rounds = 4). </li>
    <li> 32 additions in the key schedule:
    <ul>
     <li> 8 additions of arity three
     (1 row * 1 column * 4 bits * 2 rounds = 8). </li>
     <li> 24 additions of arity two 
     ((1 rows * 2 columns + 1 row * 1 column) * 4 bits * 2 rounds = 24). 
     </li>
    </ul>
    </li>
    <li> 8 bits for the constant in the key schedule. 
    (4 bits * 2 rounds = 8). </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(2,2,2,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,8],[2,5632],[3,544],[4,64],[9,704],[16,44]]
maxima> ncl_list_ss_gen(2,2,2,4,ss_mixcolumns_matrix(2,4,2),[[2,'s2],[9,'s9],[16,'s16]],[[x,[[2,'m2_2],[9,'m2_9],[16,'m2_16]]],[x+1,[[2,'m3_2],[9,'m3_9],[16,'m3_16]]]],false,aes_mc_bidirectional);
[[1,8],[2,12*s2+16*m3_2+16*m2_2],[3,544],[4,64],[9,12*s9+16*m3_9+16*m2_9],
        [16,12*s16+16*m3_16+16*m2_16]]
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 8 unit clauses for the 4-bit constant in the key expansion. </li>
    <li> 5632 binary clauses, coming from 12 Sboxes and 16 of each of the two 
    multiplications (44 * 128 = 5632). </li>
    <li> 544 ternary clauses, coming from 136 additions of arity two
    (136 * 4 = 544). </li>
    <li> 64 clauses of length four, coming from 8 additions of arity three
    (8 * 8 = 64). </li>
    <li> 704 clauses of length seven, coming from 12 Sboxes and 16 of each of 
    the two multiplications (44 * 16 = 704). </li>
    <li> 44 clauses of length sixteen, coming from from 12 Sboxes and 16 of 
    each of the two multiplications (44 * 1 = 44). </li>
   </ul>
   </li>
   <li> Then we can generate random assignments with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignments with the translations:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r2_c2_rw2_e4_f0.cnf ssaes_pkpair_r2_c2_rw2_e4_f0_s1.cnf > r2_keyfind.cnf; done
   \endverbatim
   </li>
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
   <li> However, minisat-2.2.0 and glucose need to branch a lot more:
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


  \todo Using the rbase translation
  <ul>
   <li> Generating small scale AES for two rounds:
   \verbatim
rounds : 2$
num_columns : 2$
num_rows : 2$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c2_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
328 1484 4712 0 4712 329 1
 length count
1 8
2 96
3 1008
4 372
   \endverbatim
   </li>
   <li> We have the following statistics (computed):
   \verbatim
maxima> print(ncl_list_ss(2,2,2,4,false,aes_rbase_box,aes_mc_bidirectional));
[[1,8],[2,96],[3,1008],[4,372]] 
   \endverbatim
   </li>
   <li> Then we can generate random assignments with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignments with the translations:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r2_c2_rw2_e4_f0.cnf ssaes_pkpair_r2_c2_rw2_e4_f0_s1.cnf > r2_keyfind.cnf; done
   \endverbatim
   </li>
  </ul>


  \todo r_1 base translation and bidirectional MixColumns
  <ul>
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
   <li> However, minisat2, minisat-2.2.0 and glucose need a considerable
   number of decisions (given the key is only 16-bit):
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
