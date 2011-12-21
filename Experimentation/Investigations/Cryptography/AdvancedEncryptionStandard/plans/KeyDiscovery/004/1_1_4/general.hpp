// Matthew Gwynne, 6.6.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/004/1_1_4/general.hpp
  \brief Investigations into small-scale AES key discovery with 1 row, 1 column and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> We consider the small-scale AES with 1 row, 1 column, using the 4-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,1,1,4) for r in 1,...,20. </li>
   <li> We investigate translations of the key discovery problem for
   aes(r,1,1,4) into SAT. </li>
   <li> aes(r,1,1,4) takes a 4-bit plaintext and 4-bit key and outputs a
   4-bit ciphertext. </li>
   <li> aes(r,1,1,4) applies the following operations:
    <ol>
     <li> Key schedule which takes the key and generates r+1 4-bit round
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
   where C_i is the round constant for round i. </li>
   <li> The S-box is a permutation from {0,1}^4 to {0,1}^4 which we consider
   as either:
    <ul>
     <li> an 8x1 boolean function; see ss_sbox_bf in
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
   <li> Comparing the translations using a single plaintext-ciphertext pair:
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
     <li> Canonical translation:
      <ul>
       <li> All solvers except satz215 solve all rounds up to 20 in less than
       a second. </li>
       <li> satz215 shows exponential behaviour for the time, nodes and r1
       reductions in the number of rounds!?!? See "satz". </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>


  \todo Remove linear diffusion from translation
  <ul>
   <li> Currently the AES linear diffusion operation creates equality
   clauses in the translation of this AES instance, as the translation
   does not check if the MixColumns matrix is the identity matrix. </li>
   <li> These equivalences clauses can only get in the way for the solvers
   and skew results, and therefore the translation should be updated to
   check for the identity matrix and rename variables instead. </li>
  </ul>


  \todo Translations
  <ul>
   <li> The following translations are considered in this %plans %file:
    <ul>
     <li> The canonical box translation. </li>
     <li> The small box translation. </li>
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
shell> mkdir ssaes_r1-20_c1_rw1_e4_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c1_rw1_e4_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
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
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c1_rw1_e4_f0.cnf ssaes_pcpair_r${r}_c1_rw1_e4_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
     <li> The 1-base box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c1_rw1_e4_f0_k1-1_aes_1base_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c1_rw1_e4_f0_k1-1_aes_1base_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 1$
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
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c1_rw1_e4_f0.cnf ssaes_pcpair_r${r}_c1_rw1_e4_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
     <li> The "minimum" box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c1_rw1_e4_f0_k1-1_aes_min_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c1_rw1_e4_f0_k1-1_aes_min_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 1$
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
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c1_rw1_e4_f0.cnf ssaes_pcpair_r${r}_c1_rw1_e4_f0_s${k}.cnf > r${r}_keyfind.cnf;
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
   t  cfs dec rts    r1 mem  stime  cfl
0.00    7  14   1   198  18   0.00   83
0.00   21  46   1   833  18   0.00  343
0.00   33  73   1  1974  18   0.00  572
0.00   10  23   1  1065  19   0.00  169
0.00   16  36   1  1868  19   0.00  199
0.00   29  89   1  3443  19   0.00  404
0.00    4  18   1  1409  19   0.00   92
0.00   23  44   1  4726  19   0.00  129
0.00   10  24   1  3761  19   0.00  164
0.00   22  41   1  6095  19   0.00  295
0.00    0  17   1   558  19   0.00    0
0.00   72 116   1 15165  19   0.00  711
0.00    2  17   1  1748  19   0.00   43
0.00   87 163   1 21722  19   0.00 1280
0.00    4  17   1  3299  19   0.00   86
0.01   51 144   1 13035  19   0.01  800
0.00   10  31   1  6107  19   0.00   88
0.00   85 166   1 21253  19   0.00 1423
0.01   10  25   1  8445  19   0.01  158
0.01  107 252   2 33586  19   0.00 1149
     \endverbatim
     </li>
     <li> The number of conflicts and decisions is sporadic. This experiment
     should be considered over several keys. </li>
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
cfs  dec rts    r1 mem  cfl
 10   14   1    86  18   31
 23   25   1   308  18   82
 32   38   1   647  18  148
 11   15   1   267  18   55
 32   42   1   632  18  159
 82   99   1  1887  18  399
 30   51   1   701  18  166
