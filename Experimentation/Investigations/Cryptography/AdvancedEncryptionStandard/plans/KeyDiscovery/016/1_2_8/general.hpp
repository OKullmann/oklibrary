// Matthew Gwynne, 17.11.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/016/1_2_8/general.hpp
  \brief Investigations into small-scale AES key discovery with 1 row, 2 column and 8-bit field elements


  \todo Problem specification
  <ul>
  <li> We consider the small-scale AES with 1 row, 2 columns, using the 8-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,1,2,8) for r in 1,...,20. </li>
   <li> We investigate translations of the key discovery problem for
   aes(r,1,2,8) into SAT. </li>
   <li> aes(r,1,2,8) takes a 16-bit plaintext and 16-bit key and outputs a
   16-bit ciphertext. </li>
   <li> aes(r,1,2,8) applies the following operations:
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
shell> mkdir ssaes_r1-20_c2_rw1_e8_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c2_rw1_e8_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 2$
exp : 8$
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
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
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
shell> mkdir ssaes_r1-20_c2_rw1_e8_f0_k1-1_aes_min_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c2_rw1_e8_f0_k1-1_aes_min_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 2$
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
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
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
shell> mkdir ssaes_r1-20_c2_rw1_e8_f0_k1-1_aes_1base_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c2_rw1_e8_f0_k1-1_aes_1base_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 2$
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
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
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
shell> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > AllStatistics
shell> oklib --R
R> E = read.table("AllStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r           t      cfs       dec    rts          r1  stime        cfl
 1  0.04434275   414.20   7343.05   3.65     58045.6 0.0155    77515.5
 2 10.65202565 45081.55 121107.65 142.75   8979598.6 0.0300 10981254.3
 3 22.23312730 73470.05 242139.20 210.10  36451582.0 0.0495 13648817.6
 4 11.92243455 46160.20 128910.80 142.55  44979953.7 0.0600  6671845.9
 5 14.79195020 45288.60  88174.90 143.85  82437005.8 0.0800  2652047.5
 6 18.02940290 46345.00  89600.95 145.20 117374982.8 0.0910  3583296.1
 7 18.16129075 41571.80  77206.60 130.60 124531897.7 0.1105  2975819.1
 8 13.68881710 29680.30  61385.90  99.50  99277509.4 0.1285  2747224.0
 9 18.96076700 33469.15  68423.65 109.30 133027469.9 0.1450  2557203.6
10 23.18452200 37171.25  55335.80 121.05 182595426.4 0.1605  1507128.1
11 26.06352950 34892.00  52635.20 112.95 204334296.2 0.1805  1698426.1
12 17.94907200 21608.65  27948.95  76.45 151790194.2 0.1910   714493.2
13 26.48485850 27951.90  35674.30  92.70 216053491.8 0.2115   909872.6
14 33.64824085 32776.00  43427.85 107.70 258903798.3 0.2300  1129665.8
15 44.67825600 44241.70  86246.30 141.95 307220038.8 0.2505  3776197.3
16 51.64089000 45166.55  80627.05 141.35 360726274.9 0.2610  3329597.9
17 43.65541015 39062.60  86380.90 122.25 289064004.9 0.2810  4160773.6
18 48.09762150 35265.15  46468.95 116.05 360160452.1 0.2980  1243209.9
19 33.06408505 22853.30  27234.90  80.35 274047747.8 0.3205   651255.2
20 46.76654550 32975.85  49602.45 108.20 347140633.8 0.3315  1334990.6
R> aggregate_statistics(E[c("t","cfs","r1")], by=list("r"))
 r      t.mean        t.sd    t.min      t.max cfs.mean     cfs.sd cfs.min cfs.max     r1.mean        r1.sd   r1.min    r1.max
 1  0.04434275  0.01707692 0.022996   0.093985   414.20   313.4937      16    1277     58045.6     45078.55     2827    156327
 2 10.65202565  9.74909048 0.302953  30.380400 45081.55 35965.4446    3046  117335   8979598.6   7287696.36   566960  23279593
 3 22.23312730 18.08547827 0.348946  66.337900 73470.05 53288.9736    2803  193424  36451582.0  25595817.62  1083699  90319920
 4 11.92243455 12.63610775 0.298954  52.447000 46160.20 40900.1298    1201  167293  44979953.7  40093299.55  1963181 161428683
 5 14.79195020 10.90128264 0.473927  37.360300 45288.60 27567.1403    1674  103747  82437005.8  53486215.18  2633869 207149833
 6 18.02940290 11.02804229 0.270958  47.376800 46345.00 24117.9269     432  109626 117374982.8  64906085.00  1352072 235936519
 7 18.16129075 12.51459516 0.489925  45.793000 41571.80 28487.6279    1031  113606 124531897.7  72349969.14  3151118 284250040
 8 13.68881710 12.82354293 0.724889  52.035100 29680.30 25734.9770    1269   94977  99277509.4  85229490.99  5197809 358409589
 9 18.96076700 18.88404440 1.731740  71.936100 33469.15 34194.0418    3089  145630 133027469.9 108941585.53 14315250 366601111
10 23.18452200 14.00011170 3.934400  52.873000 37171.25 21385.5026    6992   80336 182595426.4 103906435.77 33946500 393013209
11 26.06352950 17.84593141 6.595000  80.057800 34892.00 23146.3912    9071  102611 204334296.2 121243899.47 60309463 523590061
12 17.94907200 11.13595947 2.089680  35.155700 21608.65 13273.4336    2403   44116 151790194.2  92539199.40 17636032 297237771
13 26.48485850 22.64219659 0.454930  75.503500 27951.90 22983.8061     283   77220 216053491.8 181459129.23  1679387 623489366
14 33.64824085 24.25543125 0.478927  76.935300 32776.00 22629.4662     387   78185 258903798.3 178003167.19  1848100 531215319
15 44.67825600 33.23486883 4.318340 111.498000 44241.70 29240.4503    4751   84033 307220038.8 207176582.86 36383050 688340227
16 51.64089000 32.77967297 3.351490 102.484000 45166.55 28453.1594    3142  112850 360726274.9 207322314.77 27554439 657473495
17 43.65541015 38.99856910 0.965853 135.851000 39062.60 37316.0679     710  153086 289064004.9 219251053.96  5572052 780633735
18 48.09762150 36.86949135 3.618450 128.060000 35265.15 26192.3558    2407   84413 360160452.1 246897192.32 28152852 862210143
19 33.06408505 28.31062949 0.716891 102.905000 22853.30 19108.7739     324   70483 274047747.8 227423737.67  2834904 866991417
20 46.76654550 33.27328622 6.064080 139.473000 32975.85 22501.7350    4333   92618 347140633.8 223806740.35 48433557 905019304
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
done) > AllStatistics
shell> oklib --R
R> E = read.table("AllStatistics", header=TRUE)
R> options(scipen=10) # suppress use of scientific notation (5E10 etc)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r              t          cfs          dec       rts           r1          cfl
 1     0.01084785       766.55       862.20      5.80       8235.8       6859.1
 2     0.54211690     56632.80     62587.15    166.20     922111.4     748835.6
 3     6.42342235    444239.00    510487.20    949.30    8335257.5    6258776.8
 4    22.12978450   1340898.85   1726207.20   2490.10   25524993.6   19943212.8
 5    71.11845800   3775172.40   4916681.60   6447.05   78480288.1   60133820.0
 6   218.37929500   8368456.35  10699265.40  13025.95  202933684.1  138904338.1
 7   589.42431500  17410261.05  21948149.50  25437.80  472670175.7  298019604.9
 8   891.89635500  22060118.20  27641619.90  30775.95  646435851.7  385040622.4
 9  1748.61613200  37958219.55  47301397.10  50639.55 1185310599.7  675428264.0
