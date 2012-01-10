// Matthew Gwynne, 25.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
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
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/Cipher.mac.
   </li>
   <li> The translation of one round DES to SAT is given at the Maxima level by
   des_fcl_gen in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/GeneralisedConstraintTranslation.mac.
   </li>
   <li> The DES consists of certain rewiring of the bits, additions (XOR) and
   the application of 8 S-boxes (substitution boxes) for each round. </li>
   <li> We consider the DES S-boxes as 6x4 boolean functions,
   given by des_sbox_bf in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/Sboxes.mac.
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
   All such translations apply to both the 6x4 S-box functions and
   the 4 decomposed 6x1 functions.
   </li>
   <li> For initial experiments we use the Argosat-desgen plaintext-ciphertext
   pairs. See "Transferring the Argosat-desgen example" in
   Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/KnownKeyBits.hpp.
   </li>
   <li> Over one plaintext-ciphertext pair, using the:
    <ul>
     <li> 1-base translation; fastest solver solves in 553s.
     See "Using the 1-base translation for the S-boxes (6-to-4)". </li>
     <li> canonical translation; fastest solver solves in 567s.
     See "Using the canonical translation for the S-boxes (6-to-4)". </li>
     <li> "minimum" translation; fastest solver solves in 4 hours.
     See 'Using the "minimum"  translation for the S-boxes (6-to-4)'. </li>
    </ul>
   </li>
   <li> Over 20 plaintext-ciphertext pair, using the:
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
   <li> Generating plaintext and ciphertext pairs and instances:
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
   <li> On a single plaintext-ciphertext pair:
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
     <li> precosat-570.1 is still running after 13 hours XXX. </li>
    </ul>
   </li>
   </li>
   <li> Looking at 20 random plaintext-ciphertext pairs:
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
   <li> On a single plaintext-ciphertext pair:
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
   <li> Looking at 20 random plaintext-ciphertext pairs:
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
   <li> Using a single plaintext-ciphertext pair:
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
     <li> cryptominisat is still running after 13 hours XXX. </li>
     <li> OKsolver_2002 and precosat-570.1 still running after ~3.5 days XXX.
     </li>
    </ul>
   </li>
   <li> Looking at 20 random plaintext-ciphertext pairs:
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
     yields, for the first random plaintext-ciphertext pair:
     \verbatim
restarts              : 301052
conflicts             : 264442847      (9446074192 /sec)
decisions             : 510119607      (0.00 % random) (18221811288 /sec)
propagations          : 94246752051    (3366556601215 /sec)
CPU time              : 154031.16 s
     \endverbatim
     taking ~1.8 days.
     </li>
     <li> For the second random plaintext-ciphtext pair (seed = 2),
     minisat-2.2.0 takes > 1 week. </li>
    </ul>
   </li>
  </ul>

*/
