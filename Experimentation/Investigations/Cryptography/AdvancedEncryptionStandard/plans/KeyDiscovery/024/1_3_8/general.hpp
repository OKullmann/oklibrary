// Matthew Gwynne, 28.11.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/024/1_3_8/general.hpp
  \brief Investigations into small-scale AES key discovery with 1 row, 3 columns and 8-bit field elements


  \todo Problem specification
  <ul>
  <li> We consider the small-scale AES with 1 row, 3 columns, using the 8-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,1,3,8) for r in 1,...,20. </li>
   <li> We investigate translations of the key discovery problem for
   aes(r,1,3,8) into SAT. </li>
   <li> aes(r,1,3,8) takes a 24-bit plaintext and 24-bit key and outputs a
   24-bit ciphertext. </li>
   <li> aes(r,1,3,8) applies the following operations:
    <ol>
     <li> Key schedule which takes the key and generates r+1 24-bit round
     keys. </li>
     <li> Application of the following operation (the "round") r times:
      <ol>
       <li> Addition of round key n-1. </li>
       <li> Application of Sbox operation. </li>
      </ol>
     </li>
     <li> Addition of round key r+1. </li>
     <li> The result of the last round key addition is the ciphertext. </li>
    </ol>
   </li>
   <li> Round key 0 is the input key. </li>
   <li> The key schedule computes the round key i, K_(i,j), from round key
   i-1, K_(i-1), by:
   \verbatim
K_(i,j) := S-box(K_(i-1,1)) + C_i + sum(K_(i-1,k),k,1,j)
   \endverbatim
   where
    <ul>
     <li> C_i is the round constant for round i; </li>
     <li> K_(i,j) is the j-th 8-bit word of the i-th round-key. </li>
    </ul>
   </li>
   <li> The S-box is a permutation from {0,1}^8 to {0,1}^8 which we consider
   as either:
    <ul>
     <li> a 16x1 boolean function; see ss_sbox_bf in
     ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/AdvancedEncryptionStandard.mac.
     </li>
     <li> 8 8x1 boolean functions. </li>
    </ul>
   </li>
   <li> The decompositions and translations are listed in "Investigating
   dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> The plaintext and ciphertext variables are then set, and the SAT
   SAT solver is run on this instance to deduce the key variables. </li>
  </ul>


  \todo Translations
  <ul>
   <li> The following translations are considered in this %plans %file:
    <ul>
     <li> The canonical box translation. </li>
     <li> The "minimum" box translation. </li>
     <li> The 1-base translation. </li>
    </ul>
   </li>
   <li> For a full list of the possible translations, see
   "Investigating dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> Generating instances for rounds 1-20 for a 20 random keys with each
   of the translations:
    <ul>
     <li> The canonical box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c3_rw1_e8_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c3_rw1_e8_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 3$
exp : 8$
final_round_b : false$
box_tran : aes_ts_box$
mc_tran : aes_mc_bidirectional$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for s in $(seq 1 20) do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c3_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
  done
done
     \endverbatim
     </li>
     <li> The "minimum" box translation:
      <ul>
       <li> The CNFs for the Sbox and multiplications:
       \verbatim