10  3472.69134695  65138199.85  80468359.75  81906.45 2227378760.2 1184556410.5
11  3182.57669750  57403785.00  71609569.20  72338.75 1963327626.8 1048612179.5
12  4886.10054845  81472226.95 101834899.25 101558.70 2899743256.6 1503284604.5
13  7183.57609885 110770816.55 138802546.65 136692.10 4099049116.1 2061882017.8
14  4935.93546580  73920566.90  94227781.75  93009.30 2608898710.9 1365653994.2
15 11075.98989420 144904267.90 183225313.60 175937.90 5477290087.6 2715622182.4
16 14748.66769050 178879364.85 226753623.80 212271.65 6882947414.1 3368014226.9
17 14443.17104810 159014636.30 201967786.45 186311.65 6305449874.4 3007177134.3
R> aggregate_statistics(E[c("t","cfs","r1")], by=list("r"))
 r     t.mean         t.sd    t.min    t.max cfs.mean     cfs.sd cfs.min cfs.max    r1.mean       r1.sd r1.min  r1.max
 1 0.00129920 0.0008009732 0.000000 0.001999    12.60   10.40445       0      38     274.25    185.0894     68     682
 2 0.00294900 0.0012343760 0.000999 0.004999   151.80   84.17813      29     329    5006.40   2878.0053    979   12031
 3 0.01164775 0.0089209350 0.001999 0.033994  1037.55  996.54870      55    3614   35272.45  34598.4919   1603  129526
 4 0.02119625 0.0160289740 0.001999 0.065989  1632.30 1425.92087      47    5731   84047.35  73311.9963   2418  276682
 5 0.04454275 0.0467369145 0.000999 0.189971  2438.15 2869.14163       8   12149  209070.45 235044.5198    480  899674
 6 0.07833770 0.0652549156 0.007998 0.205968  3571.95 2927.67843     355    9290  384206.55 341737.4855  28295 1053167
 7 0.10123410 0.0801703915 0.003999 0.245962  4012.95 3105.24267      30    9532  497897.35 412838.7988   2119 1236544
 8 0.07293845 0.0798782079 0.008998 0.329949  2694.30 2772.04863     268   11630  346702.25 423178.3829  16885 1704241
 9 0.11693170 0.0889130329 0.024996 0.311952  3838.70 2643.94544    1015    9474  574464.05 465180.6441  99962 1606866
