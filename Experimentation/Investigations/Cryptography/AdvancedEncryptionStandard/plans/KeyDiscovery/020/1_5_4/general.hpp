// Matthew Gwynne, 17.11.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/020/1_5_4/general.hpp
  \brief Investigations into small-scale AES key discovery with 1 row, 5 columns and 4-bit field elements


  \todo Problem specification
  <ul>
  <li> We consider the small-scale AES with 1 row, 5 columns, using the 4-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,1,5,4) for r in 1,...,20. </li>
   <li> We investigate translations of the key discovery problem for
   aes(r,1,5,4) into SAT. </li>
   <li> aes(r,1,5,4) takes a 20-bit plaintext and 20-bit key and outputs a
   20-bit ciphertext. </li>
   <li> aes(r,1,5,4) applies the following operations:
    <ol>
     <li> Key schedule which takes the key and generates r+1 20-bit round
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
> cd Experimental-data/AES/1_5_4/

> E_canon = read.table("ssaes_r1-20_c5_rw1_e4_f0_k1-20_aes_canon_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)
> E_1base = read.table("ssaes_r1-20_c5_rw1_e4_f0_k1-20_aes_1base_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)
> E_min = read.table("ssaes_r1-20_c5_rw1_e4_f0_k1-20_aes_min_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)

> plot(E_canon)
> plot(E_1base)
> plot(E_min)
       \endverbatim
       </li>
       <li> Overall:
        <ul>
         <li> Ratios of average solver times (fastest to slowest):
         min -> x1.1 -> 1base -> x2.3 -> canonical. </li>
         <li> Ratios of average number of conflicts (low to high):
         min -> x1 -> 1base -> x1.1 -> canonical. </li>
         <li> Calculations:
         \verbatim
> sum(E_1base$t) / sum(E_min$t)
[1] 1.118604
> sum(E_canon$t) / sum(E_1base$t)
[1] 2.311146

> sum(E_1base$cfs) / sum(E_min$cfs)
[1] 1.035889
> sum(E_canon$cfs) / sum(E_1base$cfs)
[1] 1.063985
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
shell> mkdir ssaes_r1-20_c5_rw1_e4_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c5_rw1_e4_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 5$
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
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c5_rw1_e4_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
  done
done
     \endverbatim
     </li>
     <li> The "minimum" box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c5_rw1_e4_f0_k1-1_aes_min_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c5_rw1_e4_f0_k1-1_aes_min_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 5$
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
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c5_rw1_e4_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
  done
done
     \endverbatim
     </li>
     <li> The 1-base box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c5_rw1_e4_f0_k1-1_aes_1base_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c5_rw1_e4_f0_k1-1_aes_1base_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 5$
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
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c5_rw1_e4_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
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
 r            t        cfs        dec     rts           r1  stime         cfl
 1   0.00289900      18.80      34.65    1.00 1.224150e+03 0.0000      129.30
 2   0.00864825     150.95     202.25    1.95 2.054995e+04 0.0000     2323.00
 3   0.09938440    3205.55    4061.70   16.55 4.692709e+05 0.0025    63898.15
 4   1.97394910   56095.80   66470.75  169.10 8.556475e+06 0.0065  1205026.60
 5   3.58265675   83325.50   99447.50  228.20 1.584073e+07 0.0085  1662965.60
 6   4.48276720   79253.75   92244.20  215.45 2.260616e+07 0.0100  1585754.65
 7  32.95754350  673276.10  780147.80 1372.25 1.296335e+08 0.0100 15616870.40
 8  88.57197750 1363225.75 1547851.75 2548.10 3.033040e+08 0.0100 33078622.35
 9 236.31324750 2414303.30 2701230.55 4162.85 8.848807e+08 0.0100 59058436.75
10 109.43531050  865097.75  962761.35 1690.90 5.363367e+08 0.0100 23500386.20
11 274.08449150 1634774.60 1774671.55 2806.60 1.400587e+09 0.0100 48309179.50
12 349.99709250 1930769.45 2092893.60 3417.20 1.857727e+09 0.0105 57083817.35
13 368.13081900 1845160.60 1999870.65 3218.90 1.961107e+09 0.0140 53564687.30
14 369.74578100 1768702.95 1919838.65 3168.10 1.963695e+09 0.0185 51190010.85
15 524.95462800 2352615.10 2561227.70 4050.25 2.768973e+09 0.0195 66714646.00
16 458.02631000 1976219.90 2145367.45 3464.30 2.418103e+09 0.0200 55211831.15
17 672.52478500 2686551.65 2926971.10 4597.70 3.527381e+09 0.0200 71261989.50
18 365.57899500 1403151.65 1531964.35 2528.55 1.883815e+09 0.0200 38047782.20
19 377.10726400 1366766.50 1495685.00 2482.60 1.950081e+09 0.0200 36869190.45
20 719.15839000 2342798.75 2550079.70 4080.80 3.590161e+09 0.0200 60994987.80
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r       t.mean         t.sd     t.min       t.max   cfs.mean       cfs.sd cfs.min  cfs.max      r1.mean        r1.sd    r1.min      r1.max
 1   0.00289900 1.293709e-03  0.000999    0.004999      18.80 2.360999e+01       0       84 1.224150e+03 1.089476e+03       198        3927
 2   0.00864825 3.616275e-03  0.003999    0.016997     150.95 1.253565e+02       9      442 2.054995e+04 1.754647e+04      1609       65020
 3   0.09938440 8.277337e-02  0.008998    0.284956    3205.55 2.800350e+03      80     9309 4.692709e+05 4.167659e+05     13427     1444607
 4   1.97394910 1.496775e+00  0.104984    5.143220   56095.80 4.086345e+04    2909   142429 8.556475e+06 6.385982e+06    490058    21435727
 5   3.58265675 2.886426e+00  0.219966   10.037500   83325.50 6.302153e+04    5946   234841 1.584073e+07 1.230094e+07   1143085    43608241
 6   4.48276720 6.237955e+00  0.222966   27.984700   79253.75 9.916875e+04    5716   446965 2.260616e+07 2.967857e+07   1141349   131376957
 7  32.95754350 2.301329e+01  2.011690   69.493400  673276.10 4.449626e+05   46826  1318893 1.296335e+08 8.960912e+07   8816866   273159976
 8  88.57197750 1.075224e+02  8.889650  455.307000 1363225.75 1.587446e+06  135626  6737200 3.033040e+08 3.342192e+08  39028463  1418482701
 9 236.31324750 2.586699e+02  4.246350  929.702000 2414303.30 2.418932e+06   38307  7539012 8.848807e+08 9.838747e+08  26266158  3873827416
10 109.43531050 1.169486e+02  1.847720  471.945000  865097.75 1.061478e+06   16099  4391594 5.363367e+08 4.802721e+08  12212167  1687833673
11 274.08449150 3.370047e+02  2.452630  953.102000 1634774.60 1.949354e+06   21063  5532892 1.400587e+09 1.678051e+09  15861059  4966487634
12 349.99709250 3.326717e+02  5.589150  974.785000 1930769.45 1.789485e+06   42397  5486904 1.857727e+09 1.712279e+09  35167582  5002124985
13 368.13081900 5.632656e+02  2.462630 2440.090000 1845160.60 2.644793e+06   18447 11390661 1.961107e+09 2.931591e+09  15852466 12752829538
14 369.74578100 4.024378e+02  3.183520 1216.630000 1768702.95 1.841572e+06   22738  5584888 1.963695e+09 2.086437e+09  19888665  6436460574
15 524.95462800 5.020760e+02  3.556460 1571.380000 2352615.10 2.121471e+06   24803  6581662 2.768973e+09 2.561089e+09  22358567  7975512835
16 458.02631000 4.832726e+02  1.943700 1424.980000 1976219.90 2.010594e+06   13837  6029348 2.418103e+09 2.506961e+09  12390420  7414909692
17 672.52478500 6.478584e+02 24.673200 1889.140000 2686551.65 2.477644e+06  126669  7331165 3.527381e+09 3.340573e+09 145464097  9751285452
18 365.57899500 5.626881e+02 20.710900 1817.420000 1403151.65 2.074508e+06  100681  6793247 1.883815e+09 2.816790e+09 121935860  9163018204
19 377.10726400 3.895595e+02  8.671680 1513.270000 1366766.50 1.337108e+06   42785  5175280 1.950081e+09 1.969615e+09  53495371  7704961316
20 719.15839000 8.707486e+02 43.520400 3511.650000 2342798.75 2.612606e+06  179737 10423224 3.590161e+09 4.095705e+09 248504001 16328426069
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
 r            t        cfs        dec     rts           r1        cfl
 1   0.00064950      29.40      38.10    1.00 6.823000e+02      130.9
 2   0.00364885     293.20     336.90    2.95 1.323045e+04     2151.9
 3   0.05864055    4153.75    4751.20   20.40 2.679504e+05    47422.0
 4   0.58491040   43533.95   48895.55  135.60 2.353932e+06   565447.4
 5   2.10597985  121567.40  137261.75  315.45 8.944725e+06  1779633.1
 6   4.28570155  182510.10  202984.10  429.95 1.920775e+07  2694341.4
 7  22.83242260  911495.45 1019719.70 1732.95 8.930345e+07 15437885.4
 8  47.24301680 1579518.30 1749367.70 2783.35 1.723058e+08 28167658.6
 9  47.17114100 1219612.50 1352682.40 2214.20 1.924985e+08 23524975.4
10  66.50365550 1270099.10 1407596.60 2307.80 2.846011e+08 26421058.9
11  95.99770950 1429623.20 1570855.80 2649.90 4.204346e+08 31639411.2
12 140.93017020 1855470.95 2049501.90 3213.95 5.964000e+08 43010317.0
13  91.62492975 1168599.30 1295718.05 2193.30 4.040006e+08 26954490.0
14 129.69808200 1530116.65 1692222.90 2797.65 5.617864e+08 34638122.8
15 119.73989600 1330540.60 1482670.40 2447.75 4.977079e+08 30230900.5
16 113.84126110 1173967.40 1307629.90 2105.65 4.685661e+08 26873381.6
17 294.51272600 2706359.10 3002934.05 4609.85 1.183275e+09 60625789.1
18 165.74701400 1479386.75 1651876.75 2702.15 6.763738e+08 33125980.0
19 333.60777050 2680192.55 2976509.30 4651.60 1.305196e+09 59507876.6
20 240.81934000 1855242.85 2077078.80 3323.35 9.425185e+08 41977742.2
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r       t.mean         t.sd     t.min      t.max   cfs.mean       cfs.sd cfs.min cfs.max      r1.mean        r1.sd   r1.min     r1.max
 1   0.00064950 7.447008e-04  0.000000   0.001999      29.40 2.246494e+01       0      73 6.823000e+02 4.413464e+02      108       1588
 2   0.00364885 2.084124e-03  0.000999   0.007998     293.20 1.848011e+02      61     605 1.323045e+04 9.253165e+03     1895      29258
 3   0.05864055 5.819530e-02  0.008998   0.174973    4153.75 4.058251e+03     488   11717 2.679504e+05 2.757296e+05    26818     805708
 4   0.58491040 5.846687e-01  0.061990   2.592600   43533.95 4.203975e+04    5120  189036 2.353932e+06 2.228837e+06   263279    9887049
 5   2.10597985 2.160363e+00  0.157975   7.866800  121567.40 1.224788e+05   10674  436430 8.944725e+06 9.177784e+06   733967   33962567
 6   4.28570155 5.399971e+00  0.255961  21.137800  182510.10 2.469623e+05   14193 1054793 1.920775e+07 2.229224e+07  1202414   81667556
 7  22.83242260 3.481444e+01  0.573912 150.805000  911495.45 1.206122e+06   29210 4958950 8.930345e+07 1.356267e+08  2583198  597528441
 8  47.24301680 6.876752e+01  0.154976 241.465000 1579518.30 2.157036e+06    4778 6725211 1.723058e+08 2.387893e+08   833916  850522654
 9  47.17114100 5.199344e+01  2.217660 196.613000 1219612.50 1.399775e+06   51098 5287372 1.924985e+08 1.978187e+08 11619911  726246733
10  66.50365550 7.464648e+01  4.692290 285.945000 1270099.10 1.370871e+06   95641 5145534 2.846011e+08 2.992999e+08 23582113 1130094930
11  95.99770950 1.451039e+02  9.243590 678.550000 1429623.20 2.054001e+06  157604 9705214 4.204346e+08 5.765321e+08 46613958 2698322473
12 140.93017020 2.037908e+02  0.694894 637.387000 1855470.95 2.543997e+06   15094 7625163 5.964000e+08 8.334489e+08  3434446 2672729004
13  91.62492975 1.220264e+02  0.357945 433.267000 1168599.30 1.452505e+06    7967 4974281 4.040006e+08 5.160065e+08  1728020 1853001138
14 129.69808200 1.628861e+02  3.678440 737.292000 1530116.65 1.781297e+06   63422 8092663 5.617864e+08 6.772355e+08 17875819 3069964442
15 119.73989600 1.814923e+02  0.393940 791.959000 1330540.60 1.889462e+06    7970 8263014 4.977079e+08 7.098357e+08  1875628 3108488812
16 113.84126110 1.701823e+02  0.508922 530.972000 1173967.40 1.674122e+06   10440 5153207 4.685661e+08 6.885427e+08  2326888 2131551260
17 294.51272600 2.656847e+02  4.489320 808.606000 2706359.10 2.338053e+06   64813 7169631 1.183275e+09 1.045204e+09 20679373 3198600493
18 165.74701400 1.901186e+02  1.396790 611.698000 1479386.75 1.624268e+06   21986 5065114 6.763738e+08 7.592747e+08  6247169 2382415880
19 333.60777050 2.891081e+02  4.531310 954.897000 2680192.55 2.225748e+06   56721 7322740 1.305196e+09 1.089114e+09 20807749 3601079835
20 240.81934000 2.339962e+02 15.892600 803.547000 1855242.85 1.688265e+06  153142 5689133 9.425185e+08 8.756109e+08 70988778 2873589645
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
 r            t        cfs        dec     rts           r1  stime        cfl
 1   0.00094945      21.70      28.50    1.00 6.439500e+02 0.0000       89.5
 2   0.00289900     206.40     233.60    2.40 1.077620e+04 0.0000     1515.9
 3   0.06788930    4542.50    5077.95   21.85 3.184203e+05 0.0000    51168.3
 4   0.45887975   33229.10   37295.95  111.15 1.817065e+06 0.0000   432232.2
 5   1.57141105   86406.75   98550.50  233.95 6.443640e+06 0.0000  1183376.7
 6   1.87566465   72003.90   80395.10  201.25 8.827235e+06 0.0000  1071942.2
 7  23.36369400  952566.50 1052819.10 1838.85 9.006528e+07 0.0000 15264320.1
 8  25.68792670  893709.95  988609.80 1765.00 9.732717e+07 0.0000 15917394.2
 9  70.39030400 1775780.95 1953736.70 3185.75 2.533126e+08 0.0000 32892469.1
10  88.38798550 1663480.50 1831374.25 3013.15 3.566294e+08 0.0000 33342830.4
11  64.27675150  883323.95  964735.35 1713.90 3.000327e+08 0.0000 20233774.3
12 229.90716150 2723523.45 2949624.70 4594.45 1.026160e+09 0.0000 56979814.6
13 131.42983005 1471812.10 1609492.70 2681.40 5.985530e+08 0.0000 32637788.5
14 124.99619660 1313071.25 1440836.65 2384.35 5.643431e+08 0.0000 29517569.4
15 184.81577600 1813853.65 2002497.65 3214.90 8.212261e+08 0.0030 40287127.0
16 301.45670500 2694773.95 2958816.65 4562.20 1.289486e+09 0.0020 59180906.8
17 232.65393000 1996683.20 2206344.80 3529.90 9.937855e+08 0.0065 44750771.5
18 147.58576150 1242708.95 1384263.45 2384.90 6.464419e+08 0.0085 28547083.3
19 284.26395060 2117648.45 2336897.00 3672.75 1.184837e+09 0.0080 46953314.2
20 231.51573800 1611975.20 1786294.35 2854.65 9.546209e+08 0.0090 36457576.1
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r       t.mean         t.sd    t.min       t.max   cfs.mean       cfs.sd cfs.min  cfs.max      r1.mean        r1.sd   r1.min     r1.max
 1   0.00094945 9.440368e-04 0.000000    0.001999      21.70      20.8985       0       77 6.439500e+02 5.112817e+02      108       1976
 2   0.00289900 2.124841e-03 0.000000    0.007998     206.40     137.7387      41      519 1.077620e+04 8.179557e+03     1451      27212
 3   0.06788930 6.849929e-02 0.001999    0.235964    4542.50    4596.3492     157    16030 3.184203e+05 3.234579e+05     8788    1113643
 4   0.45887975 3.835922e-01 0.039993    1.460780   33229.10   26825.7277    2265   103992 1.817065e+06 1.464811e+06   179893    5623460
 5   1.57141105 1.503854e+00 0.164974    4.926250   86406.75   84502.5270    9063   273020 6.443640e+06 5.769243e+06   785206   19342463
 6   1.87566465 1.743450e+00 0.181972    6.532010   72003.90   61780.9773    8147   228869 8.827235e+06 8.093661e+06   863084   30004442
 7  23.36369400 2.577798e+01 1.751730   73.514800  952566.50 1019147.5190   82360  2986080 9.006528e+07 9.684856e+07  7503347  280983726
 8  25.68792670 2.827448e+01 0.757884  115.879000  893709.95  948982.4692   24740  3732246 9.732717e+07 1.017350e+08  3721910  419493766
 9  70.39030400 1.047048e+02 1.787730  435.527000 1775780.95 2495336.8356   37470 10020636 2.533126e+08 3.442079e+08  9339180 1432668867
10  88.38798550 7.736430e+01 2.032690  235.190000 1663480.50 1447287.7006   38325  4635374 3.566294e+08 2.996092e+08 10743380  953701031
11  64.27675150 5.519307e+01 2.773580  204.387000  883323.95  729682.2530   47054  2676775 3.000327e+08 2.499323e+08 14493961  936813920
12 229.90716150 2.369060e+02 4.354340  654.181000 2723523.45 2751662.5794   70773  7573870 1.026160e+09 1.042966e+09 22327694 2877741318
13 131.42983005 1.462239e+02 0.185971  575.031000 1471812.10 1566272.5042    3539  6191170 5.985530e+08 6.533528e+08   986532 2567967237
14 124.99619660 1.446693e+02 0.510922  524.111000 1313071.25 1448356.8627    9483  5225814 5.643431e+08 6.379451e+08  2721005 2297258612
15 184.81577600 1.943940e+02 5.405180  552.324000 1813853.65 1838185.1770   68280  5364805 8.212261e+08 8.484219e+08 28100972 2467722390
16 301.45670500 3.390898e+02 1.941700 1282.570000 2694773.95 2905593.7956   26273 10866779 1.289486e+09 1.403950e+09 10017469 5224282599
17 232.65393000 2.884662e+02 7.904800 1048.910000 1996683.20 2310525.0337   87920  8427203 9.937855e+08 1.175494e+09 39826109 4236125264
18 147.58576150 1.030741e+02 2.124680  321.908000 1242708.95  839003.2011   25258  2618337 6.464419e+08 4.383583e+08 10969008 1377836789
19 284.26395060 3.043281e+02 0.838872 1038.970000 2117648.45 2182704.8416   10418  7343625 1.184837e+09 1.224222e+09  4491822 4068064912
20 231.51573800 3.444293e+02 1.152820 1408.270000 1611975.20 2248302.2807   12947  9030024 9.546209e+08 1.338376e+09  6280259 5419002651
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>

*/
