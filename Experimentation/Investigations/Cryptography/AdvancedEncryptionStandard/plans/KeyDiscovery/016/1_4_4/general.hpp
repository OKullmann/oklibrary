// Matthew Gwynne, 18.11.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/016/1_4_4/general.hpp
  \brief Investigations into small-scale AES key discovery with 1 row, 4 column and 4-bit field elements


  \todo Problem specification
  <ul>
  <li> We consider the small-scale AES with 1 row, 4 column, using the 4-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,1,4,4) for r in 1,...,20. </li>
   <li> We investigate translations of the key discovery problem for
   aes(r,1,4,4) into SAT. </li>
   <li> aes(r,1,4,4) takes a 16-bit plaintext and 16-bit key and outputs a
   16-bit ciphertext. </li>
   <li> aes(r,1,4,4) applies the following operations:
    <ol>
     <li> Key schedule which takes the key and generates r+1 16-bit round
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
     <li> K_(i,j) is the j-th 4-bit word of the i-th round-key. </li>
    </ul>
   </li>d
   <li> The S-box is a permutation from {0,1}^4 to {0,1}^4 which we consider
   as either:
    <ul>
     <li> a 8x1 boolean function; see ss_sbox_bf in
     ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/AdvancedEncryptionStandard.mac.
     </li>
     <li> 4 4x1 boolean functions. </li>
    </ul>
   </li>
   <li> The decompositions and translations are listed in "Investigating
   dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> The plaintext and ciphertext variables are then set, and the SAT
   SAT solver is run on this instance to deduce the key variables. </li>
  </ul>


  \todo Overview
  <ul>
   <li> Comparison of translations:
    <ul>
     <li> minisat-2.2.0:
      <ul>
       <li> Reading in experimental data:
       \verbatim
> git clone git://github.com/MGwynne/Experimental-data.git
> cd Experimental-data/AES/1_4_4/

