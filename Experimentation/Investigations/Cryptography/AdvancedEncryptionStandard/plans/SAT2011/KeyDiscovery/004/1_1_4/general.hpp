// Matthew Gwynne, 6.6.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/004/1_1_4/general.hpp
  \brief Investigations into small scale AES key discovery with one row, one column and 4-bit field elements


  \todo Overview
  <ul>
   <li> We consider the translation of the key discovery problem into SAT,
   for the small scale AES with one row, one column, using the 4-bit field
   size. </li>
   <li> We consider variants of this small scale AES cipher with 1 to 20
   rounds. </li>
   <li> The small scale AES variant is translated to a CNF using the
   translation methods in "Investigating dimensions" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Experimentation.hpp.
   </li>
   <li> The plaintext and ciphertext variables are then set, and the SAT
   SAT solver is run on this instance to deduce the key variables. </li>
   <li> Comparing the translations:
    <ol>
     <li> Canonical translation:
      <ul>
       <li> All solvers except satz215 solve all rounds up to 20 in less than
       a second with the canonical translation </li>
       <li> satz215 shows exponential behaviour for the time, nodes and r1
       reductions in the number of rounds!?!? See "satz". </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>


  \todo Translations
  <ul>
   <li> The following translations are considered in this %plans %file:
    <ul>
     <li> The canonical box translation. </li>
    </ul>
   </li>
   <li> For a full list of the possible translations, see
   "Investigating dimensions" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Experimentation.hpp.
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
seed : 1$
mc_tran : aes_mc_bidirectional$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  output_ss_random_pc_pair(
    seed,num_rounds,num_columns,num_rows,exp,final_round_b))$
exit();
shell> for r in $(seq 1 20); do
  AppendDimacs-O3-DNDEBUG ssaes_r${r}_c1_rw1_e4_f0.cnf ssaes_pcpair_r${r}_c1_rw1_e4_f0_s1.cnf > r${r}_keyfind.cnf;
done
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo Minisat-2.2.0
  <ul>
   <li> Solving for rounds 1 to 20 with the canonical box translation:
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


  \todo precosat236
  <ul>
   <li> Solving for rounds 1 to 20 with the canonical box translation:
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


  \todo precosat-570.1
  <ul>
   <li> Solving for rounds 1 to 20 with the canonical box translation:
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


  \todo OKsolver_2002
  <ul>
   <li> Solving for rounds 1 to 20 with the canonical box translation:
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
   <li> The number of r1 reductions climbs by exactly 2 each time. </li>
   <li> The number of r2 reductions climbs, but not in a perfect straight
   line. </li>
   <li> Also the number or r2 reductions drops to 15 at rounds 8, 12, 14, 17
   and 20. </li>
  </ul>


  \todo march_pl
  <ul>
   <li> Solving for rounds 1 to 20 with the canonical box translation:
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


  \todo satz
  <ul>
   <li> Solving for rounds 1 to 20 with the canonical box translation:
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

*/
