// Matthew Gwynne, 21.12.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/008/2_1_4/general.hpp
  \brief Investigations into small-scale AES key discovery for AES with a 2x1 plaintext matrix and 4-bit field elements


  \todo Overview
  <ul>
   <li> We consider aes(r,2,1,4) for r in 1,...,20. </li>
   <li> For full description of this instance, see "Problem specification"
   below. </li>
   <li> As we increase the number of rounds, going from aes(1,2,1,4) to
   aes(20,2,1,4), the problem should get harder, and so we investigate here
   how the performance of each solver changes as we increase the number of
   rounds. </li>
   <li> Descriptions and statistics for each of the translations are provided
   in plans/KeyDiscovery/008/2_1_4/Translations.hpp.
   </li>
  </ul>


  \todo Problem specification
  <ul>
  <li> We consider the small-scale AES with 2 rows, 1 column, using the 4-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,2,1,4) for r in 1,...,20. </li>
   <li> We investigate translations of the key discovery problem for
   aes(r,2,1,4) into SAT. </li>
   <li> aes(r,2,1,4) takes a 16-bit plaintext and 16-bit key and outputs a
   16-bit ciphertext. </li>
   <li> aes(r,2,1,4) applies the following operations:
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
   <li> The key schedule computes the round key i, K_(i,j), from round key
   i-1, K_(i-1), by:
   \verbatim
K_(i,j) := S-box(K_(i-1,j)) + C_i
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
     <li> a 16x1 boolean function; see ss_sbox_bf in
     Cryptology/Lisp/CryptoSystems/Rijndael/AdvancedEncryptionStandard.mac;
     </li>
     <li> 4 4x1 boolean functions. </li>
    </ul>
   </li>
   <li> The MixColumns operation is a 16-bit boolean function representing a
   permutation from {0,1}^8 to {0,1}^8, defined as:
   \verbatim
MixColumns(I_1) := Mul02(I_1) + Mul03(I_2)
MixColumns(I_2) := Mul03(I_1) + Mul02(I_2)
   \endverbatim
   where
    <ul>
     <li> I_i is the i-th 4-bit word in the input; </li>
     <li> Mul02 is a permutation over {0,1}^4 representing multiplication
     by 02 in the Rijndael byte field; </li>
     <li> Mul03 is a permutation over {0,1}^4 representing multiplication
     by 03 in the Rijndael byte field. </li>
     <li> These boolean functions representing all 8-bit multiplication
     boolean functions are studied in
     AdvancedEncryptionStandard/plans/Representations/Mul_e_4.hpp. </li>
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
     <li> The small box translation. </li>
     <li> The 1-base translation. </li>
    </ul>
   </li>
   <li> The Sboxes and multiplications boxes are translated using the
   each translation, and are discussed in
   plans/KeyDiscovery/008/2_1_4/Translations.hpp where statistics are also
   provided. </li>
   <li> For a full list of the possible translations, see
   "Investigating dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> Generating instances for rounds 1-20 for a 20 random keys with each
   of the translations:
    <ul>
     <li> The canonical box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c6_rw1_e4_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c6_rw1_e4_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> oklib --maxima
oklib_monitor : true$
num_rows : 2$
num_columns : 1$
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
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c1_rw2_e4_f0.cnf ssaes_pcpair_r${r}_c1_rw2_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
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
   <li> The canonical box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
shell> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > AllStatistics
shell> oklib --R
R> E = read.table("AllStatistics", header=TRUE)
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
R> aggregate_statistics(E[c("t","cfs","r1")], by=list("r"))
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
  </ul>


  \todo The 1-base box translation
  <ul>
   <li> The Sboxes and multiplications boxes are translated using the
   1-base translation, discussed in "The 1-base box translation"
   in
   plans/KeyDiscovery/016/2_1_8/Translations.hpp
   where statistics are also provided. </li>
   <li> There are currently active investigations attempting to find the
   minimum-size 1-base representations for each of the boxes, discussed in
    <ul>
     <li> AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp.
     </li>
     <li> AdvancedEncryptionStandard/plans/Representations/Mul_2_8.hpp.
     </li>
     <li> AdvancedEncryptionStandard/plans/Representations/Mul_3_8.hpp.
     </li>
    </ul>
   </li>
   <li> Over 20 rounds, with 20 random plaintext-ciphertext pairs per round:
    <ul>
     <li> For instructions on generating the 1-bases, see
     "The 1-base box translation" in
     AdvancedEncryptionStandard/plans/KeyDiscovery/016/2_1_8/Translations.hpp.
     </li>
     <li> Generating the instances (in Maxima):
     \verbatim
