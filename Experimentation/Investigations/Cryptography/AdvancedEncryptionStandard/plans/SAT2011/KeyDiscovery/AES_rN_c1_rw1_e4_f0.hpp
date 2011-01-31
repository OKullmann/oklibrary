// Matthew Gwynne, 31.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/AES_rN_c1_rw1_e4_f0.hpp
  \brief Investigations into simplest small scale AES key discovery for N round AES with MixColumns (for N=1-10)


  \todo Add full description of round
  <ul>
   <li> We need a full description of what a "round" here entails, 
   including statistics etc. </li>
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
   \endverbatim
   and then we can generate a random assignment with the plaintext and 
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
   and then we can generate a random assignment with the plaintext and 
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