147  171   2  3926  18  772
167  231   2  4523  18  969
102  165   2  2657  18  650
169  244   2  4321  18  887
334  511   3 10116  19 2029
115  214   2  3309  19  705
536  803   5 16395  19 3407
391  624   3 11737  19 2632
271  490   3  7484  19 1901
261  493   3  7512  19 1788
535  917   5 19596  19 3376
297  523   3  9970  19 2149
680 1225   6 24639  19 4691
     \endverbatim
     </li>
     <li> The time is "0" for all rounds. </li>
     <li> The number of conflicts and decisions seems quadratic in
     the number of rounds, although experiments should be done over
     more rounds with more keys to better gauge this. </li>
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
    r1 mem shk nfix neq nel nmg sb   sba sbx sbi   ar arx  pb pbr pbf pblf pbmg hshm subf subb subdm strf strb dom   mpr
  1117   0   0   52  15   1  23  0   0.0 0.0 0.0 0.00   0  30   2  14   18    6   17    0    0     8    0    0  17  0.00
  2105   0   0  124   0   0  24  0   0.0 0.0 0.0 0.00   0  41   2  15    0    0   24    0    0     8    0    0  28  0.00
  4200   0   0  135  44   1  60  0   0.0 0.0 0.0 0.00   0  70   2  22   77   28   32    0    0     8    0    0  47  0.00
  5000   0   0  172  63   1  82  0   0.0 0.0 0.0 0.00   0  80   2  16  113   42   40    0    0     8    0    0  62  0.00
 19778   0  10  182 107   3 118 33 100.0 0.0 0.0 2.00   0 269   3  75   68   19   99   11  150    37    0    2 142  0.00
 25155   0  11  215 130   3 142 33 100.0 0.0 0.0 2.00   0 298   3  82   89   40  102   12  176    37    0    3 163  0.00
  9842   0   0  304  99   1 132  0   0.0 0.0 0.0 0.00   0 134   2  22  213   68   64    0    0     8    0    0 126  0.00
  8729   0   0  460   0   0  72  0   0.0 0.0 0.0 0.00   0 137   2  15    0    0   72    0    0     8    0    0 124  0.00
 12471   0   0  378 137   1 174  0   0.0 0.0 0.0 0.00   0 165   2  21  276   94   80    0    0     8    0    0 174  0.00
 14082   0   0  432 139   1 181  0   0.0 0.0 0.0 0.00   0 182   2  22  320   93   88    0    0     8    0    0 195  0.00
109711   0  79  343 266  19 275 74  85.7 7.1 7.1 2.08   2 822   3 226   56   62  213  156  601    61   13   42 372 10.97
 13145   0   0  684   0   0 104  0   0.0 0.0 0.0 0.00   0 201   2  15    0    0  104    0    0     8    0    0 188  0.00
