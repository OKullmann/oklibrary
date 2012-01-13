// Matthew Gwynne, 18.7.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/008/1_2_4/general.hpp
  \brief Investigations into small-scale AES key discovery with 1 row, 2 column and 4-bit field elements


  \todo Problem specification
  <ul>
  <li> We consider the small-scale AES with 1 row, 2 column, using the 4-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,1,2,4) for r in 1,...,20. </li>
   <li> We investigate translations of the key discovery problem for
   aes(r,1,2,4) into SAT. </li>
   <li> aes(r,1,2,4) takes a 8-bit plaintext and 8-bit key and outputs a
   8-bit ciphertext. </li>
   <li> aes(r,1,2,4) applies the following operations:
    <ol>
     <li> Key schedule which takes the key and generates r+1 8-bit round
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
   </li>
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
shell> mkdir ssaes_r1-20_c2_rw1_e4_f0_k1-20_aes_canon_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c2_rw1_e4_f0_k1-20_aes_canon_box_aes_mc_bidirectional
shell> oklib --maxima
oklib_monitor : true$
num_rows : 1$
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
  for s in $(seq 1 20) do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c2_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
  done
done
     \endverbatim
     </li>
     <li> The "minimum" box translation:
      <ul>
       <li> Please see "Computing a minimum CNF represention" in
       AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp. </li>
       <li> Generating the instances:
       \verbatim
shell> mkdir ssaes_r1-20_c2_rw1_e4_f0_k1-20_aes_min_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c2_rw1_e4_f0_k1-20_aes_min_box_aes_mc_bidirectional
shell> oklib --maxima
oklib_monitor : true$
num_rows : 1$
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
  for s in $(seq 1 20) do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw1_e4_f0.cnf ssaes_pcpair_r${r}_c2_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
  done
done
       \endverbatim
       </li>
      </ul>
     </li>
     <li> The 1-base box translation:
      <ul>
       <li> Generating the instances:
       \verbatim
shell> mkdir ssaes_r1-20_c2_rw1_e4_f0_k1-20_aes_1base_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c2_rw1_e4_f0_k1-20_aes_1base_box_aes_mc_bidirectional
shell> oklib --maxima
oklib_monitor : true$
num_rows : 1$
num_columns : 2$
exp : 8$
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
  for s in $(seq 1 20) do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c2_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
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
 r   rn    rc          t    cfs    dec  rts        r1 mem  ptime  stime     cfl
 1  112   563 0.00149905   8.50  17.90 1.00    347.00   8 0.0000 0.0000   59.70
 2  200  1094 0.00349900  61.20  96.00 1.35   2817.45   8 0.0000 0.0000  694.95
 3  288  1625 0.00704855 179.65 228.75 2.25  13386.45   8 0.0000 0.0000 2626.00
 4  376  2156 0.00904810 177.30 227.80 2.25  21553.65   8 0.0000 0.0000 2593.95
 5  464  2687 0.00989810 156.60 209.40 2.05  25016.20   8 0.0000 0.0005 2255.00
 6  552  3218 0.01339745 189.70 246.85 2.35  38732.75   8 0.0000 0.0010 2400.50
 7  640  3749 0.01289750 183.30 245.20 2.20  39609.30   8 0.0000 0.0015 2456.35
 8  728  4280 0.01659695 212.70 271.50 2.50  57342.90   8 0.0000 0.0035 2603.00
 9  816  4811 0.01939655 202.40 262.15 2.35  67278.40   8 0.0000 0.0065 2677.95