oklib_monitor : true$
set_hm(ss_sbox_rbase_cnfs,8,read_fcl_f("AES_Sbox_base.cnf"))$
set_hm(ss_field_rbase_cnfs,[8,2],read_fcl_f("AES_byte_field_mul_2_base.cnf"))$
set_hm(ss_field_rbase_cnfs,[8,3],read_fcl_f("AES_byte_field_mul_3_base.cnf"))$
for rounds : 1 thru 20 do
  output_ss_fcl_std(rounds, num_columns : 1, num_rows : 2, exp : 8, final_round_b : false, box_tran : aes_rbase_box, mc_tran : aes_mc_bidirectional)$
     \endverbatim
     </li>
     <li> Generating 20 random assignments for the plaintext and ciphertext,
     leaving the key unknown:
     \verbatim
maxima> for seed : 1 thru 20 do output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
     \endverbatim
     </li>
     <li> Running minisat-2.2.0:
     \verbatim
shell> col=1; row=2; e=8;
for r in $(seq 1 20); do
  for k in $(seq 1 20); do
      echo "Key ${k} Round ${r}";
      AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${row}_e${e}_f0.cnf ssaes_pcpair_r${r}_c${col}_rw${row}_e${e}_f0_s${k}.cnf > r${r}_k${k}.cnf;
      minisat-2.2.0 r${r}_k${k}.cnf > minisat_r${r}_k${k}.result 2>&1;
  done;
done
shell> ExtractMinisat header-only | awk ' { print $0 " r k" }' > minisat_results;
for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    cat minisat_r${r}_k${k}.result | ExtractMinisat data-only | awk " { print \$0 \"  $r  $k\" }";
  done;
done >> minisat_results;
     \endverbatim
     yields:
     \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
options(width=1000)
EM
 r   rn     rc        t       cfs       dec    rts          r1  ptime  stime       cfl
 1  192  18272   0.5285    378.40    407.10   3.55    17333.15 0.0110 0.5115    2898.3
 2  336  36480   9.6240  38637.05  41081.25 125.30  3096961.95 0.0205 0.9600  635664.8
 3  480  54688  15.2920  37360.50  39724.25 118.35  5821360.20 0.0350 1.4000  569780.4
 4  624  72896  24.8370  45409.70  49792.90 138.35  8149119.90 0.0445 1.8700  817007.7
 5  768  91104  30.2230  40354.70  44488.95 130.10  9966599.80 0.0570 2.3100  644105.6
 6  912 109312  46.9385  56145.40  62684.00 170.25 14110385.50 0.0640 2.7595  888331.9
 7 1056 127520  46.7475  53666.90  61842.45 166.15 13175222.50 0.0785 3.1975  853229.2
 8 1200 145728 102.3470  84242.95  96100.55 239.45 24387576.30 0.0915 3.6640 1316421.4
 9 1344 163936 117.9040  87504.90 101635.25 251.80 27133294.20 0.1015 4.1090 1399346.6
10 1488 182144 143.2785  97662.45 114138.40 273.80 30732365.70 0.1130 4.5550 1627517.4
11 1632 200352 100.5410  82608.05 100458.20 239.20 21793714.35 0.1245 4.9960 1477655.9
12 1776 218560 162.2785 112890.40 135246.05 309.75 33442510.10 0.1345 5.4655 2002968.6
13 1920 236768 219.8780 128707.95 154327.85 350.65 44485694.45 0.1510 5.9085 2238872.5
14 2064 254976 277.6545 149758.45 179119.00 397.55 54358522.10 0.1570 6.3560 2664466.5
15 2208 273184 278.9755 154428.90 186527.30 408.50 54256192.65 0.1710 6.7920 2824605.1
16 2352 291392 244.3250 144208.05 177472.55 386.45 48356482.55 0.1835 7.2620 2758481.2
17 2496 309600 283.3340 163903.60 202336.40 422.85 54694309.85 0.1970 7.7045 3221092.9
18 2640 327808 350.5680 180847.40 222289.35 461.60 66776355.60 0.2075 8.1530 3461162.0
19 2784 346016 350.7520 182180.55 226259.50 453.00 66744064.25 0.2190 8.5940 3528544.8
20 2928 364224 365.6620 194371.05 242715.75 491.40 69723963.15 0.2275 9.0660 3904121.5
     \endverbatim
     </li>
     <li> Consider:
     \verbatim