115424   0  11  445 292   3 311 33 100.0 0.0 0.0 2.00   0 741   3 227  139   73  238   11  456    65    0    4 362 11.54
 15353   0   0  796   0   0 120  0   0.0 0.0 0.0 0.00   0 233   2  15    0    0  120    0    0     8    0    0 221  0.00
 19327   0   0  646 205   1 270  0   0.0 0.0 0.0 0.00   0 257   2  17  496  142  128    0    0     8    0    0 286  0.00
 20997   1   0  700 207   1 286  0   0.0 0.0 0.0 0.00   0 274   2  18  531  150  136    0    0     8    0    0 306  0.00
 21904   1   0  741 222   1 297  0   0.0 0.0 0.0 0.00   0 289   2  17  573  153  144    0    0     8    0    0 321  0.00
 19769   1   0 1020   0   0 152  0   0.0 0.0 0.0 0.00   0 297   2  15    0    0  152    0    0     8    0    0 284  0.00
 24484   1   0  833 242   1 327  0   0.0 0.0 0.0 0.00   0 321   2  17  647  167  160    0    0     8    0    0 364  2.45
 21977   1   0 1132   0   0 168  0   0.0 0.0 0.0 0.00   0 329   2  15    0    0  168    0    0     8    0    0 317  2.20
     \endverbatim
     </li>
     <li> All solved in "0" time. </li>
     <li> precosat236 uses no conflicts or decisions! </li>
     <li> The number of r1 reductions grows linearly except for outliers for
     rounds 11 and 13. </li>
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
cfs  dec rts    r1 rnd its skip shk resc nfix neq nel nmg sb sbn sba sbx sbi ar  pb hshu hshm minln mindel minst mind subf subb subdm strf strb dom domh domlow  mpr
  0    0   0    93   0   0    0   9    0   13  16   7  16 57  50  25   0  25  2  24    1   16     0      0     0    0   15   21    16   29   17  16    0      0 0.00
  8   11   0   205   1   0    0   0    0   16  24   0  24  0   0   0   0   0  0  48    0   24    35      3     0    2    0    0    16    0    0  16    0      0 0.00
 33   40   0   608   1   1    0   1    0   21  33   0  33  0   0   0   0   0  0  72    0   33   123     23     0    7    0    0    23    0    0  23    5      2 0.00
 77   95   0  1483   1   1    0   1    0   25  40   0  40  0   0   0   0   0  0  96    0   40   361     18     2    7    0    0    24    0    0  24    5      3 0.00
 14   41   0   453   1   0    0   0    0   28  48   0  48  0   0   0   0   0  0 120    0   48    62      3     0    1    0    0    16    0    0  16    0      0 0.00
 35   79   0   828   1   0    0   0    0   32  56   0  56  0   0   0   0   0  0 144    0   56   171     13     2    8    0    0    16    0    0  16    0      0 0.00
140  200   1  2953   1   1    0   1    1   37  65   0  65  0   0   0   0   0  0 168    0   65   716     17     0   18    0    0    29    0    0  29    3     10 0.00
169  298   1  3985   1   3    0   3    1   44  75   0  75  0   0   0   0   0  0 192    0   75   769     16     1   24    0    0    63    0    0  63   26     21 0.00
106  191   1  2395   1   0    0   0    1   44  80   0  80  0   0   0   0   0  0 216    0   80   576     11     0   18    0    0    16    0    0  16    0      0 0.00
136  238   1  3204   1   0    0   0    1   48  88   0  88  0   0   0   0   0  0 240    0   88   761     15     1   18    0    0    16    0    0  16    0      0 0.00
 34  142   0  1301   1   0    0   0    0   52  96   0  96  0   0   0   0   0  0 264    0   96   209     11     0   11    0    0    16    0    0  16    0      0 0.00
280  545   2  6620   1   0    0   0    2   56 104   0 104  0   0   0   0   0  0 288    0  104  1621     15     1   33    0    0    18    0    0  18    0      2 0.00
 49  217   0  1787   1   0    0   0    0   60 112   0 112  0   0   0   0   0  0 312    0  112   303     11     1    9    0    0    16    0    0  16    0      0 0.00
