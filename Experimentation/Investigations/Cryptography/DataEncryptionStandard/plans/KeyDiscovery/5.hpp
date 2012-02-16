// Matthew Gwynne, 25.5.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/5.hpp
  \brief On investigations into the 5-round Data Encryption Standard key discovery


  \todo Overview
  <ul>
   <li> We consider the 5 round DES given by the encryption function
   des_encryption_gen in
   Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/Cipher.mac.
   </li>
   <li> The translation of one round DES to SAT is given at the Maxima level by
   des_fcl_gen in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/GeneralisedConstraintTranslation.mac.
   </li>
   <li> The DES consists of certain rewiring of the bits, additions (XOR) and
   the application of 8 S-boxes (substitution boxes) for each round. </li>
   <li> We consider the DES S-boxes as 6x4 boolean functions,
   given by des_sbox_bf in
   Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/Sboxes.mac.
   </li>
   <li> We should also consider the DES S-boxes as 4 6x1 functions.
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
     Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac;
     </li>
     <li> 1-base translations, see
     Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp;
     </li>
     <li> minimum translations, see
     Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp;
     </li>
     <li> their prime implicates; </li>
     <li> their canonical CNF representations. </li>
    </ul>
   All such translations apply to both the 6x4 S-box functions and
   the 4 decomposed 6x1 functions.
   </li>
   <li> For initial experiments we use the Argosat-desgen
   plaintext-ciphertext-pairs. See "Transferring the Argosat-desgen example" in
   Cryptography/DataEncryptionStandard/plans/KeyDiscovery/KnownKeyBits.hpp.
   </li>
   <li> Over one plaintext-ciphertext-pair, using the:
    <ul>
     <li> 1-base translation; fastest solver solves in 553s.
     See "Using the 1-base translation for the S-boxes (6-to-4)". </li>
     <li> canonical translation; fastest solver solves in 567s.
     See "Using the canonical translation for the S-boxes (6-to-4)". </li>
     <li> "minimum" translation; fastest solver solves in 4 hours.
     See 'Using the "minimum"  translation for the S-boxes (6-to-4)'. </li>
    </ul>
   </li>
   <li> Over 20 plaintext-ciphertext-pair, using the:
    <ul>
     <li> 1-base translation; minisat-2.2.0 solves in ~10.4 hours (avg),
     using ~ 427 million conflicts (avg).
     See "Using the 1-base translation for the S-boxes (6-to-4)". </li>
     <li> "minimum" translation; minisat-2.2.0 solves in ~5.1 hours (avg),
     using ~ 331 million conflicts (avg).
     See 'Using the "minimum"  translation for the S-boxes (6-to-4)'. </li>
    </ul>
   </li>
  </ul>


  \todo Using the Massacci DES translator
  <ul>
   <li> Generating plaintext-ciphertext-pairs and instances:
   \verbatim
> oklib --maxima
maxima> oklib_load_all()$

  format_massacci(hex) :=
    apply(sconcat,map(lambda([s],sconcat("0x",apply(sconcat,s),"_")),partition_elements(charlist(hex),2)))$

  rounds : 5$ with_stdout(sconcat("des_r",rounds,"_pkc_triples"),
    for seed : 1 thru 20 do block(
      set_random(make_random_state(seed)),
      P_hex : lpad(int2hex(random(2**64)),"0",16),
      K_hex : lpad(int2hex(random(2**64)),"0",16),
      C_hex : des_encryption_hex_gen(rounds, P_hex,K_hex),
      print(sconcat(
        format_massacci(P_hex), " ",
        format_massacci(K_hex)," ",
        format_massacci(C_hex)))))$
   \endverbatim
   </li>
   <li> Generating the instances:
   \verbatim
ExternalSources/builds/SAT/Des/des2fml-0.9> mkdir experiments && cd experiments
experiments> rounds=5; s=1;
cat des_r${rounds}_pkc_triples | grep -v '^ *$' | while read p k c; do
  echo ${p} | sed -e 's/_/ /g' > plaintxt; echo ${k} | sed -e 's/_/ /g' > key_des; rm ciph_txt
  ./des -r${rounds} && ./des2fml -r${rounds} -p -c -f1 && ./clausify formulae des_massacci_r${rounds}_s${s}.cnf
  let s=$s+1
done
   \endverbatim
   </li>
   <li> Running the solver:
   \verbatim
experiments> rounds=5;
for s in $(seq 1 20); do
  minisat-2.2.0 des_massacci_r${rounds}_s${s}.cnf > minisat_r${rounds}_s${s}.result;
done;
   \endverbatim
   </li>
   <li> Experiment running. </li>
   <li> Results (first 3 seeds):
   \verbatim
> rounds=5; results_file=r${rounds}_minisat.results; ExtractMinisat header-only | awk ' { print $0 " s" } ' > ${results_file};
for s in $(seq 1 3); do cat minisat_r${rounds}_s${s}.result | ExtractMinisat data-only | awk " { print  \$0 \" ${s}\" }" >> ${results_file}; done

> oklib --R
> E = read.table("r5_minisat.results", header=TRUE)
> summary(E)
      t              cfs                 dec                 rts               r1                ptime           stime           cfl
