// Matthew Gwynne, 18.7.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/008/1_1_8/general.hpp
  \brief Investigations into small-scale AES key discovery with 1 row, 1 column and 8-bit field elements


  \todo Problem specification
  <ul>
  <li> We consider the small-scale AES with 1 row, 1 column, using the 8-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,1,1,8) for r in 1,...,20. </li>
   <li> We investigate translations of the key discovery problem for
   aes(r,1,1,8) into SAT. </li>
   <li> aes(r,1,1,8) takes a 8-bit plaintext and 8-bit key and outputs a
   8-bit ciphertext. </li>
   <li> aes(r,1,1,8) applies the following operations:
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
   <li> The key schedule computes the round key i, K_i, from round key i-1,
   K_(i-1), by:
   \verbatim
K_i := S-box(K_(i-1)) + C_i
   \endverbatim
   where C_i is the 8-bit round constant for round i. </li>
   <li> The S-box is a permutation from {0,1}^8 to {0,1}^8 which we consider
   as either:
    <ul>
     <li> a 16x1 boolean function; see rijn_sbox_bf in
     ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/AdvancedEncryptionStandard.mac.
     </li>
     <li> 4 8x1 boolean functions. </li>
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
   <li> Comparing the translations:
    <ol>
     <li> "Minimum" translation:
      <ul>
       <li> All solvers solve all rounds up to 20 in less than a second. </li>
       <li> minisat-2.2.0, precosat236, precosat-570.1 all solve using far
       more conflicts than for the canonical translation. </li>
       <li> OKsolver_2002 and march_pl use a comparable number of nodes to
       the canonical translation. </li>
       <li> satz performs exponentially better using the minimum translation
       than with the canonical. </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>


  \todo Translations
  <ul>
   <li> The following translations are considered in this %plans %file:
    <ul>
     <li> The canonical box translation:
      <ul>
       <li> We treat S-boxes and additions as boxes. </li>
       <li> The S-box is considered as a 16x1 boolean function,
       translated using the canonical translation; see dualts_fcl in
       ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
       </li>
       <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
       boolean functions; translated using their prime implicates. </li>
       <li> The MixColumns operation is the identity. </li>
       <li> Due to limitations in the translation, clauses occur in this
       translation representing equivalence of variables in the MixColumns;
       See "Remove hard-coding of multiplication by 01 in small-scale
       MixColumn" in
       ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp.
       </li>
      </ul>
     </li>
     <li> The small box translation:
      <ul>
       <li> We treat S-boxes and additions as boxes. </li>
       <li> The S-box is considered as a 16x1 boolean function,
       translated using the minimum translation; see ss_sbox_cnfs in
       ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/data/SmallScaleSboxCNF.mac
       </li>
       <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
       boolean functions; translated using their prime implicates. </li>
       <li> The MixColumns operation is the identity. </li>
       <li> Due to limitations in the translation, clauses occur in this
       translation representing equivalence of variables in the MixColumns;
       See "Remove hard-coding of multiplication by 01 in small-scale
       MixColumn" in
       ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp.
       </li>
      </ul>
     </li>
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
shell> mkdir ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 1$
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
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c1_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c1_rw1_e8_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
     <li> The 1-base box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_1base_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_1base_box_aes_mc_bidirectional

# Generating a 1-base for the S-box
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

shell> oklib --maxima
Sbox1baseCNF : read_fcl_f("AES_Sbox_base.cnf")$
set_hm(ss_sbox_rbase_cnfs,8, Sbox1baseCNF)$
num_rows : 1$
num_columns : 1$
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
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c1_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c1_rw1_e8_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
     <li> The "minimum" box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_min_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_min_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 1$
exp : 8$
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
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c1_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c1_rw1_e8_f0_s${k}.cnf > r${r}_keyfind.cnf;
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
   <li> The canonical box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do minisat-2.2.0 r${r}_keyfind.cnf > minisat_r${r}.result 2>&1; done
