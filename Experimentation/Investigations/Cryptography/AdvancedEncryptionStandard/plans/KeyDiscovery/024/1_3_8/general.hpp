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
shell> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 4); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
shell> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
r   rn    rc            t  sat         cfs         dec      rts           r1    mem  ptime stime          cfl
1 1208 17796 4.749215e-02 1.00      286.95     2730.85     3.05      60556.7   9.00 0.0095  0.02      51757.4
2 2344 35496 1.779539e+01 1.00    69910.15   244117.35   198.00   32444019.8  58.15 0.0120  0.04   14567690.3
3 3480 53196 4.800013e+01 1.00   148693.60   597392.90   387.55   67025301.9 114.40 0.0200  0.06   29870635.6
4 4616 70896 1.121947e+04 1.05 15353353.90 54250682.65 22731.75 5516558000.2 924.35 0.0265  0.08 5309395310.2
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
  r       t.mean         t.sd    t.min       t.max    cfs.mean       cfs.sd cfs.min  cfs.max      r1.mean        r1.sd   r1.min      r1.max
1 1 4.749215e-02 1.464625e-02 0.027995 7.39880e-02      286.95     223.0127      14      698      60556.7 4.613163e+04     3393      142167
2 2 1.779539e+01 1.879794e+01 0.057991 5.25750e+01    69910.15   65680.2979      23   213489   32444019.8 3.006823e+07    17318   100808694
3 3 4.800013e+01 4.436927e+01 7.603840 1.87271e+02   148693.60  102529.6673   44744   455063   67025301.9 5.386161e+07 12725413   228186522
4 4 1.121947e+04 8.334966e+03 0.000000 2.81446e+04 15353353.90 9778328.8799       0 35989537 5516558000.2 3.567265e+09        0 13009604735
     \endverbatim
     Note here that key with seed 4 returned and error, and hence we have
     "1.05" for "sat". This experiment must be run again (it errored after
     527 minutes). </li>
    </ul>
   </li>
   <li> The "minimum" box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
shell> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 4); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
shell> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
r  rn   rc            t         cfs        dec      rts           r1   mem ptime  stime          cfl
1 184 1560 2.424565e-02     2236.85     2662.7    12.95 2.588565e+04  8.00 0.000 0.0090     21949.85
2 296 3024 4.044634e+00   298957.00   344442.8   695.70 5.702301e+06  8.00 0.000 0.0100   4162284.00
3 408 4488 6.966964e+01  3505260.35  4099517.2  5931.15 9.290159e+07 12.90 0.000 0.0135  60130846.75
4 520 5952 1.281556e+03 26518153.95 30264840.0 36318.30 1.251430e+09 34.55 0.002 0.0200 545569858.95
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
r       t.mean         t.sd    t.min       t.max    cfs.mean       cfs.sd cfs.min  cfs.max      r1.mean        r1.sd  r1.min     r1.max
1 2.424565e-02 1.205448e-02 0.007998    0.048992     2236.85     1653.853     139     5744 2.588565e+04 1.920905e+04    1752      67017
2 4.044634e+00 3.772671e+00 0.038994   13.204000   298957.00   237780.895    3059   822904 5.702301e+06 5.106003e+06   45300   17958522
3 6.966964e+01 8.429120e+01 2.242660  403.495000  3505260.35  3446073.443  187972 16727967 9.290159e+07 1.023211e+08 3601114  492327148
4 1.281556e+03 1.300257e+03 6.483010 4401.220000 26518153.95 23545349.385  385027 79797406 1.251430e+09 1.189224e+09 9342287 3970533106
     \endverbatim
     </li>
    </ul>
   </li>
   <li> The 1-base box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
shell> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 4); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
shell> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
r  rn    rc            t        cfs        dec      rts           r1    mem  ptime  stime          cfl
1 184 17976    0.5649137      189.9      209.5     2.20      5976.95  10.00 0.0110 0.5300      1329.15
2 296 35856    6.6444390    26194.4    27582.7    92.20   1757050.00  12.35 0.0195 1.1900    392016.80
3 408 53736   68.3752300   432840.7   484224.7   941.70  18866602.75  29.60 0.0295 1.7290   7417296.55
4 520 71616 7500.9050500 16254372.2 17373478.6 22877.95 959566525.75 109.05 0.0395 2.3035 349172967.80
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
r       t.mean         t.sd      t.min       t.max   cfs.mean       cfs.sd cfs.min  cfs.max      r1.mean        r1.sd   r1.min     r1.max
1    0.5649137 1.314491e-02   0.544917     0.59091      189.9 1.200254e+02      12      392      5976.95 3.731548e+03      551      12763
2    6.6444390 4.469542e+00   1.457780    18.09520    26194.4 1.985138e+04    1409    76465   1757050.00 1.357439e+06    90491    5231210
3   68.3752300 6.246889e+01  13.360000   245.36900   432840.7 4.018269e+05   88015  1658292  18866602.75 1.755783e+07  3562554   69228347
4 7500.9050500 1.211908e+04 188.654000 45321.30000 16254372.2 2.536459e+07  733692 97761567 959566525.75 1.507831e+09 43611219 5837203969
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>

*/
