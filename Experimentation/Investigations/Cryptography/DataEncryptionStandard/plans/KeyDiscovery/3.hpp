// Matthew Gwynne, 16.6.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/3.hpp
  \brief On investigations into the three-round Data Encryption Standard key discovery


  \todo Overview
  <ul>
   <li> We consider the three round DES given by the encryption function
   des_encryption_gen in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/Cipher.mac.
   </li>
   <li> The translation of one round DES to SAT is given at the Maxima level by
   des_fcl_gen in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/GeneralisedConstraintTranslation.mac.
   </li>
   <li> The DES consists of certain rewiring of the bits, additions (XOR) and
   the application of 8 S-boxes (substitution boxes) for each round. </li>
   <li> We consider the DES S-boxes as 6-bit to 4-bit boolean functions,
   given by des_sbox_bf in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/Sboxes.mac.
   </li>
   <li> We should also consider the DES S-boxes as 4 6-bit to 1-bit functions.
   See "Basic translation" in
   Investigations/Cryptography/DataEncryptionStandard/plans/general.hpp. </li>
   <li> We translate the DES by treating the additions and S-boxes as the
   boolean functions, which we consider our units of translation. </li>
   <li> The additions are translated by the set of their prime implicates.
   </li>
   <li> The S-boxes are translated using each of the following CNF
   representations:
    <ul>
     <li> canonical(+) representation, see dualts_fcl and dualtsplus_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac;
     </li>
     <li> 1-base translations, see
     Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp;
     </li>
     <li> minimum translations, see
     Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp;
     </li>
     <li> their prime implicates; </li>
     <li> their canonical CNF representations. </li>
    </ul>
   All such translations apply to both the 6-bit to 4-bit S-box functions and
   the 4 decomposed 6-bit to 1-bit functions.
   </li>
   <li> Using the following translations for the DES S-boxes, solvers solve 20
   instances with random keys with the following number of conflicts/nodes on
   average:
    <ul>
     <li> canonical (minisat-2.2.0: 3987.84, OKsolver_2002: 90.17). </li>
     <li> canonical CNF (minisat-2.2.0: 5946.08, OKsolver_2002: ??). </li>
     <li> 1-base (minisat-2.2.0: 1397.14, OKsolver_2002: 13477.65). </li>
     <li> minimum (minisat-2.2.0: 3220.98, OKsolver_2002: 408094.4). </li>
    </ul>
   </li>
   <li> Note that we use the canonical CNF translation for the S-boxes to
   compare other representations to the "hardest" representation. </li>
  </ul>


  \todo Using the canonical translation for the S-boxes (6-to-4)
  <ul>
   <li> Translating the DES Sboxes, as 6-to-4-bit boolean functions, using the
   canonical representation. That is, each Sbox is represented with the
   canonical representation given by dualts_fcl in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
   </li>
   <li> Generating instances for 20 random plaintext-ciphertext pairs:
   \verbatim
shell> mkdir -p des_3/canon
shell> cd des_3/canon
shell> oklib --maxima
oklib_load_all()$
rounds : 3$
sbox_fcl_l : create_list(dualts_fcl([listify(setn(10)), des_sbox_fulldnf_cl(i)]), i, 1, 8)$
for seed : 1 thru 20 do block(
  print(sconcat("Generating ", rounds, "-round DES with seed ", seed)),
  set_random(make_random_state(seed)),
  P_hex : lpad(int2hex(random(2**64)),"0",16),
  K_hex : lpad(int2hex(random(2**64)),"0",16),
  C_hex : des_encryption_hex_gen(rounds, P_hex,K_hex),
  P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds),
  C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds),
  F : des2fcl_gen(sbox_fcl_l,rounds),
  Fs : standardise_fcl([F[1],append(F[2],P[2],C[2])]),
  output_fcs_v(
    sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed, "; translated using the canonical translation for the S-boxes (6-to-4)."),
    Fs[1],
    sconcat("des_6t4_canon_r",rounds,"_s",seed,".cnf"),
    Fs[2]))$
print("DONE!");
   \endverbatim
   </li>
   <li> Statistics:
   \verbatim