208  406   2  5114   1   0    0   0    1   64 120   0 120  0   0   0   0   0  0 336    0  120  1196     10     1   19    0    0    16    0    0  16    0      0 0.00
116  210   1  2882   1   0    0   0    1   68 128   0 128  0   0   0   0   0  0 360    0  128   816     11     0   22    0    0    16    0    0  16    0      0 0.00
130  369   1  3986   1   0    0   0    1   72 136   0 136  0   0   0   0   0  0 384    0  136   702      9     0   30    0    0    16    0    0  16    0      0 0.00
302  639   2  8414   1   0    0   0    2   76 144   0 144  0   0   0   0   0  0 408    0  144  1900     10     0   67    0    0    16    0    0  16    0      0 0.00
765 1510   3 22436   1   7    2   7    7  272 152   0 160  0   0   0   0   0  0 432    2  160  4305     19     8   45    0    0    87    0    0  88   50     22 2.24
387  954   2 11248   1   0    0   0    3   84 160   0 160  0   0   0   0   0  0 456    0  160  2459     17     4   91    0    0    16    0    0  16    0      0 0.00
838 1730   3 26307   1   2    3   2    8   90 169   0 169  0   0   0   0   0  0 480    0  169  5124     25     2   63    0    0    60    0    0  60   38      6 2.63
     \endverbatim
     </li>
     <li> All solved in "0" time. </li>
     <li> The number of conflicts and decisions seem quadratic in the number
     of rounds. </li>
    </ul>
   </li>
  </ul>


  \todo precosat-570.1
  <ul>
   <li> Solving for rounds 1 to 20. </li>
   <li> The canonical box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do precosat-570.1 -v r${r}_keyfind.cnf > precosat-570.1_r${r}.result 2>&1; done