shell> echo "n  c  t  sat  cfs dec rts r1 mem ptime stime cfl" > minisat_results
shell> for r in $(seq 1 20); do cat minisat_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk; done >> minisat_results
shell> oklib --R
R> E = read.table("minisat_results", header=TRUE)
R> E
   t  cfs   dec rts      r1 mem ptime stime     cfl
0.06  409  2107   4   46162  19  0.00  0.04   90629
0.20 1436 11129  10  306463  20  0.00  0.07  472603
0.20  696  7797   6  348743  21  0.00  0.11  162406
0.17  102   739   2   63456  21  0.01  0.15   30154
0.27  478  4035   4  399887  22  0.02  0.18  119983
0.81 2940 42135  15 1803445  26  0.01  0.24 1277362
0.31  139  2979   2   85761  23  0.02  0.27   60933
0.34  110  3842   2   38974  24  0.02  0.31   42998
0.49  600  4343   5  796689  25  0.02  0.35  162990
0.53  753 14647   6  425323  26  0.02  0.39  363891
0.68  643  5826   6 1383777  26  0.03  0.44  190498
0.61  440 10654   4  541434  26  0.04  0.47  165969
0.69  796  9559   6  746193  27  0.04  0.51  266522
0.80  532  4294   5 1502391  29  0.04  0.55   97600
0.76  740 10193   6  681178  30  0.04  0.60  255280
0.71  225  4370   3  300923  30  0.03  0.63  107865
0.73   30   412   1  125556  30  0.03  0.68   14478
0.90  255  3075   3  855796  31  0.06  0.72   93811
1.15 1140 14137   7 1969980  31  0.07  0.75  346361
1.17  631  2511   6 2285569  32  0.07  0.80  127148
     \endverbatim
     </li>
     <li> The number of conflicts is sporadic but always less than 3000. </li>
     <li> The "minimum" translation needs anywhere from 2-1000 times this!
     </li>
    </ul>
   </li>
   <li> The "minimum" box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do minisat-2.2.0 r${r}_keyfind.cnf > minisat_r${r}.result 2>&1; done
shell> echo "n  c  t  sat  cfs dec rts r1 mem ptime stime cfl" > minisat_results
shell> for r in $(seq 1 20); do cat minisat_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk; done >> minisat_results
shell> oklib --R
R> E = read.table("minisat_results", header=TRUE)
R> E
    t     cfs     dec  rts       r1 mem  stime      cfl
 0.00     517     547    5     4620  18   0.00     3081
 0.02    4366    5097   22    43097  19   0.00    40306
 0.07   10880   14117   46   115995  19   0.00   116827
 0.21   30859   41750  112   343798  19   0.00   353791
 0.04    6559    9038   31    74037  19   0.00    80695
 0.97  117049  159563  317  1388453  19   0.01  1462206
 0.64   78912  111171  252   937577  19   0.01  1000078
 0.15   18532   28638   63   220023  19   0.01   226702
 0.41   47272   67466  141   575478  19   0.01   599580
 0.62   72342  108465  223   871012  19   0.01   934208
 3.81  401564  587313  978  4884099  20   0.01  5208548
 0.94  103914  155092  264  1270577  19   0.02  1328705
 1.73  188027  283150  509  2287632  19   0.02  2434198
 1.56  161699  251844  443  2000257  20   0.03  2099626
23.76 1929716 2823253 3768 24701055  27   0.03 25268546
 1.29  136359  216718  380  1704360  20   0.02  1762894
 0.43   46432   75897  137   575767  20   0.03   619246
 7.81  702076 1068512 1532  8812093  25   0.03  9173558
37.32 2766520 4105833 4842 36254116  32   0.04 36572031
 6.83  590138  895574 1273  7513499  27   0.04  7665893
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo precosat236
  <ul>
   <li> Solving for rounds 1 to 20. </li>
   <li> The canonical box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do precosat236 r${r}_keyfind.cnf > precosat236_r${r}.result 2>&1; done