Min.   :43625   Min.   :299645316   Min.   :341323943   Min.   :344061   Min.   :9.754e+10   Min.   :0.000   Min.   :0.04   Min.   :8.804e+09
1st Qu.:44594   1st Qu.:301412592   1st Qu.:343384331   1st Qu.:346810   1st Qu.:9.821e+10   1st Qu.:0.005   1st Qu.:0.04   1st Qu.:8.847e+09
Median :45563   Median :303179868   Median :345444719   Median :349560   Median :9.889e+10   Median :0.010   Median :0.04   Median :8.891e+09
Mean   :50863   Mean   :332472341   Mean   :379011561   Mean   :384123   Mean   :1.082e+11   Mean   :0.010   Mean   :0.04   Mean   :9.757e+09
3rd Qu.:54482   3rd Qu.:348885854   3rd Qu.:397855370   3rd Qu.:404154   3rd Qu.:1.135e+11   3rd Qu.:0.015   3rd Qu.:0.04   3rd Qu.:1.023e+10
Max.   :63401   Max.   :394591840   Max.   :450266022   Max.   :458747   Max.   :1.281e+11   Max.   :0.020   Max.   :0.04   Max.   :1.158e+10


>  sd(E)
           t          cfs          dec          rts           r1        ptime        stime          cfl
1.090159e+04 5.382608e+07 6.174256e+07 6.468503e+04 1.729533e+10 1.000000e-02 0.000000e+00 1.576850e+09
   \endverbatim
   </li>
   <li> Seed 4 takes >73000s and > 460,042,881 conflicts. </li>
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
   <li> Statistics for all instances (see below):
   \verbatim
shell> cat des_6t4_1base_r5.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
688 7048 33813 0 33813 689 1
 length count
1 128
3 1600
5 3040
6 2275
7 5
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
     <li> 1600 ternary clauses (80 * 5 = 400 binary additions); </li>
     <li> 3040 clauses of length 5 (8 * 5 = 40 S-boxes); </li>
     <li> 2275 clauses of length 6 (8 * 5 = 40 S-boxes); </li>
     <li> 5 clauses of length seven (1 * 5 = 5 S-boxes). </li>
    </ul>
   </li>
   <li> On a single plaintext-ciphertext-pair:
    <ul>
     <li> Generating the instance:
     \verbatim
rounds : 5$
sbox_fcl_l : create_list(read_fcl_f(sconcat("DES_Sbox_",i,"_1base.cnf")), i, 1, 8)$
P_hex : "038E596D4841D03B"$
K_hex : "15FBC08D31B0D521"$
C_hex : des_encryption_hex_gen(rounds, "038E596D4841D03B","15FBC08D31B0D521")$
P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds)$
C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds)$
F : des2fcl_gen(sbox_fcl_l,rounds)$
Fs : standardise_fcl([F[1],append(F[2],P[2],C[2])])$
output_fc_v(
  sconcat("DES over ",rounds," rounds; translated using 1-base translations for the S-boxes (6-to-4)."),
  Fs[1],
  sconcat("des_6t4_1base_r",rounds,".cnf"),
  Fs[2])$
print("DONE!");
     \endverbatim
     </li>
     <li> Solvers (t:time,cfs:conflicts,nds:nodes): cryptominisat
     (t:553s,cfs:3198466), minisat-2.2.0 (t:13363s,cfs:183335114),
     OKsolver_2002 (t:36760s,nds:70636225). </li>
     <li> precosat236 solves in 298212s:
     \verbatim
c 692463380 conflicts, 762268518 decisions, 1 random
c 0 iterations, 2 restarts, 762102 skipped
c 298212.3 seconds, 91 MB max, 3183 MB recycled
     \endverbatim
     </li>
     <li> precosat-570.1 is still running after 13 hours XXX ??? long ago ???. </li>
    </ul>
   </li>
   </li>
   <li> Looking at 20 pseudo-random plaintext-ciphertext-pairs:
    <ul>
     <li> Generating the instance:
     \verbatim
rounds : 5$
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
  output_fcl_v(
  sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed, "; translated using the 1-base translation for the S-boxes (6-to-4)."),
    Fs[1],
    sconcat("des_6t4_1base_r",rounds,"_s",seed,".cnf"),
    Fs[2]))$
print("DONE!");
     \endverbatim
     </li>
     <li> Running minisat-2.2.0 on these instances:
     \verbatim
shell> r=5;
shell> for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}";
    minisat-2.2.0 des_6t4_1base_r${r}_s${k}.cnf > minisat_r${r}_k${k}.result 2>&1;
  done;

shell> echo "rn  rc  t  sat  cfs dec rts r1 mem ptime stime cfl r k" > minisat_results;
for k in $(seq 1 10); do
    cat minisat_r${r}_k${k}.result | ExtractMinisat data-only | awk " { print \$0 \"  $r  $k\" }";
done >> minisat_results;
     \endverbatim
     yields:
     \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r   rn    rc        t sat       cfs       dec      rts          r1  mem  ptime
1 5 688 6920 37454.02   1 427013617 495917628 462141.3 36457618986 64.2 0.0065
  stime         cfl r    k
1  0.01 11897817864 5 10.5
     \endverbatim
     </li>
    </ul>
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
   <li> Statistics:
   \verbatim
shell> cat des_6t4_min_r5.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
688 4428 20028 0 20028 689 1
 length count
1 128
3 1600
5 1160
6 1480
7 60
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
     <li> 1600 ternary clauses (80 * 5 = 400 binary additions); </li>
     <li> 1160 clauses of length 5 (8 * 5 = 40 S-boxes); </li>
     <li> 1480 clauses of length 6 (8 * 5 = 40 S-boxes); </li>
     <li> 60 clauses of length seven (7 * 5 = 35 S-boxes). </li>
    </ul>
   </li>
   <li> On a single plaintext-ciphertext-pair:
    <ul>
     <li> Generating the instance:
     \verbatim