cat des_6t4_canon_r3.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
2000 18008 52160 0 52160 2003 1
 length count
1 128
2 15360
3 960
11 1536
64 24
   \endverbatim
   </li>
   <li> S-box statistics (canonical translation):
   \verbatim
ncl_list_fcl(dualts_fcl([listify(setn(10)), des_sbox_fulldnf_cl(1)]));
[[2,640],[11,64],[64,1]]
   \endverbatim
   </li>
   <li> We have the following number of clauses of the following sizes:
    <ul>
     <li> 128 unit-clauses (setting plaintext + ciphertext); </li>
     <li> 15360 binary clauses (8 * 3 = 24 S-boxes); </li>
     <li> 960 ternary clauses (80 * 3 = 240 binary additions); </li>
     <li> 1536 clauses of length eleven (8 * 3 = 24 S-boxes); </li>
     <li> 24 clauses of length 64 (8 * 3 = 24 S-boxes). </li>
    </ul>
   </li>
   <li> Running minisat-2.2.0 on these instances:
   \verbatim
shell> r=3;
shell> for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}; Random Seed ${s}...";
    cat des_6t4_canon_r${r}_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
    minisat-2.2.0 r${r}_k${k}_s${s}.cnf > minisat_r${r}_k${k}_s${s}.result 2>&1;
  done;
done;
shell> echo "n  c  t  sat  cfs dec rts r1 mem ptime stime cfl r k s" > minisat_results; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    OKP=~/Work/OKlibrary/OKplatform/; cat minisat_r${r}_k${k}_s${s}.result | awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk | awk " { print \$0 \"  $r  $k $s\" }";
  done;
done >> minisat_results;
   \endverbatim
   yields:
   \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r    n        c      t sat     cfs      dec rts       r1   mem ptime
1 3 2000 15978.37 0.2256   1 3987.84 21085.05  20 534094.7 20.34 2e-04
   stime    cfl r    k s
1 0.0081 407797 3 10.5 3
   \endverbatim
   </li>
   <li> Running OKsolver_2002 on these instances:
   \verbatim
shell> r=3;
shell> for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}; Random Seed ${s}...";
    cat des_6t4_canon_r${r}_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
    OKsolver_2002-O3-DNDEBUG r${r}_k${k}_s${s}.cnf > oksolver_r${r}_k${k}_s${s}.result 2>&1;
  done;
done;
shell> echo "n  c  l  t  sat  nds  r1  r2  pls  ats h file n2cr  dmcl dn  dc  dl snds qnds mnds  tel  oats  n2cs  m2cs r k s" > oksolver_results; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    OKP=~/Work/OKlibrary/OKplatform/; cat oksolver_r${r}_k${k}_s${s}.result | awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractOKsolver.awk | awk " { print \$0 \"  $r  $k $s\" }";
  done;
done >> oksolver_results;
   \endverbatim
   yields:
   \verbatim
shell> oklib --R
E = read.table("oksolver_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r    n     c     l     t sat   nds  r1       r2 pls  ats    h file  n2cr dmcl
1 3 1992 18008 52160 8.746   1 90.17 640 16790.46   0 0.05 8.37   NA 10752    0
   dn   dc    dl snds qnds mnds tel oats n2cs m2cs r    k s
1 640 5984 17536    0    0    0   0    0    0    0 3 10.5 3
   \endverbatim
   </li>
  </ul>


  \todo Using the canonical CNF translation for the S-boxes (6-to-4)
  <ul>
   <li> Translating the DES Sboxes, as 6-to-4 bit boolean functions, using the
   canonical CNFs. That is, each Sbox is represented with a CNF where all
   clauses are of length 10. </li>
   <li> Generating instances for 20 random plaintext-ciphertext pairs:
   \verbatim
rounds : 3$
sbox_fcl_l : create_list(fcs2fcl(des_sbox_fullcnf_fcs(i)), i, 1, 8)$
for seed : 1 thru 20 do block(
  print(sconcat("Generating ", rounds, "-round DES with seed ", seed)),
  set_random(make_random_state(seed)),
  P_hex : lpad(int2hex(random(2**64)),"0",16),
  K_hex : lpad(int2hex(random(2**64)),"0",16),
  C_hex : des_encryption_hex_gen(rounds, P_hex,K_hex),
  P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds),
  C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds),
  F : des2fcl_gen(sbox_fcl_l,rounds),
  Fs : standardise_fcl([F[1],append(F[2],P[2],C[2])]),
  output_fcs_v(
  sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed,"; translated using the canonical CNF translation for the S-boxes (6-to-4)."),
    Fs[1],
    sconcat("des_6t4_full_r",rounds,"_s",seed,".cnf"),
    Fs[2]))$
