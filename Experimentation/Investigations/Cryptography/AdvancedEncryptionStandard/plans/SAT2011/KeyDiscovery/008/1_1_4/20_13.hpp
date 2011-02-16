// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/008/1_1_4/20_13.hpp
  \brief Investigations into simplest 8-bit small scale AES key discovery for twenty rounds AES (20+1/3)


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   ten round small scale AES with one column, one row, using the 8-bit
   field size. </li>
   <li> The AES encryption scheme we model takes a 8-bit plaintext,
   8-bit key and applies the following operations:
   <ol>
    <li> Key schedule which takes the key and generates twenty one 8-bit round
    keys. </li>
    <li> Addition of first round key (input key) to plaintext. </li>
    <li> Application of the following operation (the "round") ten times:
     <ol>
      <li> Application of SubBytes (Sbox to each byte) operation. </li>
      <li> Application of ShiftRows operation. </li>
      <li> Application of MixColumns operation. </li>
      <li> Addition of (n+1)-th round key (from key schedule). </li>
     </ol>
    </li>
    <li> The result of the last round key is then the ciphertext. </li>
   </ol>
   </li>
   <li> Note we have the following number of full rounds, special rounds,
   sboxes in the rounds, multiplications by each field element, sboxes in
   the key expansion, additions in the key expansion and constants in the
   key expansion:
   \verbatim
> component_statistics_ss(20,1,1,8,false,aes_mc_bidirectional);
[20,0,20,488,[[1,40]],20,160,160]
> component_statistics_ss(20,1,1,8,false,aes_mc_forward);
[20,0,20,328,[[1,20]],20,160,160]
   \endverbatim
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating simplest small scale AES for 10 rounds (with MixColumns):
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
   <li> The measured statistics match up to the computed statistics:
   \verbatim
maxima> ncl_list_ss(20,1,1,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,160],[2,164480],[3,1312],[17,10240],[256,40]]
   \endverbatim
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