rounds : 5$
sbox_fcl_l : create_list(read_fcl_f(sconcat("DES_Sbox_",i,"_min.cnf")), i, 1, 8)$
P_hex : "038E596D4841D03B"$
K_hex : "15FBC08D31B0D521"$
C_hex : des_encryption_hex_gen(rounds, "038E596D4841D03B","15FBC08D31B0D521")$
P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds)$
C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds)$
F : des2fcl_gen(sbox_fcl_l,rounds)$
Fs : standardise_fcl([F[1],append(F[2],P[2],C[2])])$
output_fcl_v(
  sconcat("DES over ",rounds," rounds; translated using the minimum translation for the S-boxes (6-to-4)."),
  Fs[1],
  sconcat("des_6t4_min_r",rounds,".cnf"),
  Fs[2])$
print("DONE!");
     \endverbatim
     </li>
     <li> minisat-2.2.0 solves in 14,291s (~4 hours) using 258,451,462
     conflicts:
     \verbatim
shell> minisat-2.2.0 des_6t4_min_r5.cnf
restarts              : 294909
conflicts             : 258451462      (3953546 /sec)
decisions             : 308556223      (0.00 % random) (4720001 /sec)
propagations          : 20792733621    (318067586 /sec)
     \endverbatim
     </li>
     <li> cryptominisat solves in 520,000s (~6 days):
     \verbatim
shell> cryptominisat des_6t4_min_r5.cnf
c static restarts          : 629
c full restarts            : 8
c conflicts                : 247428989   (3780713.58 / sec)
c decisions                : 262651549   (0.19      % random)
     \endverbatim
     </li>
     <li> precosat236 solves in 167361s (~2 days):
     \verbatim
shell> precosat236 des_6t4_min_r5.cnf
c 611003123 conflicts, 674624629 decisions, 1 random
c 0 iterations, 3 restarts, 657401 skipped
c 167361.8 seconds, 51 MB max, 3661 MB recycled
     \endverbatim
     </li>
     <li> precosat-570.1 solves in 250789s (~3 days):
     \verbatim
shell> precosat-570.1 -v des_6t4_min_r5.cnf
c 129305771 conflicts, 138334129 decisions, 69735 random
c 0 iterations, 28 restarts, 1310689 skipped
c 250789.7 seconds, 176 MB max, 3937 MB recycled
     \endverbatim
     </li>
     <li> OKsolver doesn't solve in 550,000s (> 6 days). </li>
    </ul>
   </li>
   <li> Looking at 20 pseudo-random plaintext-ciphertext-pairs:
    <ul>
     <li> Generating the instance:
     \verbatim
rounds : 5$
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
  output_fcl_v(
  sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed, "; translated using the 1-base translation for the S-boxes (6-to-4)."),
    Fs[1],
    sconcat("des_6t4_min_r",rounds,"_s",seed,".cnf"),
    Fs[2]))$
print("DONE!");
     \endverbatim
     </li>
     <li> Running minisat-2.2.0 on these instances:
     \verbatim
shell> r=5;
shell> for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}";
    minisat-2.2.0 des_6t4_min_r${r}_s${k}.cnf > minisat_r${r}_k${k}.result 2>&1;
  done;
shell> echo "rn  rc  t  sat  cfs dec rts r1 mem ptime stime cfl r k" > minisat_results;
for k in $(seq 1 18) 20; do
    cat minisat_r${r}_k${k}.result | ExtractMinisat data-only | awk " { print \$0 \"  $r  $k\" }";
done >> minisat_results;
     \endverbatim
     yields (excluding seed=19, which is being rerun):
     \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r   n    c        t sat       cfs       dec      rts          r1      mem
1 5 688 4300 19386.51   1 348652824 413615499 394408.6 28356034481 45.84211
  ptime       stime        cfl r        k
1     0 0.003684211 9557061007 5 10.05263
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo Using the canonical translation for the S-boxes (6-to-4)
  <ul>
   <li> Translating the DES Sboxes, as 6x4 boolean functions, using the
   canonical representation. That is, each Sbox is represented with the
   canonical representation given by dualts_fcl in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
   </li>
   <li> Statistics:
   \verbatim
shell> cat des_6t4_canon_r5.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
3248 29928 86848 0 86848 3249 1
 length count
1 128
2 25600
3 1600
11 2560
64 40
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
     <li> 25600 binary clauses (8 * 5 = 40 S-boxes); </li>
     <li> 1600 ternary clauses (80 * 5 = 400 binary additions); </li>
     <li> 2560 clauses of length eleven (8 * 5 = 40 S-boxes); </li>
     <li> 40 clauses of length 64 (8 * 5 = 40 S-boxes). </li>
    </ul>
   </li>
   <li> Using a single plaintext-ciphertext-pair:
    <ul>
     <li> Generating the instance:
     \verbatim
rounds : 5$
sbox_fcl_l : create_list(dualts_fcl([listify(setn(10)), des_sbox_fulldnf_cl(i)]), i, 1, 8)$
P_hex : "038E596D4841D03B"$
K_hex : "15FBC08D31B0D521"$
C_hex : des_encryption_hex_gen(rounds, "038E596D4841D03B","15FBC08D31B0D521")$
P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds)$
C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds)$
F : des2fcl_gen(sbox_fcl_l,rounds)$
Fs : standardise_fcl([F[1],append(F[2],P[2],C[2])])$
output_fcl_v(
  sconcat("DES over ",rounds," rounds; translated using the canonical translation for the S-boxes (6-to-4)."),
  Fs[1],
  sconcat("des_6t4_canon_r",rounds,".cnf"),
  Fs[2])$
print("DONE!");
     \endverbatim
     </li>
     <li> precosat236 solves in 567s with 2,423,412 conflicts:
     \verbatim
c 2423412 conflicts, 4652299 decisions, 2044 random
c 0 iterations, 4094 restarts, 0 skipped
c 567.9 seconds, 57 MB max, 1479 MB recycled
     \endverbatim
     </li>
     <li> minisat-2.2.0 solves in 17221s using 40,018,619 conflicts:
     \verbatim
