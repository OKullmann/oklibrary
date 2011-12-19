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


  \todo Overview
  <ul>
   <li> Comparison of translations:
    <ul>
     <li> minisat-2.2.0:
      <ul>
       <li> Reading in experimental data (in R):
       \verbatim
> git clone git://github.com/MGwynne/Experimental-data.git
> cd Experimental-data/AES/1_2_8/

> E_canon = read.table("ssaes_r1-20_c2_rw1_e8_f0_k1-20_aes_canon_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)
> E_1base = read.table("ssaes_r1-20_c2_rw1_e8_f0_k1-20_aes_1base_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)
> E_min = read.table("ssaes_r1-20_c2_rw1_e8_f0_k1-20_aes_min_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)

> plot(E_canon)
> plot(E_1base)
> plot(E_min)
       \endverbatim
       </li>
       <li> Overall:
        <ul>
         <li> Ratios of total solver times (fastest to slowest):
         canonical -> x3.5 -> 1base -> x36 -> min. </li>
         <li> Ratios of total number of conflicts (low to high):
         canonical -> x1.3 -> 1base -> x1035 -> min. </li>
         <li> Calculations:
         \verbatim
> > sum(E_1base$t) / sum(E_canon$t)
[1] 3.496906
> sum(E_min$t) / sum(E_1base$t)
[1] 36.84483

> sum(E_1base$cfs) / sum(E_canon$cfs)
[1] 1.264163
> sum(as.numeric(E_min$cfs)) / sum(E_1base$cfs)
[1] 1035.704
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
oklib_monitor : true$
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
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c2_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
  done
done
     \endverbatim
     </li>
     <li> The "minimum" box translation:
      <ul>
       <li> The CNFs for the Sbox:
       \verbatim
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
oklib_monitor : true$
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
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c2_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c2_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
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
shell> cat AES_Sbox_base.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG nz
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
     16    4398     16     16    4398    30108     NA     NA    4398    30108     0
 length   count
      5       1
      6    1187
      7    2703
      8     503
      9       4
       \endverbatim
       </li>
       <li> Please see "r_1-bases" in
       AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp. </li>
       <li> Generating the instances:
       \verbatim
shell> mkdir ssaes_r1-20_c2_rw1_e8_f0_k1-1_aes_1base_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c2_rw1_e8_f0_k1-1_aes_1base_box_aes_mc_bidirectional
shell> oklib --maxima
Sbox1baseCNF : read_fcl_f("AES_Sbox_base.cnf")$
set_hm(ss_sbox_rbase_cnfs,8, Sbox1baseCNF)$
oklib_monitor : true$
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
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
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
done) > MinisatStatistics
shell> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
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
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r         t.mean            t.sd     t.min        t.max     cfs.mean         cfs.sd  cfs.min   cfs.max      r1.mean          r1.sd    r1.min      r1.max
 1     0.01084785     0.004257768  0.004999     0.020996       766.55       566.3343      136      2147       8235.8       5903.159      1557       22993
 2     0.54211690     0.569228735  0.027995     2.302650     56632.80     55052.7067     2260    220899     922111.4     940312.034     35427     3810926
 3     6.42342235     6.285189509  0.144977    21.624700    444239.00    370375.2374    14814   1289865    8335257.5    7915934.862    214295    27083768
 4    22.12978450    16.334395414  3.212510    52.043100   1340898.85    866847.1514   258691   2814681   25524993.6   17699122.261   4370696    56663965
 5    71.11845800    53.307897282  3.059530   186.707000   3775172.40   2257468.9782   280989   8195797   78480288.1   49735635.179   4782672   179512180
 6   218.37929500   231.840054090  2.048690   792.911000   8368456.35   6979849.2911   515609  24000204  202933684.1  184208817.207   9650846   638327622
 7   589.42431500   524.309836077 16.931400  1789.740000  17410261.05  12570715.1093  1801228  44569947  472670175.7  374538132.775  34875444  1310637866
 8   891.89635500   974.898020436  1.998700  2916.500000  22060118.20  20551257.0684   160407  62011747  646435851.7  661744342.497   2634845  1979193948
 9  1748.61613200  1625.494299388  3.025540  5448.590000  37958219.55  29941706.5057   634627  99588792 1185310599.7 1008584845.484  11498402  3384967795
10  3472.69134695  4345.221413487  0.268959 17695.400000  65138199.85  60380419.9217   210870 243422469 2227378760.2 2323983710.636   3334584  9515949807
11  3182.57669750  4489.878839369  1.003850 18608.700000  57403785.00  59219173.0773  1976894 242206876 1963327626.8 2334260209.592  38454187  9683605759
12  4886.10054845  5589.210101031  0.200969 21214.700000  81472226.95  71397681.9000  5889311 269193819 2899743256.6 2781835640.488 128526761 10524514586
13  7183.57609885  8904.694078226  0.144977 35372.700000 110770816.55  93874964.0365 11643618 381963719 4099049116.1 4033921179.537 286958736 16153693314
14  4935.93546580  8800.729211531  0.549916 40377.800000  73920566.90  89617131.0465   603606 420322033 2608898710.9 3791800060.105  10858497 17568155154
15 11075.98989420 10713.866193425  0.758884 42986.700000 144904267.90 105087990.5572  9964337 426189327 5477290087.6 4492209486.005 240020426 18314244303
16 14748.66769050 11958.083974345  1.243810 41716.300000 178879364.85 110110139.3523 30575991 404340520 6882947414.1 4817661490.830 831772882 17305917934
17 14443.17104810 18914.312042817  0.246962 60283.600000 159014636.30 159003927.5108  8152938 509983602 6305449874.4 7169262097.676 182459103 22925695530
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
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r      t.mean          t.sd     t.min      t.max cfs.mean      cfs.sd cfs.min cfs.max    r1.mean        r1.sd  r1.min   r1.max
 1   0.4615793   0.005564749  0.454930   0.473927   119.15    71.81575      17     275     2889.2     1727.097     518     6833
 2   1.6187545   0.584899981  0.942856   2.743580  8923.85  6591.32040     767   21441   261425.9   196831.802   18306   646793
 3   8.0068330   6.104847262  1.873720  23.430400 28599.25 26241.88919    2472   96421  1780963.4  1582793.669  150839  5771546
 4  18.3108645  10.139767614  3.526460  37.639300 36626.15 21263.68789    4763   76172  4277716.5  2567096.433  461140  9231668
 5  19.5331805  13.347561198  4.153370  46.801900 29543.65 21440.07540    4070   72156  4256317.9  3205399.411  474689 10684531
 6  21.2011310  12.518544380  3.887410  47.288800 24852.70 15500.29333    2386   56438  4486241.3  2974949.338  290938 10585186
 7  40.0424100  20.071324546  3.951400  86.306900 39455.10 19761.44305    1693   85015  8549035.0  4502468.425  190953 18738998
 8  36.5637710  29.551749160  4.290350 101.851000 29827.20 24079.44786    1725   82001  7277344.7  6441928.627  167284 21591568
 9  51.9343035  36.950744203  6.800970 118.736000 36145.90 24007.92727    5154   80951  9714717.9  7304868.030  630079 23562521
10  81.3013650  45.347694400 13.650900 154.682000 47681.95 23643.34989   11630   83057 14263757.3  8201332.545 1963255 27213992
11  63.9890450  42.616498018 10.983300 159.358000 35416.15 18563.93658   10083   76147 10298175.3  7069358.975 1335806 25735685
12 111.7904530  79.273722449  5.541160 241.914000 53812.65 27090.30729   11363  100187 17134415.0 11160614.748 1442172 35151067
13 102.2299100  69.297975663 10.438400 261.978000 49789.75 21874.15683   11195   97248 15049845.8  8953649.761 1063252 34921470
14 115.5656400  87.972356151 19.161100 280.189000 52713.20 24409.12799   21352   99968 15706080.5 10860689.628 2612974 36696943
15 132.3712725 101.191716937  6.902950 395.815000 60782.45 26275.56618   20459  124750 19073411.4 12567827.307 2081845 49219587
16 166.8045990 125.619557486  7.389880 427.361000 71623.80 32260.99055   19356  133530 23475642.8 15613370.586 1701869 53785870
17 171.2972950 117.124158236 20.274900 385.020000 71273.50 27081.99100   24580  116170 21326070.2 13477798.987 2707092 46329102
18 188.3432550 142.794399485 36.177500 474.381000 76957.50 30428.27151   40379  127455 22964810.9 16150213.747 5358812 56267993
19 243.2561450 162.522005719 42.066600 590.654000 90856.40 31703.52010   47429  165429 28763629.6 17263560.890 6415905 66463114
20 206.0684950 181.840824413 30.578400 611.119000 84723.20 33964.72450   36135  161909 25022602.2 19003406.305 4695669 68260451
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>

*/
