// Matthew Gwynne, 17.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/016/2_2_4/20_13.hpp
  \brief Investigations into small-scale AES key discovery for 20 + 1/3 round AES with a 2x2 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> We investigate the 20 + 1/3 round small-scale AES with 2 row,
   2 column, using the 4-bit field size. </li>
   <li> We denote this AES instance by aes(20,2,2,4). </li>
   <li> aes(20,2,2,4) takes a 16-bit plaintext and 16-bit key and
   outputs a 16-bit ciphertext. </li>
   <li> For the full specification of this AES instance, see
   "Problem specification" in
   Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/016/2_2_4/general.hpp.
   </li>
   <li> Note that we consider the canonical CNF translation, as
   this is an example of the "hardest" representation without
   new variables. See "Hardness of boolean function representations"
   in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
  </ul>


  \todo Using the canonical core round box translation
  <ul>
   <li> Translation of aes(20,2,2,4):
    <ul>
     <li> The cipher is decomposed such that each round consists of two
     "core round column operations", one to each column in the input, as well
     as key additions. </li>
     <li> A "core round column operation" is permutation over ({0,1}^4)^2,
     which is a column-wise application of the SubBytes and MixColumns
     operation combined. </li>
     <li> The S-box is considered as an 8x1 boolean function,
     translated using the canonical translation; see dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
     </li>
     <li> The core-round-column-operation is considered as a 16-bit to 1-bit
     boolean function, translated using the canonical translation;
     see dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
     </li>
     <li> Additions are translated using their prime implicates. </li>
    </ul>
   </li>
   <li> Generating small-scale AES for twenty rounds:
   \verbatim
rounds : 20$
num_rows : 2$
num_columns : 2$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_core_round_box$
output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r20_c2_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
12128 182944 538192 0 538192 12129 1
 length count
