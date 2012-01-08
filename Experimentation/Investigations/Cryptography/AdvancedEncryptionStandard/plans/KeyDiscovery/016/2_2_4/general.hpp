// Matthew Gwynne, 18.7.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/016/2_2_4/general.hpp
  \brief Investigations into small-scale AES key discovery for AES with a 2x2 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
  <li> We consider the small-scale AES with 2 row, 2 column, using the 4-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,2,2,4) for r in 1,...,20. </li>
   <li> We investigate translations of the key discovery problem for
   aes(r,2,2,4) into SAT. </li>
   <li> aes(r,2,2,4) takes a 16-bit plaintext and 16-bit key and outputs a
   16-bit ciphertext. </li>
   <li> aes(r,2,2,4) applies the following operations:
    <ol>
     <li> Key schedule which takes the key and generates r+1 16-bit round
     keys. </li>
     <li> Application of the following operation (the "round") r times:
      <ol>
       <li> Addition of round key n-1. </li>
       <li> Application of Sbox operation to each byte. </li>
       <li> Application of the MixColumns operation. </li>
      </ol>
     </li>
     <li> Addition of round key r+1. </li>
     <li> The result of the last round key addition is the ciphertext. </li>
    </ol>
   </li>
   <li> Round key 0 is the input key. </li>
   <li> The key schedule computes the round key i from round key i-1 by:
   \verbatim
K_(i,1,k) := S-box(K_(i-1,2,2)) + C_i + sum(K_(i-1,j,l),l,1,j)
K_(i,2,k) := S-box(K_(i-1,1,2)) + sum(K_(i-1,j,l),l,1,j)
   \endverbatim
   where
    <ul>
     <li> C_i is the round constant for round i; </li>
     <li> K_(i,j,k) is the 4-bit word in the j-th row, k-th column of the i-th
     round-key considered as a 2x2 matrix. </li>
    </ul>
   </li>
   <li> The S-box is a permutation from {0,1}^4 to {0,1}^4 which we consider
   as either:
    <ul>
     <li> an 8x1 boolean function; see ss_sbox_bf in
     ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/AdvancedEncryptionStandard.mac;
     </li>
     <li> 4 4x1 boolean functions. </li>
    </ul>
   </li>
   <li> The MixColumns operation is a permutation from ({0,1}^4)^2 to
   ({0,1}^4)^2, which we consider to be defined as:
   \verbatim
MixColumns(I_1) := Mul02(I_1) + Mul03(I_2)
MixColumns(I_2) := Mul03(I_1) + Mul02(I_2)
MixColumns(I_3) := Mul02(I_3) + Mul03(I_4)
MixColumns(I_4) := Mul03(I_3) + Mul02(I_4)
   \endverbatim
   where
    <ul>
     <li> I_i is the i-th 4-bit word in the input; </li>
     <li> Mul02 is a permutation over {0,1}^4 representing multiplication
     by 02 in the Rijndael byte field; </li>
     <li> Mul03 is a permutation over {0,1}^4 representing multiplication
     by 03 in the Rijndael byte field. </li>
    </ul>
   </li>
   <li> The decompositions and translations are listed in "Investigating
   dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> The plaintext and ciphertext variables are then set, and the
   SAT solver is run on this instance to deduce the key variables. </li>
  </ul>


  \todo Translations
  <ul>
   <li> The following translations are considered in this %plans %file:
    <ul>
     <li> The canonical box translation. </li>
     <li> The 1-base translation. </li>
     <li> The small box translation. </li>
     <li> The canonical core-round translation. </li>
     <li> The 1-base core-round translation. </li>
    </ul>
   </li>
   <li> For a full list of the possible translations, see
   "Investigating dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> Generating instances for rounds 1-20 for a single random key with each
   of the translations:
    <ul>
     <li> The canonical box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_canon_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_canon_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 2$
num_columns : 2$
exp : 4$
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
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw2_e4_f0.cnf ssaes_pcpair_r${r}_c2_rw2_e4_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
     <li> The 1-base box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_1base_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_1base_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 2$
num_columns : 2$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
mc_tran : aes_mc_bidirectional$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw2_e4_f0.cnf ssaes_pcpair_r${r}_c2_rw2_e4_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
     <li> The "minimum" box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c1_rw1_e4_f0_k1-20_aes_min_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c1_rw1_e4_f0_k1-20_aes_min_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 2$
num_columns : 2$
exp : 4$
final_round_b : false$
box_tran : aes_small_box$
mc_tran : aes_mc_bidirectional$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw2_e4_f0.cnf ssaes_pcpair_r${r}_c2_rw2_e4_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
     <li> The canonical box translation (forward directional MixColumns):
     \verbatim
shell> mkdir ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_canon_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_canon_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 2$
num_columns : 2$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
mc_tran : aes_mc_forward$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw2_e4_f0.cnf ssaes_pcpair_r${r}_c2_rw2_e4_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
     <li> The 1-base box translation (forward directional MixColumns):
     \verbatim
shell> mkdir ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_1base_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_1base_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 2$
num_columns : 2$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
mc_tran : aes_mc_forward$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw2_e4_f0.cnf ssaes_pcpair_r${r}_c2_rw2_e4_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
     <li> The "minimum" box translation (forward directional MixColumns):
     \verbatim
shell> mkdir ssaes_r1-20_c1_rw1_e4_f0_k1-20_aes_min_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c1_rw1_e4_f0_k1-20_aes_min_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 2$
num_columns : 2$
exp : 4$
final_round_b : false$
box_tran : aes_small_box$
mc_tran : aes_mc_forward$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw2_e4_f0.cnf ssaes_pcpair_r${r}_c2_rw2_e4_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
     <li> The canonical box core round translation:
     \verbatim
shell> mkdir ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_ts_box_aes_core_round_box
shell> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_ts_box_aes_core_round_box
shell> oklib --maxima
num_rows : 2$
num_columns : 2$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
mc_tran : aes_core_round_box$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw2_e4_f0.cnf ssaes_pcpair_r${r}_c2_rw2_e4_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
     <li> The 1-base box core round translation:
     \verbatim
shell> mkdir ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_1base_box_aes_core_round_box
shell> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_1base_box_aes_core_round_box

# Generating 1-base
maxima> output_fcs("16-bit AES Round",bf2relation_fullcnf_fcs(lambda([V],ss_round_column_bf(V,2,4)), 8),"round_column_16_full.cnf")$
shell> gen_seed=10; base_seed=5;
shell> QuineMcCluskey-n16-O3-DNDEBUG round_column_16_full.cnf > round_column_16_primes.cnf
shell> cat round_column_16_primes.cnf | RandomShuffleDimacs-O3-DNDEBUG ${gen_seed} | SortByClauseLength-O3-DNDEBUG > round_column_16_primes.cnf_sorted
shell> RUcpGen-O3-DNDEBUG round_column_16_primes.cnf_sorted > round_column_16_primes.cnf_gen
shell> cat round_column_16_primes.cnf_gen | RandomShuffleDimacs-O3-DNDEBUG ${base_seed} | SortByClauseLength-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > round_column_16_r1base.cnf
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 2712 18667 0 18667 0 1
 length count