shell> echo "n  c  t  sat  cfs  dec  rts  r1  mem rnd  its  skip  enl  shk  resc  rebi  simp red  nfix  neq  nel  nmg  sb  sbn  sba  sbx  sbi  ar  arx  pb  pbph  pbr  pbf  pblf  pbmg sccnt  sccf  sccm  hshu  hshm  minln  mindel minst  mind  subf  subb  subdm  strf  strb  dom domh  domlow  mpr  memr" > precosat236_results
shell> for r in $(seq 1 20); do cat precosat236_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractPrecosat236.awk; done >> precosat236_results
shell> oklib --R
R> E = read.table("precosat236_results", header=TRUE)
R> options(width=1000)
R> E
   t       r1 mem rnd its skip enl shk resc rebi simp red  nfix neq nel nmg sb sbn sba sbx sbi   ar arx   pb pbph pbr  pbf pblf pbmg sccnt sccf sccm hshu hshm minln mindel minst mind subf subb subdm strf strb  dom domh domlow  mpr memr
 0.0    97536   1   0   0    0   0   0    0    0    1   0   562  21   1  40  0   0   0   0   0 0.00   0  278    1   2  254    0    3     0    0    0    0   37     0      0     0    0    0    0    22    0    0   38    0      0 9.75    0
 0.0   244771   2   0   0    0   0   0    0    0    1   0  1144   0   0  48  0   0   0   0   0 0.00   0  305    1   2  255    0    0     0    0    0    0   48     0      0     0    0    0    0    16    0    0  112    0      0 6.12    1
 0.1   392820   3   0   0    0   0   0    0    0    1   0  1704   0   0  67  0   0   0   0   0 0.00   0  337    1   2  255    0    0     0    0    0    0   67     0      0     0    0    0    0    16    0    0  206    0      0 5.61    1
 0.1   591456   4   0   0    0   0  12    0    0    1   0  2109 152   3 169 33   0 100   0   0 2.00   0  552    1   2  406  853   70     0    0    0    0   99     0      0     0    0   12  364    31    0    2  461    0      0 6.57    2
 0.1   688963   5   0   0    0   0   0    0    0    1   0  2824   0   0  99  0   0   0   0   0 0.00   0  401    1   2  255    0    0     0    0    0    0   99     0      0     0    0    0    0    16    0    0  478    0      0 5.74    2
 0.1   837026   6   0   0    0   0   0    0    0    1   0  3384   0   0 115  0   0   0   0   0 0.00   0  433    1   2  255    0    0     0    0    0    0  115     0      0     0    0    0    0    16    0    0  591    0      0 5.58    3
 0.2  1010721   7   0   0    0   0   0    0    0    1   0  3765 178   1 239  0   0   0   0   0 0.00   0  484    1   2  268 2640  102     0    0    0    0  137     0      0     0    0    0    0    23    0    0  733    0      0 5.62    3
 0.2  1385680   8   0   0    0   0  11    0    0    1   0  4213 288   3 324 33   0 100   0   0 2.00   0  933    1   2  625 2339  141     0    0    0    0  183     0      0     0    0   11  704    38    0    2  990    0      0 5.77    4
 0.2  1281219   9   0   0    0   0   0    0    0    1   0  5064   0   0 163  0   0   0   0   0 0.00   0  529    1   2  255    0    0     0    0    0    0  163     0      0     0    0    0    0    16    0    0  929    0      0 5.34    4
 0.4  2524166  10   0   0    0   0  52    0    0    1   0  5187 430   7 450 71   0  80   0  20 2.25   0 2469    1   2 2001 1928  178     0    0    0    0  272     0      0     0    0   71 1584    26    0   23 1293    0      0 5.87    5
 0.3  1577347  13   0   0    0   0   0    0    0    1   0  6184   0   0 195  0   0   0   0   0 0.00   0  593    1   2  255    0    0     0    0    0    0  195     0      0     0    0    0    0    16    0    0 1173    0      0 5.09    5
 0.3  1725411  13   0   0    0   0   0    0    0    1   0  6744   0   0 211  0   0   0   0   0 0.00   0  625    1   2  255    0    0     0    0    0    0  211     0      0     0    0    0    0    16    0    0 1279    0      0 5.07    5
 0.5  2548532  14   0   0    0   0  11    0    0    1   0  6814 487   3 533 33   0 100   0   0 2.00   0 1535    1   2 1017 4152  232     0    0    0    1  301     0      0     0    0   11 1239    56    0    2 1671    0      0 5.10    6
 0.4  2021539  14   0   0    0   0   0    0    0    1   0  7864   0   0 243  0   0   0   0   0 0.00   0  689    1   2  255    0    0     0    0    0    0  243     0      0     0    0    0    0    16    0    0 1522    0      0 4.93    6
 0.4  2169603  15   0   0    0   0   0    0    0    1   0  8424   0   0 259  0   0   0   0   0 0.00   0  721    1   2  255    0    0     0    0    0    0  259     0      0     0    0    0    0    16    0    0 1621    0      0 5.05    7
 0.6  3102031  16   0   0    0   0  10    0    0    1   0  8396 585   3 651 33   0 100   0   0 2.00   0 1707    1   2 1071 4814  285     0    0    0    2  366     0      0     0    0   10 1482    56    0    2 2208    0      0 5.00    7
 0.9  5105072  18   0   0    0   0  62    0    0    1   0  8797 740   7 773 71   0  80   0  20 2.25   0 4204    1   2 3412 3628  332     0    0    0    0  441     0      0     0    0   80 2771    33    0   21 2380    0      0 5.49    8
 0.5  2632345  18   0   0    0   0   0    0    0    1   0  9704 399   1 571  0   0   0   0   0 0.00   0  823    1   2  255 8407  260     0    0    0    0  311     0      0     0    0    0    0    19    0    0 2068    0      0 4.87    8
 0.6  2761859  18   0   0    0   0   0    0    0    1   0 10664   0   0 323  0   0   0   0   0 0.00   0  849    1   2  255    0    0     0    0    0    0  323     0      0     0    0    0    0    16    0    0 2093    0      0 4.85    8
 0.6  2909923  19   0   0    0   0   0    0    0    1   0 11224   0   0 339  0   0   0   0   0 0.00   0  881    1   2  255    0    0     0    0    0    0  339     0      0     0    0    0    0    16    0    0 2226    0      0 4.77    9
     \endverbatim
     </li>
     <li> All solved in "0" time. </li>
     <li> precosat236 uses no conflicts or decisions! </li>
     <li> Note that minimum translation has uses from
     ~300 to ~500000 conflicts! </li>
     <li> The number of r1 reductions grows linearly. </li>
    </ul>
   </li>
   <li> The "minimum" box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do precosat236 r${r}_keyfind.cnf > precosat236_r${r}.result 2>&1; done
