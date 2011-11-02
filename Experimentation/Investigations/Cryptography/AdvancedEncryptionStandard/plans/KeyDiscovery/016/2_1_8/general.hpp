// Matthew Gwynne, 20.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/016/2_1_8/general.hpp
  \brief Investigations into small-scale AES key discovery for AES with a 2x1 plaintext matrix and 8-bit field elements


  \todo Overview
  <ul>
   <li> We consider aes(r,2,1,8) for r in 1,...,20. </li>
   <li> For full description of this instance, see "Problem specification"
   below. </li>
   <li> As we increase the number of rounds, going from aes(1,2,1,8) to
   aes(20,2,1,8), the problem should get harder, and so we investigate here
   how the performance of each solver changes as we increase the number of
   rounds. </li>
   <li> Descriptions and statistics for each of the translations are provided
   in plans/KeyDiscovery/016/2_1_8/Translations.hpp.
   </li>
   <li> Comparing the translations by solving key-discovery instances over 20
   rounds (averaged over 20 distinct keys):
    <ul>
     <li> Solving with minisat-2.2.0 (better translations first):
      <ol>
       <li> canonical box translation
       (discussed in "The canonical box translation"):
        <ul>
         <li> linear relationship between rounds and time (or conflicts); </li>
         <li> highest number of rounds solved (so far): aes(20,2,1,8) in 4.5
         minutes with 74,141 conflicts. </li>
        </ul>
       </li>
       <li> 1-base box translation:
       (discussed in "The 1-base box translation"):
        <ul>
         <li> linear relationship between rounds and time (or conflicts); </li>
         <li> highest number of rounds solved (so far): aes(20,2,1,8) in
         6 minutes with 194,371 conflicts. </li>
        </ul>
       </li>
       <li> "minimum" translation:
       (discussed in 'The "minimum" box translation'):
        <ul>
         <li> non-linear (exponential?) relationship between rounds and time
         (or conflicts); </li>
         <li> highest number of rounds solved (so far): aes(7,2,1,8) in 2.3
         hours with 137 million conflicts. </li>
        </ul>
       </li>
      </ul>
     </li>
    </ul>
   </li>
  </ul>


  \todo Problem specification
  <ul>
  <li> We consider the small-scale AES with 2 rows, 1 column, using the 8-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,2,1,8) for r in 1,...,20. </li>
   <li> We investigate translations of the key discovery problem for
   aes(r,2,1,8) into SAT. </li>
   <li> aes(r,2,1,8) takes a 16-bit plaintext and 16-bit key and outputs a
   16-bit ciphertext. </li>
   <li> aes(r,2,1,8) applies the following operations:
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
   <li> The S-box is a permutation from {0,1}^8 to {0,1}^8 which we consider
   as either:
    <ul>
     <li> a 16x1 boolean function; see ss_sbox_bf in
     Cryptology/Lisp/CryptoSystems/Rijndael/AdvancedEncryptionStandard.mac;
     </li>
     <li> 8 8x1 boolean functions. </li>
    </ul>
   </li>
   <li> ??? EXACTLY ONE INFORMATION IS CRUCIAL: what type of boolean
        functions do we have ??? here we have a 32-bit boolean function,
        representing a 16-bit permutation.
        "({0,1}^8)^2" is misleading.
        *** REFERENCES *** to the boolean functions involved are needed. ***
        "permutation over {0,1}^8" is misleading: we have a 16-bit boolean
        function, representing an 8-bit permutation!
        all "{0,1}^8 to {0,1}^8" need to be removed etc., replaced by
        "16-bit boolean function, representing an 8-bit permutation".
        The crucial information must stand out! We have *boolean functions*
        here, and then one should not mention "{0,1}^8" etc. --- what else?
        Mentioning it means that it should be *different*.
        And "({0,1}^8)^2" doesn't exist for a boolean function!

   The MixColumns operation is a permutation from ({0,1}^8)^2 to
   ({0,1}^8)^2, which we consider to be defined as:
   \verbatim