4 4
5 129
6 604
7 1420
8 553
9 2

# Generating instance
shell> oklib --maxima
set_hm(ss_round_column_rbase_cnfs,[4,2],read_fcl_f("round_column_16_r1base.cnf"))$
num_rows : 2$
num_columns : 2$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
mc_tran : aes_core_round_box$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw2_e4_f0.cnf ssaes_pcpair_r${r}_c2_rw2_e4_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo Minisat-2.2.0
  <ul>
   <li> Solving the key discovery problem over rounds 1 to 20. </li>
   <li> The environment for all of these experiments is:
   \verbatim
processor	: 0
vendor_id	: GenuineIntel
cpu family	: 6
model		: 23
model name	: Intel(R) Core(TM)2 Duo CPU     E8400  @ 3.00GHz
cpu MHz		: 2003.000
cache size	: 6144 KB
bogomips	: 5999.94

processor	: 1
vendor_id	: GenuineIntel
cpu family	: 6
model		: 23
model name	: Intel(R) Core(TM)2 Duo CPU     E8400  @ 3.00GHz
cpu MHz		: 2003.000
cache size	: 6144 KB
bogomips	: 5999.64
address sizes	: 36 bits physical, 48 bits virtual
             total       used       free     shared    buffers     cached
Mem:          3947       3363        583          0        165       3077
   \endverbatim
   </li>
   <li> Solving for twenty random plaintext/key/ciphertext combinations:
    <ul>
     <li> The canonical box translation:
      <ul>
       <li> The data:
       \verbatim
> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_ts_box_aes_mc_bidirectional
> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r   rn    rc           t      cfs      dec    rts           r1   mem  ptime  stime       cfl
 1  540  3510  0.01364755   311.55   458.30   3.15     34670.05  8.00 0.0000 0.0040   6368.05
 2 1032  6956  0.52856920 10837.15 12999.65  44.15   2427005.20  8.00 0.0010 0.0100 272529.15
 3 1524 10402  3.28250205 26278.20 27781.85  89.95  20810840.35  9.00 0.0035 0.0100 954880.90
 4 2016 13848  5.31044540 35558.00 39499.35 114.20  32576476.90  9.15 0.0090 0.0150 936477.45
 5 2508 17294  7.59299250 40403.35 45242.15 127.65  46406352.30  9.70 0.0090 0.0200 991025.55
 6 3000 20740  8.80541605 38345.70 42997.90 119.40  54246717.35 11.10 0.0100 0.0200 998552.00
 7 3492 24186 10.65598400 38386.70 43008.35 119.85  66994155.45 11.40 0.0105 0.0285 995404.25
 8 3984 27632 13.75685800 42818.95 47195.65 135.15  95837908.30 11.45 0.0115 0.0300 783800.25
 9 4476 31078 13.53658925 35053.85 38284.85 113.20  94843511.25 12.10 0.0125 0.0300 643052.30
10 4968 34524 17.43140000 39946.25 43722.30 128.45 122934989.50 12.50 0.0145 0.0400 714082.20
11 5460 37970 13.39596195 26641.65 28909.70  88.35  98016274.65 12.20 0.0175 0.0400 394013.55
12 5952 41416 12.69576450 24135.60 26453.70  81.15  91926358.10 11.85 0.0195 0.0455 374059.70
13 6444 44862 14.02387100 25260.40 28000.20  86.70  97402795.30 13.35 0.0200 0.0500 518350.30
14 6936 48308 19.95726750 31275.95 34450.55 102.95 139748613.35 14.50 0.0200 0.0500 593582.65
15 7428 51754 23.62795900 32862.75 35642.75 108.90 168457126.95 14.60 0.0210 0.0600 511311.95
16 7920 55200 22.59226755 29893.55 32419.05  98.55 164044405.40 14.30 0.0215 0.0600 417161.70
17 8412 58646 29.02898550 36228.55 39868.50 116.55 198818657.00 15.65 0.0260 0.0660 632841.80
18 8904 62092 36.92544100 44825.85 48974.10 138.35 256134720.95 16.30 0.0270 0.0700 829887.60
19 9396 65538 24.63130000 25950.45 27963.80  88.75 179737555.95 14.50 0.0295 0.0700 381322.95
20 9888 68984 28.53451635 28556.25 30854.75  94.55 209100968.95 15.05 0.0295 0.0795 437981.90
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r      t.mean        t.sd    t.min     t.max cfs.mean     cfs.sd cfs.min cfs.max      r1.mean        r1.sd   r1.min    r1.max
 1  0.01364755  0.00380069 0.007998  0.020996   311.55   175.4853      45     679     34670.05     21861.08     3973     71767
 2  0.52856920  0.47462842 0.032994  1.527770 10837.15  9356.6823     493   29797   2427005.20   2193960.87   114730   6965906
 3  3.28250205  3.04433188 0.238963 11.997200 26278.20 24163.5893    2182   96524  20810840.35  18912756.91  1599966  74526355
 4  5.31044540  3.84273459 0.291955 14.621800 35558.00 24680.5577    2363   93032  32576476.90  23178179.33  2024899  89241205
 5  7.59299250  5.38290478 0.238963 18.321200 40403.35 27938.8579    1079   94028  46406352.30  31187239.53  1686374 103730602
 6  8.80541605  8.79948827 0.088986 30.852300 38345.70 37009.3237     348  126784  54246717.35  51524487.02   395905 174486330
 7 10.65598400 10.23344924 1.365790 36.267500 38386.70 34152.2989    4241  117408  66994155.45  59263707.78 10392886 219853322
 8 13.75685800  9.39435408 1.258810 35.366600 42818.95 27785.3154    5343  109347  95837908.30  64439468.66  8834612 244522695
 9 13.53658925 10.28209964 0.145977 39.055100 35053.85 26250.5232     377  105019  94843511.25  69385660.19   756424 248190186
10 17.43140000 11.31897442 2.952550 39.804900 39946.25 25820.9817    9392   96340 122934989.50  79316638.50 20441034 279157839
11 13.39596195 10.14389586 0.921859 29.492500 26641.65 20519.2279    1617   61358  98016274.65  73403362.31  6747905 220758156
12 12.69576450 13.27416307 1.034840 48.621600 24135.60 25609.0495    1640   92048  91926358.10  93317246.12  7488554 340472807
13 14.02387100 12.85541312 1.172820 47.303800 25260.40 23329.9009    2021   87260  97402795.30  85516291.03  8213684 322150652
14 19.95726750 15.97387911 1.751730 54.345700 31275.95 24038.9577    4261   78496 139748613.35 111205029.42 12222671 400022220
15 23.62795900 18.43601343 2.466630 60.065900 32862.75 26724.5385    4657   90353 168457126.95 126238067.40 17851589 426652808
16 22.59226755 21.24390199 0.646901 61.781600 29893.55 27561.5109    1244   84502 164044405.40 153273685.96  4273347 455410417
17 29.02898550 21.45708606 1.261810 69.340500 36228.55 27136.1557    1603   85710 198818657.00 145345493.10  8959076 485034533
18 36.92544100 22.65076273 6.133070 77.896200 44825.85 27120.0917    6450   95215 256134720.95 151266017.95 45679923 504564025
19 24.63130000 18.14335531 2.294650 63.679300 25950.45 20546.4704    2243   77601 179737555.95 127112041.70 16815129 415492408
20 28.53451635 21.03831651 0.477927 69.485400 28556.25 20571.9374     412   62344 209100968.95 154818458.08  2795026 520146157
       \endverbatim
       </li>
      </ul>
     </li>
     <li> The 1-base box translation:
      <ul>
       <li> The data:
       \verbatim
> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_1base_box_aes_mc_bidirectional
> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r   rn    rc          t      cfs      dec    rts          r1       cfl
 1  188   754  0.0033989   149.25   167.20   2.00     5571.15    1091.2
 2  328  1444  0.1560758  9554.80 10209.45  39.00   640685.60  114874.9
 3  468  2134  1.7480837 52231.00 55363.10 150.00  7364175.45  908525.4
 4  608  2824  2.0001453 43808.65 47463.50 137.55  8759026.05  797085.0
 5  748  3514  4.0880298 75280.30 83037.55 205.25 17468998.00 1267144.1
 6  888  4204  4.1918608 64368.95 71701.85 186.75 18422076.85 1062059.1
 7 1028  4894  2.7766765 33450.45 37027.45 105.55 12802953.05  529184.2
 8 1168  5584  4.7316281 44982.05 49125.90 138.45 22347642.85  667635.8
 9 1308  6274  7.1040173 56690.55 61099.80 168.35 33984877.60  811868.8
10 1448  6964  5.8859056 37890.80 40142.35 114.90 29864289.95  454932.8
11 1588  7654  5.8619576 36815.40 39793.35 111.45 29528091.75  480677.7
12 1728  8344  5.9062527 33981.20 36910.90 102.80 28896753.80  450667.2
13 1868  9034  5.4869180 25580.80 27103.45  89.95 28757272.15  292767.2
14 2008  9724  6.4771164 28428.80 30244.95  93.50 33727804.95  326879.2
15 2148 10414  8.1572598 32502.25 34358.70 108.25 43086990.80  369439.3
16 2288 11104  4.6316998 17430.15 18699.40  65.35 24405027.60  205685.2
17 2428 11794  7.4363641 26182.80 27794.30  86.55 39030026.15  297554.6
18 2568 12484 12.0784696 39807.70 41919.05 129.80 64033641.05  443380.7
19 2708 13174  9.6543214 31074.10 33154.90 102.75 49705985.05  356674.3
20 2848 13864 11.1623086 34985.65 37356.40 117.90 58262530.40  413615.5
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r     t.mean        t.sd    t.min     t.max cfs.mean      cfs.sd cfs.min cfs.max     r1.mean        r1.sd  r1.min    r1.max
 1  0.0033989 0.001535297 0.000999  0.006998   149.25    76.93324      16     268     5571.15     3059.368     626     11341
 2  0.1560758 0.149666444 0.005999  0.475927  9554.80  9107.44314     302   28841   640685.60   621235.899   16695   1965641
 3  1.7480837 2.067959587 0.021996  7.716830 52231.00 60514.62615     630  224665  7364175.45  8471372.984   84222  31529002
 4  2.0001453 1.687959968 0.122981  6.403030 43808.65 35884.04656    2890  136380  8759026.05  7121334.669  565742  27123823
 5  4.0880298 4.268342443 0.011998 15.227700 75280.30 77703.57190     160  283868 17468998.00 17888107.935   37119  63926946
 6  4.1918608 4.653834444 0.037994 21.633700 64368.95 67516.45027     608  311437 18422076.85 19575487.753  151895  90937433
 7  2.7766765 3.435826942 0.055991 12.420100 33450.45 43200.48758     635  160552 12802953.05 15260336.444  250702  54799738
 8  4.7316281 4.589674888 0.206968 17.631300 44982.05 43589.16807    1906  165889 22347642.85 20786439.396 1031891  79870631
 9  7.1040173 5.767823674 0.051992 20.353900 56690.55 48742.67302     489  164951 33984877.60 26516092.258  221584  93867925
10  5.8859056 6.128744937 0.074988 18.390200 37890.80 39107.56240     573  118494 29864289.95 30908077.130  331268  92740350
11  5.8619576 6.646366644 0.017997 26.639900 36815.40 49213.00587      84  216808 29528091.75 31831003.694   35778 122235035
12  5.9062527 9.432870647 0.410937 43.417400 33981.20 62265.47322    2324  287865 28896753.80 42431376.510 2155226 194422363
13  5.4869180 5.422786285 0.045993 18.040300 25580.80 24953.79947     355   82977 28757272.15 28251040.936  183751  94889249
14  6.4771164 6.983112921 0.066989 21.870700 28428.80 30337.83387     505   95707 33727804.95 36100424.018  288704 113209190
15  8.1572598 4.831169461 0.680896 17.447300 32502.25 19061.75238    2893   68777 43086990.80 25494928.068 3602426  92411948
16  4.6316998 3.339272969 0.236963 10.744400 17430.15 12324.78011     988   39743 24405027.60 17580579.764 1203652  56516640
17  7.4363641 6.546561872 0.313952 27.034900 26182.80 22999.98874    1194   95872 39030026.15 34325751.357 1585152 141515365
18 12.0784696 7.462974168 0.443932 25.003200 39807.70 24466.38156    1572   81718 64033641.05 39375343.315 2318635 133707616
19  9.6543214 6.874520606 0.545917 28.683600 31074.10 21934.51738    2122   91407 49705985.05 35063694.847 2849649 146505610
20 11.1623086 7.148213257 0.578911 24.280300 34985.65 22973.41214    1890   77930 58262530.40 36679400.730 3052344 127591817
       \endverbatim
       </li>
      </ul>
     </li>
     <li> The minimum box translation:
      <ul>
       <li> The data:
       \verbatim
> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_min_box_aes_mc_bidirectional
> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r   rn    rc           t      cfs      dec    rts         r1        cfl
 1  188   652  0.00399900   189.10   218.55   2.30     8004.9    1433.75
 2  328  1240  0.09173555  6086.15  6744.05  27.70   399729.2   73145.90
 3  468  1828  1.61945150 54476.40 58758.15 156.95  7710914.2  886590.40
 4  608  2416  3.13857625 67966.75 73882.05 193.30 14723972.9 1168931.00
 5  748  3004  3.48311875 60262.65 66155.40 173.20 16527541.5 1008596.40
 6  888  3592  3.52286330 51763.60 57700.45 156.70 17195480.1  863468.90
 7 1028  4180  4.12576975 49992.50 56043.80 145.75 20479424.9  797402.45
 8 1168  4768  7.65413675 76221.15 84477.80 205.30 38234517.1 1157473.95
 9 1308  5356  5.85310895 55308.05 62975.00 159.70 29440586.1  876474.85
