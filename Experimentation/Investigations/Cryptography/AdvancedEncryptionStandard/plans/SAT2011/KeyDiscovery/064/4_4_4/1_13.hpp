// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/064/4_4_4/1_1.hpp
  \brief Investigations into small scale AES key discovery for one round AES with a 4x4 block and 4-bit field elements (1+1/3)


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   one round small scale AES with four columns, four rows, using the 4-bit
   field size. </li>
   <li> The AES encryption scheme we model takes a 64-bit plaintext,
   64-bit key and applies the following operations:
   <ol>
    <li> Key schedule which takes the key and generates two 64-bit round 
    keys. </li>
    <li> Addition of first round key (input key) to plaintext. </li>
    <li> Application of SubBytes (Sbox to each byte) operation. </li>
    <li> Application of ShiftRows operation. </li>
    <li> Application of MixColumns operation. </li>
    <li> Addition of second round key (from key schedule), resulting in the 
    ciphertext. </li>
   </ol>
   </li>
   <li> Note we have the following number of full rounds, special rounds,
   sboxes in the rounds, multiplications by each field element, sboxes in
   the key expansion, additions in the key expansion and constants in the
   key expansion:
   \verbatim
> component_statistics_ss(1,4,4,4,false,aes_mc_bidirectional);
[1,0,16,256,[[1,32],[x,16],[x+1,16],[x^3+1,16],[x^3+x+1,16],[x^3+x^2+1,16],[x^3+x^2+x,16]],4,64,4]
> component_statistics_ss(1,4,4,4,false,aes_mc_forward);
[1,0,16,192,[[1,32],[x,16],[x+1,16]],4,64,4]
   \endverbatim
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating small scale AES for one round:
   \verbatim
num_rounds : 1$
num_columns : 4$
num_rows : 4$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c4_rw4_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
2708 19656 60884 0 60884 2709 1
 length count
1 4
2 14848
3 752
4 32
5 2048
9 1856
16 116
   \endverbatim
   </li>
   <li> The measured statistics match up to the computed statistics:
   \verbatim
maxima> ncl_list_ss(1,4,4,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,4],[2,14848],[3,752],[4,32],[5,2048],[9,1856],[16,116]]
   \endverbatim
   </li>
   <li> Then we can generate a random assignment with the plaintext and 
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw4_e4_f0.cnf ssaes_pkpair_r1_c4_rw4_e4_f0_s1.cnf > experiment_r1_k1.cnf
   \endverbatim
   </li>
   <li> minisat-2.2.0 takes only 20s:
   \verbatim
restarts              : 435
conflicts             : 159689         (7684 /sec)
decisions             : 191056         (0.00 % random) (9193 /sec)
propagations          : 80837387       (3889809 /sec)
conflict literals     : 7597323        (51.64 % deleted)
Memory used           : 18.00 MB
CPU time              : 20.7818 s
   \endverbatim
   This has been verified by hand.
   </li>
   <li> glucose (apparently) solves this in a fraction of a second:
   \verbatim
shell> glucose experiment_r1_k1.cnf
c restarts              : 566
c nb ReduceDB           : 151
c nb learnts DL2        : 4417
c nb learnts size 2     : 504
c nb learnts size 1     : 2
c conflicts             : 12127207       (92588235 /sec)
c decisions             : 12923545       (1.40 % random) (98668079 /sec)
c propagations          : 5248749924     (40072911315 /sec)
c conflict literals     : 369558529      (73.72 % deleted)
c Memory used           : 112.15 MB
c CPU time              : 0.13098 s
   \endverbatim
   however, re-running glucose does not yield the same behaviour, and there
   are a very large number of restarts in just 0.13s. Does glucose have a bug 
   in it's timing code? Perhaps there is a bug in the experiment run script?
   See "MiniSAT2 based solvers return incorrect times using experiment script"
   in 
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp.
   </li>
   <li> Other solvers such as cryptominisat take longer (but still only a few 
   minutes):
   \verbatim
shell> cryptominisat experiment_r1_k1.cnf
c num threads              : 1
c restarts                 : 569
c dynamic restarts         : 426
c static restarts          : 143
c full restarts            : 6
c total simplify time      : 0.01
c learnts DL2              : 0
c learnts size 2           : 16219
c learnts size 1           : 142         (5.24      % of vars)
c filedLit time            : 0.26        (0.42      % time)
c v-elim SatELite          : 95          (3.51      % vars)
c SatELite time            : 1.44        (2.28      % time)
c v-elim xor               : 0           (0.00      % vars)
c xor elim time            : 0.01        (0.02      % time)
c num binary xor trees     : 727
c binxor trees' crown      : 1571        (2.16      leafs/tree)
c bin xor find time        : 0.01
c OTF clause improved      : 397244      (0.02      clauses/conflict)
c OTF impr. size diff      : 442410      (1.11       lits/clause)
c OTF cl watch-shrink      : 15330847    (0.70      clauses/conflict)
c OTF cl watch-sh-lit      : 131624689   (8.59       lits/clause)
c tried to recurMin cls    : 6843744     (31.05      % of conflicts)
c updated cache            : 54831       (0.01       lits/tried recurMin)
c clauses over max glue    : 0           (0.00      % of all clauses)
c conflicts                : 22042524    (350318.21 / sec)
c decisions                : 24091000    (0.14      % random)
c bogo-props               : 2610735180842 (41491984763.76 / sec)
c conflict literals        : 672895067   (67.70     % deleted)
c Memory used              : 321.81      MB
c CPU time                 : 62.92       s
   \endverbatim
   </li>
   <li> precosat236 takes a very long time (comparatively):
   \verbatim
shell> precosat236 experiment_r1_k1.cnf
c 12017467 conflicts, 13372168 decisions, 1 random
c 0 iterations, 2 restarts, 17488 skipped
c 36 enlarged, 4592 shrunken, 117385 rescored, 2282 rebiased
c 95 simplifications, 10 reductions
c
c vars: 132 fixed, 1621 equiv, 52 elim, 1621 merged
c elim: 1924760 resolutions, 43 phases, 43 rounds
c sbst: 100% substituted, 0.0% nots, 100.0% ands, 0.0% xors, 0.0% ites
c arty: 5.21 and 0.00 xor average arity
c prbe: 195926 probed, 98 phases, 98 rounds
c prbe: 0 failed, 0 lifted, 213 merged
c sccs: 1 non trivial, 0 fixed, 1 merged
c hash: 0 units, 1407 merged
c mins: 386193947 learned, 59% deleted, 52 strong, 38 depth
c subs: 5671 forward, 8118 backward, 1232 doms
c strs: 1036 forward, 336 backward
c doms: 1360 dominators, 938 high, 0 low
c prps: 3155560431 propagations, 0.70 megaprops
c
c 4505.6 seconds, 56 MB max, 1732 MB recycled
   \endverbatim
   </li>
  </ul>

*/