shell> echo "n  c  t  sat  cfs  dec  rts  r1  pls ats  mem  ptime  file rnd  its  skip  enl shk  resc  rebi  simp  red   ar  arx  atssz bck  bckct  bckj  bckjln  blkres  blkph  blkr blk  blkimp  blkexp  clsrec  clspur  clsaut  dom domh  domlow  elres  elph  elr  extf  extass extfl  glu  glusl  minln  mindel  minst  mininv mind  otfsd  otfsd2  otfsd3  otfsdlg  otfss  otfss2 otfss3  otfsslg  pb  pbph  pbr  pbf  pblf  pbmg mpr  plexp  plel  plblk  plaut  sb  sbn  sba sbx  sbi  sccnt  sccf  sccm  strf  strb  strdyn strorg  strasy  subf  subb  subdyn  suborg  subdm  subgc  srtime  otime  nfix  neq  nel  npur  nzmb naut  zmb  zmbexp  zmbel  zmbblk  memr" > precosat-570.1_results
shell> for r in $(seq 1 20); do cat precosat-570.1_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractPrecosat570.awk; done >> precosat-570.1_results
shell> oklib --R
R> E = read.table("precosat-570.1_results", header=TRUE)
R> options(width=1000)
R> E
cfs dec   r1 mem  its bck bckct bckj bckjln blkres blk blkexp clsrec dom domh domlow elres extf extass extfl  glu glusl minln mindel mind otfsd otfsd2 otfsdlg otfsslg  pb sccnt  sccm strf strasy  subdm nfix neq nel
  4   7  136   0    1   4   6.5    0    0.0   1312  32     32    114  25    0      5   660    1      1     0 1.33 0.000    34      0    2     1      1       0       0  48     8    16  112      8     16   35  16   2
  9   9  467   0    3   9   9.6    0    0.0   2672  64     64    212  48    0     26  1382    1      3     0 1.33 0.000    31     60    2     3      3       0       0  88    16    24  224     14     17   96  24   4
  8  10  585   0    1   8  10.8    0    0.0   4032  96     96    286  42   13      7  2128    0      6     0 1.67 0.000    55     41    2     1      1       0       0 128    24    32  336     17     16   52  32   6
  3   5  412   0    0   3   2.0    0    0.0   5392 128    128    376  24    0      0  2858    0      8     0 2.50 0.000    24     37    2     0      0       0       0 168    32    40  448     22     16   46  40   8
  5   6  601   0    0   5  12.8    0    0.0   6752 160    160    450  73    3     48  3602    3      7     0 1.33 0.000    28     48    2     0      0       0       0 208    40    48  560     25     18   53  48  10
  6   8  733   0    0   6  10.0    0    0.0   8112 192    192    524  26    6      4  4350    1     11     0 1.60 0.000    41     37    2     0      0       0       0 248    48    56  672     28     16   60  56  12
  4   7  492   0    0   4   2.2    0    0.0   9472 224    224    622  34   12      0  5072    0     14     0 2.00 0.000    22     39    2     0      0       0       0 288    56    64  784     34     16   70  64  14
 12  11 1103   0    3  11   4.7    0    0.0  10832 256    256    712  83   15     36  5798    2     14     0 1.67 0.000    59     36    2     4      3       1       0 328    64    72  896     39     18  372  72  16
  1   4  434   0    0   1   0.0    0    0.0  12192 288    288    778  18    0      0  6556    0     18     0 1.00 0.000     5      0    1     0      0       0       0 368    72    80 1008     41     16   85  80  18
 12  22 1299   0    3  12   6.3    4    3.0  13552 320    320    844  42   18      2  7308    2     18     0 1.86 1.000    50     34    2     3      3       0       0 408    80    88 1120     43     16   94  88  20
  2   4  587   0    0   2   0.0    0    0.0  14912 352    352    878  28    0      0  8092    2     20     0 2.50 0.000    21     25    3     0      0       0       0 448    88    96 1232     41     16   93  96  22
 12  22 1379   0    3  12   6.2    4    3.0  16272 384    384   1024  40   18      2  8768    2     22     0 1.86 1.000    50     33    2     3      3       0       0 488    96   104 1344     53     16  112 104  24
  9  14 1396   0    3   9   4.6    2    2.5  17632 416    416   1050  77   46     13  9560    0     26     0 1.83 0.667    57     24    3     3      3       0       0 528   104   112 1456     50     17  123 112  26
 12  22 1511   0    3  12   6.2    4    3.0  18992 448    448   1188  45   19      2 10244    2     26     0 1.86 1.000    51     33    2     3      3       0       0 568   112   120 1568     61     16  128 120  28
 11  17 1402   1    2  11   3.2    4    2.0  20352 480    480   1230  89   73      0 11022    6     24     0 1.50 0.000    44     44    1     2      2       0       0 608   120   128 1680     60     16  130 128  30
  0   4  740   1    0   0   0.0    0    0.0  21712 512    512   1296  24    0      0 11778    3     29     0 0.00 0.000     0      0    0     0      0       0       0 648   128   136 1792     62     16  134 136  32
 11  17 1510   1    2  11   3.2    4    2.0  23072 544    544   1378 102   79      1 12516    6     28     0 1.44 0.000    50     34    2     2      2       0       0 688   136   144 1904     66     16  144 144  34
 12  22 1695   1    3  12   6.2    4    3.0  24432 576    576   1500  46   18      2 13216    3     33     0 1.86 1.000    51     33    2     3      3       0       0 728   144   152 2016     75     16  158 152  36
 11  17 1611   1    2  11   3.2    4    2.0  25792 608    608   1526 118   95      1 14008    7     31     0 1.44 0.000    50     35    1     2      2       0       0 768   152   160 2128     72     16  158 160  38
 12  22 1767   1    3  12   6.2    4    3.0  27152 640    640   1664  44   18      2 14688    4     36     0 1.86 1.000    51     33    2     3      3       0       0 808   160   168 2240     83     16  174 168  40
     \endverbatim
     </li>
     <li> precosat-570.1 uses no restarts for all rounds. </li>
     <li> precosat-570.1 uses a sporadic number of decisions and conflicts. Why
     is this so different to precosat236? </li>
     <li> Note that the number of conflicts is always <= 12. </li>
     <li> The number of r1 reductions seems linear:
     \verbatim
