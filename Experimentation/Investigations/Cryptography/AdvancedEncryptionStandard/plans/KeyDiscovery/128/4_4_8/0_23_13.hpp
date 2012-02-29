// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/128/4_4_8/0_23_13.hpp
  \brief Investigations into AES key discovery for 1 round AES without MixColumns (0+2/3+1/3)


  \todo Show and explain sizes of minimum-translations
  <ul>
   <li> To show the "minimum-size" translation, its sizes need to be discussed
   everywhere. </li>
   <li> This must include explanations of the numbers of different boxes and
   their contributions to the overall size (explained, in each case). </li>
  </ul>


  \todo Problem specification
  <ul>
   <li> We investigate the 0 + 2/3 + 1/3 round AES with 4 row,
   4 column, using the 8-bit field size. </li>
   <li> We denote this AES instance by aes(0+2/3,4,4,8). </li>
   <li> aes(0+2/3,4,4,8) takes a 128-bit plaintext and 128-bit key and
   outputs a 128-bit ciphertext. </li>
   <li> The MixColumns is the identity in this instance . </li>
   <li> The final round - the only round - is "special"; we only
   perform 2/3 of the round (key addition + SubBytes). </li>
   <li> For the full specification of this AES instance, see
   "Problem specification" in
   Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/128/4_4_8/general.hpp.
   </li>
   <li> Note that we consider the canonical CNF translation, as
   this is an example of the "hardest" representation without
   new variables. See "Hardness of boolean function representations"
   in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
   <li> This instance has:
    <ul>
     <li> 1 special round (Key Addition, SubBytes, and ShiftRows).
     </li>
     <li> 16 Sboxes in the SubBytes operation (4 rows * 4 columns = 16). </li>
     <li> 256 additions within the round and key additions, coming from:
      <ul>
       <li> 256 additions from key additions
       (2 round keys * 128-bit additions = 256). </li>
      </ul>
     </li>
     <li> 4 Sboxes in the AES key schedule (4 rows). </li>
     <li> 128 additions in the key schedule:
     <ul>
      <li> 8 additions of arity three
      (1 row * 1 column * 8 bits = 8). </li>
      <li> 120 additions of arity two
      ((3 rows * 4 columns + 1 rows * 3 columns) * 8 bits = 120). </li>
     </ul>
     </li>
     <li> 8 bits for the constant in the key schedule. </li>
    </ul>
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translation of aes(0+2/3,4,4,8):
    <ul>
     <li> We treat S-boxes and additions as boxes. </li>
     <li> The S-box is considered as a 16x1-bit boolean function;
     translated using the canonical translation; see dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
     </li>
     <li> Additions (XORs) of arity k are considered bit-wise as (k+1)-bit
     boolean functions; translated using their 2^k prime implicates. </li>
    </ul>
   </li>
   <li> Generating instantiated key-discovery instances for key seeds 1 to 20:
    <ul>
     <li> Generating uninstantiated AES-instance for 0 + 2/3 + 1/3 round:
     \verbatim
num_rounds : 1;
num_rows : 4;
num_columns : 4;
exp : 8;
final_round_b : true;
box_tran : aes_ts_box;
mc_tran : aes_mc_bidirectional;
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran);
for seed : 1 thru 20 do
  output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);

shell> cat ssaes_r1_c4_rw4_e8_f1.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
5928 88636 260776 0 260776 5929 1
 length count
1 8
2 81920
3 1504
4 64
17 5120
256 20

shell> for seed in $(seq 1 20); do AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw4_e8_f1.cnf ssaes_pcpair_r1_c4_rw4_e8_f1_s${seed}.cnf > r1_k${seed}.cnf; done
     \endverbatim
     </li>
     <li> The number of clauses of each length in the uninstantiated
     translation before UCP (ssaes_r1_c4_rw4_e8_f1.cnf):
     \verbatim
