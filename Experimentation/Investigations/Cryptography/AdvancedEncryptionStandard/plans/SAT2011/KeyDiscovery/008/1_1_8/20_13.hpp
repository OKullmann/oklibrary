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
     <li> 20 full rounds (Key Addition and SubBytes).
     </li>
     <li> 40 Sboxes:
      <ul>
       <li> 20 from SubBytes = 1 byte * 20 round; </li>
       <li> 20 from key schedule = 1 row * 1 byte * 20 round. </li>
      </ul>
     </li>
     <li> 648 additions:
      <ul>
       <li> 320 additions of arity 1:
        <ul>
         <li> 160 from forward MixColumns = 8 bits * 20 rounds; </li>
         <li> 160 from inverse MixColumns = 8 bits * 20 rounds. </li>
        </ul>
       </li>
       <li> 328 additions of arity 2:
        <ul>
         <li> 160 from key additions = 8 bits * 20 rounds; </li>
         <li> 8 from final key addition = 8 bits; </li>
         <li> 160 from the key schedule = 8 bits * 20 rounds. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 8 bits for the constant in the key schedule. </li>
    </ul>
   </li>
   <li> The number of clauses of each length in the canonical translation of
   the S-box:
   \verbatim
maxima> ncl_list_full_dualts(8,16);
[[2,4096],[17,256],[256,1]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 160 = key schedule constant * 1; </li>
     <li> 2 : 164480 = 40 S-boxes * 4096 + 320 "additions" (arity 1) * 2;
     </li>
     <li> 3 : 1312 = 328 additions (arity 2) * 4; </li>
     <li> 17 : 10240 = 40 S-boxes * 256; </li>
     <li> 256 : 40 = 40 S-boxes * 1. </li>
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
c prps: 2928753 propagations, 5.87 megaprops
c 0.5 seconds, 15 MB max, 6 MB recycled
   \endverbatim
   </li>
  </ul>

*/
