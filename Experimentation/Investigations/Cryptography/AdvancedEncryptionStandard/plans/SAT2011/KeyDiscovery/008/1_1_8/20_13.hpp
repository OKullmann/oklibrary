// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/008/1_1_8/20_13.hpp
  \brief Investigations into simplest 8-bit small scale AES key discovery for 20 + 1/3 round AES


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   20 + 1/3 round small scale AES with one row, one column, using the 8-bit
   field size. </li>
   <li> The AES encryption scheme we model takes a 8-bit plaintext and
   8-bit key and outputs a 8-bit ciphertext.
   </li>
   <li> The 8-bit element (b_0,b_1,b_2,b_3,b_4,b_5,b_6,b_7) is considered as
   the polynomial b_0 * x^7 + b_1 * x^6 + b_2 * x^5 + b_4 * x^3 + b_5 * x^2 +
   b^6 * x + b_7. Addition and multiplication on these polynomials is defined
   as usual, modulo the polynomial x^8+x^4+x^3+x+1. </li>
   <li> The encryption scheme applies the following operations:
   <ol>
    <li> Key schedule which takes the key and generates twenty-one 8-bit round
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
   <li> The Sbox is non-linear permutation over the set of 8-bit elements,
   defined as inversion within the 8-bit field composed with an affine
   transformation. </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Generating simplest small scale AES for 20+1/3 rounds:
   \verbatim
shell> ${OKlib}/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/generate_aes_experiment 20 1 1 1 8 false aes_ts_box aes_mc_bidirectional
shell> cat ssaes_r20_c1_rw1_e8_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
11224 176232 517376 0 517376 11225 1
 length count
1 160
2 164480
3 1312
17 10240
256 40
   \endverbatim
   </li>
   <li> In this translation, we have:
   <ul>
    <li> Twenty full rounds (Key Addition, SubBytes, and diffusion operation).
    </li>
    <li> 20 Sboxes in the SubBytes operation
    (1 rows * 1 columns * 20 rounds = 20). </li>
    <li> 488 additions within the round and key additions, coming from:
     <ul>
      <li> 168 additions of arity 2 from key additions
      (21 round keys * 8-bit additions = 168). </li>
      <li> 320 additions of arity one from the identity matrix multiplication
      in the diffusion operation
      (1 rows * 1 columns * 2 directions * 8 bits * 20 rounds = 320).
      </li>
     </ul>
    </li>
    <li> 20 Sboxes in the AES key schedule
    (1 rows * 20 rounds = 20). </li>
    <li> 160 additions in the key schedule:
    <ul>
     <li> 160 additions of arity two
     (1 row * 1 column * 8 bits * 20 rounds = 160). </li>
    </ul>
    </li>
    <li> 160 bits for the constant in the key schedule
    (8 bits * 20 rounds = 160).
    </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(20,1,1,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,160],[2,164480],[3,1312],[17,10240],[256,40]]
maxima> ncl_list_ss_gen(10,1,1,4,ss_mixcolumns_matrix(2,4,1),[[2,'s2],[9,'s9],[16,'s16]],[],false,aes_mc_bidirectional);
[[1,160],[2,40*s2+640],[3,1312],[9,40*s9],[16,40*s16]]
maxima> ncl_list_full_dualts(8,16);
[[2,4096],[17,256],[256,1]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 160 unit-clauses for the 4-bit constant in the key expansion. </li>
    <li> 164480 binary clauses, coming from 40 Sboxes and 320 additions of
    arity one (40 * 4096 + 320 * 2 = 164480). </li>
    <li> 1312 ternary clauses, coming from 328 additions of arity two
    (328 * 4 = 656). </li>
    <li> 10240 clauses of length nine, coming from 40 Sboxes
    (40 * 256 = 10240). </li>
    <li> 40 clauses of length 256, coming from from 40 Sboxes
    (40 * 1 = 40). </li>
   </ul>
   </li>
   <li> Then we run experiments for AES instances with one round, up to
   those with twenty rounds, and inspect the results for round 20.
   \verbatim
shell> ${OKlib}/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/run_aes_experiment 20 1 1 1 8 false aes_ts_box aes_mc_bidirectional
   \endverbatim
   </li>
   <li> precosat236 solves this problem with no decisions in a fraction
   of a second:
   \verbatim
shell> cat experiment_r20_k1.cnf_precosat236
<snip>
c 0 conflicts, 0 decisions, 0 random
c 0 iterations, 0 restarts, 0 skipped
c 0 enlarged, 0 shrunken, 0 rescored, 0 rebiased
c 1 simplifications, 0 reductions
c
c vars: 11224 fixed, 0 equiv, 0 elim, 336 merged
c elim: 0 resolutions, 1 phases, 1 rounds
c sbst: 0% substituted, 0.0% nots, 0.0% ands, 0.0% xors, 0.0% ites
c arty: 0.00 and 0.00 xor average arity
c prbe: 881 probed, 1 phases, 2 rounds
c prbe: 255 failed, 0 lifted, 0 merged
c sccs: 0 non trivial, 0 fixed, 0 merged
c hash: 0 units, 336 merged
c mins: 0 learned, 0% deleted, 0 strong, 0 depth
c subs: 0 forward, 0 backward, 16 doms
c strs: 0 forward, 0 backward
c doms: 4894 dominators, 0 high, 0 low
c prps: 2928753 propagations, 5.87 megaprops
c
c 0.5 seconds, 15 MB max, 6 MB recycled
   \endverbatim
   </li>
  </ul>

*/