print("DONE!");
   \endverbatim
   </li>
   <li> Statistics:
   \verbatim
shell> cat des_6t4_full_r3.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
464 24128 233408 0 233408 467 1
 length count
1 128
3 960
10 23040
   \endverbatim
   </li>
   <li> S-box statistics (canonical CNF translation):
   \verbatim
ncl_list_fcl(des_sbox_fullcnf_fcs(1));
[[10,960]]
  \endverbatim
   </li>
   <li> We have the following number of clauses of the following sizes:
    <ul>
     <li> 128 unit-clauses (setting plaintext + ciphertext); </li>
     <li> 960 ternary clauses (80 * 3 = 240 binary additions); </li>
     <li> 23040 clauses of length ten (8 * 3 = 24 S-boxes); </li>
    </ul>
   </li>
   <li> Running minisat-2.2.0 on these instances:
   \verbatim
shell> r=3;
shell> for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}; Random Seed ${s}...";
    cat des_6t4_full_r${r}_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
    minisat-2.2.0 r${r}_k${k}_s${s}.cnf > minisat_r${r}_k${k}_s${s}.result 2>&1;
  done;
done;
shell> echo "n  c  t  sat  cfs dec rts r1 mem ptime stime cfl r k s" > minisat_results; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    OKP=~/Work/OKlibrary/OKplatform/; cat minisat_r${r}_k${k}_s${s}.result | awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk | awk " { print \$0 \"  $r  $k $s\" }";
  done;
done >> minisat_results;
   \endverbatim
   yields:
   \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r   n        c      t sat     cfs     dec   rts       r1 mem  ptime stime
1 3 464 21464.06 0.8104   1 5946.08 7782.91 28.03 129443.6  21 0.0114 0.711
       cfl r    k s
1 58269.88 3 10.5 3
   \endverbatim
   </li>
   <li> OKsolver_2002 still running. </li>
  </ul>


  \todo Using the 1-base translation for the S-boxes (6-to-4)
  <ul>
   <li> Translating the DES Sboxes, as 6-to-4 bit boolean functions, using
   1-bases. </li>
   <li> Generating the 1-bases:
   \verbatim
maxima> for i : 1 thru 8 do output_dessbox_fullcnf_stdname(i)$
shell> gen_seed[1]=7;gen_seed[2]=71;gen_seed[3]=185;gen_seed[4]=346;gen_seed[5]=67;gen_seed[6]=327;gen_seed[7]=148;gen_seed[8]=167;
shell> base_seed[1]=1;base_seed[2]=1;base_seed[3]=2;base_seed[4]=4;base_seed[5]=2;base_seed[6]=1;base_seed[7]=2;base_seed[8]=1;
shell> for i in $(seq 1 8); do
  QuineMcCluskey-n16-O3-DNDEBUG DES_Sbox_${i}_fullCNF.cnf > DES_Sbox_${i}_pi.cnf;
  RandomShuffleDimacs-O3-DNDEBUG ${gen_seed[$i]} < DES_Sbox_${i}_pi.cnf | SortByClauseLength-O3-DNDEBUG > DES_Sbox_${i}_sortedpi.cnf;
  RUcpGen-O3-DNDEBUG DES_Sbox_${i}_sortedpi.cnf > DES_Sbox_${i}_gen.cnf;
  RandomShuffleDimacs-O3-DNDEBUG ${base_seed[$i]}  < DES_Sbox_${i}_gen.cnf | SortByClauseLengthDescending-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > DES_Sbox_${i}_1base.cnf;