10 0.12957990 0.1008156611 0.023996 0.338948  3968.90 2799.33103    1015    9785  621640.00 522814.5371  86499 1746761
11 0.12578050 0.0824517993 0.008998 0.261960  3695.65 2182.10450     515    7152  603521.65 425248.6003  35160 1314706
12 0.16132500 0.0843856252 0.065989 0.400939  4493.40 2143.14963    1968   10636  764176.80 417913.6579 280673 1916129
13 0.10173405 0.0795967149 0.007998 0.258960  2921.50 1957.83924     124    6953  466299.90 393628.3502   5779 1222216
14 0.12433055 0.0940194734 0.031995 0.406938  3432.65 1928.31411    1098    9252  570111.75 480194.1559 111518 2018451
15 0.14357760 0.1143185382 0.026995 0.471928  3809.70 2202.92686    1242    9423  657743.45 570416.0447  95942 2345812
16 0.20856770 0.1453429394 0.018997 0.508922  5128.80 2853.20227     858   10954  959844.05 711320.3296  47785 2455814
17 0.19292015 0.1605805175 0.028995 0.488925  4639.75 3033.84817    1431    9772  873623.40 787127.4048 104618 2357194
18 0.21926605 0.1575421525 0.033994 0.530919  5214.70 2712.66110    1654    9915  996572.55 771566.2514 111519 2558907
19 0.31230205 0.1867321948 0.068989 0.649901  6683.00 2940.27992    3183   11156 1447884.50 936455.6293 247487 3280379
20 0.29610445 0.1918973678 0.048992 0.704892  6498.80 3117.63079    2138   13543 1339773.50 924026.6407 171767 3325637
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
done) > AllStatistics
shell> oklib --R
R> E = read.table("AllStatistics", header=TRUE)
R> options(scipen=10) # suppress use of scientific notation (5E10 etc)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r           t      cfs       dec    rts         r1  stime       cfl
 1   0.4615793   119.15    131.80   1.65     2889.2 0.4400     734.8
 2   1.6187545  8923.85   9398.55  36.90   261425.9 0.8805  102213.1
 3   8.0068330 28599.25  30344.80  95.95  1780963.4 1.3300  441614.5
 4  18.3108645 36626.15  38629.25 120.60  4277716.5 1.8100  527899.9
 5  19.5331805 29543.65  31600.75  99.45  4256317.9 2.2525  454458.6
 6  21.2011310 24852.70  26768.35  85.95  4486241.3 2.6940  365864.4
 7  40.0424100 39455.10  42399.85 128.15  8549035.0 3.1400  585353.9
 8  36.5637710 29827.20  32700.65  97.20  7277344.7 3.6225  427915.0
 9  51.9343035 36145.90  40267.15 118.35  9714717.9 4.0700  519361.3