1 80
2 168960
3 2304
4 640
9 640
16 40
17 10240
256 40
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 20 full rounds (Key Addition, SubBytes, and MixColumns
     operation). </li>
     <li> 40 S-boxes:
      <ul>
       <li> 40 from key schedule = 2 row * 1 word * 20 rounds. </li>
      </ul>
     </li>
     <li> 40 core round operations (Sbox + MixColumns on a column) =
     2 columns * 20 rounds. </li>
     <li> 656 additions:
      <ul>
       <li> 576 additions of arity 2:
        <ul>
         <li> 320 from key additions = 16 bits * 20 rounds; </li>
         <li> 16 from final key addition = 16 bits; </li>
         <li> 240 from the key schedule = (16 bits - 4 bits) * 20 rounds. </li>
        </ul>
        <li> 80 additions of arity 3:
         <ul>
          <li> 80 from the key schedule = 4 bits * 20 round. </li>
         </ul>
        </li>
       </li>
      </ul>
     </li>
     <li> 80 bits for the constant in the key schedule = 4 bits * 20 rounds.
     </li>
    </ul>
   </li>
   <li> The number of clauses in the canonical translation of the S-box and
   round column:
   \verbatim
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
maxima> ncl_list_full_dualts(16,256);
[[2,4096],[17,256],[256,1]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 80 = key schedule constant * 1; </li>
     <li> 2 : 168960 = 40 round column operations * 4096 + 40 S-boxes * 128;
     </li>
     <li> 3 : 2304 = 576 additions (arity 2) * 4; </li>
     <li> 4 : 640 = 80 additions (arity 3) * 8; </li>
     <li> 9 : 640 = 40 S-boxes * 16; </li>
     <li> 16 : 40 = 40 S-boxes * 1; </li>
     <li> 17 : 10240 = 40 round column operations * 256; </li>
     <li> 256 : 40 = 40 round column operations * 1. </li>
    </ul>
   </li>
   <li> Then we can generate random assignments with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignments with the translations:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r20_c2_rw2_e4_f0.cnf ssaes_pkpair_r20_c2_rw2_e4_f0_s1.cnf > experiment_r20_k1.cnf
shell> AppendDimacs-O3-DNDEBUG ssaes_r20_c2_rw2_e4_f0.cnf ssaes_pkpair_r20_c2_rw2_e4_f0_s1.cnf > experiment_r20_k5.cnf
   \endverbatim
   </li>
   <li> cryptominisat takes 192s to solve it:
   \verbatim
shell> cryptominisat experiment_r20_k1.cnf
<snip>
c num threads              : 1
c restarts                 : 1089
c dynamic restarts         : 1071
c static restarts          : 18
c full restarts            : 2
c total simplify time      : 0.07
c learnts DL2              : 0
c learnts size 2           : 235663
c learnts size 1           : 115         (0.95      % of vars)
c filedLit time            : 6.07        (3.16      % time)
c v-elim SatELite          : 60          (0.49      % vars)
c SatELite time            : 2.21        (1.15      % time)
c v-elim xor               : 0           (0.00      % vars)
c xor elim time            : 0.03        (0.02      % time)
c num binary xor trees     : 32
c binxor trees' crown      : 32          (1.00      leafs/tree)
c bin xor find time        : 0.04
c OTF clause improved      : 3594        (0.02      clauses/conflict)
c OTF impr. size diff      : 3837        (1.07       lits/clause)
c OTF cl watch-shrink      : 142795      (0.62      clauses/conflict)
c OTF cl watch-sh-lit      : 582828      (4.08       lits/clause)
c tried to recurMin cls    : 109815      (47.37      % of conflicts)
c updated cache            : 0           (0.00       lits/tried recurMin)
c clauses over max glue    : 0           (0.00      % of all clauses)
c conflicts                : 231833      (1205.08   / sec)
c decisions                : 270275      (2.13      % random)
c bogo-props               : 13765644606 (71554447.48 / sec)
c conflict literals        : 6374120     (73.06     % deleted)
c Memory used              : 97.27       MB
c CPU time                 : 192.38      s
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves this in 81s:
   \verbatim
shell> minisat-2.2.0 experiment_r20_k1.cnf
<snip>
restarts              : 250
conflicts             : 77743          (964 /sec)
decisions             : 90692          (0.00 % random) (1124 /sec)
propagations          : 597366283      (7405979 /sec)
conflict literals     : 1339579        (79.69 % deleted)
Memory used           : 38.00 MB
CPU time              : 80.66 s
   \endverbatim
   </li>
   <li> precosat236 takes 229.7s:
   \verbatim
shell> precosat236 experiment_r20_k1.cnf
<snip>
c 166198 conflicts, 443321 decisions, 244 random
c 1 iterations, 486 restarts, 0 skipped
c 12 enlarged, 50 shrunken, 1624 rescored, 62 rebiased
c 50 simplifications, 0 reductions
c
c vars: 120 fixed, 32 equiv, 80 elim, 32 merged
c elim: 4784827 resolutions, 20 phases, 7 rounds
c sbst: 100% substituted, 0.0% nots, 100.0% ands, 0.0% xors, 0.0% ites
c arty: 12.00 and 0.00 xor average arity
c prbe: 508523 probed, 660 phases, 21 rounds
c prbe: 5 failed, 1 lifted, 0 merged
c sccs: 0 non trivial, 0 fixed, 0 merged
c hash: 1 units, 32 merged
c mins: 31975241 learned, 59% deleted, 0 strong, 17 depth
c subs: 0 forward, 0 backward, 93 doms
c strs: 22038 forward, 0 backward
c doms: 93 dominators, 0 high, 2 low
c prps: 812697430 propagations, 3.54 megaprops
c
c 229.7 seconds, 47 MB max, 101 MB recycled
   \endverbatim
   </li>
   <li> precosat-570.1 takes 100s:
   \verbatim
shell> precosat-570.1 experiment_r20_k1.cnf
<snip>
c 105098 conflicts, 625006 decisions, 303 random
c 0 iterations, 2046 restarts, 0 skipped
c 11 enlarged, 0 shrunken, 1027 rescored, 45 rebiased
c 1 simplifications, 11 reductions
c
c arty: 0.00 ands 0.00 xors average arity
c autk: 0 autarkies of 0.0 avg size
c autk: dhs 0 0 0 0 0 0
c back: 104942 track with 0.1 avg cuts, 23375 jumps of 14.1 avg len
c blkd: 864384 resolutions, 1 phases, 1 rounds
c blkd: 10880 = 0 implicit + 10880 explicit
c clss: 11920 recycled, 0 pure, 0 autark
c doms: 669 dominators, 446 high, 0 low
c elim: 20278 resolutions, 1 phases, 1 rounds
c extd: 3 forced, 77 assumed, 0 flipped
c glue: 45.27 original glue, 0.148 slimmed on average
c mins: 20086455 lrnd, 47% del, 0 strng, 0 inv, 18 dpth
c otfs: dynamic 156 = 0 bin + 0 trn + 156 large
c otfs: static 6 = 0 bin + 0 trn + 6 large
c prbe: 744310 probed, 378 phases, 378 rounds
c prbe: 0 failed, 0 lifted, 0 merged
c prps: 380708765 srch props, 4.04 megaprops per second
c pure: 0 = 0 explicit + 0 elim + 0 blkd + 0 autark
c sbst: 0% subst, 0.0% nots, 0.0% ands, 0.0% xors, 0.0% ites
c sccs: 32 non trivial, 0 fixed, 32 merged
c strs: 2184 forward, 0 backward, 0 dynamic, 0 org, 0 asym
c subs: 0 fw, 0 bw, 0 dynamic, 0 org, 114 doms, 0 gc
c time: 100.3 = 94.3 srch (94%) + 5.7 simp (6%) + 0.2 othr (0%)
c vars: 112 fxd, 32 eq, 80 elim, 0 pure, 0 zmbs, 0 autk
c zmbs: 0 = 0 explicit + 0 elim + 0 blkd + 0 autark
c
c 100.3 seconds, 28 MB max, 71 MB recycled
   \endverbatim
   </li>
   <li> glucose takes 133.51s:
   \verbatim
shell> glucose experiment_r20_k1.cnf
<snip>
c restarts              : 5
c nb ReduceDB           : 3
c nb learnts DL2        : 86
c nb learnts size 2     : 0
c nb learnts size 1     : 0
c conflicts             : 63658          (477 /sec)
c decisions             : 75805          (1.91 % random) (568 /sec)
c propagations          : 323584232      (2423670 /sec)
c conflict literals     : 4141772        (81.01 % deleted)
c Memory used           : 23.38 MB
c CPU time              : 133.51 s
   \endverbatim
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translation of aes(20,2,2,4):
    <ul>
     <li> The MixColumns operation is decomposed into its field
     multiplications (02 and 03) and addition operations. </li>
     <li> The MixColumns operation is translated by translating both
     the MixColumns operation and its inverse (it is self-inverse). </li>
     <li> We treat S-boxes, field multiplications and additions as boxes.
     </li>
     <li> The S-box and field multiplications are considered as a 8x1
     boolean functions, translated using the canonical translation;
     see dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
     </li>
     <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
     boolean functions; translated using their prime implicates. </li>
    </ul>
   </li>
   <li> Generating small-scale AES for twenty rounds (with MixColumns):
   \verbatim
rounds : 20$
num_rows : 2$
num_columns : 2$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r20_c2_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
9888 69384 200272 0 200272 9889 1
 length count
1 80
2 56320
3 4864
4 640
9 7040
16 440
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 20 full rounds (Key Addition, SubBytes, and MixColumns
     operation). </li>
     <li> 120 Sboxes:
      <ul>
       <li> 80 from SubBytes = 4 byte * 20 rounds; </li>
       <li> 40 from key schedule = 2 row * 1 word * 20 rounds. </li>
      </ul>
     </li>
     <li> 160 multiplications by 02: 2 rows * 1 multiplication * 2 columns *
     20 rounds * 2 directions (forward + inverse). </li>
     <li> 160 multiplications by 03: 2 rows * 1 multiplication * 2 columns *
     20 rounds * 2 directions (forward + inverse). </li>
     <li> 1296 additions:
      <ul>
       <li> 1216 additions of arity 2:
        <ul>
         <li> 320 from key additions = 16 bits * 20 rounds; </li>
         <li> 16 from final key addition = 16 bits; </li>
         <li> 240 from the key schedule = (16 bits - 4 bits) * 20 rounds. </li>
         <li> 320 from forward MixColumns = 2 rows * 2 column * 4 bits *
         20 rounds; </li>
         <li> 320 from inverse MixColumns = 2 rows * 2 column * 4 bits * 20
         rounds. </li>
        </ul>
        <li> 80 additions of arity 3:
         <ul>
          <li> 80 from the key schedule = 4 bits * 20 round. </li>
         </ul>
        </li>
       </li>
      </ul>
     </li>
     <li> 80 bits for the constant in the key schedule = 4 bits * 20 rounds.
     </li>
    </ul>
   </li>
   <li> The number of clauses of each length in the canonical translation:
   \verbatim
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   </li>
   <li> This instances has 440 boxes = 120 S-boxes + 320 multiplications.
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 80 = key schedule constant * 1; </li>
     <li> 2 : 56320 = 440 boxes * 128; </li>
     <li> 3 : 4864 = 1216 additions (arity 2) * 4; </li>
     <li> 4 : 640 = 80 additions (arity 3) * 8; </li>
     <li> 7 : 7040 = 440 boxes * 16; </li>
     <li> 16 : 440 = 440 boxes * 1. </li>
    </ul>
   </li>
   <li> Then we can generate random assignments with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignments with the translations:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r20_c2_rw2_e4_f0.cnf ssaes_pkpair_r20_c2_rw2_e4_f0_s1.cnf > experiment_r20_k1.cnf
shell> AppendDimacs-O3-DNDEBUG ssaes_r20_c2_rw2_e4_f0.cnf ssaes_pkpair_r20_c2_rw2_e4_f0_s1.cnf > experiment_r20_k5.cnf
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves this in times ranging from 3.45s to 40s:
   \verbatim
shell> minisat-2.2.0 experiment_r20_k1.cnf
<snip>
restarts              : 18
conflicts             : 3513           (1018 /sec)
decisions             : 3838           (0.00 % random) (1112 /sec)
propagations          : 29196071       (8462629 /sec)
conflict literals     : 43580          (86.63 % deleted)
Memory used           : 24.00 MB
CPU time              : 3.45 s

shell> minisat-2.2.0 experiment_r20_k5.cnf
<snip>
restarts              : 158
conflicts             : 52416          (1426 /sec)
decisions             : 59697          (0.00 % random) (1624 /sec)
propagations          : 262216771      (7131269 /sec)
conflict literals     : 1656658        (69.32 % deleted)
Memory used           : 32.00 MB
CPU time              : 36.77 s
   \endverbatim
   </li>
  </ul>

*/