R> glm(seq(1,20) ~ E$r1)

Call:  glm(formula = seq(1, 20) ~ E$r1)

Coefficients:
(Intercept)         E$r1
   0.843286     0.009725

Degrees of Freedom: 19 Total (i.e. Null);  18 Residual
Null Deviance:	    665
Residual Deviance: 180.6 	AIC: 106.8
     \endverbatim
     </li>
    </ul>
   </li>
   <li> The "minimum" box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do precosat-570.1 -v r${r}_keyfind.cnf > precosat-570.1_r${r}.result 2>&1; done
shell> echo "n  c  t  sat  cfs  dec  rts  r1  pls ats  mem  ptime  file rnd  its  skip  enl shk  resc  rebi  simp  red   ar  arx  atssz bck  bckct  bckj  bckjln  blkres  blkph  blkr blk  blkimp  blkexp  clsrec  clspur  clsaut  dom domh  domlow  elres  elph  elr  extf  extass extfl  glu  glusl  minln  mindel  minst  mininv mind  otfsd  otfsd2  otfsd3  otfsdlg  otfss  otfss2 otfss3  otfsslg  pb  pbph  pbr  pbf  pblf  pbmg mpr  plexp  plel  plblk  plaut  sb  sbn  sba sbx  sbi  sccnt  sccf  sccm  strf  strb  strdyn strorg  strasy  subf  subb  subdyn  suborg  subdm  subgc  srtime  otime  nfix  neq  nel  npur  nzmb naut  zmb  zmbexp  zmbel  zmbblk  memr" > precosat-570.1_results
shell> for r in $(seq 1 20); do cat precosat-570.1_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractPrecosat570.awk; done >> precosat-570.1_results
shell> oklib --R
R> E = read.table("precosat-570.1_results", header=TRUE)
R> options(width=1000)
R> E
cfs dec rts   r1 its resc simp ar bck bckct bckj bckjln blkres clsrec dom domh domlow elres extf extass  glu glusl minln mindel mind otfsd otfsd2 otfsd3 otfsdlg  pb  sb   sbn  sba sccnt sccm strf strb strasy subf subb subdm nfix neq nel nzmb
  0   0   0   15   0    0    1  2   0   0.0    0    0.0    227     99  16    0      0   320    2      7 0.00 0.000     0      0    0     0      0      0       0  32  67  83.3 16.7     4   12   15   13      2   13   11     8   14  12   9    1
 14  24   0  131   0    0    1  0  13   0.1    3    3.7    532     50  10    0      0   584    1      3 2.42 2.500    51      7    3     1      0      1       0  64 100 100.0  0.0    12   20    0    0      0    0    0     8   16  20   4    0
 12  26   0  161   0    0    1  0  11   0.0    4    3.8    837     50  24    0      0   811    0      4 2.45 1.333    52      2    3     1      0      0       1  96 100 100.0  0.0    20   28    0    0      0    0    0     8   20  28   4    0
 40  71   0  559   0    0    1  0  39   1.3   12    3.6   1148     50  52    0     40  1038    1      3 2.14 1.714   166     17    4     1      0      1       0 128 100 100.0  0.0    28   36    0    0      0    0    0    10   24  36   4    0
 35  68   0  488   0    0    1  0  34   0.0    7    5.3   1450     50  14    0      0  1256    3      1 2.45 1.000   166     19    8     1      0      1       0 160 100 100.0  0.0    36   44    0    0      0    0    0     8   28  44   4    0
  4  31   0  148   0    0    1  0   4   0.0    3    8.3   1759     50  12    0      0  1480    2      2 2.75 0.000    17      0    0     0      0      0       0 192 100 100.0  0.0    44   52    0    0      0    0    0     8   32  52   4    0
 44 102   0  760   0    0    1  0  43   0.0   13    5.2   2060     50  10    0      0  1714    1      3 2.17 1.000   208     22   11     1      0      1       0 224 100 100.0  0.0    52   60    0    0      0    0    0     8   36  60   4    0