shell> minisat-2.2.0 des_6t4_canon_r5.cnf
restarts              : 56057
conflicts             : 40018619       (612167 /sec)
decisions             : 82938445       (0.00 % random) (1268714 /sec)
propagations          : 13510152814    (206665548 /sec)
conflict literals     : 7079980812     (48.30 % deleted)
     \endverbatim
     </li>
     <li> cryptominisat is still running after 13 hours XXX ??? long ago ???. </li>
     <li> OKsolver_2002 and precosat-570.1 still running after ~3.5 days XXX ??? long ago ???.
     </li>
    </ul>
   </li>
   <li> Looking at 20 pseudo-random plaintext-ciphertext-pairs:
    <ul>
     <li> Generating the instance:
     \verbatim
rounds : 5$
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
  output_fcl_v(
  sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed, "; translated using the 1-base translation for the S-boxes (6-to-4)."),
    Fs[1],
    sconcat("des_6t4_canon_r",rounds,"_s",seed,".cnf"),
    Fs[2]))$
print("DONE!");
     \endverbatim
     </li>
     <li> Running minisat-2.2.0 on these instances:
     \verbatim
shell> r=5;
shell> for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}";
    minisat-2.2.0 des_6t4_canon_r${r}_s${k}.cnf > minisat_r${r}_k${k}.result 2>&1;
  done;
     \endverbatim
     yields, for the first pseudo-random plaintext-ciphertext-pair:
     \verbatim
restarts              : 301052
conflicts             : 264442847      (9446074192 /sec)
decisions             : 510119607      (0.00 % random) (18221811288 /sec)
propagations          : 94246752051    (3366556601215 /sec)
CPU time              : 154031.16 s
     \endverbatim
     taking ~1.8 days.
     </li>
     <li> For the second pseudo-random plaintext-ciphertext-pair (seed = 2),
     minisat-2.2.0 takes > 1 week. </li>
    </ul>
   </li>
  </ul>


  \todo Applying SplittingViaOKsolver
  <ul>
   <li> Trying cube and conquer using SplittingViaOKsolver:
    <ol>
     <li> A dedicated effort is needed here, but it should be possible to
     make it our best weapon. </li>
     <li> See "More powerful processing options" in
     Interfaces/DistributedSolving/plans/ProcessSplitViaOKsolver.hpp. </li>
     <li> One important aspect is to locate the subinstance of the
     splitting containing the known (total) solution:
     \verbatim