shell> echo "n  c  t  sat  cfs  dec  rts  r1  mem rnd  its  skip  enl  shk  resc  rebi  simp red  nfix  neq  nel  nmg  sb  sbn  sba  sbx  sbi  ar  arx  pb  pbph  pbr  pbf  pblf  pbmg sccnt  sccf  sccm  hshu  hshm  minln  mindel minst  mind  subf  subb  subdm  strf  strb  dom domh  domlow  mpr  memr" > precosat236_results
shell> for r in $(seq 1 20); do cat precosat236_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractPrecosat236.awk; done >> precosat236_results
shell> oklib --R
R> E = read.table("precosat236_results", header=TRUE)
R> options(width=1000)
R> E
   t    cfs    dec rts      r1 mem rnd its skip enl shk resc rebi simp red nfix neq nmg   pb pbph pbr hshm   minln mindel minst mind subf subb subdm strf strb dom domh domlow  mpr memr
 0.0    276    295   2    1711   0   1   0    0   0  19    2    0    1   0   24  32  32   48    1   1   32    1635     24     0    3    5   19    32   18    2  32    0      0 0.00    0
 0.0   2480   2844   6   18523   0   1   1    9   4   1   24    2    1   0   33  48  48   96    1   1   48   21310     17     5   11    0    0    37    0    0  37    0      5 0.93    0
 0.0    855   1188   6    7057   0   1   0    0   0   0    8    0    1   0   40  64  64  144    1   1   64    8951      7     0   10    0    0    32    0    0  32    0      0 0.71    0
 0.0   4491   5867   6   38385   1   1   0   16   3   0   43    3    1   0   48  80  80  192    1   1   80   47635     11     2   15    0    0    32    0    0  32    0      0 0.77    0
 0.2  18051  23205   6  170094   1   1   0   56   8   1  176   12    2   0   56  96  96  240    1   1   96  203950      9     0   21    0    0    32    0    0  32    0      0 0.81    1
 0.2  19330  25413   5  164876   1   1   0   58   8   0  188   12    1   0   64 112 112  288    1   1  112  224227      7     1   29    0    0    32    0    0  32    0      0 0.72    1
 3.5 158902 189017   6 1589271   4   1   0  423  17   1 1553   61    2   0   72 128 128  656    2   2  128 1861754      7     8   35    0    0    32    0    0  32    0      0 0.45    9
 4.4 189729 236219   6 2173889   4   1   1  530  17   3 1854   62    3   0   81 145 145  752    2   2  145 2287121      7    15   48    0    0   321    0    0 321  273     16 0.49   11