maxima> ncl_list_ss(1,4,4,8,true,aes_ts_box,aes_mc_bidirectional);
[[1,8],[2,81920],[3,1504],[4,64],[17,5120],[256,20]]
maxima> ncl_list_ss_gen(1,4,4,8,ss_mixcolumns_matrix(2,8,4),[[2,'s2],[9,'s9],[16,'s16]],[],true,aes_mc_bidirectional);
[[1,8],[2,20*s2],[3,1504],[4,64],[9,20*s9],[16,20*s16]]
maxima> ncl_list_full_dualts(16,256);
[[2,4096],[17,256],[256,1]]
     \endverbatim
     comprised of:
      <ul>
       <li> 8 unit-clauses for the 8-bit constant in the key expansion. </li>
       <li> 81920 binary clauses, coming from 20 Sboxes
       (20 * 4096 = 81920). </li>
       <li> 1504 ternary clauses, coming from 376 additions of arity two
       (376 * 4 = 1504). </li>
       <li> 64 clauses of length four, coming from 8 additions of arity three
       (8 * 8 = 64). </li>
       <li> 5120 clauses of length seventeen, coming from 20 Sboxes
       (20 * 256 = 5120). </li>
       <li> 20 clauses of length 256, coming from from 20 Sboxes
       (20 * 1 = 20). </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> Solving for key seeds 1 to 20:
    <ul>
     <li> minisat-2.2.0 (all solve in < 6m, with <1million conflicts):
     \verbatim
> for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done
> (ExtractMinisat header-only |  awk " { print \$0 \" s\"}"; for s in $(seq 1 20); do
    cat ExperimentMinisat_r1_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${s}\"}";
done) > MinisatStatistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
> aggregate(E, by=list(sat=E$sat), FUN=mean)
  sat   rn    rc       t sat      cfs     dec    rts       r1   mem  ptime  stime      cfl    s
1   1 5928 88596 135.153   1 316141.5 1531617 727.05 54225724 260.8 0.0325 0.0895 75805240 10.5
R> summary(E)
       rn             rc              t               cfs              dec
 Min.   :5928   Min.   :88596   Min.   :  8.50   Min.   : 44778   Min.   : 475724
 1st Qu.:5928   1st Qu.:88596   1st Qu.: 49.25   1st Qu.:132036   1st Qu.: 841614
 Median :5928   Median :88596   Median :100.01   Median :253761   Median :1441747
 Mean   :5928   Mean   :88596   Mean   :135.15   Mean   :316141   Mean   :1531617
 3rd Qu.:5928   3rd Qu.:88596   3rd Qu.:209.46   3rd Qu.:414705   3rd Qu.:2078778
 Max.   :5928   Max.   :88596   Max.   :378.08   Max.   :850817   Max.   :3212359
      rts               r1                cfl
 Min.   : 127.0   Min.   :  7364895  Min.   : 11421272
 1st Qu.: 359.8   1st Qu.: 22478942  1st Qu.: 35039243
 Median : 589.5   Median : 42611416  Median : 61835458
 Mean   : 727.0   Mean   : 54225724  Mean   : 75805240
 3rd Qu.: 954.5   3rd Qu.: 70383331  3rd Qu.:104802310
 Max.   :1797.0   Max.   :155575015  Max.   :190427737
     \endverbatim
     </li>
     <li> oksolver_2002:
     \verbatim
> mkdir oksolver
> for s in $(seq 1 20); do OKsolver_2002-O3-DNDEBUG r1_k${k}.cnf > oksolver/r1_k${s}.result 2>&1; done
> (ExtractOKsolver header-only | awk " { print \$0 \" s\"}"; for s in $(seq 1 20); do
  cat oksolver/r1_k${s}.result | ExtractOKsolver | tail -n 1 | awk " { print \$0 \" ${s}\"}";
done) > OKsolverStatistics
R> E = read.table("OKsolverStatistics", header=TRUE)
R> aggregate(E, by=list(sat=E$sat), FUN=mean)
   n     c      l mcl       t    nds  r1       r2 pls ats     h
5928 88892 261032 256 1092.69 2269.3 264 388286.1   0   0 15.25
 n2cr dmcl  dn  dc   dl snds qnds mnds        pa         ps        tp tel
82432    0 264 808 2472 2.85    0  0.1 0.3913483 0.01715088 0.4084991   0
oats n2cs m2cs
   0    0    0
     \endverbatim
     </li>
    </ul>
   </li>
   <li> Solving for key seed = 1:
    <ul>
     <li> Most solvers solve the problem in < 6m. </li>
     <li> Using the OKsolver:
     \verbatim
shell> OKsolver_2002-O3-DNDEBUG -D15 -M r1_k1.cnf
c running_time(sec)                     362.9
c number_of_nodes                       646
c number_of_single_nodes                1
c number_of_2-reductions                120548
     \endverbatim
     </li>
     <li> Using precosat236:
     \verbatim
shell> precosat236 r1_k1.cnf
     \endverbatim
     returns the correct result in 5.2 seconds.
     </li>
     <li> Glucose finishes in 131.55 seconds:
     \verbatim
shell> precosat236 r1_k1.cnf
c restarts              : 721
c conflicts             : 402591         (3060 /sec)
c decisions             : 1562100        (1.58 % random) (11875 /sec)
c propagations          : 57405015       (436374 /sec)
c CPU time              : 131.55 s
     \endverbatim
     </li>
     <li> MiniSAT-2.2.0 finishes in 60s:
     \verbatim
shell> minisat-2.2.0 test_keyfind.cnf
<snip>
restarts              : 382
conflicts             : 143515         (2413 /sec)
decisions             : 801345         (0.00 % random) (13473 /sec)
propagations          : 25615653       (430660 /sec)
CPU time              : 59.48 s

shell> minisat2 r1_k1.cnf
restarts              : 13
conflicts             : 33963          (346 /sec)
decisions             : 214258         (1.39 % random) (2181 /sec)
propagations          : 5270395        (53637 /sec)
CPU time              : 98.26 s
    \endverbatim
    </li>
    <li> march_pl solves it in 19.92s:
    \verbatim
shell> march_pl r1_k1.cnf
c main():: nodeCount: 5
c main():: dead ends in main: 0
c main():: lookAheadCount: 133012
c main():: unitResolveCount: 5664
c main():: time=19.920000
     \endverbatim
     It apparently does this in only 5 nodes??!?
     </li>
     <li> Picosat solves it in 18 seconds:
     \verbatim
shell> picosat913 r1_k1.cnf
c 29 iterations
c 439 restarts
c 94325 conflicts
c 783798 decisions
c 35170202 propagations
c 17.7 seconds total run time
     \endverbatim
     </li>
     <li> survey_propagation doesn't converge:
     \verbatim
shell> survey_propagation r1_k1.cnf
<snip>
fixed 1 biased var (+49 ucp)
.:-)
<bias>:-nan
fixed 1 biased var (+17 ucp)
.:-)
<bias>:-nan
contradiction
     \endverbatim
     </li>
     <li> Grasp says "RESOURCES EXCEEDED":
     \verbatim
shell> sat-grasp r1_k1.cnf
Registering switches
	Done creating structures. Elapsed time: 0.12
	Done searching.... RESOURCES EXCEEDED. Elapsed time: 2256.6
     \endverbatim
     </li>
     <li> With "seed : 2$" and "seed : 3$", these runtimes and statistics
     seem stable. </li>
    </ul>
   </li>
   <li> DONE (merge_cnf.sh replaced with AppendDimacs)
   Finally we merge the assignment with the basic instance:
   \verbatim
shell> $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/merge_cnf.sh ssaes_r1_c4_rw4_e8_f1.cnf ssaes_pcpair_r1_c4_rw4_e8_f1_s1.cnf > r1_k1.cnf

> cat r1_k1.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
terminate called after throwing an instance of 'OKlib::InputOutput::ClauseInputError'
  what():  OKlib::InputOutput::StandardDIMACSInput::read_clauses:
  literal 5929 has variable index larger than the specified upper bound 5928
line 94566, column 2, total characters read 21231687
   \endverbatim
   </li>
  </ul>


  \todo Using the 1-base box translation
  <ul>
   <li> Translation of aes(0+2/3,4,4,8):
    <ul>
     <li> We treat S-boxes and additions as boxes. </li>
     <li> The S-box is considered as a 16x1-bit boolean function. </li>
     <li> Generating the smallest (so far) 1-base for the S-box (as of
     29/02/2012) from
     AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp:
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
     <li> Additions (XORs) of arity k are considered bit-wise as (k+1)-bit
     boolean functions; translated using their 2^k prime implicates. </li>
    </ul>
   </li>
   <li> Generating instantiated key-discovery instances for key seeds 1 to 20:
    <ul>
    \verbatim
set_hm(ss_sbox_rbase_cnfs, 8, read_fcl_f("AES_Sbox_base.cnf"));
num_rounds : 1;
num_rows : 4;
num_columns : 4;
exp : 8;
final_round_b : true;
box_tran : aes_rbase_box;
mc_tran : aes_mc_bidirectional;
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran);
for seed : 1 thru 20 do
  output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);