10  81.3013650 47681.95  53304.10 150.75 14263757.3 4.5100  695154.9
11  63.9890450 35416.15  41446.00 117.05 10298175.3 4.9595  555985.5
12 111.7904530 53812.65  61786.65 167.65 17134415.0 5.4435  830004.3
13 102.2299100 49789.75  59012.20 155.25 15049845.8 5.8900  828865.9
14 115.5656400 52713.20  62968.80 161.35 15706080.5 6.3305  887539.2
15 132.3712725 60782.45  72614.20 180.45 19073411.4 6.7780 1040664.8
16 166.8045990 71623.80  85736.15 209.10 23475642.8 7.2600 1237436.1
17 171.2972950 71273.50  86309.55 207.15 21326070.2 7.7100 1278912.1
18 188.3432550 76957.50  93359.35 218.85 22964810.9 8.1475 1396820.4
19 243.2561450 90856.40 109757.75 257.95 28763629.6 8.5995 1658542.9
20 206.0684950 84723.20 104262.70 246.55 25022602.2 9.0785 1603500.9
R> aggregate_statistics(E[c("t","cfs","r1")], by=list("r"))
    r      t.mean          t.sd     t.min      t.max cfs.mean      cfs.sd cfs.min cfs.max    r1.mean        r1.sd  r1.min   r1.max
1   1   0.4615793   0.005564749  0.454930   0.473927   119.15    71.81575      17     275     2889.2     1727.097     518     6833
2   2   1.6187545   0.584899981  0.942856   2.743580  8923.85  6591.32040     767   21441   261425.9   196831.802   18306   646793
3   3   8.0068330   6.104847262  1.873720  23.430400 28599.25 26241.88919    2472   96421  1780963.4  1582793.669  150839  5771546
4   4  18.3108645  10.139767614  3.526460  37.639300 36626.15 21263.68789    4763   76172  4277716.5  2567096.433  461140  9231668
5   5  19.5331805  13.347561198  4.153370  46.801900 29543.65 21440.07540    4070   72156  4256317.9  3205399.411  474689 10684531
6   6  21.2011310  12.518544380  3.887410  47.288800 24852.70 15500.29333    2386   56438  4486241.3  2974949.338  290938 10585186
7   7  40.0424100  20.071324546  3.951400  86.306900 39455.10 19761.44305    1693   85015  8549035.0  4502468.425  190953 18738998
8   8  36.5637710  29.551749160  4.290350 101.851000 29827.20 24079.44786    1725   82001  7277344.7  6441928.627  167284 21591568
9   9  51.9343035  36.950744203  6.800970 118.736000 36145.90 24007.92727    5154   80951  9714717.9  7304868.030  630079 23562521
10 10  81.3013650  45.347694400 13.650900 154.682000 47681.95 23643.34989   11630   83057 14263757.3  8201332.545 1963255 27213992
11 11  63.9890450  42.616498018 10.983300 159.358000 35416.15 18563.93658   10083   76147 10298175.3  7069358.975 1335806 25735685
12 12 111.7904530  79.273722449  5.541160 241.914000 53812.65 27090.30729   11363  100187 17134415.0 11160614.748 1442172 35151067
13 13 102.2299100  69.297975663 10.438400 261.978000 49789.75 21874.15683   11195   97248 15049845.8  8953649.761 1063252 34921470
14 14 115.5656400  87.972356151 19.161100 280.189000 52713.20 24409.12799   21352   99968 15706080.5 10860689.628 2612974 36696943
15 15 132.3712725 101.191716937  6.902950 395.815000 60782.45 26275.56618   20459  124750 19073411.4 12567827.307 2081845 49219587
16 16 166.8045990 125.619557486  7.389880 427.361000 71623.80 32260.99055   19356  133530 23475642.8 15613370.586 1701869 53785870
17 17 171.2972950 117.124158236 20.274900 385.020000 71273.50 27081.99100   24580  116170 21326070.2 13477798.987 2707092 46329102
18 18 188.3432550 142.794399485 36.177500 474.381000 76957.50 30428.27151   40379  127455 22964810.9 16150213.747 5358812 56267993
19 19 243.2561450 162.522005719 42.066600 590.654000 90856.40 31703.52010   47429  165429 28763629.6 17263560.890 6415905 66463114
20 20 206.0684950 181.840824413 30.578400 611.119000 84723.20 33964.72450   36135  161909 25022602.2 19003406.305 4695669 68260451
     \endverbatim
     </li>
    </ul>
   </li>
   <li> The canonical and 1-base translations performs *much* better than
   minimum here. Solving the key discovery instances using the minimum
   translation seems totake almost exponential time (w.r.t r). </li>
  </ul>

*/
