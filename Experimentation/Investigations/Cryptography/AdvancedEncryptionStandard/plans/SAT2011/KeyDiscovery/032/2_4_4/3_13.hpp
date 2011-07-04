// Matthew Gwynne, 13.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/032/2_4_4/3_13.hpp
  \brief Investigations into small scale AES key discovery for 3+1/3 round AES with a 2x4 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   3 + 1/3 round small scale AES with two rows, four columns, using the 4-bit
   field size. </li>
   <li> The AES encryption scheme we model takes a 32-bit plaintext and
   32-bit key and outputs a 32-bit ciphertext. The plaintext, key and
   ciphertext are all considered, column by column, as 2x4 matrices of 4-bit
   elements. </li>
   <li> In other words, in the AES blocks (plaintext, key, ciphertext etc),
   the 4-bit element at position (i,j) in the matrix is the ((i-1)*2 + j)-th
   4-bit word of the 32-bits. </li>
   <li> The 4-bit element (b_0,b_1,b_2,b_3) is considered as the polynomial
   b_0 * x^3 + b_1 * x^2 + b_2 * x + b_3. Addition and multiplication
   on these polynomials is defined as usual, modulo the polynomial x^4+x+1.
   </li>
   <li> The encryption scheme applies the following operations:
   <ol>
    <li> The round function iterated three times, consisting of (for
    round 0 <= i < 3):
    <ol>
     <li> Addition of round key i-1 to plaintext. </li>
     <li> Application of SubBytes (Sbox to each 4-bit element) operation. </li>
     <li> Application of MixColumns operation. </li>
    </ol>
    <li> Addition of round key 3, resulting in the ciphertext. </li>
   </ol>
   </li>
   <li> In this file, we collect:
   <ul>
    <li> Solvable in 61s by cryptominisat. </li>
   </ul>
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translating the AES cipher treating Sboxes and field multiplications
   as whole boxes and translating these boxes using the canonical translation.
   </li>
   <li> Generating small scale AES for 1 + 1/3 round:
   \verbatim
num_rounds : 3$
num_rows : 2$
num_columns : 4$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r3_c4_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
2916 19994 57660 0 57660 2917 1
 length count
1 12
2 16128
3 1616
4 96
9 2016
16 126
   \endverbatim
   </li>
   <li> In this translation, we have:
   <ul>
    <li> Three full rounds (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 24 Sboxes in the SubBytes operation
    (3 rounds * 2 rows * 4 columns = 24). </li>
    <li> 128 additions within the round and key additions, coming from:
     <ul>
      <li> 128 additions from key additions
      (4 round keys * 32-bit additions = 128). </li>
      <li> 192 additions from the matrix multiplication in the diffusion
      operation (3 rounds * 2 rows * 4 columns * 2 directions * 4 bits = 192).
      </li>
     </ul>
    </li>
    <li> 48 multiplications by 02 from the MixColumns operation
    (3 rounds * 2 rows * 4 columns * 2 directions = 48). </li>
    <li> 48 multiplications by 03 from the MixColumns operation
    (3 rounds * 2 rows * 4 columns * 2 directions = 48). </li>
    <li> 6 Sboxes in the AES key schedule
    (3 rounds * 2 rows = 6). </li>
    <li> 96 additions in the key schedule:
    <ul>
     <li> 12 additions of arity three
     (3 rounds * 1 row * 1 column * 4 bits = 12). </li>
     <li> 84 additions of arity two
     ((3 rounds * 1 rows * 3 columns + 2 rows * 2 columns) * 4 bits = 84).
     </li>
    </ul>
    </li>
    <li> 12 bits for the constant in the key schedule
    (3 rounds * 4-bits = 12). </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(3,4,2,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,12],[2,16128],[3,1616],[4,96],[9,2016],[16,126]]
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 12 unit-clauses for the 4-bit constants in the key expansion. </li>
    <li> 16128 binary clauses, coming from 30 Sboxes and 48 of each of the two
    multiplications (126 * 128 = 16128). </li>
    <li> 1616 ternary clauses, coming from 404 additions of arity two
    (404 * 4 = 1616). </li>
    <li> 96 clauses of length four, coming from 12 additions of arity three
    (12 * 8 = 96). </li>
    <li> 2016 clauses of length nine, coming from 30 Sboxes and 48 of each of
    the two multiplications (126 * 16 = 2016). </li>
    <li> 126 clauses of length sixteen, coming from from 30 Sboxes and 48 of
    each of the two multiplications (126 * 1 = 126). </li>
   </ul>
   </li>
   <li> Then we can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw2_e4_f0.cnf ssaes_pkpair_r1_c4_rw2_e4_f0_s1.cnf > r1_keyfind.cnf
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves it in 65s:
   \verbatim