# In splitting directory
> RandomDESTotalAssignment des_6t4_canon_r5_s1.cnf 1 5 > des_6t4_canon_r5_s1.ta
> for x in Instances/*; do PassExtends-O3-DNDEBUG des_6t4_canon_r5_s1.ta ${x}; if [[ $? == 0 ]]; then echo ${x}; fi; done
# Prints Instances/??? where ??? is the number of the instance with the satisfying assignment
     \endverbatim
     </li>
     <li> Then we run all solvers on these satisfiable subinstances and see
     which perform best. </li>
    </ol>
   </li>
   <li> Using the canonical translation:
    <ul>
     <li> Generating the instance:
     \verbatim
rounds : 5$
seed : 1$
sbox_fcl_l : create_list(dualts_fcl([listify(setn(10)), des_sbox_fulldnf_cl(i)]), i, 1, 8)$
set_random(make_random_state(seed)),
P_hex : lpad(int2hex(random(2**64)),"0",16),
K_hex : lpad(int2hex(random(2**64)),"0",16),
C_hex : des_encryption_hex_gen(rounds, P_hex,K_hex),
P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds),
C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds),
F : des2fcl_gen(sbox_fcl_l,rounds),
Fs : standardise_fcl([F[1],append(F[2],P[2],C[2])]),
output_fcl_v(
sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed, "; translated using the 1-base translation for the S-boxes (6-to-4)."),
  Fs[1],
  sconcat("des_6t4_canon_r",rounds,"_s",seed,".cnf"),
  Fs[2]))$
     \endverbatim
     </li>
     <li> Basic statistics:
     \verbatim
> cat des_6t4_canon_r5_s1.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG nz
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
   3248   29928   3240   3248   29928    86848     NA     NA   29928    86848  3249
 length   count
      1     128
      2   25600
      3    1600
     11    2560
     64      40
> cat des_6t4_canon_r5_s1.cnf | UnitClausePropagation-O3-DNDEBUG > des_6t4_canon_s1_ucp.cnf
> cat des_6t4_canon_s1_ucp.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG nz
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
   3248   29352   3112   3248   29352    84928     NA     NA   29352    84928  3254
 length   count
      2   26048
      3     704
     11    2560
     64      40
     \endverbatim
     </li>
     <li> Splitting the problem for seed=1, with D=500,600,700:
     \verbatim
# on csltok:
> SplittingViaOKsolver -D500 des_6t4_canon_r5_s1.cnf
> cd SplitViaOKsolver_D500des_6t4_canon_r5_s1cnf_2012-01-08-143508
> > cat Md5sum
0a28ef9978c86d76010af15008bda86a
> cat Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  629.0   633.0   641.0   644.4   647.0   712.0
> table(E$n)
629 630 633 637 641 643 647 657 661 665 669 673 701 702 705 706 708 709 712
484   2 486 368 126 472 480   4  36 136  72  96  16  16  32  16  14  16  14
> summary(E$d)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   8.00   11.00   12.00   12.29   13.00   17.00
> table(E$d)
  8   9  10  11  12  13  14  15  16  17
  7  74 266 588 747 579 345 196  68  16
> cat Result
c initial_maximal_clause_length         64
c initial_number_of_variables           3240
c initial_number_of_clauses             29928
c initial_number_of_literal_occurrences 86848
c number_of_initial_unit-eliminations   128
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           128
c reddiff_number_of_clauses             576
c reddiff_number_of_literal_occurrences 1920
c number_of_2-clauses_after_reduction   26048
c running_time(sec)                     1253.5
c number_of_nodes                       5771
c number_of_quasi_single_nodes          0
c number_of_2-reductions                120201
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        17
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c splitting_cases                       2886


# on cspcmg:
> SplittingViaOKsolver -D600 des_6t4_canon_r5_s1.cnf
> cd SplitViaOKsolver_D600des_6t4_canon_r5_s1cnf_2012-01-04-144308/
> cat Md5sum
bfbefbfe59b91532302dc81bce81357c
> cat Statistics
> E=read.table("Data")
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  728.0   742.0   779.0   828.4   915.0  1139.0
> table(E$n)
 728  729  730  731  732  733  734  735  736  737  738  739  740  741  742  743
1279 1328 1640 1417 1443  996  952 1025  774 1112  775  715  581  544  621  450
 744  745  746  747  748  749  750  751  752  753  754  755  756  757  758  759
 780  489  540  578  844  399  678  710  596  117  912  912  561  142  618  730
 760  761  762  763  764  765  766  767  768  769  770  771  772  773  774  775
 474  115   74  195  307  213  109  211  319  132  355  105  313  194  762  180
 776  777  778  779  780  781  782  783  784  785  786  787  788  789  790  791
 150  206  507  129  160  225  209  106  110  203  101   35   89  103  142   47
 792  793  794  795  796  797  798  799  800  801  802  803  804  805  806  807
  78   65   70   47   53   15   41   45   58   69   38   40   82  109   28   56
 808  809  810  811  812  813  814  815  816  817  818  819  820  821  822  823
  76  150   69   93   40  131   67   25  141  140  133   85  137  134   93   87
 824  825  826  827  828  829  830  831  832  833  834  835  836  837  838  839
  43  138  122   70  136  110  183   97  159   43  120   99   78   71   61   88
 840  841  842  843  844  845  846  847  848  849  850  851  852  853  854  855
  65   71   63   68   38   69   74   91   81   72   59   89  117   52   53  171
 856  857  858  859  860  861  862  863  864  865  866  867  868  869  870  871
 157   49   55  100  119   39   41   38   54   36   26   47   30   40   48   34
 872  873  874  875  876  877  878  879  880  881  882  883  884  885  886  887
  22   35   33   10   35    8    9   16   16    8    2   12   22   10   13    5
 888  889  890  891  892  893  894  895  896  897  898  899  900  901  902  903
   3   11   76   80   92  122  219  174  283  354  324  333  467  422  357  498
 904  905  906  907  908  909  910  911  912  913  914  915  916  917  918  919
 402  313  389  360  271  307  239  281  230  178  195  195  153  173  155  136
 920  921  922  923  924  925  926  927  928  929  930  931  932  933  934  935
 137  168   99  101  113   73   55   59   51  164  204  215  234  365  304  346
 936  937  938  939  940  941  942  943  944  945  946  947  948  949  950  951
 412  464  369  478  454  373  454  335  331  318  280  298  240  158  170  206
 952  953  954  955  956  957  958  959  960  961  962  963  964  965  966  967
 163  158  223  145  155  146  135   93  172  101   99   67   45   42   36   54
 968  969  970  971  972  973  974  975  976  977  978  979  980  981  982  983
  23   28   26   31   54   62   73   66   83   93   85   85  114  103   88  129
 984  985  986  987  988  989  990  991  992  993  994  995  996  997  998  999
  87   78   78  105   66  108   67   95   68   69   68   83   40   59   63   46
1000 1001 1002 1003 1004 1005 1006 1007 1008 1009 1010 1011 1012 1013 1014 1015
  44   36   25    8   16    9   21    6    8    6   12    5   20    5   17   16
1016 1017 1018 1019 1020 1021 1022 1023 1024 1025 1026 1027 1028 1029 1030 1031
  15   23   34   30   29   38   36   54   43   28   53   34   37   58   71   54
1032 1033 1034 1035 1036 1037 1038 1039 1040 1041 1042 1043 1044 1045 1046 1047
  59   49   68   56   59   58   53   58   56   42   40   35   33   43   14   23
1048 1049 1050 1051 1052 1053 1054 1055 1056 1057 1058 1059 1060 1061 1062 1063
  14   21   17    7   13   13    3   20   19   15   22   19    8   18   23    4
1064 1065 1066 1067 1068 1069 1070 1071 1072 1073 1074 1075 1076 1077 1078 1079
  14   12   12   11   20    7   14    6   10    8   10   10   11    6    3   11
1080 1081 1082 1083 1084 1085 1086 1087 1092 1093 1096 1098 1099 1100 1101 1102
   2    2    2    5    1    2    1    3    3    3    1    1    3    7    6    6
1103 1104 1105 1106 1107 1108 1109 1110 1111 1112 1113 1114 1115 1116 1117 1118
   6   18   12    7   10    7    7    7   15    4    9    7    4    2    4    4
1119 1120 1121 1122 1123 1124 1125 1126 1127 1128 1129 1130 1131 1132 1133 1134
   5    4    3    3    4    3    1    1    6    7    5    3    5    4    1    3
1136 1139
   2    1
> cat Result
c running_time(sec)                     17756.8
c number_of_nodes                       121199
c number_of_quasi_single_nodes          0
c number_of_2-reductions                2285546
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        22
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c splitting_cases                       60600

> RandomDESTotalAssignment des_6t4_canon_r5_s1.cnf 1 5 > des_6t4_canon_r5_s1.ta
> for x in Instances/*; do PassExtends-O3-DNDEBUG des_6t4_canon_r5_s1.ta ${x}; if [[ $? == 0 ]]; then echo ${x}; fi; done
Instances/32129
> cat des_6t4_canon_r5_s1.cnf | ApplyPass-O3-DNDEBUG Instances/32129 des_6t4_canon_r5_s1_p32129_sat.cnf
> minisat-2.2.0 des_6t4_canon_r5_s1_p32129_sat.cnf
restarts              : 318
conflicts             : 120874         (5694 /sec)
decisions             : 379979         (0.00 % random) (17898 /sec)
propagations          : 34967923       (1647100 /sec)
conflict literals     : 15878718       (34.51 % deleted)
CPU time              : 21.23 s
SATISFIABLE

> ProcessSplitViaOKsolver SplitViaOKsolver_D600des_6t4_canon_r5_s1cnf_2012-01-04-144308
# intermediate result:
> E=read_processsplit_minisat()
23104: 6.543d, sum-cfs=1.746500e+09, mean-t=24.470s, mean-cfs=75593
$t:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
    0.18     1.32     2.14    24.47     2.88 17780.00
sd= 458.2902
       95%        96%        97%        98%        99%       100%
    4.0685     4.2600     4.5491     5.0000     6.2300 17777.2000
sum= 565348.2
$cfs:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
    3358    13440    18180    75590    22010 41700000
sd= 1164417
        95%         96%         97%         98%         99%        100%
   28982.40    29965.76    31253.64    33208.92    40357.79 41695626.00
sum= 1746500429
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -5.0955e+00  3.3789e-01  -15.08 < 2.2e-16 ***
E$cfs        3.9111e-04  2.8958e-07 1350.62 < 2.2e-16 ***
R-squared: 0.9875


# on csltok:
> SplittingViaOKsolver -D700 des_6t4_canon_r5_s1.cnf
> cd SplitViaOKsolver_D700des_6t4_canon_r5_s1cnf_2012-01-13-095226
> > cat Md5sum
4ca0db7f8b7361a498cd31321cd57dcb
> cat Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  828.0   876.0   927.0   924.2   969.0  1634.0
> table(E$n)
 828  829  830  831  832  833  834  835  836  837  838  839  840  841  842  843
1187 1273 1097 1302 1872  934  780 1349 1704  841 1050 1177 1230  497  959  883
 844  845  846  847  848  849  850  851  852  853  854  855  856  857  858  859
 926  693 1159  632 1201  609 1178  807 1465  626  762  784 1139  529  485  663
 860  861  862  863  864  865  866  867  868  869  870  871  872  873  874  875
 856  489  584  651  962  547  428  694 1023  531  414  440  389  321  269  388
 876  877  878  879  880  881  882  883  884  885  886  887  888  889  890  891
 301  239  237  337  334  397  194  302  343  430  322  364  326  258  410  311
 892  893  894  895  896  897  898  899  900  901  902  903  904  905  906  907
 450  437  706  406  697  976  968  766 1220 1300 1133 1316 1358 1181 1447 1336
 908  909  910  911  912  913  914  915  916  917  918  919  920  921  922  923
1197 1307 1252 1373 1124 1164 1114 1248 1027  943 1232  937  860 1102  895  753
 924  925  926  927  928  929  930  931  932  933  934  935  936  937  938  939
 910  774  549  734  622  714  763  814  694 1234 1088 1188 1328 1585 1214 1618
 940  941  942  943  944  945  946  947  948  949  950  951  952  953  954  955
1533 1259 1570 1296 1343 1339 1347 1126 1114  900  914 1165  940  820 1163  903
 956  957  958  959  960  961  962  963  964  965  966  967  968  969  970  971
 860  932  758  523  827  691  494  504  619  531  535  609  968  932  973 1075
 972  973  974  975  976  977  978  979  980  981  982  983  984  985  986  987
1604 1506 1635 1794 1754 1746 1800 1885 1342 1342 1182 1267  740  832  659  651
 988  989  990  991  992  993  994  995  996  997  998  999 1000 1001 1002 1003
 367  628  427  389  310  362  351  314  322  222  228  180  220  149  191  144
1004 1005 1006 1007 1008 1009 1010 1011 1012 1013 1014 1015 1016 1017 1018 1019
 161  137  152  103  156  113  169  162  170  165  141  190  153  217  232  205
1020 1021 1022 1023 1024 1025 1026 1027 1028 1029 1030 1031 1032 1033 1034 1035
 231  264  250  273  306  277  302  286  322  273  279  285  292  170  237  186
1036 1037 1038 1039 1040 1041 1042 1043 1044 1045 1046 1047 1048 1049 1050 1051
 180  147  172  162  123  117  120  128  118  122   91  104   71  105   83   90
1052 1053 1054 1055 1056 1057 1058 1059 1060 1061 1062 1063 1064 1065 1066 1067
  78   94   62   82   84   79  108   73   51   69   69   33   62   55   53   20
1068 1069 1070 1071 1072 1073 1074 1075 1076 1077 1078 1079 1080 1081 1082 1083
  56   28   40   23   37   32   30   25   35   25   18   39   23   18   22   32
1084 1085 1086 1087 1088 1089 1090 1091 1092 1093 1094 1095 1096 1097 1098 1099
  22   13   18   17   11   18   14   22   12   11   25   23   16   16   41   32
1100 1101 1102 1103 1104 1105 1106 1107 1108 1109 1110 1111 1112 1113 1114 1115
  27   38   38   28   42   46   30   39   28   32   21   37   10   21   18   13
1116 1117 1118 1119 1120 1121 1122 1123 1124 1125 1126 1127 1128 1129 1130 1131
   9   15   12   19    9    7    9   11    6    9    8    8   11   11   16   18
1132 1133 1134 1135 1136 1137 1138 1139 1140 1141 1142 1143 1144 1145 1146 1147
  14    8   10    7   12    8    8    6    6    8    7    4    1    3    1    3
1148 1149 1150 1151 1152 1153 1156 1158 1159 1160 1161 1163 1165 1166 1167 1168
   3    4    2    2    2    3    1    3    1    3    4    1    2    1    1    4
1169 1170 1171 1175 1177 1183 1184 1188 1189 1191 1200 1202 1203 1223 1225 1226
   2    1    1    1    1    2    1    2    1    1    1    1    1    1    1    1
1228 1232 1248 1260 1261 1262 1263 1265 1267 1271 1365 1401 1408 1416 1425 1462
   1    1    1    1    2    1    2    1    1    1    1    1    1    1    1    1
1465 1554 1634
   1    1    1
> summary(E$d)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  11.00   17.00   18.00   18.34   19.00   25.00
> table(E$d)
   11    12    13    14    15    16    17    18    19    20    21    22    23
    7    37   244  1494  5860 15509 28164 36759 34949 23944 11410  3861  1076
   24    25
  191    26
> cat Result
c running_time(sec)                     76720.5
c number_of_nodes                       327063
c number_of_quasi_single_nodes          0
c number_of_2-reductions                8481477
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        25
c proportion_searched                   4.768372e-07
c proportion_single                     0.000000e+00
c total_proportion                      4.76837158203125e-07
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c splitting_cases                       163531


> RandomDESTotalAssignment des_6t4_canon_r5_s1.cnf 1 5 > des_6t4_canon_r5_s1.ta
> for x in Instances/*; do PassExtends-O3-DNDEBUG des_6t4_canon_r5_s1.ta ${x}; if [[ $? == 0 ]]; then echo ${x}; fi; done
Instances/86151
> cat des_6t4_canon_r5_s1.cnf | ApplyPass-O3-DNDEBUG Instances/86151 des_6t4_canon_r5_s1_p86151_sat.cnf
> minisat-2.2.0 des_6t4_canon_r5_s1_p86151_sat.cnf
restarts              : 180
conflicts             : 57180          (6696 /sec)
decisions             : 168413         (0.00 % random) (19720 /sec)
propagations          : 16407303       (1921230 /sec)
conflict literals     : 7743044        (32.99 % deleted)
CPU time              : 8.54 s
SATISFIABLE
> cat Data | grep " 86151"
144993 86151 844 17


> ProcessSplitViaOKsolver SplitViaOKsolver_D700des_6t4_canon_r5_s1cnf_2012-01-13-095226
# aborted:
> E=read_processsplit_minisat()
131102: 5.043d, sum-cfs=2.456681e+09, mean-t=3.324s, mean-cfs=18739
$t:
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
    0.017     1.340     2.040     3.324     3.105 16370.000
sd= 103.7205
        95%         96%         97%         98%         99%        100%
    4.78827     5.06619     5.45417     6.01209     7.01093 16369.10000
sum= 435729.8
$cfs:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
      76    11950    15610    18740    20540 28520000
sd= 188466.7
        95%         96%         97%         98%         99%        100%
   28995.00    30168.96    31728.97    34204.88    38984.87 28515965.00
sum= 2456681233
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -6.9665e+00  1.9018e-02 -366.32 < 2.2e-16 ***
E$cfs        5.4914e-04  1.0041e-07 5468.80 < 2.2e-16 ***
R-squared: 0.9956

> mean(E$t[seq(130000,131102)])
[1] 108.0599
> mean(E$t[seq(131000,131102)])
[1] 326.4455
     \endverbatim
     The running times exploded at the very end, and thus the computation was
     aborted. </li>
     <li> The position of the satisfying assignment in different orders for
     sub-instance i=86151 at D=700 (out of 163531 sub-instances):
      <ul>
       <li> using the default ordering, decreasing n, it is 144993-th;
       </li>
       <li> sorting first in increasing d, then increasing n, it is
       43119-th:
       \verbatim
> cat Data | sort -r -s | sort -k 4,4 -n -s | grep -n " 86151"
43119:144993 86151 844 17
       \endverbatim
       </li>
       <li> sorting first in increasing d, then in decreasing n, it is
       48291-th:
       \verbatim
# Sorting by the 4-th column (d) in numerical order using a stable sort
# Hence, by using a stable sort, we leave it sorted "second" by decreasing n.
> cat Data | sort -k 4,4 -n -s | grep -n " 86151"
48291:144993 86151 844 17
       \endverbatim
       </li>
      </ul>
     </li>
     <li> Using higher D-values likely takes too much time (for D=1000 the
     computation was aborted after having created 614822 subinstances), and
     makes the approach ineffective; the key is to get a good estimate how
     much time the sub-instance corresponding to the known (total) solution
     takes (and whether we can have a better prediction for the position of
     this sub-instance). </li>
    </ul>
   </li>
   <li> Using 1-bases:
    <ul>
     <li> Basic statistics:
     \verbatim
> cat des_6t4_1base_r5_s1.cnf | UnitClausePropagation-O3-DNDEBUG > des_6t4_1base_r5_s1_ucp.cnf
> cat des_6t4_1base_r5_s1_ucp.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG nz
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    688    6472    552    688    6472    31893     NA     NA    6472    31893   694
 length   count
      2     448
      3     704
      5    3040
      6    2275
      7       5
     \endverbatim
     </li>
     <li> Splitting the problem for seed=1, with D=70,80,90:
     \verbatim
> SplittingViaOKsolver -D50 des_6t4_1base_r5_s1.cnf
> cat Md5sum
f75fa9f264824ff74a5e72589b019246
> cat Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  179.0   180.0   180.0   180.4   181.2   183.0
> table(E$n)
 179  180  181  182  183
 896 2128   48  512  512
> summary(E$d)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  10.00   12.00   12.00   12.28   13.00   14.00
> table(E$d)
  10   11   12   13   14
  60  760 1548 1424  304
> cat Result
c initial_maximal_clause_length         7
c initial_number_of_variables           680
c initial_number_of_clauses             7048
c initial_number_of_literal_occurrences 33813
c number_of_initial_unit-eliminations   128
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           128
c reddiff_number_of_clauses             576
c reddiff_number_of_literal_occurrences 1920
c number_of_2-clauses_after_reduction   448
c running_time(sec)                     16.9
c number_of_nodes                       8191
c number_of_2-reductions                492
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        14
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c splitting_cases                       4096

> SplittingViaOKsolver -D90 des_6t4_1base_r5_s1.cnf
> cat Md5sum
e200c408e751b64d2d4775b5cd58dd25
> cat Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  218.0   220.0   223.0   223.7   226.0   246.0
> table(E$n)
  218   219   220   221   222   223   224   225   226   227   228   229   230
 9681 11745 17033 10698 12669 21747 14554  6294  5045  7121  6846  2839  1519
  231   232   233   234   235   236   237   238   239   240   241   242   243
 1732  2232  2413  1865  1020   859  1298  1020   216    44   173   147     1
  244   246
    6     4
> summary(E$d)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  12.00   17.00   18.00   17.78   19.00   23.00
> table(E$d)
   12    13    14    15    16    17    18    19    20    21    22    23
    2    58  1002  5741 18441 34049 39796 26273 11986  3074   393     6
> cat Result
c running_time(sec)                     563.4
c number_of_nodes                       281641
c number_of_quasi_single_nodes          0
c number_of_2-reductions                75804
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        23
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c splitting_cases                       140821
     \endverbatim
     With this data one sees clearly that the des-instances are not right:
     SplittingViaOKsolver does not take the initial ucp into account (which
     is appropriate), while the statistics have to take them into account!
     So for the above n-values one has to substract 128. </li>
     <li> Simple processing:
     \verbatim
> ProcessSplitViaOKsolver SplitViaOKsolver_D90des_6t4_1base_r5_s1cnf_2012-01-20-020203
> E=read_processsplit_minisat()
49798: 3.166d, sum-cfs=6.438756e+09, mean-t=5.492s, mean-cfs=129297, sat: 0
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.026   0.175   0.262   5.492   1.227 184.500
sd= 14.40578
      95%       96%       97%       98%       99%      100%
 39.85055  46.09436  48.96923  51.95310  67.64488 184.54300
sum= 273499.5
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
    922    6906   10020  129300   37860 3724000
sd= 316459.7
      95%       96%       97%       98%       99%      100%
 902391.8 1034263.1 1093248.0 1154811.0 1484272.2 3724230.0
sum= 6438756348
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -3.8557e-01  3.6566e-03 -105.44 < 2.2e-16 ***
E$cfs        4.5459e-05  1.0696e-08 4249.94 < 2.2e-16 ***
R-squared: 0.9973
     \endverbatim
     The pattern is a sequence of, say, 10 very easy sub-instances, followed
     by one rather hard subinstances. </li>
     <li> Locating the satisfiable sub-instances:
     \verbatim
# D=90
> RandomDESTotalAssignment des_6t4_1base_r5_s1.cnf 1 5 > Solution_1.pa
> for x in Instances/*; do PassClashes-O3-DNDEBUG Solution_1.pa ${x}; if [[ $? != 0 ]]; then echo ${x}; fi; done
Instances/102708
> cat des_6t4_1base_r5_s1.cnf | ApplyPass-O3-DNDEBUG Instances/102708 des_6t4_1base_r5_s1_102708.cnf
> minisat-2.2.0 des_6t4_1base_r5_s1_102708.cnf
conflicts             : 4243           (40804 /sec)
CPU time              : 0.103984 s

> solver="minisat-2.2.0 -cpu-lim=1" ProcessSplitViaOKsolver SplitViaOKsolver_D90des_6t4_1base_r5_s1cnf_2012-01-20-020203
XXX csltok
     \endverbatim
     The solution time is rather quick! The subinstance is actually easy for
     all our complete solvers; fastest seem precosat-570.1 and minisat-2.2.0.
     </li>
     <li> The bottleneck is that the satisfiable
     subinstance is located rather late (102708 from 140821 in OKsolver-order,
     127716 from 140821 in current SplittingViaOKsolver-order). </li>
    </ul>
   </li>
  </ul>


  \todo Systematic investigation of SplittingViaOKsolver for 1-base-instances
  <ul>
   <li> The instances for the first investigations:
   \verbatim
> ls des_6t4_1base_r5_s*
des_6t4_1base_r5_s01.cnf  des_6t4_1base_r5_s05.cnf
des_6t4_1base_r5_s02.cnf  des_6t4_1base_r5_s06.cnf
des_6t4_1base_r5_s03.cnf  des_6t4_1base_r5_s07.cnf
des_6t4_1base_r5_s04.cnf  des_6t4_1base_r5_s08.cnf

des_6t4_1base_r5_s09.cnf  des_6t4_1base_r5_s13.cnf  des_6t4_1base_r5_s17.cnf
des_6t4_1base_r5_s10.cnf  des_6t4_1base_r5_s14.cnf  des_6t4_1base_r5_s18.cnf
des_6t4_1base_r5_s11.cnf  des_6t4_1base_r5_s15.cnf  des_6t4_1base_r5_s19.cnf
des_6t4_1base_r5_s12.cnf  des_6t4_1base_r5_s16.cnf  des_6t4_1base_r5_s20.cnf
   \endverbatim
   </li>
  </ul>

*/