R> plot(E$cfs)
R> plot(E$t)
     \endverbatim
     The minisat-2.2.0 times and number of conflicts appear to be linear in
     the number of rounds! </li>
    </ul>
   </li>
  </ul>


  \todo The "minimum" box translation
  <ul>
   <li> The Sboxes and multiplications boxes are translated using the
   "minimum" translations, discussed in 'The "minimum" box translation'
   in
   plans/KeyDiscovery/016/2_1_8/Translations.hpp
   where statistics are also provided. </li>
   <li> There are currently active investigations attempting to find the
   minimum representations for each of the boxes, discussed in
    <ul>
     <li> AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp.
     </li>
     <li> AdvancedEncryptionStandard/plans/Representations/Mul_2_8.hpp.
     </li>
     <li> AdvancedEncryptionStandard/plans/Representations/Mul_3_8.hpp.
     </li>
    </ul>
   </li>
   <li> Over 20 rounds, with 20 random plaintext-ciphertext pairs per round:
    <ul>
     <li> For instructions on generating the 1-bases, see
     'The "minimum" box translation' in
     plans/KeyDiscovery/016/2_1_8/Translations.hpp.
     </li>
     <li> Generating the instances (in Maxima):
     \verbatim
oklib_monitor : true$
for rounds : 1 thru 20 do
  output_ss_fcl_std(rounds, num_columns : 1, num_rows : 2, exp : 8, final_round_b : false, box_tran : aes_small_box, mc_tran : aes_mc_bidirectional)$
     \endverbatim
     </li>
     <li> Generating 20 random assignments for the plaintext and ciphertext,
     leaving the key unknown:
     \verbatim
maxima> for seed : 1 thru 20 do output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
     \endverbatim
     </li>
     <li> Running minisat-2.2.0:
     \verbatim
shell> col=1; row=2; e=8;
for r in $(seq 1 20); do
  for k in $(seq 1 20); do
      echo "Key ${k} Round ${r}";
      AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${row}_e${e}_f0.cnf ssaes_pcpair_r${r}_c${col}_rw${row}_e${e}_f0_s${k}.cnf > r${r}_k${k}.cnf;
      minisat-2.2.0 r${r}_k${k}.cnf > minisat_r${r}_k${k}.result 2>&1;
  done;
done
shell> ExtractMinisat header-only | awk ' { print $0 " r k" }' > minisat_results;
for r in $(seq 1 7); do
  for k in $(seq 1 20); do
    cat minisat_r${r}_k${k}.result | ExtractMinisat data-only | awk " { print \$0 \"  $r  $k\" }";
  done;
done >> minisat_results;
     \endverbatim
     yields:
     \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
options(width=1000)
EM
r   rn    rc         t sat         cfs          dec       rts         r1    mem ptime  stime          cfl
1  192  1672    0.0000   1      2172.9      2519.45     12.85      30390  19.00 0.000 0.0000      20832.6
2  336  3280    6.5950   1    409431.4    500513.75    890.90   10805115   8.00 0.000 0.0100    5935048.8
3  480  4888   41.2035   1   2037583.0   2750387.90   3644.10   57608122  10.30 0.003 0.0110   31753123.5
4  624  6496  175.4085   1   7296806.1   9758815.50  11511.35  211049076  19.80 0.007 0.0200  121938539.2
5  768  8104 1283.3405   1  34959474.8  45516313.85  47351.05 1298488466  46.30 0.010 0.0200  653104802.5
6  912  9712 2271.9195   1  49611107.0  64456781.95  64882.05 2092377044  62.45 0.010 0.0215  995531156.5
7 1056 11320 8378.2225   1 136949749.2 176287970.80 164861.05 6692014820 116.10 0.010 0.0300 2934315940.0
     \endverbatim
     </li>
     <li> Consider:
     \verbatim
R> plot(E$cfs)
R> plot(E$t)
     \endverbatim
     The minisat-2.2.0 times and number of conflicts grow much more quickly in
     the number of rounds than the 1-base and canonical translations. Is this
     exponential growth? </li>
     <li> We need data for more rounds. </li>
    </ul>
   </li>
  </ul>

*/