10  904  5342 0.02234615 251.30 317.60 2.70  85509.15   8 0.0000 0.0070 2783.55
11  992  5873 0.02244620 242.95 310.40 2.65  87952.70   8 0.0000 0.0080 2885.30
12 1080  6404 0.02374590 232.60 291.10 2.70  87814.20   8 0.0020 0.0100 2614.55
13 1168  6935 0.02654545 218.90 296.15 2.50  97065.20   8 0.0015 0.0100 2496.70
14 1256  7466 0.02579565 197.65 271.80 2.25  91589.40   9 0.0010 0.0100 2423.00
15 1344  7997 0.03294440 254.95 339.60 2.75 137758.20   9 0.0040 0.0100 3023.35
16 1432  8528 0.03259450 226.00 301.95 2.50 126384.00   9 0.0050 0.0100 2597.30
17 1520  9059 0.03959355 286.35 365.45 2.90 179209.25   9 0.0040 0.0100 3898.20
18 1608  9590 0.03079480 199.95 267.95 2.50 105628.60   9 0.0065 0.0100 2530.95
19 1696 10121 0.03999335 242.95 318.75 2.65 162930.60   9 0.0060 0.0100 2917.75
20 1784 10652 0.03394440 206.35 273.70 2.45 115627.90   9 0.0060 0.0100 2608.30
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r     t.mean         t.sd    t.min    t.max cfs.mean    cfs.sd cfs.min cfs.max   r1.mean       r1.sd r1.min r1.max
 1 0.00149905 0.0007607822 0.000000 0.002999     8.50   7.85728       0      24    347.00    227.3291     93    798
 2 0.00349900 0.0015043796 0.000999 0.005999    61.20  51.85871       4     162   2817.45   2314.3034    260   7441
 3 0.00704855 0.0028735720 0.003999 0.013997   179.65 119.16741       1     404  13386.45   9263.7271    297  31762
 4 0.00904810 0.0028552427 0.004999 0.016997   177.30 106.15783      69     483  21553.65  13896.0736   6637  53960
 5 0.00989810 0.0037538360 0.003999 0.018997   156.60  92.39754      18     399  25016.20  16892.6213   2913  72365
 6 0.01339745 0.0040308469 0.006998 0.019996   189.70 107.41611       0     400  38732.75  26951.0298    478  97893
 7 0.01289750 0.0043145775 0.007998 0.022996   183.30 113.57543      30     464  39609.30  27830.7860   5468 105928
 8 0.01659695 0.0045461448 0.006998 0.024996   212.70  89.36625      32     411  57342.90  26748.3505   9796  99922
 9 0.01939655 0.0066036801 0.010998 0.031995   202.40 121.05562      29     409  67278.40  45566.6483   6653 147218
10 0.02234615 0.0070496059 0.010998 0.033994   251.30 101.53615     102     420  85509.15  48296.5004  13199 149115
11 0.02244620 0.0066359484 0.011998 0.037994   242.95  81.53041     102     440  87952.70  44966.3843  16652 195460
12 0.02374590 0.0071061983 0.013997 0.037994   232.60 102.65804     105     504  87814.20  48361.0089  25522 179884
13 0.02654545 0.0098578906 0.014997 0.047992   218.90 118.44603      65     482  97065.20  62086.5724  23322 240311
14 0.02579565 0.0071925225 0.014997 0.038994   197.65  78.55958      76     344  91589.40  49580.8027  22271 191884
15 0.03294440 0.0101939219 0.014997 0.048992   254.95 120.81934      51     470 137758.20  75864.2229  19459 261288
16 0.03259450 0.0101728329 0.019996 0.053991   226.00  98.94071     117     442 126384.00  71600.2392  33194 282497
17 0.03959355 0.0130300844 0.020996 0.058991   286.35 161.50095      76     573 179209.25  94567.9555  51430 335669
18 0.03079480 0.0109749534 0.016997 0.061990   199.95 131.48443      75     698 105628.60  76840.8989  12331 318816
19 0.03999335 0.0147946756 0.020996 0.063990   242.95 121.02130      75     448 162930.60 112201.4320  27510 335610
20 0.03394440 0.0097128370 0.021996 0.051992   206.35  73.05965     114     344 115627.90  65041.8648  27415 243180
     \endverbatim
     </li>
    </ul>
   </li>
   <li> The "minimum" box translation (up to round 17):
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
R> options(scipen=10) # suppress use of scientific notation (5E10 etc)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r  rn   rc          t     cfs     dec   rts        r1      cfl
 1  64  194 0.00104930    8.65   12.95  1.00    156.95    29.85
 2 104  356 0.00139915   56.65   66.65  1.10   1141.05   332.90
 3 144  518 0.00299900  138.50  153.85  1.80   5156.70   978.70
 4 184  680 0.00409895  222.90  253.10  2.40   9458.30  1548.40
 5 224  842 0.00489880  227.40  272.55  2.45  11261.75  1559.30
 6 264 1004 0.00559870  277.15  340.35  2.75  13557.70  2034.40
 7 304 1166 0.00924815  429.70  527.05  4.00  24319.45  3144.45
 8 344 1328 0.00974795  423.10  567.45  3.80  24091.15  3093.65
 9 384 1490 0.01074790  477.70  654.50  4.40  28355.55  3693.90