19.6 559607 679101   5 7055110   8   1   0 1158  21   1 5470  172    2   0   88 160 160  848    2   2  160 7337196      8    19   50    0    0    33    0    0  33    0      1 0.36   38
 3.1 162774 214606   6 1835747   5   2   0  437  15   2 1591   62    3   0   96 176 176  944    2   2  176 2000911      7     5   72    0    0    32    0    0  32    0      0 0.60   10
 0.6  45143  59876   5  464747   3   1   0  124   9   0  441   23    1   0  104 192 192  528    1   1  192  548797      6     3   71    0    0    32    0    0  32    0      0 0.75    2
13.4 373295 462385   6 4852180   7   2   0  886  18   2 3649  125    3   0  112 208 208 1696    3   3  208 4702888      6    24   94    0    0    32    0    0  32    0      0 0.36   24
 0.9  63778  96995   6  732899   4   2   0  183  10   0  623   29    1   0  120 224 224  624    1   1  224  840623      6     1   65    0    0    32    0    0  32    0      0 0.79    3
20.9 469354 585482   6 6709231  11   2   0 1016  18   1 4588  140    2   1  128 240 240 1984    3   3  240 6130316      7    14   98    0    0    32    0    0  32    0      0 0.32   31
 6.2 280785 400652   6 3426519   7   2   0  659  16   2 2746  100    3   0  136 257 257 2126    3   3  257 3754082      7    11  101    0    0    32    0    0  32    0      0 0.55   18
 0.1   8650  16578   6  107081   2   2   0   30   0   0   84    6    1   0  144 272 272  768    1   1  272  123666      5     2   74    0    0    32    0    0  32    0      0 0.76    0
 7.0 284158 389980   6 3399054   8   2   0  667  15   1 2778  101    2   1  152 288 288 1616    2   2  288 3651460      6    13   99    0    0    32    0    0  32    0      0 0.48   17
 6.3 217971 295925   6 2747044   8   2   0  504  14   1 2131   77    2   0  160 304 304 1712    2   2  304 2862254      7     6  120    0    0    32    0    0  32    0      0 0.44   13