shell> minisat-2.2.0 experiment_r3_k1.cnf
restarts              : 36632
conflicts             : 26578774       (406788 /sec)
decisions             : 28721930       (0.00 % random) (439590 /sec)
propagations          : 12638199080    (193427808 /sec)
conflict literals     : 690572739      (62.54 % deleted)
Memory used           : 63.00 MB
CPU time              : 65.3381 s
   \endverbatim
   </li>
   <li> cryptominisat solves it in 61.4s:
   \verbatim
shell> cryptominisat experiment_r3_k1.cnf
<snip>
c num threads              : 1
c restarts                 : 105
c dynamic restarts         : 0
c static restarts          : 105
c full restarts            : 5
c total simplify time      : 0.00
c learnts DL2              : 0
c learnts size 2           : 16933
c learnts size 1           : 78          (2.67      % of vars)
c filedLit time            : 0.34        (0.55      % time)
c v-elim SatELite          : 95          (3.26      % vars)
c SatELite time            : 3.01        (4.90      % time)
c v-elim xor               : 0           (0.00      % vars)
c xor elim time            : 0.01        (0.02      % time)
c num binary xor trees     : 945
c binxor trees' crown      : 1345        (1.42      leafs/tree)
c bin xor find time        : 0.00
c OTF clause improved      : 120638      (0.02      clauses/conflict)
c OTF impr. size diff      : 134131      (1.11       lits/clause)
c OTF cl watch-shrink      : 3069355     (0.57      clauses/conflict)
c OTF cl watch-sh-lit      : 9031454     (2.94       lits/clause)
c tried to recurMin cls    : 568500      (10.59      % of conflicts)
c updated cache            : 11910       (0.02       lits/tried recurMin)
c clauses over max glue    : 0           (0.00      % of all clauses)
c conflicts                : 5370243     (87472.26  / sec)
c decisions                : 5741789     (0.26      % random)
c bogo-props               : 339071263620 (5522903024.23 / sec)
c conflict literals        : 127462590   (59.36     % deleted)
c Memory used              : 81.37       MB
c CPU time                 : 61.39       s
   \endverbatim
   </li>
   <li> glucose solves it in 65s:
   \verbatim
shell> glucose experiment_r3_k1.cnf
<snip>
c restarts              : 50
c nb ReduceDB           : 164
c nb learnts DL2        : 2027
c nb learnts size 2     : 382
c nb learnts size 1     : 0
c conflicts             : 14316868       (219560 /sec)
c decisions             : 15063464       (1.57 % random) (231010 /sec)
c propagations          : 4612945014     (70743001 /sec)
c conflict literals     : 454828366      (65.69 % deleted)
c Memory used           : 91.69 MB
c CPU time              : 65.2071 s
   \endverbatim
   </li>
   <li> precosat236 solves it in 1246s:
   \verbatim
shell> precosat236 experiment_r3_k1.cnf
<snip>
c 5597931 conflicts, 6207027 decisions, 1 random
c 0 iterations, 58 restarts, 8769 skipped
c 34 enlarged, 4189 shrunken, 54671 rescored, 1164 rebiased
c 47 simplifications, 2 reductions
c
c vars: 76 fixed, 1301 equiv, 70 elim, 1301 merged
c elim: 868961 resolutions, 18 phases, 18 rounds
c sbst: 100% substituted, 0.0% nots, 100.0% ands, 0.0% xors, 0.0% ites
c arty: 5.40 and 0.00 xor average arity
c prbe: 212506 probed, 70 phases, 70 rounds
c prbe: 0 failed, 0 lifted, 127 merged
c sccs: 0 non trivial, 0 fixed, 0 merged
c hash: 0 units, 1174 merged
c mins: 135411627 learned, 55% deleted, 26 strong, 46 depth
c subs: 4133 forward, 5880 backward, 476 doms
c strs: 1256 forward, 87 backward
c doms: 540 dominators, 337 high, 0 low
c prps: 1627135948 propagations, 1.31 megaprops
c
c 1246.0 seconds, 22 MB max, 635 MB recycled
   \endverbatim
   </li>
  </ul>

*/