10 424 1652 0.01229755  546.25  787.30  4.85  32956.50  4381.60
11 464 1814 0.01209765  653.15  967.15  5.45  35233.05  5615.65
12 504 1976 0.01374740  674.20  995.90  5.30  39715.10  5880.40
13 544 2138 0.02064640 1019.35 1503.65  7.15  65410.90  8897.05
14 584 2300 0.01969650 1027.90 1532.10  7.40  61762.90  9338.40
15 624 2462 0.02239620 1162.75 1771.90  7.90  73411.15 10700.05
16 664 2624 0.02504555 1263.60 1967.95  8.70  85470.50 11988.80
17 704 2786 0.02789525 1374.45 2208.05  9.50  92212.65 13029.00
18 744 2948 0.02729535 1398.40 2208.35  9.60  91843.00 13660.85
19 784 3110 0.03789365 1936.00 3001.15 12.15 136687.80 18993.60
20 824 3272 0.03674385 1843.55 2966.80 11.55 129103.80 18930.10
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r     t.mean         t.sd    t.min    t.max cfs.mean     cfs.sd cfs.min cfs.max   r1.mean       r1.sd r1.min r1.max
 1 0.00104930 0.0008251764 0.000000 0.001999     8.65   5.264329       0      22    156.95    75.94006     48    336
 2 0.00139915 0.0008205134 0.000000 0.002999    56.65  38.765523       5     135   1141.05   718.35825    175   2562
 3 0.00299900 0.0010760552 0.001999 0.005999   138.50  91.057241      25     332   5156.70  3499.50777    955  12693
 4 0.00409895 0.0022451844 0.000000 0.007998   222.90 135.619398      13     425   9458.30  6423.73834    433  20032
 5 0.00489880 0.0025931814 0.000999 0.010998   227.40 176.036898       3     551  11261.75  9861.45053    181  28893
 6 0.00559870 0.0028168404 0.001999 0.010998   277.15 142.871040      40     523  13557.70  8718.71801   1560  27087
 7 0.00924815 0.0037952360 0.002999 0.017997   429.70 206.522867      22     769  24319.45 14203.39703    847  51161
 8 0.00974795 0.0043264517 0.003999 0.017997   423.10 156.536829     155     707  24091.15 13071.56078   5543  50145
 9 0.01074790 0.0046432351 0.003999 0.018997   477.70 196.012379     112     845  28355.55 16653.06906   3686  57379
10 0.01229755 0.0060610639 0.002999 0.024996   546.25 236.704318      15     936  32956.50 21201.90831    675  70857
11 0.01209765 0.0054076915 0.004999 0.020996   653.15 248.330907     161     993  35233.05 18123.69765   5340  64403
12 0.01374740 0.0076756554 0.002999 0.034994   674.20 401.006313     134    1693  39715.10 31088.17175   5399 127748
13 0.02064640 0.0076165112 0.004999 0.030995  1019.35 348.403619     255    1651  65410.90 29242.58524  11594 106730
14 0.01969650 0.0086686065 0.005999 0.034994  1027.90 512.020343      22    1849  61762.90 37506.08819    909 133132
15 0.02239620 0.0107511978 0.002999 0.045993  1162.75 595.316711      30    2115  73411.15 47951.69353   1408 196613
16 0.02504555 0.0098150233 0.007998 0.040993  1263.60 466.843871     450    2164  85470.50 43321.31886  17652 151973
17 0.02789525 0.0141693982 0.006998 0.049992  1374.45 624.153783     288    2314  92212.65 56909.15408  12220 184489
18 0.02729535 0.0130811956 0.001999 0.047992  1398.40 692.936573      68    2634  91843.00 54833.82733   4378 180854
19 0.03789365 0.0147622290 0.014997 0.060990  1936.00 669.443681     742    3352 136687.80 64890.63692  36245 227890
20 0.03674385 0.0166672364 0.003999 0.061990  1843.55 917.281308      13    3445 129103.80 69573.95859    853 230807
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
R> options(scipen=10) # suppress use of scientific notation (5E10 etc)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r  rn   rc          t    cfs    dec  rts       r1     cfl
 1  64  209 0.00099930   6.80  11.00 1.00   142.10   21.35
 2 104  386 0.00149915  42.10  49.20 1.10   950.80  222.35
 3 144  563 0.00264900 117.45 129.35 1.65  4688.15  803.50
 4 184  740 0.00359890 143.45 163.10 2.00  7360.85 1025.55
 5 224  917 0.00464875 176.65 203.50 2.05 11289.35 1275.65
 6 264 1094 0.00624855 206.10 238.20 2.35 15203.05 1536.40
 7 304 1271 0.00724850 213.65 253.50 2.45 19504.30 1554.20
 8 344 1448 0.00804825 235.45 280.95 2.50 24468.05 1709.85
 9 384 1625 0.00894805 207.70 245.40 2.25 25765.75 1556.10