31.1 564267 730122   6 9106872  14   2   0 1174  18   2 5516  176    3   0  168 320 320 2704    3   3  320 7428406      6    20  107    0    0    32    0    0  32    0      0 0.29   37
15.3 397872 539835   6 5316500  11   2   0  953  16   1 3890  126    2   0  176 336 336 1904    2   2  336 5211466      6    17  104    0    0    32    0    0  32    0      0 0.35   25
     \endverbatim
     </li>
     <li> The number of conflicts and decisions seem quadratic in the number
     of rounds. </li>
    </ul>
   </li>
  </ul>


  \todo OKsolver_2002
  <ul>
   <li> Solving for rounds 1 to 20. </li>
   <li> The canonical box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do OKsolver_2002-O3-DNDEBUG r${r}_keyfind.cnf > oksolver_r${r}.result 2>&1; done
shell> echo "n  c  l  t  sat  nds  r1  r2  pls  ats h file n2cr  dmcl dn  dc  dl snds qnds mnds  tel  oats  n2cs  m2cs" > oksolver_results
shell> for r in $(seq 1 20); do cat oksolver_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractOKsolver.awk; done >> oksolver_results
shell> oklib --R
R> E = read.table("oksolver_results", header=TRUE)
R> options(width=200);
R> E
  t  r1   r2 ats            file   n2cr dmcl  dn  dc   dl
0.0  24  254   1  r1_keyfind.cnf   8272    0  24  72  216
0.1  32  255   0  r2_keyfind.cnf  16512    0  32  96  288
0.3  40  255   0  r3_keyfind.cnf  24752    0  40 120  360
0.7  48 1032   1  r4_keyfind.cnf  32992    0  48 144  432
0.9  56  255   0  r5_keyfind.cnf  41232    0  56 168  504
1.2  64  255   0  r6_keyfind.cnf  49472    0  64 192  576
1.6  72  773   1  r7_keyfind.cnf  57712    0  72 216  648
2.1  80 2146   1  r8_keyfind.cnf  65952    0  80 240  720
1.9  88  255   0  r9_keyfind.cnf  74192    0  88 264  792
3.5  96 3692   1 r10_keyfind.cnf  82432    0  96 288  864
2.4 104  255   0 r11_keyfind.cnf  90672    0 104 312  936
2.7 112  255   0 r12_keyfind.cnf  98912    0 112 336 1008
3.8 120 3922   1 r13_keyfind.cnf 107152    0 120 360 1080
3.1 128  255   0 r14_keyfind.cnf 115392    0 128 384 1152
3.3 136  255   0 r15_keyfind.cnf 123632    0 136 408 1224
4.8 144 4304   1 r16_keyfind.cnf 131872    0 144 432 1296
7.8 152 5896   1 r17_keyfind.cnf 140112    0 152 456 1368
4.9 160 1682   1 r18_keyfind.cnf 148352    0 160 480 1440
4.2 168  255   0 r19_keyfind.cnf 156592    0 168 504 1512
4.5 176  255   0 r20_keyfind.cnf 164832    0 176 528 1584
     \endverbatim
     </li>
     <li> Note that the OKsolver uses no nodes to solve any of these instances.
     </li>
     <li> The height for all instances is 0. </li>
     <li> Why is the height 0 here but non-zero for the "minimum" translation,
     even though both use 0 nodes? </li>
     <li> The number of r1 reductions climbs by exactly 8 each time. </li>
    </ul>
   </li>
   <li> The "minimum" box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do OKsolver_2002-O3-DNDEBUG r${r}_keyfind.cnf > oksolver_r${r}.result 2>&1; done
shell> echo "n  c  l  t  sat  nds  r1  r2  pls  ats h file n2cr  dmcl dn  dc  dl snds qnds mnds  tel  oats  n2cs  m2cs" > oksolver_results
shell> for r in $(seq 1 20); do cat oksolver_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractOKsolver.awk; done >> oksolver_results
shell> oklib --R
R> E = read.table("oksolver_results", header=TRUE)
R> options(width=200);
R> E
  t   r1    r2   h n2cr  dn  dc   dl