128 269   3 2515   1    1    1  0 126   0.5   32    4.4   2355     50  31    6     11  1924    1      3 2.16 0.328   594     15   35     3      1      1       1 256 100 100.0  0.0    60   68    0    0      0    0    0    19   41  68   4    0
194 320   2 3894   1    1    1  0 185   0.4   38    3.9   2663     50 112    2     98  2156    1      3 2.75 0.434   998     17   24    10      1      2       7 288 100 100.0  0.0    68   76    0    0      0    0    0    19   45  76   4    0
144 351   4 2985   0    1    1  0 141   0.0   46    4.6   2967     50  16    0      0  2390    3      1 2.48 0.625   807     18   31     3      0      0       3 320 100 100.0  0.0    76   84    0    0      0    0    0     8   48  84   4    0
157 417   6 3541   0    1    1  0 155   0.0   44    5.4   3264     50  12    0      0  2595    1      3 2.54 0.345   856      7   25     2      0      0       2 352 100 100.0  0.0    84   92    0    0      0    0    0     8   52  92   4    0
308 580   8 6995   0    2    1  0 299   0.2   58    4.4   3572     50 128    0    114  2829    1      3 2.89 0.324  1711     16   34     9      0      1       8 384 100 100.0  0.0    92  100    0    0      0    0    0    13   56 100   4    0
 68 212   2 1543   0    0    1  0  67   0.0   21    6.5   3873     50  12    0      0  3050    1      3 2.51 0.750   345     18   11     1      0      0       1 416 100 100.0  0.0   100  108    0    0      0    0    0     8   60 108   4    0