done
   \endverbatim
   </li>
   <li> The numbers of clauses in the 1-bases are 124, 129, 138, 128, 134,
   136, 123, 152 respectively. </li>
   <li> All the 1-bases used have clauses of sizes 5 and 6, except Sbox 4
   which has clauses of size 5 and 6 as well as 2 of size 7. </li>
   <li> Generating the instance:
   \verbatim
rounds : 3$
sbox_fcl_l : create_list(read_fcl_f(sconcat("DES_Sbox_",i,"_1base.cnf")), i, 1, 8)$
for seed : 1 thru 20 do block(
  print(sconcat("Generating ", rounds, "-round DES with seed ", seed)),
  set_random(make_random_state(seed)),
  P_hex : lpad(int2hex(random(2**64)),"0",16),
  K_hex : lpad(int2hex(random(2**64)),"0",16),
  C_hex : des_encryption_hex_gen(rounds, P_hex,K_hex),
  P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds),
  C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds),
  F : des2fcl_gen(sbox_fcl_l,rounds),
  Fs : standardise_fcl([F[1],append(F[2],P[2],C[2])]),
  output_fcs_v(
  sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed, "; translated using the 1-base translation for the S-boxes (6-to-4)."),
    Fs[1],
    sconcat("des_6t4_1base_r",rounds,"_s",seed,".cnf"),
    Fs[2]))$
print("DONE!");
   \endverbatim
   </li>
   <li> Statistics:
   \verbatim
shell> cat des_6t4_1base_r3.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
464 4280 20339 0 20339 467 1
 length count
1 128
3 960
5 1824
6 1365
7 3
   \endverbatim
   </li>
   <li> S-box statistics (1-base translations):
   \verbatim
for F in sbox_fcl_l do print(ncl_list_fcl(F));
[[5,84],[6,39],[7,1]]
[[5,75],[6,54]]
[[5,76],[6,62]]
[[5,69],[6,59]]
[[5,78],[6,56]]
[[5,83],[6,53]]
[[5,75],[6,48]]
[[5,68],[6,84]]
  \endverbatim
   </li>
   <li> We have the following number of clauses of the following sizes:
    <ul>
     <li> 128 unit-clauses (setting plaintext + ciphertext); </li>
     <li> 960 ternary clauses (80 * 3 = 24 binary additions); </li>
     <li> 1824 clauses of length five (8 * 3 = 24 S-boxes); </li>
     <li> 1365 clauses of length six (8 * 3 = 24 S-boxes); </li>
     <li> 3 clauses of length seven (1 * 3 = 3 S-boxes). </li>
    </ul>
   </li>
   <li> Running minisat-2.2.0 on these instances:
   \verbatim
shell> r=3;
shell> for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}; Random Seed ${s}...";
    cat des_6t4_1base_r${r}_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
    minisat-2.2.0 r${r}_k${k}_s${s}.cnf > minisat_r${r}_k${k}_s${s}.result 2>&1;
  done;
done;
shell> echo "n  c  t  sat  cfs dec rts r1 mem ptime stime cfl r k s" > minisat_results; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    OKP=~/Work/OKlibrary/OKplatform/; cat minisat_r${r}_k${k}_s${s}.result | awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk | awk " { print \$0 \"  $r  $k $s\" }";
  done;
done >> minisat_results;
   \endverbatim
   yields:
   \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r   n       c      t sat     cfs     dec  rts       r1 mem ptime  stime
1 3 464 3695.79 0.0204   1 1397.14 1709.12 9.28 51398.62  19     0 0.0062
       cfl r    k s
1 13416.54 3 10.5 3
   \endverbatim
   </li>
   <li> Running OKsolver_2002 on these instances:
   \verbatim
shell> r=3;
shell> for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}; Random Seed ${s}...";
    cat des_6t4_1base_r${r}_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
    OKsolver_2002-O3-DNDEBUG r${r}_k${k}_s${s}.cnf > oksolver_r${r}_k${k}_s${s}.result 2>&1;
  done;