shell> cat ssaes_r1_c4_rw4_e8_f1.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG nz
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    808   89536    808    808   89536   606936     NA     NA   89536   606936   809
 length   count
      1       8     # 8-bit round constant.
      3    1504     # 356 additions of arity 2 (376 * 4 = 1504).
      4      64     # 8 additions of arity 3 (8 * 8 = 64).
      5      20     # 20 S-boxes.
      6   23740     # 20 S-boxes (20 * 1187 = 23740).
      7   54060     # 20 S-boxes (20 * 2703 = 54060).
      8   10060     # 20 S-boxes (20 * 503  = 10060).
      9      80     # 20 S-boxes (20 * 4    = 80).

# S-boxes make up 87960 of the clauses (98.2%).

shell> for seed in $(seq 1 20); do AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw4_e8_f1.cnf ssaes_pcpair_r1_c4_rw4_e8_f1_s${seed}.cnf > r1_k${seed}.cnf; done
     \endverbatim
     </li>
    </ul>
   </li>
   <li> Solving for key seeds 1 to 20:
    <ul>
     <li> minisat-2.2.0 (all solve in < 1m, with < 200,000 conflicts):
     \verbatim
> mkdir minisat22
> for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done
> (ExtractMinisat header-only |  awk " { print \$0 \" s\"}"; for s in $(seq 1 20); do
    cat ExperimentMinisat_r1_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${s}\"}";
done) > MinisatStatistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
> aggregate(E, by=list(sat=E$sat), FUN=mean)
 rn    rc       t sat      cfs      dec   rts      r1   mem  ptime stime     cfl