10 1448  5944  6.56200270 53253.15 60614.05 158.50 33275258.6  833880.95
11 1588  6532  7.21875565 52129.70 59756.00 155.75 36414104.2  797379.00
12 1728  7120  5.54640820 36904.75 43719.00 118.00 28490370.9  573733.75
13 1868  7708  3.89255725 25057.25 31165.25  88.40 19766465.8  397743.30
14 2008  8296  9.70932785 55377.75 64801.25 167.70 48039657.2  857162.55
15 2148  8884  8.64913855 44668.95 53716.75 138.75 43275406.6  672921.40
16 2288  9472 11.71262080 53850.50 63706.40 164.50 58616273.5  778701.05
17 2428 10060 11.11050870 49083.55 59035.95 147.80 55933750.2  719613.40
18 2568 10648 14.24123650 62073.15 73910.75 184.40 70225773.3  941919.60
19 2708 11236 11.50096080 48416.60 59446.80 150.65 57770304.3  755932.80
20 2848 11824 13.72051850 53076.90 65159.55 158.80 69327930.4  810749.45
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r      t.mean         t.sd    t.min     t.max cfs.mean     cfs.sd cfs.min cfs.max    r1.mean        r1.sd   r1.min    r1.max
 1  0.00399900  0.001486784 0.000999  0.005999   189.10   101.6826      32     363     8004.9     4545.466     1029     15908
 2  0.09173555  0.084537287 0.010998  0.318951  6086.15  5741.8835     449   20822   399729.2   385269.245    27196   1408700
 3  1.61945150  2.286073979 0.019996 10.271400 54476.40 76764.6619     598  348564  7710914.2 10335818.454    72398  46236148
 4  3.13857625  3.565729730 0.108983 12.823100 67966.75 72801.8548    2573  263125 14723972.9 16209855.604   526418  57783485
 5  3.48311875  4.258006399 0.310952 15.756600 60262.65 72631.1150    6372  263386 16527541.5 19614362.417  1561725  73081045
 6  3.52286330  3.267820921 0.071989 14.548800 51763.60 47516.7669    1406  211490 17195480.1 15169212.037   315293  67203720
 7  4.12576975  4.332333569 0.081987 15.776600 49992.50 53442.2552    1915  212129 20479424.9 20913351.135   347039  75274789
 8  7.65413675  9.080496347 0.119981 29.281500 76221.15 85268.2367    2576  279459 38234517.1 44419883.133   540013 142762980
 9  5.85310895  5.976899996 0.195970 22.386600 55308.05 57892.3487    3073  225902 29440586.1 29368735.436   898288 107704674
10  6.56200270  5.923326849 0.077988 22.203600 53253.15 49290.6745    2274  193190 33275258.6 29177219.740   345761 107504599
11  7.21875565  8.496822113 0.349946 23.617400 52129.70 57288.0377    5258  161517 36414104.2 42851379.137  1552259 117132819
12  5.54640820  4.149040940 0.695894 14.769800 36904.75 26609.0104    6976   97417 28490370.9 20903079.597  3416364  74735853
13  3.89255725  2.444202784 0.950855  8.209750 25057.25 13992.3238    8703   60012 19766465.8 12745670.425  4487228  42896394
14  9.70932785 13.038447682 0.503923 52.801000 55377.75 74042.9085    6029  308619 48039657.2 62635610.399  2230480 248479494
15  8.64913855  7.240719748 0.582911 27.225900 44668.95 34472.3907    6867  135621 43275406.6 36758245.414  2670325 138981155
16 11.71262080  8.047231804 0.749886 31.540200 53850.50 35282.6621    6799  151267 58616273.5 39697761.325  3429439 150393377
17 11.11050870 10.768603777 0.378942 34.180800 49083.55 43427.0985    6649  146145 55933750.2 54887410.941  1667981 176208555
18 14.24123650 12.596970963 1.109830 44.925200 62073.15 48825.3462   10048  174863 70225773.3 61984801.953  5165453 222570361
19 11.50096080  8.812440420 0.744886 32.414100 48416.60 32724.7134   10936  128072 57770304.3 44683566.568  3240968 162990175
20 13.72051850 10.702400699 2.915560 36.349500 53076.90 35819.9058   18573  128249 69327930.4 54970747.001 13689144 185784444
         \endverbatim
         </li>
         <li> The canonical box translation (forward directional MixColumns):
          <ul>
           <li> The data:
           \verbatim
> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_ts_box_aes_mc_forward
> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
    r   rn    rc          t sat      cfs      dec    rts           r1   mem  ptime  stime       cfl  r    s
1   1  380  2286  0.0092981   1   228.95   340.15   2.60     17554.55  8.00 0.0000 0.0000    4312.3  1 10.5
2   2  712  4508  0.2794070   1  7969.20 10043.05  33.75   1088819.15  8.00 0.0000 0.0025  187364.0  2 10.5
3   3 1044  6730  3.8534594   1 49178.00 57672.40 143.65  17943795.50  8.65 0.0020 0.0100 1326427.4  3 10.5
4   4 1376  8952  4.6529401   1 47495.90 56525.85 138.90  23489338.30  9.00 0.0030 0.0100 1207678.6  4 10.5
5   5 1708 11174  8.0313296   1 53355.20 59713.05 160.85  44763062.55  9.10 0.0055 0.0100 1139402.6  5 10.5
6   6 2040 13396  6.5609030   1 31747.95 34043.05 105.15  41458871.75  9.00 0.0085 0.0150  456987.5  6 10.5
7   7 2372 15618  7.6048417   1 30586.85 32665.75 102.50  48919489.70  9.05 0.0085 0.0200  416239.3  7 10.5
8   8 2704 17840  8.7335194   1 29811.70 31570.50  99.30  57994516.80 10.00 0.0100 0.0200  350034.9  8 10.5
9   9 3036 20062  7.8488613   1 23752.55 25342.80  83.55  52480995.10 10.00 0.0100 0.0200  283849.0  9 10.5
10 10 3368 22284  7.9720381   1 21506.85 23014.85  75.60  53473458.30 10.05 0.0105 0.0265  256527.2 10 10.5
11 11 3700 24506 13.6251819   1 33083.80 35169.45 108.85  91407340.50 10.50 0.0100 0.0300  387371.8 11 10.5
12 12 4032 26728 14.4830910   1 32006.15 34031.95 104.40  96760698.35 10.65 0.0115 0.0300  379026.1 12 10.5
13 13 4364 28950 11.3472780   1 23638.70 25406.25  83.00  74391645.70 10.45 0.0115 0.0300  295670.7 13 10.5
14 14 4696 31172 16.0557603   1 30051.80 31723.15  98.80 108842312.45 11.45 0.0140 0.0380  347368.8 14 10.5
15 15 5028 33394 16.9988170   1 30971.30 33121.25 104.60 110522684.60 11.60 0.0155 0.0400  385447.4 15 10.5
16 16 5360 35616 17.2213320   1 30105.35 32515.45 100.50 110628578.60 11.95 0.0165 0.0400  383600.5 16 10.5
17 17 5692 37838 15.7411005   1 24941.90 26846.70  87.55 104247264.80 12.30 0.0185 0.0400  317847.6 17 10.5
18 18 6024 40060 21.9853069   1 33684.00 36265.80 108.75 142969008.75 12.90 0.0185 0.0485  434341.0 18 10.5
19 19 6356 42282 19.3864000   1 28219.85 30595.15  95.35 126462114.65 12.60 0.0200 0.0500  359165.0 19 10.5
20 20 6688 44504 21.0597084   1 28553.95 30716.70  95.05 138119136.95 12.85 0.0200 0.0500  351637.5 20 10.5
         \endverbatim
         </li>
        </ul>
       </li>
       <li> The 1-base box translation  (forward directional MixColumns):
        <ul>
         <li> The data:
         \verbatim
> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_1base_box_aes_mc_forward
> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
    r   rn   rc          t sat      cfs      dec    rts         r1 mem  ptime  stime       cfl  r    s
1   1  156  554 0.00324895   1   200.30   228.30   2.35     5113.6   8 0.0000 0.0000    1449.1  1 10.5
2   2  264 1044 0.16842390   1 13365.10 14424.65  50.75   664523.6   8 0.0000 0.0000  162438.8  2 10.5
3   3  372 1534 1.09448335   1 45381.90 50532.55 136.65  4226637.3   8 0.0000 0.0000  802966.1  3 10.5
4   4  480 2024 2.20406465   1 70909.65 80150.95 199.10  8604571.7   8 0.0000 0.0000 1202218.1  4 10.5
5   5  588 2514 2.46492420   1 60073.65 68051.80 170.85 10060853.1   8 0.0000 0.0005 1003174.2  5 10.5
6   6  696 3004 4.18656745   1 80621.60 90719.60 218.85 17316692.6   8 0.0000 0.0045 1302213.4  6 10.5
7   7  804 3494 4.15092015   1 63270.15 70527.25 181.30 17519616.2   8 0.0000 0.0015 1023470.0  7 10.5
8   8  912 3984 3.02129045   1 36797.55 40270.95 112.10 13530365.8   8 0.0000 0.0070  557536.9  8 10.5
9   9 1020 4474 2.83752005   1 27481.35 29494.25  91.35 13533895.4   8 0.0000 0.0075  367796.2  9 10.5
10 10 1128 4964 5.87715310   1 50111.25 53142.20 148.55 28468014.6   8 0.0000 0.0060  629794.0 10 10.5
11 11 1236 5454 3.54571350   1 26476.15 28205.60  88.55 17649358.6   9 0.0010 0.0100  316967.3 11 10.5
12 12 1344 5944 4.93014800   1 32934.55 34875.10 109.30 24924266.3   9 0.0020 0.0100  376871.2 12 10.5
13 13 1452 6434 5.36238825   1 32753.30 34491.75 104.10 27397533.6   9 0.0015 0.0100  364729.8 13 10.5
14 14 1560 6924 4.89585255   1 27867.55 29438.50  97.00 25430465.8   9 0.0025 0.0100  319360.3 14 10.5
15 15 1668 7414 7.41972480   1 39482.00 41573.85 123.65 37921117.1   9 0.0035 0.0100  446113.8 15 10.5
16 16 1776 7904 7.95934675   1 44933.75 48518.00 135.40 39291875.6   9 0.0050 0.0100  587084.4 16 10.5
17 17 1884 8394 5.65869050   1 26330.95 27901.60  91.45 29344639.9   9 0.0090 0.0100  302157.5 17 10.5
18 18 1992 8884 8.96828960   1 39799.55 42073.25 127.90 45992723.9   9 0.0060 0.0100  447073.3 18 10.5
19 19 2100 9374 6.26964700   1 26452.50 28211.85  88.45 32182672.4   9 0.0075 0.0100  302180.2 19 10.5
20 20 2208 9864 7.93763830   1 31675.15 33459.20 101.95 41408154.9   9 0.0075 0.0100  352688.5 20 10.5
         \endverbatim
         </li>
        </ul>
       </li>
       <li> The minimum box translation (forward directional MixColumns):
        <ul>
         <li> The data:
         \verbatim
> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_min_box_aes_mc_forward
> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
    r   rn   rc          t sat        cfs        dec     rts           r1   mem  ptime  stime         cfl  r    s
1   1  156  488  0.0039489   1     235.20     275.55    2.70      6633.75  8.00 0.0000 0.0000     1808.15  1 10.5
2   2  264  912  0.1360789   1   11531.00   12728.45   45.65    580282.75  8.00 0.0000 0.0000   141793.20  2 10.5
3   3  372 1336  1.3900885   1   62364.40   69124.05  180.50   5879966.65  8.00 0.0000 0.0000  1029253.00  3 10.5
4   4  480 1760  2.1767687   1   69840.00   79339.10  208.70   9155641.70  8.00 0.0000 0.0000  1181436.70  4 10.5
5   5  588 2184  2.7028886   1   66915.80   76594.35  190.00  11511994.50  8.00 0.0000 0.0000  1117890.20  5 10.5
6   6  696 2608  3.3925827   1   68286.15   78482.45  194.00  14583667.55  8.00 0.0000 0.0010  1144257.65  6 10.5
7   7  804 3032  6.9664348   1  113079.15  128935.35  304.50  29862000.75  8.00 0.0000 0.0005  1901669.95  7 10.5
8   8  912 3456  4.0499788   1   59948.20   70366.25  171.55  17662378.20  8.00 0.0000 0.0010  1027623.15  8 10.5
9   9 1020 3880  3.7727732   1   52083.00   62862.55  157.75  16720895.10  8.00 0.0000 0.0055   947847.05  9 10.5
10 10 1128 4304  7.8122147   1  108640.95  131874.10  293.20  32451447.15  8.00 0.0000 0.0060  2166319.40 10 10.5
11 11 1236 4728  8.4633636   1  111472.45  137186.50  304.70  35444653.90  8.00 0.0000 0.0075  2335677.85 11 10.5
12 12 1344 5152 11.3586135   1  158892.40  199793.05  412.05  45162727.50  8.15 0.0000 0.0095  3795119.50 12 10.5
13 13 1452 5576 10.8882925   1  166039.40  212856.95  427.40  41781765.35  9.00 0.0010 0.0100  4295960.60 13 10.5
14 14 1560 6000 14.9887125   1  231897.35  297024.80  568.05  55144179.70  9.05 0.0010 0.0100  6152835.70 14 10.5
15 15 1668 6424 24.4547300   1  348896.20  442507.15  814.95  86045518.10  9.95 0.0030 0.0100  9610170.55 15 10.5
16 16 1776 6848 24.1164870   1  360840.60  460758.10  828.00  81475191.80 10.65 0.0040 0.0100 10209225.90 16 10.5
17 17 1884 7272 52.4160035   1  733047.45  916762.80 1481.00 163263176.50 13.50 0.0050 0.0100 21433528.40 17 10.5
18 18 1992 7696 45.7999325   1  625816.15  788618.00 1295.80 143197008.80 13.00 0.0040 0.0100 18304852.60 18 10.5
19 19 2100 8120 60.1304885   1  833884.65 1051830.75 1706.30 176895388.05 15.35 0.0055 0.0100 25233506.20 19 10.5
20 20 2208 8544 94.1981145   1 1186513.95 1486910.85 2363.85 265792310.00 17.95 0.0070 0.0100 35996778.70 20 10.5
       \endverbatim
       </li>
       <li> The canonical box core-round translation:
        <ul>
         <li> The data:
         \verbatim
> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_ts_box_aes_core_round_box
> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
    r    rn     rc          t sat      cfs       dec    rts           r1   mem  ptime  stime         cfl  r    s
1   1   652   9188  0.0220961   1   270.45   3382.30   2.95     25501.85  9.00 0.0000 0.0100    26555.65  1 10.5
2   2  1256  18312  2.2349100   1 15705.15 108398.45  59.45   3275674.00 20.35 0.0090 0.0175  2867594.45  2 10.5
3   3  1860  27436  6.0271298   1 34430.90 102595.95 110.90  21245071.85 35.80 0.0100 0.0200  6290467.30  3 10.5
4   4  2464  36560 21.2028254   1 71376.65 269625.90 206.15  54168390.30 59.75 0.0105 0.0300 12737835.60  4 10.5
5   5  3068  45684 19.0071160   1 60435.10 217418.05 180.45  62095050.05 60.75 0.0175 0.0395 10965927.30  5 10.5
6   6  3672  54808 20.4612305   1 58506.35 168601.15 175.65  95061022.40 50.80 0.0200 0.0400  7569535.80  6 10.5
7   7  4276  63932 17.3204601   1 43730.70 117769.95 133.55  84254339.20 38.65 0.0200 0.0500  5909088.10  7 10.5
8   8  4880  73056 16.9428649   1 36255.15  90779.85 115.10  98560071.80 32.80 0.0290 0.0600  4059931.35  8 10.5
9   9  5484  82180 33.9713900   1 67902.20 218239.65 196.85 150900199.85 72.85 0.0300 0.0650 10479257.30  9 10.5
10 10  6088  91304 21.1601700   1 44625.35  63818.80 142.00 168985806.40 21.40 0.0315 0.0700  1351395.85 10 10.5
11 11  6692 100428 27.6508220   1 47192.60 129999.40 139.80 154637696.85 46.35 0.0355 0.0800  5926670.95 11 10.5
12 12  7296 109552 31.6057780   1 50707.50 124002.40 150.10 196678014.40 52.65 0.0400 0.0865  5977192.75 12 10.5
13 13  7900 118676 24.5251280   1 36104.15  70356.00 114.45 174199903.80 28.35 0.0420 0.0915  2544256.65 13 10.5
14 14  8504 127800 31.0614745   1 49192.55 152137.80 148.60 197865430.15 51.90 0.0455 0.1000  6338464.70 14 10.5
15 15  9108 136924 30.0157824   1 38852.25  97984.90 122.65 205648425.75 44.75 0.0495 0.1100  4930653.25 15 10.5
16 16  9712 146048 25.5498675   1 34334.75  89285.90 109.60 181525639.40 38.55 0.0545 0.1125  4225013.75 16 10.5
17 17 10316 155172 25.5526680   1 32007.95  88997.75 105.00 181161419.20 36.85 0.0585 0.1235  3729837.15 17 10.5
18 18 10920 164296 40.7553485   1 44876.55 100632.60 138.80 317226512.70 41.55 0.0610 0.1300  4138549.35 18 10.5
19 19 11524 173420 26.1210730   1 25248.65  42053.10  83.30 222279231.00 24.40 0.0635 0.1400  1071781.10 19 10.5
20 20 12128 182544 55.3055095   1 53360.35 104595.35 163.40 373384461.60 37.40 0.0695 0.1445  3210131.00 20 10.5
         \endverbatim
         </li>
        </ul>
       </li>
       <li> The 1-base box core-round translation:
        <ul>
         <li> The data:
         \verbatim
> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_1base_box_aes_core_round_box
> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
    r   rn   rc         t sat      cfs      dec    rts         r1 mem ptime  stime       cfl  r    s
1   1  156  554 0.0023490   1   200.30   228.30   2.35     5113.6   8 0e+00 0.0000    1449.1  1 10.5
2   2  264 1044 0.1661242   1 13365.10 14424.65  50.75   664523.6   8 0e+00 0.0000  162438.8  2 10.5
3   3  372 1534 1.0946825   1 45381.90 50532.55 136.65  4226637.3   8 0e+00 0.0000  802966.1  3 10.5
4   4  480 2024 2.2011151   1 70909.65 80150.95 199.10  8604571.7   8 0e+00 0.0000 1202218.1  4 10.5
5   5  588 2514 2.4522760   1 60073.65 68051.80 170.85 10060853.1   8 0e+00 0.0000 1003174.2  5 10.5
6   6  696 3004 4.1750181   1 80621.60 90719.60 218.85 17316692.6   8 0e+00 0.0000 1302213.4  6 10.5
7   7  804 3494 4.1516193   1 63270.15 70527.25 181.30 17519616.2   8 0e+00 0.0000 1023470.0  7 10.5
8   8  912 3984 3.0210411   1 36797.55 40270.95 112.10 13530365.8   8 0e+00 0.0005  557536.9  8 10.5
9   9 1020 4474 2.8337196   1 27481.35 29494.25  91.35 13533895.4   8 0e+00 0.0045  367796.2  9 10.5
10 10 1128 4964 5.8909521   1 50111.25 53142.20 148.55 28468014.6   8 0e+00 0.0030  629794.0 10 10.5
11 11 1236 5454 3.5313646   1 26476.15 28205.60  88.55 17649358.6   9 0e+00 0.0085  316967.3 11 10.5
12 12 1344 5944 4.9361961   1 32934.55 34875.10 109.30 24924266.3   9 0e+00 0.0095  376871.2 12 10.5
13 13 1452 6434 5.3597388   1 32753.30 34491.75 104.10 27397533.6   9 0e+00 0.0100  364729.8 13 10.5
14 14 1560 6924 4.8854086   1 27867.55 29438.50  97.00 25430465.8   9 0e+00 0.0100  319360.3 14 10.5
15 15 1668 7414 7.4153699   1 39482.00 41573.85 123.65 37921117.1   9 1e-03 0.0100  446113.8 15 10.5
16 16 1776 7904 7.9600922   1 44933.75 48518.00 135.40 39291875.6   9 5e-04 0.0100  587084.4 16 10.5
17 17 1884 8394 5.6519410   1 26330.95 27901.60  91.45 29344639.9   9 3e-03 0.0100  302157.5 17 10.5
18 18 1992 8884 8.9534892   1 39799.55 42073.25 127.90 45992723.9   9 2e-03 0.0100  447073.3 18 10.5
19 19 2100 9374 6.2704420   1 26452.50 28211.85  88.45 32182672.4   9 3e-03 0.0100  302180.2 19 10.5
20 20 2208 9864 7.9197884   1 31675.15 33459.20 101.95 41408154.9   9 2e-03 0.0100  352688.5 20 10.5
         \endverbatim
         </li>
        </ul>
       </li>
      </ul>
     </li>
    </ul>
   </li>


  \todo Glucose-2.0
  <ul>
   <li> Solving the key discovery problem over rounds 1 to 20. </li>
   <li> The environment for all of these experiments is:
   \verbatim