done;
shell> echo "n  c  l  t  sat  nds  r1  r2  pls  ats h file n2cr  dmcl dn  dc  dl snds qnds mnds  tel  oats  n2cs  m2cs r k s" > oksolver_results; for s in $(seq 1 2); do
  for k in $(seq 1 20); do
    OKP=~/Work/OKlibrary/OKplatform/; cat oksolver_r${r}_k${k}_s${s}.result | awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractOKsolver.awk | awk " { print \$0 \"  $r  $k $s\" }";
  done;
done >> oksolver_results;
   \endverbatim
   yields:
   \verbatim
shell> oklib --R
E = read.table("oksolver_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r   n    c     l      t sat      nds    r1      r2 pls ats     h file  n2cr
1 3 456 4280 20339 9.2325   1 13477.65 162.3 27326.2   0 0.1 18.45   NA 351.5
  dmcl    dn     dc     dl  snds qnds mnds tel oats n2cs m2cs r    k   s
1    0 162.3 1402.8 6723.5 0.925    0 3.35   0    0    0    0 3 10.5 1.5
   \endverbatim
   </li>
  </ul>


  \todo Using the "minimum" translation for the S-boxes (6-to-4)
  <ul>
   <li> Translating the DES Sboxes, as 6-to-4 bit boolean functions, using the
   "minimum" (inf-based) representations. </li>
   <li> Generating the "minimum" CNFs for the Sboxes:
   \verbatim
maxima> for i : 1 thru 8 do output_dessbox_fullcnf_stdname(i)$
shell> for i in $(seq 1 8); do
  QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG DES_Sbox_${i}_fullCNF.cnf > DES_Sbox_${i}_shg.cnf;
  cat DES_Sbox_${i}_shg.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > DES_Sbox_${i}_shg.wcnf;
done
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 67 -solve 1 -seed 2444475534 -i DES_Sbox_1_shg.wcnf -r model DES_Sbox_1.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 67 -solve 1 -seed 2521057446 -i DES_Sbox_2_shg.wcnf -r model DES_Sbox_2.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 68 -solve 1 -seed 3544367510 -i DES_Sbox_3_shg.wcnf -r model DES_Sbox_3.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 69 -solve 1 -seed 3808694681 -i DES_Sbox_4_shg.wcnf -r model DES_Sbox_4.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 67 -solve 1 -seed 1876503362 -i DES_Sbox_5_shg.wcnf -r model DES_Sbox_5.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 66 -solve 1 -seed 68018538 -i DES_Sbox_6_shg.wcnf -r model DES_Sbox_6.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 67 -solve 1 -seed 1856244582 -i DES_Sbox_7_shg.wcnf -r model DES_Sbox_7.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 69 -solve 1 -seed 4223500633 -i DES_Sbox_8_shg.wcnf -r model DES_Sbox_8.ass;
shell> for i in $(seq 1 8); do
  cat DES_Sbox_${i}_fullCNF.cnf_primes | FilterDimacs DES_Sbox_${i}.ass > DES_Sbox_${i}_min.cnf;
done
   \endverbatim
   </li>
   <li> The numbers of clauses in the CNFs are 67, 67, 68, 69, 67, 66, 67, and
   69 respectively. </li>
   <li> Generating the instance:
   \verbatim
rounds : 3$
sbox_fcl_l : create_list(read_fcl_f(sconcat("DES_Sbox_",i,"_min.cnf")), i, 1, 8)$
for seed : 1 thru 20 do block(
  print(sconcat("Generating ", rounds, "-round DES with seed ", seed)),
  set_random(make_random_state(seed)),
  P_hex : lpad(int2hex(random(2**64)),"0",16),
  K_hex : lpad(int2hex(random(2**64)),"0",16),
  C_hex : des_encryption_hex_gen(rounds, P_hex,K_hex),
  P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds),
  C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds),
  F : des2fcl_gen(sbox_fcl_l,rounds),
  Fs : standardise_fcl([F[1],append(F[2],P[2],C[2])]),
  output_fcs_v(
  sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed, "; translated using the minimum translation for the S-boxes (6-to-4)."),
    Fs[1],
    sconcat("des_6t4_min_r",rounds,"_s",seed,".cnf"),
    Fs[2]))$