808 89496 18.5295   1 100209.8 109272.1 279.1 3570575 37.85 0.0345 2.757 1440958
R> summary(E)
      t               cfs              dec              rts              r1
Min.   : 3.260   Min.   :  5703   Min.   :  6613   Min.   : 30.0   Min.   : 180100
1st Qu.: 8.595   1st Qu.: 51497   1st Qu.: 56678   1st Qu.:166.2   1st Qu.:1667288
Median :19.450   Median :110232   Median :119672   Median :290.5   Median :3808369
Mean   :18.529   Mean   :100210   Mean   :109272   Mean   :279.1   Mean   :3570575
3rd Qu.:27.655   3rd Qu.:141995   3rd Qu.:155228   3rd Qu.:388.2   3rd Qu.:5182173
Max.   :36.350   Max.   :194696   Max.   :210509   Max.   :510.0   Max.   :7114841
    ptime            stime            cfl                s
Min.   :0.0200   Min.   :2.740   Min.   :  84767   Min.   : 1.00
1st Qu.:0.0300   1st Qu.:2.750   1st Qu.: 744394   1st Qu.: 5.75
Median :0.0400   Median :2.750   Median :1577062   Median :10.50
Mean   :0.0345   Mean   :2.757   Mean   :1440958   Mean   :10.50
3rd Qu.:0.0400   3rd Qu.:2.760   3rd Qu.:2044834   3rd Qu.:15.25
Max.   :0.0400   Max.   :2.800   Max.   :2792138   Max.   :20.00
     \endverbatim
     </li>
    </ul>
   </li>


  \todo Using the "minimum" box translation
  <ul>
   <li> Translation of aes(0+2/3,4,4,8):
    <ul>
     <li> We treat S-boxes and additions as boxes. </li>
     <li> The S-box is considered as a 16x1-bit boolean function. </li>
     <li> Generating the smallest S-box CNF (as of 29/02/2012) from
     AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp:
     \verbatim