10 424 1802 0.00924805 211.35 252.85 2.35 28207.20 1555.10
11 464 1979 0.00914820 180.95 222.60 2.20 27685.40 1378.35
12 504 2156 0.01144780 257.70 307.10 2.70 36377.35 1912.50
13 544 2333 0.01424740 286.80 358.40 2.95 48465.30 2086.20
14 584 2510 0.01059790 190.20 233.95 2.35 37179.25 1453.50
15 624 2687 0.01439745 256.80 309.70 2.65 52477.75 1906.90
16 664 2864 0.01459740 218.75 268.30 2.50 49879.35 1637.05
17 704 3041 0.01364750 199.30 240.80 2.20 46370.65 1515.35
18 744 3218 0.01404735 165.00 206.40 2.20 40861.20 1292.55
19 784 3395 0.01529715 198.10 236.95 2.25 52447.65 1454.55
20 824 3572 0.01349760 160.55 198.20 2.00 40813.90 1188.80
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r")
 r     t.mean         t.sd    t.min    t.max cfs.mean     cfs.sd cfs.min cfs.max  r1.mean       r1.sd r1.min r1.max
 1 0.00099930 0.0007943221 0.000000 0.001999     6.80   5.187028       0      18   142.10    75.36151     48    324
 2 0.00149915 0.0008268839 0.000000 0.002999    42.10  33.532231       2     106   950.80   678.02644    126   2247
 3 0.00264900 0.0011367081 0.000999 0.004999   117.45  82.556185      21     280  4688.15  3293.69316    811  10850
 4 0.00359890 0.0016024061 0.000999 0.006998   143.45  91.943046       7     289  7360.85  5045.11907    424  16335
 5 0.00464875 0.0023228896 0.000999 0.007998   176.65 132.198127       0     432 11289.35  8924.22538    176  29434
 6 0.00624855 0.0025722047 0.001999 0.010998   206.10 105.070853      32     401 15203.05  9217.64703   1198  32685
 7 0.00724850 0.0030582331 0.001999 0.012998   213.65 112.195917       7     414 19504.30 11315.90377    716  39758
 8 0.00804825 0.0027615928 0.002999 0.013997   235.45 111.521759      24     421 24468.05 12243.24323   3140  48504
 9 0.00894805 0.0038719376 0.003999 0.015997   207.70 130.239900      16     441 25765.75 16938.35685   2342  53639
10 0.00924805 0.0040371140 0.002999 0.016997   211.35 138.600249       1     452 28207.20 19428.42717    506  63894
11 0.00914820 0.0039634170 0.002999 0.016997   180.95 124.825130       6     431 27685.40 19402.88555   1228  67403
12 0.01144780 0.0051645941 0.004999 0.019996   257.70 121.716712     103     536 36377.35 22641.91082   8674  80958
13 0.01424740 0.0048968734 0.004999 0.024996   286.80 114.597970     104     495 48465.30 26093.43278  11705  98724
14 0.01059790 0.0055754085 0.001999 0.021996   190.20 130.668967       1     460 37179.25 28950.23916    626  92886
15 0.01439745 0.0060637179 0.002999 0.024996   256.80 152.186382       1     526 52477.75 32058.21031    682 106293
16 0.01459740 0.0033459985 0.009998 0.021996   218.75  78.752694     108     379 49879.35 20211.91242  19958  95334
17 0.01364750 0.0060625208 0.004999 0.023996   199.30 136.249809      30     428 46370.65 34569.64721   6126 107126
18 0.01404735 0.0063525184 0.003999 0.027995   165.00 121.870077       1     441 40861.20 32466.19078   1040 120299
19 0.01529715 0.0075040278 0.006998 0.028995   198.10 148.080312      18     469 52447.65 40882.21670   4158 130806
20 0.01349760 0.0080218387 0.003999 0.031995   160.55 146.070810       0     488 40813.90 42390.67943    656 141951
     \endverbatim
     </li>
     <li> Total times and conflicts over 20 rounds and keys:
     \verbatim
1_2_4> oklib --R
R> E_canon = read.table(Sys.glob("ssaes_r1-20_c*_aes_canon_box*/MinisatStatistics")[1], header=TRUE)
R> E_1base = read.table(Sys.glob("ssaes_r1-20_c*_aes_1base_box*/MinisatStatistics")[1], header=TRUE)
R> E_min = read.table(Sys.glob("ssaes_r1-20_c*_aes_min_box*/MinisatStatistics")[1], header=TRUE)
R> options(width=1000)
R> sum(E_canon$t); sum(E_1base$t); sum(E_min$t)
[1] 8.480512
[1] 3.681245
[1] 6.110865
R> sum(E_canon$cfs); sum(E_1base$cfs); sum(E_min$cfs)
[1] 78626
[1] 73497
[1] 303227
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


*/