0.0   24    26   7   80  24  72  216
0.0   32   924  10  128  32  96  288
0.1   40  1648  12  176  40 120  360
0.1   48  2402  13  224  48 144  432
0.2   56  3156  15  272  56 168  504
0.3   64  3957  17  320  64 192  576
0.1   72   759  17  368  72 216  648
0.5   80  5432  18  416  80 240  720
0.6   88  6174  19  464  88 264  792
0.5   96  4556  20  512  96 288  864
0.8  104  7775  20  560 104 312  936
1.0  112  8557  21  608 112 336 1008
1.1  120  9305  22  656 120 360 1080
1.3  128 10076  24  704 128 384 1152
1.5  136 10909  23  752 136 408 1224
0.3  144  1657  23  800 144 432 1296
1.9  152 12418  26  848 152 456 1368
2.1  160 13310  26  896 160 480 1440
2.4  168 14020  27  944 168 504 1512
2.6  176 14786  30  992 176 528 1584
     \endverbatim
     </li>
     <li> Note that the OKsolver uses no nodes to solve any of these instances.
     </li>
     <li> The number of r1 reductions climbs by exactly 8 each time (the same
     as the canonical translation). </li>
     <li> The number of r2 reductions climbs linearly, but in a sporadic
     manner. </li>
     <li> Note that the number of r2 reductions is comparable with the
     canonical translation in this instance. </li>
    </ul>
   </li>
  </ul>


  \todo march_pl
  <ul>
   <li> Solving for rounds 1 to 20. </li>
   <li> The canonical box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do march_pl r${r}_keyfind.cnf > march_pl_r${r}.result 2>&1; done
shell> echo "n c t sat nds r1 r2" > march_pl_results
shell> for r in $(seq 1 20); do cat march_pl_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMarchpl.awk; done >> march_pl_results
shell> oklib --R
R> E = read.table("march_pl_results", header=TRUE)
R> E
     t nds    r1   r2
  7.41   3   560  519
 13.34   2  1112  257
 20.61   2  1664  257
 28.74   4  2216  850
 35.32   2  2768  257
 42.56   2  3320  257
 53.49   3  3872  542
 61.85   4  4424 1598
 64.36   2  4976  257
 84.07   4  5528 3768
 78.77   2  6080  257
 86.38   2  6632  257
106.72   4  7184 3184
101.01   2  7736  257
108.42   2  8288  257
134.41   4  8840 3543
156.20   4  9392 6672
156.82   3  9944 1075
137.40   2 10496  257
144.90   2 11048  257
     \endverbatim
     </li>
     <li> march_pl uses at most 4 nodes to solve these instances, but the
     number of nodes remains fairly constant. </li>
     <li> r1 = 552 * r + 8. What are these 552 variables? </li>
     <li> Time is related linearly to the number of rounds. However, the
     increase in time per round is significant. </li>
     <li> The canonical translation outperforms the "minimum" by a factor of
     20-1000x in terms of the number of nodes, but the "minimum" translation
     vastly outperforms the canonical here when it comes to time. </li>
    </ul>
   </li>
   <li> The "minimum" box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do march_pl r${r}_keyfind.cnf > march_pl_r${r}.result 2>&1; done
shell> echo "n c t sat nds r1 r2" > march_pl_results
shell> for r in $(seq 1 20); do cat march_pl_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMarchpl.awk; done >> march_pl_results
shell> oklib --R
R> E = read.table("march_pl_results", header=TRUE)
R> E
   t  nds    r1     r2
0.01   56   134    745
0.07  289   864  12223
0.16  466  3505  28017
0.17  334  4134  23693
0.45  967 12848  79932
0.43  938 15454  53411
0.64 1411 25190  79915
0.28  434  7772  30387
1.19 2099 41058 210471
0.85 1200 23318 166197
2.23 3048 60262 542893
2.94 3660 71135 781462
0.64 1142 22686  60756
0.45  593 12163  39037
0.51  631 13473  49788
0.33  236  5315  21442
0.45  349  7866  55636
0.61  485 11571  99054
0.76  606 13200 143224
1.05  813 18504 249992
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo satz
  <ul>
   <li> Solving for rounds 1 to 20. </li>
   <li> The canonical box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do satz215 r${r}_keyfind.cnf > satz_r${r}.result 2>&1; done
