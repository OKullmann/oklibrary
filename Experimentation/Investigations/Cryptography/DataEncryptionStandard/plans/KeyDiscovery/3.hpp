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
   <li> Investigating translation of 3-round DES into SAT, see
   Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/general.hpp.
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


  \todo Using the canonical translation for the S-boxes
  <ul>
   <li> Translating the DES Sboxes using the canonical representation.
   That is, each Sbox is represented with the canonical representation
   given by dualts_fcl in
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
    sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed),
    Fs[1],
    sconcat("des_argocomp_r",rounds,"_s",seed,".cnf"),
    Fs[2]))$
print("DONE!");
   \endverbatim
   </li>
   <li> Running minisat-2.2.0 on these instances:
   \verbatim
shell> r=3;
shell> for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}; Random Seed ${s}...";
    cat des_argocomp_r${r}_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
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
    cat des_argocomp_r${r}_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
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


  \todo Using the canonical CNF translation for the S-boxes
  <ul>
   <li> Translating the DES Sboxes using the canonical CNFs.
   That is, each Sbox is represented with a CNF where all
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
    sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed),
    Fs[1],
    sconcat("des_argocomp_r",rounds,"_s",seed,".cnf"),
    Fs[2]))$
print("DONE!");
   \endverbatim
   </li>
   <li> Running minisat-2.2.0 on these instances:
   \verbatim
shell> r=3;
shell> for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}; Random Seed ${s}...";
    cat des_argocomp_r${r}_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
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


  \todo Using the 1-base translation for the S-boxes
  <ul>
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
    sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed),
    Fs[1],
    sconcat("des_argocomp_r",rounds,"_s",seed,".cnf"),
    Fs[2]))$
print("DONE!");
   \endverbatim
   </li>
   <li> Running minisat-2.2.0 on these instances:
   \verbatim
shell> r=3;
shell> for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}; Random Seed ${s}...";
    cat des_argocomp_r${r}_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
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
    cat des_argocomp_r${r}_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
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


  \todo Using the "minimum" translation
  <ul>
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
    sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed),
    Fs[1],
    sconcat("des_argocomp_r",rounds,"_s",seed,".cnf"),
    Fs[2]))$
print("DONE!");
   \endverbatim
   </li>
   <li> Running minisat-2.2.0 on these instances:
   \verbatim
shell> r=3;
shell> for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}; Random Seed ${s}...";
    cat des_argocomp_r${r}_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
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
    cat des_argocomp_r${r}_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
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