/* Multiplication by 02: */
maxima> FieldMul2CNF : [{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},{{-9,2},{-2,9},{-10,3},{-3,10},{-11,4},{-4,11},{-12,-5,-1},{-12,1,5},{-5,1,12},{-1,5,12},{-13,-6,-1},{-1,6,13},{-14,7},{-7,14},{-15,1,8},{-8,1,15},{-16,-15,-8},{-16,8,15},{-13,6,16},{-6,13,16}}]$
set_hm(ss_field_cnfs,[8,2], FieldMul2CNF));
/* Multiplication by 03: */
maxima> FieldMul3CNF :
 [[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16],
  [{-9,-2,-1},{-2,1,9},{-10,2,3},{-10,-9,-3,1},{-10,-3,-1,9},{-3,2,10},{-9,1,3,10},{-1,3,9,10},{-11,-4,-3},{-11,3,4},{-4,3,11},{-3,4,11},{-12,-5,-4,1},{-12,-4,-1,5},{-5,1,4,12},{-1,4,5,12},{-13,-5,-1,6},{-13,1,5,6},{-13,-12,-6,4},{-13,-6,-4,12},{-6,-5,-1,13},{-6,1,5,13},
   {-12,4,6,13},{-4,6,12,13},{-14,-7,-6},{-14,6,7},{-7,6,14},{-6,7,14},{-16,-8,-1},{-16,1,8},{-16,-15,-7},{-16,7,15},{-8,1,16},{-1,8,16},{-15,7,16},{-7,15,16}]]$
set_hm(ss_field_cnfs,[8,2], FieldMul3CNF));
/* Sbox: */
maxima> output_rijnsbox_fullcnf_stdname();
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_Sbox_shg.cnf
shell> cat AES_Sbox_shg.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > AES_Sbox_shg.wcnf
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 40000000 -wtarget 294 -solve 1 -seed 3213901809 -i AES_Sbox_shg.wcnf -r model AES_Sbox_s294.ass;
shell> cat  AES_Sbox_full.cnf_primes | FilterDimacs AES_Sbox_s294.ass > AES_Sbox_s294.cnf
maxima> SboxMinCNF : read_fcl_f("AES_Sbox_s294.cnf");
maxima> set_hm(ss_sbox_cnfs,8, SboxMinCNF));
       \endverbatim
       </li>
       <li> Please see "Computing a minimum CNF represention" in
       AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp. </li>
       <li> Generating the instances:
       \verbatim
shell> mkdir ssaes_r1-20_c3_rw1_e8_f0_k1-1_aes_min_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c3_rw1_e8_f0_k1-1_aes_min_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 3$
exp : 8$
final_round_b : false$
box_tran : aes_small_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for s in $(seq 1 20) do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c3_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
  done
done
       \endverbatim
       </li>
      </ul>
     </li>
     <li> The 1-base box translation:
      <ul>
       <li> Generating a 1-base for the S-box from
   Cryptography/AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp. :
   \verbatim
maxima> output_rijnsbox_fullcnf_stdname();
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_Sbox_pi.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 103 < AES_Sbox_pi.cnf | SortByClauseLength-O3-DNDEBUG > AES_Sbox_sortedpi.cnf
shell> RUcpGen-O3-DNDEBUG AES_Sbox_sortedpi.cnf > AES_Sbox_gen.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_Sbox_gen.cnf | SortByClauseLengthDescending-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > AES_Sbox_base.cnf
shell> cat AES_Sbox_base.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 4398 30108 0 30108 0 1
 length count
5 1
6 1187
7 2703
8 503
9 4
       \endverbatim
       </li>
       <li> Please see "r_1-bases" in
       AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp. </li>
       <li> Generating the instances:
       \verbatim
shell> mkdir ssaes_r1-20_c3_rw1_e8_f0_k1-1_aes_1base_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c3_rw1_e8_f0_k1-1_aes_1base_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 3$
exp : 8$
final_round_b : false$
box_tran : aes_rbase_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for s in $(seq 1 20) do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c3_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
  done
done
       \endverbatim
       </li>
      </ul>
     </li>
    </ul>
   </li>
  </ul>


  \todo Minisat-2.2.0
  <ul>
   <li> Solving the key discovery problem over rounds 1 to 20. </li>
   <li> The canonical box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
     \endverbatim
     Experiments are currently running. </li>
    </ul>
   </li>
   <li> The "minimum" box translation (up to round 17):
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
     \endverbatim
     Experiments are currently running. </li>
    </ul>
   </li>
   <li> The 1-base box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
     \endverbatim
     Experiments are currently running. </li>
    </ul>
   </li>
  </ul>

*/