shell> echo "n  c  t  sat  nds  r1  pls  file  bck src  fix  dc  src2  fix2" > satz_results
shell> for r in $(seq 1 20); do cat satz_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractSatz.awk; done >> satz_results
shell> oklib --R
R> E = read.table("satz_results", header=TRUE)
R> options(width=150)
R> E
      t     nds       r1    bck       src     fix  dc src2 fix2
  0.004       3       52      0        89       3  44    1    1
  0.005       2       98      0       137      10  64    8    3
  0.006       2      148      0       226       5  84    8    3
  0.009      24     1052     10      1807      29 104    3    0
  0.009       8      356      1      1055       5 124    3    0
  0.015     112     5291     54     11173     141 144    0    0
  0.022     221    10505    108     24791     279 164    0    0
  0.039     414    19546    204     52745     519 184    0    0
  0.080     826    39237    410    115163    1041 204    0    0
  0.038     286    12334    139     45217     355 224    0    0
  0.168    1635    73279    814    273538    2061 244    0    0
  0.694    6764   320281   3378   1233737    8473 264    0    0
  0.731    6851   306103   3422   1335270    8605 284    0    0
  3.049   26837  1269930  13414   5643741   33589 304    0    0
  5.591   47538  2236988  23765  10590324   59522 324    0    0
 13.287  106918  5057826  53454  25474627  133741 344    0    0
 24.498  189283  8896080  94637  47464186  236810 364    0    0
 57.987  428867 20296263 214428 114187055  536279 384    0    0
105.671  755389 35476989 377689 210566490  944654 404    0    0
254.204 1713789 81100021 856888 504283111 2142635 424    0    0
     \endverbatim
     </li>
     <li> The time, number of nodes and r1 reductions seem to grow
     exponentially!?!? </li>
     <li> Why is this solver so much worse than the rest? </li>
    </ul>
   </li>
   <li> The "minimum" box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do satz215 r${r}_keyfind.cnf > satz_r${r}.result 2>&1; done
shell> echo "n  c  t  sat  nds  r1  pls  file  bck src  fix  dc  src2  fix2" > satz_results
shell> for r in $(seq 1 20); do cat satz_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractSatz.awk; done >> satz_results
shell> oklib --R
R> E = read.table("satz_results", header=TRUE)
R> options(width=150)
R> E
    t nds   r1 bck  src fix  dc src2 fix2
0.003   2   20   0  106   3  40   22    1
0.003   2   40   0  132   7  56   11    4
0.004   8  204   3  361  37  72   92   21
0.004  10  313   4  433  45  88  101   21
0.004   5  144   1  447  19 104   88   10
0.003   4  117   0  368   9 120   28    5
0.003   3  134   0  419  27 136   50   20
0.004  11  587   5  756 108 152  204   69
0.006   6  369   2  714  84 168  135   60
0.006   3  192   0  550  33 184   39   23
0.006   3  209   0  609  53 200  109   40
0.007   8  567   3  845 129 216  180  101
0.007   4  309   2  973 110 232  276   91
0.009  15 1367   7 1517 191 248  288  112
0.008   7  812   3 1634  99 264  142   60
0.009   8  875   4 1589 151 280  191   92
0.008   4  327   1 1074  54 296  140   40
0.011  19 1929   9 2338 289 312  458  181
0.009   8  736   3 1277  92 328  214   52
0.010   6  706   2 1984  94 344  156   59
     \endverbatim
     </li>
     <li> The number of nodes are very sporadic but all less than 20. </li>
     <li> This is considerably better than in the case of the canonical
     translation where there was an exponential growth in the number of nodes
     over the number of rounds. </li>
    </ul>
   </li>
  </ul>

*/