processor	: 0
vendor_id	: GenuineIntel
cpu family	: 6
model		: 23
model name	: Intel(R) Core(TM)2 Duo CPU     E8400  @ 3.00GHz
cpu MHz		: 2003.000
cache size	: 6144 KB
bogomips	: 5999.94

processor	: 1
vendor_id	: GenuineIntel
cpu family	: 6
model		: 23
model name	: Intel(R) Core(TM)2 Duo CPU     E8400  @ 3.00GHz
cpu MHz		: 2003.000
cache size	: 6144 KB
bogomips	: 5999.64
address sizes	: 36 bits physical, 48 bits virtual
             total       used       free     shared    buffers     cached
Mem:          3947       3363        583          0        165       3077
   \endverbatim
   </li>
   <li> Solving for 20 random plaintext/key/ciphertext combinations:
    <ul>
     <li> The canonical box translation:
      <ul>
       <li> The data:
       \verbatim
> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_ts_box_aes_mc_bidirectional
> for r in $(seq 1 20); do for s in $(seq 1 20); do glucose-2.0 r${r}_k${s}.cnf > r${r}_k${s}.result ; done; done
> OKP=~/OKplatform; (echo "n c t sat cfs dec rts r1 mem ptime cfl rdb ldlc l2c  l1c r s"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat r${r}_k${s}.result | awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractGlucose.awk | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > Glucose2Statistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
    r     n      c          t sat      cfs       dec   rts           r1 mem  ptime        cfl  rdb   ldlc   l2c  l1c  r    s
1   1   652   9188  0.0139474   1   310.85   2116.10  1.55     24855.45   0 0.0000    36452.9 0.00  44.55 39.50 1.70  1 10.5
2   2  1256  18312  1.3407462   1 11240.75  37446.30 12.75   2123750.75   0 0.0000  1947485.8 1.35  68.85 21.80 0.60  2 10.5
3   3  1860  27436  6.6042957   1 34502.85  52514.80 18.05  12452627.80   0 0.0055  9207593.1 3.50  97.15 42.85 0.10  3 10.5
4   4  2464  36560 10.0692157   1 34729.35  54690.30 21.40  22185391.20   0 0.0100  7464247.8 2.95  31.00  2.20 0.20  4 10.5
5   5  3068  45684 10.4297221   1 44387.75  49275.90  3.45  35345103.60   0 0.0100  5812851.8 4.20  77.30  9.85 0.40  5 10.5
6   6  3672  54808 11.4736494   1 38501.30  62827.70 24.85  26258886.65   0 0.0100  6463354.5 3.60  67.35  2.30 0.20  6 10.5
7   7  4276  63932 11.1830972   1 39092.00  44333.15  6.70  43492687.35   0 0.0110  2667856.5 3.70  94.80  4.60 0.20  7 10.5
8   8  4880  73056 21.4256820   1 63791.05  92538.65 28.85  58501038.90   0 0.0145  7461331.1 5.30 104.10  6.70 0.65  8 10.5
9   9  5484  82180 24.7375308   1 49868.35  71938.65 20.95  46970686.80   0 0.0170  9412945.8 4.50  45.45  3.80 0.30  9 10.5
10 10  6088  91304 23.6191035   1 47924.00  55583.40  7.40  82065733.15   0 0.0190  4412613.4 4.30  87.50 12.85 0.45 10 10.5
11 11  6692 100428 28.8994640   1 61534.85  88998.85 23.75  78036242.25   0 0.0205  8963359.2 4.70 100.45 15.50 0.95 11 10.5
12 12  7296 109552 19.2658640   1 46934.90  52543.10  3.70  79092617.70   0 0.0205  2640797.4 4.05 146.30  4.60 0.25 12 10.5
13 13  7900 118676 26.5718585   1 49900.45  61982.85 12.30  89782863.35   0 0.0230  4093389.1 4.25 109.55  7.85 0.50 13 10.5
14 14  8504 127800 25.5284685   1 57892.70  81248.80 20.60  83032867.50   0 0.0270  5608846.3 4.55 144.15  2.25 0.35 14 10.5
15 15  9108 136924 49.8341655   1 98776.60 129533.45 27.85 144715173.80   0 0.0285  8026280.7 6.55 239.05  9.65 0.80 15 10.5
16 16  9712 146048 43.9806265   1 76292.20 118115.15 43.55 102266600.10   0 0.0290 10362057.7 5.55 154.25  8.45 0.80 16 10.5
17 17 10316 155172 44.6868610   1 67787.60  79933.85 15.65 128733619.35   0 0.0320  6354463.1 5.20 171.25  6.95 0.40 17 10.5
18 18 10920 164296 37.6731245   1 59608.60  68248.95  7.70 154071861.95   0 0.0350  3666602.9 4.70 198.75  7.15 0.45 18 10.5
19 19 11524 173420 30.3478810   1 43564.40  49079.25  4.50 125614266.80   0 0.0375  1010848.2 4.20 143.00  3.95 0.25 19 10.5
20 20 12128 182544 50.5544300   1 81242.40  96430.55  8.35 182232342.10   0 0.0395  3352434.8 6.10 243.55 10.10 0.35 20 10.5
       \endverbatim
       </li>
      </ul>
     </li>
     <li> The 1-base box translation:
      <ul>
       <li> The data:
       \verbatim
> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_1base_box_aes_mc_bidirectional
> for r in $(seq 1 20); do for s in $(seq 1 20); do glucose-2.0 r${r}_k${s}.cnf > r${r}_k${s}.result ; done; done
> OKP=~/OKplatform; (echo "n c t sat cfs dec rts r1 mem ptime cfl rdb ldlc l2c  l1c r s"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat r${r}_k${s}.result | awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractGlucose.awk | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > Glucose2Statistics

> oklib --R
R> E = read.table("Glucose2Statistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
    r    n     c          t sat       cfs       dec  rts         r1 mem  ptime        cfl  rdb   ldlc   l2c  l1c  r    s
1   1  188   754  0.0023991   1    158.45    174.20 1.00     8314.8   0 0.0000    1172.25 0.00   8.75  2.45 0.30  1 10.5
2   2  328  1444  0.1356789   1   5364.55   5639.75 1.00   447819.6   0 0.0000   72656.90 0.65  40.40  3.80 0.20  2 10.5
3   3  468  2134  1.1305774   1  18856.25  19245.65 1.00  3107695.5   0 0.0000  368702.50 2.20  26.90  1.30 0.05  3 10.5
4   4  608  2824  1.9259060   1  28908.70  30461.45 1.00  6870000.5   0 0.0000  560241.05 2.80  56.40  7.15 0.50  4 10.5
5   5  748  3514  2.5206667   1  34317.65  36629.10 1.05  8980614.0   0 0.0000  615788.30 3.25  72.60  4.30 0.25  5 10.5
6   6  888  4204  3.6471465   1  43879.25  47312.85 1.10 12928042.7   0 0.0000  717109.10 4.10 117.65  5.20 0.35  6 10.5
7   7 1028  4894  4.3768877   1  47385.15  51466.30 1.15 15434153.3   0 0.0000  700406.00 4.05 144.60  6.20 0.40  7 10.5
8   8 1168  5584  4.9208539   1  50071.30  55047.55 1.35 17944163.1   0 0.0000  699939.80 4.25 168.65  3.75 0.20  8 10.5
9   9 1308  6274  6.6726852   1  60482.45  66508.20 1.35 23931293.2   0 0.0000  809305.65 5.00 209.90  5.75 0.35  9 10.5
10 10 1448  6964  7.8231604   1  66376.15  73491.25 1.45 28062319.1   0 0.0000  849187.80 5.20 240.25  7.75 0.55 10 10.5
11 11 1588  7654  8.6997259   1  69435.35  77140.90 1.50 30400295.8   0 0.0000  878640.95 5.30 277.95  7.35 0.50 11 10.5
12 12 1728  8344  6.9042009   1  55049.50  61350.65 1.55 26350364.4   0 0.0000  741683.50 4.65 261.50  5.05 0.30 12 10.5
13 13 1868  9034  8.9104884   1  71596.30  80276.25 1.90 30777015.1   0 0.0000  968654.70 5.70 344.00  3.80 0.25 13 10.5
14 14 2008  9724  6.1897607   1  49240.25  55164.20 1.40 22962891.2   0 0.0000  676426.60 4.35 282.05  1.80 0.10 14 10.5
15 15 2148 10414 11.2401894   1  59980.10  64589.30 2.35 50487136.2   0 0.0000  681907.40 5.05 184.20 11.45 0.75 15 10.5
16 16 2288 11104 17.3221198   1 126868.45 140357.30 3.10 53546038.5   0 0.0000 1858295.90 7.90 459.70  4.60 0.25 16 10.5
17 17 2428 11794 12.5141990   1  85842.35  96432.45 2.15 46489923.9   0 0.0015 1187395.30 6.30 383.40  3.00 0.10 17 10.5
18 18 2568 12484 16.7306104   1 102572.75 115508.20 3.05 60030956.6   0 0.0005 1350944.55 6.80 488.95  5.60 0.35 18 10.5
19 19 2708 13174 10.1642541   1  69992.20  78808.80 2.35 37780852.0   0 0.0010  975998.25 5.15 421.65  2.10 0.10 19 10.5
20 20 2848 13864 18.9224676   1 103656.35 116577.75 3.80 69567746.2   0 0.0010 1312615.50 6.90 540.80  6.70 0.45 20 10.5
       \endverbatim
       </li>
      </ul>
     </li>
     <li> The minimum box translation:
      <ul>
       <li> The data:
       \verbatim
> cd ssaes_r1-20_c2_rw2_e4_f0_k1-20_aes_min_box_aes_mc_bidirectional
> for r in $(seq 1 20); do for s in $(seq 1 20); do glucose-2.0 r${r}_k${s}.cnf > r${r}_k${s}.result ; done; done
> OKP=~/OKplatform; (echo "n c t sat cfs dec rts r1 mem ptime cfl rdb ldlc l2c  l1c r s"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat r${r}_k${s}.result | awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractGlucose.awk | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > Glucose2Statistics

> oklib --R
R> E = read.table("Glucose2Statistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
    r    n     c          t sat       cfs       dec   rts         r1 mem ptime        cfl  rdb    ldlc   l2c  l1c  r    s
1   1  188   652  0.0017492   1    219.45    244.85  1.00    10834.7   0     0    1721.45 0.00   10.85  2.50 0.25  1 10.5
2   2  328  1240  0.2035186   1   7894.30   8265.10  1.00   623329.2   0     0  108289.05 1.10   69.90  7.20 0.35  2 10.5
3   3  468  1828  1.0033977   1  19473.05  20152.85  1.00  3147306.1   0     0  369349.05 2.35   65.25  1.45 0.05  3 10.5
4   4  608  2416  2.5186671   1  38863.10  41634.70  1.00  7615251.0   0     0  713214.70 3.55  147.80  8.15 0.45  4 10.5
5   5  748  3004  2.9596481   1  40867.85  44630.70  1.05  9298919.9   0     0  735420.35 3.50  213.05  5.35 0.25  5 10.5
6   6  888  3592  4.1208206   1  50889.45  56735.15  1.25 13829305.2   0     0  892581.60 4.50  310.40  5.40 0.20  6 10.5
7   7 1028  4180  5.0766309   1  55254.50  62125.70  1.15 16884388.2   0     0  886040.80 4.65  364.95  8.45 0.45  7 10.5
8   8 1168  4768  6.6423856   1  64243.85  73856.95  2.40 22738701.9   0     0  975117.10 5.10  473.05 11.05 0.50  8 10.5
9   9 1308  5356  7.8435526   1  70476.20  82492.50  3.10 27006555.8   0     0 1042564.45 5.40  555.70 13.20 0.65  9 10.5
10 10 1448  5944 10.6195888   1  83668.90  97869.40  4.70 34128023.3   0     0 1215886.10 5.80  672.00 18.85 0.95 10 10.5
11 11 1588  6532 17.5732420   1 120881.20 137297.05  5.05 42265172.3   0     0 1985173.90 7.05  714.95 12.70 0.45 11 10.5
12 12 1728  7120 23.7405380   1 159639.65 177489.40  5.90 56023086.5   0     0 2709333.80 8.25  802.95 17.05 0.65 12 10.5
13 13 1868  7708 15.0382185   1 114524.85 133339.25  6.10 43840390.6   0     0 1779541.45 7.50  928.65 14.00 0.35 13 10.5
14 14 2008  8296 15.3129275   1 108931.05 130447.35  7.80 47418759.0   0     0 1640711.30 7.05 1048.85 15.60 0.35 14 10.5
15 15 2148  8884 12.1362045   1  81499.05 104572.80  9.95 42906507.9   0     0 1128061.80 6.05 1121.15 12.25 0.15 15 10.5
16 16 2288  9472 15.4009592   1 100804.55 126357.70 10.70 49206396.0   0     0 1579796.35 6.75 1173.65 17.25 0.35 16 10.5
17 17 2428 10060 19.8477775   1 126749.80 154914.30 14.50 58720960.1   0     0 1929955.85 7.60 1278.90 18.65 0.25 17 10.5
18 18 2568 10648 25.5661142   1 151860.10 185475.45 18.85 73099132.7   0     0 2525227.25 8.75 1544.60 25.50 0.50 18 10.5
19 19 2708 11236 27.5703590   1 164314.60 202775.80 16.40 80580821.0   0     0 2693545.10 9.35 1627.65 20.40 0.20 19 10.5
20 20 2848 11824 34.6494585   1 194965.35 233980.50 19.35 92701612.3   0     0 3033703.25 9.90 1776.70 25.90 0.65 20 10.5
       \endverbatim
       </li>
      </ul>
     </li>
    </ul>
   </li>

*/