print("DONE!");
   \endverbatim
   </li>
   <li> Statistics:
   \verbatim
shell> cat des_6t4_min_r3.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
464 2708 12068 0 12068 467 1
 length count
1 128
3 960
5 696
6 888
7 36
   \endverbatim
   </li>
   <li> S-box statistics ("minimum" translation):
   \verbatim
for F in sbox_fcl_l do print(ncl_list_fcl(F));

[[5,30],[6,35],[7,2]]
[[5,33],[6,33],[7,1]]
[[5,28],[6,38],[7,2]]
[[5,29],[6,38],[7,2]]
[[5,29],[6,36],[7,2]]
[[5,28],[6,38]]
[[5,29],[6,37],[7,1]]
[[5,26],[6,41],[7,2]]
  \endverbatim
   </li>
   <li> We have the following number of clauses of the following sizes:
    <ul>
     <li> 128 unit-clauses (setting plaintext + ciphertext); </li>
     <li> 960 ternary clauses (80 * 3 = 240 binary additions); </li>
     <li> 696 clauses of length five (8 * 3 = 24 S-boxes); </li>
     <li> 888 clauses of length six (8 * 3 = 24 S-boxes); </li>
     <li> 36 clauses of length seven (7 * 3 = 21 S-boxes). </li>
    </ul>
   </li>
   <li> Running minisat-2.2.0 on these instances:
   \verbatim
shell> r=3;
shell> for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}; Random Seed ${s}...";
    cat des_6t4_min_r${r}_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
    minisat-2.2.0 r${r}_k${k}_s${s}.cnf > minisat_r${r}_k${k}_s${s}.result 2>&1;
  done;
done;
shell> echo "n  c  t  sat  cfs dec rts r1 mem ptime stime cfl r k s" > minisat_results; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    OKP=~/Work/OKlibrary/OKplatform/; cat minisat_r${r}_k${k}_s${s}.result | awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk | awk " { print \$0 \"  $r  $k $s\" }";
  done;
done >> minisat_results;
   \endverbatim
   yields:
   \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r   n       c      t sat     cfs     dec   rts       r1 mem ptime stime
1 3 464 2232.97 0.0251   1 3220.98 4117.38 17.28 97138.71  19     0     0
       cfl r    k s
1 30744.64 3 10.5 3
   \endverbatim
   </li>
   <li> Running OKsolver_2002 on these instances:
   \verbatim
shell> r=3;
shell> for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}; Random Seed ${s}...";
    cat des_6t4_min_r${r}_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
    OKsolver_2002-O3-DNDEBUG r${r}_k${k}_s${s}.cnf > oksolver_r${r}_k${k}_s${s}.result 2>&1;
  done;
done;
shell> echo "n  c  l  t  sat  nds  r1  r2  pls  ats h file n2cr  dmcl dn  dc  dl snds qnds mnds  tel  oats  n2cs  m2cs r k s" > oksolver_results; for s in $(seq 1 2); do
  for k in $(seq 1 20); do
    OKP=~/Work/OKlibrary/OKplatform/; cat oksolver_r${r}_k${k}_s${s}.result | awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractOKsolver.awk | awk " { print \$0 \"  $r  $k $s\" }";
  done;
done >> oksolver_results;
   \endverbatim
   yields:
   \verbatim
shell> oklib --R
E = read.table("oksolver_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r   n    c     l        t sat      nds     r1       r2 pls  ats     h file
1 3 456 2708 12068 161.1525   1 408094.4 160.85 838335.8   0 0.05 25.15   NA
   n2cr dmcl     dn     dc     dl    snds qnds  mnds tel oats n2cs m2cs r    k
1 331.9    0 160.85 983.95 4308.3 185.875    0 327.9   0    0    0    0 3 10.5
    s
1 1.5
   \endverbatim
   </li>
  </ul>
   <li> OKsolver_2002 still running. </li>
  </ul>

*/