> E_canon = read.table("ssaes_r1-20_c4_rw1_e4_f0_k1-20_aes_canon_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)
> E_1base = read.table("ssaes_r1-20_c4_rw1_e4_f0_k1-20_aes_1base_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)
> E_min = read.table("ssaes_r1-20_c4_rw1_e4_f0_k1-20_aes_min_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)

> plot(E_canon)
> plot(E_1base)
> plot(E_min)
       \endverbatim
       </li>
       <li> Overall:
        <ul>
         <li> Ratios of average solver times (fastest to slowest):
         min -> x1 -> 1base -> x2 -> canonical. </li>
         <li> Ratios of average number of conflicts (low to hight):
         canonical -> x1 -> 1base -> x1.1 -> min. </li>
         <li> Calculations:
         \verbatim
> sum(E_1base$t) / sum(E_min$t)
[1] 1.007217
> sum(E_canon$t) / sum(E_1base$t)
[1] 1.923262

> sum(E_1base$cfs) / sum(E_canon$cfs)
[1] 1.089174
> sum(E_min$cfs) / sum(E_1base$cfs)
[1] 1.130550
         \endverbatim
         </li>
        </ul>
       </li>
      </ul>
     </li>
    </ul>
   </li>
  </ul>


  \todo Translations
  <ul>
   <li> The following translations are considered in this %plans %file:
    <ul>
     <li> The canonical box translation. </li>
     <li> The small box translation. </li>
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
shell> mkdir ssaes_r1-20_c4_rw1_e4_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c4_rw1_e4_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 4$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
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
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c4_rw1_e4_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
  done
done
     \endverbatim
     </li>
     <li> The "minimum" box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c4_rw1_e4_f0_k1-1_aes_min_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c4_rw1_e4_f0_k1-1_aes_min_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 4$
exp : 4$
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
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c4_rw1_e4_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
  done
done
     \endverbatim
     </li>
     <li> The 1-base box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c4_rw1_e4_f0_k1-1_aes_1base_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c4_rw1_e4_f0_k1-1_aes_1base_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 4$
exp : 4$
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
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c4_rw1_e4_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
  done
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
   <li> The canonical box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
shell> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
shell> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r           t      cfs      dec    rts          r1   mem        cfl
 1  0.00269900    20.25    33.00   1.00     1225.10  8.00     157.05
 2  0.00879820   213.15   263.80   2.30    22674.75  8.00    2901.10
 3  0.08338685  3180.95  3897.60  16.00   382919.80  8.00   61228.45
 4  0.13582880  4486.30  5480.95  22.20   631139.15  8.00   87747.05
 5  0.23991315  6576.75  8169.85  30.25  1266287.25  8.00  102868.70
 6  2.34264415 60158.90 70782.50 178.25 10365778.00  8.00 1352412.55
 7  2.95514960 47239.55 53746.55 137.35 15777120.70  8.00 1191854.60
 8  4.47786830 53552.80 59239.85 159.20 25678917.30  9.00 1407935.75
 9  5.23820265 51313.20 56447.30 155.95 31244391.55  9.00 1401197.15
10  4.83556005 44758.85 49742.35 137.25 29034607.10  9.00 1184624.55
11  5.82646515 50416.20 56113.00 152.70 35208748.20  9.00 1267457.20
12  7.34758350 59680.90 66619.50 174.80 44174938.75  9.05 1471183.90
13 11.28938880 85316.25 94595.05 230.75 66813278.15  9.15 2020057.55
14  9.98898385 71097.00 79226.40 200.10 59267778.35  9.15 1680962.85
15  8.01888385 53830.50 60205.65 151.85 47962298.00  9.25 1284297.75
16  9.77551600 60182.25 67443.10 178.25 58228026.35  9.40 1414355.20
17  9.25088615 54013.00 60623.80 157.95 55398708.80  9.40 1272681.15
18  8.96903840 50377.15 56617.75 149.05 54518769.85  9.45 1199247.70
19 10.78156355 55924.50 62781.45 165.85 65593440.20  9.70 1300850.50
20 12.57903690 62063.05 69544.95 180.85 76549419.50 10.10 1447875.45
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r      t.mean         t.sd    t.min     t.max cfs.mean      cfs.sd cfs.min cfs.max     r1.mean        r1.sd  r1.min    r1.max
 1  0.00269900  0.001031095 0.000999  0.004999    20.25    17.16445       0      52     1225.10      810.087     163      2836
 2  0.00879820  0.003380498 0.003999  0.015997   213.15   130.00133      23     382    22674.75    13413.904    2730     44111
 3  0.08338685  0.068828314 0.006998  0.218966  3180.95  2759.05935      31    8486   382919.80   330410.505    3953   1017652
 4  0.13582880  0.086684326 0.024996  0.307953  4486.30  2734.97215     849    9789   631139.15   426328.450   93987   1526529
 5  0.23991315  0.140067871 0.049992  0.527919  6576.75  3456.41396    1873   12902  1266287.25   758529.908  240050   2896013
 6  2.34264415  1.689187854 0.258960  5.913100 60158.90 41232.07199    7704  156453 10365778.00  7795347.897 1251259  31988459
 7  2.95514960  2.716797044 0.100984  9.940490 47239.55 58140.33561    1608  241923 15777120.70 12436154.441  607191  44690739
 8  4.47786830  4.718516931 0.154976 17.573300 53552.80 64582.10516    2114  267534 25678917.30 25137157.350  979159 101631500
 9  5.23820265  5.618698864 0.401938 20.725800 51313.20 50457.07606    4993  187778 31244391.55 32339707.130 2618852 119520639
10  4.83556005  5.572169502 0.098984 22.199600 44758.85 47230.60152    1205  189744 29034607.10 32489376.979  583364 128855536
11  5.82646515  6.210474708 0.021996 19.708000 50416.20 49963.59465     181  161904 35208748.20 36989061.236   61668 117406258
12  7.34758350  6.137709566 1.610760 21.675700 59680.90 46244.15140   15330  165828 44174938.75 36011157.682 9989379 126785597
13 11.28938880  9.726607064 0.354946 32.542100 85316.25 69068.88826    3532  231228 66813278.15 56032358.241 2378250 185793478
14  9.98898385  9.978842698 0.096985 42.252600 71097.00 64604.83893     757  270961 59267778.35 57846933.657  596543 245078288
15  8.01888385  8.764114559 0.187971 36.073500 53830.50 55369.70338    1509  225475 47962298.00 50788278.527 1287870 207711389
16  9.77551600 10.190500618 1.165820 33.917800 60182.25 59230.90658    8843  200995 58228026.35 58897421.546 7839990 194718390
17  9.25088615 11.816809004 0.518921 45.919000 54013.00 63610.70074    3805  246580 55398708.80 68478422.496 3592239 265766313
18  8.96903840  7.789922250 0.081987 31.732200 50377.15 41044.18285     550  163243 54518769.85 45672437.738  424539 183665338
19 10.78156355  9.972888158 0.358945 31.324200 55924.50 48284.81660    2489  150645 65593440.20 59092176.734 2488143 184763478
20 12.57903690 10.291170941 0.147977 33.662900 62063.05 47964.07943     992  157212 76549419.50 61551739.788  938002 199683874
     \endverbatim
     </li>
    </ul>
   </li>
   <li> The "minimum" box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
shell> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
shell> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r          t      cfs       dec      rts          r1       stime        cfl
 1 0.00114920    19.50     26.80   1.0000      451.75 0.000000000      77.75
 2 0.00434890   221.05    250.05   2.5000     9151.75 0.000000000    1587.30
 3 0.03974355  3682.80   4183.95  18.8000   164583.70 0.000000000   37530.10
 4 0.13477900 10274.90  11435.50  40.4500   587228.60 0.000000000  118463.40
 5 0.58436045 38690.40  42954.20 111.4000  2526416.50 0.000000000  486693.45
 6 0.86326805 46240.05  50804.65 136.5000  3990008.45 0.000000000  629693.15
 7 1.44168060 57212.40  63401.50 167.6000  6999462.25 0.000000000  888931.20
 8 2.31509840 71033.10  78584.50 194.2500 11465450.90 0.000000000 1191047.60
 9 1.59635620 40065.25  44953.35 125.6500  8267990.85 0.000000000  721189.80
10 3.61305365 78916.90  87595.45 212.2000 18184566.95 0.000000000 1389166.65
11 3.96435015 81058.50  90876.45 217.5500 19331198.40 0.000000000 1449723.70
12 5.33169055 99837.30 111883.65 265.2500 26317726.55 0.000000000 1738312.80
13 4.07323025 70828.80  80361.25 195.5500 19922590.80 0.001500000 1228178.95
14 4.68612532 76664.37  87231.32 210.0526 22659927.89 0.002631579 1339212.26
15 4.47996545 68102.45  78298.20 194.7500 21427350.90 0.000500000 1210555.20
16 5.83256180 82540.60  94613.20 219.9000 27662154.35 0.001000000 1390541.45
17 4.08837800 57920.25  68223.45 169.6500 19109428.40 0.001500000 1034891.85
18 3.84961355 51654.15  61196.65 155.4000 18184869.30 0.004500000  898368.55
19 5.88240125 71645.00  83978.25 203.7500 26767517.05 0.003500000 1275747.55
20 6.37782790 73932.75  87328.80 212.5500 29115832.35 0.006500000 1289980.90
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r     t.mean        t.sd    t.min     t.max cfs.mean       cfs.sd cfs.min cfs.max     r1.mean        r1.sd  r1.min    r1.max
 1 0.00114920 0.000744835 0.000000  0.001999    19.50     17.13261       0      61      451.75 3.318336e+02      88      1272
 2 0.00434890 0.001725008 0.001999  0.007998   221.05    138.44873       4     486     9151.75 6.443219e+03     221     22098
 3 0.03974355 0.033949594 0.004999  0.125980  3682.80   3469.56393     256   12900   164583.70 1.465824e+05   15430    532787
 4 0.13477900 0.195167838 0.020996  0.911861 10274.90  15949.46313    1332   74108   587228.60 8.263345e+05   69872   3831853
 5 0.58436045 0.985078685 0.020996  3.609450 38690.40  66292.14811     882  240594  2526416.50 4.097445e+06   67248  14811691
 6 0.86326805 1.176690372 0.031995  4.505310 46240.05  61075.14411    1467  238301  3990008.45 5.357463e+06  141580  20035276
 7 1.44168060 1.576098960 0.017997  5.184210 57212.40  60045.87553     801  185706  6999462.25 7.778103e+06   84855  26623902
 8 2.31509840 2.407955539 0.014997  9.540550 71033.10  76507.39848     423  235585 11465450.90 1.167924e+07   44166  48244344
 9 1.59635620 1.333388830 0.018997  5.159210 40065.25  31368.84636     802  124678  8267990.85 6.885009e+06   62673  26577061
10 3.61305365 4.328017879 0.015997 14.048900 78916.90  90378.97722     613  301375 18184566.95 2.146706e+07   41064  66911015
11 3.96435015 5.305796792 0.036994 17.902300 81058.50 102720.12187    1245  359544 19331198.40 2.555522e+07  147101  83786784
12 5.33169055 4.737595481 0.385941 13.899900 99837.30  82209.19886    9892  247647 26317726.55 2.337012e+07 1837621  70551686
13 4.07323025 4.808472434 0.100984 14.160800 70828.80  77351.86759    2720  224506 19922590.80 2.351708e+07  466113  69440267
14 4.68612532 4.827871614 0.131979 18.354200 76664.37  71390.22967    3293  272266 22659927.89 2.358348e+07  618484  89583736
15 4.47996545 5.280953422 0.149977 22.210600 68102.45  70330.75610    3931  297123 21427350.90 2.507961e+07  666961 104531562
16 5.83256180 6.673450088 0.030995 20.885800 82540.60  87151.32605    1077  278397 27662154.35 3.149208e+07  103701  98664695
17 4.08837800 4.143115896 0.042993 15.330700 57920.25  52851.79191    1715  195288 19109428.40 1.900631e+07  166000  69800010
18 3.84961355 3.709475694 0.111982 11.749200 51654.15  44420.26193    3051  148424 18184869.30 1.754795e+07  470535  54515483
19 5.88240125 6.452472732 0.372943 23.901400 71645.00  70394.17958    7223  266835 26767517.05 2.862272e+07 1724071 105068887
20 6.37782790 5.628433633 0.076988 18.100200 73932.75  58036.25524    2519  189144 29115832.35 2.545665e+07  302847  82757766
     \endverbatim
     </li>
    </ul>
   </li>
   <li> The 1-base box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
shell> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
shell> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r          t      cfs       dec    rts         r1  stime        cfl
 1 0.00139910    18.65     24.75   1.00      514.0 0.0000      71.70
 2 0.00404885   220.55    245.35   2.50     9660.7 0.0000    1630.80
 3 0.03694380  3109.35   3464.65  15.95   156640.6 0.0000   30929.20
 4 0.07798765  4877.60   5393.80  23.15   356325.4 0.0000   52648.80
 5 0.11203255  6651.45   7493.50  30.20   512883.8 0.0000   77975.45
 6 0.86101850 42406.95  46296.75 132.75  4006733.3 0.0000  594221.00
 7 1.51491905 56886.65  62066.30 161.80  7229550.4 0.0000  879719.30
 8 3.29519785 77672.20  83970.60 215.50 16870942.8 0.0000 1412451.80
 9 3.65494610 77980.50  85371.80 212.55 18783349.2 0.0000 1422459.80
10 1.67269555 35246.65  39593.40 113.10  8742863.4 0.0000  654674.20
11 5.56155370 99616.85 109357.70 267.45 28330557.8 0.0000 1759154.30
12 2.95900120 49843.05  55439.30 139.75 15124143.2 0.0005  873397.20
13 4.91825265 77774.20  86711.80 213.05 25062512.4 0.0020 1383380.95
14 4.87756140 70985.40  79207.70 193.50 24618276.2 0.0005 1263076.00
15 5.72952425 76353.95  84958.25 210.80 28932663.6 0.0015 1340953.60
16 6.94539830 84956.25  94636.80 229.80 35060665.8 0.0020 1486355.55
17 4.25550300 50906.70  57459.95 151.50 21697350.1 0.0050  907595.15
18 3.53391165 40120.10  45677.00 127.85 18216260.3 0.0050  723840.80
19 5.72678235 59817.60  67350.35 173.15 29313354.2 0.0040 1065242.40
20 3.61225290 36930.35  42068.80 116.15 18821924.6 0.0035  666931.65
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
    r     t.mean         t.sd    t.min     t.max cfs.mean      cfs.sd cfs.min cfs.max    r1.mean        r1.sd  r1.min    r1.max
1   1 0.00139910 0.0006803405 0.000000  0.001999    18.65    17.02096       0      48      514.0 3.808926e+02      88      1294
2   2 0.00404885 0.0023274597 0.000999  0.008998   220.55   155.95461      27     607     9660.7 7.002133e+03     886     25869
3   3 0.03694380 0.0308880900 0.006998  0.118981  3109.35  3100.97294     374   11701   156640.6 1.368283e+05   24514    510584
4   4 0.07798765 0.0598466548 0.005999  0.185971  4877.60  3669.28561     157   11588   356325.4 2.826946e+05    8109    878511
5   5 0.11203255 0.0696597388 0.007998  0.241963  6651.45  4068.50048     284   13423   512883.8 3.296344e+05   24912   1126664
6   6 0.86101850 0.6820339656 0.088986  2.669590 42406.95 36789.66961    2994  140708  4006733.3 2.993825e+06  443582  11842668
7   7 1.51491905 2.0313221073 0.059990  8.165760 56886.65 90674.89965    1941  383137  7229550.4 8.920815e+06  310719  34488548
8   8 3.29519785 3.6427024629 0.153976 14.410800 77672.20 82064.59826    4377  320241 16870942.8 1.838134e+07  796763  72662439
9   9 3.65494610 3.6620719327 0.007998 11.340300 77980.50 74701.27867     122  233230 18783349.2 1.871302e+07   11493  58139003
10 10 1.67269555 1.5926983709 0.051992  6.609000 35246.65 31368.15445    1449  131186  8742863.4 8.221464e+06  268789  33887775
11 11 5.56155370 4.9631564005 0.241963 16.499500 99616.85 84410.55735    5577  280998 28330557.8 2.481054e+07 1273378  82016395
12 12 2.95900120 4.4644971742 0.053991 13.912900 49843.05 71512.37236    1079  224864 15124143.2 2.262530e+07  272739  70527883
13 13 4.91825265 4.8513398398 0.429934 16.603500 77774.20 72373.02132    7883  248876 25062512.4 2.431153e+07 2331542  83637949
14 14 4.87756140 6.0437511920 0.046992 23.913400 70985.40 82440.57526     738  322730 24618276.2 2.970711e+07  228323 117425288
15 15 5.72952425 6.1015386247 0.022996 24.703200 76353.95 76963.26617     344  309757 28932663.6 3.012371e+07  109256 121283706
16 16 6.94539830 6.5607280025 0.036994 22.977500 84956.25 75314.39451     583  260033 35060665.8 3.263197e+07  182306 115161302
17 17 4.25550300 4.3575163752 0.042993 15.445700 50906.70 49381.25038     534  176040 21697350.1 2.175424e+07  209244  76547905
18 18 3.53391165 3.3709905498 0.085986 14.408800 40120.10 34688.46666    1244  148027 18216260.3 1.685999e+07  446985  72075096
19 19 5.72678235 4.5704831055 0.435933 14.692800 59817.60 43962.75673    6037  143262 29313354.2 2.280960e+07 2420467  72550608
20 20 3.61225290 4.4839022658 0.040993 20.024000 36930.35 41418.89717     501  183648 18821924.6 2.254737e+07  193277 100336390
     \endverbatim
     </li>
     <li> Total times and conflicts over 20 rounds and keys:
     \verbatim
1_4_4> oklib --R
R> E_canon = read.table(Sys.glob("ssaes_r1-20_c*_aes_canon_box*/MinisatStatistics")[1], header=TRUE)
R> E_1base = read.table(Sys.glob("ssaes_r1-20_c*_aes_1base_box*/MinisatStatistics")[1], header=TRUE)
R> E_min = read.table(Sys.glob("ssaes_r1-20_c*_aes_min_box*/MinisatStatistics")[1], header=TRUE)
R> options(width=1000)
R> sum(E_canon$t); sum(E_1base$t); sum(E_min$t)
[1] 2282.948
[1] 1187.019
[1] 1178.514
R> sum(E_canon$cfs); sum(E_1base$cfs); sum(E_min$cfs)
[1] 17488030
[1] 19047500
[1] 21534146
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>

*/
