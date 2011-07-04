// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/064/4_4_4/1_13.hpp
  \brief Investigations into small scale AES key discovery for one round AES with a 4x4 plaintext matrix and 4-bit field elements (1+1/3)


  \todo Problem specification ??? The MAIN POINT here is to see information
  about the boolean functions involved --- but nothing is said ???
  <ul>
   <li> In this file, we collect the investigations into translations of
   1 + 1/3 round small scale AES with four rows, four columns, using the 4-bit
   field size. </li>
   <li> The AES encryption scheme we model takes a 64-bit plaintext and
   64-bit key and outputs a 64-bit ciphertext. The plaintext, key and
   ciphertext are all considered, column by column, as 4x4 matrices of 4-bit
   elements. </li>
   <li> In other words, in the AES blocks (plaintext, key, ciphertext etc),
   the 4-bit element at position (i,j) in the matrix is the ((i-1)*4 + j)-th
   4-bit word of the 64-bits. </li>
   <li> The 4-bit element (b_0,b_1,b_2,b_3) is considered as the polynomial
   b_0 * x^3 + b_1 * x^2 + b_2 * x + b_3. Addition and multiplication
   on these polynomials is defined as usual, modulo the polynomial x^4+x+1.
   </li>
   <li> The encryption scheme applies the following operations:
    <ol>
     <li> Addition of round key 0 (input key) to plaintext. </li>
     <li> Application of SubBytes (Sbox to each 4-bit element) operation. </li>
     <li> Application of MixColumns operation. </li>
     <li> Addition of round key 1, resulting in the ciphertext. </li>
    </ol>
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications
   as whole boxes and translating these boxes using the canonical translation.
   </li>
   <li> Generating small scale AES for one round:
   \verbatim
num_rounds : 1$
num_rows : 4$
num_columns : 4$
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
  <li> In this translation, we have:
   <ul>
    <li> One full round (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 16 Sboxes in the SubBytes operation (4 rows * 4 columns = 16). </li>
    <li> 256 additions within the round and key additions, coming from:
     <ul>
      <li> 128 additions from key additions
      (2 round keys * 64-bit additions = 128). </li>
      <li> 128 additions from the matrix multiplication in the diffusion
      operation of arity 4
      (4 rows * 4 columns * 2 directions * 4 bits = 128).
      </li>
     </ul>
    </li>
    <li> 16 multiplications by 02 from the MixColumns operation
    (4 rows * 4 columns = 16). </li>
    <li> 16 multiplications by 03 from the MixColumns operation
    (4 rows * 4 columns = 16). </li>
    <li> 16 multiplications by 09 from the MixColumns operation
    (4 rows * 4 columns = 16). </li>
    <li> 16 multiplications by 11 from the MixColumns operation
    (4 rows * 4 columns = 16). </li>
    <li> 16 multiplications by 13 from the MixColumns operation
    (4 rows * 4 columns = 16). </li>
    <li> 16 multiplications by 14 from the MixColumns operation
    (4 rows * 4 columns = 16). </li>
    <li> 4 Sboxes in the AES key schedule (4 rows). </li>
    <li> 64 additions in the key schedule:
    <ul>
     <li> 4 additions of arity three (1 row * 1 column * 4 bits = 4). </li>
     <li> 60 additions of arity two
     ((3 rows * 4 columns + 1 rows * 3 columns) * 4 bits = 60). </li>
    </ul>
    </li>
    <li> 4 bits for the constant in the key schedule. </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(1,4,4,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,4],[2,14848],[3,752],[4,32],[5,2048],[9,1856],[16,116]]
maxima> mul_map(epoly) := block([e:poly2nat(epoly,2)],
  [epoly,[[2,'m(e,2)],[9,'m(e,9)],[16,'m(e,16)]]])$
maxima> ncl_list_ss_gen(1,4,4,4,ss_mixcolumns_matrix(2,4,4),[[2,'s2],[9,'s9],[16,'s16]],create_list(mul_map(p),p,[x,x+1,x^3+1,x^3+x+1,x^3+x^2+1,x^3+x^2+x]),false,aes_mc_bidirectional);
[[1,4],
 [2,20*s2+16*'m(14,2)+16*'m(13,2)+16*'m(11,2)+16*'m(9,2)+16*'m(3,2)+16*'m(2,2)],
 [3,752],[4,32],[5,2048],
 [9,20*s9+16*'m(14,9)+16*'m(13,9)+16*'m(11,9)+16*'m(9,9)+16*'m(3,9)+16*'m(2,9)],
 [16,20*s16+16*'m(14,16)+16*'m(13,16)+16*'m(11,16)+16*'m(9,16)+16*'m(3,16)+16*'m(2,16)]]
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 4 unit-clauses for the 4-bit constant in the key expansion. </li>
    <li> 14848 binary clauses, coming from 20 Sboxes and 16 of each of the six
    multiplications (116 * 128 = 14848). </li>
    <li> 752 ternary clauses, coming from 188 additions of arity two
    (188 * 4 = 752). </li>
    <li> 32 clauses of length four, coming from 4 additions of arity three
    (4 * 8 = 32). </li>
    <li> 2048 clauses of length five, 128 additions of arity 4
    (256 * 16 = 2048). </li>
    <li> 1856 clauses of length nine, coming from 20 Sboxes and 16 of each of
    the six multiplications (116 * 16 = 1856). </li>
    <li> 116 clauses of length sixteen, coming from from 20 Sboxes and 16 of
    each of the six multiplications (116 * 1 = 116). </li>
   </ul>
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
??? This todo is obviously too long --- the solvers etc. must go into another
todo ???
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
??? This is trivial to fix --- just apply "time" ???
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
??? where are 2 rounds etc.? where is the general plans-file ???
  </ul>

*/