MixColumns(I_1) := Mul02(I_1) + Mul03(I_2)
MixColumns(I_2) := Mul03(I_1) + Mul02(I_2)
   \endverbatim
   where
    <ul>
     <li> I_i is the i-th 8-bit word in the input; </li>
     <li> Mul02 is a permutation over {0,1}^8 representing multiplication
     by 02 in the Rijndael byte field; </li>
     <li> Mul03 is a permutation over {0,1}^8 representing multiplication
     by 03 in the Rijndael byte field. </li>
    </ul>
   </li>
   <li> The decompositions and translations are listed in "Investigating
   dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> The plaintext and ciphertext variables are then set, and the SAT
   SAT solver is run on this instance to deduce the key variables. </li>
  </ul>


  \todo The canonical box translation
  <ul>
   <li> The Sboxes and multiplications boxes are translated using the
   canonical translation, discussed in "The canonical box translation"
   in
   plans/KeyDiscovery/016/2_1_8/Translations.hpp
   where statistics are also provided. </li>
   <li> Over 20 rounds, with 20 random plaintext-ciphertext pairs per round:
    <ul>
     <li> Generating the instances:
     \verbatim
oklib_monitor : true$
for rounds : 1 thru 20 do
  output_ss_fcl_std(rounds, num_columns : 1, num_rows : 2, exp : 8, final_round_b : false, box_tran : aes_ts_box, mc_tran : aes_mc_bidirectional)$
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
done;
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
 r    rn      rc        t      cfs      dec    rts           r1    mem  ptime  stime        cfl
 1  3264   52508   0.0650   458.95   6076.0   4.00     423525.7  11.00 0.0215 0.0400   124822.4
 2  6480  104952  75.7310 67356.80 242387.5 192.40   97916518.9 139.60 0.0410 0.0800 21278854.4
 3  9696  157396  41.2970 53390.60 248458.1 157.80  109114823.9 106.95 0.0660 0.1200 17142892.6
 4 12912  209840  73.2580 64889.50 239838.5 186.90  235101310.2 130.65 0.0870 0.1600 18468994.6
 5 16128  262284 102.0715 72181.40 278187.7 208.95  345243337.9 159.45 0.1095 0.2000 21840963.2
 6 19344  314728  90.4860 65694.70 255653.8 191.80  391090908.6 134.80 0.1345 0.2400 18871607.4
 7 22560  367172  69.3640 43421.75 181484.0 135.20  308959433.1 106.95 0.1585 0.2800 13469693.6
 8 25776  419616  82.4390 52619.20 239547.9 155.60  415826427.1 127.40 0.1790 0.3200 14776510.8
 9 28992  472060 102.6980 57760.20 256061.6 170.55  478385590.6 146.30 0.2000 0.3600 18764325.2
10 32208  524504 145.2080 74473.55 328906.0 207.35  692318870.9 176.85 0.2260 0.4015 23922493.2
11 35424  576948 219.1985 88613.30 325327.2 250.20  988175081.3 203.60 0.2430 0.4440 27092520.0
12 38640  629392 134.8720 58642.20 239197.1 175.30  712113529.7 137.00 0.2725 0.4810 15833605.6
13 41856  681836 240.9780 84993.45 334794.7 238.25 1178269114.9 201.85 0.2940 0.5240 24913310.5
14 45072  734280 216.8590 73503.25 299043.2 215.90 1022098704.8 194.25 0.3150 0.5675 22868749.3
15 48288  786724 196.6845 65558.30 308100.1 189.20 1012224859.0 182.30 0.3385 0.6085 21748863.7
16 51504  839168 237.2150 75545.25 286732.1 219.90 1334688295.2 181.20 0.3640 0.6480 21172958.2
17 54720  891612 226.4960 71584.25 326660.5 198.10 1145534955.9 201.45 0.3875 0.6900 22636613.2
18 57936  944056 235.7845 73793.80 311720.4 209.20 1297866385.2 193.85 0.4065 0.7295 22467222.5
19 61152  996500 187.8575 56659.20 263318.7 165.95 1129640755.5 172.10 0.4295 0.7700 16518592.8
20 64368 1048944 268.0105 74141.35 349173.7 206.25 1358448327.8 225.15 0.4555 0.8105 22652719.3
     \endverbatim
     </li>
     <li> Consider:
     \verbatim
R> plot(EM$cfs)
R> plot(EM$t)
     \endverbatim
     The minisat-2.2.0 times and number of conflicts appear to be linear in
     the number of rounds! </li>
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