maxima> output_rijnsbox_fullcnf_stdname();
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_Sbox_shg.cnf
shell> cat AES_Sbox_shg.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > AES_Sbox_shg.wcnf
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 40000000 -wtarget 294 -solve 1 -seed 3213901809 -i AES_Sbox_shg.wcnf -r model AES_Sbox_s294.ass;
shell> cat  AES_Sbox_full.cnf_primes | FilterDimacs AES_Sbox_s294.ass > AES_Sbox_s294.cnf
shell> cat AES_Sbox_s294.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG nz
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
     16     294     16     16     294     1939     NA     NA     294     1939     1
 length   count
      6     143
      7     127
      8      24
     \endverbatim
     </li>
     <li> Additions (XORs) of arity k are considered bit-wise as (k+1)-bit
     boolean functions; translated using their 2^k prime implicates. </li>
    </ul>
   </li>
   <li> Generating instantiated key-discovery instances for key seeds 1 to 20:
    <ul>
    \verbatim
set_hm(ss_sbox_cnfs, 8, read_fcl_f("AES_Sbox_s294.cnf"));
num_rounds : 1;
num_rows : 4;
num_columns : 4;
exp : 8;
final_round_b : true;
box_tran : aes_small_box;
mc_tran : aes_mc_bidirectional;
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran);
for seed : 1 thru 20 do
  output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);

shell> cat ssaes_r1_c4_rw4_e8_f1.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG nz
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    808    7456    808    808    7456    43556     NA     NA    7456    43556   809
 length   count
      1       8     # 8-bit round constant.
      3    1504     # 356 additions of arity 2 (376 * 4 = 1504).
      4      64     # 8 additions of arity 3 (8 * 8 = 64).
      6    2860     # 20 S-boxes (20 * 143 = 2860).
      7    2540     # 20 S-boxes (20 * 127 = 2540).
      8     480     # 20 S-boxes (20 * 24  = 480).

# S-boxes make up 5880 of the clauses (78.9%).

shell> for seed in $(seq 1 20); do AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw4_e8_f1.cnf ssaes_pcpair_r1_c4_rw4_e8_f1_s${seed}.cnf > r1_k${seed}.cnf; done
     \endverbatim
     </li>
    </ul>
   </li>
   <li> Solving for key seeds 1 to 20:
    <ul>
     <li> minisat-2.2.0 (all solve in < 2m, with < 5 million conflicts):
     \verbatim
> mkdir minisat22
> for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done
> (ExtractMinisat header-only |  awk " { print \$0 \" s\"}"; for s in $(seq 1 20); do
    cat ExperimentMinisat_r1_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${s}\"}";
done) > MinisatStatistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
> aggregate(E, by=list(sat=E$sat), FUN=mean)
 rn   rc      t     cfs     dec     rts       r1  mem ptime  stime        cfl
808 7416 16.425 1180878 1362256 2262.45 20572165 22.5     0 0.0185 1 15619589
R> summary(E)
      t               sat         cfs             dec               rts               r1
Min.   : 0.030   Min.   :1   Min.   :   1171 Min.   :   2035   Min.   :   7.0   Min.   :   17687
1st Qu.: 4.043   1st Qu.:1   1st Qu.: 367447 1st Qu.: 430745   1st Qu.: 866.5   1st Qu.: 5790918
Median :10.450   Median :1   Median : 865446 Median :1009142   Median :1859.5   Median :13935462
Mean   :16.425   Mean   :1   Mean   :1180878 Mean   :1362256   Mean   :2262.4   Mean   :20572165
3rd Qu.:20.517   3rd Qu.:1   3rd Qu.:1543638 3rd Qu.:1777449   3rd Qu.:3005.8   3rd Qu.:26393793
Max.   :84.270   Max.   :1   Max.   :5113383 Max.   :5820085   Max.   :8191.0   Max.   :97606508
    ptime       stime             cfl
Min.   :0   Min.   :0.0100   Min.   :   13866
1st Qu.:0   1st Qu.:0.0200   1st Qu.: 4841715
Median :0   Median :0.0200   Median :11288819
Mean   :0   Mean   :0.0185   Mean   :15619589
3rd Qu.:0   3rd Qu.:0.0200   3rd Qu.:20415258
Max.   :0   Max.   :0.0200   Max.   :68075622
     \endverbatim
     </li>
    </ul>
   </li>

*/
