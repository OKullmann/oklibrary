// Matthew Gwynne, 17.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_2_4/20_13.hpp
  \brief Investigations into small scale AES key discovery for 20 + 1/3 round AES with a 2x2 plaintext matrix and 4-bit field elements

  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   20 + 1/3 round small scale AES with two rows, two columns, using the 4-bit
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
    <li> Round (iterated twenty times):
    <ol>
     <li> Addition of round key (n-1). </li>
     <li> Application of SubBytes (Sbox to each 4-bit element) operation. </li>
     <li> Application of linear diffusion operation. </li>
    </ol>
    </li>
    <li> Addition of round key 20 yielding the ciphertext. </li>
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


  \todo Using the canonical core round box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes in the key schedule
   and the operation of the round (SubBytes and MixColumns) as boxes
   using the canonical translation for those boxes. </li>
   <li> Generating small scale AES for twenty rounds:
   \verbatim
rounds : 20$
num_columns : 2$
num_rows : 2$
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
    <li> Twenty full rounds (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 40 Sboxes from the key schedule (20 rounds * 2 rows = 40). </li>
    <li> 40 core round column operations, the combined Sbox and MixColumn
    operation on each column (20 rounds * 2 columns = 40). </li>
    <li> 336 additions within the round and key additions, coming from:
     <ul>
      <li> 48 additions from key additions 
      (21 round keys * 16-bit additions = 336). </li>
     </ul>
    </li>
    <li> 320 additions in the key schedule:
    <ul>
     <li> 80 additions of arity three
     (1 row * 1 column * 4 bits * 20 rounds = 80). </li>
     <li> 240 additions of arity two 
     ((1 rows * 2 columns + 1 row * 1 column) * 4 bits * 20 rounds = 240). 
     </li>
    </ul>
    </li>
    <li> 80 bits for the constant in the key schedule. 
    (4 bits * 20 rounds = 80). </li>
   </ul>
   </li>
   <li> The number of clauses in the canonical translation of the round
   column:
   \verbatim
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
maxima> ncl_list_full_dualts(16,256);
[[2,4096],[17,256],[256,1]]
   \endverbatim
   We have the following number of clauses of each size:
   <ul>
    <li> 80 unit clauses for the 4-bit constant in the key expansion. </li>
    <li> 168960 binary clauses, coming from 40 core round column operations
    and 20 Sboxes (40 * 4096 + 40 * 128 = 168960). </li>
    <li> 2304 ternary clauses, coming from 576 additions of arity two
    (576 * 4 = 2304). </li>
    <li> 640 clauses of length four, coming from 80 additions of arity three
    (80 * 8 = 640). </li>
    <li> 640 clauses of length nine, coming from 40 Sboxes (40 * 16 = 640). 
    </li>
    <li> 40 clauses of length sixteen, coming 40 Sboxes (40 * 1 = 40). </li>
    <li> 10240 clauses of length seventeen, coming from from 40 core round
    column operations (40 * 256 = 10240). </li>
    <li> 40 clauses of length 256, coming from 40 core round column operations
    (40 * 1 = 40). </li>
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
   <li> Translating the AES cipher treating Sboxes and field multiplications 
   as whole boxes and translating these boxes using the canonical translation.
   </li>
   <li> Generating small scale AES for twenty rounds (with MixColumns):
   \verbatim
rounds : 20$
num_columns : 2$
num_rows : 2$
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
    <li> Twenty full rounds (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 80 Sboxes in the SubBytes operation
    (2 rows * 2 columns * 20 rounds = 80). </li>
    <li> 976 additions within the round and key additions, coming from:
     <ul>
      <li> 48 additions from key additions 
      (21 round keys * 16-bit additions = 336). </li>
      <li> 640 additions from the matrix multiplication in the diffusion 
      operation 
      (2 rows * 2 columns * 2 directions * 4 bits * 20 rounds = 640).
      </li>
     </ul>
    </li>
    <li> 160 multiplications by 02 from the MixColumns operation
    (2 rows * 2 columns * 2 directions * 20 rounds = 160). </li>
    <li> 160 multiplications by 03 from the MixColumns operation
    (2 rows * 2 columns * 2 directions * 20 rounds = 160). </li>
    <li> 40 Sboxes in the AES key schedule
    (2 rows * 20 rounds = 40). </li>
    <li> 320 additions in the key schedule:
    <ul>
     <li> 80 additions of arity three
     (1 row * 1 column * 4 bits * 20 rounds = 80). </li>
     <li> 24 additions of arity two 
     ((1 rows * 2 columns + 1 row * 1 column) * 4 bits * 20 rounds = 240). 
     </li>
    </ul>
    </li>
    <li> 80 bits for the constant in the key schedule. 
    (4 bits * 20 rounds = 80). </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(20,2,2,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,80],[2,56320],[3,4864],[4,640],[9,7040],[16,440]]
maxima> ncl_list_ss_gen(20,2,2,4,ss_mixcolumns_matrix(2,4,2),[[2,'s2],[9,'s9],[16,'s16]],[[x,[[2,'m2_2],[9,'m2_9],[16,'m2_16]]],[x+1,[[2,'m3_2],[9,'m3_9],[16,'m3_16]]]],false,aes_mc_bidirectional);
[[1,80],[2,120*s2+160*m3_2+160*m2_2],[3,4864],[4,640],[9,120*s9+160*m3_9+160*m2_9],
        [16,120*s16+160*m3_16+160*m2_16]]
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 80 unit clauses for the 4-bit constant in the key expansion. </li>
    <li> 56320 binary clauses, coming from 120 Sboxes and 160 of each of the 
    two multiplications (440 * 128 = 56320). </li>
    <li> 4864 ternary clauses, coming from 1216 additions of arity two
    (1216 * 4 = 4864). </li>
    <li> 640 clauses of length four, coming from 80 additions of arity three
    (80 * 8 = 640). </li>
    <li> 7040 clauses of length seven, coming from 120 Sboxes and 160 of each
    of the two multiplications (440 * 16 = 7040). </li>
    <li> 440 clauses of length sixteen, coming from from 120 Sboxes and 160
    of each of the two multiplications (440 * 1 = 440). </li>
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