154 462   6 3957   0    1    1  0 151   0.0   42    6.6   4167     50  12    0      0  3271    1      3 2.70 0.326   887     19   29     3      0      0       3 448 100 100.0  0.0   108  116    0    0      0    0    0     8   64 116   4    0
170 520   9 4200   0    1    1  0 168   0.0   51    5.6   4473     50  16    0      0  3503    1      3 2.79 0.451   974     11   43     2      0      0       2 480 100 100.0  0.0   116  124    0    0      0    0    0     8   68 124   4    0
203 626   9 5732   0    1    1  0 199   0.0   56    6.4   4773     50  24    0      0  3721    1      3 3.12 0.444  1258     16   48     4      0      0       4 512 100 100.0  0.0   124  132    0    0      0    0    0     8   72 132   4    0
216 676  11 6053   0    2    1  0 212   0.0   77    5.1   5082     50  12    0      0  3959    1      3 2.95 0.440  1259     14   43     4      0      0       4 544 100 100.0  0.0   132  140    0    0      0    0    0     8   76 140   4    0
259 817  12 7826   0    2    1  0 258   0.0   89    5.6   5387     50   8    0      0  4173    1      3 2.84 0.320  1630     22   41     1      0      0       1 576 100 100.0  0.0   140  148    0    0      0    0    0     8   80 148   4    0
269 825  13 7871   0    2    1  0 266   0.0   72    6.2   5690     50  12    0      0  4394    1      3 2.97 0.451  1619     12   66     3      0      0       3 608 100 100.0  0.0   148  156    0    0      0    0    0     8   84 156   4    0
332 947  14 9235   0    3    1  0 330   0.0   93    5.6   5995     50  12    0      0  4617    1      3 3.32 0.393  2104     13   83     2      0      0       2 640 100 100.0  0.0   156  164    0    0      0    0    0     8   88 164   4    0
     \endverbatim
     </li>
     <li> precosat-570.1 uses no restarts for all rounds. </li>
     <li> The number of conflicts and decisions seems linear or quadratic.
     Note this is considerably more than with the canonical translation.
     </li>
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
r1  r2 ats n2cr dn  dc  dl
12  19   1  296 12  36 108
16  15   0  576 16  48 144
20  43   1  856 20  60 180
24  71   1 1136 24  72 216
28 105   1 1416 28  84 252
32  84   1 1696 32  96 288
36  92   1 1976 36 108 324
40  15   0 2256 40 120 360
44 156   1 2536 44 132 396
48 148   1 2816 48 144 432
52 135   1 3096 52 156 468
56  15   0 3376 56 168 504
60 161   1 3656 60 180 540
64  15   0 3936 64 192 576
68 223   1 4216 68 204 612
72 207   1 4496 72 216 648
76 239   1 4776 76 228 684
80  15   0 5056 80 240 720
84 283   1 5336 84 252 756
88  15   0 5616 88 264 792
     \endverbatim
     </li>
     <li> Note that the OKsolver uses no nodes to solve any of these instances.
     </li>
     <li> The number of r1 reductions climbs by exactly 4 each time. </li>
     <li> The number of r2 reductions climbs, but not in a perfect straight
     line. </li>
     <li> Also the number or r2 reductions drops to 15 at rounds 8, 12, 14, 17
     and 20. </li>
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
r1  r2 ats h n2cr dn  dc  dl
12   3   1 2   40 12  36 108
16  13   0 2   64 16  48 144
20   6   0 2   88 20  60 180
24   2   0 3  112 24  72 216
28  10   0 5  136 28  84 252
32  34   0 5  160 32  96 288
36  61   0 4  184 36 108 324
40 117   0 5  208 40 120 360
44 102   0 5  232 44 132 396
48 128   0 5  256 48 144 432
52  51   0 5  280 52 156 468
56 106   0 5  304 56 168 504
60  94   0 5  328 60 180 540
64 240   0 5  352 64 192 576
68 101   0 5  376 68 204 612
72 132   0 5  400 72 216 648
76 256   1 5  424 76 228 684
80  95   0 4  448 80 240 720
84  27   0 3  472 84 252 756
88  58   0 4  496 88 264 792
     \endverbatim
     </li>
     <li> Note that the OKsolver uses no nodes to solve any of these instances.
     </li>
     <li> The number of r1 reductions climbs by exactly 4 each time (the same
     as the canonical translation). </li>
     <li> The number of r2 reductions climbs, but in a sporadic manner. </li>
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
   t nds   r1   r2
0.00   3   56   24
0.00   3  108   53
0.00   3  160   99
0.00   3  212  110
0.01   4  264  285
0.00   4  316  281
0.01   3  368  185
0.01   2  420    5
0.01   3  472  267
0.01   3  524  299
0.04   4  576 1066
0.01   2  628    3
0.03   4  680  629
0.01   2  732    2
0.03   3  784  349
0.03   3  836  383
0.03   3  888  404
0.02   2  940    2
0.04   3  992  424
0.02   2 1044    9
     \endverbatim
     </li>
     <li> march_pl uses at most 4 nodes to solve these instances, but the
     number of nodes remains fairly constant. </li>
     <li> The number of r1 reductions goes up linearly, in a perfectly straight
     line (r1 = 52 * r + 4 where r is the number of rounds). What are these
     52 variables? </li>
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
   t nds  r1 r2
0.00   2  24  5
0.00   2  44  4
0.00   3  64 31
0.00   3  84 26
0.00   4 104 82
0.00   3 124 29
0.00   4 145 39
0.00   4 165 29
0.01   3 184 28
0.00   4 204 59
0.01   4 224 56
0.00   3 244 54
0.01   3 264 72
0.01   3 284 82
0.00   3 304 52
0.02   5 325 78
0.01   3 344 72
0.01   4 365 57
0.01   3 384 73
0.01   4 405 59
     \endverbatim
     </li>
     <li> march_pl uses at most 5 nodes to solve these instances, but the
     number of nodes remains fairly constant. </li>
     <li> Note that this is similar to the number of nodes used for the canonical
     translation. </li>
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
